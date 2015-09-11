//---------------------------------------------------------------------------
#pragma hdrstop
#include "UContainerDesenhos.h"
#include "UErros.h"
#include "TDesenho.h"
#include "Graph.h"


void CContainerDesenhos::addDrawing( std::shared_ptr<CDadosGenerico> dados, double altura, string fileName )
{
    // Cria um novo desenho
    shared_ptr<TDesenho> desenho = make_shared<TDesenho>();

    // O Id
	dados->_drawing = desenho;

    // E o ID
    desenho->Altura = altura;
    desenho->GrafoDesenho = make_shared<CGrafoDesenho>( make_shared<Graph>(), dados);
	desenho->NomeArquivo = fileName;

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


void CContainerDesenhos::Conclui()
{
	_graph = make_shared<Graph>();
	for( int i(0); i < ListaDesenhos.size(); ++i)
	{
		_graph->merge( ListaDesenhos[i]->GrafoDesenho->_graph );
	}

    if (ListaDesenhos.size() > 1)
    {
		shared_ptr<CGrafoDesenho> grafoDesenho = ListaDesenhos[0]->GrafoDesenho;
        // Checa vertices duplos(?)
        grafoDesenho->ChecagemVerticeDuplo( ListaDesenhos );
        ligaColaresEntreDesenhos();
    }
    // Cria um novo InfoCircuitos baseado nos par�metros
    _graph->GeraListaAdjacencias();

    InfoCircuitos = make_shared<CInfoCircuitos>( _graph );
}
//---------------------------------------------------------------------------



void CContainerDesenhos::ligaColaresEntreDesenhos()
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

		double alturaDaAresta = fabs( V1->drawing->Altura - V2->drawing->Altura );
        shared_ptr<TAresta> Aresta = make_shared<TAresta>( "" );
        Aresta->AdicionaVertices( V1, V2, alturaDaAresta );
		_graph->_arestas.push_back( Aresta );

		V1->EhColar = V2->EhColar = true;
    }
}
//---------------------------------------------------------------------------



void CContainerDesenhos::MostraCircuito(string circuito)
{
  bool AchouCircuito;
  TCircuito Circuito;
	if ( InfoCircuitos->ArestasDoCircuito[circuito].get() == 0 )
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


