#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb/stb_image_write.h"
#include <stdint.h>
#include <stdio.h>

#define TILE_WIDTH 12
#define TILE_HEIGHT 12
#define TILESET_COLS 16
#define TILESET_ROWS 16

// Grid dimensions (in characters)
#define GRID_WIDTH 80
#define GRID_HEIGHT 50

typedef struct {
    uint8_t r, g, b, a;
} Color;

typedef struct {
    unsigned char ch;
    Color fg;
    Color bg;
} Cell;

typedef struct {
    uint8_t* pixels;
    int width;
    int height;
    int channels;
} Image;

// Load tileset image
Image load_image(const char* path)
{
    Image img = { 0 };
    img.pixels = stbi_load(path, &img.width, &img.height, &img.channels, 4);
    img.channels = 4; // Force RGBA
    return img;
}

// Create blank image
Image create_image(int width, int height)
{
    Image img = { 0 };
    img.width = width;
    img.height = height;
    img.channels = 4;
    img.pixels = (uint8_t*)calloc(width * height * 4, sizeof(uint8_t));
    return img;
}

// Get pixel from image
void get_pixel(Image* img, int x, int y, Color* out)
{
    if (x < 0 || x >= img->width || y < 0 || y >= img->height) {
        out->r = out->g = out->b = out->a = 0;
        return;
    }
    int idx = (y * img->width + x) * 4;
    out->r = img->pixels[idx + 0];
    out->g = img->pixels[idx + 1];
    out->b = img->pixels[idx + 2];
    out->a = img->pixels[idx + 3];
}

// Set pixel in image
void set_pixel(Image* img, int x, int y, Color color)
{
    if (x < 0 || x >= img->width || y < 0 || y >= img->height)
        return;
    int idx = (y * img->width + x) * 4;
    img->pixels[idx + 0] = color.r;
    img->pixels[idx + 1] = color.g;
    img->pixels[idx + 2] = color.b;
    img->pixels[idx + 3] = color.a;
}

// Blend two colors (simple alpha blending)
Color blend_colors(Color fg, Color bg)
{
    if (fg.a == 255)
        return fg;
    if (fg.a == 0)
        return bg;

    float alpha = fg.a / 255.0f;
    Color result;
    result.r = (uint8_t)(fg.r * alpha + bg.r * (1 - alpha));
    result.g = (uint8_t)(fg.g * alpha + bg.g * (1 - alpha));
    result.b = (uint8_t)(fg.b * alpha + bg.b * (1 - alpha));
    result.a = 255;
    return result;
}

// Apply color modulation to a pixel
Color modulate_color(Color pixel, Color mod)
{
    Color result;
    result.r = (pixel.r * mod.r) / 255;
    result.g = (pixel.g * mod.g) / 255;
    result.b = (pixel.b * mod.b) / 255;
    result.a = pixel.a;
    return result;
}

// Check if a color is magenta (transparency key)
int is_magenta(Color c)
{
    // Check if it's bright magenta (255, 0, 255) or close to it
    return (c.r > 250 && c.g < 5 && c.b > 250);
}

// Draw a glyph to the output image
void draw_glyph(Image* output, Image* tileset, unsigned char ch,
    int grid_x, int grid_y, Color fg, Color bg)
{
    // Calculate source position in tileset
    int src_x = (ch % TILESET_COLS) * TILE_WIDTH;
    int src_y = (ch / TILESET_COLS) * TILE_HEIGHT;

    // Calculate destination position in output
    int dst_x = grid_x * TILE_WIDTH;
    int dst_y = grid_y * TILE_HEIGHT;

    // First, fill with background color
    for (int py = 0; py < TILE_HEIGHT; py++) {
        for (int px = 0; px < TILE_WIDTH; px++) {
            set_pixel(output, dst_x + px, dst_y + py, bg);
        }
    }

    // Then draw the glyph on top
    for (int py = 0; py < TILE_HEIGHT; py++) {
        for (int px = 0; px < TILE_WIDTH; px++) {
            Color tile_pixel;
            get_pixel(tileset, src_x + px, src_y + py, &tile_pixel);

            // Skip magenta pixels (treat as transparent)
            if (is_magenta(tile_pixel)) {
                continue;
            }

            // Modulate tileset pixel with foreground color
            Color modulated = modulate_color(tile_pixel, fg);

            // Blend with background (in case tile has partial transparency)
            Color final = blend_colors(modulated, bg);

            set_pixel(output, dst_x + px, dst_y + py, final);
        }
    }
}

// Render entire grid
void render_grid(Image* output, Image* tileset, Cell grid[GRID_HEIGHT][GRID_WIDTH])
{
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            Cell* cell = &grid[y][x];
            draw_glyph(output, tileset, cell->ch, x, y, cell->fg, cell->bg);
        }
    }
}

// Create example scene
void create_example_scene(Cell grid[GRID_HEIGHT][GRID_WIDTH])
{
    Color white = { 255, 255, 255, 255 };
    Color black = { 0, 0, 0, 255 };
    Color red = { 255, 0, 0, 255 };
    Color green = { 0, 255, 0, 255 };
    Color blue = { 0, 100, 255, 255 };
    Color brown = { 200, 150, 100, 255 };

    // Fill with spaces
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            grid[y][x].ch = ' ';
            grid[y][x].fg = white;
            grid[y][x].bg = black;
        }
    }

    // Draw player
    grid[25][40].ch = '@';
    grid[25][40].fg = white;

    // Draw enemies
    grid[20][30].ch = 'g';
    grid[20][30].fg = green;

    grid[30][50].ch = 'D';
    grid[30][50].fg = red;

    // Draw terrain
    for (int x = 10; x < 30; x++) {
        grid[35][x].ch = '~';
        grid[35][x].fg = blue;
    }

    // Add title
    const char* title = "Dwarf Fortress Style ASCII Art";
    for (int i = 0; title[i] != '\0'; i++) {
        grid[2][25 + i].ch = title[i];
        grid[2][25 + i].fg = white;
    }
}

int main()
{
    // Load tileset
    printf("Loading tileset...\n");
    Image tileset = load_image("../bitmaps/DB_curses_12x12.bmp");
    if (!tileset.pixels) {
        printf("Failed to load tileset.bmp\n");
        return 1;
    }
    printf("Tileset loaded: %dx%d\n", tileset.width, tileset.height);

    // Create output image
    Image output = create_image(GRID_WIDTH * TILE_WIDTH, GRID_HEIGHT * TILE_HEIGHT);

    // Create and render scene
    Cell grid[GRID_HEIGHT][GRID_WIDTH];
    create_example_scene(grid);

    printf("Rendering...\n");
    render_grid(&output, &tileset, grid);

    // Save output
    printf("Saving output.png...\n");
    if (!stbi_write_png("output.png", output.width, output.height, 4,
            output.pixels, output.width * 4)) {
        printf("Failed to save output.png\n");
        return 1;
    }
    printf("Success! Image saved to output.png\n");

    // Cleanup
    stbi_image_free(tileset.pixels);
    free(output.pixels);

    return 0;
}
