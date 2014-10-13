//---------------------------------------------------------------------------

#ifndef UNiveisProjetoH
#define UNiveisProjetoH
#include "UDefines.h"
#include <algorithm>
#include <vector>
#include <string>

//---------------------------------------------------------------------------
struct TNiveisProjetoTransfer
{
  std::vector<std::string> ListaInstrumento, ListaBandeirola, ListaCabo, ListaTag;
};
//---------------------------------------------------------------------------
struct TNiveisProjeto
{
  TNiveisProjeto();
  ~TNiveisProjeto();

  TVectorString *ListaInstrumento, *ListaBandeirola, *ListaCabo, *ListaTag;
  ///<summary>Como entrada tem o nome de um nível, usa essa string para buscar em todas 
  ///as listas e retorna qual � o tipo de elemento associado a esse nível.</summary>
  ///<param name="nivel">String que d� nome a um nível.</param>
  ///<returns>Um elemento do tipo TTipoElemento que pode ser Cabo, Bandeirola, Equipamento,
  ///tag ou Nada.</returns>
  TTipoElemento buscaListas(string nivel);
  ///<summary>Exporta o conte�do do objeto TNiveisProjeto para um objeto de 
  ///transfer�ncia</summary>
  ///<param name"Niveis">Objeto do tipo TNiveisProjetoTransfer para o qual será transferido
  ///os dados contidos na inst�ncia.</param>
  void exportaTransfer(TNiveisProjetoTransfer *Niveis);
  ///<summary>Importa o conte�do do objeto de transfer�ncia para a inst�ncia atual de 
  ///TNiveisProjeto</summary>
  ///<param name"Niveis">Objeto do tipo TNiveisProjetoTransfer de onde será transferido os 
  ///dados para a inst�ncia.</param>
  void importaTransfer(TNiveisProjetoTransfer *Niveis);
};
//---------------------------------------------------------------------------
///<summary>Fun��o que serve para liberar os elementos contidos em um objeto do tipo 
///TNiveisProjetoTransfer</summary>
///<param name"Niveis">Objeto do tipo TNiveisProjetoTransfer que ter� seu conte�do 
///liberado.</param>
void liberaTNiveisProjetoTransfer(TNiveisProjetoTransfer *Niveis);
//---------------------------------------------------------------------------

#endif
