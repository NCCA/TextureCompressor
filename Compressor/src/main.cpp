#include <SDL.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <ngl/Types.h>
#include <SDL_image.h>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include "pystring.h"
#include <squish.h>

void createCompressedTexture(SDL_Surface *_surface, std::string _output);
// command line arguments h (help) c[1,2,3] compression values
// e [name] extension v verbose
const static char *ARGUMENTS="hc:e:v";
// enum for different texture types
enum COMPRESSION{DXT1,DXT3,DXT5};

// as this is a simple tool lets use some globals
// command line args can modify the following
COMPRESSION g_comp=DXT1;
// default extension
std::string g_ext("cmp");
// compression type for libsquish
int g_scomp=squish::kDxt1;
// use verbose mode
bool g_verbose=false;

int main(int argc, char **argv)
{
  // the character returned from the getopt function
  char c;

  // now loop and parse the command line options

  while( (c=getopt(argc,argv,ARGUMENTS)) !=EOF)
  {
    switch(c) // which option has been chosen
    {
      case 'h' :
        std::cout<<"TextureCompressor [filename(s)] for default DXT1 compression\n";
        std::cout<<"-e [name] to change extension from default .cmp\n";
        std::cout<<"-c [1][3][5] for DxT1 DxT3 or Dxt5 compression \n";
        std::cout<<"-v verbose \n";
        std::cout<<"it will process all files on command line\n";
        exit(EXIT_SUCCESS);

      case 'e' :
         g_ext=optarg;
      break;
      case 'v' :
        g_verbose=true;
      break;

      case '?' : // unknown option report this and exit
        // where optopt is the current option
        std::cout<<"Unknown argument "<<optopt<<std::endl;
        exit(EXIT_FAILURE);
      break;

      case 'c' :
        if(optarg[0]=='1')
        {
          // this is the default but set it anyway
          g_comp=DXT1;
          g_scomp=squish::kDxt1;
        }
        else if(optarg[0]=='3')
        {
          g_comp=DXT3;
          g_scomp=squish::kDxt3;
        }
        else if(optarg[0]=='5')
        {
          g_comp=DXT5;
          g_scomp=squish::kDxt5;
        }

       break;

    }

  }

  // Initialize SDL's Video subsystem
  if (SDL_Init(SDL_INIT_VIDEO) < 0 )
  {
    // Or die on error
    std::cerr<<"Problem with SDL\n";
    std::exit(EXIT_FAILURE);
  }
  // image data to load
  SDL_Surface *image;

  for(int file=optind; file<argc; ++file)
  {
    // load current file and see if it is ok
    image=IMG_Load(argv[file]);
    if(!image)
    {
      std::cerr<<"Problem loading "<<argv[file]<<" " <<IMG_GetError()<<"\n";
      continue;
    }
    // now we split the file and extract the extension
    // using pystring as it is easy and quick
    std::vector <std::string> tokens;
    // split the file into tokens based on the .
    pystring::split(argv[file],tokens,".");
    // make the last token the new extension
    tokens[tokens.size()-1]=g_ext;
    // create a new string joining the tokens with a .
    std::string outputString = pystring::join(".", tokens);
    if(g_verbose)
    {
      std::cout<<"saving to "<<outputString<<"\n";
    }
    // now compress.
    createCompressedTexture(image,outputString);
    // and free the image
    SDL_free(image);
  }
  // now tidy up and exit SDL
 SDL_Quit();
}



void createCompressedTexture(SDL_Surface *_surface, std::string _output)
{
  // internal format used by OpenGL
  GLenum internalFormat;
  if(g_comp==squish::kDxt1)
  {
    internalFormat=GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
  }
  else if(g_comp==squish::kDxt3)
  {
    internalFormat=GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
  }
  else if(g_comp==squish::kDxt5)
  {
    internalFormat=GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
  }

  // surface used to store data
  SDL_Surface* s = _surface;
  // find what we need to store the data
  int size = squish::GetStorageRequirements(s->w, s->h, g_scomp);
  // make space for it.
  unsigned char dxtData[size];
  if(g_verbose)
  {
    std::cout<<"W/H "<<s->w<<" "<<s->h<<"\n";
    std::cout << "Beginning compression" << std::endl;
    std::cout<<"data size "<<size<<"\n";
  }
  // now compress
  squish::CompressImage((unsigned char *)s->pixels, s->w, s->h, dxtData, g_scomp | squish::kColourIterativeClusterFit);
  if(g_verbose)
  {
    std::cout << "Completed compression" << std::endl;
  }
  std::ofstream fileOut;
  // and write out
  // 10 bytes ngl::cmptx
  // sizeof(int) width
  // sizeof(int) height
  // sizeof(GLenum) internalformat
  // sizeof(enum) compression enum for type
  // sizeof(int) size of data
  // raw compressed data unsigned char[size]
  fileOut.open(_output.c_str(),std::ios::out | std::ios::binary);
  const std::string header("ngl::cmptx");
  fileOut.write(header.c_str(),header.length());
  fileOut.write(reinterpret_cast<char *>(&s->w),sizeof(s->w));
  fileOut.write(reinterpret_cast<char *>(&s->h),sizeof(s->h));
  fileOut.write(reinterpret_cast<char *>(&internalFormat),sizeof(internalFormat));
  fileOut.write(reinterpret_cast<char *>(&g_comp),sizeof(COMPRESSION));
  // data size
  fileOut.write(reinterpret_cast <char *>(&size),sizeof(int));
  fileOut.write(reinterpret_cast<char *>(dxtData),size);
  // close file
  fileOut.close();

}

