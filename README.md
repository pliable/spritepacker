spritepacker
============

A sprite packer, a packer of sprites

Based on this algorithm: http://www.sitioweb.fr/blog/3-technical/3-sprite-packing-algorithm

This uses the FreeImage library, found here: http://freeimage.sourceforge.net/

5/3/14: Actually usable to an extent; will pack images in one big horizontal row. Next step is implementing algorithm
        using FreeImage

Usage: ./spacker [image_directory] [output_name].bmp

This is going to remain how it is (for now, I'll go back and tackle this later and probably try to get glib working). There are too many underlying implementation issues that would be much easier dealt with with a higher level language like Java. Gonna start rewrite in Java ASAP....

5/25/14: My buddy John translated the french algorithm for me in his own words and made it a whollllllllllllllllle lot clearer, to the point where I think I can modify the horizontal packer to the one in the article.

Clarified algorithm courtesy of John:
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
