//---------------------------------------------------------------------------

#pragma hdrstop

#include "UVerticesArestas.h"
#include <algorithm>
#include <string>


TAresta::TAresta( string layer ) :
	_layer( layer )
{
    IndiceDesenho = I_DESENHO_NULO;
    IDArquivo = I_DESENHO_NULO;
}
//---------------------------------------------------------------------------

TArestasCircuito::TArestasCircuito(int numDesenhos)
{
    NumDesenhos = numDesenhos;
	ArestasDesenho.resize( NumDesenhos );
}
//---------------------------------------------------------------------------

TArestasCircuito::~TArestasCircuito()
{
}
//---------------------------------------------------------------------------
TArestasCircuito::TArestasCircuito(const TArestasCircuito &cpy)
{
    Circuito = cpy.Circuito;

    Arestas.assign( cpy.Arestas.begin(), cpy.Arestas.end() );
    NumDesenhos = cpy.NumDesenhos;
//    NumDesenhos=numDesenhos;
    idCircuito = cpy.idCircuito;
	ArestasDesenho.resize( NumDesenhos );
    ArestasDesenho.assign( cpy.ArestasDesenho.begin(), cpy.ArestasDesenho.end() );
}
//---------------------------------------------------------------------------

void TArestasCircuito::Apaga()
{
    for (int n = 0; n < NumDesenhos; n++)
        ArestasDesenho[n].clear();
    Arestas.clear();
    Circuito = "";
    idCircuito = -1;
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
    iDesenho = 0;
    IDArquivo = I_DESENHO_NULO;
    TipoElemento = NADA;
    dist = 0;
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
    IDArquivo = cpy.IDArquivo;
    TipoElemento = cpy.TipoElemento;
    dist = cpy.dist;
    texto = cpy.texto;
    iDesenho = cpy.iDesenho;
    IndiceOriginal = cpy.IndiceOriginal;
    EhColar = cpy.EhColar;
    TipoVertice = cpy.TipoVertice;
    ListaVerticesEArestas = new TListaVerticesEArestas( *cpy.ListaVerticesEArestas );
}
//---------------------------------------------------------------------------


TVerticesGerais::TVerticesGerais()
{
}


void TVerticesGerais::Adiciona(TVerticeGeral &Item)
{
    shared_ptr<TVerticeGeral> temp( new TVerticeGeral() );
    TListaVerticesEArestas *ListaVerticesEArestasT = temp->ListaVerticesEArestas;
    *temp = Item;
    temp->ListaVerticesEArestas = ListaVerticesEArestasT;
//  lista->Add((void *)temp);
	vertices.push_back( temp );
}
//---------------------------------------------------------------------------

int TVerticesGerais::AchaVerticePeloTexto(string Texto)
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
            return n;
    }

    return -1;
}
//---------------------------------------------------------------------------

bool TVerticesGerais::VerticeEhEquipamento(int n)
{
    if (vertices[ n ]->TipoElemento == INSTRUMENTO)
        return true;
    return false;
}
//---------------------------------------------------------------------------

bool TVerticesGerais::VerticeEhBandeirola(int n)
{
    if (vertices[ n ]->TipoElemento == BANDEIROLA)
        return true;
    return false;
}
//---------------------------------------------------------------------------

void TVerticesGerais::ListaOrd(vector<shared_ptr<TVerticeGeral> > &ListaOrdenada)
{
    int n;
	//TODO n�o alocar nada aqui para n�o precisar desalocar em quem chamar essa fun��o
	ListaOrdenada.assign( vertices.begin(), vertices.end() );

    for (n = 0; n < (int) ListaOrdenada.size(); n++)
        ListaOrdenada[ n ]->IndiceOriginal = n;
    sort( ListaOrdenada.begin(), ListaOrdenada.end(), ComparaTVerticeGeral );
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void TAresta::AdicionaVertices(int v1, int v2, double dist)
{
    Vertice1 = v1;
    Vertice2 = v2;
    Tam = dist;
}
//---------------------------------------------------------------------------   
//---------------------------------------------------------------------------

void TListaVerticesEArestas::AdicionaVerticeEAresta(int vertice, int aresta)
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
