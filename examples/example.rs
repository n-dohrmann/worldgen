use image::{Rgb, RgbImage};
use ab_glyph::{FontRef, Glyph, PxScale, point, Font};
use std::path::Path;

const CELL_SIZE: u32 = 32;

fn draw_ascii_art(matrix: &[Vec<(char, [u8; 3], [u8; 3])>], filename: &str) {
    let width = (matrix[0].len() as u32) * CELL_SIZE;
    let height = (matrix.len() as u32) * CELL_SIZE;
    let mut img = RgbImage::new(width, height);

    // Load a basic font (use a monospaced font)
    let font_data = include_bytes!("/Users/noahdohrmann/Library/Fonts/MesloLGMNerdFont-Regular.ttf");
    let font = FontRef::try_from_slice(font_data).expect("Failed to load font");
    let scale = PxScale::from(CELL_SIZE as f32 * 0.8);

    for (y, row) in matrix.iter().enumerate() {
        for (x, &(ch, fg, bg)) in row.iter().enumerate() {
            // Draw background color
            let mut dampened = bg.clone();
            for i in 0..bg.len() {
                dampened[i] = bg[i] / 2;
            }
            for dy in 0..CELL_SIZE {
                for dx in 0..CELL_SIZE {
                    img.put_pixel(x as u32 * CELL_SIZE + dx, y as u32 * CELL_SIZE + dy, Rgb(dampened));
                }
            }

            // Draw the character
            let glyph = font.glyph_id(ch).with_scale_and_position(scale, point(0.0, 0.0));
            //let glyph = glyph.scaled(scale).positioned(point(0.0, 0.0));

            if let Some(outline) = font.outline_glyph(glyph) {
                outline.draw(|px, py, c| {
                    let px = px * CELL_SIZE;
                    let py = py * CELL_SIZE;
                    if px > width || py > height {
                        return;
                    }
                    img.put_pixel(px * CELL_SIZE , py * CELL_SIZE, Rgb(fg));
                });
                //for segment in outline.segments() {
                //    for point in segment.points() {
                //        let px = x as f32 * CELL_SIZE as f32 + point.x;
                //        let py = y as f32 * CELL_SIZE as f32 + point.y;
                        //if px >= 0.0 && py >= 0.0 && px < width as f32 && py < height as f32 {
                        //    img.put_pixel(px as u32, py as u32, Rgb(fg));
                        //}
                //    }
                //}
            }
        }
    }

    img.save(Path::new(filename)).expect("Failed to save image");
}

fn main() {
    let matrix = vec![
        vec![('@', [255, 0, 0], [0, 0, 255]); 10],
        vec![('A', [0, 255, 0], [0, 0, 0]); 10],
        vec![('B', [0, 0, 255], [255, 255, 0]); 10],
    ];

    draw_ascii_art(&matrix, "ascii_art_text_rendered.bmp");
    println!("Image with text saved as ascii_art_text_rendered.bmp");
}

