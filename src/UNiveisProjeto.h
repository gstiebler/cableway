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
  ///as listas e retorna qual é o tipo de elemento associado a esse nível.</summary>
  ///<param name="nivel">String que dá nome a um nível.</param>
  ///<returns>Um elemento do tipo TTipoElemento que pode ser Cabo, Bandeirola, Equipamento,
  ///tag ou Nada.</returns>
  TTipoElemento buscaListas(string nivel);
  ///<summary>Exporta o conteúdo do objeto TNiveisProjeto para um objeto de 
  ///transferência</summary>
  ///<param name"Niveis">Objeto do tipo TNiveisProjetoTransfer para o qual será transferido
  ///os dados contidos na instância.</param>
  void exportaTransfer(TNiveisProjetoTransfer *Niveis);
  ///<summary>Importa o conteúdo do objeto de transferência para a instância atual de 
  ///TNiveisProjeto</summary>
  ///<param name"Niveis">Objeto do tipo TNiveisProjetoTransfer de onde será transferido os 
  ///dados para a instância.</param>
  void importaTransfer(TNiveisProjetoTransfer *Niveis);
};
//---------------------------------------------------------------------------
///<summary>Função que serve para liberar os elementos contidos em um objeto do tipo 
///TNiveisProjetoTransfer</summary>
///<param name"Niveis">Objeto do tipo TNiveisProjetoTransfer que terá seu conteúdo 
///liberado.</param>
void liberaTNiveisProjetoTransfer(TNiveisProjetoTransfer *Niveis);
//---------------------------------------------------------------------------

#endif
