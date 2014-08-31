//---------------------------------------------------------------------------


#pragma hdrstop

#include "UNiveisProjeto.h"

//---------------------------------------------------------------------------
TNiveisProjeto::TNiveisProjeto()
{
	ListaInstrumento = new TVectorString();
	ListaBandeirola = new TVectorString();
	ListaCabo = new TVectorString();
	ListaTag = new TVectorString();
}
//---------------------------------------------------------------------------
TNiveisProjeto::~TNiveisProjeto()
{
	delete ListaInstrumento;
	delete ListaBandeirola;
	delete ListaCabo;
	delete ListaTag;
}
//---------------------------------------------------------------------------
TTipoElemento TNiveisProjeto::buscaListas(string nivel)
{
	transform(nivel.begin(), nivel.end(), nivel.begin(), toupper);
	vector<string>::iterator it;
	for ( it = ListaInstrumento->begin() ; it < ListaInstrumento->end() ; it++ )
		if ( *it == nivel )
			return INSTRUMENTODESCON;
	for ( it = ListaBandeirola->begin() ; it < ListaBandeirola->end() ; it++ )
		if ( *it == nivel )
			return BANDEIROLA;
	for ( it = ListaCabo->begin() ; it < ListaCabo->end() ; it++ )
		if ( *it == nivel )
			return CABO;
	for ( it = ListaTag->begin() ; it < ListaTag->end() ; it++ )
		if ( *it == nivel )
			return TAG;

	return NADA;
}
//---------------------------------------------------------------------------
void TNiveisProjeto::exportaTransfer(TNiveisProjetoTransfer *Niveis)
{
	/*int tam = 0;
	vector<string>::iterator it;
	for ( it = ListaInstrumento->begin() ; it < ListaInstrumento->end() ; it++ )
		if ( (int) it->size() > tam )
			tam = it->size();
	for ( it = ListaBandeirola->begin() ; it < ListaBandeirola->end() ; it++ )
		if ( (int) it->size() > tam )
			tam = it->size();
	for ( it = ListaCabo->begin() ; it < ListaCabo->end() ; it++ )
		if ( (int)it->size() > tam )
			tam = it->size();
	for ( it = ListaTag->begin() ; it < ListaTag->end() ; it++ )
		if ( (int)it->size() > tam )
			tam = it->size();
	tam++;
	Niveis->tamString = tam;
	Niveis->ListaInstrumento = new char*[ListaInstrumento->size()];
	for ( int i = 0 ; i <(int) ListaInstrumento->size() ; i++ )
	{
		Niveis->ListaInstrumento[i] = new char[tam];
		memset(Niveis->ListaInstrumento[i], 0, tam);
		strcpy(Niveis->ListaInstrumento[i], ListaInstrumento->at(i).c_str());
	}
	Niveis->tamListaInstrumento = ListaInstrumento->size();

	Niveis->ListaBandeirola = new char*[ListaBandeirola->size()];
	for ( int i = 0 ; i <(int) ListaBandeirola->size() ; i++ )
	{
		Niveis->ListaBandeirola[i] = new char[tam];
		memset(Niveis->ListaBandeirola[i], 0, tam);
		strcpy(Niveis->ListaBandeirola[i], ListaBandeirola->at(i).c_str());
	}
	Niveis->tamListaBandeirola = ListaBandeirola->size();

	Niveis->ListaCabo = new char*[ListaCabo->size()];
	for ( int i = 0 ; i <(int) ListaCabo->size() ; i++ )
	{
		Niveis->ListaCabo[i] = new char[tam];
		memset(Niveis->ListaCabo[i], 0, tam);
		strcpy(Niveis->ListaCabo[i], ListaCabo->at(i).c_str());
	}
	Niveis->tamListaCabo = ListaCabo->size();

	Niveis->ListaTag = new char*[ListaTag->size()];
	for ( int i = 0 ; i <(int) ListaTag->size() ; i++ )
	{
		Niveis->ListaTag[i] = new char[tam];
		memset(Niveis->ListaTag[i], 0, tam);
		strcpy(Niveis->ListaTag[i], ListaTag->at(i).c_str());
	}
	Niveis->tamListaTag = ListaTag->size();*/

}
//---------------------------------------------------------------------------
void TNiveisProjeto::importaTransfer(TNiveisProjetoTransfer *Niveis)
{
	//string temp;
	//for ( int i = 0 ; i < Niveis->tamListaInstrumento ; i++ )
	//{
	//	temp = Niveis->ListaInstrumento[i];
	//	ListaInstrumento->push_back(temp);
	//}
	//for ( int i = 0 ; i < Niveis->tamListaBandeirola ; i++ )
	//{
	//	temp = Niveis->ListaBandeirola[i];
	//	ListaBandeirola->push_back(Niveis->ListaBandeirola[i]);
	//}
	//for ( int i = 0 ; i < Niveis->tamListaCabo ; i++ )
	//{
	//	temp = Niveis->ListaCabo[i];
	//	ListaCabo->push_back(Niveis->ListaCabo[i]);
	//}
	//for ( int i = 0 ; i < Niveis->tamListaTag ; i++ )
	//{
	//	temp = Niveis->ListaTag[i];
	//	ListaTag->push_back(Niveis->ListaTag[i]);
	//}

}
//---------------------------------------------------------------------------
void liberaTNiveisProjetoTransfer(TNiveisProjetoTransfer *Niveis)
{
	/*for ( int i = 0 ; i < Niveis->tamListaInstrumento ; i++ )
		delete[] Niveis->ListaInstrumento[i];
	delete[] Niveis->ListaInstrumento;

	for ( int i = 0 ; i < Niveis->tamListaBandeirola ; i++ )
		delete[] Niveis->ListaBandeirola[i];
	delete[] Niveis->ListaBandeirola;

	for ( int i = 0 ; i < Niveis->tamListaCabo ; i++ )
		delete[] Niveis->ListaCabo[i];
	delete[] Niveis->ListaCabo;

	for ( int i = 0 ; i < Niveis->tamListaTag ; i++ )
		delete[] Niveis->ListaTag[i];
	delete[] Niveis->ListaTag;*/
}

//---------------------------------------------------------------------------
#pragma package(smart_init)
