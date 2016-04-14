//---------------------------------------------------------------------------

#ifndef UDefinesH
#define UDefinesH
//---------------------------------------------------------------------------
#include <vector>
#include <string>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define DIST_MIN_ELEM_CABO 0.1
enum TTipoVertice {VERTICE_BANDEIROLA, VERTICE_ARCO, VERTICE_PONTA_CABO,
    VERTICE_INSTRUMENTO_RETA, VERTICE_INSTRUMENTO_ARCO, VERTICE_CENTRO_INSTRUMENTO};

enum TTipoVetorCW {VNADA, VARCO, VMULTIPOINT};

typedef std::vector<int> TVectorInt;
typedef std::vector<std::string> TVectorString;

//para informar se um tipo de elemento é definido por cor ou nivel
enum TTipoReferencia {NIVEL, COR};
// Informa os tipos de elemento
enum TTipoElemento {NADA, CABO, BANDEIROLA, ESTRUTURA, INSTRUMENTO, TAG, INSTRUMENTODESCON};

std::string ExtractFileName(std::string path);

//---------------------------------------------------------------------------
#endif
