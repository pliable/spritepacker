#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "FreeImage.h"
#include "helper.h"
#include "magic.h"

void calc_optimal_width_and_height(char* dirName, unsigned* minWidth, unsigned* minHeight);
/*
 * Sprite packer
 * -------------
 * This program is based on an algorithm found at:
 * http://www.sitioweb.fr/blog/3-technical/3-sprite-packing-algorithm
 * 
 * This is for shits and giggles, fun and practice, games and...whatever,
 * you get what I mean
 *
 * @author Steve Choo
 */
int main(int argc, char *argv[]) {
   unsigned optimalWidth, optimalHeight;
   int numFiles = 0;
   bmp_info* bmps;

   /* needs to be called at beginning */
   FreeImage_Initialise(false);

   /* skipping robust checking because 2 args */
   if(argc != NEEDED_ARGS) {
      fprintf(stderr, "Use: ./spritepacker inputFolder outputName\n");
      exit(EXIT_FAILURE);
   }

   numFiles = count_files(argv[DIRECTORY]);
   populate_bmp_info(&bmps, argv[DIRECTORY], numFiles);

   /*calc_optimal_width_and_height(argv[DIRECTORY], &optimalWidth, &optimalHeight);
   */

   FreeImage_DeInitialise();
   return 0;
}
