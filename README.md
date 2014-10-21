#Texture Compression
![alt tag](http://nccastaff.bournemouth.ac.uk/jmacey/GraphicsLib/Demos/TextureComp.png)

A series of demos to show how to create and then use [DXT](http://en.wikipedia.org/wiki/S3_Texture_Compression) compressed textures.

Note these may not work on some linux drivers (nouveau / Mesa3D as compressed texture support doesn't seem to work)

[Compressor](https://github.com/NCCA/TextureCompressor/tree/master/Compressor) Is a simple command line tool to create basic DxT1,3,5 textures to be used with [glCompressedTexImage2D](https://www.khronos.org/opengles/sdk/docs/man/xhtml/glCompressedTexImage2D.xml) formats.

This tool takes command line arguments to change the different compressions and will attempt to convert and save all valid image files it is passed on the command line.

It requires linking to [SDL2](https://www.libsdl.org/download-2.0.php) [SDL2_image](https://www.libsdl.org/projects/SDL_image/) and the [squish](https://code.google.com/p/libsquish/) library.

Minimal error checking is done so it may not always work.

[DXTViewer](https://github.com/NCCA/TextureCompressor/tree/master/DXTViewer) is a simple tool to load in the compressed files and view them. You can specify the name of the file on the command line or use the file open dialog presented at startup. Use o to reload a file.

[Compressor](https://github.com/NCCA/TextureCompressor/tree/master/Compressor) is a tool to compress a series of texture files into one packed texture file. The command line options are similar to the TetureCompressor program but has the option to specify the output.

[PackTool](https://github.com/NCCA/TextureCompressor/tree/master/PackTool) is a tool to compress a series of texture files into one packed texture file. The command line options are similar to the TetureCompressor program but has the option to specify the output.

[PackViewer](https://github.com/NCCA/TextureCompressor/tree/master/PackViewer) is a tool to view the packed texture files and also contains the essential TexturePack class which may be used in your own projects to load in the packed textures.



