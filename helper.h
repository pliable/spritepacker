#include "magic.h"

typedef struct {
   char filename[FULLPATH];
   unsigned height;
   unsigned width;
} bmp_info;
unsigned umax(unsigned l, unsigned r);
int comp(const void *p1, const void *p2);
int count_files(char* dirName);
void populate_bmp_info(bmp_info** out_bmps, char* dirName, int file_count);
void calc_optimal_width_and_height(char* dirName, unsigned* minWidth, unsigned* minHeight);
