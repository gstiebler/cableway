//---------------------------------------------------------------------------
#pragma hdrstop
#include "UInfoCircuitos.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#include <queue>

#include "UGeometria.h"
#include "UInfoCircuitos.h"

#include "UGrafoDesenho.h"
#include "UArmazenamentoCircuitos.h"
#include "UAuxString.h"
#include "UConfig.h"
#include "UTCallbackStatusCarregamento.h"

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

string CInfoCircuitos::ErrosDoCircuito(string Origem, string Destino, string rota)
{
    string erro;
    erro = "";
    if (VerticesGerais->AchaVerticePeloTexto( Origem ) < 0)
    {
        bool exists;
        exists = false;
        exists = CallVT.ponteiroFuncao( CallVT.PonteiroProThis, Origem.c_str() );
//		for ( int j = 0 ; j < ContainerDesenhos->NumDesenhos() ; j++ )
//		{
//			TDesenho *pnt = ContainerDesenhos->getDesenho(j);
//			for ( int i = 0 ; i < (int)pnt->GrafoDesenho->Dados->Textos.size() ; i++ )
//			{
//				if ( pnt->GrafoDesenho->Dados->Textos[i].texto == Origem )
//				{
//					exists = true;
//					break;
//				}
//			}
//			if ( exists )
//				break;
//		}

        if (exists)
        {
            erro +=
                    "O texto de origem está nos desenhos, porém não está associado a um equipamento ou bandeirola, ou ainda, está fora do nível; ";
//			erroShow +="O texto de origem está nos desenhos, porém não está associado a um equipamento ou bandeirola, ou ainda, está fora do nível;\n";
        }
        else
        {
            erro += "O texto de origem não existe nos desenhos; ";
//			erroShow += "O texto de origem não existe nos desenhos;\n";
        }
    }
    if (VerticesGerais->AchaVerticePeloTexto( Destino ) < 0)
    {
        bool exists;
        exists = false;
        exists = CallVT.ponteiroFuncao( CallVT.PonteiroProThis, Destino.c_str() );
//		for ( int j = 0 ; j < ContainerDesenhos->NumDesenhos() ; j++ )
//		{
//			TDesenho *pnt = ContainerDesenhos->getDesenho(j);
//			for ( int i = 0 ; i < (int)pnt->GrafoDesenho->Dados->Textos.size() ; i++ )
//			{
//				if ( pnt->GrafoDesenho->Dados->Textos[i].texto == Destino )
//				{
//					exists = true;
//					break;
//				}
//			}
//			if ( exists )
//				break;
//		}

        if (exists)
        {
            erro +=
                    "O texto de destino está nos desenhos, porém não está associado a um equipamento ou bandeirola, ou ainda, está fora do nível.";
//			erroShow += "O texto de destino está nos desenhos, porém não está associado a um equipamento ou bandeirola, ou ainda, está fora do nível.\n";
        }
        else
        {
            erro += "O texto de destino não existe nos desenhos.";
//			erroShow += "O texto de destino não existe nos desenhos.\n";
        }
    }
    if (erro == "" && rota == "")
    {
        erro = "Não foi encontrado caminho.";
//		erroShow = "Não foi encontrado caminho.";
    }
    return erro;
}

CInfoCircuitos::CInfoCircuitos(TParamsInfoCircuitos *ParamsInfoCircuitos, callbackStatusCarregamento& call, callbackVerificaTexto &callVT) :
        VerticesGerais( NULL )
{
    // CallVT.ponteiroFuncao = callVT.ponteiroFuncao;
    // CallVT.PonteiroProThis = callVT.PonteiroProThis;
    NumDesenhos = ParamsInfoCircuitos->NumDesenhos;
    VerticesGerais = ParamsInfoCircuitos->VerticesGerais;
    Arestas = ParamsInfoCircuitos->Arestas;

    //DM_oRetorno->qryGeral->SQL->Clear();
    //DM_oRetorno->qryGeral->SQL->Add("DELETE FROM ARQUIVOSPROJETOCIRCUITO WHERE PROJETO_ID = " + DM_oRetorno->tbProjeto->FieldByName("ID")->AsString);
    //DM_oRetorno->qryGeral->ExecSQL();
    //CTempoExec *tempo = CTempoExec::getInstance();
    //tempo->MarcaTempo("Inicio da marcacao de tempo no InfoCircuitos");

    dentroEquipamento = CConfig::DentroEquipamento();

    ListaCircuitosArestas = new TListaCircuitos[Arestas->Tamanho()];
    for (int n = 0; n < Arestas->Tamanho(); n++)
        ListaCircuitosArestas[n].NumCircuitos = 0;
    GeraInfoCircuitos( call );
}
//---------------------------------------------------------------------------

CInfoCircuitos::~CInfoCircuitos()
{
    // if ( !CTempoExec::isNull() )
    // {
    //   CTempoExec *tempo = CTempoExec::getInstance();
    //   #ifdef DEBUG_BUILDER
    //     tempo->MostraTempo((string)"Tempo\\InfoCircuitos.txt");
    //   #endif
    // }
    //delete [] ListaCircuitosArestas;
    //delete ArestasDoCircuito;
}
//---------------------------------------------------------------------------

void CInfoCircuitos::GeraInfoCircuitos(callbackStatusCarregamento &call)
{
    //// Pega o número de circuitos
    //int NumReg=CArmazenamentoCircuitos::NumReg();
    //// Cria um vetor de TCircuito com o número de circuitos
    //TCircuito *Circuitos=new TCircuito [NumReg];
    //// Coloca os circuitos num vetor
    //CArmazenamentoCircuitos::VetorCircuitos(Circuitos);
    //// ???
    //CArmazenamentoBandeirola::ApagaBandeirolas();
    //for (int n=0; n<NumReg; n++)
    // {
    //   if ( call.ponteiroFuncao ) call.ponteiroFuncao(call.PonteiroProThis, "Circuito ", n+1, NumReg);
    //	AdicionaCircuito(Circuitos[n]);
    // }
    ////DM->Transaction->Commit();
    //delete [] Circuitos;
}
//---------------------------------------------------------------------------

void CInfoCircuitos::AdicionaCircuito(TCircuito &Circuito)
{
//	int m;
//	int Aresta;
//	TCircuitoAreas *CircuitoAreas;
//	CircuitoAreas = new TCircuitoAreas[NumDesenhos];
//	TArestasCircuito *ArestasCircuito;
//	string rota, SubRotas;
//	TVectorInt Bandeirolas;
//	ArestasDoCircuito->Adicionar(NumDesenhos);
//	// O item está em branco
//	ArestasCircuito=ArestasDoCircuito->Ultimo();
//	TStringList *DebugArestas=NULL;
//	bool erro, erro_interno;
//#ifdef DEBUG_BUILDER
//  DebugArestas = new TStringList();
////	DebugArestas=new vector<string>;
//#endif
//
//  CTempoExec *tempo = CTempoExec::getInstance();
//  tempo->MarcaTempo("Circuito: "+Circuito.NomeCircuito);
//
//  erro_interno = false;
//  try
//  {
//	  // Chama desse jeito se não tiver rota do usuário
//	  if (stringTrim(Circuito.RotaUsuario) == "" )
//	  {
//      erro = GeraRota(Circuito.Destino, Circuito.Origem, Circuito.metragem, Circuito.rota, ArestasCircuito, &Bandeirolas, DebugArestas, SubRotas, CircuitoAreas);
//    }
//	  // E assim caso tenha..
//	  else
//	  {
//		  erro = GeraRota(Circuito.Destino, Circuito.Origem, Circuito.RotaUsuario, Circuito.metragem, Circuito.rota, ArestasCircuito, &Bandeirolas, DebugArestas, CircuitoAreas);
//	  }
//  }
//  catch(...)
//  {
//    erro_interno = true;
//  }
//  tempo->MarcaTempo((string)"Fim da Geração" + (erro?" Nao achou":" Achou"));
//
//	if ( !erro )
//	{
//		ArestasCircuito->Circuito=Circuito.NomeCircuito;
//		ArestasCircuito->idCircuito=Circuito.ID;
//		//Adiciona o circuito na lista da aresta
//		for (m=0; m<(int)ArestasCircuito->Arestas.size(); m++)
//		{
//			Aresta=ArestasCircuito->Arestas[m];
//			ListaCircuitosArestas[Aresta].AdicionaCircuito(Circuito.NomeCircuito);
//		}
//		CArmazenamentoCircuitos::AtualizaRotaMetragem(Circuito);
//		CArmazenamentoBandeirola::AdicionaBandeirolas(Circuito.ID, Bandeirolas, VerticesGerais);
//		for ( int i = 0 ; i < NumDesenhos ; i++ )
//		{
//			if ( !CircuitoAreas[i].ativo )
//				continue;
//			DM_oRetorno->qryGeral->SQL->Clear();
//			DM_oRetorno->qryGeral->SQL->Add("INSERT INTO ARQUIVOSPROJETOCIRCUITO (PROJETO_ID, ARQUIVOSPROJETO_ID, CIRCUITO_ID, ROTA) VALUES (" + DM_oRetorno->tbProjeto->FieldByName("ID")->AsString + ", " + CircuitoAreas[i].IDArquivo + ", " + Circuito.ID + ", '" +CircuitoAreas[i].rota.c_str()+ "')" );
//			DM_oRetorno->qryGeral->ExecSQL();
//		}
//#ifdef DEBUG_BUILDER
//		DebugArestas->SaveToFile(ExtractFilePath(Application->ExeName)+"Vertices\\"+Circuito.NomeCircuito.c_str()+".txt");
//		delete DebugArestas;
//#endif
//	}
//  else if ( erro && stringTrim(Circuito.RotaUsuario) != "" && Circuito.rota != "" )
//  {
//    // Se deu erro, mas era rota de usuário e conseguiu completar alguma parte...
//		ArestasCircuito->Circuito=Circuito.NomeCircuito;
//		ArestasCircuito->idCircuito=Circuito.ID;
//		//Adiciona o circuito na lista da aresta
//		for (m=0; m<(int)ArestasCircuito->Arestas.size(); m++)
//		{
//			Aresta=ArestasCircuito->Arestas[m];
//			ListaCircuitosArestas[Aresta].AdicionaCircuito(Circuito.NomeCircuito);
//		}
//		CArmazenamentoCircuitos::AtualizaRotaMetragem(Circuito);
//		CArmazenamentoBandeirola::AdicionaBandeirolas(Circuito.ID, Bandeirolas, VerticesGerais);
//		for ( int i = 0 ; i < NumDesenhos ; i++ )
//		{
//			if ( !CircuitoAreas[i].ativo )
//				continue;
//			DM_oRetorno->qryGeral->SQL->Clear();
//			DM_oRetorno->qryGeral->SQL->Add("INSERT INTO ARQUIVOSPROJETOCIRCUITO (PROJETO_ID, ARQUIVOSPROJETO_ID, CIRCUITO_ID, ROTA) VALUES (" + DM_oRetorno->tbProjeto->FieldByName("ID")->AsString + ", " + CircuitoAreas[i].IDArquivo + ", " + Circuito.ID + ", '" +CircuitoAreas[i].rota.c_str()+ "')" );
//			DM_oRetorno->qryGeral->ExecSQL();
//		}
//#ifdef DEBUG_BUILDER
//		DebugArestas->SaveToFile(ExtractFilePath(Application->ExeName)+"\\Vertices\\"+Circuito.NomeCircuito.c_str()+".txt");
//		delete DebugArestas;
//#endif
//		//CArmazenamentoCircuitos::DeuErro(Circuito);
//  }
//	else
//	{
//    if(erro_interno)
//    {        
//		  CArmazenamentoCircuitos::DeuErro(Circuito, "Erro interno. Favor contactar o administrador.");
//    }
//    else
//    {          
//		  ArestasDoCircuito->Remove(ArestasDoCircuito->Tamanho()-1);
//      string erros = ErrosDoCircuito(Circuito.Origem, Circuito.Destino, Circuito.rota);
//		  CArmazenamentoCircuitos::DeuErro(Circuito, erros);
//    }
//	}
//
//	//delete Bandeirolas;
//	delete[] CircuitoAreas;
}
//---------------------------------------------------------------------------

int CInfoCircuitos::ArestaDoPonto(TPonto ponto, TPonto &PontoNaReta, int IndiceDesenho)
{
    int m;
    double MenorDist, Dist, DistPonta1, DistPonta2;
    TPonto Reta[2], retorno;
    int IndiceAresta = -1;
    MenorDist = Infinity;
    for (m = 0; m < Arestas->Tamanho(); m++)
    {
        if (Arestas->getItem( m )->IndiceDesenho != IndiceDesenho)
            continue;
        Reta[0] = VerticesGerais->getItem( Arestas->getItem( m )->Vertice1 )->pos;
        Reta[1] = VerticesGerais->getItem( Arestas->getItem( m )->Vertice2 )->pos;

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
    vertices[0] = Arestas->getItem( iAresta )->Vertice1;
    vertices[1] = Arestas->getItem( iAresta )->Vertice2;

    Pontos[0] = VerticesGerais->getItem( vertices[0] )->pos;
    Pontos[1] = VerticesGerais->getItem( vertices[1] )->pos;
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

void CInfoCircuitos::MergeRota(string &rota, string NovaParte)
{
    size_t pos = 0;

    // Se não tiver rota
    if (rota.size() == 0)
    {
        // então a rota é só a nova parte..
        rota = NovaParte;
        return;
    }

    // senão, procura a última barra (porque a gente quer tirar o destino, da rota atual e substituir pela nova parte)
    pos = rota.find_last_of( "/" );

    // se não encontrou, retorna. Algum problema aconteceu! (ERRO)
    if (pos == string::npos)
        return;

    // a rota agora é a rota com o destino substituído pela nova parte.
    rota = rota.substr( 0, pos + 1 ) + NovaParte;
}
//---------------------------------------------------------------------------

//Recebe a origem e o destino, e toda a rota do usuário
bool CInfoCircuitos::GeraRota(string Destino, string Origem, string ListaPontos, double &tam, string &rota,
        TArestasCircuito *ArestasCircuito, TVectorInt *ListaBandeirolas,
        TStringList*DEBUG_arestas, TCircuitoAreas *CircuitoAreas)
{
    vector<string> * ListaRota = new vector<string>();
    int i;
    string rotaTemp, SubRotasTemp;
    double tamTemp;
    bool erro = false;

    ListaRota->push_back(Origem);
    SeparaRota(ListaPontos, ListaRota);
    ListaRota->push_back(Destino);

    rota = "";
    tam = 0;

    bool ultimo = true;

    for ( i = 0; i < (int)(ListaRota->size()-1); i++ )
    {
        bool erroNessaRota = false;
        tamTemp = 0;
        rotaTemp = "";
        if (GeraRota(ListaRota->at(i+1), ListaRota->at(i), tamTemp, rotaTemp, ArestasCircuito, ListaBandeirolas, DEBUG_arestas, SubRotasTemp, CircuitoAreas))
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
            MergeRota(rota, ".../...");
        }
        ultimo = erroNessaRota;
    }

    delete ListaRota;

    return erro;
}
//---------------------------------------------------------------------------

bool CInfoCircuitos::GeraRota(string Destino, string Origem, double &tam, string &rota, TArestasCircuito *ArestasCircuito,
        TVectorInt *ListaBandeirolas,
        TStringList*DEBUG_arestas, string &SubRotas, TCircuitoAreas *CircuitoAreas)
{
    int n, m;
    int vertice[2], vfila, vatual;
    int iArestaTemp;
    TAresta *ArestaTemp;
    vertice[0] = vertice[1]=-1;
    TVectorInt *ListaArestas=NULL;
    TVectorInt *ArestasDesenho;
    string UltTemp="";
    string V[2];
    V[0] = Destino;
    V[1] = Origem;

    // CTempoExec *tempo = CTempoExec::getInstance();

    if (ArestasCircuito)
    {
        /*
         * ArestasCircuito é um vetor que vai guardar o caminho feito, em termos de aresta, para encontrar o menor caminho.
         * ListaArestas guarda todas as arestas, enquanto ArestasDesenho guarda as arestas específicas de cada desenho
         * A grande diferença é que no ListaArestas também estão as arestas entre desenhos.
         */
        ListaArestas=&ArestasCircuito->Arestas;
        ArestasDesenho=ArestasCircuito->ArestasDesenho;
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
        rota = "";
        tam = 0.0;
        return 1;
    }
    //tempo->MarcaTempo("Vai alocar memória");
    // Djikstra
    double *DistanciaDjikstra;
    int *anterior;

//  tempo->MarcaTempo("Marcação de tempo antes de alocar as coisas do InfoCircuitos");
    int *PaisVertices=new int [VerticesGerais->Tamanho()];//armazena os pais de cada vértice na árvore
    int *vArestas=new int [VerticesGerais->Tamanho()];//armazena a aresta de cada vértice referente em PaisVertices
    DistanciaDjikstra = new double[VerticesGerais->Tamanho()];
    anterior = new int[VerticesGerais->Tamanho()];
//  tempo->MarcaTempo("Fim da Marcação de tempo.");
//  tempo->MostraTempo((string)"Tempo InfoCircuitos.txt");
    TListaVerticesEArestas *ListaVerticesEArestasT;
    priority_queue<VerticesDjikstra> heap;

    heap.empty();

    //tempo->MarcaTempo("Alocou memória");

    /* initialize single source */
    for ( int n = 0; n < VerticesGerais->Tamanho(); n++ )
    {
        DistanciaDjikstra[n] = Infinity;
        anterior[n] = -1;
    }

    DistanciaDjikstra[vertice[0]] = 0; // Distância do vértice pra ele mesmo é zero.

    //  for (m=0; ArestasCircuito && m<(int)ArestasCircuito->Arestas.size(); m++)
    //  {
    //    VerticesExplorados[Arestas->getItem(ArestasCircuito->Arestas[m])->Vertice1] = true;
    //    VerticesExplorados[Arestas->getItem(ArestasCircuito->Arestas[m])->Vertice2] = true;
    //  }
    //  VerticesExplorados[vertice[0]]=false;
    //  VerticesExplorados[vertice[1]]=false;

    //tempo->MarcaTempo("fez heap");
    TVerticeGeral *VerticeTemp;
    TVerticeEAresta *VerticeEArestaTemp;
    heap.push(VerticesDjikstra(vertice[0], 0));
    double dist;
    int iAresta;
    bool achou_final = false;
    while(heap.size())
    {
        vfila = heap.top().n;
        dist = heap.top().distancia;
        heap.pop();

        if(vfila == vertice[1])
        achou_final = true;

        if(dist > DistanciaDjikstra[vfila])
        continue;

        ListaVerticesEArestasT = VerticesGerais->getItem(vfila)->ListaVerticesEArestas;
        for(n = 0; n < ListaVerticesEArestasT->list.size(); n++)
        {
            VerticeEArestaTemp = ListaVerticesEArestasT->getVerticeEAresta(n);
            vatual = VerticeEArestaTemp->Vertice;
            int alt;
            if ( dentroEquipamento )
            {
                VerticeTemp = VerticesGerais->getItem(vatual);
                if (VerticeTemp->TipoElemento == INSTRUMENTO && ( !VerticeTemp->EhColar ) ) //|| !VerticesGerais->getItem(vatual)->EhPrensaCabo ) )
                {
                    if (!VerticeTemp->texto.empty() && vatual != vertice[0] && vatual != vertice[1])
                    continue;
                }
            }
            iAresta = VerticeEArestaTemp->Aresta;
            alt = DistanciaDjikstra[vfila] + Arestas->getItem(iAresta)->Tam;
            if ( alt < DistanciaDjikstra[vatual] )
            {
                DistanciaDjikstra[vatual] = alt;
                anterior[vatual] = vfila;
                vArestas[vatual] = iAresta;
                heap.push(VerticesDjikstra(vatual, alt));
            }
        }
    }

    //tempo->MarcaTempo("Fez rota");

    double TamSubRota=0;
    //CAMINHO INVERSO NA ÁRVORE DE LARGURA
    vatual=vertice[1];
    tam= VerticesGerais->getItem(vertice[0])->dist + VerticesGerais->getItem(vertice[1])->dist;
    string temp, sRota;
    int UltDesenho = -1;
    sRota="";
    if(achou_final)
    {
        while (anterior[vatual] > 0)
        {
            iArestaTemp=vArestas[vatual];
            if ( ListaArestas )
            ListaArestas->push_back(iArestaTemp);
            ArestaTemp=Arestas->getItem(iArestaTemp);
            if (ArestasCircuito && ArestaTemp->IndiceDesenho!=I_DESENHO_NULO)
            ArestasDesenho[ArestaTemp->IndiceDesenho].push_back(iArestaTemp);
            if ( CircuitoAreas )
            {
                CircuitoAreas[VerticesGerais->getItem(vatual)->iDesenho].IDArquivo = VerticesGerais->getItem(vatual)->IDArquivo;
                CircuitoAreas[VerticesGerais->getItem(vatual)->iDesenho].ativo = true;

                if ( VerticesGerais->getItem(vatual)->iDesenho != UltDesenho && UltDesenho >= 0 )
                {
                    CircuitoAreas[UltDesenho].rota = CircuitoAreas[UltDesenho].rota.substr(0, CircuitoAreas[UltDesenho].rota.size()-1);
                }

                if (VerticesGerais->getItem(vatual)->texto!="")
                {
                    temp=VerticesGerais->getItem(vatual)->texto;
                    if (UltTemp!=temp || VerticesGerais->getItem(vatual)->iDesenho != UltDesenho )
                    CircuitoAreas[VerticesGerais->getItem(vatual)->iDesenho].rota+=temp+"/";
                    //					UltTemp=temp;
                    UltDesenho = VerticesGerais->getItem(vatual)->iDesenho;
                }
            }
            /*			if (CircuitoAreas && ArestaTemp->IndiceDesenho!=I_DESENHO_NULO && ArestaTemp->IDArquivo!=I_DESENHO_NULO)
             {
             CircuitoAreas[ArestaTemp->IndiceDesenho].IDArquivo = ArestaTemp->IDArquivo;
             CircuitoAreas[ArestaTemp->IndiceDesenho].ativo = true;


             if (VerticesGerais->getItem(vatual)->texto!="")
             {
             temp=VerticesGerais->getItem(vatual)->texto;
             if (UltTemp!=temp)
             CircuitoAreas[ArestaTemp->IndiceDesenho].rota+=temp+"/";
             UltTemp=temp;
             }
             }*/
            tam+=Arestas->getItem(vArestas[vatual])->Tam;
            TamSubRota+=Arestas->getItem(vArestas[vatual])->Tam;

            if ( Arestas->getItem(vArestas[vatual])->Vertice1 > 0 && VerticesGerais->getItem(Arestas->getItem(vArestas[vatual])->Vertice1)->EhPrensaCabo )
            {
                ListaVerticesEArestasT = VerticesGerais->getItem(Arestas->getItem(vArestas[vatual])->Vertice1)->ListaVerticesEArestas;
                for ( int i = 0; i < ListaVerticesEArestasT->list.size(); i++ )
                {
                    //Arestas->getItem(ListaVerticesEArestasT->getVerticeEAresta(i)->Aresta)->Tam = Infinity;
                }
            }

            if ( Arestas->getItem(vArestas[vatual])->Vertice2 > 0 && VerticesGerais->getItem(Arestas->getItem(vArestas[vatual])->Vertice2)->EhPrensaCabo )
            {
                ListaVerticesEArestasT = VerticesGerais->getItem(Arestas->getItem(vArestas[vatual])->Vertice2)->ListaVerticesEArestas;
                for ( int i = 0; i < ListaVerticesEArestasT->list.size(); i++ )
                {
                    //Arestas->getItem(ListaVerticesEArestasT->getVerticeEAresta(i)->Aresta)->Tam = Infinity;
                }
            }

            //if (DEBUG_arestas)
            //{
            //	TAresta *Aresta=Arestas->getItem(vArestas[vatual]);
            //	DEBUG_arestas->Add("Vértices: "+IntToStr(Aresta->Vertice1)+", "+IntToStr(Aresta->Vertice2));
            //	//        DEBUG_arestas->push_back((string)"Tamanho Aresta: "+FormatFloat("0.00", Aresta->Tam));
            //	DEBUG_arestas->Add("Tamanho Aresta: "+FormatFloat("0.00",Aresta->Tam));
            //	DEBUG_arestas->Add(FormatFloat("0.00",VerticesGerais->getItem(Aresta->Vertice1)->pos.x)+", "+
            //			FormatFloat("0.00",VerticesGerais->getItem(Aresta->Vertice1)->pos.y));
            //	DEBUG_arestas->Add(FormatFloat("0.00",VerticesGerais->getItem(Aresta->Vertice2)->pos.x)+", "+
            //			FormatFloat("0.00",VerticesGerais->getItem(Aresta->Vertice2)->pos.y));
            //if ( VerticesGerais->getItem(Aresta->Vertice1)->texto!="" )
            //	DEBUG_arestas->Add("Texto do Vértice 1:" +(AnsiString)VerticesGerais->getItem(Aresta->Vertice1)->texto.c_str());
            //if ( VerticesGerais->getItem(Aresta->Vertice2)->texto!="" )
            //DEBUG_arestas->Add("Texto do vértice 2:"+ (AnsiString)VerticesGerais->getItem(Aresta->Vertice2)->texto.c_str());
            //	DEBUG_arestas->Add("");
            //}
            if (VerticesGerais->getItem(vatual)->texto!="")
            {
                //if ( VerticesGerais->getItem(vatual)->TipoElemento!=INSTRUMENTO || (VerticesGerais->getItem(vatual)->texto.UpperCase()==Origem.UpperCase() || VerticesGerais->getItem(vatual)->texto.UpperCase()==Destino.UpperCase()))
                {
                    temp=VerticesGerais->getItem(vatual)->texto;
                    if (UltTemp!=temp && temp != Destino)
                    sRota+=temp+"/";
                    UltTemp=temp;
                    SubRotas+=to_string(TamSubRota)+"/";
                    TamSubRota=0;
                    if (VerticesGerais->getItem(vatual)->TipoElemento==BANDEIROLA)
                    {
                        if (ListaBandeirolas)
                        ListaBandeirolas->push_back(vatual);
                    }
                    else if ( VerticesGerais->getItem(vatual)->EhColar )
                    {
                        if (ListaBandeirolas)
                        ListaBandeirolas->push_back(vatual);
                    }
                }
            }
            vatual=anterior[vatual];
        }
        sRota+=V[0];
        if (CircuitoAreas && ArestaTemp->IndiceDesenho!=I_DESENHO_NULO && ArestaTemp->IDArquivo!=I_DESENHO_NULO)
        {
            CircuitoAreas[ArestaTemp->IndiceDesenho].rota += V[0];
        }
    }
    else
    tam=0;

    //tempo->MarcaTempo("Fez caminho inverso");

    rota=sRota;

    heap.empty();
    delete [] anterior;
    delete [] DistanciaDjikstra;
    delete [] PaisVertices;
    delete [] vArestas;
    if ( rota == "" )
    return 1;
    return 0;

    return 0;
}
//---------------------------------------------------------------------------

void CInfoCircuitos::Arvore(int Vertice, TVectorInt &ListaArestas, int IndiceDesenho)
{
    int n;
    queue<int> fila;
    //fila=new CFila(VerticesGerais->Tamanho());
    int vfila, vatual;
    int iArestaTemp;
    bool *VerticesVisitados;
    VerticesVisitados = new bool[VerticesGerais->Tamanho()];
    memset( VerticesVisitados, 0, VerticesGerais->Tamanho() * sizeof(bool) );
    fila.push( Vertice );
    VerticesVisitados[Vertice] = true;
    //BUSCA EM LARGURA
    TListaVerticesEArestas *ListaVerticesEArestas;
    while (fila.size())
    {
        vfila = fila.front();
        fila.pop();
        ListaVerticesEArestas = VerticesGerais->getItem( vfila )->ListaVerticesEArestas;
        for (n = 0; n < ListaVerticesEArestas->list.size(); n++)
        {
            //vfila é o vértice que queremos a lista de adjacência
            //n é o enésimo vértice na lista de adjacência de vfila
            //.Vertice é pq podemos pegar também .Aresta
            vatual = ListaVerticesEArestas->getVerticeEAresta( n )->Vertice;
            iArestaTemp = ListaVerticesEArestas->getVerticeEAresta( n )->Aresta;
            // ListaArestas é a lista de arestas da árvore no desenho atual
            // A árvore é regerada para cada desenho
            if (Arestas->getItem( iArestaTemp )->IndiceDesenho == IndiceDesenho)
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
