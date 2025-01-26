#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "bmp.h"
#include "sepia.h"
#include "time.h"

int main(int argc, char** argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <source> <dest> <dest_asm>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *in = fopen(argv[1], "rb");
    if (!in) {
        fprintf(stderr, "Failed to open input file\n");
        return ENOENT;
    }

    struct image img;
    enum read_status read_status = from_bmp(in, &img);
    fclose(in);

    if (read_status != READ_OK) {
        fprintf(stderr, "Error while reading input file\n");
        return ENOMEM;
    }

    struct image img2 = clone(&img);

    clock_t start_c = clock();
    sepia_c(&img);
    clock_t end_c = clock();

    clock_t start_asm = clock();
    sepia_asm(&img2);
    clock_t end_asm = clock();

    FILE *out = fopen(argv[2], "wb");
    if (!out) {
        fprintf(stderr, "Failed to open output file\n");
        destroy_image(&img);
        return EXIT_FAILURE;
    }

    enum write_status write_status = to_bmp(out, &img);
    fclose(out);
    destroy_image(&img);

    if (write_status != WRITE_OK) {
        fprintf(stderr, "Error writing to output C file\n");
        return EIO;
    }

    FILE *out_asm = fopen(argv[3], "wb");
    if (!out_asm) {
        fprintf(stderr, "Failed to open output ASM file\n");
        destroy_image(&img2);
        return EXIT_FAILURE;
    }

    write_status = to_bmp(out_asm, &img2);
    fclose(out_asm);
    destroy_image(&img2);

    if (write_status != WRITE_OK) {
        fprintf(stderr, "Error writing to output ASM file\n");
        return EIO;
    }

    double time_c = (double)(end_c - start_c) / CLOCKS_PER_SEC;
    double time_asm = (double)(end_asm - start_asm) / CLOCKS_PER_SEC;

    fprintf(stdout, "Time for C: %.5f seconds\n", time_c);
    fprintf(stdout, "Time for ASM: %.5f seconds\n", time_asm);

    return EXIT_SUCCESS;
}
