//---------------------------------------------------------------------------
#pragma hdrstop
#include "UGrafoDesenho.h"
#include "UErros.h"
#include "UTempoExec.h"
#include "UGeometria.h"
#include "UContainerDesenhos.h"
#include "UListaCelulas.h"

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
bool CCaboReta::EhOPrimeiroPonto(TPonto ponto, vector<TMultipoint> Multipoint, int IndiceCabo)
{
    TPonto pontoM = Multipoint[Indice].pontos[0]; //primeiro ponto
    TPonto pontoR = Multipoint[Indice].pontos[Multipoint[Indice].pontos.size() - 1]; //último ponto
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
bool CCaboReta::EhOUltimoPonto(TPonto ponto, vector<TMultipoint> Multipoint, int IndiceCabo)
{
    TPonto pontoM = Multipoint[Indice].pontos[Multipoint[Indice].pontos.size() - 1];
    TPonto pontoR = Multipoint[Indice].pontos[0];
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
//                                            : CDadosDGN(ParamsGrafoDesenho)
{
    pri = 0;
    CarregaGrafo = ParamsGrafoDesenho.CarregaGrafo;

    CTempoExec *tempo = CTempoExec::getInstance();

    tempo->MarcaTempo( "Inicio da marcacao de tempo no grafoDesenho" );

    // Cria um DadosGenerico para ser usado
    this->Dados = Dados;

    if (CarregaGrafo)
    {
        unsigned int n;
        memset( TipoElementoCor, 0, NUM_CORES * sizeof(TTipoElemento) );
        VerticesGerais = ParamsGrafoDesenho.VerticesGerais;

        //O vértice 0 não pode ser usado, por isso adiciona-se este vértice vazio
        TVerticeGeral temp;
        VerticesGerais->Adiciona( temp );

        Arestas = ParamsGrafoDesenho.Arestas;

        _numCabosReta = 0;
        _numCabosArco = 0;

        AlocaElementos();
#ifdef DEBUG_BUILDER
        tempo->MarcaTempo("AlocaElementos");
#endif
        GeraListaCabos();
#ifdef DEBUG_BUILDER
        tempo->MarcaTempo("GeraListaCabos");
#endif
        GeraVerticesBandeirola();
#ifdef DEBUG_BUILDER
        tempo->MarcaTempo("GeraVerticesBandeirola");
#endif
        GeraVerticesArcos();
#ifdef DEBUG_BUILDER
        tempo->MarcaTempo("GeraVerticesArcos");
#endif
        GeraVerticesPontaCabos();
#ifdef DEBUG_BUILDER
        tempo->MarcaTempo("GeraVerticesPontaCabos");
#endif
        GeraVerticesInstrumentos();
#ifdef DEBUG_BUILDER
        tempo->MarcaTempo("GeraVerticesInstrumentos");
        TStringList *ListaVertices=new TStringList;
        AnsiString texto;
        AnsiString StrTipoVertice;
        for (int p=0; p<VerticesGerais->Tamanho(); p++)
        {
            switch (VerticesGerais->getItem(p)->TipoVertice)
            {
                case VERTICE_BANDEIROLA: StrTipoVertice="VERTICE_BANDEIROLA"; break;
                case VERTICE_ARCO: StrTipoVertice="VERTICE_ARCO"; break;
                case VERTICE_PONTA_CABO: StrTipoVertice="VERTICE_PONTA_CABO"; break;
                case VERTICE_INSTRUMENTO_RETA: StrTipoVertice="VERTICE_INSTRUMENTO_RETA"; break;
                case VERTICE_INSTRUMENTO_ARCO: StrTipoVertice="VERTICE_INSTRUMENTO_ARCO"; break;
                case VERTICE_CENTRO_INSTRUMENTO: StrTipoVertice="VERTICE_CENTRO_INSTRUMENTO "; break;
            }
            texto=VerticesGerais->getItem(p)->texto.c_str();
            ListaVertices->Add("Vertice: "+IntToStr(p)+" Posi��o: ("+VerticesGerais->getItem(p)->pos.x+
                    +", "+VerticesGerais->getItem(p)->pos.y+") Texto: "+texto+
                    +" iDesenho: "+IntToStr(VerticesGerais->getItem(p)->iDesenho)+
                    +" IDArquivo: "+IntToStr(VerticesGerais->getItem(p)->IDArquivo)+
                    +" Tipo vértice: "+StrTipoVertice );
        }
        ListaVertices->SaveToFile("t:\\ListaVertices.txt");
        delete ListaVertices;
#endif
        OrdenaVerticesRetas();
        GeraArestas();
//    ult = Dados->NumMultipoint;
#ifdef DEBUG_BUILDER
        tempo->MostraTempo((string)"Tempo " + ExtractFileName(Dados->NomeArq.c_str()).c_str() + ".txt");
#endif
    }
    ult = Dados->Multipoint.size();
}
//---------------------------------------------------------------------------

CGrafoDesenho::~CGrafoDesenho()
{
    if (CarregaGrafo)
    {
        int n;
        for (n = 0; n < Dados->Multipoint.size(); n++)
            delete CabosReta[n];
        for (n = 0; n < Dados->Arcos.size(); n++)
            delete CabosArco[n];
        delete[] CabosArco;
        delete[] CabosReta;
        //		delete [] iCabosArco;
        delete[] iLinhasBandeirola;
        delete[] iCirculosBandeirola;
        delete[] iRetangulosInstrumento;
        delete[] iCirculosInstrumento;
        CarregaGrafo = false;
    }
}
//---------------------------------------------------------------------------

void CGrafoDesenho::AlocaElementos()
{
    int n;
    CabosReta = new CCaboReta *[Dados->Multipoint.size()];
    for (n = 0; n < Dados->Multipoint.size(); n++)
        CabosReta[n] = new CCaboReta();
    CabosArco = new CCaboArco *[Dados->Arcos.size()];
    for (n = 0; n < Dados->Arcos.size(); n++)
        CabosArco[n] = new CCaboArco();

    //	iCabosArco =             new int [Dados->NumArcos];
    iLinhasBandeirola = new int[Dados->Multipoint.size()];
    iCirculosBandeirola = new int[Dados->Arcos.size()];
    iRetangulosInstrumento = new int[Dados->Multipoint.size()];
    iCirculosInstrumento = new int[Dados->Multipoint.size()];
}
//---------------------------------------------------------------------------

//percorre os elementos b�sicos de desenho do CableWay, como Multipoint e Arcos
//e converte para os elementos CabosReta
void CGrafoDesenho::GeraListaCabos()
{
    int n;
    vector<TPonto> Pontos;
    double DifX, DifY;
    for (n = 0; n < Dados->Multipoint.size(); n++)
    {
        //verifica o tipo de elemento pela cor
        if ((Dados->Multipoint[n].Nivel == CABO)/* &&
         (Multipoint[n].Estilo==0)*/)  //ESTILO S�LIDO
        {
            CabosReta[_numCabosReta]->Indice = n;
            Pontos = Dados->Multipoint[n].pontos;
            DifX = fabs( Pontos[0].x - Pontos[1].x );
            DifY = fabs( Pontos[0].y - Pontos[1].y );
            CabosReta[_numCabosReta]->TipoOrientacao = (DifY > DifX) ? VERTICAL : HORIZONTAL;
            //      CabosReta[NumCabosReta]->ponta[0] = CabosReta[NumCabosReta]->ponta[1] = false;
            _numCabosReta++;
        }
    }

    double SomaRaios = 0;
    for (n = 0; n < Dados->Arcos.size(); n++)
    {
        if ((Dados->Arcos[n].Nivel == CABO) && (Dados->Arcos[n].AngTam < 180.0)/* &&
         (Arcos[n].Estilo==0)*/)  //ESTILO S�LIDO
        {
            CabosArco[_numCabosArco]->Indice = n;
            //			iCabosArco[NumCabosArco]=n;
            _numCabosArco++;
            SomaRaios += Dados->Arcos[n].EixoPrimario;
        }
    }
    MediaRaioCaboArco = _numCabosArco > 0 ? SomaRaios / _numCabosArco : 0;
    //DistMinElemCaboPraOpenGL = MediaRaioCaboArco * FATOR_DIST_MIN_ELEM_CABO;
	//temporary!
	DistMinElemCaboPraOpenGL = 10.0;
}
//---------------------------------------------------------------------------

//Acha o cabo mais próximo a um ponto. Retorna o índice do cabo,
//a Distância � este cabo, e o ponto do cabo que está mais próximo ao ponto dado
void CGrafoDesenho::CaboMaisProximo(TPonto &ponto, int &IndiceCabo, double &DistMaisProx,
        TPonto &PosVertice, int Diferente, int Nivel)
{
    int m;
    double MenorDist, Dist;
    vector<TPonto> PontosCabo;
    IndiceCabo = -1;
    MenorDist = Infinity;
    for (m = 0; m < _numCabosReta; m++)
    {
        if ((Nivel != -1) && (Dados->Multipoint[CabosReta[m]->Indice].Nivel != Nivel))
            continue;
        PontosCabo = Dados->Multipoint[CabosReta[m]->Indice].pontos;
        TPonto volta;
        Dist = DistPontoParaSegmentoReta( &PontosCabo[0], ponto, volta );
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
                (string) "Atenção: " + ExtractFileName( Dados->NomeArq.c_str() ).c_str()
                        + " não possui cabos." );
        /*    if ( Dados->IMP )
         {
         //ShowMessage("Erro: não há cabo.");
         }
         */
    }
}
//---------------------------------------------------------------------------

// Acha a ponta 1
void CGrafoDesenho::AchaPonta(int &iPonto, int &iM, int &Ponta, TListaItensCelula *ListaItens,
        double DistMinBandeirola)
{
    int m;
    TVectorInt *JahPassou = new TVectorInt();
    int menores = 0;
    double MenorDist, Dist;
    TPonto Ponto;
    int IndiceCabo, CaboPonta;

    TPonto PontoInicial;
    JahPassou->clear();
    JahPassou->push_back( 0 );

    Ponto = Dados->Multipoint[iPonto].pontos[Ponta];
    PontoInicial = Ponto;

    do
    {
        IndiceCabo = -1;
        MenorDist = Infinity;
        menores = 0;
        for (m = 0; m < ListaItens->Tamanho(); m++)
        {
            if (m == iM)
                continue;

            vector<TPonto> &PontosReta = Dados->Multipoint[ListaItens->getItem( m )->Indice].pontos;
            // V� a Distância pra primeira ponta do cabo
            Dist = DistPontos( PontosReta[0], Ponto );
            if (Dist - MenorDist < DistMinBandeirola && Dist < DistMinBandeirola)
            {
                menores++;
                MenorDist = Dist;
                IndiceCabo = m;
                CaboPonta = Dados->Multipoint[ListaItens->getItem( m )->Indice].pontos.size() - 1;
                if (menores > 1)
                    CaboPonta = 0;
            }

            // V� a Distância pra segunda ponta do cabo
            Dist =
                    DistPontos(
                            PontosReta[Dados->Multipoint[ListaItens->getItem( m )->Indice].pontos.size()
                                    - 1], Ponto );
            if (Dist - MenorDist < DistMinBandeirola && Dist < DistMinBandeirola)
            {
                menores++;
                MenorDist = Dist;
                IndiceCabo = m;
                CaboPonta = 0;
                if (menores > 1)
                    CaboPonta = Dados->Multipoint[ListaItens->getItem( m )->Indice].pontos.size()
                            - 1;
            }
        }  //for (m=0; m<NumCabosReta; m++)

        if (menores > 1)
        {
            iM = IndiceCabo;
            iPonto = ListaItens->getItem( IndiceCabo )->Indice;
            Ponta = CaboPonta;
            Ponto = Dados->Multipoint[iPonto].pontos[Ponta];
            break;
        }

        if (IndiceCabo >= 0)
        {
            iM = IndiceCabo;
            iPonto = ListaItens->getItem( IndiceCabo )->Indice;
            Ponta = CaboPonta;
            Ponto = Dados->Multipoint[iPonto].pontos[Ponta];
            if (iM != JahPassou->at( 0 ))
                JahPassou->push_back( iM );
            if (fabs( Ponto.x - PontoInicial.x ) < DistMinBandeirola
                    && fabs( Ponto.y - PontoInicial.y ) < DistMinBandeirola)
            {
                while (JahPassou->size())
                {
                    int frente = JahPassou->front();
                    ListaItens->Remove( frente );
                    JahPassou->erase( JahPassou->begin() );
                    for (m = 0; m < (int) JahPassou->size(); m++)
                    {
                        if (JahPassou->at( m ) > frente)
                            JahPassou->at( m )--;}
                    }
                if (ListaItens->Tamanho() == 0)
                    break;
                IndiceCabo = 0;
                CaboPonta = 0;
                iM = IndiceCabo;
                iPonto = ListaItens->getItem( IndiceCabo )->Indice;
                Ponta = CaboPonta;
                Ponto = Dados->Multipoint[iPonto].pontos[Ponta];
            }
        }
    } while (IndiceCabo >= 0);
    delete JahPassou;
}

//---------------------------------------------------------------------------
//adiciona os vértices das bandeirolas nas estruturas dos cabos
void CGrafoDesenho::GeraVerticesBandeirola()
{
    int n, m, iMenorDist;
    TPonto PontoBandeirola[2], PontoNaReta;
    TListaCelulas *ListaBandeirolas;
    TListaItensCelula *ListaItens;
    TItemCelula *ItemCelula;
    ListaBandeirolas = Dados->InfoCelula.ListaCelulasBandeirolas;

    //percorre todas as bandeirolas
    for (n = 0; n < ListaBandeirolas->Tamanho(); n++)
    {
        int NumArcosBandeirola = 0;
        ListaItens = new TListaItensCelula( *ListaBandeirolas->getItem( n ) );
        TVerticeGeral VerticeGeral;
        VerticeGeral.TipoVertice = VERTICE_BANDEIROLA;

        vector<TPonto> PontosExtremidadesElementosBandeirola;
        //utilizado para verificar se a bandeirola � composta de uma reta horizontal e outra inclinada
        vector<TReta> RetasBandeirola;

        //transfere as retas e arcos para vetores vis�veis somente nesta fun��o
        for (m = 0; m < ListaItens->Tamanho(); m++)
        {
            // Cada elemento reta da bandeirola
            if (ListaItens->getItem( m )->TipoVetorCW == VMULTIPOINT)
            {
                int IndiceNoVetorMultipoint = ListaItens->getItem( m )->Indice;
                int NumPontos = Dados->Multipoint[IndiceNoVetorMultipoint].pontos.size();
                int IndiceDoUltimoPonto = NumPontos - 1;
                // Pega o primeiro e o último ponto do multipoint e p�e na lista de extremidades.
                PontosExtremidadesElementosBandeirola.push_back(
                        Dados->Multipoint[IndiceNoVetorMultipoint].pontos[0] );
                PontosExtremidadesElementosBandeirola.push_back(
                        Dados->Multipoint[IndiceNoVetorMultipoint].pontos[IndiceDoUltimoPonto] );
                for (int k = 0; k < (NumPontos - 1); k++)
                {
                    TReta reta;
                    reta.pontos[0] = Dados->Multipoint[IndiceNoVetorMultipoint].pontos[k];
                    reta.pontos[1] = Dados->Multipoint[IndiceNoVetorMultipoint].pontos[k + 1];
                    RetasBandeirola.push_back( reta );
                }
            }
            // Cada elemento arco da bandeirola
            else if (ListaItens->getItem( m )->TipoVetorCW == VARCO)
            {
                TPonto pontas[2];
                int IndiceNoVetorArco = ListaItens->getItem( m )->Indice;
                Dados->Arcos[IndiceNoVetorArco].PontasArco( pontas );
                // Pega as pontas do arco e p�e na lista de extremidades
                PontosExtremidadesElementosBandeirola.push_back( pontas[0] );
                PontosExtremidadesElementosBandeirola.push_back( pontas[1] );
                NumArcosBandeirola++;
            }
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
            VerticeGeral.texto = Dados->Textos[ListaItens->iTextos[0]].texto;
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
                TPonto origemTexto = Dados->Textos[ListaItens->iTextos[0] ].origem;
                origemTexto.x += (Dados->Textos[ListaItens->iTextos[0] ].FatorAltura *
                FATOR_FONTE * Dados->Textos[ListaItens->iTextos[0] ].texto.size())
                        * cos( Dados->Textos[ListaItens->iTextos[0]].rotacao ) / 2;
                origemTexto.x += Dados->Textos[ListaItens->iTextos[0]].FatorAltura *
                FATOR_FONTE * cos( Dados->Textos[ListaItens->iTextos[0]].rotacao * M_PI / 2 )
                        / 2;
                origemTexto.y += (Dados->Textos[ListaItens->iTextos[0]].FatorAltura *
                FATOR_FONTE * Dados->Textos[ListaItens->iTextos[0]].texto.size())
                        * sin( Dados->Textos[ListaItens->iTextos[0]].rotacao ) / 2;
                origemTexto.y += Dados->Textos[ListaItens->iTextos[0]].FatorAltura *
                FATOR_FONTE * sin( Dados->Textos[ListaItens->iTextos[0]].rotacao * M_PI / 2 )
                        / 2;
                //verifica qual o ponto livre da bandeirola mais longe do centro do texto
                PontoMaisDistante( origemTexto, PontosExtremidadesElementosBandeirola, MaisDist );
            }
            //verifica qual o cabo mais próximo da ponta da bandeirola
            CaboMaisProximo( MaisDist, iMenorDist, DistMaisProx, PontoNaReta, -1, -1 );
            if (iMenorDist >= 0)
            {
                CabosReta[iMenorDist]->AdicionaVertice( VerticesGerais->Tamanho(), PontoNaReta );
                VerticeGeral.pos = PontoNaReta;

                TPontosBandeirola pontoTemp;
                pontoTemp.NaBandeirola = MaisDist;
                pontoTemp.NoCabo = PontoNaReta;

                PontosPraMostrarBandeirola.push_back( pontoTemp );
            }
            else //avisa caso o desenhos não possuam nenhum cabo
            {
                CErrosMsg *erros = CErrosMsg::getInstance();
                erros->novoErro(
                        (string) "Atenção: " + ExtractFileName( Dados->NomeArq.c_str() ).c_str()
                                + " não possui cabos." );
            }
        }
        else if (ListaItens->iTextos.size() > 1) //avisa caso a bandeirola tenha mais de um texto
        {
            CErrosMsg *erros = CErrosMsg::getInstance();
            erros->novoErro(
                    (string) "No desenho " + ExtractFileName( Dados->NomeArq.c_str() ).c_str()
                            + " existe um grupamento em nível de bandeirola com mais de um texto associado. Textos: " );
            for (int i = 0; i < ListaItens->iTextos.size(); i++)
                erros->novoErro( Dados->Textos[ListaItens->iTextos[i]].texto );
            erros->novoErro( "" );
        }
        else if (ListaItens->iTextos.size() == 0) //avisa caso exista bandeirola sem texto nenhum
        {
            CErrosMsg *erros = CErrosMsg::getInstance();
            erros->novoErro(
                    (string) "No desenho " + ExtractFileName( Dados->NomeArq.c_str() ).c_str()
                            + " existe um grupamento em nível de bandeirola sem textos associados.\n" );
        }

        VerticeGeral.TipoElemento = BANDEIROLA;
        VerticeGeral.iDesenho = Dados->IndiceDesenho;
        VerticeGeral.IDArquivo = Dados->IDArquivo;
        VerticesGerais->Adiciona( VerticeGeral );
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
        bool isAdded;
        isAdded = ListaItensCelula->VerificaSeCaboRetaJaFoiLigadoAoEquipamento(
                ListaMenores.at( n ).IndiceCabo );

        if (!isAdded)
        {
            ligado = ligaEquipamentoSeDesligado( ListaItensCelula, ligado );
            //adiciona o vértice na lista de vértices do cabo
            CabosReta[ListaMenores.at( n ).IndiceCabo]->AdicionaVertice( VerticesGerais->Tamanho(),
                    ListaMenores.at( n ).PosVertice );

            TVerticeGeral VerticeGeral;
            VerticeGeral.pos = ListaMenores.at( n ).PosVertice;
            VerticeGeral.TipoElemento = INSTRUMENTO;
            VerticeGeral.iDesenho = Dados->IndiceDesenho;
            VerticeGeral.IDArquivo = Dados->IDArquivo;
            VerticeGeral.TipoVertice = VERTICE_INSTRUMENTO_RETA;
            VerticesGerais->Adiciona( VerticeGeral );

            ListaItensCelula->cabosRetaRelacionados.push_back( ListaMenores.at( n ).IndiceCabo );

            if (CabosReta[ListaMenores.at( n ).IndiceCabo]->EhOPrimeiroPonto(
                    ListaMenores.at( n ).PosVertice, Dados->Multipoint,
                    ListaMenores.at( n ).IndiceCabo ))
            {
                CabosReta[ListaMenores.at( n ).IndiceCabo]->ponta[0] = true;
            }
            else if (CabosReta[ListaMenores.at( n ).IndiceCabo]->EhOUltimoPonto(
                    ListaMenores.at( n ).PosVertice, Dados->Multipoint,
                    ListaMenores.at( n ).IndiceCabo ))
            {
                CabosReta[ListaMenores.at( n ).IndiceCabo]->ponta[1] = true;
            }

            TAresta Aresta;
            for (int i = 0; i < ListaItensCelula->iTextos.size(); i++)
            {
                Aresta.AdicionaVertices( iVerticesInstrumento->at( i ),
                        VerticesGerais->Tamanho() - 1,
                        DistPontosManhattan( PosVertice, PosVertice ) );
                Aresta.IndiceDesenho = Dados->IndiceDesenho;
                Aresta.IDArquivo = Dados->IDArquivo;
                Arestas->Adiciona( Aresta );
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
            ligado = ligaEquipamentoSeDesligado( ListaItensCelula, ligado );
            //if ( ListaItensCelula->iTexto > 0 )
            //  VerticesGerais->getItem(IndiceVertice)->texto = Textos[ListaItensCelula->iTexto].texto;
            ListaItensCelula->cabosArcoRelacionados.push_back( ListaMenores.at( n ).IndiceCabo );
            TAresta Aresta;

            CabosArco[ListaMenores.at( n ).IndiceCabo]->ponta[ListaMenores.at( n ).IndiceArco] =
                    true;
            for (int i = 0; i < ListaItensCelula->iTextos.size(); i++)
            {
                Aresta.AdicionaVertices( iVerticesInstrumento->at( i ),
                        ListaMenores.at( n ).IndiceVertice,
                        DistPontosManhattan( PosVertice, PosVertice ) );
                Aresta.IndiceDesenho = Dados->IndiceDesenho;
                Aresta.IDArquivo = Dados->IDArquivo;
                Arestas->Adiciona( Aresta );
            }
        }
    }
}
//---------------------------------------------------------------------------

void CGrafoDesenho::GeraVerticesInstrumentosAdicionaMultipoint(int Indice,
        TListaItensCelula *ListaItensCelula, bool &ligado, TVectorInt *iVerticesInstrumento,
        TPonto PosVertice)
{
    TVectorPontoEIndiceCabo ListaMenores;
    int j;
    TMultipoint MultipointT;
    TPonto reta[2];
    MultipointT = Dados->Multipoint[Indice];
    for (j = 0; j < MultipointT.pontos.size(); j++)
    {
        reta[0] = MultipointT.pontos[j];
        if (j + 1 != MultipointT.pontos.size())
            reta[1] = MultipointT.pontos[j + 1];
        else
            reta[1] = MultipointT.pontos[0];

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

void CGrafoDesenho::GeraVerticesInstrumentosAdicionaArco(int Indice,
        TListaItensCelula *ListaItensCelula, bool &ligado, TVectorInt *iVerticesInstrumento,
        TPonto PosVertice)
{
    TVectorPontoEIndiceCabo ListaMenores;
    TArco Arco;
    int IndiceCabo;
//  double DistMaisPerto;
    Arco = Dados->Arcos[Indice];
    ListaMenores.clear();
    DistArcoParaTodasPontasRetaCabo( Arco, ListaMenores, DIST_MIN_ELEM_CABO );

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
            ligado = ligaEquipamentoSeDesligado( ListaItensCelula, ligado );
            //adiciona o vértice na lista de vértices do cabo
            CabosReta[IndiceCabo]->AdicionaVertice( VerticesGerais->Tamanho(), PosVertice );

            TVerticeGeral VerticeGeral;
            VerticeGeral.pos = PosVertice;
            VerticeGeral.IDArquivo = Dados->IDArquivo;
            VerticeGeral.iDesenho = Dados->IndiceDesenho;
            VerticeGeral.TipoElemento = INSTRUMENTO;
            VerticeGeral.TipoVertice = VERTICE_INSTRUMENTO_ARCO;
            VerticesGerais->Adiciona( VerticeGeral );

            ListaItensCelula->cabosRetaRelacionados.push_back( IndiceCabo );
            if (CabosReta[IndiceCabo]->EhOPrimeiroPonto( PosVertice, Dados->Multipoint,
                    IndiceCabo ))
            {
                CabosReta[IndiceCabo]->ponta[0] = true;
            }
            else if (CabosReta[IndiceCabo]->EhOUltimoPonto( PosVertice, Dados->Multipoint,
                    IndiceCabo ))
            {
                CabosReta[IndiceCabo]->ponta[1] = true;
            }
            TAresta Aresta;
            for (int i = 0; i < ListaItensCelula->iTextos.size(); i++)
            {
                Aresta.AdicionaVertices( iVerticesInstrumento->at( i ),
                        VerticesGerais->Tamanho() - 1,
                        //								DistPontosManhattan(PosVertice, PosVertice));
                        DistPontos( PosVertice, PosVertice ) );
                Aresta.IndiceDesenho = Dados->IndiceDesenho;
                Aresta.IDArquivo = Dados->IDArquivo;
                Arestas->Adiciona( Aresta );
            }
        }
    }
}
//---------------------------------------------------------------------------

//Preenche o vetor que armazena os vértices de cada instrumento
void CGrafoDesenho::GeraVerticesInstrumentos()
{
    int n, i;
    TListaItensCelula *ListaItensCelula;
    TItemCelula *ItemCelula;

    //  int indice2;
    //  double DistMaisPerto;
    bool PrensaCabo;
    TPonto PosVertice;
    //percorre c�lulas de instrumentos
    for (n = 0; n < Dados->InfoCelula.ListaCelulasInstrumentos->Tamanho(); n++) //percorre todas as c�lulas do desenho
    {
        bool ligado = false;
        ListaItensCelula = Dados->InfoCelula.ListaCelulasInstrumentos->getItem( n );
 
		vector<int> iVerticesInstrumento;
        PrensaCabo = ListaItensCelula->iTextos.size() > 2;
        if (ListaItensCelula->iTextos.size() == 0)
        {
            CErrosMsg *erros = CErrosMsg::getInstance();
            erros->novoErro(
                    (string) "No desenho " + ExtractFileName( Dados->NomeArq.c_str() ).c_str()
                            + " existe um grupamento em nível de equipamento sem textos associados.\n" );
            continue;
        }

        PosVertice = AchaPosVerticeInstrumento( ListaItensCelula );
        CriaVerticesEArestasInstrumento( ListaItensCelula, &iVerticesInstrumento, PosVertice,
                PrensaCabo );

        for (i = 0; i < ListaItensCelula->Tamanho(); i++)  //percorre todos os itens da c�lula atual
        {
            ItemCelula = ListaItensCelula->getItem( i );
            switch (ItemCelula->TipoVetorCW)
            //verifica o tipo do elemento do equipamento
            {
            case VMULTIPOINT:  //adiciona um vértice de um instrumento na ponta de um cabo
                GeraVerticesInstrumentosAdicionaMultipoint( ItemCelula->Indice, ListaItensCelula,
                        ligado, &iVerticesInstrumento, PosVertice );
                break;
            case VARCO:
                GeraVerticesInstrumentosAdicionaArco( ItemCelula->Indice, ListaItensCelula, ligado,
                        &iVerticesInstrumento, PosVertice );
                break;
            }  //switch (ItemCelula->TipoVetorCW)
        }  //for (i=0; i<ListaItensCelula->Tamanho(); i++)
    }
}
//---------------------------------------------------------------------------

bool CGrafoDesenho::ligaEquipamentoSeDesligado(TListaItensCelula *ListaItensCelula, bool ligado)
{
    if (!ligado)
    {
        for (int k = 0; k < ListaItensCelula->Tamanho(); k++)
        {
            if (ListaItensCelula->getItem( k )->TipoVetorCW == VMULTIPOINT)
            {
                Dados->Multipoint[ListaItensCelula->getItem( k )->Indice].Nivel = INSTRUMENTO;
            }
            if (ListaItensCelula->getItem( k )->TipoVetorCW == VARCO)
            {
                Dados->Arcos[ListaItensCelula->getItem( k )->Indice].Nivel = INSTRUMENTO;
            }
        }
        ligado = true;
    }

    return ligado;
}
//---------------------------------------------------------------------------

void CGrafoDesenho::CriaVerticesEArestasInstrumento(TListaItensCelula *ListaItensCelula,
        TVectorInt *iVerticesInstrumento, TPonto PosVertice, bool PrensaCabo)
{
    //para cada texto da c�lula cria um vértice
    for (int i = 0; i < ListaItensCelula->iTextos.size(); i++)
    {
        TVerticeGeral VerticeInstrumento;
        VerticeInstrumento.TipoElemento = INSTRUMENTO;
        VerticeInstrumento.iDesenho = Dados->IndiceDesenho;
        VerticeInstrumento.IDArquivo = Dados->IDArquivo;
        VerticeInstrumento.texto = Dados->Textos[ListaItensCelula->iTextos[i]].texto;
        VerticeInstrumento.pos = PosVertice;
        //      VerticeInstrumento.EhPrensaCabo = true;
        VerticeInstrumento.EhPrensaCabo = PrensaCabo;
        iVerticesInstrumento->push_back( VerticesGerais->Tamanho() );
        VerticeInstrumento.TipoVertice = VERTICE_CENTRO_INSTRUMENTO;
        VerticesGerais->Adiciona( VerticeInstrumento );
    }

    // Caso o equipamento seja um colar de subida/descida, então � necess�rio adicionar uma aresta entre a subida e a descida.
    if (ListaItensCelula->iTextos.size() == 2)
    {
        TAresta Aresta;
        Aresta.AdicionaVertices( iVerticesInstrumento->at( 0 ), iVerticesInstrumento->at( 1 ), 0 );
        Aresta.IndiceDesenho = Dados->IndiceDesenho;
        Aresta.IDArquivo = Dados->IDArquivo;
        Arestas->Adiciona( Aresta );
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
    for (i = 0; i < ListaItensCelula->Tamanho(); i++)
    {
        //se o item corrente da c�lula for multipoint, continua
		TItemCelula *itemCelula = ListaItensCelula->getItem( i );
        if (itemCelula->TipoVetorCW == VMULTIPOINT)
        {
            for (j = 0; j < Dados->Multipoint[itemCelula->Indice].pontos.size();
                    j++)
            {
                if (first)
                {
                    first = false;
                    maiorX = menorX =
                            Dados->Multipoint[ListaItensCelula->getItem( i )->Indice].pontos[j].x;
                    maiorY = menorY =
                            Dados->Multipoint[ListaItensCelula->getItem( i )->Indice].pontos[j].y;
                }
                if (Dados->Multipoint[ListaItensCelula->getItem( i )->Indice].pontos[j].x > maiorX)
                    maiorX = Dados->Multipoint[ListaItensCelula->getItem( i )->Indice].pontos[j].x;
                if (Dados->Multipoint[ListaItensCelula->getItem( i )->Indice].pontos[j].x < menorX)
                    menorX = Dados->Multipoint[ListaItensCelula->getItem( i )->Indice].pontos[j].x;
                if (Dados->Multipoint[ListaItensCelula->getItem( i )->Indice].pontos[j].y > maiorY)
                    maiorY = Dados->Multipoint[ListaItensCelula->getItem( i )->Indice].pontos[j].y;
                if (Dados->Multipoint[ListaItensCelula->getItem( i )->Indice].pontos[j].y < menorY)
                    menorY = Dados->Multipoint[ListaItensCelula->getItem( i )->Indice].pontos[j].y;
            }
        }     //se o item da c�lula for um arco
        else if (ListaItensCelula->getItem( i )->TipoVetorCW == VARCO)
        {
            TPonto Centro;
            double Raio;
            Centro = Dados->Arcos[ListaItensCelula->getItem( i )->Indice].Centro;
            Raio = Dados->Arcos[ListaItensCelula->getItem( i )->Indice].EixoPrimario;
            if (first)
            {
                first = false;
                maiorX = Centro.x + Raio;
                menorX = Centro.x - Raio;
                maiorY = Centro.y + Raio;
                menorY = Centro.y - Raio;
            }
            if (Centro.x + Raio > maiorX)
                maiorX = Centro.x + Raio;
            if (Centro.x + Raio > menorX)
                menorX = Centro.x - Raio;
            if (Centro.y + Raio > maiorY)
                maiorY = Centro.y + Raio;
            if (Centro.y - Raio > menorY)
                menorY = Centro.y - Raio;
        }
    }     //fim do loop que percorre os itens da c�lula
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
    TArco *Arco;
    TVerticeGeral VerticeGeral;
    VerticeGeral.TipoVertice = VERTICE_ARCO;
    int IndiceCabo, IndiceVertice, PontaArco;
    n = m = iMenorDist = IndiceCabo = IndiceVertice = 0;
    DistMaisProx = 0;
    PontoNaReta.x = PontoNaReta.y = PontoTemp.x = PontoTemp.y = p[0].x = p[0].y = p[1].x = p[1].y =
            0.0;
    for (n = 0; n < _numCabosArco; n++)
    {
        /* Olha todos os arcos do desenho */
        Arco = &Dados->Arcos[CabosArco[n]->Indice];
        Arco->PontasArco( p );

        for (m = 0; m < 2; m++)
        {
            // iV[m] � o índice do novo vértice que será criado, e como ele será o último vértice,
            // o seu índice � igual ao total atual de vértices
            Arco->iV[m] = VerticesGerais->Tamanho();

            //verifica se a ponta deste cabo arco está ligada na ponta de outro cabo arco
            //se já estiver, não cria um vértice novo, aproveita o vértice que já existe
            //no outro cabo arco
            DistPontoParaPontaCaboArco( p[m], IndiceCabo, DistMaisProx, PontoTemp, IndiceVertice, n,
                    PontaArco );
            if (DistMaisProx < DIST_MIN_ELEM_CABO)
            {
                Arco->iV[m] = IndiceVertice;
                CabosArco[n]->ponta[m] = true;
                CabosArco[IndiceCabo]->ponta[PontaArco] = true;

            }
            else //se o vértice não foi aproveitado de outro cabo arco, verifica se este vértice está próximo a um cabo reta
            {
                /* Olha cada ponta do arco */
                CaboMaisProximo( p[m], iMenorDist, DistMaisProx, PontoNaReta, -1, Arco->Nivel );
                if (DistMaisProx < DIST_MIN_ELEM_CABO)
                {
                    // Se a Distância for menor do que o limite, então esse vértice também será adicionado ao cabo mais próximo
                    CabosReta[iMenorDist]->AdicionaVertice( Arco->iV[m], PontoNaReta );
                    VerticeGeral.pos = PontoNaReta;
                    CabosArco[n]->ponta[m] = true;
                    if (CabosReta[iMenorDist]->EhOPrimeiroPonto( PontoNaReta, Dados->Multipoint,
                            iMenorDist ))
                    {
                        CabosReta[iMenorDist]->ponta[0] = true;
                    }
                    else if (CabosReta[iMenorDist]->EhOUltimoPonto( PontoNaReta, Dados->Multipoint,
                            iMenorDist ))
                    {
                        CabosReta[iMenorDist]->ponta[1] = true;
                    }
                }
                else
                    VerticeGeral.pos = p[m];

                VerticeGeral.iDesenho = Dados->IndiceDesenho;
                VerticeGeral.IDArquivo = Dados->IDArquivo;
                VerticesGerais->Adiciona( VerticeGeral );
            }
        }

        TAresta Aresta;
        Aresta.AdicionaVertices( Arco->iV[0], Arco->iV[1], DistPontosManhattan( p[0], p[1] ) );
        Aresta.IndiceDesenho = Dados->IndiceDesenho;
        Aresta.IDArquivo = Dados->IDArquivo;
        Arestas->Adiciona( Aresta );
    }
}
//---------------------------------------------------------------------------

void CGrafoDesenho::GeraVerticesPontaCabos()
{
    int n, m;
    int iMenorDist;
    double DistMaisProx;
    TPonto PontoNaReta;
    TMultipoint *tMultipoint;
    TVerticeGeral VerticeGeral;
    VerticeGeral.TipoVertice = VERTICE_PONTA_CABO;
    for (n = 0; n < _numCabosReta; n++)
    {
        /* Olha todos os cabos reta do desenho */
        tMultipoint = &Dados->Multipoint[CabosReta[n]->Indice];

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
                CabosReta[iMenorDist]->AdicionaVertice( VerticesGerais->Tamanho(), PontoNaReta );
                VerticeGeral.pos = PontoNaReta;
                //        if ( CabosReta[iMenorDist]->EhOPrimeiroPonto(PontoNaReta, Dados->Multipoint, DistMinElemCabo) )
                //          CabosReta[iMenorDist]->ponta[0] = true;
                //       if ( CabosReta[iMenorDist]->EhOUltimoPonto(PontoNaReta, Dados->Multipoint, DistMinElemCabo) )
                //          CabosReta[iMenorDist]->ponta[1] = true;
                //        CabosReta[iMenorDist]->ponta[m] = true;
                if (m == 0)
                    CabosReta[n]->ponta[0] = true;
                else
                    CabosReta[n]->ponta[1] = true;
            }
            else
                VerticeGeral.pos = tMultipoint->pontos[m];
            CabosReta[n]->AdicionaVertice( VerticesGerais->Tamanho(), tMultipoint->pontos[m] );
            VerticeGeral.iDesenho = Dados->IndiceDesenho;
            VerticeGeral.IDArquivo = Dados->IDArquivo;
            VerticesGerais->Adiciona( VerticeGeral );
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
    for (m = 0; m < _numCabosReta; m++)
    {
        // PontosCabo tem os pontos de in�cio e fim do cabo
        vector<TPonto> &PontosCabo = Dados->Multipoint[CabosReta[m]->Indice].pontos;
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
    for (m = 0; m < _numCabosReta; m++)
    {
        // PontosCabo tem os pontos de in�cio e fim do cabo
        vector<TPonto> &PontosCabo = Dados->Multipoint[CabosReta[m]->Indice].pontos;
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
    TPonto PontosCabo[2], PontoTemp;
    IndiceCabo = -1;
    MenorDist = Infinity;
    for (m = 0; m < _numCabosArco; m++)
    {
        PontosCabo[0] = VerticesGerais->getItem( Dados->Arcos[CabosArco[m]->Indice].iV[0] )->pos;
        PontosCabo[1] = VerticesGerais->getItem( Dados->Arcos[CabosArco[m]->Indice].iV[1] )->pos;
        for (n = 0; n < 2; n++)
        {
            Dist = DistPontoParaSegmentoReta( Reta, PontosCabo[n], PontoTemp );
            if (Dist < MenorDist)
            {
                MenorDist = Dist;
                IndiceCabo = m;
                PosVertice = PontosCabo[n];
                IndiceVertice = Dados->Arcos[CabosArco[m]->Indice].iV[n];
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
    TPonto PontosCabo[2], PontoTemp;
    TPontoEIndiceCabo Cabo;
    for (m = 0; m < _numCabosArco; m++)
    {
        PontosCabo[0] = VerticesGerais->getItem( Dados->Arcos[CabosArco[m]->Indice].iV[0] )->pos;
        PontosCabo[1] = VerticesGerais->getItem( Dados->Arcos[CabosArco[m]->Indice].iV[1] )->pos;
        for (n = 0; n < 2; n++)
        {
            Dist = DistPontoParaSegmentoReta( Reta, PontosCabo[n], PontoTemp );
            if (Dist < DistMinElemCabo)
            {
                Cabo.IndiceCabo = m;
                Cabo.PosVertice = PontosCabo[n];
                Cabo.IndiceVertice = Dados->Arcos[CabosArco[m]->Indice].iV[n];
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
    for (m = 0; m < _numCabosReta; m++)
    {
        vector<TPonto> &PontosCabo = Dados->Multipoint[CabosReta[m]->Indice].pontos;
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
    for (m = 0; m < _numCabosReta; m++)
    {
        vector<TPonto> &PontosCabo = Dados->Multipoint[CabosReta[m]->Indice].pontos;
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
    for (m = 0; m < _numCabosArco; m++)
    {
        Dados->Arcos[CabosArco[m]->Indice].PontasArco( PontosCabo );
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
    TPonto PontosCabo[2], PontoTemp;
    IndiceCabo = -1;
    MenorDist = Infinity;
    int menor = IndiceMax;
    if (menor > _numCabosArco)
        menor = _numCabosArco;
    for (m = 0; m < menor; m++)
    {
        PontosCabo[0] = VerticesGerais->getItem( Dados->Arcos[CabosArco[m]->Indice].iV[0] )->pos;
        PontosCabo[1] = VerticesGerais->getItem( Dados->Arcos[CabosArco[m]->Indice].iV[1] )->pos;
        for (n = 0; n < 2; n++)
        {
            Dist = DistPontos( ponto, PontosCabo[n] );
            if (Dist < MenorDist)
            {
                MenorDist = Dist;
                IndiceCabo = m;
                PosVertice = PontosCabo[n];
                pontaCabo = n;
                IndiceVertice = Dados->Arcos[CabosArco[m]->Indice].iV[n];
            }
        }  //for (n=0; n<2; n++)
    }  //for (m=0; m<NumCabosReta; m++)
    DistMaisPerto = MenorDist;
}
//---------------------------------------------------------------------------

void CGrafoDesenho::OrdenaVerticesRetas()
{
    int n;
    for (n = 0; n < _numCabosReta; n++)
    {
        if (CabosReta[n]->TipoOrientacao == VERTICAL)
        {
            sort(CabosReta[n]->VerticesReta.begin(), CabosReta[n]->VerticesReta.end(), OrdenaRetaVertical );
        }
        else
        {
            sort(CabosReta[n]->VerticesReta.begin(), CabosReta[n]->VerticesReta.end(), OrdenaRetaHorizontal );
        }
    }
}
//---------------------------------------------------------------------------

void CGrafoDesenho::GeraColares(const std::vector<TDesenho*> &ListaDesenhos)
{
    if (!CarregaGrafo)
        return;
    int n;
    vector<TVerticeGeral*> *Lista = new vector<TVerticeGeral*>();
    VerticesGerais->ListaOrd( Lista );  //gera lista ordenada
    TVerticeGeral *V1, *V2;
    for (n = 0; n < (int) (Lista->size() - 1); n++)
    {
        V1 = Lista->at( n );
        if (V1->texto != "" && !V1->EhPrensaCabo)
        {
            V2 = Lista->at( n + 1 );
            if ((V1->texto == V2->texto && !V2->EhPrensaCabo) && (V1->iDesenho != V2->iDesenho)
                    && (V1->TipoElemento == INSTRUMENTO) && (V2->TipoElemento == INSTRUMENTO))
            {
                double alturaDaAresta = ListaDesenhos[V1->iDesenho]->Altura
                        - ListaDesenhos[V2->iDesenho]->Altura;
                if (alturaDaAresta < 0)
                    alturaDaAresta *= -1;
                TAresta Aresta;
                Aresta.AdicionaVertices( V1->IndiceOriginal, V2->IndiceOriginal, alturaDaAresta );
                Aresta.IndiceDesenho = I_DESENHO_NULO;
                Aresta.IDArquivo = I_DESENHO_NULO;
                Arestas->Adiciona( Aresta );

                VerticesGerais->getItem( V1->IndiceOriginal )->EhColar = VerticesGerais->getItem(
                        V2->IndiceOriginal )->EhColar = true;
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

void CGrafoDesenho::ChecagemVerticeDuplo(const std::vector<TDesenho*> &ListaDesenhos)
{
    if (!CarregaGrafo)
        return;

    int n;
    vector<TVerticeGeral*> *Lista = new vector<TVerticeGeral*>();
    VerticesGerais->ListaOrd( Lista );  //gera lista ordenada
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
#ifdef DEBUG_BUILDER
    TStringList *proArquivo = new TStringList();
    AnsiString vertices = "";
#endif
    for (n = 0; n < _numCabosReta; n++)
    {
#ifdef DEBUG_BUILDER
        vertices += IntToStr(n) + ": ";
        VerticesReta2=CabosReta[n]->VerticesReta.getItem(0);
        vertices += " " + IntToStr(VerticesReta2->ID) + "(" + FormatFloat("###,###,###",VerticesReta2->pos.x) + " - " + FormatFloat("###,###,###",VerticesReta2->pos.y) + ")";
#endif
        for (m = 0; m < CabosReta[n]->NumVertices - 1; m++)
        {
            VerticesReta1 = &CabosReta[n]->VerticesReta[m];
            VerticesReta2 = &CabosReta[n]->VerticesReta[m + 1];
#ifdef DEBUG_BUILDER
            vertices += " " + IntToStr(VerticesReta2->ID) + "(" + FormatFloat("###,###,###",VerticesReta2->pos.x) + " - " + FormatFloat("###,###,###",VerticesReta2->pos.y) + ")";
#endif
            TAresta Aresta;
            Aresta.AdicionaVertices( VerticesReta1->ID, VerticesReta2->ID,
            //					DistPontosManhattan(VerticesReta1->pos, VerticesReta2->pos));
                    DistPontos( VerticesReta1->pos, VerticesReta2->pos ) );
            Aresta.IndiceDesenho = Dados->IndiceDesenho;
            Aresta.IDArquivo = Dados->IDArquivo;
            Arestas->Adiciona( Aresta );
        }
#ifdef DEBUG_BUILDER
        proArquivo->Add(vertices);
        vertices="";
#endif
    }
#ifdef DEBUG_BUILDER
    proArquivo->SaveToFile(ExtractFilePath(Application->ExeName) + "arestas.txt");
    delete proArquivo;
#endif
}
//---------------------------------------------------------------------------

