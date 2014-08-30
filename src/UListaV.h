//---------------------------------------------------------------------------

#ifndef UListaVH
#define UListaVH
//---------------------------------------------------------------------------

#include <vector>
#include "UDefines.h"
using namespace std;

template <typename T>
struct TListaV
{
private:
  //vector<T *> lista;
  T** lista;
  int tam;
  int ultimo;
protected:
public:
  TListaV();
  ~TListaV();
  T * getItem(int Indice) const;
  void Adiciona(T Item);
  void Adiciona(T *Item);
  void Remove(int Indice);
  int Tamanho() const;
  T * Ultimo();
  void assign (T** start, T** end) { copy(start, end, lista); ultimo = end-start; }
  void CopiaListaPara(vector<T *> *cpy);
  void Ordena(bool (*comp_T) (T *, T *) );
};
//---------------------------------------------------------------------------

template <typename T> TListaV<T>::TListaV()
{
  tam = TAMINICIAL;
  lista = new T*[tam];
  ultimo = 0;
//  lista=new vector<T *>();
}
//---------------------------------------------------------------------------

template <typename T> TListaV<T>::~TListaV()
{
  for (int n=0; n<ultimo; n++)
    delete lista[n];
  delete[] lista;
  lista = NULL;
//  delete lista;
//  lista = NULL;
}
//---------------------------------------------------------------------------

template <typename T> T * TListaV<T>::getItem(int Indice) const
{
//  return (T *)(lista->Items[Indice]);
  return lista[Indice];
}
//---------------------------------------------------------------------------

template <typename T> void TListaV<T>::Remove(int Indice)
{
  if ( Indice > ultimo )
    return;

  delete lista[Indice];

  for ( int i = Indice ; i < ultimo-1 ; i++ )
    lista[i] = lista[i+1];
    
  ultimo--;
}
//---------------------------------------------------------------------------

template <typename T> void TListaV<T>::Adiciona(T Item)
{
  if ( ultimo >= tam )
  {
    T** tempNew = new T*[tam*VEZES];
//    for ( int i = 0 ; i < tam ; i++ )
//      tempNew[i] = lista[i];
    copy(&lista[0], &lista[tam], tempNew);
    tam *= VEZES;
    delete[] lista;
    lista = tempNew;
    tempNew = NULL;
  }
  T *temp;
  temp=new T(Item);
//  *temp=Item;
  lista[ultimo] = temp;
  ultimo++;
}
//---------------------------------------------------------------------------
template <typename T> void TListaV<T>::Adiciona(T *Item)
{
  if ( ultimo >= tam )
  {
    T** tempNew = new T*[tam*VEZES];
//    for ( int i = 0 ; i < tam ; i++ )
//      tempNew[i] = lista[i];
    copy(&lista[0], &lista[tam], tempNew);
    tam *= VEZES;
    delete[] lista;
    lista = tempNew;
    tempNew = NULL;
  }
  T *temp=new T(*Item);
//  *temp=Item;
//  lista.push_back(temp);
  lista[ultimo] = temp;
  ultimo++;
}
//---------------------------------------------------------------------------

template <typename T> int TListaV<T>::Tamanho() const
{
  return ultimo;
  //return lista.size();
}
//---------------------------------------------------------------------------

template <typename T> T * TListaV<T>::Ultimo()
{
//  return (T *)(lista->Items[lista->Count-1]);
  //return *(lista.rbegin());
  return lista[ultimo-1];
}
//---------------------------------------------------------------------------
template <typename T> void TListaV<T>::CopiaListaPara(vector<T *> *cpy)
{
//  cpy->assign(lista.begin(), lista.end());
    for ( int i = 0 ; i < Tamanho() ; i++) //cpy->lista->Count ; i++ )
    {
      T *temp = new T( *(lista[i]) );
     cpy->push_back(temp);
//     delete temp;
    }
}
//---------------------------------------------------------------------------
template <typename T> void TListaV<T>::Ordena(bool (*comp_T) (T *, T *) )
{
  sort(&lista[0], &lista[ultimo], comp_T);
}
//---------------------------------------------------------------------------

#endif
