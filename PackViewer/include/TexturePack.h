#ifndef TEXTUREPACK_H__
#define TEXTUREPACK_H__

#include <ngl/Types.h>
#include <map>
#include <string>


class TexturePack
{
  public :
    TexturePack() : m_numTextures(0){;}
    TexturePack(const std::string &_fname);
    ~TexturePack();
    bool load(const std::string &_fname);
    void clear();
    GLuint getTexture(const std::string &_name);
    void bind(const std::string &_fname);
    void unbind();
    void append(const std::string &_name);
    inline unsigned int getNumTextures() const {return m_numTextures;}
    std::map<std::string,GLuint>::const_iterator begin()const { return m_textures.begin();}
    std::map<std::string,GLuint>::const_iterator end()const { return m_textures.end();}

  private :

    std::map<std::string,GLuint> m_textures;
    unsigned int m_numTextures;
    enum COMPRESSION{DXT1,DXT3,DXT5};

};




#endif
