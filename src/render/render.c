
#include "render.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "../../stb/stb_image.h"
#endif // STB_IMAGE_IMPLEMENTATION

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../stb/stb_image_write.h"
#endif // STB_IMAGE_WRITE_IMPLEMENTATION

const Color white = { 255, 255, 255, 255 };
const Color black = { 0, 0, 0, 255 };
const Color red = { 255, 0, 0, 255 };
const Color green = { 0, 255, 0, 255 };
const Color blue = { 0, 100, 255, 255 };

Image load_image(const char* path)
{
    Image img = { 0 };
    int _channels;
    img.pixels = stbi_load(path, &img.width, &img.height, &_channels, IMAGE_CHANNELS);
    return img;
}

// Create blank image
Image create_image(int width, int height)
{
    Image img = { 0 };
    img.width = width;
    img.height = height;
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

// Blend two colors
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

Grid new_grid(int width, int height)
{
    Grid grid = {0};
    grid.width = width;
    grid.height = height;
    grid.cells = (Cell*)calloc(width * height, sizeof(Cell));
    if (!grid.cells) {
        fprintf(stderr, "Failed to allocate memory for grid\n");
        exit(EXIT_FAILURE);
    }
    return grid;
}

Cell* get_cell(Grid* grid, int x, int y)
{
    if (x < 0 || x >= grid->width || y < 0 || y >= grid->height) {
        return NULL;
    }
    return &(grid->cells[y * grid->width + x]);
}

void set_cell(Grid* grid, int x, int y, Cell cell)
{
    if (x < 0 || x >= grid->width || y < 0 || y >= grid->height) {
        return;
    }
    grid->cells[y * grid->width + x] = cell;
}

void free_grid(Grid* grid)
{
    free(grid->cells);
}

// Render entire grid
void render_grid(Image* output, Image* tileset, Grid* grid)
{
    for (int y = 0; y < grid->height; y++) {
        for (int x = 0; x < grid->width; x++) {
            Cell* cell = &(grid->cells[y * grid->width + x]);
            draw_glyph(output, tileset, cell->ch, x, y, cell->fg, cell->bg);
        }
    }
}
