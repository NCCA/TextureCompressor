#TextureCompressor

A simple command line tool to create basic DxT1,3,5 textures to be used with glCompressedTexImage2D(https://www.khronos.org/opengles/sdk/docs/man/xhtml/glCompressedTexImage2D.xml) formats.
This tool takes command line arguments to change the different compressions and will do all valid file files it finds.
It requires linking to [SDL2](https://www.libsdl.org/download-2.0.php)[SDL2_image](https://www.libsdl.org/projects/SDL_image/) and the [squish](https://code.google.com/p/libsquish/) library.

Minimal error checking is done so it may not always work.
