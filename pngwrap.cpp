//#ifdef _MSC_VER
//#define _CRT_SECURE_NO_WARNINGS
//#endif

#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#define PNG_SIG_BYTES 8
#include <zlib.h>

#include "pngwrap.h"

void readpng_version_info(void)
{
    printf("[LOG] Compiled with libpng %s; using libpng %s.\n", PNG_LIBPNG_VER_STRING, png_libpng_ver);
    printf("[LOG] Compiled with zlib %s; using zlib %s.\n", ZLIB_VERSION, zlib_version);
}

int imread(const char* filename, unsigned char** image, unsigned int* width, unsigned int* height)
{
    unsigned char hdr[PNG_SIG_BYTES];

    FILE *file_ptr = NULL;
    png_structp png_ptr;
    png_infop info_ptr, end_info;
    int bit_depth, color_type;

    file_ptr = fopen(filename, "rb");
    if (file_ptr == NULL)
    {
        fprintf(stderr, "[PNG] Unable to open file '%s'\n", filename);
        exit(EXIT_FAILURE);
    }

    size_t bytesRead;
    bytesRead = fread(hdr, 1, PNG_SIG_BYTES, file_ptr);
    if (bytesRead != PNG_SIG_BYTES)
    {
        //problem reading signature
        fprintf(stderr, "[PNG] The file's Signature couldn't be read\n");
        fclose(file_ptr);
        exit(EXIT_FAILURE);
    }
    if (!png_check_sig(hdr, PNG_SIG_BYTES))
    {
        //bad signature
        fprintf(stderr, "[PNG] The file %s is not a PNG!\n", filename);
        fclose(file_ptr);
        exit(EXIT_FAILURE);
    }

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL)
    {
        fprintf(stderr, "[PNG] png_create_read_struct() failed!\n");
        fclose(file_ptr);
        exit(EXIT_FAILURE);
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL)
    {
        fprintf(stderr, "[PNG] png_create_info_struct() failed!\n");
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(file_ptr);
        exit(EXIT_FAILURE);
    }

    end_info = png_create_info_struct(png_ptr);
    if (end_info == NULL) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(file_ptr);
        fprintf(stderr, "[PNG] png_create_info_struct() failed for end_info!\n");
        exit(EXIT_FAILURE);
    }

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(file_ptr);
        fprintf(stderr, "[PNG] exception!\n");
        exit(EXIT_FAILURE);
    }

    png_init_io(png_ptr, file_ptr);
    png_set_sig_bytes(png_ptr, PNG_SIG_BYTES);
    png_read_info(png_ptr, info_ptr);

    *width = png_get_image_width(png_ptr, info_ptr);
    *height = png_get_image_height(png_ptr, info_ptr);
    if (*width != *height)
    {
        fprintf(stdout, "[PNG] [WARNING] Just quadratic images are supported yet.\n");
        //exit(EXIT_SUCCESS);
        // TODO: adjust for non-quadratic images later
        // actually no big deal and just interesting for this application
    }
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    if (color_type != PNG_COLOR_TYPE_GRAY)
    {
        fprintf(stdout, "[PNG] ---------->[WARNING] Just grayscale images are supported yet.\n");
        //exit(EXIT_SUCCESS);
        // TODO: something with RGB!
        /*
        if (color_type != PNG_COLOR_TYPE_RGB)
        {
            fprintf(stderr, "Just Grayscale and RGB images are supported.\n");
        }
        // here something because of RGB
        */
    }
    if (bit_depth != 8)
    {
        fprintf(stdout, "[PNG] ---------->[WARNING] Just 8 bit images are supported yet.\n");
        //exit(EXIT_SUCCESS);
        // TODO: something with 16bit maybe
    }

    // size of one row:
    size_t row_size = png_get_rowbytes(png_ptr, info_ptr);
    // complete array:
    unsigned char *png_bytes = (unsigned char*)malloc(*height * row_size);
    unsigned char **png_row_ptrs = (unsigned char**)malloc(*height * sizeof(unsigned char*));
    for (unsigned int i = 0; i < *height; i++)
    {
        png_row_ptrs[i] = png_bytes + row_size *i;
    }
    png_read_image(png_ptr, png_row_ptrs);
    *image = png_bytes;

    // Recycling
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(png_row_ptrs);
    png_row_ptrs = NULL;
    png_ptr = NULL;
    info_ptr = NULL;
    end_info = NULL;
    fclose(file_ptr);
    file_ptr = NULL;

    return PNG_WRAP_SUCCESS;
}

int imwrite(const char* filename, unsigned char* image, unsigned int width, unsigned int height)
{
    FILE * file_ptr = NULL;
    png_structp png_ptr;
    png_infop info_ptr;

    // Initialization
    file_ptr = fopen(filename, "wb");
    if (file_ptr == NULL)
    {
        fprintf(stderr, "[PNG] Unable to open file '%s'\n", filename);
        exit(EXIT_FAILURE);
    }
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL)
    {
        fprintf(stderr, "[PNG] png_create_write_struct() failed!\n");
        fclose(file_ptr);
        exit(EXIT_FAILURE);
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL)
    {
        fprintf(stderr, "[PNG] png_create_info_struct() failed!\n");
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(file_ptr);
        exit(EXIT_FAILURE);
    }
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(file_ptr);
        fprintf(stderr, "[PNG] exception!\n");
        exit(EXIT_FAILURE);
    }
    png_init_io(png_ptr, file_ptr);

    // Writing Header
    png_set_IHDR(png_ptr, info_ptr, width, height,
        8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_write_info(png_ptr, info_ptr);

    /* write bytes */
    size_t row_size = png_get_rowbytes(png_ptr, info_ptr);
    unsigned char **png_row_ptrs = (unsigned char**)malloc(height * sizeof(unsigned char*));
    for (unsigned int i = 0; i < height; i++)
    {
        // hope this works :)
        png_row_ptrs[i] = image + row_size *i;
    }
    png_write_image(png_ptr, png_row_ptrs);
    
    // write end
    png_write_end(png_ptr, NULL);

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    free(png_row_ptrs);
    png_row_ptrs = NULL;
    fclose(file_ptr);
    file_ptr = NULL;


    return PNG_WRAP_SUCCESS;
}