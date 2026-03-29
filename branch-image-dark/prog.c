#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>

enum {W = 15360, H = 8640};

double wtime()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1E-9;
}

int image_is_dark(uint8_t *img, int width, int height)
{
    int count = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (img[i * width + j] >= 128) {
                count++;
            }
        }
    }
    return count < width * height / 2;
}

int image_is_dark_opt(uint8_t *img, int width, int height)
{
    int total = width * height;
    int half  = total / 2;
    int count = 0;

    uint8_t *p = img;
    uint8_t *end = img + total;

    for (; p + 8 <= end; p += 8) {
        count += (p[0] >= 128);
        count += (p[1] >= 128);
        count += (p[2] >= 128);
        count += (p[3] >= 128);
        count += (p[4] >= 128);
        count += (p[5] >= 128);
        count += (p[6] >= 128);
        count += (p[7] >= 128);
    }

    for (; p < end; ++p) {
        count += (*p >= 128);
    }

    return count < half;
}

int main()
{
    uint8_t *image = malloc(sizeof(*image) * W * H);
    srand(0);
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            image[i * W + j] = rand() % 256;
        }
    }

    double t = wtime();
    int dark = image_is_dark(image, W, H);
    //int dark = image_is_dark_opt(image, W, H);
    t = wtime() - t;

    printf("Time %.6f, dark %d\n", t, dark);

    free(image);
    return 0;
}
