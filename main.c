#include <stdio.h>
#include <stdlib.h>
#include "FreeImage.h"

#define NEEDED_ARGS 3

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
   /* skipping robust checking because 2 args */
   if(argc != NEEDED_ARGS) {
      fprintf(stderr, "Use: ./spritepacker inputFolder outputName\n");
      exit(EXIT_FAILURE);
   }

   return 0;
}
