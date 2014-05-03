#include "magic.h"
typedef struct {
   char filename[FULLPATH];
   unsigned height;
   unsigned width;
   FREE_IMAGE_FORMAT imgType;
} bmp_info;
unsigned umax(unsigned l, unsigned r);
int comp(const void *p1, const void *p2);
int count_files(char* dirName);
void populate_bmp_info(bmp_info** outBmps, char* dirName, int fileCount);
void calc_optimal_width_and_height(bmp_info* bmps, int numFiles, unsigned* minWidth, unsigned* minHeight);
void make_horizontal_sprite(FIBITMAP** canvas, bmp_info* bmps, int numBmps);
unsigned total_width(bmp_info* bmps, int numBmps);
unsigned max_height(bmp_info* bmps, int numBmps);
