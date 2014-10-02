#include "DXTTexture.h"
#include <ngl/Util.h>
#include <fstream>
#include <iostream>
#include <cstring>
DXTTexture::~DXTTexture()
{
  reset();
}

void DXTTexture::reset()
{
  // clean up textures
  glDeleteTextures(1,&m_texID);
  m_width=0;
  m_height=0;
  m_texID=0;
  m_dataSize=0;
}


DXTTexture::DXTTexture(const std::string &_name)
{
  load(_name);
}


bool DXTTexture::load(const std::string &_name)
{
  std::ifstream fileIn;

  fileIn.open(_name.c_str(),std::ios::in | std::ios::binary);

  char header[11];
  fileIn.read(header,10*sizeof(char));
  header[10]=0; // for strcmp we need \n
  // basically I used the magick string ngl::cmptx (I presume unique in files!) and
  // we test against it.
  if(strcmp(header,"ngl::cmptx"))
  {
    // best close the file and exit
    fileIn.close();
    std::cout<<"this is not an ngl::cmptx file "<<std::endl;
    return false;
  }
  // data is in the format
  // 10 bytes ngl::cmptx
  // sizeof(int) width
  // sizeof(int) height
  // sizeof(GLenum) internalformat
  // sizeof(enum) compression enum for type
  // sizeof(int) size of data
  // raw compressed data unsigned char[size]

  fileIn.read(reinterpret_cast<char *>(&m_width),sizeof( int));
  fileIn.read(reinterpret_cast<char *>(&m_height),sizeof( int));
  GLenum internalFormat;
  fileIn.read(reinterpret_cast<char *>(&internalFormat),sizeof(GLenum));
  COMPRESSION format;
  fileIn.read(reinterpret_cast<char *>(&format),sizeof(COMPRESSION));
  std::cout<<"iFormat "<<internalFormat<<" "<<format<<"\n";

  fileIn.read(reinterpret_cast <char *>(&m_dataSize),sizeof(unsigned int));
  GLubyte *data = new GLubyte[m_dataSize];
  std::cout<<"data size "<<m_dataSize<<"\n";
  fileIn.read(reinterpret_cast<char *>(&data[0]),m_dataSize);


  fileIn.close();

  // now create texture and load to GL
  glGenTextures(1, &m_texID);
  glBindTexture(GL_TEXTURE_2D, m_texID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

  // use simple format for now can always change later
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glCompressedTexImage2D(GL_TEXTURE_2D, 0,  internalFormat, m_width, m_height, 0,  m_dataSize,&data[0]);


  glGenerateMipmap(GL_TEXTURE_2D);
  // make sure OpenGL was happy
  ngl::NGLCheckGLError("DXTTexture creation",__LINE__);
  // done with the data now so delete
  delete [] data;
  return true;
}


