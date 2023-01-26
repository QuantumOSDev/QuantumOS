/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <core/tga.h>
#include <core/string.h>

#include <quantum/init.h>
#include <sys/memory.h>

tga_structure_t* parse_tga(char* data, int data_size)
{
    quantum_info(2, " TGA    ", "Parsing tga");
    
    tga_structure_t* tga_structure = kmalloc(sizeof(tga_header_t));
    tga_structure->header      = (tga_header_t*)data;

    if (tga_structure->header->signature != 0)
    {
        quantum_info(1, " TGA    ", "This isn't an tga file format");
        return NULL;
    }

    if (tga_structure->header->colormap != 0)
    {
        quantum_info(1, " TGA    ", "This isn't an tga file format or it is not correct");
        return NULL;
    }

    if (tga_structure->header->encoding != 2)
    {
        quantum_info(1, " TGA    ", "Unsupported encoding of tga file");
        return NULL;
    }

    if (tga_structure->header->bpp != 32)
    {
        quantum_info(1, " TGA    ", "This tga file is not correct because bpp is not 32");
        return NULL;
    }

    quantum_info(0, " TGA    ", "This is an tga file format");
    quantum_info(0, " TGA    ", "Image size: %dx%d", tga_structure->header->h, tga_structure->header->w);
    quantum_info(0, " TGA    ", "Encoding: %d", tga_structure->header->encoding);
    quantum_info(0, " TGA    ", "BPP: %d", tga_structure->header->bpp);
    quantum_info(0, " TGA    ", "Pixel type: %d", tga_structure->header->pixeltype);

    tga_structure->pixel_data = data + sizeof(tga_header_t);
    return tga_structure;
}