//---------------------------------------------------------------------------
#pragma hdrstop
#include "UContainerDesenhos.h"
#include "UErros.h"
#include "TDesenho.h"
#include "Graph.h"
#include "Electrical/ElectricalElementsBuilder.h"
#include "Electrical/ElectricalElements.h"
#include "GraphBuilder.h"
#include "UDadosGenerico.h"

using namespace std;

void CContainerDesenhos::addDrawing( std::shared_ptr<CDadosGenerico> dados, double altura, string fileName )
{
    // Cria um novo desenho
    shared_ptr<TDesenho> desenho = make_shared<TDesenho>();
	dados->_drawing = desenho;
    desenho->Altura = altura;
	desenho->_dados = dados;
	desenho->_graph =  make_shared<Graph>();
	desenho->_electricalElements = make_shared<ElectricalElements>();

	ElectricalElementsBuilder elementsBuilder( desenho->_graph, dados, desenho->_electricalElements );
	elementsBuilder.build();
	GraphBuilder::build( desenho->_graph, desenho->_electricalElements );
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


void CContainerDesenhos::Conclui()
{
	_graph = make_shared<Graph>();
	for( int i(0); i < ListaDesenhos.size(); ++i)
	{
		_graph->merge( ListaDesenhos[i]->_graph );
	}

    if (ListaDesenhos.size() > 1)
    {
        // Checa vertices duplos(?)
		ChecagemVerticeDuplo( ListaDesenhos[0]->_graph );
        ligaColaresEntreDesenhos();
    }
    // Cria um novo InfoCircuitos baseado nos par�metros
    _graph->GeraListaAdjacencias();

    InfoCircuitos = make_shared<CInfoCircuitos>( _graph );
}
//---------------------------------------------------------------------------


void CContainerDesenhos::ChecagemVerticeDuplo( shared_ptr<Graph> graph )
{
    int n;
    vector< shared_ptr<TVerticeGeral> > Lista;
    graph->_verticesGerais->ListaOrd( Lista );  //gera lista ordenada
    shared_ptr<TVerticeGeral> V1, V2;
    string Ultimo;
    for (n = 0; n < (int) (Lista.size() - 1); n++)
    {
        V1 = Lista[ n ];
        if (V1->texto == "")
			continue;

        V2 = Lista[ n + 1 ];
        if (V1->texto != V2->texto)
			continue;

		if (V1->drawing.get() != V2->drawing.get()
                && (V1->TipoElemento == INSTRUMENTO && V2->TipoElemento == INSTRUMENTO)
                && (n + 2 < (int) (Lista.size() - 1)
                        && (Lista[ n + 1 ])->texto != V1->texto))
        {
            n++;
            break;
        }

        CErrosMsg *erros = CErrosMsg::getInstance();
        erros->novoErro( "Elementos com o texto \"" + V1->texto + "\" repetido: " );

        for (; n < (int) Lista.size(); n++)
        {
            if ((Lista[ n ])->texto != V1->texto)
				break;
            string tipo;
            if ( Lista[ n ]->TipoElemento == INSTRUMENTO)
            {
                tipo = "Equipamento";
            }
            else if ( Lista[ n ]->TipoElemento == CABO)
            {
                tipo = "Cabo";
            }
            else if ( Lista[ n ]->TipoElemento == BANDEIROLA)
            {
                tipo = "Bandeirola";
            }
            else
                tipo = "Desconhecido";

			erros->novoErro( "No desenho: " + (Lista[ n ])->drawing->NomeArquivo + " em nível de " + tipo );
        }
        erros->novoErro( "" );
    }
}


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
  if ( InfoCircuitos->_circuitEdges[circuito].get() == 0 )
    {
 string erro;
      erro = "não foi encontrado caminho.";
      bool exists, equips;
      equips = true;
      if ( InfoCircuitos->_graph->_verticesGerais->AchaVerticePeloTexto(Circuito.Origem).get() == 0 )
      {
        exists = false;
        equips = false;
		for ( auto drawing : ListaDesenhos )
        {
			for ( auto text : drawing->_dados->Textos )
			{
				if ( text->texto == Circuito.Origem )
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
		for ( auto drawing : ListaDesenhos )
        {
			for ( auto text : drawing->_dados->Textos )
			{
				if ( text->texto == Circuito.Destino )
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


