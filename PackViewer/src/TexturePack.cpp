#include "TexturePack.h"
#include <fstream>
#include <iostream>
#include <ngl/Util.h>

TexturePack* TexturePack::s_instance=0;

TexturePack *TexturePack::instance()
{
  if(s_instance ==0)
  {
    s_instance= new TexturePack;
  }
  return s_instance;
}


bool TexturePack::load(const std::string &_fname)
{
  std::ifstream fileIn;
  fileIn.open(_fname.c_str(),std::ios::in | std::ios::binary);
  if (!fileIn.is_open())
  {
    std::cout <<"File : "<<_fname<<" could not be opened for reading"<<std::endl;
    return false;
  }
  char header[17];
  fileIn.read(header,16*sizeof(char));
  header[16]=0; // for strcmp we need \n
  // basically I used the magick string ngl::bin (I presume unique in files!) and
  // we test against it.
  if(strcmp(header,"ngl::packtexture"))
  {
    // best close the file and exit
    fileIn.close();
    std::cout<<"this is not an ngl::packtexture file "<<std::endl;
    return false;
  }
  fileIn.read(reinterpret_cast<char *>(&m_numTextures),sizeof(m_numTextures));
  std::cout<<"found "<<m_numTextures<<" textures in the file\n";
  for(unsigned int t=0; t<m_numTextures; ++t)
  {
    // length of file name string
    int stringSize;
    fileIn.read(reinterpret_cast<char *>(&stringSize),sizeof(stringSize));
    // now the string we first need to allocate space then copy in
    std::string textureName;
    textureName.resize(stringSize);
    fileIn.read(reinterpret_cast<char *>(&textureName[0]),stringSize);
    std::cout<<"loading "<<textureName<<"\n";
    int width;
    fileIn.read(reinterpret_cast<char *>(&width),sizeof(width));
    int height;
    fileIn.read(reinterpret_cast<char *>(&height),sizeof(height));
    GLenum internalFormat;
    fileIn.read(reinterpret_cast<char *>(&internalFormat),sizeof(internalFormat));
    // sizeof(enum) compression enum for type
    COMPRESSION comp;
    fileIn.read(reinterpret_cast<char *>(&comp),sizeof(COMPRESSION));
    // file name
    // sizeof(int) size of data
    int dataSize;
    fileIn.read(reinterpret_cast<char *>(&dataSize),sizeof(dataSize));
    // raw compressed data unsigned char[dataSize]
    unsigned char dxtData[dataSize];
    fileIn.read(reinterpret_cast<char *>(&dxtData),dataSize);
    // now create texture and load to GL
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    // use simple format for now can always change later
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glCompressedTexImage2D(GL_TEXTURE_2D, 0,  internalFormat, width, height, 0,  dataSize,&dxtData[0]);

    glGenerateMipmap(GL_TEXTURE_2D);
    m_textures[textureName]=texID;
    // make sure OpenGL was happy
    ngl::NGLCheckGLError("DXTTexture creation",__LINE__);

  }


  fileIn.close();

  return true;
}

void TexturePack::append(const std::string &_name)
{
  load(_name);
}

void TexturePack::clear()
{
  // create an iterator for our map.
  // not the iterator gives us two components .first .second which lets us access the key,object elements
  std::map<std::string,GLuint>::iterator it;
  // grab an iterator to the end to make loop quicker
  std::map<std::string,GLuint>::iterator end=m_textures.end();
  for( it=m_textures.begin() ; it != end; ++it )
  {
    GLuint id=(*it).second;
    glDeleteTextures(1,&id);
  }

}

GLuint TexturePack::getTexture(const std::string &_name)
{
  std::map <std::string, GLuint >::const_iterator tex=m_textures.find(_name);
  // make sure we have a valid shader
  if(tex!=m_textures.end())
  {
    return tex->second;
  }
  else
    return 0;
}

void TexturePack::bind(const std::string &_name)
{
  std::map <std::string, GLuint >::const_iterator tex=m_textures.find(_name);
  // make sure we have a valid shader
  if(tex!=m_textures.end())
  {
    glBindTexture(GL_TEXTURE_2D, tex->second);
  }
  else
  {
    glBindTexture(GL_TEXTURE_2D,0);

  }

}

void TexturePack::unbind()
{
  glBindTexture(GL_TEXTURE_2D,0);

}
