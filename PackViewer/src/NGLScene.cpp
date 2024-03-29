#include <QFileDialog>
#include <QMouseEvent>
#include <QGuiApplication>
#include <iostream>
#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>

NGLScene::NGLScene(const std::string &_fname)
{
  // store filename for loading
  m_filename = _fname;
  setTitle(QString("DXT Viewer use o to load new file current %1").arg(m_filename.c_str()));
}

NGLScene::~NGLScene()
{
  std::cout << "Shutting down NGL, removing VAO's and Shaders\n";
  m_texturePack->instance()->clear();
}

void NGLScene::resizeGL(int _w, int _h)
{
  m_width = _w * devicePixelRatio();
  m_height = _h * devicePixelRatio();
}

void NGLScene::resizeGL(QResizeEvent *_event)
{
  int w = _event->size().width();
  int h = _event->size().height();
  m_width = w * devicePixelRatio();
  m_height = h * devicePixelRatio();
}

void NGLScene::initializeGL()
{
  // we must call this first before any other GL commands to load and link the
  // gl commands from the lib, if this is not done program will crash
  ngl::NGLInit::initialize();
  // create a simple screen quad for drawing
  m_screenQuad.reset(new ScreenQuad("Texture"));

  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);
  // now to load the shader and set the values
  // we are creating a shader called Texture
  ngl::ShaderLib::createShaderProgram("Texture");
  // now we are going to create empty shaders for Frag and Vert
  ngl::ShaderLib::attachShader("TextureVertex", ngl::ShaderType::VERTEX);
  ngl::ShaderLib::attachShader("TextureFragment", ngl::ShaderType::FRAGMENT);
  // attach the source
  ngl::ShaderLib::loadShaderSource("TextureVertex", "shaders/TextureVertex.glsl");
  ngl::ShaderLib::loadShaderSource("TextureFragment", "shaders/TextureFragment.glsl");
  // compile the shaders
  ngl::ShaderLib::compileShader("TextureVertex");
  ngl::ShaderLib::compileShader("TextureFragment");
  // add them to the program
  ngl::ShaderLib::attachShaderToProgram("Texture", "TextureVertex");
  ngl::ShaderLib::attachShaderToProgram("Texture", "TextureFragment");
  // now bind the shader attributes for most NGL primitives we use the following
  // layout attribute 0 is the vertex data (x,y,z)
  ngl::ShaderLib::bindAttribute("Texture", 0, "inVert");
  // attribute 1 is the UV data u,v (if present)
  ngl::ShaderLib::bindAttribute("Texture", 1, "inUV");

  // now we have associated this data we can link the shader
  ngl::ShaderLib::linkProgramObject("Texture");
  // and make it active ready to load values
  ngl::ShaderLib::use("Texture");
  // as re-size is not explicitly called we need to do this.
  // set the viewport for openGL we need to take into account retina display
  // etc by using the pixel ratio as a multiplyer
  glViewport(0, 0, width() * devicePixelRatio(), height() * devicePixelRatio());
  // load the texture
  m_texturePack = m_texturePack->instance();
  if (m_filename == "")
  {
    reload();
  }
  else
  {
    bool loaded = m_texturePack->load(m_filename);
    if (!loaded)
    {
      reload();
    }
  }
  m_currentTex = m_texturePack->begin();

  // resize window to 1K
  setWidth(1024);
  setHeight(1024);
}

void NGLScene::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, m_width, m_height);
  // draw quad
  std::cout << "binding " << m_currentTex->second << "\n";
  glBindTexture(GL_TEXTURE_2D, m_currentTex->second);
  setTitle(m_currentTex->first.c_str());
  m_screenQuad->draw();
}

void NGLScene::reload()
{
  QString filename = QFileDialog::getOpenFileName(
      0,
      tr("load texture"),
      QDir::currentPath(),
      tr("*.*"));
  if (!filename.isNull())
  {
    m_texturePack->clear();
    m_texturePack->load(filename.toStdString());
    m_filename = filename.toStdString();
    setTitle(QString("DXT Viewer use o to load new file current %1").arg(m_filename.c_str()));
  }
}

void NGLScene::append()
{
  QString filename = QFileDialog::getOpenFileName(
      0,
      tr("load texture"),
      QDir::currentPath(),
      tr("*.*"));
  if (!filename.isNull())
  {
    m_texturePack->append(filename.toStdString());
    m_filename = filename.toStdString();
    setTitle(QString("DXT Viewer use o to load new file current %1").arg(m_filename.c_str()));
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseMoveEvent(QMouseEvent *_event)
{
  NGL_UNUSED(_event);
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mousePressEvent(QMouseEvent *_event)
{
  NGL_UNUSED(_event);
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseReleaseEvent(QMouseEvent *_event)
{
  NGL_UNUSED(_event);
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::wheelEvent(QWheelEvent *_event)
{
  NGL_UNUSED(_event);
}
//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quite
  case Qt::Key_Escape:
    QGuiApplication::exit(EXIT_SUCCESS);
    break;
  case Qt::Key_F:
    showFullScreen();
    break;
  // show windowed
  case Qt::Key_N:
    showNormal();
    break;
  case Qt::Key_O:
    reload();
    break;
  case Qt::Key_A:
    append();
    break;

  case Qt::Key_Right:
    if (++m_currentTex == m_texturePack->end())
      --m_currentTex;
    break;

  case Qt::Key_Left:
    if (m_currentTex != m_texturePack->begin())
      --m_currentTex;
    break;

  default:
    break;
  }
  update();
}
