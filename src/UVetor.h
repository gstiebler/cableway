//---------------------------------------------------------------------------

#ifndef UVetorH
#define UVetorH
//---------------------------------------------------------------------------
#include "UDefines.h"
template <typename T>
struct Vetor
{
private:
	T* lista;
	int tam;
	int ultimo;
protected:
public:
	Vetor();
	~Vetor();
	T& at(int a) { return lista[a]; };
	T& operator[]( int n ) { return lista[n]; };
	void push_back(T Item);
	int size () const;
	T* begin() { return &lista[0]; }
	T* end()   { return &lista[ultimo]; }
	void clear() { ultimo = 0; }
	T front () { return lista[0]; }
	void erase (T* foca) {  } // foca
	//  void Remove(int Indice);
	//  T * Ultimo();
	void assign (T* start, T* end) 
	{   
		ultimo = end-start;
		if ( ultimo >= tam )
		{
			do
			{
				tam = tam* VEZES;
			} while ( ultimo >= tam );

			T* tempNew = new T[tam];
			delete[] lista;
			lista = tempNew;
			tempNew = NULL;
		}
		copy(start, end, lista);
	}
};
template <typename T> Vetor<T>::Vetor()
{
	tam = TAMINICIAL;
	lista = new T[tam];
	ultimo = 0;
	//  lista=new vector<T *>();
}
//---------------------------------------------------------------------------

template <typename T> Vetor<T>::~Vetor()
{
	//  for (int n=0; n<ultimo; n++)
	//    delete lista[n];
	delete[] lista;
	lista = NULL;
	//  delete lista;
	//  lista = NULL;
}
//---------------------------------------------------------------------------

//template <typename T> void Vetor<T>::Remove(int Indice)
//{
//  lista->Delete(Indice);
//lista.erase(lista.begin()+Indice);

// IMPLEMENTAR!
//}
//---------------------------------------------------------------------------

template <typename T> void Vetor<T>::push_back(T Item)
{
	if ( ultimo >= tam )
	{
		T* tempNew = new T[tam*VEZES];
		copy(&lista[0], &lista[tam], tempNew);
		tam *= VEZES;
		delete[] lista;
		lista = tempNew;
		tempNew = NULL;
	}
	T temp(Item);
	lista[ultimo++] = temp;
}
//---------------------------------------------------------------------------

template <typename T> int Vetor<T>::size() const
{
	return ultimo;
	//return lista.size();
}
//---------------------------------------------------------------------------

//template <typename T> T * Vetor<T>::Ultimo()
//{
//  return (T *)(lista->Items[lista->Count-1]);
//return *(lista.rbegin());
//  return lista[ultimo-1];
//}
//---------------------------------------------------------------------------
//template <typename T> void Vetor<T>::CopiaListaPara(vector<T *> *cpy)
//{
//  cpy->assign(lista.begin(), lista.end());
//    for ( int i = 0 ; i < Tamanho() ; i++) //cpy->lista->Count ; i++ )
//    {
//      T *temp = new T( *(lista[i]) );
//     cpy->push_back(temp);
//    }
//}
//---------------------------------------------------------------------------
//template <typename T> void Vetor<T>::Ordena(bool (*comp_T) (T *, T *) )
//{
//  sort(&lista[0], &lista[ultimo-1], comp_T);
//}
//---------------------------------------------------------------------------
#endif
