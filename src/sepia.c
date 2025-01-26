#include <math.h>

#include "sepia.h"

extern void sepia_filter_asm(struct pixel* pixel);

void transform(struct pixel* pixel){
    float r = pixel->r;
    float g = pixel->g;
    float b = pixel->b;

    uint8_t new_r = (uint8_t)fmin(0.393f * r + 0.769f * g + 0.189f * b, 255);
    uint8_t new_g = (uint8_t)fmin(0.349f * r + 0.686f * g + 0.168f * b, 255);
    uint8_t new_b = (uint8_t)fmin(0.272f * r + 0.534f * g + 0.131f * b, 255);

    pixel->r = new_r;
    pixel->g = new_g;
    pixel->b = new_b;
}

void sepia_c(struct image* image) {
    if (!image || !image->data)
        return;

    struct pixel* data_end = image->data + image->width * image->height;
    for (struct pixel* p = image->data; p < data_end; ++p) {
        transform(p);
    }
}

void sepia_asm(struct image* image) {
    if (!image || !image->data)
        return;

    struct pixel* data_end = image->data + image->width * image->height;
    for (struct pixel* p = image->data; p < data_end; ++p) {
        sepia_filter_asm(p);
    }
}