//---------------------------------------------------------------------------
#pragma hdrstop
#include "UConfig.h"

#include "UDefines.h"
#include "UNiveisProjeto.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

using namespace std;


void ShowMessage(std::string msg)
{
}

void CConfig::CarregaBanco(TNiveisProjeto* NiveisProjeto)
{
//  TVectorString *ListaInstrumento, *ListaBandeirola, *ListaCabo, *ListaTag;
//  ListaInstrumento = NiveisProjeto->ListaInstrumento;
//  ListaBandeirola = NiveisProjeto->ListaBandeirola;
//  ListaCabo = NiveisProjeto->ListaCabo;
//  ListaTag = NiveisProjeto->ListaTag;
//
//  AnsiString valor;
////  ListaInstrumento.clear();
////  ListaBandeirola.clear();
////  ListaCabo.clear();
//
//  int projeto_id;
//
//  // ID do projeto aberto
//  projeto_id = DM_oRetorno->tbProjetoID->AsInteger;
//
//  // Pega o registro de configuração de instrumento e transforma o seu valor numa lista.
//  DM_oRetorno->qryGeral->SQL->Text="select * from configprojeto where nome='INSTRUMENTO' and projeto_id = " + IntToStr(projeto_id);
//  DM_oRetorno->qryGeral->Open();
//  GeraLista(DM_oRetorno->qryGeral->FieldByName("valor")->AsString.c_str(), ListaInstrumento);
//  DM_oRetorno->qryGeral->Close();
//  DM_oRetorno->qryGeral->SQL->Clear();
//
//  // Idem, para bandeirola
//  DM_oRetorno->qryGeral->SQL->Text="select * from configprojeto where nome='BANDEIROLA' and projeto_id = " + IntToStr(projeto_id);
//  DM_oRetorno->qryGeral->Open();
//  GeraLista(DM_oRetorno->qryGeral->FieldByName("valor")->AsString.c_str(), ListaBandeirola);
//  DM_oRetorno->qryGeral->Close();
//  DM_oRetorno->qryGeral->SQL->Clear();
//
//  // Idem, para cabo
//  DM_oRetorno->qryGeral->SQL->Text="select * from configprojeto where nome='CABO' and projeto_id = " + IntToStr(projeto_id);
//  DM_oRetorno->qryGeral->Open();
//  GeraLista(DM_oRetorno->qryGeral->FieldByName("valor")->AsString.c_str(), ListaCabo);
//  DM_oRetorno->qryGeral->Close();
//  DM_oRetorno->qryGeral->SQL->Clear();
//
//  // Idem, para tag
//  DM_oRetorno->qryGeral->SQL->Text="select * from configprojeto where nome='TAG' and projeto_id = " + IntToStr(projeto_id);
//  DM_oRetorno->qryGeral->Open();
//  GeraLista(DM_oRetorno->qryGeral->FieldByName("valor")->AsString.c_str(), ListaTag);
//  DM_oRetorno->qryGeral->Close();
//  DM_oRetorno->qryGeral->SQL->Clear();
}
//---------------------------------------------------------------------------
bool CConfig::DentroEquipamento()
{
 /* DM_oRetorno->qryGeral->SQL->Text="SELECT VALOR FROM CONFIGPROJETO WHERE NOME='DENTROEQUIPAMENTO' AND PROJETO_ID = " + DM_oRetorno->tbProjetoID->AsString;
  DM_oRetorno->qryGeral->Open();
  if ( DM_oRetorno->qryGeral->RecordCount > 0 )
  {
    if ( !strcmpi(DM_oRetorno->qryGeral->FieldByName("VALOR")->AsString.Trim().c_str(), "true") || DM_oRetorno->qryGeral->FieldByName("VALOR")->AsString.Trim() == "1" )
      return true;  
  }
  DM_oRetorno->qryGeral->Close();
  DM_oRetorno->qryGeral->SQL->Clear();
*/
  return false;
}
//---------------------------------------------------------------------------
int CConfig::NumReg()
{
  // Retorna o número de registros de configuração
  //return DM_oRetorno->NumReg("configprojeto");
    return 0;
}
//---------------------------------------------------------------------------

void CConfig::GeraLista(string ListaValores, TVectorString *lista)
{
//  size_t pos;
//  string temp;
//  while (true)
//  {
//    // Encontra a primeira ocorrência de '/' na Lista de valores..
//    pos=ListaValores.find("/");
//
//    // Se não for encontrado '/', então pára
//    if (pos==string::npos)
//      break;
//
//    // Pega o primeiro nome da lista
//    temp = ListaValores.substr(0, pos);
//
//    // Bota tudo em maiúsculo
//    transform(temp.begin(), temp.end(), temp.begin(), toupper);
//
//    // E guarda no fim da lista
//    lista->push_back(temp);
//
//    // Daí coloca a lista sem o primeiro nome e a '/'
//    ListaValores=ListaValores.substr(pos+1);
//  }
//  if ( ListaValores != "" )
//  {
//    temp = ListaValores;
////    temp = ListaValores.substr(0, ListaValores.size());
//    // Bota tudo em maiúsculo
//    transform(temp.begin(), temp.end(), temp.begin(), toupper);
//    // E bota a palavra pro fim da lista
//    lista->push_back(temp);
//  }
//  else
//  {
//    // Só chega aqui se tiver um problema no formato em que foi escrito na configuração
//    int x = 5;//ShowMessage("Por favor, verifique a sua configuração de níveis.");
//  }
}
//---------------------------------------------------------------------------


