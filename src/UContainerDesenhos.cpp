//---------------------------------------------------------------------------
#pragma hdrstop
#include "UContainerDesenhos.h"
#include "UStructs.h"
#include "UErros.h"

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
	int iV1, iV2;
	for ( int n(0); n<ParamsInfoCircuitos.Arestas.size(); n++)
	{
		iV1 = ParamsInfoCircuitos.Arestas[n]->Vertice1;
		iV2 = ParamsInfoCircuitos.Arestas[n]->Vertice2;

		shared_ptr<TVerticeGeral> v1 = ParamsInfoCircuitos.VerticesGerais->vertices[iV1];
		shared_ptr<TVerticeGeral> v2 = ParamsInfoCircuitos.VerticesGerais->vertices[iV2];

		v1->ListaVerticesEArestas->AdicionaVerticeEAresta( iV2, n );
		v2->ListaVerticesEArestas->AdicionaVerticeEAresta( iV1, n );
	}
}



void CContainerDesenhos::Conclui()
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

    if (ListaDesenhos.size() > 1)
    {
		CGrafoDesenho* grafoDesenho = ListaDesenhos[0]->GrafoDesenho;
        // Checa vertices duplos(?)
        grafoDesenho->ChecagemVerticeDuplo( ListaDesenhos );
        GeraColares();
    }
    // V� o n�mero de desenhos
    ParamsInfoCircuitos.NumDesenhos = NumDesenhos();
    // Cria um novo InfoCircuitos baseado nos par�metros
    GeraListaAdjacencias();

    InfoCircuitos = new CInfoCircuitos( &ParamsInfoCircuitos );
}
//---------------------------------------------------------------------------



void CContainerDesenhos::GeraColares()
{
    vector< shared_ptr<TVerticeGeral> > Lista;
	ParamsInfoCircuitos.VerticesGerais->ListaOrd( Lista );  //gera lista ordenada
    shared_ptr<TVerticeGeral> V1, V2;
    for (int n = 0; n < (int) (Lista.size() - 1); n++)
    {
        V1 = Lista[n];
        if ( V1->texto == "" )
			continue;

        V2 = Lista[n + 1];
        if ((V1->texto != V2->texto) || (V1->iDesenho == V2->iDesenho) || (V1->TipoElemento != INSTRUMENTO) || (V2->TipoElemento != INSTRUMENTO))
			continue;

        double alturaDaAresta = ListaDesenhos[V1->iDesenho]->Altura - ListaDesenhos[V2->iDesenho]->Altura;
        if (alturaDaAresta < 0)
            alturaDaAresta *= -1;
        shared_ptr<TAresta> Aresta( new TAresta( "" ) );
        Aresta->AdicionaVertices( V1->IndiceOriginal, V2->IndiceOriginal, alturaDaAresta );
        Aresta->IndiceDesenho = I_DESENHO_NULO;
        Aresta->IDArquivo = I_DESENHO_NULO;
		ParamsInfoCircuitos.Arestas.push_back( Aresta );

		V1->EhColar = V2->EhColar = true;
    }
}
//---------------------------------------------------------------------------



void CContainerDesenhos::MostraCircuito(string circuito)
{
  int IndiceCircuitoArestas;
  bool AchouCircuito;
  TCircuito Circuito;
  

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
	  
	CErrosMsg::getInstance()->novoErro( erro );

      /***/

      if ( equips )
        MostraDoubleArvore(Circuito.Origem, Circuito.Destino);
    }
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


