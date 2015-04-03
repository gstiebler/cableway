//---------------------------------------------------------------------------

#pragma hdrstop

#include "UVerticesArestas.h"
#include <algorithm>
#include <string>


TAresta::TAresta( string layer ) :
	_layer( layer )
{
}
//---------------------------------------------------------------------------

TArestasCircuito::TArestasCircuito()
{
}
//---------------------------------------------------------------------------

TArestasCircuito::~TArestasCircuito()
{
}
//---------------------------------------------------------------------------
TArestasCircuito::TArestasCircuito(const TArestasCircuito &cpy)
{
    Arestas.assign( cpy.Arestas.begin(), cpy.Arestas.end() );
	ArestasDesenho = cpy.ArestasDesenho;
}
//---------------------------------------------------------------------------


void TArestasCircuito::calcArestasDesenho()
{
	ArestasDesenho.clear();
	for( int i(0); i < Arestas.size(); ++i )
	{
		if( Arestas[i]->_drawing.get() != 0)
			ArestasDesenho[Arestas[i]->_drawing.get()].push_back( Arestas[i] );
	}
}


TListaVerticesEArestas::TListaVerticesEArestas(const TListaVerticesEArestas &cpy)
{
	try
	{
		list.assign( cpy.list.begin(), cpy.list.end() );
	}
	catch(...)
	{
		printf( "Erro!\n" );
	}
}


TVerticeGeral::TVerticeGeral()
{
    pos.x = 0;
    pos.y = 0;
    TipoElemento = NADA;
    texto = "";
    ListaVerticesEArestas = new TListaVerticesEArestas;
    EhColar = false;
}
//---------------------------------------------------------------------------

TVerticeGeral::~TVerticeGeral()
{
    delete ListaVerticesEArestas;
}
//---------------------------------------------------------------------------
TVerticeGeral::TVerticeGeral(const TVerticeGeral &cpy)
{
    pos.x = cpy.pos.x;
    pos.y = cpy.pos.y;
    TipoElemento = cpy.TipoElemento;
    texto = cpy.texto;
    drawing = cpy.drawing;
    IndiceOriginal = cpy.IndiceOriginal;
    EhColar = cpy.EhColar;
    TipoVertice = cpy.TipoVertice;
    ListaVerticesEArestas = new TListaVerticesEArestas( *cpy.ListaVerticesEArestas );
}
//---------------------------------------------------------------------------


void TVerticesGerais::Adiciona(shared_ptr<TVerticeGeral> Item)
{
	vertices.push_back( Item );
}
//---------------------------------------------------------------------------

shared_ptr<TVerticeGeral> TVerticesGerais::AchaVerticePeloTexto(string Texto)
{
    int n;
    string texto1, texto2;
    texto1 = Texto;
    std::transform( Texto.begin(), Texto.end(), texto1.begin(), ::toupper );

	for (n = 0; n < vertices.size(); n++)
    {
        texto2 = vertices[ n ]->texto;
        transform( vertices[ n ]->texto.begin(), vertices[ n ]->texto.end(), texto2.begin(),
                ::toupper );
//    if (getItem(n)->texto==Texto)
        //    if (getItem(n)->texto.UpperCase()==Texto.UpperCase())
        if (texto1 == texto2)
            return vertices[n];
    }

    return shared_ptr<TVerticeGeral>();
}
//---------------------------------------------------------------------------


void TVerticesGerais::ListaOrd(vector<shared_ptr<TVerticeGeral> > &ListaOrdenada)
{
    int n;
	//TODO não alocar nada aqui para não precisar desalocar em quem chamar essa função
	ListaOrdenada.assign( vertices.begin(), vertices.end() );

    for (n = 0; n < (int) ListaOrdenada.size(); n++)
        ListaOrdenada[ n ]->IndiceOriginal = n;
    sort( ListaOrdenada.begin(), ListaOrdenada.end(), ComparaTVerticeGeral );
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void TAresta::AdicionaVertices(shared_ptr<TVerticeGeral> v1, shared_ptr<TVerticeGeral> v2, double dist)
{
    _vertices[0] = v1;
    _vertices[1] = v2;
    Tam = dist;
}
//---------------------------------------------------------------------------   
//---------------------------------------------------------------------------

void TListaVerticesEArestas::AdicionaVerticeEAresta(shared_ptr<TVerticeGeral> vertice, shared_ptr<TAresta> aresta)
{
    TVerticeEAresta temp;
    temp.Vertice = vertice;
    temp.Aresta = aresta;
    list.push_back( temp );
}
//---------------------------------------------------------------------------   

TVerticeEAresta * TListaVerticesEArestas::getVerticeEAresta(int Indice)
{
    return &list[Indice];
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

bool ComparaTVerticeGeral(shared_ptr<TVerticeGeral> Item1, shared_ptr<TVerticeGeral> Item2)
{
    return (Item1->texto < Item2->texto);
}
//---------------------------------------------------------------------------
