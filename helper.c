/*
    Spacker
    Copyright (C) 2014 Steve Choo

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <math.h>
#include "helper.h"

/* Do i really need to pass in FIBITMAP** canvas? I'm not even using it in main so wtf am i doing */

/* really need to break this into smaller functions... */
void make_packed_sprite(FIBITMAP** canvas, bmp_info* bmps, int numBmps, unsigned width, unsigned height, char* outputName) {
   int i;
   unsigned widthSoFar = 0, heightSoFar = 0, currentRowHeight = 0;
   FIBITMAP* c = NULL, *bitmap = NULL;

   /* allocate canvas */
   *canvas = FreeImage_Allocate(width, height, 32, 0, 0, 0);

   if(!(*canvas)) {
      fprintf(stderr, "FreeImage_Allocate fail, exiting...\n");
      exit(EXIT_FAILURE);
   }

   /* for easier referencing down the line */
   c = *canvas;
   /* this is still dirty but eh*/
   currentRowHeight = bmps[0].height;

   for(i = 0; i < numBmps; i++) {
      /* if width of current image goes over width, start new row */
      if( (widthSoFar + bmps[i].width) > width) {
         /* calculate new height for further pastes */
         heightSoFar += currentRowHeight;

         /* get new row height */
         currentRowHeight = bmps[i].height;

         /* if new image goes over height, copy old canvas, paste on allocated new one */
         if(heightSoFar + currentRowHeight > height) {
            /* grab current canvas */
            FIBITMAP *temp = NULL, *newCanvas = NULL;
            temp = FreeImage_Copy(c, 0, 0, width, height);
            /* overwrite new height */
            height = heightSoFar + currentRowHeight;
            /* allocate new canvas */
            newCanvas = FreeImage_Allocate(width, height, 32, 0, 0, 0);
         
            /* paste old canvas on top of new */
            if(!FreeImage_Paste(newCanvas, temp, 0, 0, 9000)) {
               fprintf(stderr, "(1)Image failed to paste, exiting...\n");
               exit(EXIT_FAILURE);
            }

            /* free old canvas */
            FreeImage_Unload(c);
            /* free temp canvas */
            FreeImage_Unload(temp);

            /* update them pointers */
            c = newCanvas;
         }

         /* reset width to 0 */
         widthSoFar = 0;
      }

      /* load image */
      if( !(bitmap = FreeImage_Load(bmps[i].type, bmps[i].filename, 0)) ) {
         fprintf(stderr, "Image failed to load, exiting...\n");
         exit(EXIT_FAILURE);
      }
      
      /* paste image on canvas */
      if( !FreeImage_Paste(c, bitmap, widthSoFar, heightSoFar, /*it's over*/ 9000)) {
         fprintf(stderr, "(2)Image failed to paste, exiting...\n");
         exit(EXIT_FAILURE);
      }

      /* unload image */
      FreeImage_Unload(bitmap);
      /* track where we are */
      widthSoFar += bmps[i].width;
   }

   if( !FreeImage_Save(FIF_BMP, c, outputName, 0) ) {
      fprintf(stderr, "Image failed to save, exiting...\n");
      exit(EXIT_FAILURE);
   }

   /* freeimage unload for canvas? */


}

void make_horizontal_sprite(FIBITMAP** canvas, bmp_info* bmps, int numBmps, char* outputName) {
   int i;
   unsigned widthSoFar = 0;
   FIBITMAP* c = NULL, *bitmap = NULL;

   /* allocate canvas */
   *canvas = FreeImage_Allocate(total_width(bmps, numBmps), max_height(bmps, numBmps), 32, 0, 0, 0);

   if(!(*canvas)) {
      fprintf(stderr, "FreeImage_Allocate fail, exiting...\n");
      exit(EXIT_FAILURE);
   }

   c = *canvas;

   for(i = 0; i < numBmps; i++) {
      /* load image */
      if( !(bitmap = FreeImage_Load(bmps[i].type, bmps[i].filename, 0)) ) {
         fprintf(stderr, "Image failed to load, exiting...\n");
         exit(EXIT_FAILURE);
      }

      /* paste image on canvas */
      if( !FreeImage_Paste(c, bitmap, widthSoFar, 0, /*it's over*/ 9000)) {
         fprintf(stderr, "Image failed to paste, exiting...\n");
         exit(EXIT_FAILURE);
      }

      /* unload image */
      FreeImage_Unload(bitmap);
      /* track where we are */
      widthSoFar += bmps[i].width;
   }

   if( !FreeImage_Save(FIF_BMP, c, outputName, 0) ) {
      fprintf(stderr, "Image failed to save, exiting...\n");
      exit(EXIT_FAILURE);
   }

   /* freeimage unload for canvas? */
}

unsigned total_width(bmp_info* bmps, int numBmps) {
   int i;
   unsigned totalWidth = 0;

   for(i = 0; i < numBmps; i++) {
      totalWidth += bmps[i].width;
   }

   return totalWidth;
}

unsigned max_height(bmp_info* bmps, int numBmps) {
   int i;
   unsigned maxSoFar = 0;

   for(i = 0; i < numBmps; i++) {
      if(maxSoFar < bmps[i].height) {
         maxSoFar = bmps[i].height;
      }
   }

   return maxSoFar;
}

void calc_optimal_width_and_height(bmp_info* bmps, int numFiles, unsigned* optimalWidth, unsigned* optimalHeight) {
   unsigned minWidth = 0, minHeight = 0, optimalArea = 0;
   int i;

   for(i = 0; i < numFiles; i++) {
      minWidth = umax(minWidth, bmps[i].width);
      minHeight = umax(minHeight, bmps[i].height);
      optimalArea += bmps[i].width * bmps[i].height;
   }

   *optimalWidth = umax(minWidth, (unsigned)(sqrt(optimalArea) + 0.5));
   *optimalHeight = umax(minHeight, (optimalArea / (*optimalWidth)));
}

/* modified from here: http://stackoverflow.com/questions/1121383/counting-the-number-of-files-in-a-directory-using-c */
/* count_files will be used to make an array of bmp_infos,
   which will contain various info about each bmp in the folder
   */
int count_files(char* dirName) {
   int fileCount = 0;
   DIR *currDir;
   struct dirent* currEntry;

   /* we open dir */
   if((currDir = opendir(dirName)) == NULL) {
      perror("opendir");
      exit(EXIT_FAILURE);
   }

   while ((currEntry = readdir(currDir)) != NULL) {

      if (currEntry->d_type == DT_REG) { /* If the entry is a regular file */
         fileCount++;
      }
   }

   closedir(currDir);

   return fileCount;
}

/* Okay so, glib is acting weird on my PC and I really don't
   feel like writing my own dynamic array, since I just
   want to just finish the damn packer. This populates a
   bmp_info array with filename, width, and height */
void populate_bmp_info(bmp_info** outBmps, char* dirName, int fileCount) {
   char fullPath[FULLPATH] = {'\0'};
   DIR* currDir;
   struct dirent* currEntry;
   FREE_IMAGE_FORMAT imgType = FIF_UNKNOWN;
   FIBITMAP* bitmap = NULL;
   int currBMPIdx = 0;
   bmp_info* bmps;

   /* callocing bmps array */
   *outBmps = (bmp_info*)calloc(fileCount, sizeof(bmp_info));
   /*outBmps = (bmp_info*)malloc(fileCount * sizeof(bmp_info)); */

   if(*outBmps == NULL) {
      fprintf(stderr, "calloc fail, exiting...\n");
      exit(EXIT_FAILURE);
   }

   /* according to a buddy, the cheating cheaters way of using
      a double pointer :P
   */
   bmps = *outBmps;

   /* we open dir */
   if((currDir = opendir(dirName)) == NULL) {
      perror("opendir");
      exit(EXIT_FAILURE);
   }

   /* modified from fsize from K&R, pg 182 */
   while((currEntry = readdir(currDir)) != NULL) {
      
      if (!(currEntry->d_type == DT_REG)) { /* If the entry is a regular file */
         continue;
      }
      
      /* grab file name and get file type, exit if path is stupidly long */
      if(!(strlen(dirName) + strlen(currEntry->d_name) < FULLPATH)) {
         fprintf(stderr, "Path: %s%s too long, exiting...\n", dirName, currEntry->d_name);
         exit(EXIT_FAILURE);
      }

      strcat(fullPath, dirName);
      strcat(fullPath, currEntry->d_name);
      
      if(currBMPIdx >= fileCount) {
          /* something went wrong so bounce this popstinkle stand */
          break;
      }

      /* copy filename over to struct */
      strncpy(bmps[currBMPIdx].filename, fullPath, FULLPATH);

      imgType = FreeImage_GetFileType(fullPath, 0);
      if(imgType == FIF_UNKNOWN) {
         imgType = FreeImage_GetFIFFromFilename(fullPath);
      }

      bmps[currBMPIdx].type = imgType;

      /* load image */
      if( !(bitmap = FreeImage_Load(imgType, fullPath, 0)) ) {
         fprintf(stderr, "Image failed to load, exiting...\n");
         exit(EXIT_FAILURE);
      }

      /* get width and height in pixels */
      bmps[currBMPIdx].height = FreeImage_GetHeight(bitmap);
      bmps[currBMPIdx].width = FreeImage_GetWidth(bitmap);

      /* unload image - don't want memory leaks! */
      FreeImage_Unload(bitmap);
      
      /* clearing buffer */
      fullPath[0] = '\0';
      currBMPIdx++;
   }

   if(closedir(currDir) < 0) {
      perror("closedir");
      exit(EXIT_FAILURE);
   }
}

unsigned umax(unsigned l, unsigned r) {
   return l < r ? r : l;
}

/* comparator function for qsort(), used to sort in
   **descending** order*/
int comp(const void *p1, const void *p2) {
   return (((bmp_info*)p2)->height) - (((bmp_info*)p1)->height);
}
