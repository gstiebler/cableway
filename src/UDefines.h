//---------------------------------------------------------------------------

#ifndef UDefinesH
#define UDefinesH
//---------------------------------------------------------------------------
#include <vector>
#include <string>
//#include "UVetor.h"
using namespace std;
#define NUM_CORES 64

#define FATOR_DIST_MIN_ELEM_CABO 0.1
#define I_DESENHO_NULO -123456
#define NAO_INICIALIZADO -1
//#define DIST_CONVESES 3
#define TAMINICIAL 64
#define VEZES 2
#define FATOR_FONTE 0.6
#ifndef MINLONG
#define MINLONG 0x80000000
#endif
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
//#define DEBUG_BUILDER

#define DIST_MIN_ELEM_CABO 0.1

#define LINE_CLOSED 0
#define LINE_OPEN 1

enum TTipoVertice {VERTICE_BANDEIROLA, VERTICE_ARCO, VERTICE_PONTA_CABO,
    VERTICE_INSTRUMENTO_RETA, VERTICE_INSTRUMENTO_ARCO, VERTICE_CENTRO_INSTRUMENTO};

//#define BUILDER6

enum TTipoVetorCW {VNADA, VARCO, VMULTIPOINT};

//---------------------------------------------------------------------------
#define NUM_NIVEIS 64
typedef vector<int> TVectorInt;
typedef vector<string> TVectorString;

//para informar se um tipo de elemento é definido por cor ou nivel
enum TTipoReferencia {NIVEL, COR};
// Informa os tipos de elemento
enum TTipoElemento {NADA, CABO, BANDEIROLA, ESTRUTURA, CALHA, INSTRUMENTO, TAG, INSTRUMENTODESCON};

//---------------------------------------------------------------------------
struct TPonto
{
	double x, y;

	TPonto( double X, double Y ) :
		x( X ),
		y( Y )
	{}

	TPonto() {}
};

struct TReta
{
  TPonto pontos[2];
};

std::string ExtractFileName(std::string path);

#define Infinity 9999999999999999.9999


#define TStringList vector<std::string>

//---------------------------------------------------------------------------
#endif
