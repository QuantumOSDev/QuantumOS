#ifndef TGA_H
#define TGA_H

#pragma pack(push)  // save the original data alignment
#pragma pack(1)     // Set data alignment to 1 byte boundary

typedef struct {
  unsigned char signature;             // must be zero
  unsigned char colormap;              // must be zero
  unsigned char encoding;              // must be 2
  unsigned short cmaporig, cmaplen;    // must be zero
  unsigned char cmapent;               // must be zero
  unsigned short x;                    // must be zero
  unsigned short y;                    // image's height
  unsigned short h;                    // image's height
  unsigned short w;                    // image's width
  unsigned char bpp;                   // must be 32
  unsigned char pixeltype;             // must be 40
} __attribute__((packed)) tga_header_t;

#pragma pack(pop)  // restore the previous pack setting

typedef struct __tga_structure_t {
    tga_header_t*  header;
    unsigned char* pixel_data;
} tga_structure_t;

tga_structure_t* parse_tga(char* data, int data_size);

#endif