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
#include "FreeImage.h"
#include "helper.h"

/*this is going to be completely rewritten */
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

      minWidth = umax(minWidth, width);
      minHeight = umax(minHeight, height);
      optimalArea += width * height;

      /* unload image - don't want memory leaks! */
      FreeImage_Unload(bitmap);
      
      /* clearing buffer */
      fullPath[0] = '\0';
   }

   (*optimalWidth) = umax(minWidth, (unsigned)(sqrt(optimalArea) + 0.5));
   (*optimalHeight) = umax(minHeight, (optimalArea / (*optimalWidth)));

   if(closedir(currDir) < 0) {
      perror("closedir");
      exit(EXIT_FAILURE);
   }
}
/* modified from here: http://stackoverflow.com/questions/1121383/counting-the-number-of-files-in-a-directory-using-c */
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
      /* skipping self and parent */
      if( (strcmp(currEntry->d_name, ".") == 0) || (strcmp(currEntry->d_name, "..") == 0)) {
         continue;
      }

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
void populate_bmp_info(bmp_info** out_bmps, char* dirName, int file_count) {
   char fullPath[FULLPATH] = {'\0'};
   DIR* currDir;
   struct dirent* currEntry;
   FREE_IMAGE_FORMAT imgType = FIF_UNKNOWN;
   FIBITMAP* bitmap = NULL;
   int currBMPIdx = 0;
   bmp_info* bmps;

   /* mallocing bmps array */
   *out_bmps = (bmp_info*)malloc(file_count * sizeof(bmp_info));

   if(*out_bmps == NULL) {
      fprintf(stderr, "malloc fail, exiting...\n");
      exit(EXIT_FAILURE);
   }

   bmps = *out_bmps;

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

      /* copy filename over to struct */
      strncpy(bmps[currBMPIdx].filename, fullPath, FULLPATH);

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
      bmps[currBMPIdx].height = FreeImage_GetHeight(bitmap);
      bmps[currBMPIdx].width = FreeImage_GetWidth(bitmap);

      /* unload image - don't want memory leaks! */
      FreeImage_Unload(bitmap);
      
      /* clearing buffer */
      fullPath[0] = '\0';
      currBMPIdx++;
   }
}

unsigned umax(unsigned l, unsigned r) {
   return l < r ? r : l;
}

/* comparator function for qsort() */
int comp(const void *p1, const void *p2) {
   return *((int*)p1) - *((int*)p2);
}
