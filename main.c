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

 --------------------------------------------------------------------------
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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "FreeImage.h"
#include "helper.h"
#include "magic.h"

int main(int argc, char *argv[]) {
   unsigned optimalWidth, optimalHeight;
   int numFiles = 0;
   bmp_info* bmps;
   /* this is probably completely unnecessary but keeping for now in case
      i want to use canvas in the main scope */
   FIBITMAP* canvas;

   /* needs to be called at beginning */
   FreeImage_Initialise(false);

   /* skipping robust checking because 2 args */
   if(argc != NEEDED_ARGS) {
      fprintf(stderr, "Use: ./spritepacker inputFolder outputName\n");
      exit(EXIT_FAILURE);
   }

   /* count_files will be used to make an array of bmp_infos,
      which will contain various info about each bmp in the folder
      */
   numFiles = count_files(argv[DIRECTORY]);
   populate_bmp_info(&bmps, argv[DIRECTORY], numFiles);

   /*sort it. s-sort it reaaaaaal good */
   qsort(bmps, numFiles, sizeof(bmp_info), comp);
   calc_optimal_width_and_height(bmps, numFiles, &optimalWidth, &optimalHeight);
   make_packed_sprite(&canvas, bmps, numFiles, optimalWidth, optimalHeight, argv[OUTPUTFILE]);
   /* make_horizontal_sprite(&canvas, bmps, numFiles, argv[OUTPUTFILE]);*/


   FreeImage_DeInitialise();
   return 0;
}
