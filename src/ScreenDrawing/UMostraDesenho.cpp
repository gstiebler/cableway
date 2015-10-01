//---------------------------------------------------------------------------
#pragma hdrstop
#include "UMostraDesenho.h"
#include <math.h>
#include <time.h>
#include <assert.h>
#include "UInfoCircuitos.h"
#include "UDadosGenerico.h"
#include "Electrical\ElectricalElements.h"
#include "Electrical\Bandeirola.h"
#include "Electrical\StraightCable.h"
#include "Electrical\ArcCable.h"

#define LINE_CLOSED 0
#define LINE_OPEN 1

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


CMostraDesenho::CMostraDesenho(  std::shared_ptr<CDadosGenerico> dados, std::shared_ptr<ElectricalElements> electricalElements, 
	  shared_ptr<CInfoCircuitos> infoCircuitos, GLCoords *glCoords ) :
        bMostraArvore2( false ),
        xBola( -1 ),
        yBola( - 1),
        tamBola( -1 ),
		_glCoords( glCoords ),
		_dados( dados ),
		_electricalElements( electricalElements )
{
  semCores = false;
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
	_glCoords->initializeLimits();
	for ( auto multiPoint : _dados->Multipoint )
		for ( auto point : multiPoint->pontos )
			_glCoords->updateLimits( point.x, point.y );

	for ( auto arc : _dados->Arcos )
	{
			x = arc->Centro.x;
			y = arc->Centro.y;

			double raio = arc->EixoPrimario;

			_glCoords->updateLimits( x + raio, y + raio );
	}

	_glCoords->updateProportion();
}


void CMostraDesenho::setColor( unsigned char r, unsigned char g, unsigned char b )
{
	_pen.setColor( QColor( r, g, b ) );
	//_painter->setPen( _pen );

	//_pen.setColor( QColor( 255, 255, 255 ) );
	_pen.setStyle( Qt::SolidLine );
	_pen.setWidth( 50 );
	_painter->setPen( _pen );

	_brush.setColor( QColor( 255, 255, 255 ) );
	_painter->setBrush( _brush );
}


void CMostraDesenho::setColorFromLevel( int level )
{
	QColor color;
	switch ( level )
	{
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
	_pen.setWidth( 2 );
	_painter->setPen( _pen );
	for ( auto multipoint : _dados->Multipoint )
	{
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
			double x = multipoint->pontos[0].x;
			double y = multipoint->pontos[0].y;

			polygon.setPoint( numPoints - 1, x, y );
		}
		_painter->drawPolyline( polygon );
	}
}


void CMostraDesenho::drawArcs()
{
	assert( _painter );
	for ( auto arc : _dados->Arcos )
	{
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
	//_pen.setWidth( 3 );
	_painter->setPen( _pen );
	vector< shared_ptr<TAresta> > &edges = _arestasCircuito->ArestasDesenho[ _dados->_drawing.get() ];

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
		QLineF line( QPoint( Pontos[0].x, Pontos[0].y ), QPoint( Pontos[1].x, Pontos[1].y ) );
		_painter->drawLine( line );
	}
	//_pen.setWidth( 1 );
	_painter->setPen( _pen );
}



void CMostraDesenho::showTree()
{
	string origem = VerticeArvore->texto.c_str();
	setColor( pegaVermelho(CORARVORE), pegaVerde(CORARVORE), pegaAzul(CORARVORE));
	//_pen.setWidth( 3.0 );
	_painter->setPen( _pen );

	vector< shared_ptr<TAresta> > Arestas;
	InfoCircuitos->Arvore(VerticeArvore, Arestas, _dados->_drawing);
	shared_ptr<TAresta> Aresta;
	TPonto Pontos[2];
	for (int n=0; n<(int)Arestas.size(); n++)
	{
		Aresta = Arestas[n];
		string debugTexto = Aresta->_vertices[0]->texto;
		Pontos[0] = Aresta->_vertices[0]->pos;
		Pontos[1] = Aresta->_vertices[1]->pos;
		QLineF line( QPoint( Pontos[0].x, Pontos[0].y ), QPoint( Pontos[1].x, Pontos[1].y ) );
		_painter->drawLine( line );
#define TAMBOLACOLAR (1000)
		if ( Aresta->_vertices[0]->EhColar )
		{
			DesenhaBolaFechada(Pontos[0].x, Pontos[0].y, _glCoords->getWorldWidth() / TAMBOLACOLAR, _glCoords->getWorldWidth()/TAMBOLACOLAR, 0, 2*M_PI );
		}
		if ( Aresta->_vertices[1]->EhColar )
		{
			DesenhaBolaFechada(Pontos[1].x, Pontos[1].y, _glCoords->getWorldWidth()/TAMBOLACOLAR, _glCoords->getWorldWidth()/TAMBOLACOLAR, 0, 2*M_PI );
		}
	}
	//_pen.setWidth( 1.0 );
	_painter->setPen( _pen );
	if (bMostraArvore2)
	{
		// Roxo - Destino
		string destino = VerticeArvore2->texto;
//				if ( ponteiroPraFuncao )
//				    ponteiroPraFuncao(ponteiroProThis, origem , destino);
		setColor(pegaVermelho(CORARVORE2), pegaVerde(CORARVORE2), pegaAzul(CORARVORE2));
		//			glColor3f(0.7, 0.0, 0.7);
	//_pen.setWidth( 1.0 );
	_painter->setPen( _pen );

		vector< shared_ptr<TAresta> > Arestas;
		InfoCircuitos->Arvore(VerticeArvore2, Arestas, _dados->_drawing);
		TPonto Pontos[2];
		for (int n=0; n<(int)Arestas.size(); n++)
		{
			Aresta = Arestas[n];
			for( int i(0); i < 2; ++i )
				Pontos[i] = Aresta->_vertices[i]->pos;

			QLineF line( QPoint( Pontos[0].x, Pontos[0].y ), QPoint( Pontos[1].x, Pontos[1].y ) );
			_painter->drawLine( line );
			
			for( int i(0); i < 2; ++i )
			{
				if ( Aresta->_vertices[i]->EhColar )
				{
					DesenhaBolaFechada(Pontos[i].x, Pontos[i].y, _glCoords->getWorldWidth()/TAMBOLACOLAR, _glCoords->getWorldWidth()/TAMBOLACOLAR, 0, 2*M_PI );
				}
			}
		}
	}
}



template<class T>
void CMostraDesenho::showDisconnectedCircuitEndings( std::vector< std::shared_ptr<T> > &geometricEdges )
{
	// TODO set a dynamic size
	const int GAMBIARRA_SIZE = 10.0;
	for ( auto element : geometricEdges )
		for( auto edge : element->_edges )
			if( edge->ListaVerticesEArestas->list.size() == 1 )
				DesenhaBolaFechada( edge->pos.x, edge->pos.y, GAMBIARRA_SIZE, GAMBIARRA_SIZE, 0, 2*M_PI );
}


void CMostraDesenho::showDisconnectedCircuitEndings()
{
	showDisconnectedCircuitEndings( _electricalElements->_straightCables );
	showDisconnectedCircuitEndings( _electricalElements->_arcCables );
}



void CMostraDesenho::showBandeirolaEndings()
{		
	setColor(1.0, 1.0, 1.0);
	_pen.setWidth( 4.0 );
	for ( auto bandeirola : _electricalElements->_bandeirolas )
	{
		double size = bandeirola->getWidth() * 0.2;
		DesenhaArco( bandeirola->_edges[0]->pos.x, bandeirola->_edges[0]->pos.y, size, size, 0, 2*M_PI );
	}
	_pen.setWidth( 1.0 );
	_painter->setPen( _pen );
}



void CMostraDesenho::drawTexts()
{
	for ( auto texto : _dados->Textos)
	{
		if ( destacaCoresDeEquipamentos )
		{
			setColorFromLevel( texto->Nivel );

		}
		else if ( semCores )
			setColorFromLevel( -1 );
		else
			setColor( texto->CorR, texto->CorG, texto->CorB);

		EscreveTexto(texto->texto, texto->origem, texto->rotacao, texto->FatorAltura);
	}
}



void CMostraDesenho::DrawObjects( QPainter *painter )
{
	_painter = painter;
	_brush.setColor( QColor(64, 255, 64) );
	_painter->setBrush( _brush );
	_painter->setPen( _pen );
	QRect rect( _glCoords->getLeft(), _glCoords->getRight(), _glCoords->getWorldWidth(), _glCoords->getWorldHeight() );
	painter->fillRect( rect, QColor(64, 255, 64));

	setColor( 255, 255, 255);

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
			setColor( 0, 0, 255 );
			DesenhaBolaFechada(xBola, yBola, tamBola, tamBola, 0, 2*M_PI );
		}
		
	_painter = NULL;
}
//---------------------------------------------------------------------------

void CMostraDesenho::MostraCircuito( shared_ptr<TArestasCircuito> arestasCircuito )
{
	ExibirCircuito=true;
	_arestasCircuito = arestasCircuito;
}
//---------------------------------------------------------------------------

void CMostraDesenho::MostraNumVerticesDEBUG(bool mostra)
{
	bMostraNumVerticesDEBUG=mostra;
}
//---------------------------------------------------------------------------

void CMostraDesenho::MostraArvore(shared_ptr<TVerticeGeral> vertice)
{
	VerticeArvore=vertice;
	bMostraArvore=true;
	bMostraArvore2=false;
}
//---------------------------------------------------------------------------

void CMostraDesenho::MostraDoubleArvore(shared_ptr<TVerticeGeral> vertice, shared_ptr<TVerticeGeral> vertice2)
{
	VerticeArvore=vertice;
	VerticeArvore2=vertice2;
	bMostraArvore=true;
	bMostraArvore2=true;
}
//---------------------------------------------------------------------------
void CMostraDesenho::SetDestacaBandeirolas(bool facilita)
{
	facilitarVerBandeirola = facilita;
}
//---------------------------------------------------------------------------


void CMostraDesenho::SetDestacaCores(bool DestacaCores)
{
	destacaCoresDeEquipamentos = DestacaCores;
}
//---------------------------------------------------------------------------
void CMostraDesenho::SetMostraChegaEquip(bool MostraEquip)
{
	mostraLigacaoEquipamento = MostraEquip;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void CMostraDesenho::SetMostrarPontasDeCaboDescon(bool mostraPontas)
{
	MostrarPontasDeCaboDescon = mostraPontas;
}


void CMostraDesenho::DesenhaArco(float x_center, float y_center, float w,
          float h, float startAngle, float endAngle )
{
	QRectF rectf( x_center - w / 2, y_center - h / 2, w, h );
	_painter->drawArc( rectf, startAngle, endAngle - startAngle );
}
//---------------------------------------------------------------------------


void CMostraDesenho::DesenhaBolaFechada(float x_center, float y_center, float w,
          float h, float startAngle, float arcAngle )
{
	_painter->drawEllipse( QRectF( x_center - w / 2, y_center - h / 2, w, h ) );
}
//---------------------------------------------------------------------------

void CMostraDesenho::EscreveTexto(string texto, TPonto origem, double rotacao, double FatorAltura)
{
	QFont textFont;
	double canvasWidth = _glCoords->getRight() - _glCoords->getLeft();
    textFont.setPixelSize( 4500000 / canvasWidth );
	_painter->setFont( textFont );
	_painter->setPen( QPen(Qt::white) );
	QPointF coords( origem.x, origem.y );
	_painter->rotate( rotacao );
	_painter->drawText( coords, QString( texto.c_str() ) );
}
//---------------------------------------------------------------------------

