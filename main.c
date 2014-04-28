#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <string.h>
#include <math.h>
#include <dirent.h>
#include "FreeImage.h"

/* #defines to remove magic numbers
   since we all hate that
   */
#define DIRECTORY 1
#define FULLPATH 256
#define NEEDED_ARGS 3

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

   /* needs to be called at beginning */
   FreeImage_Initialise(false);

   /* skipping robust checking because 2 args */
   if(argc != NEEDED_ARGS) {
      fprintf(stderr, "Use: ./spritepacker inputFolder outputName\n");
      exit(EXIT_FAILURE);
   }

   calc_optimal_width_and_height(argv[DIRECTORY], &optimalWidth, &optimalHeight);

   FreeImage_DeInitialise();
   return 0;
}

void calc_optimal_width_and_height(char* dirName, unsigned* optimalWidth, unsigned* optimalHeight) {
   /* Enough to hold 256 chars which should be way more than enough */
   char fullPath[FULLPATH];
   DIR* currDir;
   struct dirent* currEntry;
   FREE_IMAGE_FORMAT imgType = FIF_UNKNOWN;
   FIBITMAP* bitmap;
   unsigned minWidth = 0, minHeight = 0, optimalArea = 0, width, height;

   /* we open dir */
   if((currDir = opendir(dirName)) == NULL) {
      perror("opendir");
      exit(EXIT_FAILURE);
   }

   /* modified from fsize from K&R, pg 182 */
   while((currEntry = readdir(currDir)) != NULL) {
      /* skipping self and parent */
      if( (strcmp(currEntry->d_name, ".") == 0) || (strcmp(currEntry->d_name, "..") == 0)) {
         continue;
      }

      /* grab file name and get file type */
      /* This is dumb and there is probably and easier way to pass the full
         path but I'm brain dead right now */
      strcat(fullPath, dirName);
      strcat(fullPath, currEntry->d_name);
      imgType = FreeImage_GetFileType(fullPath, 0);
      if(imgType == FIF_UNKNOWN) {
         imgType = FreeImage_GetFIFFromFilename(fullPath);
      }

      /* load image */
      bitmap = FreeImage_Load(imgType, fullPath, 0);

      /* get width and height in pixels */
      width = FreeImage_GetWidth(bitmap);
      height = FreeImage_GetHeight(bitmap);

      minWidth = fmax(minWidth, width);
      minHeight = fmax(minWidth, height);
      optimalArea += width * height;
   }

   *optimalWidth = fmax(minWidth, (unsigned)sqrt(optimalArea) + 0.5);
   *optimalHeight = fmax(minHeight, optimalArea / *optimalWidth);
}
