//---------------------------------------------------------------------------
#pragma hdrstop
#include "UMostraDesenho.h"
#include <UGrafoDesenho.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)

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


CMostraDesenho::CMostraDesenho(CGrafoDesenho *grafoDesenho, CInfoCircuitos *infoCircuitos, QWidget *parent) :
        COpenGL(600, 600, parent),
        bMostraArvore2( false ),
        CircuitoAExibir( -1 ),
        xBola( -1 ),
        yBola( - 1),
        tamBola( -1 ),
        VerticeArvore( -1 ),
        VerticeArvore2( -1 )
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



void CMostraDesenho::paintGL()
{  
    Paint();    
}



void CMostraDesenho::initializeLimits()
{	
	float x, y;
	_glCoords.initializeLimits();
	if (GrafoDesenho->ult > GrafoDesenho->Dados->Multipoint.size())
		GrafoDesenho->ult = GrafoDesenho->Dados->Multipoint.size();
	for (int n=GrafoDesenho->pri; n<GrafoDesenho->ult; n++)
	{
		vector<TPonto> &points = GrafoDesenho->Dados->Multipoint[n].pontos;
		int numPoints = points.size();
		for (int i=0; i < numPoints; i++)
		{
			x = points[i].x;
			y = points[i].y;

			_glCoords.updateLimits( x, y );
		}
	}
	for ( int i=0; i<GrafoDesenho->Dados->Arcos.size(); i++)
	{
		if ( fabs(360.0 - GrafoDesenho->Dados->Arcos[i].AngTam) < 0.001 )
		{
			x = GrafoDesenho->Dados->Arcos[i].Centro.x;
			y = GrafoDesenho->Dados->Arcos[i].Centro.y;

			double raio = GrafoDesenho->Dados->Arcos[i].EixoPrimario;

			_glCoords.updateLimits( x + raio, y + raio );
		}
	}

	_glCoords.updateProportion();
	initialized = true;
}

void CMostraDesenho::setColorFromLevel( int level )
{
	switch ( level )
	{
		case CALHA:
			glColor3f(0.0, 1, 1);
			break;
		case CABO:
			glColor3f(pegaVermelho(CORCABO)/255.0, pegaVerde(CORCABO)/255.0, pegaAzul(CORCABO)/255.0);
			break;
		case BANDEIROLA:
			glColor3f(pegaVermelho(CORBANDEIROLA)/255.0, pegaVerde(CORBANDEIROLA)/255.0, pegaAzul(CORBANDEIROLA)/255.0);
			break;
		case TAG:
			glColor3f(pegaVermelho(CORTAG)/255.0, pegaVerde(CORTAG)/255.0, pegaAzul(CORTAG)/255.0);
			break;
		case INSTRUMENTO:
			glColor3f(pegaVermelho(CORINSTRUMENTO)/255.0, pegaVerde(CORINSTRUMENTO)/255.0, pegaAzul(CORINSTRUMENTO)/255.0);
			break;
		case INSTRUMENTODESCON:
			glColor3f(pegaVermelho(CORINSTRUMENTODESCON)/255.0, pegaVerde(CORINSTRUMENTODESCON)/255.0, pegaAzul(CORINSTRUMENTODESCON)/255.0);
			break;
		case NADA:
		default:
			glColor3f(pegaVermelho(CORNADA)/255.0, pegaVerde(CORNADA)/255.0, pegaAzul(CORNADA)/255.0);
			break;
	}
}

void CMostraDesenho::drawMultipoints()
{
	for (int n=0; n<GrafoDesenho->Dados->Multipoint.size(); n++)
			//    for (int n=GrafoDesenho->pri; n<GrafoDesenho->ult; n++)
	{
		//      byte cor=GrafoDesenho->Dados->Multipoint[n].Cor;
		glColor3f(GrafoDesenho->Dados->Multipoint[n].CorR/255.0, GrafoDesenho->Dados->Multipoint[n].CorG/255.0,
				GrafoDesenho->Dados->Multipoint[n].CorB/255.0);
		//      glColor3f(GrafoDesenho->Dados->TabelaCores[cor][0]/255.0, GrafoDesenho->Dados->TabelaCores[cor][1]/255.0,
		//                                                          GrafoDesenho->Dados->TabelaCores[cor][2]/255.0);
		if (destacaCoresDeEquipamentos)
			setColorFromLevel( GrafoDesenho->Dados->Multipoint[n].Nivel );
    if ( semCores )
    {
			glColor3f(pegaVermelho(CORNADA)/255.0, pegaVerde(CORNADA)/255.0, pegaAzul(CORNADA)/255.0);
    }
		switch (GrafoDesenho->Dados->Multipoint[n].tipo)
		{
			case LINE_CLOSED:
				glBegin(GL_LINE_LOOP);
				break;
			case LINE_OPEN:
				glBegin(GL_LINE_STRIP);
				break;
		}
		//glLineWidth((GLfloat)(1.0+GrafoDesenho->Multipoint[n].Peso));
		//if (GrafoDesenho->Multipoint[n].tam<10)
		{
			for (int i=0; i<GrafoDesenho->Dados->Multipoint[n].pontos.size(); i++)
			{
				double x = GrafoDesenho->Dados->Multipoint[n].pontos[i].x;
				double y = GrafoDesenho->Dados->Multipoint[n].pontos[i].y;
				glVertex2f(x, y);
			}
		}
		glEnd();
	}
}



void CMostraDesenho::drawArcs()
{
	//TArco *Arcos=GrafoDesenho->Arcos;
	vector<TArco> Arcos = GrafoDesenho->Dados->Arcos;
	for (int n=0; n<GrafoDesenho->Dados->Arcos.size(); n++)
	{
		//      byte cor=GrafoDesenho->Dados->Arcos[n].Cor;
		//      glColor3f(GrafoDesenho->Dados->TabelaCores[cor][0]/255.0, GrafoDesenho->Dados->TabelaCores[cor][1]/255.0,
		//                                                          GrafoDesenho->Dados->TabelaCores[cor][2]/255.0);
		glColor3f(GrafoDesenho->Dados->Arcos[n].CorR/255.0, GrafoDesenho->Dados->Arcos[n].CorG/255.0,
				GrafoDesenho->Dados->Arcos[n].CorB/255.0);
		if ( destacaCoresDeEquipamentos )
			setColorFromLevel( GrafoDesenho->Dados->Arcos[n].Nivel );
    if ( semCores )
    {
			glColor3f(pegaVermelho(CORNADA)/255.0, pegaVerde(CORNADA)/255.0, pegaAzul(CORNADA)/255.0);
    }
		DesenhaArco(Arcos[n].Centro.x, Arcos[n].Centro.y, Arcos[n].EixoPrimario, Arcos[n].EixoSecundario,
				LondDegToRad(Arcos[n].AngIni+Arcos[n].Rotacao), LondDegToRad(Arcos[n].AngTam), 20);
	}
}



void CMostraDesenho::showCircuit()
{
	int IndiceDesenho=GrafoDesenho->Dados->IndiceDesenho;
	TVectorInt *iArestas;
	TPonto Pontos[2];
	glColor3f(pegaVermelho(CORCAMINHO)/255.0, pegaVerde(CORCAMINHO)/255.0, pegaAzul(CORCAMINHO)/255.0);
//			glColor3f(0.0, 0.0, 1.0);
	glLineWidth((GLfloat)(3.0));
	iArestas=InfoCircuitos->ArestasCircuito(CircuitoAExibir, IndiceDesenho);
	for (int n=0; n<(int)iArestas->size(); n++)
	{
		if ( ( n == 0 || n == (int)iArestas->size()-1 ) && !mostraLigacaoEquipamento )
			continue;
		InfoCircuitos->PontosAresta(Pontos, (*iArestas)[n]);
		glBegin(GL_LINE_STRIP);
		glVertex2f(Pontos[0].x, Pontos[0].y);
		glVertex2f(Pontos[1].x, Pontos[1].y);
		glEnd();
	}
	glLineWidth((GLfloat)(1.0));
}



void CMostraDesenho::showTree()
{
	// Laranja - Origem

	//			glColor3f(1.0, 0.5, 0.0);
	string origem = GrafoDesenho->VerticesGerais->getItem(VerticeArvore)->texto.c_str();
//			if ( !bMostraArvore2 && ponteiroPraFuncao)
//			    ponteiroPraFuncao(ponteiroProThis, origem , "");
	glColor3f(pegaVermelho(CORARVORE)/255.0, pegaVerde(CORARVORE)/255.0, pegaAzul(CORARVORE)/255.0);
	glLineWidth((GLfloat)(3.0));
	glPushMatrix();
	//			EscreveTexto(("Origem: "+ GrafoDesenho->VerticesGerais->getItem(VerticeArvore)->texto).c_str(), pos, 0,
	//					GrafoDesenho->Dados->Textos[0].FatorAltura*10);
	glPopMatrix();
	int IndiceDesenho=GrafoDesenho->Dados->IndiceDesenho;
	TVectorInt iArestas;
	InfoCircuitos->Arvore(VerticeArvore, iArestas, IndiceDesenho);
	TAresta Aresta;
	TPonto Pontos[2];
	for (int n=0; n<(int)iArestas.size(); n++)
	{
		Aresta=*(GrafoDesenho->Arestas->getItem(iArestas[n]));
		string debugTexto = GrafoDesenho->VerticesGerais->getItem(Aresta.Vertice1)->texto;
		Pontos[0]=GrafoDesenho->VerticesGerais->getItem(Aresta.Vertice1)->pos;
		Pontos[1]=GrafoDesenho->VerticesGerais->getItem(Aresta.Vertice2)->pos;
		glBegin(GL_LINE_STRIP);
		glVertex2f(Pontos[0].x, Pontos[0].y);
		glVertex2f(Pontos[1].x, Pontos[1].y);
		glEnd();
#define TAMBOLACOLAR (1000)
		if ( GrafoDesenho->VerticesGerais->getItem(Aresta.Vertice1)->EhColar )
		{
			DesenhaBolaFechada(Pontos[0].x, Pontos[0].y, _glCoords.getWorldWidth() / TAMBOLACOLAR, _glCoords.getWorldWidth()/TAMBOLACOLAR, 0, 2*M_PI, 20);
		}
		if ( GrafoDesenho->VerticesGerais->getItem(Aresta.Vertice2)->EhColar )
		{
			DesenhaBolaFechada(Pontos[1].x, Pontos[1].y, _glCoords.getWorldWidth()/TAMBOLACOLAR, _glCoords.getWorldWidth()/TAMBOLACOLAR, 0, 2*M_PI, 20);
		}
	}
	glLineWidth((GLfloat)(1.0));
	if (bMostraArvore2)
	{
		// Roxo - Destino
		string destino = GrafoDesenho->VerticesGerais->getItem(VerticeArvore2)->texto.c_str();
//				if ( ponteiroPraFuncao )
//				    ponteiroPraFuncao(ponteiroProThis, origem , destino);
		glColor3f(pegaVermelho(CORARVORE2)/255.0, pegaVerde(CORARVORE2)/255.0, pegaAzul(CORARVORE2)/255.0);
		//			glColor3f(0.7, 0.0, 0.7);
		glLineWidth((GLfloat)(3.0));

		glPushMatrix();
		//				EscreveTexto(("Destino: "+ GrafoDesenho->VerticesGerais->getItem(VerticeArvore2)->texto).c_str(), pos, 0,
		//						GrafoDesenho->Dados->Textos[0].FatorAltura*10);
		glPopMatrix();

		int IndiceDesenho=GrafoDesenho->Dados->IndiceDesenho;
		TVectorInt iArestas;
		InfoCircuitos->Arvore(VerticeArvore2, iArestas, IndiceDesenho);
		TAresta Aresta;
		TPonto Pontos[2];
		for (int n=0; n<(int)iArestas.size(); n++)
		{
			Aresta=*(GrafoDesenho->Arestas->getItem(iArestas[n]));
			Pontos[0]=GrafoDesenho->VerticesGerais->getItem(Aresta.Vertice1)->pos;
			Pontos[1]=GrafoDesenho->VerticesGerais->getItem(Aresta.Vertice2)->pos;
			glBegin(GL_LINE_STRIP);
			glVertex2f(Pontos[0].x, Pontos[0].y);
			glVertex2f(Pontos[1].x, Pontos[1].y);
			glEnd();
			if ( GrafoDesenho->VerticesGerais->getItem(Aresta.Vertice1)->EhColar )
			{
				DesenhaBolaFechada(Pontos[0].x, Pontos[0].y, _glCoords.getWorldWidth()/TAMBOLACOLAR, _glCoords.getWorldWidth()/TAMBOLACOLAR, 0, 2*M_PI, 20);
			}
			if ( GrafoDesenho->VerticesGerais->getItem(Aresta.Vertice2)->EhColar )
			{
				DesenhaBolaFechada(Pontos[1].x, Pontos[1].y, _glCoords.getWorldWidth()/TAMBOLACOLAR, _glCoords.getWorldWidth()/TAMBOLACOLAR, 0, 2*M_PI, 20);
			}
		}
		glLineWidth((GLfloat)(1.0));
	}
}



void CMostraDesenho::showDisconnectedCircuitEndings()
{
	#ifdef DEBUG_BUILDER
	TStringList *proArquivo = new TStringList();
	AnsiString vertices = "";
#endif
	glColor3f(pegaVermelho(CORINSTRUMENTODESCON)/255.0, pegaVerde(CORINSTRUMENTODESCON)/255.0, pegaAzul(CORINSTRUMENTODESCON)/255.0);
	for ( int i = 0 ; i < GrafoDesenho->NumCabosReta ; i++ )
	{
		if ( !GrafoDesenho->CabosReta[i]->ponta[0] )
		{
			TPonto pontos = GrafoDesenho->Dados->Multipoint[GrafoDesenho->CabosReta[i]->Indice].pontos[0];
			DesenhaBolaFechada(pontos.x, pontos.y, GrafoDesenho->DistMinElemCaboPraOpenGL*4, GrafoDesenho->DistMinElemCaboPraOpenGL*4, 0, 2*M_PI, 20);
#ifdef DEBUG_BUILDER
			vertices = "Multipoint 0 "+IntToStr(i)+" X:" + FormatFloat("###,###,###",pontos.x) + " Y:" + FormatFloat("###,###,###",pontos.y);
			proArquivo->Add(vertices);
#endif
		}
		if ( !GrafoDesenho->CabosReta[i]->ponta[1] )
		{
			int tam = GrafoDesenho->Dados->Multipoint[GrafoDesenho->CabosReta[i]->Indice].pontos.size();
			TPonto pontos = GrafoDesenho->Dados->Multipoint[GrafoDesenho->CabosReta[i]->Indice].pontos[tam-1];
			DesenhaBolaFechada(pontos.x, pontos.y, GrafoDesenho->DistMinElemCaboPraOpenGL*4, GrafoDesenho->DistMinElemCaboPraOpenGL*4, 0, 2*M_PI, 20);
#ifdef DEBUG_BUILDER
			vertices = "Multipoint 1 "+IntToStr(i)+" X:" + FormatFloat("###,###,###",pontos.x) + " Y:" + FormatFloat("###,###,###",pontos.y);
			proArquivo->Add(vertices);
#endif
		}
	}

	for ( int i = 0 ; i < GrafoDesenho->NumCabosArco ; i++ )
	{
		TPonto pontos[2];
		GrafoDesenho->Dados->Arcos[GrafoDesenho->CabosArco[i]->Indice].PontasArco(pontos);
		if ( !GrafoDesenho->CabosArco[i]->ponta[0] )
		{
			DesenhaBolaFechada(pontos[0].x, pontos[0].y, GrafoDesenho->DistMinElemCaboPraOpenGL*4, GrafoDesenho->DistMinElemCaboPraOpenGL*4, 0, 2*M_PI, 20);
#ifdef DEBUG_BUILDER
			vertices = "Arcos 0 " + IntToStr(i) +" X:" + FormatFloat("###,###,###",pontos[0].x) + " Y:" + FormatFloat("###,###,###",pontos[0].y);
			proArquivo->Add(vertices);
#endif
		}
		if ( !GrafoDesenho->CabosArco[i]->ponta[1] )
		{
			DesenhaBolaFechada(pontos[1].x, pontos[1].y, GrafoDesenho->DistMinElemCaboPraOpenGL*4, GrafoDesenho->DistMinElemCaboPraOpenGL*4, 0, 2*M_PI, 20);
#ifdef DEBUG_BUILDER
			vertices = "Arcos 1 " + IntToStr(i) +" X:" + FormatFloat("###,###,###",pontos[1].x) + " Y:" + FormatFloat("###,###,###",pontos[1].y);
			proArquivo->Add(vertices);
#endif
		}
	}
#ifdef DEBUG_BUILDER
	proArquivo->SaveToFile(ExtractFilePath(Application->ExeName) + "CabosPonta.txt");
	delete proArquivo;
#endif
}



void CMostraDesenho::showBandeirolaEndings()
{
	for (int n=0; n<(int)GrafoDesenho->PontosPraMostrarBandeirola.size();n++)
	{
		glColor3f(1.0, 1.0, 1.0);
		glLineWidth((GLfloat)(4.0));
		if (DistPontos(GrafoDesenho->PontosPraMostrarBandeirola.at(n).NaBandeirola, GrafoDesenho->PontosPraMostrarBandeirola.at(n).NoCabo) > GrafoDesenho->DistMinElemCaboPraOpenGL) // Se a dist�ncia entre os pontos n�o for muito pequena mostra uma reta
		{
			glBegin(GL_LINE_STRIP);
			glVertex2f(GrafoDesenho->PontosPraMostrarBandeirola.at(n).NaBandeirola.x, GrafoDesenho->PontosPraMostrarBandeirola.at(n).NaBandeirola.y);
			glVertex2f(GrafoDesenho->PontosPraMostrarBandeirola.at(n).NoCabo.x, GrafoDesenho->PontosPraMostrarBandeirola.at(n).NoCabo.y);
			glEnd();
		}
		else // Sen�o, faz um c�rculo em volta dos pontos
		{
			DesenhaArco(GrafoDesenho->PontosPraMostrarBandeirola.at(n).NoCabo.x, GrafoDesenho->PontosPraMostrarBandeirola.at(n).NoCabo.y,
					GrafoDesenho->DistMinElemCaboPraOpenGL*4, GrafoDesenho->DistMinElemCaboPraOpenGL*4, 0, 2*M_PI, 20);
		}
		glLineWidth((GLfloat)(1.0));
	}
}



void CMostraDesenho::drawTexts()
{
	for (int n=0; n<GrafoDesenho->Dados->Textos.size(); n++)
	{
		TTexto &texto = GrafoDesenho->Dados->Textos[n];

		if ( destacaCoresDeEquipamentos )
			setColorFromLevel( texto.Nivel );
		else if ( semCores )
			glColor3f(pegaVermelho(CORNADA)/255.0, pegaVerde(CORNADA)/255.0, pegaAzul(CORNADA)/255.0);
		else
			glColor3f( texto.CorR/255.0, texto.CorG/255.0, texto.CorB/255.0);

		glPushMatrix();

		EscreveTexto(texto.texto, texto.origem, texto.rotacao, texto.FatorAltura);
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

//		if (bMostraNumVerticesDEBUG)
//		{
//			randomize();
//			for (int n=1; n<GrafoDesenho->VerticesGerais->Tamanho(); n++)
//			{
//				int IndiceDesenho=GrafoDesenho->Dados->IndiceDesenho;
//
//				if ( GrafoDesenho->VerticesGerais->getItem(n)->iDesenho == IndiceDesenho )
//				{
//
//					glColor3f(GeraCor(), GeraCor(), GeraCor());
//					glPushMatrix();
//					char texto[10]={0};
//					sprintf(texto, "%d", n);
//          TPonto ponto_debug=GrafoDesenho->VerticesGerais->getItem(n)->pos;
//          ponto_debug.x += random(5);
//          ponto_debug.y += random(5);
//					EscreveTexto(texto, GrafoDesenho->VerticesGerais->getItem(n)->pos, 0,
//							GrafoDesenho->Dados->Textos[0].FatorAltura/2);
//					//                          FATOR_TEXTO_NUM_VERTICES/fator);
//					glPopMatrix();
//				}
//			}
//		}
		if (bMostraBola)
		{
			glColor3f(0.0, 0.0, 1.0);
			DesenhaBolaFechada(xBola, yBola, tamBola, tamBola, 0, 2*M_PI, 20);
		}
	//	glEndList();
	//}
	//else
	//	glCallList(1);
	primeiro=false;
	glPopMatrix();   
}
//---------------------------------------------------------------------------

void CMostraDesenho::MostraCircuito(int Circuito)
{
	ExibirCircuito=true;
	primeiro=true;
	CircuitoAExibir=Circuito;
}
//---------------------------------------------------------------------------

void CMostraDesenho::MostraNumVerticesDEBUG(bool mostra)
{
	primeiro=true;
	bMostraNumVerticesDEBUG=mostra;
}
//---------------------------------------------------------------------------

void CMostraDesenho::MostraArvore(int vertice)
{
	primeiro=true;
	VerticeArvore=vertice;
	bMostraArvore=true;
	bMostraArvore2=false;
}
//---------------------------------------------------------------------------

void CMostraDesenho::MostraDoubleArvore(int vertice, int vertice2)
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

void CMostraDesenho::MostraBola(double x, double y, double tam)
{
	primeiro=true;
	bMostraBola=true;
	xBola=x;
	yBola=y;
	tamBola=tam * (_glCoords.getWorldWidth() /100);
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
