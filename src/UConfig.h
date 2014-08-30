//---------------------------------------------------------------------------

#ifndef UConfigH
#define UConfigH
//---------------------------------------------------------------------------

//#include "UdmCentral.h"
//#ifndef UDadosGenericoH
//  #include "UdmORetorno.h"
//#endif
#include <vector>
#include <string>
//#include <vcl.h>

class TNiveisProjeto;

class CConfig
{
private:
public:
//  TTipoReferencia Instrumento, Bandeirola, Cabo;
  static void CarregaBanco(TNiveisProjeto* NiveisProjeto);
  static void GeraLista(std::string ListaValores, std::vector<std::string> *lista);
  static int NumReg();
  static bool DentroEquipamento();
};
//---------------------------------------------------------------------------

void ShowMessage(std::string msg);

#endif

