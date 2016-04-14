//---------------------------------------------------------------------------
#pragma hdrstop
#include "UInfoCircuitos.h"
#include <queue>

#include "UGeometria.h"
#include "UInfoCircuitos.h"

#include "UArmazenamentoCircuitos.h"
#include "Graph.h"
#include "Debug.h"

using namespace std;

CInfoCircuitos::CInfoCircuitos( shared_ptr<Graph> graph ) :
	_graph( graph )
{
}
//---------------------------------------------------------------------------

CInfoCircuitos::~CInfoCircuitos()
{
}
//---------------------------------------------------------------------------



void CInfoCircuitos::AdicionaCircuito( TCircuito &Circuito )
{
	int m;
	shared_ptr<TAresta> Aresta;
	shared_ptr<TArestasCircuito> ArestasCircuito = make_shared<TArestasCircuito>();
	_circuitEdges[Circuito.NomeCircuito] = ArestasCircuito;
	bool erro = true, erro_interno;

	erro_interno = false;
	try
	{
	  // Chama desse jeito se não tiver rota do usuário
		if ( Circuito.RotaUsuario == "" )
		{
			erro = GeraRota(Circuito.Destino, Circuito.Origem, Circuito.metragem, Circuito.rota, ArestasCircuito->Arestas);
			ArestasCircuito->calcArestasDesenho();
		}
		// E assim caso tenha..
		else
		{
			erro = GeraRota(Circuito.Destino, Circuito.Origem, Circuito.RotaUsuario, Circuito.metragem, Circuito.rota, ArestasCircuito );
			ArestasCircuito->calcArestasDesenho();
		}
		if( erro )
			Circuito.metragem = 0.0;
	  }
	  catch(...)
	  {
		erro_interno = true;
	  }
}
//---------------------------------------------------------------------------


vector< shared_ptr<TAresta> >& CInfoCircuitos::ArestasCircuito(std::string circuitName, shared_ptr<TDesenho> drawing) const
{
	std::map<std::string, shared_ptr<TArestasCircuito> >::const_iterator it = _circuitEdges.find( circuitName );
    return it->second->ArestasDesenho[drawing.get()];
}
//---------------------------------------------------------------------------

void CInfoCircuitos::PontosAresta(TPonto Pontos[2], shared_ptr<TAresta> Aresta)
{
	Pontos[0] = Aresta->_vertices[0]->pos;
    Pontos[1] = Aresta->_vertices[1]->pos;
}

//---------------------------------------------------------------------------

void CInfoCircuitos::SeparaRota(string ListaPontos, vector<string> &ListaRota)
{
    size_t pos;
    while (true)
    {
        // Procura uma barra na lista..
        pos = ListaPontos.find( "/" );
        // Se não achar, então sai.
        if (pos == string::npos)
            break;
        // Adiciona a substring sem a "/" na lista..
        string temp = ListaPontos.substr( 0, pos );
        ListaRota.push_back( ListaPontos.substr( 0, pos ) );
        // E tira a substring da lista..
        ListaPontos = ListaPontos.substr( pos + 1 );
    }
    // No fim, bota o que sobrou na lista..
    ListaRota.push_back( ListaPontos );
}
//---------------------------------------------------------------------------

void CInfoCircuitos::MergeRota(vector<std::string> &rota, vector<std::string> NovaParte)
{
    size_t pos = 0;

    // Se não tiver rota
    if (rota.size() == 0)
    {
        // então a rota � s� a nova parte..
        rota = NovaParte;
        return;
    }
	rota.pop_back();

    // a rota agora � a rota com o destino substitu�do pela nova parte.
    rota.insert( rota.end(), NovaParte.begin(), NovaParte.end() );
}
//---------------------------------------------------------------------------

//Recebe a origem e o destino, e toda a rota do usuário
bool CInfoCircuitos::GeraRota(string Destino, string Origem, string ListaPontos, double &tam, vector<string> &rota, 
							  shared_ptr<TArestasCircuito> ArestasCircuito) const
{
    vector<string> ListaRota;
    int i;
    vector<string> rotaTemp;
    double tamTemp;
    bool erro = false;

    ListaRota.push_back(Origem);
    SeparaRota(ListaPontos, ListaRota);
    ListaRota.push_back(Destino);

	rota.clear();
    tam = 0;

    bool ultimo = true;

    for ( i = 0; i < (int)(ListaRota.size()-1); i++ )
    {
        bool erroNessaRota = false;
        tamTemp = 0;
        rotaTemp.clear();
		if (GeraRota(ListaRota[i+1], ListaRota[i], tamTemp, rotaTemp, ArestasCircuito->Arestas))
        {
			ArestasCircuito->calcArestasDesenho();
            erroNessaRota = erro = true;
        }
        if ( !erroNessaRota )
        {
            MergeRota(rota, rotaTemp);
            tam += tamTemp;
        }
        else
        {
            if ( ultimo )
			{
				vector<string> rotaTemp( 2 );
				rotaTemp[0] = "..";
				rotaTemp[1] = "..";
				MergeRota(rota, rotaTemp);
			}
        }
        ultimo = erroNessaRota;
    }

    return erro;
}
//---------------------------------------------------------------------------


set<string> CInfoCircuitos::getLevelsFromVertex( shared_ptr<TVerticeGeral> vertexIndex ) const
{
    shared_ptr<TListaVerticesEArestas> verticeEArestaTemp = vertexIndex->ListaVerticesEArestas;
	set<string> result;
	for (int i(0); i < verticeEArestaTemp->list.size(); ++i)
	{
		shared_ptr<TAresta> Aresta = verticeEArestaTemp->list[i].Aresta;
		result.insert( Aresta->_layer );
	}
	return result;
}


bool CInfoCircuitos::GeraRota(string Destino, string Origem, double &tam, vector<string> &rota, vector< shared_ptr<TAresta> > &edgesList ) const
{
    int n, m;
	shared_ptr<TVerticeGeral> vertice[2];
    string V[2];
    V[0] = Destino;
    V[1] = Origem;

    for (m=0; m<2; m++)
    {
        // vertice[] guarda os índices dos vértices, já que eles são passados pelo nome.
		vertice[m] = _graph->_verticesGerais->AchaVerticePeloTexto(V[m]);
        if (vertice[m].get() == 0)
			return 1;
    }
    if(vertice[0].get() == vertice[1].get())
    {
		rota.clear();
        tam = 0.0;
        return 1;
    }

	string selectedLayer;
	set<string> levelsFrom1 = getLevelsFromVertex( vertice[0] );
	set<string> levelsFrom2 = getLevelsFromVertex( vertice[1] );
	set<string>::iterator it, e = levelsFrom1.end();
	for( it = levelsFrom1.begin(); it != e; ++it)
	{
		if ( levelsFrom2.find( *it ) != levelsFrom2.end() )
			selectedLayer = *it;
	}

    vector< shared_ptr<TVerticeGeral> > anterior( _graph->_verticesGerais->vertices.size() );
    vector< shared_ptr<TAresta> > vArestas( _graph->_verticesGerais->vertices.size() );//armazena a aresta de cada vértice referente em PaisVertices
	bool achou_final = _graph->generateDistanceTree( vertice, anterior, vArestas, selectedLayer );
	
    if(achou_final)
    {
		_graph->getEdgesFromPath( anterior, vArestas, edgesList, rota, vertice[1] );
		rota.push_back( Destino );
		
		tam = 0;
		for( auto edge : edgesList )
			tam += edge->Tam;

		Debug::printEdges( Origem, Destino, edgesList );
    }
    else
		tam=0;

	if ( rota.size() == 0 )
		return 1;

    return 0;
}
//---------------------------------------------------------------------------


void CInfoCircuitos::Arvore( shared_ptr<TVerticeGeral> Vertice, std::vector< shared_ptr<TAresta> > &ListaArestas, shared_ptr<TDesenho> drawing) const
{
    int n;
    queue<TVerticeGeral*> fila;
    //fila=new CFila(VerticesGerais->Tamanho());
    TVerticeGeral *vfila, *vatual;
    shared_ptr<TAresta> ArestaTemp;
    set<TVerticeGeral*> VerticesVisitados;
	fila.push( Vertice.get() );
    VerticesVisitados.insert( Vertice.get() );
    //BUSCA EM LARGURA
    shared_ptr<TListaVerticesEArestas> ListaVerticesEArestas;
    while (fila.size())
    {
        vfila = fila.front();
        fila.pop();
        ListaVerticesEArestas = vfila->ListaVerticesEArestas;
        for (n = 0; n < ListaVerticesEArestas->list.size(); n++)
        {
            //vfila � o vértice que queremos a lista de adjac�ncia
            //n � o en�simo vértice na lista de adjac�ncia de vfila
            //.Vertice � pq podemos pegar também .Aresta
			vatual = ListaVerticesEArestas->getVerticeEAresta( n )->Vertice.get();
            ArestaTemp = ListaVerticesEArestas->getVerticeEAresta( n )->Aresta;
            // ListaArestas � a lista de arestas da �rvore no desenho atual
            // A �rvore � regerada para cada desenho
			if (ArestaTemp->_drawing.get() == drawing.get())
                ListaArestas.push_back( ArestaTemp );
            if ( VerticesVisitados.find( vatual ) == VerticesVisitados.end() )
            {
				VerticesVisitados.insert( vatual );
                fila.push( vatual );
            }
        }
    }
}
//---------------------------------------------------------------------------

//--------------------------------------------------------------------------
