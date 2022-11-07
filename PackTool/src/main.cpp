#include <SDL.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <ngl/Types.h>
#include <SDL_image.h>
#include <ngl/pystring.h>
#include <squish.h>
#include <cxxopts.hpp>

void createCompressedTexture(std::ofstream &_file, SDL_Surface *_surface, std::string _output);
// command line arguments h (help) c[1,2,3] compression values
// e [name] extension v verbose
// -o output name for packed texture
const static char *ARGUMENTS = "hc:e:vo:";
// enum for different texture types
enum COMPRESSION
{
  DXT1,
  DXT3,
  DXT5
};

// as this is a simple tool lets use some globals
// command line args can modify the following
COMPRESSION g_comp = DXT1;
// default extension
std::string g_ext("cmp");
// compression type for libsquish
int g_scomp = squish::kDxt1;
// use verbose mode
bool g_verbose = false;
// default pack name
std::string g_output = "texture.pack";

int main(int argc, char **argv)
{
  // clang-format off
  cxxopts::Options options("Compressor", "DXT texture compressor");
  options.add_options()("e,extension", "The extension to use for the output file Defauly .cmp", cxxopts::value<std::string>()->default_value("cmp"));
  
  options.add_options()("c,compression", "The compression values 1,3,5 for DxT1 DxT3 or Dxt5", cxxopts::value<int>()->default_value("1"));
  
  options.add_options()("v,verbose", "Verbose mode on or off", cxxopts::value<bool>());
  
  options.add_options()("u,help", "Print usage");
  // clang-format on

  auto results = options.parse(argc, argv);
  if (results.count("help"))
  {
    std::cout << options.help() << '\n';
    exit(EXIT_FAILURE);
  }

  // the character returned from the getopt function
  g_ext = results["extension"].as<std::string>();
  g_verbose = results["verbose"].as<bool>();
  auto mode = results["compression"].as<int>();
  if (mode == '1')
  {
    // this is the default but set it anyway
    g_comp = DXT1;
    g_scomp = squish::kDxt1;
  }
  else if (mode == '3')
  {
    g_comp = DXT3;
    g_scomp = squish::kDxt3;
  }
  else if (mode == '5')
  {
    g_comp = DXT5;
    g_scomp = squish::kDxt5;
  }
  // Initialize SDL's Video subsystem
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    // Or die on error
    std::cerr << "Problem with SDL\n";
    std::exit(EXIT_FAILURE);
  }
  // image data to load
  SDL_Surface *image;

  std::ofstream fileOut;
  fileOut.open(g_output.c_str(), std::ios::out | std::ios::binary);
  const std::string header("ngl::packtexture");
  fileOut.write(header.c_str(), header.length());
  int numFiles = 0;
  // need to store this position for later so we can write out
  // the actual number of files packed.
  int numFileLocation = fileOut.tellp();
  // now write out dummy size;
  fileOut.write(reinterpret_cast<char *>(&numFiles), sizeof(int));
  auto arguments = results.arguments();
  for (const auto &file : results.unmatched())
  {
    // load current file and see if it is ok
    image = IMG_Load(file.c_str());
    if (!image)
    {
      std::cerr << "Problem loading " << file << " " << IMG_GetError() << '\n';
      continue;
    }
    // now compress.
    createCompressedTexture(fileOut, image, file);
    // and free the image
    SDL_free(image);
    ++numFiles;
  }
  fileOut.seekp(numFileLocation, std::ios_base::beg);
  fileOut.write(reinterpret_cast<char *>(&numFiles), sizeof(int));

  // close file
  fileOut.close();

  // now tidy up and exit SDL
  SDL_Quit();
}

void createCompressedTexture(std::ofstream &_file, SDL_Surface *_surface, std::string _output)
{
  // internal format used by OpenGL
  GLenum internalFormat;
  if (g_comp == DXT1)
  {
    internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
  }
  else if (g_comp == DXT3)
  {
    internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
  }
  else if (g_comp == DXT5)
  {
    internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
  }

  // surface used to store data
  SDL_Surface *s = _surface;
  // find what we need to store the data
  int size = squish::GetStorageRequirements(s->w, s->h, g_scomp);
  // make space for it.
  unsigned char dxtData[size];
  if (g_verbose)
  {
    std::cout << "W/H " << s->w << " " << s->h << "\n";
    std::cout << "Beginning compression" << std::endl;
    std::cout << "data size " << size << "\n";
  }
  // now compress
  squish::CompressImage((unsigned char *)s->pixels, s->w, s->h, dxtData, g_scomp | squish::kColourIterativeClusterFit);
  if (g_verbose)
  {
    std::cout << "Completed compression" << std::endl;
  }
  // and write out
  // length of file name string
  // file name
  // sizeof(int) width
  // sizeof(int) height
  // sizeof(GLenum) internalformat
  // sizeof(enum) compression enum for type
  // sizeof(int) size of data
  // raw compressed data unsigned char[size]
  int len = _output.length();
  _file.write(reinterpret_cast<char *>(&len), sizeof(int));
  _file.write(_output.c_str(), _output.length());
  _file.write(reinterpret_cast<char *>(&s->w), sizeof(s->w));
  _file.write(reinterpret_cast<char *>(&s->h), sizeof(s->h));
  _file.write(reinterpret_cast<char *>(&internalFormat), sizeof(internalFormat));
  _file.write(reinterpret_cast<char *>(&g_comp), sizeof(COMPRESSION));
  // data size
  _file.write(reinterpret_cast<char *>(&size), sizeof(int));
  _file.write(reinterpret_cast<char *>(dxtData), size);
}
