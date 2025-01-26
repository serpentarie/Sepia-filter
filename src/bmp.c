#include "bmp.h"

#define BF_TYPE 0x4D42
#define BF_RESERVED 0
#define BI_SIZE 40
#define BI_PLANES 1
#define BI_BIT_COUNT 24
#define BI_COMPRESSION 0
#define BI_XPELS_PER_METER 0
#define BI_YPELS_PER_METER 0
#define BI_CLR_USED 0
#define BI_CLR_IMPORTANT 0

static uint32_t padding(uint32_t width){
    return (4 - (width * 3) % 4) % 4;
}

static struct bmp_header create_bmp_header(const struct image *img){
    return (struct bmp_header) {
            .bfType = BF_TYPE,
            .bfileSize = sizeof(struct bmp_header) + (img->width * img->height * sizeof(struct pixel)),
            .bfReserved = BF_RESERVED,
            .bOffBits = sizeof(struct bmp_header),
            .biSize = BI_SIZE,
            .biWidth = img->width,
            .biHeight = img->height,
            .biPlanes = BI_PLANES,
            .biBitCount = BI_BIT_COUNT,
            .biCompression = BI_COMPRESSION,
            .biSizeImage = img->width * img->height * sizeof(struct pixel),
            .biXPelsPerMeter = BI_XPELS_PER_METER,
            .biYPelsPerMeter = BI_YPELS_PER_METER,
            .biClrUsed = BI_CLR_USED,
            .biClrImportant = BI_CLR_IMPORTANT
    };
}
enum read_status from_bmp(FILE* in, struct image *img){
   struct bmp_header bmp = {0};

    if (fread(&bmp, sizeof(bmp), 1, in) != 1) {
        fprintf(stderr, "Failed to read BMP header.\n");
        return READ_IO_ERROR;
    }
    if (bmp.bfType != BF_TYPE){
        fprintf(stderr, "Invalid BMP signature. Expected 0x%X, got 0x%X.\n",
                BF_TYPE, bmp.bfType);
        return READ_INVALID_SIGNATURE;
    }
    if (bmp.biBitCount != BI_BIT_COUNT){
        fprintf(stderr, "Unsupported bit count. Expected %u, got %u.\n",
                BI_BIT_COUNT, bmp.biBitCount);
        return READ_INVALID_BITS;
    }

    if (bmp.biWidth == 0 || bmp.biHeight == 0) {
        fprintf(stderr, "Invalid image dimensions: width = %u, height = %u.\n",
                bmp.biWidth, bmp.biHeight);
        return READ_INVALID_HEADER;
    }

    *img = create_image(bmp.biWidth, bmp.biHeight);
    const uint32_t p = padding(bmp.biWidth);

    for (uint64_t y = 0; y < img->height; ++y){
        if (fread(img->data + y*img->width, sizeof(struct pixel), img->width, in) != img->width){
            destroy_image(img);
            return READ_IO_ERROR;
        }
        if (fseek(in, p, SEEK_CUR) != 0){
            destroy_image(img);
            return READ_IO_ERROR;
        }
    }
    return READ_OK;
}
enum write_status to_bmp(FILE *out, const struct image *img){
    struct bmp_header bmp = create_bmp_header(img);

    if (fwrite(&bmp, sizeof(bmp), 1, out) != 1)
        return WRITE_ERROR;

    const uint32_t p = padding(img->width);
    uint8_t pad[3] = {0};

    for (uint64_t y = 0; y < img->height ; ++y) {
        if (fwrite(img->data + y * img->width, sizeof(struct pixel), img->width, out) != img->width)
            return WRITE_ERROR;
        if (fwrite(pad, 1, p, out) != p)
            return WRITE_ERROR;
    }
    return WRITE_OK;
}
