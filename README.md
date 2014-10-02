#Texture Compression
![alt tag](http://nccastaff.bournemouth.ac.uk/jmacey/GraphicsLib/Demos/TextureComp.png)

A series of demos to show how to create and then use [DXT](http://en.wikipedia.org/wiki/S3_Texture_Compression) compressed textures.

[Compressor](https://github.com/NCCA/TextureCompressor/tree/master/Compressor) Is a simple command line tool to create basic DxT1,3,5 textures to be used with [glCompressedTexImage2D](https://www.khronos.org/opengles/sdk/docs/man/xhtml/glCompressedTexImage2D.xml) formats.
This tool takes command line arguments to change the different compressions and will do all valid file files it finds.
It requires linking to [SDL2](https://www.libsdl.org/download-2.0.php) [SDL2_image](https://www.libsdl.org/projects/SDL_image/) and the [squish](https://code.google.com/p/libsquish/) library.

Minimal error checking is done so it may not always work.

[DXTViewer](https://github.com/NCCA/TextureCompressor/tree/master/DXTViewer) is a simple tool to load in the compressed files and view them

##Todo
-Write a file format to compress multiple textures into one packed file
-Write a demo to show the compress textures in action
-Use it in the sponza demos