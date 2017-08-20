#include "Bang/ImageIO.h"

#include <png.h>
#include <stdio.h>
#include <stdlib.h>

#include "Bang/Debug.h"

void ImageIO::Write(const Path &filepath, const G_Image &img)
{
    if (filepath.GetExtension().EqualsNoCase("png"))
    {
        ImageIO::WritePNG(filepath, img);
    }
    else
    {
        Debug_Error("Unrecognized image format " << filepath.GetExtension());
    }
}

void ImageIO::Read(const Path &filepath, G_Image *img, bool *_ok)
{
    bool ok = false;

    if (filepath.GetExtension().EqualsNoCase("png"))
    {
        ImageIO::ReadPNG(filepath, img, &ok);
    }
    else
    {
        Debug_Error("Unrecognized image format " << filepath.GetExtension());
    }

    if (_ok) { *_ok = ok; }
}

void ImageIO::WritePNG(const Path &filepath, const G_Image &img)
{
    FILE *fp = fopen(filepath.GetAbsolute().ToCString(), "wb");
    ENSURE(fp);

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                              NULL, NULL, NULL);
    ENSURE(png);

    png_infop info = png_create_info_struct(png);
    ENSURE(info);
    ENSURE ( !setjmp(png_jmpbuf(png)) );

    png_init_io(png, fp);

    png_set_IHDR(
        png,
        info,
        img.GetWidth(),
        img.GetHeight(),
        8,
        PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png, info);


    png_bytep *rowPointers = new png_bytep[img.GetHeight()];
    for (int y = 0; y < img.GetHeight(); y++)
    {
        rowPointers[y] =
                new png_byte[ png_get_rowbytes(png, info) / sizeof(png_byte)];
        for (int x = 0; x < img.GetWidth(); ++x)
        {
            rowPointers[y][x * 4 + 0] = SCAST<Byte>(img.GetPixel(x, y).r * 255);
            rowPointers[y][x * 4 + 1] = SCAST<Byte>(img.GetPixel(x, y).g * 255);
            rowPointers[y][x * 4 + 2] = SCAST<Byte>(img.GetPixel(x, y).b * 255);
            rowPointers[y][x * 4 + 3] = SCAST<Byte>(img.GetPixel(x, y).a * 255);
        }
    }
    png_write_image(png, rowPointers);
    png_write_end(png, NULL);

    for (int y = 0; y < img.GetHeight(); y++)
    {
        delete[] rowPointers[y];
    }
    delete[] rowPointers;

    png_destroy_write_struct(&png, &info);
    fclose(fp);
}

void ImageIO::ReadPNG(const Path &filepath, G_Image *img, bool *ok)
{
    *ok = false;

    FILE *fp = fopen(filepath.GetAbsolute().ToCString(), "rb");
    ENSURE(fp);

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                             NULL, NULL, NULL);
    ENSURE(png);

    png_infop info = png_create_info_struct(png);
    ENSURE(info);

    ENSURE( !setjmp( png_jmpbuf(png) ) );

    png_init_io(png, fp);
    png_read_info(png, info);

    png_byte bit_depth = png_get_bit_depth(png, info);
    if (bit_depth == 16) { png_set_strip_16(png); }

    png_byte colorType = png_get_color_type(png, info);
    if (colorType == PNG_COLOR_TYPE_PALETTE) { png_set_palette_to_rgb(png); }
    if (colorType == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    {
        png_set_expand_gray_1_2_4_to_8(png);
    }

    if (png_get_valid(png, info, PNG_INFO_tRNS))
    {
        png_set_tRNS_to_alpha(png);
    }

    // These color_type don't have an alpha channel then fill it with 0xff.
    if (colorType == PNG_COLOR_TYPE_RGB  ||
        colorType == PNG_COLOR_TYPE_GRAY ||
        colorType == PNG_COLOR_TYPE_PALETTE)
    {
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    }

    if (colorType == PNG_COLOR_TYPE_GRAY ||
        colorType == PNG_COLOR_TYPE_GRAY_ALPHA)
    {
        png_set_gray_to_rgb(png);
    }

    png_read_update_info(png, info);

    int height = png_get_image_height(png, info);
    png_bytep *rowPointers = new png_bytep[height];
    for (int y = 0; y < height; y++)
    {
        rowPointers[y] =
                new png_byte[ png_get_rowbytes(png, info) / sizeof(png_byte)];
    }
    png_read_image(png, rowPointers);

    int width = png_get_image_width(png, info);
    img->Create(width, height);
    for (int y = 0; y < height; ++y)
    {
        png_bytep row = rowPointers[y];
        for (int x = 0; x < width; ++x)
        {
            Color c(row[x * 4 + 0] / 255.0f,
                    row[x * 4 + 1] / 255.0f,
                    row[x * 4 + 2] / 255.0f,
                    row[x * 4 + 3] / 255.0f);
            img->SetPixel(x, y, c);
        }
        delete[] row;
    }

    delete[] rowPointers;
    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);

    *ok = true;
}
