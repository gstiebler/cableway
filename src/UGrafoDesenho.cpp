//---------------------------------------------------------------------------
#pragma hdrstop
#include "UGrafoDesenho.h"
#include "UErros.h"
#include "UGeometria.h"
#include "UContainerDesenhos.h"
#include "UListaItensCelula.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

CCaboReta::CCaboReta()
{
    NumVertices = 0;
    ponta[0] = ponta[1] = false;
}
//---------------------------------------------------------------------------

CCaboReta::~CCaboReta()
{
}
//---------------------------------------------------------------------------

void CCaboReta::AdicionaVertice(int ID, TPonto &ponto)
{
    TVerticeReta VerticeReta;
    VerticeReta.ID = ID;
    VerticeReta.pos = ponto;
	VerticesReta.push_back( VerticeReta );
    NumVertices++;
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

CGrafoDesenho::CGrafoDesenho(TParamsGrafoDesenho &ParamsGrafoDesenho, std::shared_ptr<CDadosGenerico> Dados)
{
    _pri = 0;
    CarregaGrafo = ParamsGrafoDesenho.CarregaGrafo;

    // Cria um DadosGenerico para ser usado
    this->_dados = Dados;

    if (CarregaGrafo)
    {
        unsigned int n;
        memset( TipoElementoCor, 0, NUM_CORES * sizeof(TTipoElemento) );
        _verticesGerais = ParamsGrafoDesenho.VerticesGerais;

        //O vértice 0 não pode ser usado, por isso adiciona-se este vértice vazio
        TVerticeGeral temp;
        _verticesGerais->Adiciona( temp );

        AlocaElementos();
        GeraListaCabos();
        GeraVerticesBandeirola();
        GeraVerticesArcos();
        GeraVerticesPontaCabos();
        GeraVerticesInstrumentos();
        OrdenaVerticesRetas();
        GeraArestas();
    }
    _ult = Dados->Multipoint.size();
}
//---------------------------------------------------------------------------

CGrafoDesenho::~CGrafoDesenho()
{
}
//---------------------------------------------------------------------------

void CGrafoDesenho::AlocaElementos()
{
    _cabosReta.reserve( _dados->Multipoint.size() );
    _cabosArco.reserve( _dados->Arcos.size() );
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
			_cabosReta.push_back( CCaboReta() );
			_cabosReta.back()._multipoint = _dados->Multipoint[n];
            Pontos = _dados->Multipoint[n]->pontos;
            DifX = fabs( Pontos[0].x - Pontos[1].x );
            DifY = fabs( Pontos[0].y - Pontos[1].y );
            _cabosReta.back().TipoOrientacao = (DifY > DifX) ? VERTICAL : HORIZONTAL;
        }
    }

    double SomaRaios = 0;
    for (n = 0; n < _dados->Arcos.size(); n++)
    {
		if ( _dados->Arcos[n]->Nivel == CABO )
        {
			_cabosArco.push_back( CCaboArco() );
			_cabosArco.back()._arco = _dados->Arcos[n];

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
void CGrafoDesenho::CaboMaisProximo(TPonto &ponto, int &IndiceCabo, double &DistMaisProx,
        TPonto &PosVertice, int Diferente, int Nivel)
{
    int m;
    double MenorDist, Dist;
    IndiceCabo = -1;
    MenorDist = Infinity;
	for (m = 0; m < _cabosReta.size(); m++)
    {
		shared_ptr<TMultipoint> multipoint = _cabosReta[m]._multipoint;
        if ((Nivel != -1) && (multipoint->Nivel != Nivel))
            continue;
		vector<TPonto> &pontosCabo = multipoint->pontos;
        TPonto volta;
        Dist = DistPontoParaSegmentoReta( &pontosCabo[0], ponto, volta );
        if ((Dist < MenorDist) && (m != Diferente))
        {
            MenorDist = Dist;
            IndiceCabo = m;
            PosVertice = volta;
        }
    }  //for (m=0; m<NumCabosReta; m++)
    DistMaisProx = MenorDist;
    if (IndiceCabo == -1)
    {
        CErrosMsg *erros = CErrosMsg::getInstance();
        erros->novoErro(
                (string) "Atenção: " + ExtractFileName( _dados->NomeArq.c_str() ).c_str()
                        + " não possui cabos." );
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
    int n, m, iMenorDist;
    TPonto PontoBandeirola[2], PontoNaReta;
    vector<TListaItensCelula> &ListaBandeirolas = _dados->InfoCelula.ListaCelulasBandeirolas;
    TListaItensCelula *ListaItens;

    //percorre todas as bandeirolas
    for (n = 0; n < ListaBandeirolas.size(); n++)
    {
        int NumArcosBandeirola = 0;
        ListaItens = new TListaItensCelula( ListaBandeirolas[ n ] );
        TVerticeGeral VerticeGeral;
        VerticeGeral.TipoVertice = VERTICE_BANDEIROLA;

        vector<TPonto> PontosExtremidadesElementosBandeirola;
        //utilizado para verificar se a bandeirola � composta de uma reta horizontal e outra inclinada
        vector<TReta> RetasBandeirola;

        //transfere as retas e arcos para vetores vis�veis somente nesta fun��o
		for (m = 0; m < ListaItens->_multipoints.size(); m++)
        {
			vector<TPonto> &points = ListaItens->_multipoints[m]->pontos;
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

		
		for (m = 0; m < ListaItens->_arcs.size(); m++)
		{
            TPonto pontas[2];
            ListaItens->_arcs[m]->PontasArco( pontas );
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
                    PontosExtremidadesElementosBandeirola.erase(
                            PontosExtremidadesElementosBandeirola.begin() + j );
                    PontosExtremidadesElementosBandeirola.erase(
                            PontosExtremidadesElementosBandeirola.begin() + i );
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

        if (ListaItens->iTextos.size() == 1) //se a bandeirola possui um e somente um texto, gera o vértice
        {
            VerticeGeral.texto = _dados->Textos[ListaItens->iTextos[0]]->texto;
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
				shared_ptr<TTexto> text = _dados->Textos[ListaItens->iTextos[0] ];
                TPonto origemTexto = text->origem;
                origemTexto.x += (text->FatorAltura * FATOR_FONTE * text->texto.size()) * cos(text->rotacao ) / 2;
                origemTexto.x += text->FatorAltura * FATOR_FONTE * cos( text->rotacao * M_PI / 2 ) / 2;
                origemTexto.y += (text->FatorAltura * FATOR_FONTE * text->texto.size()) * sin( text->rotacao ) / 2;
                origemTexto.y += text->FatorAltura *
                FATOR_FONTE * sin( text->rotacao * M_PI / 2 ) / 2;
                //verifica qual o ponto livre da bandeirola mais longe do centro do texto
                PontoMaisDistante( origemTexto, PontosExtremidadesElementosBandeirola, MaisDist );
            }
            //verifica qual o cabo mais próximo da ponta da bandeirola
            CaboMaisProximo( MaisDist, iMenorDist, DistMaisProx, PontoNaReta, -1, -1 );
            if (iMenorDist >= 0)
            {
                _cabosReta[iMenorDist].AdicionaVertice( _verticesGerais->Tamanho(), PontoNaReta );
                VerticeGeral.pos = PontoNaReta;

                TPontosBandeirola pontoTemp;
                pontoTemp.NaBandeirola = MaisDist;
                pontoTemp.NoCabo = PontoNaReta;

                _pontosPraMostrarBandeirola.push_back( pontoTemp );
            }
            else //avisa caso o desenhos não possuam nenhum cabo
            {
                CErrosMsg *erros = CErrosMsg::getInstance();
                erros->novoErro(
                        (string) "Atenção: " + ExtractFileName( _dados->NomeArq.c_str() ).c_str()
                                + " não possui cabos." );
            }
        }
        else if (ListaItens->iTextos.size() > 1) //avisa caso a bandeirola tenha mais de um texto
        {
            CErrosMsg *erros = CErrosMsg::getInstance();
            erros->novoErro(
                    (string) "No desenho " + ExtractFileName( _dados->NomeArq.c_str() ).c_str()
                            + " existe um grupamento em nível de bandeirola com mais de um texto associado. Textos: " );
            for (int i = 0; i < ListaItens->iTextos.size(); i++)
                erros->novoErro( _dados->Textos[ListaItens->iTextos[i]]->texto );
            erros->novoErro( "" );
        }
        else if (ListaItens->iTextos.size() == 0) //avisa caso exista bandeirola sem texto nenhum
        {
            CErrosMsg *erros = CErrosMsg::getInstance();
            erros->novoErro(
                    (string) "No desenho " + ExtractFileName( _dados->NomeArq.c_str() ).c_str()
                            + " existe um grupamento em nível de bandeirola sem textos associados.\n" );
        }

        VerticeGeral.TipoElemento = BANDEIROLA;
        VerticeGeral.iDesenho = _dados->IndiceDesenho;
        VerticeGeral.IDArquivo = _dados->IDArquivo;
        _verticesGerais->Adiciona( VerticeGeral );
        delete ListaItens;
    }
}
//---------------------------------------------------------------------------

void CGrafoDesenho::GeraVerticesInstrumentosAdicionaMultipointCaboReta(
        TListaItensCelula *ListaItensCelula, TVectorPontoEIndiceCabo &ListaMenores, bool &ligado,
        TVectorInt *iVerticesInstrumento, TPonto PosVertice)
{
//percorre lista de cabos ligados ao equipamento
    for (int n = 0; n < (int) ListaMenores.size(); n++)
    {
		TPontoEIndiceCabo &pontoEIndiceCabo = ListaMenores[ n ];
        bool isAdded = ListaItensCelula->VerificaSeCaboRetaJaFoiLigadoAoEquipamento( pontoEIndiceCabo.IndiceCabo );

        if (!isAdded)
        {
            //adiciona o vértice na lista de vértices do cabo
            _cabosReta[pontoEIndiceCabo.IndiceCabo].AdicionaVertice( _verticesGerais->Tamanho(),
                    pontoEIndiceCabo.PosVertice );

            TVerticeGeral VerticeGeral;
            VerticeGeral.pos = pontoEIndiceCabo.PosVertice;
            VerticeGeral.TipoElemento = INSTRUMENTO;
            VerticeGeral.iDesenho = _dados->IndiceDesenho;
            VerticeGeral.IDArquivo = _dados->IDArquivo;
            VerticeGeral.TipoVertice = VERTICE_INSTRUMENTO_RETA;
            _verticesGerais->Adiciona( VerticeGeral );
			
            ListaItensCelula->cabosRetaRelacionados.push_back( pontoEIndiceCabo.IndiceCabo );

            if (_cabosReta[pontoEIndiceCabo.IndiceCabo].EhOPrimeiroPonto( pontoEIndiceCabo.PosVertice  ))
            {
                _cabosReta[pontoEIndiceCabo.IndiceCabo].ponta[0] = true;
            }
            else if (_cabosReta[pontoEIndiceCabo.IndiceCabo].EhOUltimoPonto( pontoEIndiceCabo.PosVertice ))
            {
                _cabosReta[pontoEIndiceCabo.IndiceCabo].ponta[1] = true;
            }

            shared_ptr<TAresta> Aresta( new TAresta() );
            for (int i = 0; i < ListaItensCelula->iTextos.size(); i++)
            {
                Aresta->AdicionaVertices( iVerticesInstrumento->at( i ),
                        _verticesGerais->Tamanho() - 1,
                        DistPontosManhattan( PosVertice, PosVertice ) );
                Aresta->IndiceDesenho = _dados->IndiceDesenho;
                Aresta->IDArquivo = _dados->IDArquivo;
				_arestas.push_back( Aresta );
            }
        }
    }
}
//---------------------------------------------------------------------------

void CGrafoDesenho::GeraVerticesInstrumentosAdicionaMultipointCaboArco(
        TListaItensCelula *ListaItensCelula, TVectorPontoEIndiceCabo &ListaMenores, bool &ligado,
        TVectorInt *iVerticesInstrumento, TPonto PosVertice)
{
    for (int n = 0; n < (int) ListaMenores.size(); n++)
    {
        bool isAdded;
        isAdded = ListaItensCelula->VerificaSeCaboArcoJaFoiLigadoAoEquipamento(
                ListaMenores.at( n ).IndiceCabo );

        if (!isAdded)
        {
            //if ( ListaItensCelula->iTexto > 0 )
            //  VerticesGerais->getItem(IndiceVertice)->texto = Textos[ListaItensCelula->iTexto].texto;
            ListaItensCelula->cabosArcoRelacionados.push_back( ListaMenores.at( n ).IndiceCabo );
            shared_ptr<TAresta> Aresta( new TAresta() );

            _cabosArco[ListaMenores.at( n ).IndiceCabo].ponta[ListaMenores.at( n ).IndiceArco] =
                    true;
            for (int i = 0; i < ListaItensCelula->iTextos.size(); i++)
            {
                Aresta->AdicionaVertices( iVerticesInstrumento->at( i ),
                        ListaMenores.at( n ).IndiceVertice,
                        DistPontosManhattan( PosVertice, PosVertice ) );
                Aresta->IndiceDesenho = _dados->IndiceDesenho;
                Aresta->IDArquivo = _dados->IDArquivo;
				_arestas.push_back( Aresta );
            }
        }
    }
}
//---------------------------------------------------------------------------

void CGrafoDesenho::GeraVerticesInstrumentosAdicionaMultipoint( shared_ptr<TMultipoint> multipoint,
        TListaItensCelula *ListaItensCelula, bool &ligado, TVectorInt *iVerticesInstrumento, TPonto PosVertice)
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
        GeraVerticesInstrumentosAdicionaMultipointCaboReta( ListaItensCelula, ListaMenores, ligado,
                iVerticesInstrumento, PosVertice );

        ListaMenores.clear();
        //localiza o cabo arco com a ponta mais pr�xima � reta
        DistRetaParaTodasPontasCaboArco( reta, ListaMenores, DIST_MIN_ELEM_CABO );
        //se a Distância for pequena, considera-se que estáo ligados
        GeraVerticesInstrumentosAdicionaMultipointCaboArco( ListaItensCelula, ListaMenores, ligado,
                iVerticesInstrumento, PosVertice );

    }
}
//---------------------------------------------------------------------------

void CGrafoDesenho::GeraVerticesInstrumentosAdicionaArco( shared_ptr<TArco> arc,
        TListaItensCelula *ListaItensCelula, bool &ligado, TVectorInt *iVerticesInstrumento, TPonto PosVertice)
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
            //adiciona o vértice na lista de vértices do cabo
            _cabosReta[IndiceCabo].AdicionaVertice( _verticesGerais->Tamanho(), PosVertice );

            TVerticeGeral VerticeGeral;
            VerticeGeral.pos = PosVertice;
            VerticeGeral.IDArquivo = _dados->IDArquivo;
            VerticeGeral.iDesenho = _dados->IndiceDesenho;
            VerticeGeral.TipoElemento = INSTRUMENTO;
            VerticeGeral.TipoVertice = VERTICE_INSTRUMENTO_ARCO;
            _verticesGerais->Adiciona( VerticeGeral );

            ListaItensCelula->cabosRetaRelacionados.push_back( IndiceCabo );
            if (_cabosReta[IndiceCabo].EhOPrimeiroPonto( PosVertice ))
            {
                _cabosReta[IndiceCabo].ponta[0] = true;
            }
            else if (_cabosReta[IndiceCabo].EhOUltimoPonto( PosVertice ))
            {
                _cabosReta[IndiceCabo].ponta[1] = true;
            }
            shared_ptr<TAresta> Aresta( new TAresta() );
            for (int i = 0; i < ListaItensCelula->iTextos.size(); i++)
            {
                Aresta->AdicionaVertices( iVerticesInstrumento->at( i ),
                        _verticesGerais->Tamanho() - 1,
                        //								DistPontosManhattan(PosVertice, PosVertice));
                        DistPontos( PosVertice, PosVertice ) );
                Aresta->IndiceDesenho = _dados->IndiceDesenho;
                Aresta->IDArquivo = _dados->IDArquivo;
				_arestas.push_back( Aresta );
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
 
		vector<int> iVerticesInstrumento;
        if (ListaItensCelula->iTextos.size() == 0)
        {
            CErrosMsg *erros = CErrosMsg::getInstance();
            erros->novoErro(
                    (string) "No desenho " + ExtractFileName( _dados->NomeArq.c_str() ).c_str()
                            + " existe um grupamento em nível de equipamento sem textos associados.\n" );
            continue;
        }

        PosVertice = AchaPosVerticeInstrumento( ListaItensCelula );
        CriaVerticesEArestasInstrumento( ListaItensCelula, iVerticesInstrumento, PosVertice );

		for (i = 0; i < ListaItensCelula->_multipoints.size(); i++)  //percorre todos os itens da c�lula atual
			GeraVerticesInstrumentosAdicionaMultipoint( ListaItensCelula->_multipoints[i], ListaItensCelula, ligado, &iVerticesInstrumento, PosVertice );

		for (i = 0; i < ListaItensCelula->_arcs.size(); i++)  //percorre todos os itens da c�lula atual
			GeraVerticesInstrumentosAdicionaArco( ListaItensCelula->_arcs[i], ListaItensCelula, ligado, &iVerticesInstrumento, PosVertice );
    }
}
//---------------------------------------------------------------------------

void CGrafoDesenho::CriaVerticesEArestasInstrumento(TListaItensCelula *ListaItensCelula,
        TVectorInt &iVerticesInstrumento, TPonto PosVertice )
{
    //para cada texto da c�lula cria um vértice
    for (int i = 0; i < ListaItensCelula->iTextos.size(); i++)
    {
        TVerticeGeral VerticeInstrumento;
        VerticeInstrumento.TipoElemento = INSTRUMENTO;
        VerticeInstrumento.iDesenho = _dados->IndiceDesenho;
        VerticeInstrumento.IDArquivo = _dados->IDArquivo;
        VerticeInstrumento.texto = _dados->Textos[ListaItensCelula->iTextos[i]]->texto;
        VerticeInstrumento.pos = PosVertice;
        iVerticesInstrumento.push_back( _verticesGerais->Tamanho() );
        VerticeInstrumento.TipoVertice = VERTICE_CENTRO_INSTRUMENTO;
        _verticesGerais->Adiciona( VerticeInstrumento );
    }

    // Caso o equipamento seja um colar de subida/descida, então � necess�rio adicionar uma aresta entre a subida e a descida.
    if (ListaItensCelula->iTextos.size() == 2)
    {
        shared_ptr<TAresta> Aresta( new TAresta() );
        Aresta->AdicionaVertices( iVerticesInstrumento[0], iVerticesInstrumento[1], 0 );
        Aresta->IndiceDesenho = _dados->IndiceDesenho;
        Aresta->IDArquivo = _dados->IDArquivo;
		_arestas.push_back( Aresta );
    }

}
//---------------------------------------------------------------------------

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
    int iMenorDist;     //, iV[2];
    double DistMaisProx;
    TPonto p[2], PontoNaReta, PontoTemp;
    shared_ptr<TArco> Arco;
    TVerticeGeral VerticeGeral;
    VerticeGeral.TipoVertice = VERTICE_ARCO;
    int IndiceCabo, IndiceVertice, PontaArco;
    n = m = iMenorDist = IndiceCabo = IndiceVertice = 0;
    DistMaisProx = 0;
    PontoNaReta.x = PontoNaReta.y = PontoTemp.x = PontoTemp.y = p[0].x = p[0].y = p[1].x = p[1].y =
            0.0;
    for (n = 0; n < _cabosArco.size(); n++)
    {
        /* Olha todos os arcos do desenho */
        Arco = _cabosArco[n]._arco;
        Arco->PontasArco( p );

        for (m = 0; m < 2; m++)
        {
            // iV[m] � o índice do novo vértice que será criado, e como ele será o último vértice,
            // o seu índice � igual ao total atual de vértices
            Arco->iV[m] = _verticesGerais->Tamanho();

            //verifica se a ponta deste cabo arco está ligada na ponta de outro cabo arco
            //se já estiver, não cria um vértice novo, aproveita o vértice que já existe
            //no outro cabo arco
            DistPontoParaPontaCaboArco( p[m], IndiceCabo, DistMaisProx, PontoTemp, IndiceVertice, n,
                    PontaArco );
            if (DistMaisProx < DIST_MIN_ELEM_CABO)
            {
                Arco->iV[m] = IndiceVertice;
                _cabosArco[n].ponta[m] = true;
                _cabosArco[IndiceCabo].ponta[PontaArco] = true;

            }
            else //se o vértice não foi aproveitado de outro cabo arco, verifica se este vértice está próximo a um cabo reta
            {
                /* Olha cada ponta do arco */
                CaboMaisProximo( p[m], iMenorDist, DistMaisProx, PontoNaReta, -1, Arco->Nivel );
				CCaboReta *caboReta = &(_cabosReta[iMenorDist]);
                if (DistMaisProx < DIST_MIN_ELEM_CABO)
                {
                    // Se a Distância for menor do que o limite, então esse vértice também será adicionado ao cabo mais próximo
                    caboReta->AdicionaVertice( Arco->iV[m], PontoNaReta );
                    VerticeGeral.pos = PontoNaReta;
                    _cabosArco[n].ponta[m] = true;
                    if (caboReta->EhOPrimeiroPonto( PontoNaReta ))
                    {
                        caboReta->ponta[0] = true;
                    }
                    else if (caboReta->EhOUltimoPonto( PontoNaReta ))
                    {
                        caboReta->ponta[1] = true;
                    }
                }
                else
                    VerticeGeral.pos = p[m];

                VerticeGeral.iDesenho = _dados->IndiceDesenho;
                VerticeGeral.IDArquivo = _dados->IDArquivo;
                _verticesGerais->Adiciona( VerticeGeral );
            }
        }

        shared_ptr<TAresta> Aresta( new TAresta() );
        Aresta->AdicionaVertices( Arco->iV[0], Arco->iV[1], DistPontosManhattan( p[0], p[1] ) );
        Aresta->IndiceDesenho = _dados->IndiceDesenho;
        Aresta->IDArquivo = _dados->IDArquivo;
		_arestas.push_back( Aresta );
    }
}
//---------------------------------------------------------------------------

void CGrafoDesenho::GeraVerticesPontaCabos()
{
    int n, m;
    int iMenorDist;
    double DistMaisProx;
    TPonto PontoNaReta;
    shared_ptr<TMultipoint> tMultipoint;
    TVerticeGeral VerticeGeral;
    VerticeGeral.TipoVertice = VERTICE_PONTA_CABO;
	for (n = 0; n < _cabosReta.size(); n++)
    {
        /* Olha todos os cabos reta do desenho */
		tMultipoint = _cabosReta[n]._multipoint;

        for (m = 0; m < tMultipoint->pontos.size(); m++)
        {
            if (m != 0 && m != tMultipoint->pontos.size() - 1)
                continue;
            /* Olha cada ponta da reta */
            CaboMaisProximo( tMultipoint->pontos[m], iMenorDist, DistMaisProx, PontoNaReta, n,
                    tMultipoint->Nivel );
            if (DistMaisProx < DIST_MIN_ELEM_CABO)
            {
                // Se a Distância for menor do que o limite, então esse vértice também será adicionado ao cabo mais próximo
                _cabosReta[iMenorDist].AdicionaVertice( _verticesGerais->Tamanho(), PontoNaReta );
                VerticeGeral.pos = PontoNaReta;
                //        if ( CabosReta[iMenorDist]->EhOPrimeiroPonto(PontoNaReta, Dados->Multipoint, DistMinElemCabo) )
                //          CabosReta[iMenorDist]->ponta[0] = true;
                //       if ( CabosReta[iMenorDist]->EhOUltimoPonto(PontoNaReta, Dados->Multipoint, DistMinElemCabo) )
                //          CabosReta[iMenorDist]->ponta[1] = true;
                //        CabosReta[iMenorDist]->ponta[m] = true;
                if (m == 0)
                    _cabosReta[n].ponta[0] = true;
                else
                    _cabosReta[n].ponta[1] = true;
            }
            else
                VerticeGeral.pos = tMultipoint->pontos[m];
            _cabosReta[n].AdicionaVertice( _verticesGerais->Tamanho(), tMultipoint->pontos[m] );
            VerticeGeral.iDesenho = _dados->IndiceDesenho;
            VerticeGeral.IDArquivo = _dados->IDArquivo;
            _verticesGerais->Adiciona( VerticeGeral );
        }
    }
}
//---------------------------------------------------------------------------

void CGrafoDesenho::DistRetaParaPontaCaboReta(TPonto Reta[2], int &IndiceCabo,
        double &DistMaisPerto, TPonto &PosVertice)
{
    int m, n;
    unsigned char ContadorMaior;
    double MenorDist, Dist;
    TPonto PontoTemp;
    IndiceCabo = -1;
    MenorDist = Infinity;
	for (m = 0; m < _cabosReta.size(); m++)
    {
        // PontosCabo tem os pontos de in�cio e fim do cabo
		vector<TPonto> &PontosCabo = _cabosReta[m]._multipoint->pontos;
        for (n = 0; n < 2; n++)
        {
            Dist = DistPontoParaSegmentoReta( Reta, PontosCabo[n], PontoTemp );
            if (Dist < MenorDist)
            {
                MenorDist = Dist;
                IndiceCabo = m;
                PosVertice = PontosCabo[n];
            }
        }        //for (n=0; n<2; n++)
    }        //for (m=0; m<NumCabosReta; m++)
    DistMaisPerto = MenorDist;
}
//---------------------------------------------------------------------------

void CGrafoDesenho::DistRetaParaTodasPontasCaboReta(TPonto Reta[2],
        TVectorPontoEIndiceCabo &ListaMenores, double DistMinElemCabo)
{
    int m, n;
    unsigned char ContadorMaior;
    double Dist;
    TPonto PontoTemp;
    TPontoEIndiceCabo Cabo;
	for (m = 0; m < _cabosReta.size(); m++)
    {
        // PontosCabo tem os pontos de in�cio e fim do cabo
		vector<TPonto> &PontosCabo = _cabosReta[m]._multipoint->pontos;
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

void CGrafoDesenho::DistRetaParaPontaCaboArco(TPonto Reta[2], int &IndiceCabo,
        double &DistMaisPerto, TPonto &PosVertice, int &IndiceVertice)
{
    int m, n;
    unsigned char ContadorMaior;
    double MenorDist, Dist;
    TPonto pontoCabo, PontoTemp;
    IndiceCabo = -1;
    MenorDist = Infinity;
	for (m = 0; m < _cabosArco.size(); m++)
    {
		shared_ptr<TArco> arco = _cabosArco[m]._arco;
        for (n = 0; n < 2; n++)
        {
			pontoCabo = _verticesGerais->getItem( arco->iV[n] )->pos;
            Dist = DistPontoParaSegmentoReta( Reta, pontoCabo, PontoTemp );
            if (Dist < MenorDist)
            {
                MenorDist = Dist;
                IndiceCabo = m;
                PosVertice = pontoCabo;
                IndiceVertice = arco->iV[n];
            }
        }        //for (n=0; n<2; n++)
    }        //for (m=0; m<NumCabosReta; m++)
    DistMaisPerto = MenorDist;
}
//---------------------------------------------------------------------------

void CGrafoDesenho::DistRetaParaTodasPontasCaboArco(TPonto Reta[2],
        TVectorPontoEIndiceCabo &ListaMenores, double DistMinElemCabo)
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
			pontoCabo = _verticesGerais->getItem( _cabosArco[m]._arco->iV[n] )->pos;
            Dist = DistPontoParaSegmentoReta( Reta, pontoCabo, PontoTemp );
            if (Dist < DistMinElemCabo)
            {
                Cabo.IndiceCabo = m;
                Cabo.PosVertice = pontoCabo;
                Cabo.IndiceVertice = _cabosArco[m]._arco->iV[n];
                Cabo.IndiceArco = n;
                ListaMenores.push_back( Cabo );
                //        MenorDist=Dist;
                //        IndiceCabo=m;
                //        PosVertice=PontosCabo[n];
            }
        }        //for (n=0; n<2; n++)
    }        //for (m=0; m<NumCabosReta; m++)
}
//---------------------------------------------------------------------------

void CGrafoDesenho::DistArcoParaTodasPontasRetaCabo(TArco &Arco,
        TVectorPontoEIndiceCabo &ListaMenores, double DistMinElemCabo)
{
    int m, n;
    unsigned char ContadorMaior;
    double Dist;
    TPonto *PontosCabo;

#ifdef DEBUG_BUILDER
    TStringList *lista=new TStringList;
#endif
    //	IndiceCabo=-1;
    //	MenorDist=Infinity;
    TPontoEIndiceCabo Cabo;
	for (m = 0; m < _cabosReta.size(); m++)
    {
		vector<TPonto> &PontosCabo = _cabosReta[m]._multipoint->pontos;
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

#ifdef DEBUG_BUILDER
                lista->Add("indice cabo: "+IntToStr(m));
#endif
                Cabo.IndiceCabo = m;
                Cabo.PosVertice = PontosCabo[n];
                Cabo.Dist = Dist;
                ListaMenores.push_back( Cabo );
            }
        }  //for (n=0; n<2; n++)
    }  //for (m=0; m<NumCabosReta; m++)
       //	DistMaisPerto=MenorDist;
#ifdef DEBUG_BUILDER
    delete lista;
#endif
}
//---------------------------------------------------------------------------

//ESTA FUN��O DEVE SE ADAPTAR PARA TRABALHAR COM ARCOS QUE não são C�RCULOS
void CGrafoDesenho::DistArcoParaPontaRetaCabo(TArco &Arco, int &IndiceCabo, double &DistMaisPerto,
        TPonto &PosVertice)
{
    int m, n;
    unsigned char ContadorMaior;
    double MenorDist, Dist;
    IndiceCabo = -1;
    MenorDist = Infinity;
	for (m = 0; m < _cabosReta.size(); m++)
    {
        vector<TPonto> &PontosCabo = _cabosReta[m]._multipoint->pontos;
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
void CGrafoDesenho::DistArcoParaPontaArcoCabo(TArco &Arco, int &IndiceArcoCabo,
        double &DistMaisPerto, TPonto &PosVertice)
{
    int m, n;
    unsigned char ContadorMaior;
    double MenorDist, Dist;
    TPonto PontosCabo[2];
    IndiceArcoCabo = -1;
    MenorDist = Infinity;
    for (m = 0; m < _cabosArco.size(); m++)
    {
        _cabosArco[m]._arco->PontasArco( PontosCabo );
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

void CGrafoDesenho::DistPontoParaPontaCaboArco(TPonto ponto, int &IndiceCabo, double &DistMaisPerto,
        TPonto &PosVertice, int &IndiceVertice, int IndiceMax, int &pontaCabo)
{
    int m, n;
    unsigned char ContadorMaior;
    double MenorDist, Dist;
    TPonto pontoCabo, PontoTemp;
    IndiceCabo = -1;
    MenorDist = Infinity;
    int menor = IndiceMax;
    if (menor > _cabosArco.size())
        menor = _cabosArco.size();
    for (m = 0; m < menor; m++)
    {
        for (n = 0; n < 2; n++)
        {
			pontoCabo = _verticesGerais->getItem( _cabosArco[m]._arco->iV[n] )->pos;
            Dist = DistPontos( ponto, pontoCabo );
            if (Dist < MenorDist)
            {
                MenorDist = Dist;
                IndiceCabo = m;
                PosVertice = pontoCabo;
                pontaCabo = n;
                IndiceVertice = _cabosArco[m]._arco->iV[n];
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
        if (_cabosReta[n].TipoOrientacao == VERTICAL)
        {
            sort(_cabosReta[n].VerticesReta.begin(), _cabosReta[n].VerticesReta.end(), OrdenaRetaVertical );
        }
        else
        {
            sort(_cabosReta[n].VerticesReta.begin(), _cabosReta[n].VerticesReta.end(), OrdenaRetaHorizontal );
        }
    }
}
//---------------------------------------------------------------------------

void CGrafoDesenho::GeraColares(const std::vector<TDesenho*> &ListaDesenhos)
{
    if (!CarregaGrafo)
        return;
    int n;
    vector<TVerticeGeral*> Lista;
    _verticesGerais->ListaOrd( &Lista );  //gera lista ordenada
    TVerticeGeral *V1, *V2;
    for (n = 0; n < (int) (Lista.size() - 1); n++)
    {
        V1 = Lista[n];
        if ( V1->texto != "" )
        {
            V2 = Lista[n + 1];
            if ((V1->texto == V2->texto) && (V1->iDesenho != V2->iDesenho)
                    && (V1->TipoElemento == INSTRUMENTO) && (V2->TipoElemento == INSTRUMENTO))
            {
                double alturaDaAresta = ListaDesenhos[V1->iDesenho]->Altura
                        - ListaDesenhos[V2->iDesenho]->Altura;
                if (alturaDaAresta < 0)
                    alturaDaAresta *= -1;
                shared_ptr<TAresta> Aresta( new TAresta() );
                Aresta->AdicionaVertices( V1->IndiceOriginal, V2->IndiceOriginal, alturaDaAresta );
                Aresta->IndiceDesenho = I_DESENHO_NULO;
                Aresta->IDArquivo = I_DESENHO_NULO;
				_arestas.push_back( Aresta );

                _verticesGerais->getItem( V1->IndiceOriginal )->EhColar = _verticesGerais->getItem(
                        V2->IndiceOriginal )->EhColar = true;
            }
        }
    }
    for (int i = 0; i < (int) Lista.size(); i++)
    {
        delete Lista[i];
    }
}
//---------------------------------------------------------------------------

void CGrafoDesenho::ChecagemVerticeDuplo(const std::vector<TDesenho*> &ListaDesenhos)
{
    if (!CarregaGrafo)
        return;

    int n;
    vector<TVerticeGeral*> *Lista = new vector<TVerticeGeral*>();
    _verticesGerais->ListaOrd( Lista );  //gera lista ordenada
    TVerticeGeral *V1, *V2;
    string Ultimo;
    for (n = 0; n < (int) (Lista->size() - 1); n++)
    {
        V1 = Lista->at( n );
        if (V1->texto != "")
        {
            V2 = Lista->at( n + 1 );
            if (V1->texto == V2->texto)
            {

                if (V1->iDesenho != V2->iDesenho
                        && (V1->TipoElemento == INSTRUMENTO && V2->TipoElemento == INSTRUMENTO)
                        && (n + 2 < (int) (Lista->size() - 1)
                                && (Lista->at( n + 1 ))->texto != V1->texto))
                {
                    n++;
                    break;
                }

                CErrosMsg *erros = CErrosMsg::getInstance();
                erros->novoErro( "Elementos com o texto \"" + V1->texto + "\" repetido: " );

                for (; n < (int) Lista->size(); n++)
                {
                    if ((Lista->at( n ))->texto == V1->texto)
                    {
                        string tipo;
                        if ((Lista->at( n ))->TipoElemento == INSTRUMENTO)
                        {
                            tipo = "Equipamento";
                        }
                        else if ((Lista->at( n ))->TipoElemento == CABO)
                        {
                            tipo = "Cabo";
                        }
                        else if ((Lista->at( n ))->TipoElemento == BANDEIROLA)
                        {
                            tipo = "Bandeirola";
                        }
                        else
                            tipo = "Desconhecido";

                        erros->novoErro(
                                "No desenho: "
                                        + ListaDesenhos[((TVerticeGeral *) (Lista->at( n )))->iDesenho]->NomeArquivo + " em nível de "
                                        + tipo );
                    }
                    else
                        break;
                }
                erros->novoErro( "" );
            }
        }
    }
    for (int i = 0; i < (int) Lista->size(); i++)
    {
        delete Lista->at( i );
    }
    delete Lista;
}
//---------------------------------------------------------------------------

void CGrafoDesenho::GeraArestas()
{
    int n, m;
    TVerticeReta *VerticesReta1, *VerticesReta2;
	for (n = 0; n < _cabosReta.size(); n++)
    {
        for (m = 0; m < _cabosReta[n].NumVertices - 1; m++)
        {
            VerticesReta1 = &_cabosReta[n].VerticesReta[m];
            VerticesReta2 = &_cabosReta[n].VerticesReta[m + 1];
            shared_ptr<TAresta> Aresta( new TAresta() );
            Aresta->AdicionaVertices( VerticesReta1->ID, VerticesReta2->ID,
            //					DistPontosManhattan(VerticesReta1->pos, VerticesReta2->pos));
                    DistPontos( VerticesReta1->pos, VerticesReta2->pos ) );
            Aresta->IndiceDesenho = _dados->IndiceDesenho;
            Aresta->IDArquivo = _dados->IDArquivo;
			_arestas.push_back( Aresta );
        }
    }
}
//---------------------------------------------------------------------------

