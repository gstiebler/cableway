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
#include "UErros.h"

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
	second = line.substr(index + 2, line.size());
}



bool CweLoader::readElement()
{
	string line, obj, type;
	getline(_readFile, line);
	if(line == "END_GROUPS")
		return false;

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
	else if ( type == "DBTEXT" )
	{
		readDBText();
	}
	else if ( type == "LINE" )
	{
		readLine();
	}
	else
		CErrosMsg::getInstance()->novoErro( "Unsupported type " + type );
	
	return true;
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
			CErrosMsg::getInstance()->novoErro( "Error reading text " + key );
	}
	
    _dados->Textos.push_back( texto );
}



void CweLoader::readDBText()
{
	string line, key, value;
	
    TTexto texto;

	while (true)
	{
		getline(_readFile, line);
		if ( line == "END_OBJ" )
			break;

		breakLine( line, key, value );
		if( key == "LAYER" )
		{
			texto.Nivel = _userParams->getTipoElemento( value );
		}
		else if( key == "TEXT" )
		{
			texto.texto = value;
		}
		else if( key == "X" )
		{
			sscanf( value.c_str(), "%lf", &(texto.origem.x) );
		}
		else if ( key == "Y" )
		{
			sscanf( value.c_str(), "%f", &(texto.origem.y) );
		}
		else if ( key == "WIDTH_FACTOR" )
		{
			sscanf( value.c_str(), "%f", &(texto.FatorAltura) );
		}
		else
			CErrosMsg::getInstance()->novoErro( "Error reading " + key );
	}
	
    _dados->Textos.push_back( texto );
}




void CweLoader::readLine()
{
	string line, key, value;
	
	
    TMultipoint multipoint;

	while (true)
	{
		getline(_readFile, line);
		if ( line == "END_OBJ" )
			break;

		breakLine( line, key, value );
		if( key == "LAYER" )
		{
			multipoint.Nivel = _userParams->getTipoElemento( value );
		}
		else if( key == "START_POINT_X" )
		{
			TPonto startPoint;
			sscanf( value.c_str(), "%lf", &(startPoint.x) );

			getline(_readFile, line);
			breakLine( line, key, value );
			
			if( key != "START_POINT_Y" )
				CErrosMsg::getInstance()->novoErro( "Error reading " + key );
			
			sscanf( value.c_str(), "%lf", &(startPoint.y) );

			multipoint.pontos.push_back( startPoint );
		}
		else if( key == "END_POINT_X" )
		{
			TPonto startPoint;
			sscanf( value.c_str(), "%lf", &(startPoint.x) );

			getline(_readFile, line);
			breakLine( line, key, value );
			
			if( key != "END_POINT_Y" )
				CErrosMsg::getInstance()->novoErro( "Error reading " + key );
			
			sscanf( value.c_str(), "%lf", &(startPoint.y) );

			multipoint.pontos.push_back( startPoint );
		}
		else
			CErrosMsg::getInstance()->novoErro( "Error reading " + key );
	}
	
    _dados->Multipoint.push_back( multipoint );
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
		
		_dados->InfoCelula.EntraCelula( 0, false );

		while (readElement());

		_dados->InfoCelula.FechaCelula();

    }
}



void CweLoader::readElements()
{
	while (readElement());
}


