#include "DXTTexture.h"
#include <ngl/Util.h>
#include <fstream>
#include <iostream>
DXTTexture::~DXTTexture()
{

}

DXTTexture::DXTTexture(const std::string &_name)
{
  load(_name);
}

DXTTexture::DXTTexture(const ngl::Texture &_t)
{

}

bool DXTTexture::load(const std::string &_name)
{
  std::ifstream fileIn;

  fileIn.open(_name.c_str(),std::ios::in | std::ios::binary);
  char header[11];
  fileIn.read(header,10*sizeof(char));
  header[10]=0; // for strcmp we need \n
  // basically I used the magick string ngl::bin (I presume unique in files!) and
  // we test against it.
  if(strcmp(header,"ngl::cmptx"))
  {
    // best close the file and exit
    fileIn.close();
    std::cout<<"this is not an ngl::cmptx file "<<std::endl;
    return false;
  }

  fileIn.read(reinterpret_cast<char *>(&m_width),sizeof( int));
  fileIn.read(reinterpret_cast<char *>(&m_height),sizeof( int));
  std::cout<<"W/H "<<m_width<<" "<<m_height<<"\n";
  int internalFormat;
  fileIn.read(reinterpret_cast<char *>(&internalFormat),sizeof(internalFormat));
  COMPRESSION format;
  fileIn.read(reinterpret_cast<char *>(&format),sizeof(COMPRESSION));
  std::cout<<"iFormat "<<internalFormat<<" "<<format<<"\n";

  fileIn.read(reinterpret_cast <char *>(&m_dataSize),sizeof(unsigned int));
  m_data = new GLubyte[m_dataSize];
  std::cout<<"data size "<<m_dataSize<<"\n";
  fileIn.read(reinterpret_cast<char *>(&m_data[0]),m_dataSize);


fileIn.close();


glGenTextures(1, &m_textID);
glBindTexture(GL_TEXTURE_2D, m_textID);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);


glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
if(format == DXT1)
  glCompressedTexImage2D(GL_TEXTURE_2D, 0,  GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, m_width, m_height, 0,  m_dataSize,&m_data[0]);
else if(format == DXT3)
glCompressedTexImage2D(GL_TEXTURE_2D, 0,  GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, m_width, m_height, 0,  m_dataSize,&m_data[0]);
else if(format == DXT5)
glCompressedTexImage2D(GL_TEXTURE_2D, 0,  GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, m_width, m_height, 0,  m_dataSize,&m_data[0]);


glGenerateMipmap(GL_TEXTURE_2D);
ngl::NGLCheckGLError("DXTTexture creation",__LINE__);


}

void DXTTexture::save(const std::string &_name)
{

}
