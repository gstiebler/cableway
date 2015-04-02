//---------------------------------------------------------------------------
#pragma hdrstop
#include "UContainerDesenhos.h"
#include "UErros.h"
#include "TDesenho.h"
#include "Graph.h"

int ID_PROJETO;

TDesenho::~TDesenho()
{
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
    // Cria um novo desenho
    shared_ptr<TDesenho> desenho( new TDesenho );

    // O Id
	dados->_drawing = desenho;
	shared_ptr<Graph> graph( new Graph );

    shared_ptr<CGrafoDesenho> grafoDesenho( new CGrafoDesenho( graph, dados) );

    // E o ID
    desenho->Altura = altura;
    desenho->GrafoDesenho = grafoDesenho;

    ListaDesenhos.push_back( desenho );
}

shared_ptr<TDesenho> CContainerDesenhos::getDesenho(int Indice)
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
    shared_ptr<TDesenho> pnt = getDesenho(j);
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
	for ( int n(0); n < _graph->_arestas.size(); n++)
	{
		shared_ptr<TAresta> aresta = _graph->_arestas[n];
		shared_ptr<TVerticeGeral> v1 = aresta->_vertices[0];
		shared_ptr<TVerticeGeral> v2 = aresta->_vertices[1];

		v1->ListaVerticesEArestas->AdicionaVerticeEAresta( v2, aresta );
		v2->ListaVerticesEArestas->AdicionaVerticeEAresta( v1, aresta );
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

	_graph = shared_ptr<Graph>( new Graph );
	for( int i(0); i < ListaDesenhos.size(); ++i)
	{
		_graph->merge( ListaDesenhos[i]->GrafoDesenho->_graph );
	}

    if (ListaDesenhos.size() > 1)
    {
		shared_ptr<CGrafoDesenho> grafoDesenho = ListaDesenhos[0]->GrafoDesenho;
        // Checa vertices duplos(?)
        grafoDesenho->ChecagemVerticeDuplo( ListaDesenhos );
        GeraColares();
    }
    // Cria um novo InfoCircuitos baseado nos par�metros
    GeraListaAdjacencias();

    InfoCircuitos = shared_ptr<CInfoCircuitos>( new CInfoCircuitos( _graph ) );
}
//---------------------------------------------------------------------------



void CContainerDesenhos::GeraColares()
{
    vector< shared_ptr<TVerticeGeral> > Lista;
	_graph->_verticesGerais->ListaOrd( Lista );  //gera lista ordenada
    shared_ptr<TVerticeGeral> V1, V2;
    for (int n = 0; n < (int) (Lista.size() - 1); n++)
    {
        V1 = Lista[n];
        if ( V1->texto == "" )
			continue;

        V2 = Lista[n + 1];
		if ((V1->texto != V2->texto) || (V1->drawing.get() == V2->drawing.get()) || (V1->TipoElemento != INSTRUMENTO) || (V2->TipoElemento != INSTRUMENTO))
			continue;

		double alturaDaAresta = V1->drawing->Altura - V2->drawing->Altura;
        if (alturaDaAresta < 0)
            alturaDaAresta *= -1;
        shared_ptr<TAresta> Aresta( new TAresta( "" ) );
        Aresta->AdicionaVertices( V1, V2, alturaDaAresta );
		_graph->_arestas.push_back( Aresta );

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
      if ( InfoCircuitos->_graph->_verticesGerais->AchaVerticePeloTexto(Circuito.Origem).get() == 0 )
      {
        exists = false;
        equips = false;
        for ( int j = 0 ; j < this->NumDesenhos() ; j++ )
        {
          shared_ptr<TDesenho> pnt = getDesenho(j);
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
      if ( InfoCircuitos->_graph->_verticesGerais->AchaVerticePeloTexto(Circuito.Destino).get() == 0 )
      {
        exists = false;
        equips = false;
        for ( int j = 0 ; j < this->NumDesenhos() ; j++ )
        {
          shared_ptr<TDesenho> pnt = getDesenho(j);
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

}
//---------------------------------------------------------------------------

void CContainerDesenhos::MostraDoubleArvore(string Nome, string Nome2)
{

}
//---------------------------------------------------------------------------


