#include <string.h>
#include <stdio.h>

#include "image.h"

struct image create_image(uint64_t width, uint64_t height){
    struct image img = { .width = width, .height = height};
    img.data = malloc(width * height * sizeof(struct pixel));
    if (!img.data) {
        fprintf(stderr, "Failed to allocate memory for image\n");
        exit(12);
    }
    memset(img.data, 0, width * height * sizeof(struct pixel));
    return img;
}
void destroy_image(struct image *img){
    if (img->data) {
        free(img->data);
        img->data = NULL;
    }
    img->width = img->height = 0;
}
struct pixel* get_pixel_address(struct image *img, uint64_t x, uint64_t y) {
    if (x >= img->width || y >= img->height) {
        exit(EXIT_FAILURE);
    }
    return &img->data[y * img->width + x];
}

struct image clone(struct image *img) {
    struct image cloned = create_image(img->width, img->height);
    if (!cloned.data) {
        memset(&cloned, 0, sizeof(cloned));
        return cloned;
    }

    memcpy(cloned.data, img->data, img->width * img->height * sizeof(*img->data));
    return cloned;
}