#include <SDL.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <ngl/NGLInit.h>
#include <SDL_image.h>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include "pystring.h"
#include <squish.h>

void createCompressedTexture(SDL_Surface *_surface, std::string _output);

const static char *ARGUMENTS="hc:e:";

enum COMPRESSION{DXT1,DXT3,DXT5};

// as this is a simple tool lets use some globals
// command line args can modify the following
COMPRESSION g_comp=DXT1;
// default extension
std::string g_ext("cmp");
// compression type for libsquish
int g_scomp=squish::kDxt1;

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
        std::cout<<"TextureCompressor [filename] for default DXT1 compression\n";
        std::cout<<"-e [name] to change extension from default .cmp\n";
        std::cout<<"-c [1][3][5] for DxT1 DxT3 or Dxt5 compression \n";
        exit(EXIT_SUCCESS);

      case 'e' :
         g_ext=optarg;
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
  SDL_Surface *image;

  for(int file=optind; file<argc; ++file)
  {

    image=IMG_Load(argv[file]);
    if(!image)
    {
      std::cerr<<"Problem loading "<<argv[file]<<" " <<IMG_GetError()<<"\n";
      continue;
    }

    std::vector <std::string> tokens;
    pystring::split(argv[file],tokens,".");
    tokens[tokens.size()-1]=g_ext;
    std::string outputString = pystring::join(".", tokens);
    std::cout<<"saving to "<<outputString<<"\n";
    //GLuint id=
    createCompressedTexture(image,outputString);

    SDL_free(image);
  }
  // now tidy up and exit SDL
 SDL_Quit();
}



void createCompressedTexture(SDL_Surface *_surface, std::string _output)
{
  GLuint textureId;
  GLenum internalFormat;
  SDL_Surface* s = _surface;
  std::cout<<"W/H "<<s->w<<" "<<s->h<<"\n";

  std::cout << "Beginning compression" << std::endl;
  int size = squish::GetStorageRequirements(s->w, s->h, g_scomp);
  std::cout<<"data size "<<size<<"\n";

  unsigned char dxtData[size];

  squish::CompressImage((unsigned char *)s->pixels, s->w, s->h, dxtData, g_scomp | squish::kColourIterativeClusterFit);
  std::cout << "Completed compression" << std::endl;
  std::ofstream fileOut;

  fileOut.open(_output.c_str(),std::ios::out | std::ios::binary);
  const std::string header("ngl::cmptx");
  fileOut.write(header.c_str(),header.length());
  fileOut.write(reinterpret_cast<char *>(&s->w),sizeof(s->w));
  fileOut.write(reinterpret_cast<char *>(&s->h),sizeof(s->h));
  fileOut.write(reinterpret_cast<char *>(&internalFormat),sizeof(internalFormat));
  fileOut.write(reinterpret_cast<char *>(&g_comp),sizeof(COMPRESSION));
  // data size
  fileOut.write(reinterpret_cast <char *>(&size),sizeof(int));
  std::cout<<(int)size<<"\n";
  fileOut.write(reinterpret_cast<char *>(dxtData),size);


   fileOut.close();

}

