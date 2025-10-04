
#include <worldgen/render.h>

// Grid dimensions (in characters)
#define GRID_WIDTH 80
#define GRID_HEIGHT 50

// path to tileset from root
#define TILESET_PATH "./bitmaps/DB_curses_12x12.bmp"

// Create example scene
void create_example_scene(Grid* grid)
{
    // Fill with spaces
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            Cell cell = {
                .ch = ' ',
                .fg = white,
                .bg = black
            };
            set_cell(grid, x, y, cell);
        }
    }

    // Draw player
    Cell player = {
        .ch = '@',
        .fg = white,
        .bg = black
    };
    set_cell(grid, 25, 40, player);

    // Draw enemies
    Cell enemy = {
        .ch = 'g',
        .fg = green,
        .bg = black
    };
    set_cell(grid, 20, 30, enemy);

    // Draw terrain
    Cell water = {
        .ch = '~',
        .fg = blue,
        .bg = black
    };
    for (int x = 10; x < 30; x++) {
        set_cell(grid, x, 35, water);
    }

    // Add title
    const char* title = "Dwarf Fortress Style ASCII Art";
    Cell title_cell = {
        .ch = ' ',
        .fg = white,
        .bg = black
    };
    for (int i = 0; title[i] != '\0'; i++) {
        title_cell.ch = title[i];
        set_cell(grid, 25 + i, 2, title_cell);
    }
}

int main()
{
    // Load tileset
    printf("Loading tileset...\n");
    Image tileset = load_image(TILESET_PATH);
    if (!tileset.pixels) {
        printf("Failed to load tileset.bmp\n");
        return 1;
    }
    printf("Tileset loaded: %dx%d\n", tileset.width, tileset.height);

    // Create output image
    Image output = create_image(GRID_WIDTH * TILE_WIDTH, GRID_HEIGHT * TILE_HEIGHT);

    // Create and render scene
    Grid grid = new_grid(GRID_HEIGHT, GRID_WIDTH);
    create_example_scene(&grid);

    printf("Rendering...\n");
    render_grid(&output, &tileset, &grid);

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
    free_grid(&grid);

    return 0;
}
