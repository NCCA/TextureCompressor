#ifndef SCREEN_QUAD_H__
#define SCREEN_QUAD_H__
#include <ngl/Vec2.h>
#include <ngl/Vec3.h>
#include <string>
class ScreenQuad
{
  public :
  /// @brief ctor, note this will scale to any screen size so
  /// the width and height are for the texture image passed in
    ScreenQuad(const std::string &_shader );
    ~ScreenQuad();
    void draw();

  private :
    GLuint m_vao;
    std::string m_shader;
    int m_windowW;
    int m_windowH;



};




#endif
