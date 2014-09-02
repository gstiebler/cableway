//---------------------------------------------------------------------------
#pragma hdrstop
#include "UTPilhaDesenhos.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#include <stdio.h>

TPilhaDesenhos::TPilhaDesenhos()
{
  No=0;
  Nos.push_back(No);
}
//---------------------------------------------------------------------------

TPilhaDesenhos::~TPilhaDesenhos()
{
}                          
//---------------------------------------------------------------------------

void TPilhaDesenhos::Adiciona(string nome)
{
  string NomeArq=ExtractFileName(nome);
  PilhaArquivos.push_back(NomeArq);
  MontaString();
  No++;
  Nos.push_back(No);
  char temp[256];
  sprintf(temp, "%d - %s", No, StringPilha.c_str());
  ListaNos.push_back(temp);
}                               
//---------------------------------------------------------------------------

void TPilhaDesenhos::Retira()
{
	PilhaArquivos.pop_back();
  MontaString();
  Nos.pop_back();
  No=Nos.back();
}
//---------------------------------------------------------------------------

void TPilhaDesenhos::MontaString()
{     
  int n;
  StringPilha="";
  for (n=0; n<PilhaArquivos.size(); n++)
    StringPilha += PilhaArquivos[n] + " -> ";
  //StringPilha.substr( StringPilha.length - 2, StringPilha.length);
}
//---------------------------------------------------------------------------

string TPilhaDesenhos::StringPilhaArquivos()
{
  return StringPilha;
}
//---------------------------------------------------------------------------

int TPilhaDesenhos::GetNo()
{
  return No;
}
//---------------------------------------------------------------------------
