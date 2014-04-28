#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <dirent.h>
#include "FreeImage.h"

/* #defines to remove magic numbers
   since we all hate that
   */
#define DIRECTORY 1
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
   FreeImage_Initialise(false);
   DIR* currDir;
   struct dirent* currEntry;
   /* skipping robust checking because 2 args */
   if(argc != NEEDED_ARGS) {
      fprintf(stderr, "Use: ./spritepacker inputFolder outputName\n");
      exit(EXIT_FAILURE);
   }

   /* we open dir */
   if((currDir = opendir(argv[DIRECTORY])) == NULL) {
      perror("opendir");
      exit(EXIT_FAILURE);
   }

   FreeImage_DeInitialise();
   return 0;
}
