//---------------------------------------------------------------------------
#pragma hdrstop
#include "UContainerDesenhos.h"
#include "UStructs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

int ID_PROJETO;

TDesenho::~TDesenho()
{
  delete GrafoDesenho;
}
//---------------------------------------------------------------------------

CContainerDesenhos::CContainerDesenhos()
{
  // Cria uma nova lista pra guardar os desenhos
  //ListaDesenhos=new TList;

  // Bota NULL no InfoCircuitos e no frmDesenhoAbas
  InfoCircuitos=NULL;
  //frmDesenhoAbas=NULL;
}
//---------------------------------------------------------------------------



CContainerDesenhos::~CContainerDesenhos()
{
//  // Se o InfoCircuitos já não for nulo, então o apaga
//  if (InfoCircuitos)
//  {
//    delete InfoCircuitos;
//    InfoCircuitos = NULL;
//  }
}
//---------------------------------------------------------------------------

void CContainerDesenhos::addDrawing( std::shared_ptr<CDadosGenerico> dados, double altura )
{
    // Cria um novo params
    TParamsGrafoDesenho paramsGrafoDesenho;
    // O Id
    paramsGrafoDesenho.IDArquivo = ListaDesenhos.size();
    // Para carregar o grafo
    paramsGrafoDesenho.CarregaGrafo = true;
    paramsGrafoDesenho.Altura = altura;
    // Preenche o índice do desenho
    paramsGrafoDesenho.IndiceDesenho = ListaDesenhos.size();
	dados->IndiceDesenho = ListaDesenhos.size();
    // Passa um ponteiro para o VerticesGerais (TVerticesGerais)
    paramsGrafoDesenho.VerticesGerais = ParamsInfoCircuitos.VerticesGerais;

    CGrafoDesenho* grafoDesenho = new CGrafoDesenho(paramsGrafoDesenho, dados);

    // Cria um novo desenho
    TDesenho *desenho = new TDesenho;
    // E o ID
	desenho->ID = ListaDesenhos.size();
    desenho->Altura = altura;
    desenho->GrafoDesenho = grafoDesenho;

    ListaDesenhos.push_back( desenho );
}

TDesenho * CContainerDesenhos::getDesenho(int Indice)
{
  return ListaDesenhos[Indice];
}
//---------------------------------------------------------------------------

int CContainerDesenhos::NumDesenhos()
{
    return ListaDesenhos.size();
}
//---------------------------------------------------------------------------

bool CContainerDesenhos::verificaTextoWrap(void* PonteiroThis, const char *str)
{
  CContainerDesenhos *interno = (CContainerDesenhos*)PonteiroThis;
  return interno->verificaTexto(str);
}
bool CContainerDesenhos::verificaTexto(string str)
{
  bool exists = false;
  for ( int j = 0 ; j < NumDesenhos() ; j++ )
  {
    TDesenho *pnt = getDesenho(j);
    for ( int i = 0 ; i < (int)pnt->GrafoDesenho->_dados->Textos.size() ; i++ )
    {
      if ( pnt->GrafoDesenho->_dados->Textos[i]->texto == str )
      {
        exists = true;
        break;
      }
    }
    if ( exists )
      break;
  }
  return exists;
}

void CContainerDesenhos::GeraListaAdjacencias()
{
  int n, v1, v2;
  for (n=0; n<ParamsInfoCircuitos.Arestas.size(); n++)
  {
    v1=ParamsInfoCircuitos.Arestas[n]->Vertice1;
    v2=ParamsInfoCircuitos.Arestas[n]->Vertice2;
    ParamsInfoCircuitos.VerticesGerais->getItem(v1)->ListaVerticesEArestas->AdicionaVerticeEAresta(v2, n);
    ParamsInfoCircuitos.VerticesGerais->getItem(v2)->ListaVerticesEArestas->AdicionaVerticeEAresta(v1, n);
  }
}

void CContainerDesenhos::ReduzGrafo()
{
#ifdef DEBUG_BUILDER
  FILE *arq=fopen("t:\\debug antes reducao.txt", "w");
//  fprintf(arq, " Indice: %d     Circuito: %s   metragem: %f\n",  CONTADOR_DEBUG, Circuito.NomeCircuito.c_str(), Circuito.metragem);
  for ( int k = 0 ; k < ParamsInfoCircuitos.Arestas->Tamanho() ; k++ )
  {
    TAresta *dbgAr;
    dbgAr = ParamsInfoCircuitos.Arestas->getItem(k);
    fprintf(arq, "%d:\nV1: %d\nV2: %d\nTam: %f\n\n", k, dbgAr->Vertice1, dbgAr->Vertice2, dbgAr->Tam);
  }
  fclose(arq);
#endif
  bool *VerticesVisitados;

  VerticesVisitados = new bool [ParamsInfoCircuitos.VerticesGerais->Tamanho()];
  memset(VerticesVisitados, 0, ParamsInfoCircuitos.VerticesGerais->Tamanho()*sizeof(bool));
  while ( 1 )
  {
  int i = 1;
  bool visitouTudo = true;

  for ( i = 1 ; i < ParamsInfoCircuitos.VerticesGerais->Tamanho() ; i++ )
  {
    TVerticeGeral *vertice = ParamsInfoCircuitos.VerticesGerais->getItem(i);
    // Pega o primeiro vértice que não tenha chance de ser eliminado para come�ar a busca.
    if ( !VerticesVisitados[i] )
    if ( vertice->ListaVerticesEArestas->list.size() != 2 || vertice->texto != "" )
    {
      visitouTudo = false;
      break;
    }
  }
  // i � o primeiro vértice da busca.
  if ( visitouTudo )
    break;


  TArestaReduzida arestaRed;
  VerticesVisitados[i] = true;
  buscaEmProfundidadeOsVertices(VerticesVisitados, i, true, arestaRed);
  }
#ifdef DEBUG_BUILDER
  FILE *arq2=fopen("t:\\debug depois reducao.txt", "w");
  for ( int k = 0 ; k < ParamsInfoCircuitos.ArestasReduzidas->Tamanho() ; k++ )
  {
    TArestaReduzida *dbgArRed;
    dbgArRed = ParamsInfoCircuitos.ArestasReduzidas->getItem(k);
    fprintf(arq2, "%d:\nV1: %d\nV2: %d\nTam: %f\nArestas Retiradas:\n", k, dbgArRed->Vertice1, dbgArRed->Vertice2, dbgArRed->Tam);
    for ( int m = 0; m < dbgArRed->ArestasRetiradas->size() ; m++ )
    {
      fprintf(arq2, "%d ", dbgArRed->ArestasRetiradas->at(m));
    }
    fprintf(arq2, "\n\n");
  }
  fclose(arq2);
#endif
}

void CContainerDesenhos::buscaEmProfundidadeOsVertices(bool *VerticesVisitados, int indice, bool arestazerada, TArestaReduzida arestaRed)
{

  TVerticeGeral *vertice = new TVerticeGeral(*ParamsInfoCircuitos.VerticesGerais->getItem(indice));
  TListaVerticesEArestas *verticesArestas = vertice->ListaVerticesEArestas;
//  delete vertice->ListaVerticesEArestas;
  vertice->ListaVerticesEArestas = new TListaVerticesEArestas;
  int tamanho = -1;

  if ( !arestazerada )
  {
    // Se a aresta não está zerada, então já tem uma redu��o em efeito..
    // Mas como estamos falando de um vértice que não tem tamanho 2, ou que � nomeado, então ele � o fim da redu��o, por isso:
    if ( verticesArestas->list.size() != 2
      || vertice->texto != "" )
    {
      arestaRed.Vertice2 = ParamsInfoCircuitos.VerticesReduzidos->Tamanho();
      tamanho = ParamsInfoCircuitos.VerticesReduzidos->Tamanho();
      ParamsInfoCircuitos.VerticesReduzidos->Adiciona(*vertice);
      ParamsInfoCircuitos.ArestasReduzidas.push_back(arestaRed);
      arestaRed.limpa();
      arestazerada = true;
    }//if ( verticesArestas->Tamanho() != 2 || vertice->texto != "" )

    else if ( verticesArestas->list.size() == 2 && vertice->texto == "" )
    {
      // então, segue removendo...

      for ( int j = 0 ; j < verticesArestas->list.size() ; j++ )
      {
        TVerticeEAresta *VeA = verticesArestas->getVerticeEAresta(j);
        if ( !VerticesVisitados[VeA->Vertice] ) // Em teoria, isso s� vai entrar uma vez.
        {
          VerticesVisitados[VeA->Vertice] = true;
          arestaRed.ArestasRetiradas->push_back(VeA->Aresta);
          arestaRed.Tam += ParamsInfoCircuitos.Arestas[VeA->Aresta]->Tam;
          buscaEmProfundidadeOsVertices(VerticesVisitados, VeA->Vertice, false, arestaRed);
        }//if ( !VerticesVisitados[VeA->Vertice] )
      }//for ( int j = 0 ; j < verticesArestas->Tamanho() ; j++ )
    }//if ( verticesArestas->Tamanho() == 2 && vertice->texto == "" )
  }//if ( !arestazerada )

  if ( arestazerada )
  {
    for ( int j = 0 ; j < verticesArestas->list.size() ; j++ )
    {
      TVerticeEAresta *VeA = verticesArestas->getVerticeEAresta(j);
      if ( !VerticesVisitados[VeA->Vertice] )
      {
        VerticesVisitados[VeA->Vertice] = true;
        if ( tamanho < 0 )
        {
          tamanho = ParamsInfoCircuitos.VerticesReduzidos->Tamanho();
          ParamsInfoCircuitos.VerticesReduzidos->Adiciona(*vertice);
        }
        arestaRed.Vertice1 = tamanho;
        arestaRed.ArestasRetiradas->push_back(VeA->Aresta);
        arestaRed.Tam += ParamsInfoCircuitos.Arestas[VeA->Aresta]->Tam;
        //          busca.push(intVertices(VeA->Vertice, arestaRed.Vertice1)));
        buscaEmProfundidadeOsVertices(VerticesVisitados, VeA->Vertice, false, arestaRed);
        arestaRed.limpa();
        //arestazerada = false;
      }//if ( !VerticesVisitados[VeA->Vertice] )
      // Senão, o vértice já foi visitado. então não há a��o a ser tomada.
    }//for ( int j = 0 ; j < verticesArestas->Tamanho() ; j++ )
  }
  delete vertice;
  delete verticesArestas;
}

void CContainerDesenhos::Conclui(callbackStatusCarregamento& call)
{
//	for(int i(0); i < ParamsInfoCircuitos.VerticesGerais->Tamanho(); ++i)
//	{
//		printf( "(%f, %f) - %s \n", ParamsInfoCircuitos.VerticesGerais->getItem( i )->pos.x,
//						ParamsInfoCircuitos.VerticesGerais->getItem( i )->pos.y,
//						ParamsInfoCircuitos.VerticesGerais->getItem( i )->texto.c_str());
//	}

	for( int i(0); i < ListaDesenhos.size(); ++i)
	{
		vector<shared_ptr<TAresta> > &edges = ListaDesenhos[i]->GrafoDesenho->_arestas;
		ParamsInfoCircuitos.Arestas.insert( ParamsInfoCircuitos.Arestas.end(), edges.begin(), edges.end() );
	}

    int indice = ListaDesenhos.size() - 1;
    if (indice >= 0)
    {
		CGrafoDesenho* grafoDesenho = ListaDesenhos[indice]->GrafoDesenho;
        // Checa vertices duplos(?)
        grafoDesenho->ChecagemVerticeDuplo( ListaDesenhos );
        grafoDesenho->GeraColares( ListaDesenhos );
    }
    // V� o n�mero de desenhos
    ParamsInfoCircuitos.NumDesenhos = NumDesenhos();
    // Cria um novo InfoCircuitos baseado nos par�metros
    callbackVerificaTexto callVT;
    callVT.PonteiroProThis = this;
    callVT.ponteiroFuncao = CContainerDesenhos::verificaTextoWrap;
    GeraListaAdjacencias();

//  ReduzGrafo();
    InfoCircuitos = new CInfoCircuitos( &ParamsInfoCircuitos, call, callVT );
}
//---------------------------------------------------------------------------



void CContainerDesenhos::MostraCircuito(string circuito)
{
  int IndiceCircuitoArestas;
  bool AchouCircuito;
  TCircuito Circuito;

  //AchouCircuito diz se existe um circuito com o nome selecionado
  //AchouCircuito=CArmazenamentoCircuitos::PegaCircuito(circuito, Circuito);

  if (AchouCircuito)
  {
    IndiceCircuitoArestas=InfoCircuitos->ListaArestasDoCircuito(circuito);
    if (IndiceCircuitoArestas>=0)
    {

    }
    else
    {
      string erro;
      erro = "não foi encontrado caminho.";
      bool exists, equips;
      equips = true;
      if ( InfoCircuitos->VerticesGerais->AchaVerticePeloTexto(Circuito.Origem) < 0 )
      {
        exists = false;
        equips = false;
        for ( int j = 0 ; j < this->NumDesenhos() ; j++ )
        {
          TDesenho *pnt = this->getDesenho(j);
          for ( int i = 0 ; i < (int)pnt->GrafoDesenho->_dados->Textos.size() ; i++ )
          {
            if ( pnt->GrafoDesenho->_dados->Textos[i]->texto == Circuito.Origem )
            {
              exists = true;
              break;
            }
          }
          if ( exists )
            break;
        }

        if ( exists )
          erro += "\nO texto de origem está nos desenhos, porém não está associado a um equipamento ou bandeirola.";
        else
          erro += "\nO texto de origem não existe nos desenhos";
      }

      /***/
      if ( InfoCircuitos->VerticesGerais->AchaVerticePeloTexto(Circuito.Destino) < 0 )
      {
        exists = false;
        equips = false;
        for ( int j = 0 ; j < this->NumDesenhos() ; j++ )
        {
          TDesenho *pnt = this->getDesenho(j);
          for ( int i = 0 ; i < (int)pnt->GrafoDesenho->_dados->Textos.size() ; i++ )
          {
            if ( pnt->GrafoDesenho->_dados->Textos[i]->texto == Circuito.Destino )
            {
              exists = true;
              break;
            }
          }
          if ( exists )
            break;
        }

        if ( exists )
          erro += "\nO texto de destino está nos desenhos, porém não está associado a um equipamento ou bandeirola.";
        else
          erro += "\nO texto de destino não existe nos desenhos";
      }

      /***/

      ShowMessage(erro);

      /***/

      if ( equips )
        MostraDoubleArvore(Circuito.Origem, Circuito.Destino);
    }
  }
  else
    ShowMessage("Circuito desconhecido.");
}
//---------------------------------------------------------------------------

void CContainerDesenhos::MostraArvore(string Nome)
{
  for (int n=0; n<NumDesenhos(); n++)
  {
    int vertice=InfoCircuitos->VerticesGerais->AchaVerticePeloTexto(Nome);
  }
}
//---------------------------------------------------------------------------

void CContainerDesenhos::MostraDoubleArvore(string Nome, string Nome2)
{
  for (int n=0; n<NumDesenhos(); n++)
  {
    int vertice=InfoCircuitos->VerticesGerais->AchaVerticePeloTexto(Nome);
    int vertice2=InfoCircuitos->VerticesGerais->AchaVerticePeloTexto(Nome2);
  }
}
//---------------------------------------------------------------------------


