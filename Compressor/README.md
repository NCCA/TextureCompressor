#TextureCompressor

A simple command line tool to create basic DxT1,3,5 textures to be used with glCompressedTexImage2D(https://www.khronos.org/opengles/sdk/docs/man/xhtml/glCompressedTexImage2D.xml) formats.
This tool takes command line arguments to change the different compressions and will do all valid file files it finds.
It requires linking to [SDL2](https://www.libsdl.org/download-2.0.php)[SDL2_image](https://www.libsdl.org/projects/SDL_image/) and the [squish](https://code.google.com/p/libsquish/) library.

Minimal error checking is done so it may not always work.

##File format
If you want to write your own loader the file format is as follows and can be found in [this](https://github.com/NCCA/TextureCompressor/blob/master/DXTViewer/src/DXTTexture.cpp) file as well
```c++
// 10 bytes ngl::cmptx
// sizeof(int) width
// sizeof(int) height
// sizeof(GLenum) internalformat
// sizeof(enum) compression enum for type
// sizeof(int) size of data
// raw compressed data unsigned char[size]
enum COMPRESSION{DXT1,DXT3,DXT5};
std::string name;
int width;
int height;
// internal format is GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
// GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
// GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
GLenum internalFormat;
COMPRESSION compression;
int size;
unsigned char data[size];
```

