//---------------------------------------------------------------------------
#pragma hdrstop
#include "UGrafoDesenho.h"
#include "UErros.h"
#include "UGeometria.h"
#include "UContainerDesenhos.h"
#include "UListaItensCelula.h"
#include "TDesenho.h"
#include "Graph.h"


CCaboReta::CCaboReta()
{
    ponta[0] = ponta[1] = false;
}
//---------------------------------------------------------------------------

CCaboReta::~CCaboReta()
{
}
//---------------------------------------------------------------------------

void CCaboReta::AdicionaVertice(shared_ptr<TVerticeGeral> Vertice)
{
	VerticesReta.push_back( Vertice );
}
//---------------------------------------------------------------------------

//Retorna verdadeiro se o ponto recebido está próximo ao primeiro ponto do vetor
bool CCaboReta::EhOPrimeiroPonto( TPonto ponto )
{
	TPonto pontoM = _multipoint->pontos[0]; //primeiro ponto
    TPonto pontoR = _multipoint->pontos[_multipoint->pontos.size() - 1]; //último ponto
    //pega Distância entre ponto inicial e final, pode ser a Distância em x ou em y, e joga em DistMin
    //essas linhas são utilizadas para se ter uma no��o de quanto � uma Distância pequena no desenho
    double DistMin = fabs( pontoM.x - pontoR.x ) * 0.1;
    if (fabs( pontoM.y - pontoR.y ) * 0.1 > DistMin)
        DistMin = fabs( pontoM.y - pontoR.y ) * 0.1;
//  double distX, distY;

//  distX = fabs(pontoM.x - ponto.x);
//  distY = fabs(pontoM.y - ponto.y);

    //retorna verdadeiro se o ponto recebido � próximo tanto em x quanto em y do primeiro ponto
    if (fabs( pontoM.x - ponto.x ) < DistMin && fabs( pontoM.y - ponto.y ) < DistMin)
        return true;

    return false;
}

//Retorna verdadeiro se o ponto recebido está próximo ao último ponto do vetor
bool CCaboReta::EhOUltimoPonto( TPonto ponto )
{
    TPonto pontoM = _multipoint->pontos[_multipoint->pontos.size() - 1];
    TPonto pontoR = _multipoint->pontos[0];
    double DistMin = fabs( pontoM.x - pontoR.x ) * 0.1;
    if (fabs( pontoM.y - pontoR.y ) * 0.1 > DistMin)
        DistMin = fabs( pontoM.y - pontoR.y ) * 0.1;
//  double distX, distY;

//  distX = fabs(pontoM.x - ponto.x);
//  distY = fabs(pontoM.y - ponto.y);

    if (fabs( pontoM.x - ponto.x ) < DistMin && fabs( pontoM.y - ponto.y ) < DistMin)
    {
        return true;
    }

    return false;
}

//---------------------------------------------------------------------------

CGrafoDesenho::CGrafoDesenho( shared_ptr<Graph> graph, std::shared_ptr<CDadosGenerico> Dados) :
	_graph( graph )
{
    // Cria um DadosGenerico para ser usado
    this->_dados = Dados;

    unsigned int n;
    memset( TipoElementoCor, 0, NUM_CORES * sizeof(TTipoElemento) );

    //O vértice 0 não pode ser usado, por isso adiciona-se este vértice vazio
    shared_ptr<TVerticeGeral> temp = make_shared<TVerticeGeral>();
    _graph->_verticesGerais->Adiciona( temp );

    GeraListaCabos();
    GeraVerticesBandeirola();
    GeraVerticesArcos();
    GeraVerticesPontaCabos();
    GeraVerticesInstrumentos();
    OrdenaVerticesRetas();
    GeraArestas();

    _ult = Dados->Multipoint.size();
}
//---------------------------------------------------------------------------

CGrafoDesenho::~CGrafoDesenho()
{
}
//---------------------------------------------------------------------------

//percorre os elementos b�sicos de desenho do CableWay, como Multipoint e Arcos
//e converte para os elementos CabosReta
void CGrafoDesenho::GeraListaCabos()
{
    int n;
    vector<TPonto> Pontos;
    double DifX, DifY;
    for (n = 0; n < _dados->Multipoint.size(); n++)
    {
        //verifica o tipo de elemento pela cor
        if ( _dados->Multipoint[n]->Nivel == CABO )
        {
			shared_ptr<CCaboReta> caboReta = make_shared<CCaboReta>();
			_cabosReta.push_back( caboReta );
			caboReta->_multipoint = _dados->Multipoint[n];
            Pontos = _dados->Multipoint[n]->pontos;
            DifX = fabs( Pontos[0].x - Pontos[1].x );
            DifY = fabs( Pontos[0].y - Pontos[1].y );
            caboReta->TipoOrientacao = (DifY > DifX) ? VERTICAL : HORIZONTAL;
        }
    }

    double SomaRaios = 0;
    for (n = 0; n < _dados->Arcos.size(); n++)
    {
		if ( _dados->Arcos[n]->Nivel == CABO )
        {
			_cabosArco.push_back( make_shared<CCaboArco>() );
			_cabosArco.back()->_arco = _dados->Arcos[n];

            SomaRaios += _dados->Arcos[n]->EixoPrimario;
        }
    }
    _mediaRaioCaboArco = _cabosArco.size() > 0 ? SomaRaios / _cabosArco.size() : 0;
    //DistMinElemCaboPraOpenGL = MediaRaioCaboArco * FATOR_DIST_MIN_ELEM_CABO;
	//temporary!
	_distMinElemCaboPraOpenGL = 10.0;
}
//---------------------------------------------------------------------------

//Acha o cabo mais próximo a um ponto. Retorna o índice do cabo,
//a Distância � este cabo, e o ponto do cabo que está mais próximo ao ponto dado
void CGrafoDesenho::CaboMaisProximo(TPonto &ponto, std::shared_ptr<CCaboReta> &_straightCable, double &DistMaisProx, TPonto &PosVertice, int Diferente, int Nivel)
{
    int m;
    double MenorDist, Dist;
    _straightCable.reset();
    MenorDist = Infinity;
	for (m = 0; m < _cabosReta.size(); m++)
    {
		shared_ptr<TMultipoint> multipoint = _cabosReta[m]->_multipoint;
        if ((Nivel != -1) && (multipoint->Nivel != Nivel))
            continue;
		vector<TPonto> &pontosCabo = multipoint->pontos;
        TPonto volta;
        Dist = DistPontoParaSegmentoReta( &pontosCabo[0], ponto, volta );
        if ((Dist < MenorDist) && (m != Diferente))
        {
            MenorDist = Dist;
            _straightCable = _cabosReta[m];
            PosVertice = volta;
        }
    }  //for (m=0; m<NumCabosReta; m++)
    DistMaisProx = MenorDist;
    if ( !_straightCable )
    {
        CErrosMsg *erros = CErrosMsg::getInstance();
        erros->novoErro( "Atenção: " + _dados->NomeArq + " não possui cabos." );
        /*    if ( Dados->IMP )
         {
         //ShowMessage("Erro: não há cabo.");
         }
         */
    }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//adiciona os vértices das bandeirolas nas estruturas dos cabos
void CGrafoDesenho::GeraVerticesBandeirola()
{
    int n, m;
    TPonto PontoBandeirola[2], PontoNaReta;
    vector<TListaItensCelula> &ListaBandeirolas = _dados->InfoCelula.ListaCelulasBandeirolas;

    //percorre todas as bandeirolas
    for (n = 0; n < ListaBandeirolas.size(); n++)
    {
        int NumArcosBandeirola = 0;
        TListaItensCelula ListaItens( ListaBandeirolas[ n ] );
        shared_ptr<TVerticeGeral> VerticeGeral = make_shared<TVerticeGeral>();
        VerticeGeral->TipoVertice = VERTICE_BANDEIROLA;

        vector<TPonto> PontosExtremidadesElementosBandeirola;
        //utilizado para verificar se a bandeirola � composta de uma reta horizontal e outra inclinada
        vector<TReta> RetasBandeirola;

        //transfere as retas e arcos para vetores vis�veis somente nesta fun��o
		for (m = 0; m < ListaItens._multipoints.size(); m++)
        {
			vector<TPonto> &points = ListaItens._multipoints[m]->pontos;
            int NumPontos = points.size();
            int IndiceDoUltimoPonto = NumPontos - 1;
            // Pega o primeiro e o último ponto do multipoint e p�e na lista de extremidades.
            PontosExtremidadesElementosBandeirola.push_back (points[0] );
            PontosExtremidadesElementosBandeirola.push_back( points[IndiceDoUltimoPonto] );
            for (int k = 0; k < (NumPontos - 1); k++)
            {
                TReta reta;
                reta.pontos[0] = points[k];
                reta.pontos[1] = points[k + 1];
                RetasBandeirola.push_back( reta );
            }
        }

		
		for (m = 0; m < ListaItens._arcs.size(); m++)
		{
            TPonto pontas[2];
            ListaItens._arcs[m]->PontasArco( pontas );
            // Pega as pontas do arco e p�e na lista de extremidades
            PontosExtremidadesElementosBandeirola.push_back( pontas[0] );
            PontosExtremidadesElementosBandeirola.push_back( pontas[1] );
            NumArcosBandeirola++;
		}

        //VAI PRECISAR DEPOIS
        //compara cada ponto com cada ponto para verificar os que estáo juntos e os apaga
        for (int i = 0; i < (int) PontosExtremidadesElementosBandeirola.size(); i++)
        {
            for (int j = i + 1; j < (int) PontosExtremidadesElementosBandeirola.size(); j++)
            {
                double distancia = DistPontos( PontosExtremidadesElementosBandeirola.at( i ),
                        PontosExtremidadesElementosBandeirola.at( j ) );
                double fat = FATOR_DIST_MIN_ELEM_CABO * 6;
                if (distancia < fat)
                {
                    // Se tem dois pontos ligados dentro da bandeirola, então, apaga os dois. Pois isso significa que eles são o mesmo ponto.
                    PontosExtremidadesElementosBandeirola.erase( PontosExtremidadesElementosBandeirola.begin() + j );
                    PontosExtremidadesElementosBandeirola.erase( PontosExtremidadesElementosBandeirola.begin() + i );
                    i--;
                    break;
                }
            }
        }

        //se a bandeirola não tem pontos soltos, avisa
        if (!PontosExtremidadesElementosBandeirola.size())
        {
            CErrosMsg *erros = CErrosMsg::getInstance();
            erros->novoErro(
                    (string) "Atenção: há algo em nível de bandeirola fora do formato esperado." );

            continue;
        }

        if (ListaItens._texts.size() == 1) //se a bandeirola possui um e somente um texto, gera o vértice
        {
			VerticeGeral->texto = ListaItens._texts[0]->texto;
            TPonto MaisDist;
            double DistMaisProx;
            bool Bandeirola2Retas = false;

            if ((RetasBandeirola.size() == 2) && (NumArcosBandeirola == 0)) //verifica se a bandeirola possui uma reta horizontal e outra inclinada
            {
                double Inclinacao, MenorInclinacao, DifX, DifY;
                int iMaisInclinado, iMenosInclinado;
                MenorInclinacao = Infinity;
                //percorre as 2 retas e verifica qual a que possui menor inclina��o
                for (int k = 0; k < 2; k++)
                {
                    DifX = RetasBandeirola.at( k ).pontos[0].x
                            - RetasBandeirola.at( k ).pontos[1].x;
                    DifY = RetasBandeirola.at( k ).pontos[0].y
                            - RetasBandeirola.at( k ).pontos[1].y;
                    if (fabs( DifX ) > 0.00001)
                        Inclinacao = fabs( DifY / DifX );
                    else
                        Inclinacao = Infinity;
                    if (Inclinacao < MenorInclinacao)
                    {
                        MenorInclinacao = Inclinacao;
                        iMenosInclinado = k;
                        //se o menos inclinado for 1, o mais inclinado vai ser 0, e vice-versa
                        iMaisInclinado = (k + 1) % 2;
                    }
                }
                if (MenorInclinacao < 0.0001)    //pelo menos uma das retas � horizontal
                {
                    int iPontoMaisLonge;
                    double DifY, MaiorDifY = 0;
                    //percorre os pontos da reta que não � a horizontal e verifica qual fica mais longe da reta horizontal
                    for (int k = 0; k < 2; k++)
                    {
                        DifY = fabs(
                                RetasBandeirola.at( iMaisInclinado ).pontos[k].y
                                        - RetasBandeirola.at( iMenosInclinado ).pontos[0].y );
                        if (DifY > MaiorDifY)
                        {
                            MaiorDifY = DifY;
                            iPontoMaisLonge = k;
                        }
                    }
                    MaisDist = RetasBandeirola.at( iMaisInclinado ).pontos[iPontoMaisLonge];
                    Bandeirola2Retas = true;
                }
            }
            if (!Bandeirola2Retas)
            {
				shared_ptr<TTexto> text = ListaItens._texts[0];
                TPonto origemTexto = text->origem;
                origemTexto.x += (text->FatorAltura * FATOR_FONTE * text->texto.size()) * cos(text->rotacao ) / 2;
                origemTexto.x += text->FatorAltura * FATOR_FONTE * cos( text->rotacao * M_PI / 2 ) / 2;
                origemTexto.y += (text->FatorAltura * FATOR_FONTE * text->texto.size()) * sin( text->rotacao ) / 2;
                origemTexto.y += text->FatorAltura *
                FATOR_FONTE * sin( text->rotacao * M_PI / 2 ) / 2;
                //verifica qual o ponto livre da bandeirola mais longe do centro do texto
                PontoMaisDistante( origemTexto, PontosExtremidadesElementosBandeirola, MaisDist );
            }
			{
				shared_ptr<CCaboReta> straightCableMinDist;
				//verifica qual o cabo mais próximo da ponta da bandeirola
				CaboMaisProximo( MaisDist, straightCableMinDist, DistMaisProx, PontoNaReta, -1, -1 );
				if ( straightCableMinDist )
				{
					VerticeGeral->pos = PontoNaReta;
					straightCableMinDist->AdicionaVertice( VerticeGeral );

					TPontosBandeirola pontoTemp;
					pontoTemp.NaBandeirola = MaisDist;
					pontoTemp.NoCabo = PontoNaReta;

					_pontosPraMostrarBandeirola.push_back( pontoTemp );
				}
				else //avisa caso o desenhos não possuam nenhum cabo
				{
					CErrosMsg *erros = CErrosMsg::getInstance();
					erros->novoErro( "Atenção: " +  _dados->NomeArq + " não possui cabos." );
				}
			}
        }
		else if (ListaItens._texts.size() > 1) //avisa caso a bandeirola tenha mais de um texto
        {
            CErrosMsg *erros = CErrosMsg::getInstance();
            erros->novoErro("No desenho " + _dados->NomeArq
                            + " existe um grupamento em nível de bandeirola com mais de um texto associado. Textos: " );
            for (int i = 0; i < ListaItens._texts.size(); i++)
                erros->novoErro( ListaItens._texts[i]->texto );
            erros->novoErro( "" );
        }
        else if (ListaItens._texts.size() == 0) //avisa caso exista bandeirola sem texto nenhum
        {
            CErrosMsg *erros = CErrosMsg::getInstance();
            erros->novoErro(
                    (string) "No desenho " + _dados->NomeArq
                            + " existe um grupamento em nível de bandeirola sem textos associados.\n" );
        }

        VerticeGeral->TipoElemento = BANDEIROLA;
		VerticeGeral->drawing = _dados->_drawing;
        _graph->_verticesGerais->Adiciona( VerticeGeral );
    }
}
//---------------------------------------------------------------------------

void CGrafoDesenho::GeraVerticesInstrumentosAdicionaMultipointCaboReta(
        TListaItensCelula *ListaItensCelula, TVectorPontoEIndiceCabo &ListaMenores, bool &ligado,
        std::vector< shared_ptr<TVerticeGeral> > &VerticesInstrumento, TPonto PosVertice)
{
//percorre lista de cabos ligados ao equipamento
    for (int n = 0; n < (int) ListaMenores.size(); n++)
    {
		TPontoEIndiceCabo &pontoEIndiceCabo = ListaMenores[ n ];
        bool isAdded = ListaItensCelula->VerificaSeCaboRetaJaFoiLigadoAoEquipamento( pontoEIndiceCabo.IndiceCabo );

        if (!isAdded)
        {
            shared_ptr<TVerticeGeral> cableVertex = make_shared<TVerticeGeral>();
			shared_ptr<CCaboReta> straightCable = _cabosReta[pontoEIndiceCabo.IndiceCabo];

            cableVertex->pos = pontoEIndiceCabo.PosVertice;
            cableVertex->TipoElemento = INSTRUMENTO;
			cableVertex->drawing = _dados->_drawing;
            cableVertex->TipoVertice = VERTICE_INSTRUMENTO_RETA;
            _graph->_verticesGerais->Adiciona( cableVertex );
            //adiciona o vértice na lista de vértices do cabo
            straightCable->AdicionaVertice( cableVertex );
			
            ListaItensCelula->cabosRetaRelacionados.push_back( pontoEIndiceCabo.IndiceCabo );

            if (straightCable->EhOPrimeiroPonto( pontoEIndiceCabo.PosVertice  ))
            {
                straightCable->ponta[0] = true;
            }
            else if (straightCable->EhOUltimoPonto( pontoEIndiceCabo.PosVertice ))
            {
                straightCable->ponta[1] = true;
            }

            for (int i = 0; i < VerticesInstrumento.size(); i++)
            {
				shared_ptr<TAresta> Aresta = make_shared<TAresta>( straightCable->_multipoint->layerName );
                Aresta->AdicionaVertices( VerticesInstrumento[ i ], cableVertex, DistPontosManhattan( PosVertice, PosVertice ) );
                Aresta->_drawing = _dados->_drawing;
				_graph->_arestas.push_back( Aresta );
            }
        }
    }
}
//---------------------------------------------------------------------------

void CGrafoDesenho::GeraVerticesInstrumentosAdicionaMultipointCaboArco(
        TListaItensCelula *ListaItensCelula, TVectorPontoEIndiceCabo &ListaMenores, bool &ligado,
        std::vector< shared_ptr<TVerticeGeral> > &VerticesInstrumento, TPonto PosVertice)
{
    for (int n = 0; n < (int) ListaMenores.size(); n++)
    {
        bool isAdded;
        isAdded = ListaItensCelula->VerificaSeCaboArcoJaFoiLigadoAoEquipamento(
                ListaMenores.at( n ).IndiceCabo );

        if (!isAdded)
        {
			int cableIndex = ListaMenores.at( n ).IndiceCabo;
            //if ( ListaItensCelula->iTexto > 0 )
            //  VerticesGerais->getItem(IndiceVertice)->texto = Textos[ListaItensCelula->iTexto].texto;
            ListaItensCelula->cabosArcoRelacionados.push_back( cableIndex );
			shared_ptr<TAresta> Aresta = make_shared<TAresta>( _cabosArco[cableIndex]->_arco->layerName );

            _cabosArco[cableIndex]->ponta[ListaMenores[n].IndiceArco] =
                    true;
            for (int i = 0; i < ListaItensCelula->_texts.size(); i++)
            {
                Aresta->AdicionaVertices( VerticesInstrumento[ i ],
                        ListaMenores[ n ]._vertex,
                        DistPontosManhattan( PosVertice, PosVertice ) );
				Aresta->_drawing = _dados->_drawing;
				_graph->_arestas.push_back( Aresta );
            }
        }
    }
}
//---------------------------------------------------------------------------

void CGrafoDesenho::GeraVerticesInstrumentosAdicionaMultipoint( shared_ptr<TMultipoint> multipoint,
        TListaItensCelula *ListaItensCelula, bool &ligado, std::vector< shared_ptr<TVerticeGeral> > &VerticesInstrumento, TPonto PosVertice)
{
    TVectorPontoEIndiceCabo ListaMenores;
    int j;
    TPonto reta[2];
    for (j = 0; j < multipoint->pontos.size(); j++)
    {
        reta[0] = multipoint->pontos[j];
        if (j + 1 != multipoint->pontos.size())
            reta[1] = multipoint->pontos[j + 1];
        else
            reta[1] = multipoint->pontos[0];

        ListaMenores.clear();
        //retorna a lista de cabos que possuem Distância pequena para o equipamento
        DistRetaParaTodasPontasCaboReta( reta, ListaMenores, DIST_MIN_ELEM_CABO );
        GeraVerticesInstrumentosAdicionaMultipointCaboReta( ListaItensCelula, ListaMenores, ligado, VerticesInstrumento, PosVertice );

        ListaMenores.clear();
        //localiza o cabo arco com a ponta mais pr�xima � reta
        DistRetaParaTodasPontasCaboArco( reta, ListaMenores, DIST_MIN_ELEM_CABO );
        //se a Distância for pequena, considera-se que estáo ligados
        GeraVerticesInstrumentosAdicionaMultipointCaboArco( ListaItensCelula, ListaMenores, ligado, VerticesInstrumento, PosVertice );
    }
}
//---------------------------------------------------------------------------

void CGrafoDesenho::GeraVerticesInstrumentosAdicionaArco( shared_ptr<TArco> arc,
        TListaItensCelula *ListaItensCelula, bool &ligado, std::vector< shared_ptr<TVerticeGeral> > &VerticesInstrumento, TPonto PosVertice)
{
    TVectorPontoEIndiceCabo ListaMenores;
    int IndiceCabo;
    ListaMenores.clear();
    DistArcoParaTodasPontasRetaCabo( *arc, ListaMenores, DIST_MIN_ELEM_CABO );

    for (int n = 0; n < (int) ListaMenores.size(); n++)
    {
        IndiceCabo = ListaMenores[n].IndiceCabo;
//    DistMaisPerto = ListaMenores[n].Dist;
        PosVertice = ListaMenores[n].PosVertice;

        //verifica se o cabo já não faz parte da c�lula (?)
        bool isAdded;
        isAdded = ListaItensCelula->VerificaSeCaboRetaJaFoiLigadoAoEquipamento( IndiceCabo );

        if (!isAdded)
        {
            shared_ptr<TVerticeGeral> VerticeGeral = make_shared<TVerticeGeral>();
            //adiciona o vértice na lista de vértices do cabo
            _cabosReta[IndiceCabo]->AdicionaVertice( VerticeGeral );

            VerticeGeral->pos = PosVertice;
			VerticeGeral->drawing = _dados->_drawing;
            VerticeGeral->TipoElemento = INSTRUMENTO;
            VerticeGeral->TipoVertice = VERTICE_INSTRUMENTO_ARCO;
            _graph->_verticesGerais->Adiciona( VerticeGeral );

            ListaItensCelula->cabosRetaRelacionados.push_back( IndiceCabo );
            if (_cabosReta[IndiceCabo]->EhOPrimeiroPonto( PosVertice ))
            {
                _cabosReta[IndiceCabo]->ponta[0] = true;
            }
            else if (_cabosReta[IndiceCabo]->EhOUltimoPonto( PosVertice ))
            {
                _cabosReta[IndiceCabo]->ponta[1] = true;
            }
			shared_ptr<TAresta> Aresta = make_shared<TAresta>( _cabosReta[IndiceCabo]->_multipoint->layerName );
            for (int i = 0; i < ListaItensCelula->_texts.size(); i++)
            {
                Aresta->AdicionaVertices( VerticesInstrumento[ i ], VerticeGeral, DistPontos( PosVertice, PosVertice ) );
				Aresta->_drawing = _dados->_drawing;
				_graph->_arestas.push_back( Aresta );
            }
        }
    }
}
//---------------------------------------------------------------------------

//Preenche o vetor que armazena os vértices de cada instrumento
void CGrafoDesenho::GeraVerticesInstrumentos()
{
    int n, i;

    TPonto PosVertice;
    //percorre c�lulas de instrumentos
    for (n = 0; n < _dados->InfoCelula.ListaCelulasInstrumentos.size(); n++) //percorre todas as c�lulas do desenho
    {
        bool ligado = false;
		TListaItensCelula *ListaItensCelula = &(_dados->InfoCelula.ListaCelulasInstrumentos[ n ]);
 
		vector< shared_ptr<TVerticeGeral> > VerticesInstrumento;
        if (ListaItensCelula->_texts.size() == 0)
        {
            CErrosMsg *erros = CErrosMsg::getInstance();
            erros->novoErro("No desenho " + _dados->NomeArq + " existe um grupamento em nível de equipamento sem textos associados.\n" );
            continue;
        }

        PosVertice = AchaPosVerticeInstrumento( ListaItensCelula );
        CriaVerticesEArestasInstrumento( ListaItensCelula, VerticesInstrumento, PosVertice );
		createColarEdges( ListaItensCelula, VerticesInstrumento );

		for (i = 0; i < ListaItensCelula->_multipoints.size(); i++)  //percorre todos os itens da c�lula atual
			GeraVerticesInstrumentosAdicionaMultipoint( ListaItensCelula->_multipoints[i], ListaItensCelula, ligado, VerticesInstrumento, PosVertice );

		for (i = 0; i < ListaItensCelula->_arcs.size(); i++)  //percorre todos os itens da c�lula atual
			GeraVerticesInstrumentosAdicionaArco( ListaItensCelula->_arcs[i], ListaItensCelula, ligado, VerticesInstrumento, PosVertice );
    }
}
//---------------------------------------------------------------------------

void CGrafoDesenho::CriaVerticesEArestasInstrumento(TListaItensCelula *ListaItensCelula, std::vector< shared_ptr<TVerticeGeral> > &VerticesInstrumento, TPonto PosVertice )
{
    //para cada texto da c�lula cria um vértice
    for (int i = 0; i < ListaItensCelula->_texts.size(); i++)
    {
        shared_ptr<TVerticeGeral> VerticeInstrumento = make_shared<TVerticeGeral>();
        VerticeInstrumento->TipoElemento = INSTRUMENTO;
		VerticeInstrumento->drawing = _dados->_drawing;
        VerticeInstrumento->texto = ListaItensCelula->_texts[i]->texto;
        VerticeInstrumento->pos = PosVertice;
        VerticesInstrumento.push_back( VerticeInstrumento );
        VerticeInstrumento->TipoVertice = VERTICE_CENTRO_INSTRUMENTO;
        _graph->_verticesGerais->Adiciona( VerticeInstrumento );
    }

}
//---------------------------------------------------------------------------


void CGrafoDesenho::createColarEdges( TListaItensCelula *ListaItensCelula, vector< shared_ptr<TVerticeGeral> > &VerticesInstrumento )
{
    // Caso o equipamento seja um colar de subida/descida, então é necessário adicionar uma aresta entre a subida e a descida.
    if (ListaItensCelula->_texts.size() == 2)
    {
        shared_ptr<TAresta> Aresta = make_shared<TAresta>( "" );
		// the 0.1 is used for this edge not to be the preferred one sometimes
        Aresta->AdicionaVertices( VerticesInstrumento[0], VerticesInstrumento[1], 0.1 );
        Aresta->_drawing = _dados->_drawing;
		_graph->_arestas.push_back( Aresta );
    }
}


TPonto CGrafoDesenho::AchaPosVerticeInstrumento(TListaItensCelula *ListaItensCelula)
{
    int i, j;
    TPonto PosVertice;
    bool first;
    int maiorX, menorX, maiorY, menorY;
	menorX = menorY = 100000000;
	maiorX = maiorY = -1;
    first = true;
    //percorre itens da c�lula
	for (i = 0; i < ListaItensCelula->_multipoints.size(); i++)
    {
		vector<TPonto> &points = ListaItensCelula->_multipoints[i]->pontos;
        for (j = 0; j < points.size(); j++)
        {
            if ( points[j].x > maiorX)
                maiorX = points[j].x;
            if (points[j].x < menorX)
                menorX = points[j].x;
            if (points[j].y > maiorY)
                maiorY = points[j].y;
            if (points[j].y < menorY)
                menorY = points[j].y;
        }
	}
	for (i = 0; i < ListaItensCelula->_arcs.size(); i++)
	{
		shared_ptr<TArco> arc = ListaItensCelula->_arcs[i];
        TPonto Centro;
        double Raio;
        Centro = arc->Centro;
        Raio = arc->EixoPrimario;
        if (Centro.x + Raio > maiorX)
            maiorX = Centro.x + Raio;
        if (Centro.x + Raio > menorX)
            menorX = Centro.x - Raio;
        if (Centro.y + Raio > maiorY)
            maiorY = Centro.y + Raio;
        if (Centro.y - Raio > menorY)
            menorY = Centro.y - Raio;
    }     
    PosVertice.x = (maiorX + menorX) / 2;
    PosVertice.y = (maiorY + menorY) / 2;

    return PosVertice;
}
//---------------------------------------------------------------------------

//Arcos que são cabos
void CGrafoDesenho::GeraVerticesArcos()
{
    int n, m;
    double DistMaisProx;
    TPonto p[2], PontoNaReta, PontoTemp;
    shared_ptr<TArco> Arco;
    int PontaArco;
    n = m = 0;
    DistMaisProx = 0;
    PontoNaReta.x = PontoNaReta.y = PontoTemp.x = PontoTemp.y = p[0].x = p[0].y = p[1].x = p[1].y =
            0.0;
    for (n = 0; n < _cabosArco.size(); n++)
    {
        /* Olha todos os arcos do desenho */
        Arco = _cabosArco[n]->_arco;
        Arco->PontasArco( p );

        for (m = 0; m < 2; m++)
        {
			shared_ptr<TVerticeGeral> VerticeGeral = make_shared<TVerticeGeral>();
			VerticeGeral->TipoVertice = VERTICE_ARCO;
            // iV[m] � o índice do novo vértice que será criado, e como ele será o último vértice,
            // o seu índice � igual ao total atual de vértices
            Arco->_vertices[m] = VerticeGeral;

            //verifica se a ponta deste cabo arco está ligada na ponta de outro cabo arco
            //se já estiver, não cria um vértice novo, aproveita o vértice que já existe
            //no outro cabo arco
			shared_ptr<TVerticeGeral> vertex;
			shared_ptr<CCaboArco> arcCable;
            DistPontoParaPontaCaboArco( p[m], arcCable, DistMaisProx, PontoTemp, vertex, n, PontaArco );
            if (DistMaisProx < DIST_MIN_ELEM_CABO)
            {
				Arco->_vertices[m] = vertex;
                _cabosArco[n]->ponta[m] = true;
                arcCable->ponta[PontaArco] = true;

            }
            else //se o vértice não foi aproveitado de outro cabo arco, verifica se este vértice está próximo a um cabo reta
            {
				shared_ptr<CCaboReta> straightCableMinDist;
                /* Olha cada ponta do arco */
                CaboMaisProximo( p[m], straightCableMinDist, DistMaisProx, PontoNaReta, -1, Arco->Nivel );
                if (DistMaisProx < DIST_MIN_ELEM_CABO)
                {
                    // Se a Distância for menor do que o limite, então esse vértice também será adicionado ao cabo mais próximo
					straightCableMinDist->AdicionaVertice( Arco->_vertices[m] );
                    VerticeGeral->pos = PontoNaReta;
                    _cabosArco[n]->ponta[m] = true;
                    if (straightCableMinDist->EhOPrimeiroPonto( PontoNaReta ))
                    {
                        straightCableMinDist->ponta[0] = true;
                    }
                    else if (straightCableMinDist->EhOUltimoPonto( PontoNaReta ))
                    {
                        straightCableMinDist->ponta[1] = true;
                    }
                }
                else
                    VerticeGeral->pos = p[m];

				VerticeGeral->drawing = _dados->_drawing;
                _graph->_verticesGerais->Adiciona( VerticeGeral );
            }
        }

		shared_ptr<TAresta> Aresta = make_shared<TAresta>( Arco->layerName );
		Aresta->AdicionaVertices( Arco->_vertices[0], Arco->_vertices[1], DistPontosManhattan( p[0], p[1] ) );
        Aresta->_drawing = _dados->_drawing;
		_graph->_arestas.push_back( Aresta );
    }
}
//---------------------------------------------------------------------------

void CGrafoDesenho::GeraVerticesPontaCabos()
{
    int n, m;
    double DistMaisProx;
    TPonto PontoNaReta;
    shared_ptr<TMultipoint> tMultipoint;
	for (n = 0; n < _cabosReta.size(); n++)
    {
        /* Olha todos os cabos reta do desenho */
		tMultipoint = _cabosReta[n]->_multipoint;

        for (m = 0; m < tMultipoint->pontos.size(); m++)
        {
			shared_ptr<TVerticeGeral> VerticeGeral = make_shared<TVerticeGeral>();
			VerticeGeral->TipoVertice = VERTICE_PONTA_CABO;
            if (m != 0 && m != tMultipoint->pontos.size() - 1)
                continue;
            /* Olha cada ponta da reta */
			shared_ptr<CCaboReta> straightCableMinDist;
            CaboMaisProximo( tMultipoint->pontos[m], straightCableMinDist, DistMaisProx, PontoNaReta, n,
                    tMultipoint->Nivel );
            if (DistMaisProx < DIST_MIN_ELEM_CABO)
            {
                // Se a Distância for menor do que o limite, então esse vértice também será adicionado ao cabo mais próximo
                VerticeGeral->pos = PontoNaReta;
                straightCableMinDist->AdicionaVertice( VerticeGeral );
                if (m == 0)
                    _cabosReta[n]->ponta[0] = true;
                else
                    _cabosReta[n]->ponta[1] = true;
            }
            else
                VerticeGeral->pos = tMultipoint->pontos[m];
            _cabosReta[n]->AdicionaVertice( VerticeGeral );
			VerticeGeral->drawing = _dados->_drawing;
            _graph->_verticesGerais->Adiciona( VerticeGeral );
        }
    }
}
//---------------------------------------------------------------------------


void CGrafoDesenho::DistRetaParaTodasPontasCaboReta(TPonto Reta[2], TVectorPontoEIndiceCabo &ListaMenores, double DistMinElemCabo) const
{
    int m, n;
    unsigned char ContadorMaior;
    double Dist;
    TPonto PontoTemp;
    TPontoEIndiceCabo Cabo;
	for (m = 0; m < _cabosReta.size(); m++)
    {
        // PontosCabo tem os pontos de in�cio e fim do cabo
		vector<TPonto> &PontosCabo = _cabosReta[m]->_multipoint->pontos;
        for (n = 0; n < 2; n++)
        {
            Dist = DistPontoParaSegmentoReta( Reta, PontosCabo[n], PontoTemp );
            if (Dist < DistMinElemCabo)
            {
                //IndiceCabo=m;
                Cabo.IndiceCabo = m;
                Cabo.PosVertice = PontosCabo[n];
                ListaMenores.push_back( Cabo );
                //PosVertice=PontosCabo[n];
            }
        }        //for (n=0; n<2; n++)
    }        //for (m=0; m<NumCabosReta; m++)
}
//---------------------------------------------------------------------------


void CGrafoDesenho::DistRetaParaTodasPontasCaboArco(TPonto Reta[2], TVectorPontoEIndiceCabo &ListaMenores, double DistMinElemCabo) const
{
    int m, n;
    unsigned char ContadorMaior;
    double Dist;
    TPonto pontoCabo, PontoTemp;
    TPontoEIndiceCabo Cabo;
    for (m = 0; m < _cabosArco.size(); m++)
    {
        for (n = 0; n < 2; n++)
        {
			pontoCabo = _cabosArco[m]->_arco->_vertices[n]->pos;
            Dist = DistPontoParaSegmentoReta( Reta, pontoCabo, PontoTemp );
            if (Dist < DistMinElemCabo)
            {
                Cabo.IndiceCabo = m;
                Cabo.PosVertice = pontoCabo;
				Cabo._vertex = _cabosArco[m]->_arco->_vertices[n];
                Cabo.IndiceArco = n;
                ListaMenores.push_back( Cabo );
            }
        }        //for (n=0; n<2; n++)
    }        //for (m=0; m<NumCabosReta; m++)
}
//---------------------------------------------------------------------------

void CGrafoDesenho::DistArcoParaTodasPontasRetaCabo(TArco &Arco, TVectorPontoEIndiceCabo &ListaMenores, double DistMinElemCabo) const
{
    int m, n;
    unsigned char ContadorMaior;
    double Dist;
    TPonto *PontosCabo;

    TPontoEIndiceCabo Cabo;
	for (m = 0; m < _cabosReta.size(); m++)
    {
		vector<TPonto> &PontosCabo = _cabosReta[m]->_multipoint->pontos;
        for (n = 0; n < 2; n++)
        {
            //eixo prim�rio em um c�rculo � o raio
            Dist = fabs(
                    sqrt(
                            pow( Arco.Centro.x - PontosCabo[n].x, 2 )
                                    + pow( Arco.Centro.y - PontosCabo[n].y, 2 ) )
                            - Arco.EixoPrimario );
            if (Dist < DistMinElemCabo)
            {
                Cabo.IndiceCabo = m;
                Cabo.PosVertice = PontosCabo[n];
                Cabo.Dist = Dist;
                ListaMenores.push_back( Cabo );
            }
        }  //for (n=0; n<2; n++)
    }  //for (m=0; m<NumCabosReta; m++)
}
//---------------------------------------------------------------------------

//ESTA FUN��O DEVE SE ADAPTAR PARA TRABALHAR COM ARCOS QUE não são C�RCULOS
void CGrafoDesenho::DistArcoParaPontaRetaCabo(TArco &Arco, int &IndiceCabo, double &DistMaisPerto, TPonto &PosVertice) const
{
    int m, n;
    unsigned char ContadorMaior;
    double MenorDist, Dist;
    IndiceCabo = -1;
    MenorDist = Infinity;
	for (m = 0; m < _cabosReta.size(); m++)
    {
        vector<TPonto> &PontosCabo = _cabosReta[m]->_multipoint->pontos;
        for (n = 0; n < 2; n++)
        {
            //eixo prim�rio em um c�rculo � o raio
            Dist = fabs(
                    sqrt(
                            pow( Arco.Centro.x - PontosCabo[n].x, 2 )
                                    + pow( Arco.Centro.y - PontosCabo[n].y, 2 ) )
                            - Arco.EixoPrimario );
            if (Dist < MenorDist)
            {
                MenorDist = Dist;
                IndiceCabo = m;
                PosVertice = PontosCabo[n];
            }
        }  //for (n=0; n<2; n++)
    }  //for (m=0; m<NumCabosReta; m++)
    DistMaisPerto = MenorDist;
}
//---------------------------------------------------------------------------

//ESTA FUN��O DEVE SE ADAPTAR PARA TRABALHAR COM ARCOS QUE não são C�RCULOS
void CGrafoDesenho::DistArcoParaPontaArcoCabo(TArco &Arco, int &IndiceArcoCabo, double &DistMaisPerto, TPonto &PosVertice) const
{
    int m, n;
    unsigned char ContadorMaior;
    double MenorDist, Dist;
    TPonto PontosCabo[2];
    IndiceArcoCabo = -1;
    MenorDist = Infinity;
    for (m = 0; m < _cabosArco.size(); m++)
    {
        _cabosArco[m]->_arco->PontasArco( PontosCabo );
        for (n = 0; n < 2; n++)
        {
            //eixo prim�rio em um c�rculo � o raio
            Dist = fabs( sqrt( pow( Arco.Centro.x - PontosCabo[n].x, 2 ) + pow( Arco.Centro.y - PontosCabo[n].y, 2 ) ) - Arco.EixoPrimario );
            if (Dist < MenorDist)
            {
                MenorDist = Dist;
                IndiceArcoCabo = m;
                PosVertice = PontosCabo[n];
            }
        }  //for (n=0; n<2; n++)
    }  //for (m=0; m<NumCabosReta; m++)
    DistMaisPerto = MenorDist;
}
//---------------------------------------------------------------------------

void CGrafoDesenho::DistPontoParaPontaCaboArco(TPonto ponto, shared_ptr<CCaboArco> &arcCable, double &DistMaisPerto, TPonto &PosVertice, 
	  shared_ptr<TVerticeGeral> &vertex, int IndiceMax, int &pontaCabo) const
{
    int m, n;
    unsigned char ContadorMaior;
    double MenorDist, Dist;
    TPonto pontoCabo, PontoTemp;
    vertex = shared_ptr<TVerticeGeral>( 0 );
    MenorDist = Infinity;
    int menor = IndiceMax;
    if (menor > _cabosArco.size())
        menor = _cabosArco.size();
    for (m = 0; m < menor; m++)
    {
        for (n = 0; n < 2; n++)
        {
			pontoCabo = _cabosArco[m]->_arco->_vertices[n]->pos;
            Dist = DistPontos( ponto, pontoCabo );
            if (Dist < MenorDist)
            {
                MenorDist = Dist;
                arcCable = _cabosArco[m];
                PosVertice = pontoCabo;
                pontaCabo = n;
				vertex = _cabosArco[m]->_arco->_vertices[n];
            }
        }  //for (n=0; n<2; n++)
    }  //for (m=0; m<NumCabosReta; m++)
    DistMaisPerto = MenorDist;
}
//---------------------------------------------------------------------------

void CGrafoDesenho::OrdenaVerticesRetas()
{
    int n;
	for (n = 0; n < _cabosReta.size(); n++)
    {
        if (_cabosReta[n]->TipoOrientacao == VERTICAL)
        {
            sort(_cabosReta[n]->VerticesReta.begin(), _cabosReta[n]->VerticesReta.end(), OrdenaRetaVertical );
        }
        else
        {
            sort(_cabosReta[n]->VerticesReta.begin(), _cabosReta[n]->VerticesReta.end(), OrdenaRetaHorizontal );
        }
    }
}
//---------------------------------------------------------------------------



void CGrafoDesenho::ChecagemVerticeDuplo(const std::vector< shared_ptr<TDesenho> > &ListaDesenhos)
{
    int n;
    vector< shared_ptr<TVerticeGeral> > Lista;
    _graph->_verticesGerais->ListaOrd( Lista );  //gera lista ordenada
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
//---------------------------------------------------------------------------

void CGrafoDesenho::GeraArestas()
{
    int n, m;
    shared_ptr<TVerticeGeral> VerticesReta1, VerticesReta2;
	for (n = 0; n < _cabosReta.size(); n++)
    {
		shared_ptr<CCaboReta> caboReta = _cabosReta[n];
		for (m = 0; m < caboReta->VerticesReta.size() - 1; m++)
        {
            VerticesReta1 = caboReta->VerticesReta[m];
            VerticesReta2 = caboReta->VerticesReta[m + 1];
			shared_ptr<TAresta> Aresta = make_shared<TAresta>( caboReta->_multipoint->layerName );
			Aresta->AdicionaVertices( VerticesReta1, VerticesReta2,
            //					DistPontosManhattan(VerticesReta1->pos, VerticesReta2->pos));
                    DistPontos( VerticesReta1->pos, VerticesReta2->pos ) );
			Aresta->_drawing = _dados->_drawing;
			_graph->_arestas.push_back( Aresta );
        }
    }
}
//---------------------------------------------------------------------------

