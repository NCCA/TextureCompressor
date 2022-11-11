#ifndef NGLSCENE_H__
#define NGLSCENE_H__
#include <ngl/Text.h>
#include <map>
#include "ScreenQuad.h"
#include "TexturePack.h"
#include <QOpenGLWindow>

#include <memory>

//----------------------------------------------------------------------------------------------------------------------
/// @file NGLScene.h
/// @brief this class inherits from the Qt OpenGLWindow and allows us to use NGL to draw OpenGL
/// @author Jonathan Macey
/// @version 1.0
/// @date 10/9/13
/// Revision History :
/// This is an initial version used for the new NGL6 / Qt 5 demos
/// @class NGLScene
/// @brief our main glwindow widget for NGL applications all drawing elements are
/// put in this file
//----------------------------------------------------------------------------------------------------------------------

class NGLScene : public QOpenGLWindow
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor for our NGL drawing class
  /// @param [in] parent the parent window to the class
  //----------------------------------------------------------------------------------------------------------------------
  NGLScene(const std::string &_fname);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief dtor must close down ngl and release OpenGL resources
  //----------------------------------------------------------------------------------------------------------------------
  ~NGLScene();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the initialize class is called once when the window is created and we have a valid GL context
  /// use this to setup any default GL stuff
  //----------------------------------------------------------------------------------------------------------------------
  void initializeGL();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this is called everytime we want to draw the scene
  //----------------------------------------------------------------------------------------------------------------------
  void paintGL();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Qt Event called when the window is re-sized
  /// @param [in] _event the Qt event to query for size etc
  //----------------------------------------------------------------------------------------------------------------------
  // Qt 5.5.1 must have this implemented and uses it
  void resizeGL(QResizeEvent *_event);
  // Qt 5.x uses this instead! http://doc.qt.io/qt-5/qopenglwindow.html#resizeGL
  void resizeGL(int _w, int _h);

private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Qt Event called when a key is pressed
  /// @param [in] _event the Qt event to query for size etc
  //----------------------------------------------------------------------------------------------------------------------
  void keyPressEvent(QKeyEvent *_event);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called every time a mouse is moved
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void mouseMoveEvent(QMouseEvent *_event);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called everytime the mouse button is pressed
  /// inherited from QObject and overridden here.
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void mousePressEvent(QMouseEvent *_event);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called everytime the mouse button is released
  /// inherited from QObject and overridden here.
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void mouseReleaseEvent(QMouseEvent *_event);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called everytime the mouse wheel is moved
  /// inherited from QObject and overridden here.
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void wheelEvent(QWheelEvent *_event);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief load new pack file
  //----------------------------------------------------------------------------------------------------------------------
  void reload();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief append to the current loaded textures
  //----------------------------------------------------------------------------------------------------------------------
  void append();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief name of texture to load (from command line)
  //----------------------------------------------------------------------------------------------------------------------
  std::string m_filename;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief full screen quad for rendering texture
  //----------------------------------------------------------------------------------------------------------------------
  std::unique_ptr<ScreenQuad> m_screenQuad;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief our packed texture (is a singleton class)
  //----------------------------------------------------------------------------------------------------------------------
  TexturePack *m_texturePack;
  //----------------------------------------------------------------------------------------------------------------------
  // pointer to the current texture in the pack
  //----------------------------------------------------------------------------------------------------------------------
  std::map<std::string, GLuint>::const_iterator m_currentTex;
  int m_width;
  int m_height;
};

#endif
