//---------------------------------------------------------------------------

#pragma hdrstop

#include "UVerticesArestas.h"
#include <algorithm>
#include <string>

int TAresta::counter = 1;
int TVerticeGeral::counter = 1;

using namespace std;

TAresta::TAresta( string layer ) :
	Tam( 0.0 ),
	_layer( layer )
{
	_autogenId = counter++;
}
//---------------------------------------------------------------------------


TAresta::TAresta( TAresta *other )
{
	*this = *other;
}


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


void TListaVerticesEArestas::removeEdge( TVerticeGeral *vertex )
{
	for( int i(list.size() - 1); i >= 0; --i)
	{
		if( list[i].Vertice.get() == vertex )
		{
			list.erase( list.begin() + i );
			return;
		}
	}
}


TVerticeGeral::TVerticeGeral()
{
    pos.x = 0;
    pos.y = 0;
    TipoElemento = NADA;
    texto = "";
    ListaVerticesEArestas = make_shared<TListaVerticesEArestas>();
    EhColar = false;
	_autogenId = counter++;
}
//---------------------------------------------------------------------------


void TVerticeGeral::removeEdges()
{
	for( int i(0); i < ListaVerticesEArestas->list.size(); ++i )
	{
		shared_ptr<TVerticeGeral> otherVertex = ListaVerticesEArestas->list[i].Aresta->otherVertex( this );
		otherVertex->ListaVerticesEArestas->removeEdge( this );
	}
}


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


std::shared_ptr<TVerticeGeral> TAresta::otherVertex( TVerticeGeral *vertex )
{
	if( _vertices[0].get() == vertex )
		return _vertices[1];
	else
		return _vertices[0];
}


void TAresta::createAdjancency( shared_ptr<TAresta> edge )
{
	shared_ptr<TVerticeGeral> v1 = edge->_vertices[0];
	shared_ptr<TVerticeGeral> v2 = edge->_vertices[1];

	v1->ListaVerticesEArestas->AdicionaVerticeEAresta( v2, edge );
	v2->ListaVerticesEArestas->AdicionaVerticeEAresta( v1, edge );
}


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
