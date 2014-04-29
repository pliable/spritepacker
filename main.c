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
   printf("optimalWidth: %u\n", optimalWidth);
   printf("optimalHeight: %u\n", optimalHeight);

   FreeImage_DeInitialise();
   return 0;
}

void calc_optimal_width_and_height(char* dirName, unsigned* optimalWidth, unsigned* optimalHeight) {
   /* Enough to hold 256 chars which should be way more than enough */
   char fullPath[FULLPATH] = {'\0'};
   DIR* currDir;
   struct dirent* currEntry;
   FREE_IMAGE_FORMAT imgType = FIF_UNKNOWN;
   FIBITMAP* bitmap = NULL;
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

      /* grab file name and get file type, exit if path is stupidly long */
      if(!(strlen(dirName) + strlen(currEntry->d_name) < FULLPATH)) {
         fprintf(stderr, "Path: %s%s too long, exiting...\n", dirName, currEntry->d_name);
         exit(EXIT_FAILURE);
      }

      strcat(fullPath, dirName);
      strcat(fullPath, currEntry->d_name);

      imgType = FreeImage_GetFileType(fullPath, 0);
      if(imgType == FIF_UNKNOWN) {
         imgType = FreeImage_GetFIFFromFilename(fullPath);
      }

      /* load image */
      if( !(bitmap = FreeImage_Load(imgType, fullPath, 0)) ) {
         fprintf(stderr, "Image failed to load, exiting...\n");
         exit(EXIT_FAILURE);
      }

      /* get width and height in pixels */
      width = FreeImage_GetWidth(bitmap);
      height = FreeImage_GetHeight(bitmap);

      minWidth = fmax(minWidth, width);
      minHeight = fmax(minWidth, height);
      optimalArea += width * height;

      /* unload image - don't want memory leaks! */
      FreeImage_Unload(bitmap);
      
      /* clearing buffer */
      fullPath[0] = '\0';
   }

   (*optimalWidth) = (unsigned)fmax(minWidth, (sqrt(optimalArea) + 0.5));
   (*optimalHeight) = (unsigned)fmax(minHeight, optimalArea / (*optimalWidth));
}
