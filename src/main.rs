//! Draws text into `image_example.png`.
//!
//! Use a custom font file: `cargo run --example image /path/to/font.otf`
use ab_glyph::{point, Font, FontRef, FontVec, PxScale, Point, ScaleFont, Glyph};
use image::{DynamicImage, Rgba};

const TEXT: &str = "Yyyyyyyy\nYyyyyyyyyyyy";

fn main() {
    if let Some(font_path) = std::env::args().nth(1) {
        let font_path = std::env::current_dir().unwrap().join(font_path);
        let data = std::fs::read(&font_path).unwrap();
        let font = FontVec::try_from_vec(data).unwrap_or_else(|_| {
            panic!("error constructing a Font from data at {:?}", font_path);
        });
        if let Some(name) = font_path.file_name().and_then(|n| n.to_str()) {
            eprintln!("Using font: {name}");
        }
        draw_image(font);
    } else {
        eprintln!("No font specified ... using OpenSans-Italic.ttf");
        let font = FontRef::try_from_slice(
            include_bytes!("../font/fonts/ttf/JetBrainsMono-Regular.ttf")
        ).unwrap();
        draw_image(font);
    };
}

fn draw_image<F: Font>(font: F) {
    /// Dark red colour
    const COLOUR: (u8, u8, u8) = (0x8c, 0, 0);

    // The font size to use
    let scale = PxScale::from(45.0);

    let scaled_font = font.as_scaled(scale);

    let mut glyphs = Vec::new();
    layout_paragraph(scaled_font, point(20.0, 20.0), 9999.0, TEXT, &mut glyphs);

    // to work out the exact size needed for the drawn glyphs we need to outline
    // them and use their `px_bounds` which hold the coords of their render bounds.
    let outlined: Vec<_> = glyphs
        .into_iter()
        // Note: not all layout glyphs have outlines (e.g. " ")
        .filter_map(|g| font.outline_glyph(g))
        .collect();

    // combine px_bounds to get min bounding coords for the entire layout
    let Some(all_px_bounds) = outlined
        .iter()
        .map(|g| g.px_bounds())
        .reduce(|mut b, next| {
            b.min.x = b.min.x.min(next.min.x);
            b.max.x = b.max.x.max(next.max.x);
            b.min.y = b.min.y.min(next.min.y);
            b.max.y = b.max.y.max(next.max.y);
            b
        })
    else {
        println!("No outlined glyphs?");
        return;
    };

    // create a new rgba image using the combined px bound width and height
    let mut image =
        DynamicImage::new_rgba8(all_px_bounds.width() as _, all_px_bounds.height() as _).to_rgba8();

    // Loop through the glyphs in the text, positing each one on a line
    for glyph in outlined {
        let bounds = glyph.px_bounds();
        // calc top/left ords in "image space"
        // image-x=0 means the *left most pixel*, equivalent to
        // px_bounds.min.x which *may be non-zero* (and similarly with y)
        // so `- px_bounds.min` converts the left-most/top-most to 0
        let img_left = bounds.min.x as u32 - all_px_bounds.min.x as u32;
        let img_top = bounds.min.y as u32 - all_px_bounds.min.y as u32;
        // Draw the glyph into the image per-pixel by using the draw closure
        glyph.draw(|x, y, v| {
            // Offset the position by the glyph bounding box
            let px = image.get_pixel_mut(img_left + x, img_top + y);
            // Turn the coverage into an alpha value (blended with any previous)
            match v {
                v if v < 0.1 => {
                    *px = Rgba([
                        0,
                        0,
                        0,
                        255
                    ]);
                },
                v => {
                    *px = Rgba([
                        COLOUR.0,
                        COLOUR.1,
                        COLOUR.2,
                        px.0[3].saturating_add((v * 255.0) as u8),
                    ]);
                }
            }
        });
    }

    // Save the image to a png file
    image.save("image_example.png").unwrap();
    println!(
        "Generated: image_example.png ({}x{})",
        image.width(),
        image.height()
    );
}

pub fn layout_paragraph<F, SF>(
    font: SF,
    position: Point,
    max_width: f32,
    text: &str,
    target: &mut Vec<Glyph>,
) where
    F: Font,
    SF: ScaleFont<F>,
{
    let v_advance = 0.8 * (font.height() + font.line_gap());
    let mut caret = position + point(0.0, font.ascent());
    let mut last_glyph: Option<Glyph> = None;
    for c in text.chars() {
        if c.is_control() {
            if c == '\n' {
                caret = point(position.x, caret.y + v_advance);
                last_glyph = None;
            }
            continue;
        }
        let mut glyph = font.scaled_glyph(c);
        if let Some(previous) = last_glyph.take() {
            caret.x += font.kern(previous.id, glyph.id);
        }
        glyph.position = caret;

        last_glyph = Some(glyph.clone());
        caret.x += font.h_advance(glyph.id);

        if !c.is_whitespace() && caret.x > position.x + max_width {
            caret = point(position.x, caret.y + v_advance);
            glyph.position = caret;
            last_glyph = None;
        }

        target.push(glyph);
    }
}
