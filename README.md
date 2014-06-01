spritepacker
============

A sprite packer, a packer of sprites

Was going to be based on this algorithm: http://www.sitioweb.fr/blog/3-technical/3-sprite-packing-algorithm

But the dudes English is horrible and I don't understand it at all (especially since I've barely worked with graphics),
so I've instead implemented a modified version from a buddy.

This uses the FreeImage library, found here: http://freeimage.sourceforge.net/

Needs to be linked in makefile

Usage: ./spacker [image_directory] [output_name].bmp

Modified algorithm courtesy of my buddy John:
1. you figure out the image size first
2. then you sort your images by height descending
3. the first row's height is the height of the first image
4. keep adding images in that row until they don't fit anymore
      they're all guaranteed to fit in that height
      because of the sort
5. then when you can't fit anymore
    you add a new row with the height of the next image
    and repeat
6. until everything is in the image

5/31/14: Fixes done, packer done, now just need to rewrite and clean and optimize
