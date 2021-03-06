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
  m_filename=_fname;
  setTitle(QString("DXT Viewer use o to load new file current %1").arg( m_filename.c_str()));
}


NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}

void NGLScene::resizeGL(int _w , int _h)
{
  m_width=_w*devicePixelRatio();
  m_height=_h*devicePixelRatio();
}

void NGLScene::resizeGL(QResizeEvent *_event)
{
  int w=_event->size().width();
  int h=_event->size().height();
  m_width=w*devicePixelRatio();
  m_height=h*devicePixelRatio();
}


void NGLScene::initializeGL()
{
  // we must call this first before any other GL commands to load and link the
  // gl commands from the lib, if this is not done program will crash
  ngl::NGLInit::instance();
  // create a simple screen quad for drawing
  m_screenQuad.reset( new ScreenQuad("Texture"));

  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);
   // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // we are creating a shader called Texture
  shader->createShaderProgram("Texture");
  // now we are going to create empty shaders for Frag and Vert
  shader->attachShader("TextureVertex",ngl::ShaderType::VERTEX);
  shader->attachShader("TextureFragment",ngl::ShaderType::FRAGMENT);
  // attach the source
  shader->loadShaderSource("TextureVertex","shaders/TextureVertex.glsl");
  shader->loadShaderSource("TextureFragment","shaders/TextureFragment.glsl");
  // compile the shaders
  shader->compileShader("TextureVertex");
  shader->compileShader("TextureFragment");
  // add them to the program
  shader->attachShaderToProgram("Texture","TextureVertex");
  shader->attachShaderToProgram("Texture","TextureFragment");
  // now bind the shader attributes for most NGL primitives we use the following
  // layout attribute 0 is the vertex data (x,y,z)
  shader->bindAttribute("Texture",0,"inVert");
  // attribute 1 is the UV data u,v (if present)
  shader->bindAttribute("Texture",1,"inUV");

  // now we have associated this data we can link the shader
  shader->linkProgramObject("Texture");
  // and make it active ready to load values
  (*shader)["Texture"]->use();
  // as re-size is not explicitly called we need to do this.
  // set the viewport for openGL we need to take into account retina display
  // etc by using the pixel ratio as a multiplyer
  glViewport(0,0,width()*devicePixelRatio(),height()*devicePixelRatio());
  // load the texture
  if(m_filename=="")
  {
    reload();
  }
  else
  {
    bool loaded=m_texture.load(m_filename);
    if(!loaded)
    {
      reload();
    }
  }
   // resize window to screen size
  setWidth(m_texture.width());
  setHeight(m_texture.height());
}




void NGLScene::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_width,m_height);
  // draw quad
  m_screenQuad->draw();

}

void NGLScene::reload()
{
	QString filename = QFileDialog::getOpenFileName(
						0,
						tr("load texture"),
						QDir::currentPath(),
						tr("*.*") );
				if( !filename.isNull() )
				{
					m_texture.reset();
					m_texture.load(filename.toStdString());
					m_filename=filename.toStdString();
					setTitle(QString("DXT Viewer use o to load new file current %1").arg( m_filename.c_str()));
				}
}


//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseMoveEvent (QMouseEvent * _event)
{
	NGL_UNUSED(_event);
}


//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mousePressEvent ( QMouseEvent * _event)
{
	NGL_UNUSED(_event);

}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseReleaseEvent ( QMouseEvent * _event )
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
  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
  case Qt::Key_F : showFullScreen(); break;
  // show windowed
  case Qt::Key_N : showNormal(); break;
  case Qt::Key_O : reload(); break;
  default : break;
  }
  update();
}





