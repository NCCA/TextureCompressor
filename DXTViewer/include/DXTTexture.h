#ifndef DXTTTEXTURE_H__
#define DXTTTEXTURE_H__
#include <ngl/Types.h>
#include <ngl/Texture.h>
class DXTTexture
{
  public :
    enum COMPRESSION{DXT1,DXT3,DXT5};

    DXTTexture(): m_width(0),m_height(0),m_data(0),m_dataSize(0),m_textID(0){;}
    DXTTexture(const std::string &_name);
    DXTTexture(const ngl::Texture &_t);
    bool load(const std::string &_name);
    void save(const std::string &_name);
    GLuint getTexID()const {return m_textID;}
    int width()const {return m_width;}
    int height()const {return m_height;}

    ~DXTTexture();
  private :
    int m_width;
    int m_height;
    GLubyte *m_data;
    unsigned int m_dataSize;
    GLuint m_textID;

};


#endif
