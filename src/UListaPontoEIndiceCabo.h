//---------------------------------------------------------------------------

#ifndef UListaPontoEIndiceCaboH
#define UListaPontoEIndiceCaboH
//---------------------------------------------------------------------------
#endif

#include "UDefines.h"
#include "UListaV.h"

struct TPontoEIndiceCabo
{
  int IndiceCabo;
  TPonto PosVertice;
  int IndiceVertice;
  int IndiceArco;
  double Dist;
};

struct TVectorPontoEIndiceCabo
{
private:
  //vector<T *> lista;
  TPontoEIndiceCabo ** lista;
  int tam;
  int ultimo;
protected:
public:
    TVectorPontoEIndiceCabo();
    ~TVectorPontoEIndiceCabo();
  TPontoEIndiceCabo * getItem(int Indice) const;
  void Adiciona(TPontoEIndiceCabo Item);
  void Adiciona(TPontoEIndiceCabo *Item);
  void Remove(int Indice);
  void Limpa();
  int Tamanho() const;
  TPontoEIndiceCabo * Ultimo();
};
//---------------------------------------------------------------------------

TVectorPontoEIndiceCabo::TVectorPontoEIndiceCabo()
{
  tam = TAMINICIAL;
  lista = new TPontoEIndiceCabo*[tam];
  ultimo = 0;
//  lista=new vector<T *>();
}
//---------------------------------------------------------------------------

TVectorPontoEIndiceCabo::~TVectorPontoEIndiceCabo()
{
  for (int n=0; n<ultimo; n++)
    delete lista[n];
  delete[] lista;
  lista = NULL;
//  delete lista;
//  lista = NULL;
}
//---------------------------------------------------------------------------

//TPontoEIndiceCabo* TVectorPontoEIndiceCabo::getItem(int Indice) const
//{
////  return (T *)(lista->Items[Indice]);
//  return lista[Indice];
//}
//---------------------------------------------------------------------------
//
//template <typename T> void TListaV<T>::Remove(int Indice)
//{
//  if ( Indice > ultimo )
//    return;
//
//  for ( int i = Indice ; i < ultimo-1 ; i++ )
//    lista[i] = lista[i+1];
//    
//  ultimo--;
//}
////---------------------------------------------------------------------------
//
//template <typename T> void TListaV<T>::Limpa()
//{
//  ultimo=0;
//}
////---------------------------------------------------------------------------
//
//template <typename T> void TListaV<T>::Adiciona(T Item)
//{
//  if ( ultimo >= tam )
//  {
//    T** tempNew = new T*[tam*VEZES];
////    for ( int i = 0 ; i < tam ; i++ )
////      tempNew[i] = lista[i];
//    copy(&lista[0], &lista[tam], tempNew);
//    tam *= VEZES;
//    delete[] lista;
//    lista = tempNew;
//    tempNew = NULL;
//  }
//  T *temp;
//  temp=new T(Item);
////  *temp=Item;
//  lista[ultimo] = temp;
//  ultimo++;
//}
////---------------------------------------------------------------------------
//template <typename T> void TListaV<T>::Adiciona(T *Item)
//{
//  if ( ultimo >= tam )
//  {
//    T** tempNew = new T*[tam*VEZES];
////    for ( int i = 0 ; i < tam ; i++ )
////      tempNew[i] = lista[i];
//    copy(&lista[0], &lista[tam], tempNew);
//    tam *= VEZES;
//    delete[] lista;
//    lista = tempNew;
//    tempNew = NULL;
//  }
//  T *temp=new T(*Item);
////  *temp=Item;
////  lista.push_back(temp);
//  lista[ultimo] = temp;
//  ultimo++;
//}
////---------------------------------------------------------------------------
//
//template <typename T> int TListaV<T>::Tamanho() const
//{
//  return ultimo;
//  //return lista.size();
//}
////---------------------------------------------------------------------------
//
//template <typename T> T * TListaV<T>::Ultimo()
//{
////  return (T *)(lista->Items[lista->Count-1]);
//  //return *(lista.rbegin());
//  return lista[ultimo-1];
//}
////---------------------------------------------------------------------------
//template <typename T> void TListaV<T>::CopiaListaPara(vector<T *> *cpy)
//{
////  cpy->assign(lista.begin(), lista.end());
//    for ( int i = 0 ; i < Tamanho() ; i++) //cpy->lista->Count ; i++ )
//    {
//      T *temp = new T( *(lista[i]) );
//     cpy->push_back(temp);
////     delete temp;
//    }
//}
////---------------------------------------------------------------------------
//template <typename T> void TListaV<T>::Ordena(bool (*comp_T) (T *, T *) )
//{
//  sort(&lista[0], &lista[ultimo], comp_T);
//}
////---------------------------------------------------------------------------
