//---------------------------------------------------------------------------
#pragma hdrstop
#include "UOpenGL.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

COpenGL::COpenGL(int ClientWidth, int ClientHeight, QWidget *parent) : 
        QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
		_glCoords( ClientWidth, ClientHeight )
{
  SetPixelFormatDescriptor();

  glClearColor(0.0f, 1.0f, 0.0f, 0.5f);
  static int CONTADOR=0;
  DEBUG=CONTADOR;
  CONTADOR++;
  initialized = false;
}
//---------------------------------------------------------------------------

COpenGL::~COpenGL()
{
}
//---------------------------------------------------------------------------

void COpenGL::SetPixelFormatDescriptor()
{

}
//---------------------------------------------------------------------------

void COpenGL::RenderGLScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    DrawObjects();
    glFlush();
}
//---------------------------------------------------------------------------



void COpenGL::Resize(int ClientWidth, int ClientHeight)
{
	_glCoords.resize( ClientWidth, ClientHeight );
	glViewport(0, 0, _glCoords.canvasWidth, _glCoords.canvasHeight);
	AjustaExibicao();
}
//---------------------------------------------------------------------------

void COpenGL::Paint()
{
	RenderGLScene();
}



void COpenGL::DesenhaArco(float x_center, float y_center, float w,
          float h, float startAngle, float arcAngle, int n)
{
  float pheta, angle_increment;
  float x, y;
  if (n <= 0)
    n = 1;
  if (arcAngle<0)
  {
    arcAngle*=-1;
    startAngle-=arcAngle;
  }
  angle_increment = M_PI_2 / n;
  glPushMatrix ();
  //  center the oval at x_center, y_center
  glTranslatef (x_center, y_center, 0);
  //  draw the oval using line segments
  glBegin (GL_LINE_STRIP);
  for (pheta = startAngle; pheta - (startAngle + arcAngle)
                        < 0.001; pheta += angle_increment)
  {
    x = w * cos (pheta);
    y = h * sin (pheta);
    glVertex2f (x, y);
  }
  x = w * cos (startAngle + arcAngle);
  y = h * sin (startAngle + arcAngle);
  glVertex2f (x, y);
  glEnd ();
  glPopMatrix ();
}
//---------------------------------------------------------------------------

void COpenGL::DesenhaBolaFechada(float x_center, float y_center, float w,
          float h, float startAngle, float arcAngle, int n)
{
  float pheta, angle_increment;
  float x, y;
  if (n <= 0)
    n = 1;
  if (arcAngle<0)
  {
    arcAngle*=-1;
    startAngle-=arcAngle;
  }
  angle_increment = M_PI_2 / n;
  glPushMatrix ();
  //  center the oval at x_center, y_center
  glTranslatef (x_center, y_center, 0);
  //  draw the oval using line segments
  glBegin (GL_TRIANGLE_FAN);
    glVertex2f (0, 0);
  for (pheta = startAngle; pheta - (startAngle + arcAngle)
                        < 0.001; pheta += angle_increment)
  {
    x = w * cos (pheta);
    y = h * sin (pheta);
    glVertex2f (x, y);

//    glBegin(GL_TRIANGLES);		// Drawing Using Triangles
//	    glVertex3f( 0.0f, 1.0f, 0.0f);		// Top
//	    glVertex3f(-1.0f,-1.0f, 0.0f);		// Bottom Left
//	    glVertex3f( 1.0f,-1.0f, 0.0f);		// Bottom Right
 //   glEnd();
  }
  x = w * cos (startAngle + arcAngle);
  y = h * sin (startAngle + arcAngle);
  glVertex2f (x, y);
  glEnd ();
  glPopMatrix ();
}
//---------------------------------------------------------------------------

void COpenGL::EscreveTexto(string texto, TPonto origem, double rotacao, double FatorAltura)
{
  glPushMatrix();
    //glTranslatef(origem.x, origem.y, 0);
    //glScalef(FatorAltura*FATOR_FONTE, FatorAltura*FATOR_FONTE, 0);
    glRotatef(rotacao, 0, 0, 1);
	QString txt( texto.c_str() );
	double canvasWidth = _glCoords.getRight() - _glCoords.getLeft();
	int fontSize = 300000 / canvasWidth;
	if( fontSize < 1 )
		fontSize = 1;
	renderText(origem.x, origem.y, 0.0, txt, QFont("Arial", fontSize, QFont::Bold, false) );
  glPopMatrix();
}
//---------------------------------------------------------------------------

void COpenGL::AjustaExibicao()
{
	glMatrixMode(GL_PROJECTION); // Muda a pilha de transforma��es para a matriz de proje��o
	glLoadIdentity(); // Carrega a matriz identidade na matriz atual
	glOrtho( _glCoords.getLeft(), _glCoords.getRight(), _glCoords.getBottom(), _glCoords.getTop(), -50, 50);
	// distX/distY servem pra enquanto o desenho est� sendo deslocado
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
//---------------------------------------------------------------------------
