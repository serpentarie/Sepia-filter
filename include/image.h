#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>
#include <stdlib.h>

struct pixel {
    uint8_t b, g, r;
};
struct image {
    uint64_t width, height;
    struct pixel *data;
};

struct image create_image(uint64_t width, uint64_t height);
void destroy_image(struct image *img);
struct image clone(struct image *img);
struct pixel* get_pixel_address(struct image *img, uint64_t x, uint64_t y);

#endif //IMAGE_H
