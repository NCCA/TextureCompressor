#ifndef DXTTTEXTURE_H__
#define DXTTTEXTURE_H__
#include <ngl/Types.h>
#include <iostream>

/// @brief simple class to load a Compressed DXTTextue created with the
/// texture tool

class DXTTexture
{
public:
  /// @brief enums for the texture tyes
  enum COMPRESSION
  {
    DXT1,
    DXT3,
    DXT5
  };
  /// @brief default ctor set all to zero
  DXTTexture() : m_width(0), m_height(0), m_dataSize(0), m_texID(0) { ; }
  /// @brief  ctor passing in filename
  /// @param _name the name of the file to load from disk
  DXTTexture(const std::string &_name);
  /// @brief load the file
  /// @param _name name of thefile to load
  /// @returns true if succesfull load.
  bool load(const std::string &_name);
  /// @brief get the OpenGL texture ID
  /// @returns m_textID the texture ID for the compressed texture
  inline GLuint getTexID() const { return m_texID; }
  /// @brief width of loaded texture
  /// @returns width
  inline int width() const { return m_width; }
  /// @brief height of loaded texture
  /// @returns height
  int height() const { return m_height; }
  /// @brief size in bytes of the texture loaded
  /// @returns size
  int size() const { return m_dataSize; }
  /// @brief reset all of the internal data
  void reset();
  /// @brief dtor will clear the GL texture
  ~DXTTexture();

private:
  /// @brief the width of the texture loaded
  int m_width;
  /// @brief the height of the texture loaded
  int m_height;
  /// @brief the size of data stored of the texture loaded
  unsigned int m_dataSize;
  /// @brief the OpenGL texture ID
  GLuint m_texID;
};

#endif
