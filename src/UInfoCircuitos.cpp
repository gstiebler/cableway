//---------------------------------------------------------------------------
#pragma hdrstop
#include "UInfoCircuitos.h"

#include <queue>

#include "UGeometria.h"
#include "UInfoCircuitos.h"
#include "UParamsStructs.h"

#include "UArmazenamentoCircuitos.h"

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



CInfoCircuitos::CInfoCircuitos(TParamsInfoCircuitos *ParamsInfoCircuitos) :
        VerticesGerais( NULL )
{
    // CallVT.ponteiroFuncao = callVT.ponteiroFuncao;
    // CallVT.PonteiroProThis = callVT.PonteiroProThis;
    NumDesenhos = ParamsInfoCircuitos->NumDesenhos;
    VerticesGerais = ParamsInfoCircuitos->VerticesGerais;
    Arestas = ParamsInfoCircuitos->Arestas;

    ListaCircuitosArestas = new TListaCircuitos[Arestas.size()];
    for (int n = 0; n < Arestas.size(); n++)
        ListaCircuitosArestas[n].NumCircuitos = 0;
}
//---------------------------------------------------------------------------

CInfoCircuitos::~CInfoCircuitos()
{
}
//---------------------------------------------------------------------------



void CInfoCircuitos::AdicionaCircuito(TCircuito &Circuito)
{
	int m;
	int Aresta;
	TCircuitoAreas *CircuitoAreas;
	CircuitoAreas = new TCircuitoAreas[NumDesenhos];
	TArestasCircuito *ArestasCircuito;
	string rota, SubRotas;
	TVectorInt Bandeirolas;
	ArestasDoCircuito.push_back( NumDesenhos );
	// O item está em branco
	ArestasCircuito = &ArestasDoCircuito.back();
	TStringList *DebugArestas=NULL;
	bool erro, erro_interno;
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
      erro = GeraRota(Circuito.Destino, Circuito.Origem, Circuito.metragem, Circuito.rota, ArestasCircuito, &Bandeirolas, SubRotas);
    }
	  // E assim caso tenha..
	  else
	  {
		  erro = GeraRota(Circuito.Destino, Circuito.Origem, Circuito.RotaUsuario, Circuito.metragem, Circuito.rota, ArestasCircuito, &Bandeirolas, DebugArestas, CircuitoAreas);
	  }
  }
  catch(...)
  {
    erro_interno = true;
  }
  //tempo->MarcaTempo((string)"Fim da Gera��o" + (erro?" Nao achou":" Achou"));

	if ( !erro )
	{
		ArestasCircuito->Circuito=Circuito.NomeCircuito;
		ArestasCircuito->idCircuito=Circuito.ID;
		//Adiciona o circuito na lista da aresta
		for (m=0; m<(int)ArestasCircuito->Arestas.size(); m++)
		{
			Aresta=ArestasCircuito->Arestas[m];
			ListaCircuitosArestas[Aresta].AdicionaCircuito(Circuito.NomeCircuito);
		}
		//CArmazenamentoCircuitos::AtualizaRotaMetragem(Circuito);

#ifdef DEBUG_BUILDER
		DebugArestas->SaveToFile(ExtractFilePath(Application->ExeName)+"Vertices\\"+Circuito.NomeCircuito.c_str()+".txt");
		delete DebugArestas;
#endif
	}
	else if ( erro && Circuito.RotaUsuario != "" && Circuito.rota.size() > 0 )
  {
    // Se deu erro, mas era rota de usuário e conseguiu completar alguma parte...
		ArestasCircuito->Circuito=Circuito.NomeCircuito;
		ArestasCircuito->idCircuito=Circuito.ID;
		//Adiciona o circuito na lista da aresta
		for (m=0; m<(int)ArestasCircuito->Arestas.size(); m++)
		{
			Aresta=ArestasCircuito->Arestas[m];
			ListaCircuitosArestas[Aresta].AdicionaCircuito(Circuito.NomeCircuito);
		}
		//CArmazenamentoCircuitos::AtualizaRotaMetragem(Circuito);

		//CArmazenamentoCircuitos::DeuErro(Circuito);
	}
	else
	{
		if(erro_interno)
		{        
		  //CArmazenamentoCircuitos::DeuErro(Circuito, "Erro interno. Favor contactar o administrador.");
		}
		else
		{          
			ArestasDoCircuito.pop_back();
		  //CArmazenamentoCircuitos::DeuErro(Circuito, erros);
		}
	}

	//delete Bandeirolas;
	delete[] CircuitoAreas;
}
//---------------------------------------------------------------------------

int CInfoCircuitos::ArestaDoPonto(TPonto ponto, TPonto &PontoNaReta, int IndiceDesenho)
{
    int m;
    double MenorDist, Dist, DistPonta1, DistPonta2;
    TPonto Reta[2], retorno;
    int IndiceAresta = -1;
    MenorDist = Infinity;
    for (m = 0; m < Arestas.size(); m++)
    {
        if (Arestas[ m ]->IndiceDesenho != IndiceDesenho)
            continue;
        Reta[0] = VerticesGerais->vertices[ Arestas[ m ]->Vertice1 ]->pos;
        Reta[1] = VerticesGerais->vertices[ Arestas[ m ]->Vertice2 ]->pos;

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
VerticesDjikstra::VerticesDjikstra(int numero, double Distancia)
{
    n = numero;
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

//--------------------------------------------------------------------------
string CInfoCircuitos::CircuitosDaAresta(int Aresta)
{
    int n;
    string retorno, temp;
    retorno = "";
    for (n = 0; n < ListaCircuitosArestas[Aresta].NumCircuitos; n++)
    {
        temp = ListaCircuitosArestas[Aresta].Circuitos->at( n );
        retorno += "/" + temp;
    }
    retorno.erase( 0, 1 );
    return retorno;
}
//---------------------------------------------------------------------------

int CInfoCircuitos::ListaArestasDoCircuito(string circuito)
{
    int n;
    for (n = 0; n < ArestasDoCircuito.size(); n++)
        if (circuito == ArestasDoCircuito[n].Circuito)
            return n;
    return -1;
}
//---------------------------------------------------------------------------

int CInfoCircuitos::ListaArestasDoCircuito(int idCircuito)
{
    int n;
    for (n = 0; n < ArestasDoCircuito.size(); n++)
        if (idCircuito == ArestasDoCircuito[n].idCircuito)
            return n;
    return -1;
}
//---------------------------------------------------------------------------

int CInfoCircuitos::ApagaArestasDoCircuito(string circuito, int idCircuito)
{
    int n = ListaArestasDoCircuito( idCircuito );
    if (n < 0)
    {
        return -1;
    }

    TVectorInt Arestas = ArestasDoCircuito[n].Arestas;

    for (int i = 0; i < (int) Arestas.size(); i++)
    {
        int Aresta = Arestas.at( i );
        for (int j = 0; j < (int) ListaCircuitosArestas[Aresta].Circuitos->size(); j++)
        {
            if (ListaCircuitosArestas[Aresta].Circuitos->at( j ) == circuito)
            {
                ListaCircuitosArestas[Aresta].Circuitos->erase( ListaCircuitosArestas[Aresta].Circuitos->begin() + j );
                ListaCircuitosArestas[Aresta].NumCircuitos--;
                j--;
            }
        }
    }
    ArestasDoCircuito.erase( ArestasDoCircuito.begin() + n );
    return 0;
}

TVectorInt * CInfoCircuitos::ArestasCircuito(int circuito, int IndiceDesenho)
{
    return &ArestasDoCircuito[circuito].ArestasDesenho[IndiceDesenho];
}
//---------------------------------------------------------------------------

void CInfoCircuitos::PontosAresta(TPonto Pontos[2], int iAresta)
{
    int vertices[2];
    vertices[0] = Arestas[ iAresta ]->Vertice1;
    vertices[1] = Arestas[ iAresta ]->Vertice2;

	Pontos[0] = VerticesGerais->vertices[ vertices[0] ]->pos;
    Pontos[1] = VerticesGerais->vertices[ vertices[1] ]->pos;
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
        TArestasCircuito *ArestasCircuito, TVectorInt *ListaBandeirolas,
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

bool CInfoCircuitos::GeraRota(string Destino, string Origem, double &tam, vector<string> &rota, TArestasCircuito *ArestasCircuito,
        TVectorInt *ListaBandeirolas, string &SubRotas )
{
    int n, m;
    int vertice[2];
    vertice[0] = vertice[1]=-1;
    TVectorInt *ListaArestas=NULL;
    vector< vector<int> > &ArestasDesenho = ArestasCircuito->ArestasDesenho;
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
        ListaArestas=&ArestasCircuito->Arestas;
    }

    //tempo->MarcaTempo("Vai achar vértices");
    for (m=0; m<2; m++)
    {
        /*
         * vertice[] guarda os índices dos vértices, já que eles são passados pelo nome.
         */
        vertice[m] = VerticesGerais->AchaVerticePeloTexto(V[m]);
        if (vertice[m] < 0)
			return 1;
    }
    if(vertice[0] == vertice[1])
    {
		rota.clear();
        tam = 0.0;
        return 1;
    }

    vector<int> anterior( VerticesGerais->vertices.size() );
    vector<int> vArestas( VerticesGerais->vertices.size() );//armazena a aresta de cada vértice referente em PaisVertices
	bool achou_final = CInfoCircuitos::generateDistanceTree( vertice, anterior, vArestas );
	
	vector<string> sRota;
    if(achou_final)
    {
		int iArestaTemp;
		shared_ptr<TAresta> ArestaTemp;
		double TamSubRota=0;
		//CAMINHO INVERSO NA �RVORE DE LARGURA
		int vatual = vertice[1];
		tam= VerticesGerais->vertices[vertice[0]]->dist + VerticesGerais->vertices[vertice[1]]->dist;
		string temp;

        while (anterior[vatual] > 0)
        {
            iArestaTemp = vArestas[vatual];
            if ( ListaArestas )
				ListaArestas->push_back(iArestaTemp);
            ArestaTemp = Arestas[iArestaTemp];
            if (ArestasCircuito && ArestaTemp->IndiceDesenho!=I_DESENHO_NULO)
				ArestasDesenho[ArestaTemp->IndiceDesenho].push_back(iArestaTemp);

            tam+=Arestas[vArestas[vatual]]->Tam;
            TamSubRota += Arestas[vArestas[vatual]]->Tam;


            if (VerticesGerais->vertices[vatual]->texto!="")
            {
                //if ( VerticesGerais->getItem(vatual)->TipoElemento!=INSTRUMENTO || (VerticesGerais->getItem(vatual)->texto.UpperCase()==Origem.UpperCase() || VerticesGerais->getItem(vatual)->texto.UpperCase()==Destino.UpperCase()))
                {
                    temp=VerticesGerais->vertices[vatual]->texto;
                    if (UltTemp!=temp && temp != Destino)
						sRota.push_back( temp );
                    UltTemp=temp;
                    SubRotas+=to_string(TamSubRota)+"/";
                    TamSubRota=0;
                    if ( (VerticesGerais->vertices[vatual]->TipoElemento==BANDEIROLA) || VerticesGerais->vertices[vatual]->EhColar)
                    {
                        if (ListaBandeirolas)
							ListaBandeirolas->push_back(vatual);
                    }
                }
            }
            vatual=anterior[vatual];
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


bool CInfoCircuitos::generateDistanceTree( int vertice[2], vector<int> &anterior, vector<int> &vArestas )
{
	vector<int> PaisVertices( VerticesGerais->vertices.size() );//armazena os pais de cada vértice na �rvore
	vector<double> DistanciaDjikstra( VerticesGerais->vertices.size() );
    shared_ptr<TVerticeGeral> VerticeTemp;
    TVerticeEAresta *VerticeEArestaTemp;
	int vfila, vatual;
    double dist;
    int iAresta;
    TListaVerticesEArestas *ListaVerticesEArestasT;
    priority_queue<VerticesDjikstra> heap;
    heap.push(VerticesDjikstra(vertice[0], 0));
	bool achou_final = false;

	    /* initialize single source */
    for ( int n = 0; n < VerticesGerais->vertices.size(); n++ )
    {
        DistanciaDjikstra[n] = Infinity;
        anterior[n] = -1;
    }

    DistanciaDjikstra[vertice[0]] = 0; // Distância do vértice pra ele mesmo � zero.

    while(heap.size())
    {
        vfila = heap.top().n;
        dist = heap.top().distancia;
        heap.pop();

        if(vfila == vertice[1])
            achou_final = true;

        if(dist > DistanciaDjikstra[vfila])
            continue;

        ListaVerticesEArestasT = VerticesGerais->vertices[vfila]->ListaVerticesEArestas;
        for(int n = 0; n < ListaVerticesEArestasT->list.size(); n++)
        {
            VerticeEArestaTemp = ListaVerticesEArestasT->getVerticeEAresta(n);
            vatual = VerticeEArestaTemp->Vertice;
            int alt;

            VerticeTemp = VerticesGerais->vertices[vatual];
     //       if (VerticeTemp->TipoElemento == INSTRUMENTO && ( !VerticeTemp->EhColar ) ) //|| !VerticesGerais->getItem(vatual)->EhPrensaCabo ) )
     //       {
     //           if (!VerticeTemp->texto.empty() && vatual != vertice[0] && vatual != vertice[1])
					//continue;
     //       }

            iAresta = VerticeEArestaTemp->Aresta;
            alt = DistanciaDjikstra[vfila] + Arestas[iAresta]->Tam;
            if ( alt < DistanciaDjikstra[vatual] )
            {
                DistanciaDjikstra[vatual] = alt;
                anterior[vatual] = vfila;
                vArestas[vatual] = iAresta;
                heap.push(VerticesDjikstra(vatual, alt));
            }
        }
    }

	return achou_final;
}


void CInfoCircuitos::Arvore(int Vertice, TVectorInt &ListaArestas, int IndiceDesenho)
{
    int n;
    queue<int> fila;
    //fila=new CFila(VerticesGerais->Tamanho());
    int vfila, vatual;
    int iArestaTemp;
    bool *VerticesVisitados;
	VerticesVisitados = new bool[VerticesGerais->vertices.size()];
    memset( VerticesVisitados, 0, VerticesGerais->vertices.size() * sizeof(bool) );
    fila.push( Vertice );
    VerticesVisitados[Vertice] = true;
    //BUSCA EM LARGURA
    TListaVerticesEArestas *ListaVerticesEArestas;
    while (fila.size())
    {
        vfila = fila.front();
        fila.pop();
        ListaVerticesEArestas = VerticesGerais->vertices[ vfila ]->ListaVerticesEArestas;
        for (n = 0; n < ListaVerticesEArestas->list.size(); n++)
        {
            //vfila � o vértice que queremos a lista de adjac�ncia
            //n � o en�simo vértice na lista de adjac�ncia de vfila
            //.Vertice � pq podemos pegar também .Aresta
            vatual = ListaVerticesEArestas->getVerticeEAresta( n )->Vertice;
            iArestaTemp = ListaVerticesEArestas->getVerticeEAresta( n )->Aresta;
            // ListaArestas � a lista de arestas da �rvore no desenho atual
            // A �rvore � regerada para cada desenho
            if (Arestas[ iArestaTemp ]->IndiceDesenho == IndiceDesenho)
                ListaArestas.push_back( iArestaTemp );
            if (!VerticesVisitados[vatual])
            {
                VerticesVisitados[vatual] = true;
                fila.push( vatual );
            }
        }
    }
    delete[] VerticesVisitados;
}
//---------------------------------------------------------------------------

//--------------------------------------------------------------------------
