//---------------------------------------------------------------------------
#pragma hdrstop
#include "UMostraDesenho.h"
#include <UGrafoDesenho.h>

using namespace std;


unsigned char pegaAzul ( int cor )
{
  return ( cor % (0xFF+1) );
}

unsigned char pegaVerde ( int cor )
{
  return ( cor % (0xFFFF+1) ) >> ( 4 * 2 );
}

unsigned char pegaVermelho ( int cor )
{
  return ( cor % (0xFFFFFF+1) ) >> ( 4 * 4 );
}


CMostraDesenho::CMostraDesenho(shared_ptr<CGrafoDesenho> grafoDesenho, shared_ptr<CInfoCircuitos> infoCircuitos, QWidget *parent) :
        COpenGL(600, 600, parent),
        bMostraArvore2( false ),
        xBola( -1 ),
        yBola( - 1),
        tamBola( -1 )
{
  semCores = false;
	primeiro=true;
	GrafoDesenho=grafoDesenho;
	InfoCircuitos=infoCircuitos;
	ExibirCircuito=false;
	bMostraNumVerticesDEBUG=false;
	bMostraArvore=false;
	bMostraBola=false;
	destacaCoresDeEquipamentos = true;
	facilitarVerBandeirola = true;
	mostraLigacaoEquipamento = false;
	MostrarPontasDeCaboDescon = true;
}
//---------------------------------------------------------------------------

CMostraDesenho::~CMostraDesenho()
{

}
//---------------------------------------------------------------------------



void CMostraDesenho::initializeLimits()
{	
	float x, y;
	_glCoords.initializeLimits();
	if (GrafoDesenho->_ult > GrafoDesenho->_dados->Multipoint.size())
		GrafoDesenho->_ult = GrafoDesenho->_dados->Multipoint.size();
	for (int n=0; n<GrafoDesenho->_ult; n++)
	{
		vector<TPonto> &points = GrafoDesenho->_dados->Multipoint[n]->pontos;
		int numPoints = points.size();
		for (int i=0; i < numPoints; i++)
		{
			x = points[i].x;
			y = points[i].y;

			_glCoords.updateLimits( x, y );
		}
	}
	for ( int i=0; i<GrafoDesenho->_dados->Arcos.size(); i++)
	{
			x = GrafoDesenho->_dados->Arcos[i]->Centro.x;
			y = GrafoDesenho->_dados->Arcos[i]->Centro.y;

			double raio = GrafoDesenho->_dados->Arcos[i]->EixoPrimario;

			_glCoords.updateLimits( x + raio, y + raio );
	}

	_glCoords.updateProportion();
	initialized = true;
}


void CMostraDesenho::setColor( unsigned char r, unsigned char g, unsigned char b )
{
	_brush.setColor( QColor( r, g, b ) );
	_painter->setBrush( _brush );
}


void CMostraDesenho::setColorFromLevel( int level )
{
	QColor color;
	switch ( level )
	{
		case CALHA:
			setColor( 0, 255, 255 );
			break;
		case CABO:
			setColor( pegaVermelho(CORCABO), pegaVerde(CORCABO), pegaAzul(CORCABO) );
			break;
		case BANDEIROLA:
			setColor( pegaVermelho(CORBANDEIROLA), pegaVerde(CORBANDEIROLA), pegaAzul(CORBANDEIROLA) );
			break;
		case TAG:
			setColor( pegaVermelho(CORTAG), pegaVerde(CORTAG), pegaAzul(CORTAG) );
			break;
		case INSTRUMENTO:
			setColor( pegaVermelho(CORINSTRUMENTO), pegaVerde(CORINSTRUMENTO), pegaAzul(CORINSTRUMENTO) );
			break;
		case INSTRUMENTODESCON:
			setColor( pegaVermelho(CORINSTRUMENTODESCON), pegaVerde(CORINSTRUMENTODESCON), pegaAzul(CORINSTRUMENTODESCON) );
			break;
		case NADA:
		default:
			setColor( pegaVermelho(CORNADA), pegaVerde(CORNADA), pegaAzul(CORNADA) );
			break;
	}
}


void CMostraDesenho::drawMultipoints()
{
	for (int n=0; n<GrafoDesenho->_dados->Multipoint.size(); n++)
			//    for (int n=GrafoDesenho->pri; n<GrafoDesenho->ult; n++)
	{
		shared_ptr<TMultipoint> multipoint = GrafoDesenho->_dados->Multipoint[n];
		setColor(multipoint->CorR, multipoint->CorG, multipoint->CorB);
		if (destacaCoresDeEquipamentos)
			setColorFromLevel( multipoint->Nivel );
		if ( semCores )
		{
			setColorFromLevel( -1 );
		}
		int numPoints = multipoint->pontos.size();
		if( multipoint->tipo == LINE_CLOSED )
			numPoints++;
		QPolygon polygon( numPoints );

		for (int i=0; i<multipoint->pontos.size(); i++)
		{
			double x = multipoint->pontos[i].x;
			double y = multipoint->pontos[i].y;

			polygon.setPoint( i, x, y );
		}

		if( multipoint->tipo == LINE_CLOSED )
		{
			double x = multipoint->pontos[numPoints - 1].x;
			double y = multipoint->pontos[numPoints - 1].y;

			polygon.setPoint( numPoints - 1, x, y );
		}
		_painter->drawPolyline( polygon );
	}
}


void CMostraDesenho::drawArcs()
{
	for (int n=0; n<GrafoDesenho->_dados->Arcos.size(); n++)
	{
		shared_ptr<TArco> arc = GrafoDesenho->_dados->Arcos[n];
		setColor( arc->CorR, arc->CorG, arc->CorB);
		if ( destacaCoresDeEquipamentos )
			setColorFromLevel( arc->Nivel );
		if ( semCores )
		{
			setColorFromLevel( -1 );
		}
		DesenhaArco( arc->Centro.x, arc->Centro.y, arc->EixoPrimario, arc->EixoSecundario, arc->AngIni, arc->AngEnd );
	}
}


void CMostraDesenho::showCircuit()
{
	TPonto Pontos[2];
	setColor( pegaVermelho(CORCAMINHO), pegaVerde(CORCAMINHO), pegaAzul(CORCAMINHO) );
	glLineWidth((GLfloat)(3.0));
	vector< shared_ptr<TAresta> > &edges = _arestasCircuito->ArestasDesenho[GrafoDesenho->_dados->_drawing.get()];

	int ini = 0;
	int end = edges.size()-1;
	if( !mostraLigacaoEquipamento )
	{
		ini++;
		end--;
	}
	for (int n=ini; n<end; n++)
	{
		CInfoCircuitos::PontosAresta(Pontos, edges[n]);
		glBegin(GL_LINE_STRIP);
		glVertex2f(Pontos[0].x, Pontos[0].y);
		glVertex2f(Pontos[1].x, Pontos[1].y);
		glEnd();
	}
	glLineWidth((GLfloat)(1.0));
}



void CMostraDesenho::showTree()
{
	string origem = VerticeArvore->texto.c_str();
	glColor3f(pegaVermelho(CORARVORE)/255.0, pegaVerde(CORARVORE)/255.0, pegaAzul(CORARVORE)/255.0);
	glLineWidth((GLfloat)(3.0));

	vector< shared_ptr<TAresta> > Arestas;
	InfoCircuitos->Arvore(VerticeArvore, Arestas, GrafoDesenho->_dados->_drawing);
	shared_ptr<TAresta> Aresta;
	TPonto Pontos[2];
	for (int n=0; n<(int)Arestas.size(); n++)
	{
		Aresta = Arestas[n];
		string debugTexto = Aresta->_vertices[0]->texto;
		Pontos[0] = Aresta->_vertices[0]->pos;
		Pontos[1] = Aresta->_vertices[1]->pos;
		glBegin(GL_LINE_STRIP);
		glVertex2f(Pontos[0].x, Pontos[0].y);
		glVertex2f(Pontos[1].x, Pontos[1].y);
		glEnd();
#define TAMBOLACOLAR (1000)
		if ( Aresta->_vertices[0]->EhColar )
		{
			DesenhaBolaFechada(Pontos[0].x, Pontos[0].y, _glCoords.getWorldWidth() / TAMBOLACOLAR, _glCoords.getWorldWidth()/TAMBOLACOLAR, 0, 2*M_PI );
		}
		if ( Aresta->_vertices[1]->EhColar )
		{
			DesenhaBolaFechada(Pontos[1].x, Pontos[1].y, _glCoords.getWorldWidth()/TAMBOLACOLAR, _glCoords.getWorldWidth()/TAMBOLACOLAR, 0, 2*M_PI );
		}
	}
	glLineWidth((GLfloat)(1.0));
	if (bMostraArvore2)
	{
		// Roxo - Destino
		string destino = VerticeArvore2->texto;
//				if ( ponteiroPraFuncao )
//				    ponteiroPraFuncao(ponteiroProThis, origem , destino);
		glColor3f(pegaVermelho(CORARVORE2)/255.0, pegaVerde(CORARVORE2)/255.0, pegaAzul(CORARVORE2)/255.0);
		//			glColor3f(0.7, 0.0, 0.7);
		glLineWidth((GLfloat)(3.0));

		glPushMatrix();
		//				EscreveTexto(("Destino: "+ GrafoDesenho->VerticesGerais->getItem(VerticeArvore2)->texto).c_str(), pos, 0,
		//						GrafoDesenho->Dados->Textos[0].FatorAltura*10);
		glPopMatrix();

		vector< shared_ptr<TAresta> > Arestas;
		InfoCircuitos->Arvore(VerticeArvore2, Arestas, GrafoDesenho->_dados->_drawing);
		TPonto Pontos[2];
		for (int n=0; n<(int)Arestas.size(); n++)
		{
			Aresta = Arestas[n];
			for( int i(0); i < 2; ++i )
				Pontos[i] = Aresta->_vertices[i]->pos;

			glBegin(GL_LINE_STRIP);
			for( int i(0); i < 2; ++i )
				glVertex2f(Pontos[i].x, Pontos[i].y);
			glEnd();
			
			for( int i(0); i < 2; ++i )
			{
				if ( Aresta->_vertices[i]->EhColar )
				{
					DesenhaBolaFechada(Pontos[i].x, Pontos[i].y, _glCoords.getWorldWidth()/TAMBOLACOLAR, _glCoords.getWorldWidth()/TAMBOLACOLAR, 0, 2*M_PI );
				}
			}
		}
		glLineWidth((GLfloat)(1.0));
	}
}



void CMostraDesenho::showDisconnectedCircuitEndings()
{
	glColor3f(pegaVermelho(CORINSTRUMENTODESCON)/255.0, pegaVerde(CORINSTRUMENTODESCON)/255.0, pegaAzul(CORINSTRUMENTODESCON)/255.0);
	for ( int i = 0 ; i < GrafoDesenho->_cabosReta.size() ; i++ )
	{
		if ( !GrafoDesenho->_cabosReta[i]->ponta[0] )
		{
			TPonto pontos = GrafoDesenho->_cabosReta[i]->_multipoint->pontos[0];
			DesenhaBolaFechada(pontos.x, pontos.y, GrafoDesenho->_distMinElemCaboPraOpenGL*4, GrafoDesenho->_distMinElemCaboPraOpenGL*4, 0, 2*M_PI );
		}
		if ( !GrafoDesenho->_cabosReta[i]->ponta[1] )
		{
			int tam = GrafoDesenho->_cabosReta[i]->_multipoint->pontos.size();
			TPonto pontos = GrafoDesenho->_cabosReta[i]->_multipoint->pontos[tam-1];
			DesenhaBolaFechada(pontos.x, pontos.y, GrafoDesenho->_distMinElemCaboPraOpenGL*4, GrafoDesenho->_distMinElemCaboPraOpenGL*4, 0, 2*M_PI );
		}
	}

	for ( int i = 0 ; i < GrafoDesenho->_cabosArco.size() ; i++ )
	{
		TPonto pontos[2];
		GrafoDesenho->_cabosArco[i]->_arco->PontasArco(pontos);
		if ( !GrafoDesenho->_cabosArco[i]->ponta[0] )
		{
			DesenhaBolaFechada(pontos[0].x, pontos[0].y, GrafoDesenho->_distMinElemCaboPraOpenGL*4, GrafoDesenho->_distMinElemCaboPraOpenGL*4, 0, 2*M_PI );
		}
		if ( !GrafoDesenho->_cabosArco[i]->ponta[1] )
		{
			DesenhaBolaFechada(pontos[1].x, pontos[1].y, GrafoDesenho->_distMinElemCaboPraOpenGL*4, GrafoDesenho->_distMinElemCaboPraOpenGL*4, 0, 2*M_PI );
		}
	}
}



void CMostraDesenho::showBandeirolaEndings()
{
	for (int n=0; n<(int)GrafoDesenho->_pontosPraMostrarBandeirola.size();n++)
	{
		glColor3f(1.0, 1.0, 1.0);
		glLineWidth((GLfloat)(4.0));
		double dist = DistPontos(GrafoDesenho->_pontosPraMostrarBandeirola.at(n).NaBandeirola, GrafoDesenho->_pontosPraMostrarBandeirola.at(n).NoCabo);
		if (dist > GrafoDesenho->_distMinElemCaboPraOpenGL) // Se a distância entre os pontos não for muito pequena mostra uma reta
		{
			glBegin(GL_LINE_STRIP);
			glVertex2f(GrafoDesenho->_pontosPraMostrarBandeirola.at(n).NaBandeirola.x, GrafoDesenho->_pontosPraMostrarBandeirola.at(n).NaBandeirola.y);
			glVertex2f(GrafoDesenho->_pontosPraMostrarBandeirola.at(n).NoCabo.x, GrafoDesenho->_pontosPraMostrarBandeirola.at(n).NoCabo.y);
			glEnd();
		}
		else // Senão, faz um círculo em volta dos pontos
		{
			TPontosBandeirola &pontosBandeirola = GrafoDesenho->_pontosPraMostrarBandeirola[n];
			DesenhaArco( pontosBandeirola.NoCabo.x, pontosBandeirola.NoCabo.y,
					GrafoDesenho->_distMinElemCaboPraOpenGL*4, GrafoDesenho->_distMinElemCaboPraOpenGL*4, 0, 2*M_PI );
		}
		glLineWidth((GLfloat)(1.0));
	}
}



void CMostraDesenho::drawTexts()
{
	for (int n=0; n<GrafoDesenho->_dados->Textos.size(); n++)
	{
		shared_ptr<TTexto> texto = GrafoDesenho->_dados->Textos[n];

		if ( destacaCoresDeEquipamentos )
		{
			if( texto->Nivel == INSTRUMENTO)
				glColor3f(pegaVermelho(CORTAG)/255.0, pegaVerde(CORTAG)/255.0, pegaAzul(CORTAG)/255.0);
			else if( texto->Nivel == BANDEIROLA)
				glColor3f(pegaVermelho(CORBANDEIROLA)/255.0, pegaVerde(CORBANDEIROLA)/255.0, pegaAzul(CORBANDEIROLA)/255.0);
			else
				glColor3f(pegaVermelho(CORNADA)/255.0, pegaVerde(CORNADA)/255.0, pegaAzul(CORNADA)/255.0);

		}
		else if ( semCores )
			glColor3f(pegaVermelho(CORNADA)/255.0, pegaVerde(CORNADA)/255.0, pegaAzul(CORNADA)/255.0);
		else
			glColor3f( texto->CorR/255.0, texto->CorG/255.0, texto->CorB/255.0);

		glPushMatrix();

		EscreveTexto(texto->texto, texto->origem, texto->rotacao, texto->FatorAltura);
		glPopMatrix();
	}
}



void CMostraDesenho::DrawObjects()
{
	//glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();

	if (primeiro && !initialized)
		initializeLimits();

	AjustaExibicao();//DESLOCA IMAGEM E D� ZOOM

	//if (primeiro)
	//{
	//	glNewList(1, GL_COMPILE);
		//LINHAS
		drawMultipoints();

		//ARCOS
		drawArcs();

		if (ExibirCircuito)
			showCircuit();

		if (bMostraArvore)
			showTree();

		// Facilitar pontas de cabo desconectadas
		if ( MostrarPontasDeCaboDescon )
			showDisconnectedCircuitEndings();

		if ( facilitarVerBandeirola )
			showBandeirolaEndings();

		//TEXTOS
		drawTexts();

		if (bMostraBola)
		{
			glColor3f(0.0, 0.0, 1.0);
			DesenhaBolaFechada(xBola, yBola, tamBola, tamBola, 0, 2*M_PI );
		}
	//	glEndList();
	//}
	//else
	//	glCallList(1);
	primeiro=false;
	glPopMatrix();   
}
//---------------------------------------------------------------------------

void CMostraDesenho::MostraCircuito( shared_ptr<TArestasCircuito> arestasCircuito )
{
	ExibirCircuito=true;
	primeiro=true;
	_arestasCircuito = arestasCircuito;
}
//---------------------------------------------------------------------------

void CMostraDesenho::MostraNumVerticesDEBUG(bool mostra)
{
	primeiro=true;
	bMostraNumVerticesDEBUG=mostra;
}
//---------------------------------------------------------------------------

void CMostraDesenho::MostraArvore(shared_ptr<TVerticeGeral> vertice)
{
	primeiro=true;
	VerticeArvore=vertice;
	bMostraArvore=true;
	bMostraArvore2=false;
}
//---------------------------------------------------------------------------

void CMostraDesenho::MostraDoubleArvore(shared_ptr<TVerticeGeral> vertice, shared_ptr<TVerticeGeral> vertice2)
{
	primeiro=true;
	VerticeArvore=vertice;
	VerticeArvore2=vertice2;
	bMostraArvore=true;
	bMostraArvore2=true;
}
//---------------------------------------------------------------------------
void CMostraDesenho::SetDestacaBandeirolas(bool facilita)
{
	primeiro=true;
	facilitarVerBandeirola = facilita;
}
//---------------------------------------------------------------------------



void CMostraDesenho::SetDestacaCores(bool DestacaCores)
{
	primeiro = true;
	destacaCoresDeEquipamentos = DestacaCores;
}
//---------------------------------------------------------------------------
void CMostraDesenho::SetMostraChegaEquip(bool MostraEquip)
{
	primeiro = true;
	mostraLigacaoEquipamento = MostraEquip;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void CMostraDesenho::SetMostrarPontasDeCaboDescon(bool mostraPontas)
{
	primeiro = true;
	MostrarPontasDeCaboDescon = mostraPontas;
}



void CMostraDesenho::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}



void CMostraDesenho::resizeGL(int width, int height)
{
	Resize( width, height );
}



void CMostraDesenho::mousePressEvent( QMouseEvent *event )
{
	_glCoords.mousePress( event->x(), event->y() );
}



void CMostraDesenho::mouseMoveEvent( QMouseEvent *event )
{
	_glCoords.mouseMove( event->x(), event->y() );
	repaint();
}



void CMostraDesenho::wheelEvent(QWheelEvent * event)
{
	double increase = 1.0 + (event->angleDelta().y() / 1200.0);
	_glCoords.incZoom( increase );
	repaint();
}
