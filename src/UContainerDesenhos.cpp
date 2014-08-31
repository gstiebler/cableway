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

CContainerDesenhos::CContainerDesenhos(TNiveisProjetoTransfer *niveis)
{
  // Cria uma nova lista pra guardar os desenhos
  //ListaDesenhos=new TList;

  // Bota NULL no InfoCircuitos e no frmDesenhoAbas
  InfoCircuitos=NULL;
  //frmDesenhoAbas=NULL;

  Niveis = niveis;
}
//---------------------------------------------------------------------------

void CContainerDesenhos::MudaNiveisDeProjeto(TNiveisProjeto* NiveisProjeto)
{
  liberaTNiveisProjetoTransfer(Niveis);
  delete Niveis;
  CConfig::CarregaBanco(NiveisProjeto);
  // Depois cria um TNiveisProjetoTransfer, carrega o NiveisProjeto nele e apaga o NiveisProjeto.
  Niveis = new TNiveisProjetoTransfer();
  NiveisProjeto->exportaTransfer(Niveis);
}

CContainerDesenhos::~CContainerDesenhos()
{
  // Apaga o frmDesenhoAbas (n�o � necess�rio apagar nada l� dentro?)
  //if (frmDesenhoAbas)
  //  delete frmDesenhoAbas;

  // Apaga o conte�do da lista de desenhos
    for (int n=0; n<ListaDesenhos.size(); n++)
    delete (TDesenho *)(ListaDesenhos[n]);

  // Se o InfoCircuitos j� n�o for nulo, ent�o o apaga
  if (InfoCircuitos)
  {
    delete InfoCircuitos;
    InfoCircuitos = NULL;
  }

  // Libera as coisas dentro do TNiveisProjetoTransfer e depois ele
  liberaTNiveisProjetoTransfer(Niveis);
  delete Niveis;
}
//---------------------------------------------------------------------------

void CContainerDesenhos::AdicionaDesenho(string NomeArquivo, int id, double altura, bool carregagrafo)
{
  // Cria um novo desenho
  TDesenho *Desenho=new TDesenho;
  // Preenche o nome
  Desenho->NomeArquivo=NomeArquivo;
  // E o ID
  Desenho->ID = id;
  Desenho->Altura = altura;

  // Cria um novo params
  TParamsGrafoDesenho ParamsGrafoDesenho;
  // O Id
  ParamsGrafoDesenho.IDArquivo = id;
  // Para carregar o grafo
  ParamsGrafoDesenho.CarregaGrafo=carregagrafo;
  // Define as alturas -- HARDCODED?? :(
  ParamsGrafoDesenho.AlturaTeto=2.0;
  ParamsGrafoDesenho.AlturaInterrup=0.7;
  ParamsGrafoDesenho.Altura = altura;
  // Preenche o �ndice do desenho
  ParamsGrafoDesenho.IndiceDesenho=ListaDesenhos.size();
  // Passa um ponteiro para o VerticesGerais (TVerticesGerais)
  ParamsGrafoDesenho.VerticesGerais=ParamsInfoCircuitos.VerticesGerais;
  // E um ponteiro pro Arestas (TListaArestas)
  ParamsGrafoDesenho.Arestas=ParamsInfoCircuitos.Arestas;
  // E um ponteiro pro NiveisProjeto (TNiveisProjetoTransfer)
  ParamsGrafoDesenho.NiveisProjeto=Niveis;
  try
  {
	  //TODO carregar a estrutura TDadosTransfer
	CDadosGenerico dados;
    // Tenta criar um grafodesenho com os par�metros passados
    Desenho->GrafoDesenho=new CGrafoDesenho(ParamsGrafoDesenho, &dados);
    // E adicionar o desenho na lista de desenhos
    ListaDesenhos.push_back(Desenho);
  }
  catch (...)
  {
    // Algo deu errado ao criar/adicionar o desenho.
    ShowMessage("Erro ao criar desenho na mem�ria.");
  }
}
//---------------------------------------------------------------------------

void CContainerDesenhos::addDrawing( TDesenho *Desenho )
{
    ListaDesenhos.push_back( Desenho );
}

TDesenho * CContainerDesenhos::getDesenho(int Indice)
{
  return (TDesenho *)(ListaDesenhos[Indice]);
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
    for ( int i = 0 ; i < (int)pnt->GrafoDesenho->Dados->Textos.size() ; i++ )
    {
      if ( pnt->GrafoDesenho->Dados->Textos[i].texto == str )
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
  for (n=0; n<ParamsInfoCircuitos.Arestas->Tamanho(); n++)
  {
    v1=ParamsInfoCircuitos.Arestas->getItem(n)->Vertice1;
    v2=ParamsInfoCircuitos.Arestas->getItem(n)->Vertice2;
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
    // Pega o primeiro v�rtice que n�o tenha chance de ser eliminado para come�ar a busca.
    if ( !VerticesVisitados[i] )
    if ( vertice->ListaVerticesEArestas->Tamanho() != 2 || vertice->texto != "" )
    {
      visitouTudo = false;
      break;
    }
  }
  // i � o primeiro v�rtice da busca.
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
    // Se a aresta n�o est� zerada, ent�o j� tem uma redu��o em efeito..
    // Mas como estamos falando de um v�rtice que n�o tem tamanho 2, ou que � nomeado, ent�o ele � o fim da redu��o, por isso:
    if ( verticesArestas->Tamanho() != 2
      || vertice->texto != "" )
    {
      arestaRed.Vertice2 = ParamsInfoCircuitos.VerticesReduzidos->Tamanho();
      tamanho = ParamsInfoCircuitos.VerticesReduzidos->Tamanho();
      ParamsInfoCircuitos.VerticesReduzidos->Adiciona(*vertice);
      ParamsInfoCircuitos.ArestasReduzidas->Adiciona(arestaRed);
      arestaRed.limpa();
      arestazerada = true;
    }//if ( verticesArestas->Tamanho() != 2 || vertice->texto != "" )

    else if ( verticesArestas->Tamanho() == 2 && vertice->texto == "" )
    {
      // ent�o, segue removendo...

      for ( int j = 0 ; j < verticesArestas->Tamanho() ; j++ )
      {
        TVerticeEAresta *VeA = verticesArestas->getVerticeEAresta(j);
        if ( !VerticesVisitados[VeA->Vertice] ) // Em teoria, isso s� vai entrar uma vez.
        {
          VerticesVisitados[VeA->Vertice] = true;
          arestaRed.ArestasRetiradas->push_back(VeA->Aresta);
          arestaRed.Tam += ParamsInfoCircuitos.Arestas->getItem(VeA->Aresta)->Tam;
          buscaEmProfundidadeOsVertices(VerticesVisitados, VeA->Vertice, false, arestaRed);
        }//if ( !VerticesVisitados[VeA->Vertice] )
      }//for ( int j = 0 ; j < verticesArestas->Tamanho() ; j++ )
    }//if ( verticesArestas->Tamanho() == 2 && vertice->texto == "" )
  }//if ( !arestazerada )

  if ( arestazerada )
  {
    for ( int j = 0 ; j < verticesArestas->Tamanho() ; j++ )
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
        arestaRed.Tam += ParamsInfoCircuitos.Arestas->getItem(VeA->Aresta)->Tam;
        //          busca.push(intVertices(VeA->Vertice, arestaRed.Vertice1)));
        buscaEmProfundidadeOsVertices(VerticesVisitados, VeA->Vertice, false, arestaRed);
        arestaRed.limpa();
        //arestazerada = false;
      }//if ( !VerticesVisitados[VeA->Vertice] )
      // Sen�o, o v�rtice j� foi visitado. Ent�o n�o h� a��o a ser tomada.
    }//for ( int j = 0 ; j < verticesArestas->Tamanho() ; j++ )
  }
  delete vertice;
  delete verticesArestas;
}

void CContainerDesenhos::Conclui(callbackStatusCarregamento& call)
{
    int indice=ListaDesenhos.size()-1;
  if (indice>=0)
  {
    // Checa vertices duplos(?)
    ((TDesenho *)(ListaDesenhos[indice]))->GrafoDesenho->ChecagemVerticeDuplo(ListaDesenhos);
    ((TDesenho *)(ListaDesenhos[indice]))->GrafoDesenho->GeraColares(ListaDesenhos);
  }
  // V� o n�mero de desenhos
  ParamsInfoCircuitos.NumDesenhos=NumDesenhos();
  // Cria um novo InfoCircuitos baseado nos par�metros
  callbackVerificaTexto callVT;
  callVT.PonteiroProThis = this;
  callVT.ponteiroFuncao = CContainerDesenhos::verificaTextoWrap;
  GeraListaAdjacencias();
  
#ifdef DEBUG_BUILDER
    TStringList *lista_histograma = new TStringList();
    int histograma[300] = {0};
    for(int k = 0; k < ParamsInfoCircuitos.VerticesGerais->Tamanho(); k++)
    {
      TVerticeGeral *item = ParamsInfoCircuitos.VerticesGerais->getItem(k);
      histograma[item->ListaVerticesEArestas->Tamanho()]++;
    }
    for(int k = 0; k < 300; k++)
      lista_histograma->Add("Vertices " + IntToStr(k) + ": " + IntToStr(histograma[k]));
    lista_histograma->SaveToFile("t:\\histograma_vertices.txt");
#endif
//  ReduzGrafo();
  InfoCircuitos=new CInfoCircuitos(&ParamsInfoCircuitos, call, callVT);
}
//---------------------------------------------------------------------------
//TfrmDesenhoAbas *CContainerDesenhos::getfrmDesenhoAbas()
//{
//  return frmDesenhoAbas;
//}

void CContainerDesenhos::CriaFormDesenho(int Indice)
{
  //// Se n�o form de abas, ent�o faz umnovo
  //if (!frmDesenhoAbas)
  //  frmDesenhoAbas=new TfrmDesenhoAbas(NULL, &frmDesenhoAbas, ListaDesenhos->Count);
  //// Adiciona uma aba
  //frmDesenhoAbas->AdicionaAba(InfoCircuitos, getDesenho(Indice)->GrafoDesenho, Indice);
  //// Bota o form maximizado
  //frmDesenhoAbas->WindowState=wsMaximized;
  //// E mostra
  //frmDesenhoAbas->Show();
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
      for (int n=0; n<NumDesenhos(); n++)
        if (InfoCircuitos->ArestasDoCircuito->getItem(IndiceCircuitoArestas)->ArestasDesenho[n].size())
        {
          CriaFormDesenho(n);
          //frmDesenhoAbas->Abas[n].fraDesenho->MostraDesenho->MostraCircuito(IndiceCircuitoArestas);
          //frmDesenhos[n]->MostraDesenho->MostraCircuito(IndiceCircuitoArestas);
          //frmDesenhos[n]->Show();
        }
    }
    else
    {
      string erro;
      erro = "N�o foi encontrado caminho.";
      bool exists, equips;
      equips = true;
      if ( InfoCircuitos->VerticesGerais->AchaVerticePeloTexto(Circuito.Origem) < 0 )
      {
        exists = false;
        equips = false;
        for ( int j = 0 ; j < this->NumDesenhos() ; j++ )
        {
          TDesenho *pnt = this->getDesenho(j);
          for ( int i = 0 ; i < (int)pnt->GrafoDesenho->Dados->Textos.size() ; i++ )
          {
            if ( pnt->GrafoDesenho->Dados->Textos[i].texto == Circuito.Origem )
            {
              exists = true;
              break;
            }
          }
          if ( exists )
            break;
        }

        if ( exists )
          erro += "\nO texto de origem est� nos desenhos, por�m n�o est� associado a um equipamento ou bandeirola.";
        else
          erro += "\nO texto de origem n�o existe nos desenhos";
      }

      /***/
      if ( InfoCircuitos->VerticesGerais->AchaVerticePeloTexto(Circuito.Destino) < 0 )
      {
        exists = false;
        equips = false;
        for ( int j = 0 ; j < this->NumDesenhos() ; j++ )
        {
          TDesenho *pnt = this->getDesenho(j);
          for ( int i = 0 ; i < (int)pnt->GrafoDesenho->Dados->Textos.size() ; i++ )
          {
            if ( pnt->GrafoDesenho->Dados->Textos[i].texto == Circuito.Destino )
            {
              exists = true;
              break;
            }
          }
          if ( exists )
            break;
        }

        if ( exists )
          erro += "\nO texto de destino est� nos desenhos, por�m n�o est� associado a um equipamento ou bandeirola.";
        else
          erro += "\nO texto de destino n�o existe nos desenhos";
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
    CriaFormDesenho(n);
    int vertice=InfoCircuitos->VerticesGerais->AchaVerticePeloTexto(Nome);
    //if (vertice>=0)                                           
    //  frmDesenhoAbas->Abas[n].fraDesenho->MostraDesenho->MostraArvore(vertice);
    //frmDesenhos[n]->MostraDesenho->MostraArvore(vertice);
    //frmDesenhos[n]->Show();
  }
}
//---------------------------------------------------------------------------

void CContainerDesenhos::MostraDoubleArvore(string Nome, string Nome2)
{
  for (int n=0; n<NumDesenhos(); n++)
  {
    CriaFormDesenho(n);
    int vertice=InfoCircuitos->VerticesGerais->AchaVerticePeloTexto(Nome);
    int vertice2=InfoCircuitos->VerticesGerais->AchaVerticePeloTexto(Nome2);
    //if (vertice>=0&&vertice2>=0)          
    //  frmDesenhoAbas->Abas[n].fraDesenho->MostraDesenho->MostraDoubleArvore(vertice, vertice2);
    //frmDesenhos[n]->MostraDesenho->MostraDoubleArvore(vertice, vertice2);
    //frmDesenhos[n]->Show();
  }
}
//---------------------------------------------------------------------------


