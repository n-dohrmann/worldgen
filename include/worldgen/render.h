
#ifndef RENDER_H
#define RENDER_H

#include <worldgen/include_paths.h>

#include STB_IMAGE_PATH_D
#include STB_IMAGE_WRITE_PATH_D
#include <stdint.h>
#include <stdio.h>

#define TILE_WIDTH 12
#define TILE_HEIGHT 12
#define TILESET_COLS 16
#define TILESET_ROWS 16

typedef struct
{
    uint8_t r, g, b, a;
} Color;

// common color definitions
extern const Color white;
extern const Color black;
extern const Color red;
extern const Color green;
extern const Color blue;

typedef struct
{
    unsigned char ch;
    Color fg;
    Color bg;
} Cell;

typedef struct
{
    Cell* cells;
    int width;
    int height;
} Grid;

typedef struct
{
    uint8_t* pixels;
    int width;
    int height;
#define IMAGE_CHANNELS 4 // rgba
} Image;

// loading in tilesets
Image load_image(const char* path);

// alloc new image
Image create_image(int width, int height);

// clean up
void free_image(Image* image);

// retrieve pixel data
void get_pixel(Image* img, int x, int y, Color* out);

// set pixel data
void set_pixel(Image* img, int x, int y, Color color);

// Blend two colors
Color blend_colors(Color fg, Color bg);

// Apply color modulation to a pixel
Color modulate_color(Color pixel, Color mod);

// Check if a color is magenta (transparency key)
int is_magenta(Color c);

// Draw a glyph to the output image
void draw_glyph(Image* output,
    Image* tileset,
    unsigned char ch,
    int grid_x,
    int grid_y,
    Color fg,
    Color bg);

// create new grid
Grid new_grid(int width, int height);

// get cell at position
Cell* get_cell(Grid* grid, int x, int y);

// set cell at position
void set_cell(Grid* grid, int x, int y, Cell cell);

// grid clean up
void free_grid(Grid* grid);

// render whole grid
void render_grid(Image* output, Image* tileset, Grid* grid);

#endif // RENDER_H
