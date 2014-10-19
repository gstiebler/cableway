//---------------------------------------------------------------------------
#pragma hdrstop
#include "UOpenGL.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

COpenGL::COpenGL(int ClientWidth, int ClientHeight)
{
  size = TAMANHO;

  SetPixelFormatDescriptor();

  w = ClientWidth;
  h = ClientHeight;
  x2=0;
  y2=0;

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  zoom=50 / 10000.0;
  FatorZoomX=FatorZoomY=0.0;
  oldZoom = 0;
  static int CONTADOR=0;
  DEBUG=CONTADOR;
  CONTADOR++;
  menorx = maiorx = 0.0;
  menory = maiory = 0.0;
  distX = distY = 0.0;
  intervaloX = intervaloY = 0.0;
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

void COpenGL::DrawObjects() {}

//---------------------------------------------------------------------------

void COpenGL::Resize(int ClientWidth, int ClientHeight)
{
  w = ClientWidth;
  h = ClientHeight;
  if(h == 0)
    h = 1;
  glViewport(0, 0, w, h);
  AjustaExibicao();
  xMeioTela=w/2;
  yMeioTela=h/2;
}
//---------------------------------------------------------------------------

void COpenGL::Paint()
{
  RenderGLScene();
}
//---------------------------------------------------------------------------

void COpenGL::MouseMove(int X, int Y)
{
  if (apertado)
  {
    x2=X-x1;
    y2=y1-Y;
    distX = - intervaloX/w * x2;
    distY = - intervaloY/h * y2;
  }
}
//---------------------------------------------------------------------------

void COpenGL::MouseUp()
{
  apertado=false;
  mediax-=x2*FATOR_TELA/zoom;
  mediay-=y2*FATOR_TELA/zoom;
  maiorx += distX;
  menorx += distX;
  menory += distY;
  maiory += distY;
  x2=0;
  y2=0;
  distX = distY = 0;
}
//---------------------------------------------------------------------------

void COpenGL::DeslocaDesenho(int X, int Y)
{
  apertado=true;
  x1=X;
  y1=Y;
}
//---------------------------------------------------------------------------

void COpenGL::SetZoom(int Zoom)
{
  int difZoom;
  difZoom = Zoom - oldZoom;
  menorx += (FatorZoomX * difZoom) /2;
  maiorx -= (FatorZoomX * difZoom) /2;
  menory += (FatorZoomY * difZoom) /2;
  maiory -= (FatorZoomY * difZoom) /2;
  intervaloX -= (FatorZoomX * difZoom);
  intervaloY -= (FatorZoomY * difZoom);
  oldZoom = Zoom;
}
//---------------------------------------------------------------------------

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
    glTranslatef(origem.x, origem.y, 0);
    glScalef(FatorAltura*FATOR_FONTE, FatorAltura*FATOR_FONTE, 0);
    glRotatef(rotacao, 0, 0, 1);
    //YsDrawUglyFont(texto.c_str(), 0, 0);
  glPopMatrix();
}
//---------------------------------------------------------------------------

void COpenGL::AjustaExibicao()
{
  glMatrixMode(GL_PROJECTION); // Muda a pilha de transforma��es para a matriz de proje��o
  glLoadIdentity(); // Carrega a matriz identidade na matriz atual
  glOrtho (menorx + distX, menorx + distX + intervaloX, menory + distY, menory + distY + intervaloY, -50, 50);
  // distX/distY servem pra enquanto o desenho est� sendo deslocado
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
//---------------------------------------------------------------------------

TPonto COpenGL::ConvertePonto(int X, int Y)
{
  TPonto retorno;

  retorno.x = intervaloX/w * X + menorx + distX;
  retorno.y = intervaloY/h * (h-Y) + menory + distY;

  return retorno;
}
//---------------------------------------------------------------------------
