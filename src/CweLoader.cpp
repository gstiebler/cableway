/*
 * DwgLoader.cpp
 *
 *  Created on: 15/09/2014
 *      Author: Guilherme
 */

#include "CweLoader.h"

#include <stdio.h>
#include <string>
#include <vector>


#include "UDadosGenerico.h"
#include "UDefines.h"
#include "UInfoCelula.h"
#include "UItemCelula.h"
#include "UListaCelulas.h"
#include "UListaItensCelula.h"
#include "UListaV.h"
#include "UserParams/UserParams.h"

using namespace std;


CweLoader::CweLoader( std::string fileName, std::shared_ptr<CDadosGenerico> dados, UserParams *userParams ) :
	_dados( dados ),
	_userParams( userParams ),
	_readFile( fileName )
{
	if (!_readFile.is_open())
	{
		printf( "Error reading file %s\n", fileName.c_str() );
		return;
	}

	readGroups();
	readElements();
}



CweLoader::~CweLoader()
{
	if (_readFile.is_open())
		_readFile.close();
}



void CweLoader::breakLine( string line, string &first, string &second )
{
	int index = line.find_first_of(':');
	first = line.substr(0, index);
	second = line.substr(index + 1, line.size());
}



bool CweLoader::readElement()
{
	string line, obj, type;
	getline(_readFile, line);
	breakLine( line, obj, type );
	if( obj != "OBJ" )
	{
		printf("Error reading obj\n");
		return false;
	}

	if( type == "TEXT")
	{
		readText();
	}
	
	return false;
}



void CweLoader::readText()
{
	string line, key, value;
	
    TTexto texto;

	while (true)
	{
		getline(_readFile, line);
		breakLine( line, key, value );
		if( key == "X" )
		{
			sscanf( value.c_str(), "%f", texto.origem.x );
		}
		else if ( key == "Y" )
		{
			sscanf( value.c_str(), "%f", texto.origem.y );
		}
		else if ( key == "END_OBJ" )
			break;
		else
			printf( "Error reading %s\n", key.c_str() );
	}

	
    _dados->Textos.push_back( texto );
}



void CweLoader::readGroups()
{
	string line;

	while( getline(_readFile, line) )
    {
		
		if( line == "END_GROUPS" )
			return;

		if( line != "OPEN_GROUP" )
		{
			printf( "Error reading groups\n" );
			return;
		}

		while (readElement());

    }
}



void CweLoader::readElements()
{
	while (readElement());
}


