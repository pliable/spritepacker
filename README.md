spritepacker
============

A sprite packer, a packer of sprites

Based on this algorithm: http://www.sitioweb.fr/blog/3-technical/3-sprite-packing-algorithm

This uses the FreeImage library, found here: http://freeimage.sourceforge.net/

5/3/14: Actually usable to an extent; will pack images in one big horizontal row. Next step is implementing algorithm
        using FreeImage

Usage: ./spacker [image_directory] [output_name].bmp

This is going to remain how it is (for now, I'll go back and tackle this later and probably try to get glib working). There are too many underlying implementation issues that would be much easier dealt with with a higher level language like Java. Gonna start rewrite in Java ASAP....
