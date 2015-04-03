//---------------------------------------------------------------------------
#pragma hdrstop
#include "UInfoCircuitos.h"

#include <queue>

#include "UGeometria.h"
#include "UInfoCircuitos.h"

#include "UArmazenamentoCircuitos.h"
#include "Graph.h"

using namespace std;

void TListaCircuitos::AdicionaCircuito(string Circuito)
{
    Circuitos->push_back( Circuito );
    NumCircuitos++;
}
//---------------------------------------------------------------------------

TListaCircuitos::TListaCircuitos()
{
    NumCircuitos = 0;
    Circuitos = new vector<string>;
}
//---------------------------------------------------------------------------

TListaCircuitos::~TListaCircuitos()
{
    NumCircuitos = 0;
    delete Circuitos;
}
//---------------------------------------------------------------------------



CInfoCircuitos::CInfoCircuitos( shared_ptr<Graph> graph ) :
	_graph( graph )
{
}
//---------------------------------------------------------------------------

CInfoCircuitos::~CInfoCircuitos()
{
}
//---------------------------------------------------------------------------



void CInfoCircuitos::AdicionaCircuito( TCircuito &Circuito, int numDrawings )
{
	int m;
	shared_ptr<TAresta> Aresta;
	TCircuitoAreas *CircuitoAreas;
	CircuitoAreas = new TCircuitoAreas[numDrawings];
	shared_ptr<TArestasCircuito> ArestasCircuito( new TArestasCircuito );
	string rota, SubRotas;
	vector< shared_ptr<TVerticeGeral> > Bandeirolas;
	ArestasDoCircuito[Circuito.NomeCircuito] = ArestasCircuito;
	TStringList *DebugArestas=NULL;
	bool erro = true, erro_interno;
#ifdef DEBUG_BUILDER
  DebugArestas = new TStringList();
//	DebugArestas=new vector<string>;
#endif

  //CTempoExec *tempo = CTempoExec::getInstance();
  //tempo->MarcaTempo("Circuito: "+Circuito.NomeCircuito);

  erro_interno = false;
  try
  {
	  // Chama desse jeito se não tiver rota do usuário
	  if ( Circuito.RotaUsuario == "" )
	  {
      erro = GeraRota(Circuito.Destino, Circuito.Origem, Circuito.metragem, Circuito.rota, ArestasCircuito, Bandeirolas, SubRotas);
    }
	  // E assim caso tenha..
	  else
	  {
		  erro = GeraRota(Circuito.Destino, Circuito.Origem, Circuito.RotaUsuario, Circuito.metragem, Circuito.rota, ArestasCircuito, Bandeirolas, DebugArestas, CircuitoAreas);
	  }
	  if( erro )
		  Circuito.metragem = 0.0;
  }
  catch(...)
  {
    erro_interno = true;
  }
  //tempo->MarcaTempo((string)"Fim da Gera��o" + (erro?" Nao achou":" Achou"));

	if ( !erro )
	{

#ifdef DEBUG_BUILDER
		DebugArestas->SaveToFile(ExtractFilePath(Application->ExeName)+"Vertices\\"+Circuito.NomeCircuito.c_str()+".txt");
		delete DebugArestas;
#endif
	}
	else if ( erro && Circuito.RotaUsuario != "" && Circuito.rota.size() > 0 )
  {
	}
	else
	{
		if(erro_interno)
		{        
		  //CArmazenamentoCircuitos::DeuErro(Circuito, "Erro interno. Favor contactar o administrador.");
		}
		else
		{          
		}
	}

	//delete Bandeirolas;
	delete[] CircuitoAreas;
}
//---------------------------------------------------------------------------

int CInfoCircuitos::ArestaDoPonto(TPonto ponto, TPonto &PontoNaReta, shared_ptr<TDesenho> drawing)
{
    int m;
    double MenorDist, Dist, DistPonta1, DistPonta2;
    TPonto Reta[2], retorno;
    int IndiceAresta = -1;
    MenorDist = Infinity;
    for (m = 0; m < _graph->_arestas.size(); m++)
    {
        if (_graph->_arestas[ m ]->_drawing != drawing)
            continue;
		Reta[0] = _graph->_arestas[ m ]->_vertices[0]->pos;
		Reta[1] = _graph->_arestas[ m ]->_vertices[1]->pos;

        Dist = DistPontoParaSegmentoReta( Reta, ponto, retorno );

        if (Dist < MenorDist)
        {
            MenorDist = Dist;
            IndiceAresta = m;
            PontoNaReta = retorno;
        }
    }	//for (m=0; m<NumCabosReta; m++)
    return IndiceAresta;
}
//---------------------------------------------------------------------------
VerticesDjikstra::VerticesDjikstra(shared_ptr<TVerticeGeral> vertex, double Distancia)
{
    _vertex = vertex;
    distancia = Distancia;
}

bool VerticesDjikstra::operator<(const VerticesDjikstra& right) const
{
    return distancia > right.distancia;
}

bool VerticesDjikstra::operator>(const VerticesDjikstra& right) const
{
    return distancia < right.distancia;
}


vector< shared_ptr<TAresta> >& CInfoCircuitos::ArestasCircuito(std::string circuitName, shared_ptr<TDesenho> drawing)
{
    return ArestasDoCircuito[circuitName]->ArestasDesenho[drawing.get()];
}
//---------------------------------------------------------------------------

void CInfoCircuitos::PontosAresta(TPonto Pontos[2], shared_ptr<TAresta> Aresta)
{
	Pontos[0] = Aresta->_vertices[0]->pos;
    Pontos[1] = Aresta->_vertices[1]->pos;
}

//---------------------------------------------------------------------------

void CInfoCircuitos::SeparaRota(string ListaPontos, vector<string> *ListaRota)
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
        ListaRota->push_back( ListaPontos.substr( 0, pos ) );
        // E tira a substring da lista..
        ListaPontos = ListaPontos.substr( pos + 1 );
    }
    // No fim, bota o que sobrou na lista..
    ListaRota->push_back( ListaPontos );
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
        shared_ptr<TArestasCircuito> ArestasCircuito, vector< shared_ptr<TVerticeGeral> > &ListaBandeirolas,
        TStringList*DEBUG_arestas, TCircuitoAreas *CircuitoAreas)
{
    vector<string> * ListaRota = new vector<string>();
    int i;
    vector<string> rotaTemp;
	string SubRotasTemp;
    double tamTemp;
    bool erro = false;

    ListaRota->push_back(Origem);
    SeparaRota(ListaPontos, ListaRota);
    ListaRota->push_back(Destino);

	rota.clear();
    tam = 0;

    bool ultimo = true;

    for ( i = 0; i < (int)(ListaRota->size()-1); i++ )
    {
        bool erroNessaRota = false;
        tamTemp = 0;
        rotaTemp.clear();
        if (GeraRota(ListaRota->at(i+1), ListaRota->at(i), tamTemp, rotaTemp, ArestasCircuito, ListaBandeirolas, SubRotasTemp))
        {
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

    delete ListaRota;

    return erro;
}
//---------------------------------------------------------------------------


set<string> CInfoCircuitos::getLevelsFromVertex( shared_ptr<TVerticeGeral> vertexIndex )
{
    TListaVerticesEArestas *verticeEArestaTemp = vertexIndex->ListaVerticesEArestas;
	set<string> result;
	for (int i(0); i < verticeEArestaTemp->list.size(); ++i)
	{
		shared_ptr<TAresta> Aresta = verticeEArestaTemp->list[i].Aresta;
		result.insert( Aresta->_layer );
	}
	return result;
}


bool CInfoCircuitos::GeraRota(string Destino, string Origem, double &tam, vector<string> &rota, shared_ptr<TArestasCircuito> ArestasCircuito,
        vector< shared_ptr<TVerticeGeral> > &ListaBandeirolas, string &SubRotas )
{
    int n, m;
	shared_ptr<TVerticeGeral> vertice[2];
    vector< shared_ptr<TAresta> > *ListaArestas=NULL;
    map< TDesenho*, vector< shared_ptr<TAresta> > > &ArestasDesenho = ArestasCircuito->ArestasDesenho;
    string UltTemp="";
    string V[2];
    V[0] = Destino;
    V[1] = Origem;

    // CTempoExec *tempo = CTempoExec::getInstance();

    if (ArestasCircuito)
    {
        /*
         * ArestasCircuito � um vetor que vai guardar o caminho feito, em termos de aresta, para encontrar o menor caminho.
         * ListaArestas guarda todas as arestas, enquanto ArestasDesenho guarda as arestas espec�ficas de cada desenho
         * A grande diferen�a � que no ListaArestas também estáo as arestas entre desenhos.
         */
        ListaArestas = &ArestasCircuito->Arestas;
    }

    //tempo->MarcaTempo("Vai achar vértices");
    for (m=0; m<2; m++)
    {
        /*
         * vertice[] guarda os índices dos vértices, já que eles são passados pelo nome.
         */
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
		{
			selectedLayer = *it;
		}
	}

    vector< shared_ptr<TVerticeGeral> > anterior( _graph->_verticesGerais->vertices.size() );
    vector< shared_ptr<TAresta> > vArestas( _graph->_verticesGerais->vertices.size() );//armazena a aresta de cada vértice referente em PaisVertices
	bool achou_final = CInfoCircuitos::generateDistanceTree( vertice, anterior, vArestas, selectedLayer );
	
	vector<string> sRota;
    if(achou_final)
    {
		shared_ptr<TAresta> ArestaTemp;
		double TamSubRota=0;
		//CAMINHO INVERSO NA �RVORE DE LARGURA
		shared_ptr<TVerticeGeral> vatual = vertice[1];
		tam = vertice[0]->dist + vertice[1]->dist;
		string temp;

		while (anterior[vatual->IndiceOriginal].get() != 0)
        {
			ArestaTemp = vArestas[vatual->IndiceOriginal];
            if ( ListaArestas )
				ListaArestas->push_back( ArestaTemp );

            if (ArestasCircuito && ArestaTemp->_drawing.get() != 0)
				ArestasDesenho[ArestaTemp->_drawing.get()].push_back( ArestaTemp );

			tam += vArestas[vatual->IndiceOriginal]->Tam;
			TamSubRota += vArestas[vatual->IndiceOriginal]->Tam;


            if (vatual->texto != "")
            {
                //if ( VerticesGerais->getItem(vatual)->TipoElemento!=INSTRUMENTO || (VerticesGerais->getItem(vatual)->texto.UpperCase()==Origem.UpperCase() || VerticesGerais->getItem(vatual)->texto.UpperCase()==Destino.UpperCase()))
                {
                    temp = vatual->texto;
                    if (UltTemp!=temp && temp != Destino)
						sRota.push_back( temp );
                    UltTemp=temp;
                    SubRotas+=to_string(TamSubRota)+"/";
                    TamSubRota=0;
                    if ( (vatual->TipoElemento==BANDEIROLA) || vatual->EhColar)
						ListaBandeirolas.push_back(vatual);
                }
            }
			vatual = anterior[vatual->IndiceOriginal];
        }
        sRota.push_back( V[0] );
    }
    else
    tam=0;

    rota = sRota;

	if ( rota.size() == 0 )
		return 1;

    return 0;
}
//---------------------------------------------------------------------------


bool CInfoCircuitos::generateDistanceTree( shared_ptr<TVerticeGeral> vertice[2], vector< shared_ptr<TVerticeGeral> > &anterior, vector< shared_ptr<TAresta> > &vArestas, string layer )
{
	//vector<int> PaisVertices( VerticesGerais->vertices.size() );//armazena os pais de cada vértice na �rvore
	vector<double> DistanciaDjikstra( _graph->_verticesGerais->vertices.size() );
    shared_ptr<TVerticeGeral> VerticeTemp;
    TVerticeEAresta *VerticeEArestaTemp;
	shared_ptr<TVerticeGeral> vfila, vatual;
    double dist;
    TListaVerticesEArestas *ListaVerticesEArestasT;
    priority_queue<VerticesDjikstra> heap;
    heap.push(VerticesDjikstra(vertice[0], 0));
	bool achou_final = false;

	    /* initialize single source */
    for ( int n = 0; n < _graph->_verticesGerais->vertices.size(); n++ )
    {
        DistanciaDjikstra[n] = Infinity;
        anterior[n] = shared_ptr<TVerticeGeral>();
		_graph->_verticesGerais->vertices[n]->IndiceOriginal = n;
    }

	DistanciaDjikstra[ vertice[0]->IndiceOriginal ] = 0; // Distância do vértice pra ele mesmo � zero.

    while(heap.size())
    {
		vfila = heap.top()._vertex;
        dist = heap.top().distancia;
        heap.pop();

        if(vfila == vertice[1])
            achou_final = true;

		if(dist > DistanciaDjikstra[vfila->IndiceOriginal])
            continue;

        ListaVerticesEArestasT = vfila->ListaVerticesEArestas;
        for(int n = 0; n < ListaVerticesEArestasT->list.size(); n++)
        {
            VerticeEArestaTemp = ListaVerticesEArestasT->getVerticeEAresta(n);
			shared_ptr<TAresta> edge = VerticeEArestaTemp->Aresta;
			if ( edge->_layer != "" && edge->_layer != layer )
				continue;

            vatual = VerticeEArestaTemp->Vertice;
            int alt;

			alt = DistanciaDjikstra[vfila->IndiceOriginal] + edge->Tam;
            if ( alt < DistanciaDjikstra[vatual->IndiceOriginal] )
            {
				DistanciaDjikstra[vatual->IndiceOriginal] = alt;
                anterior[vatual->IndiceOriginal] = vfila;
                vArestas[vatual->IndiceOriginal] = edge;
                heap.push(VerticesDjikstra(vatual, alt));
            }
        }
    }

	return achou_final;
}


void CInfoCircuitos::Arvore( shared_ptr<TVerticeGeral> Vertice, std::vector< shared_ptr<TAresta> > &ListaArestas, shared_ptr<TDesenho> drawing)
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
    TListaVerticesEArestas *ListaVerticesEArestas;
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
