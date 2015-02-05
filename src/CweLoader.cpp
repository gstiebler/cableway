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
		CErrosMsg::getInstance()->novoErro( "Error reading file " + fileName );
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



void CweLoader::getKeyValue( std::string &key, std::string &value )
{
	string line;
	getline(_readFile, line);
	breakLine( line, key, value );
}



eReadElementResult CweLoader::readElement()
{
	string line, obj, type;
	getline(_readFile, line);
	if(line == "END_GROUPS")
		return E_END_GROUPS;
	else if(line == "CLOSE_GROUP")
		return E_CLOSE_GROUP;

	breakLine( line, obj, type );
	if( obj != "OBJ" )
	{
		CErrosMsg::getInstance()->novoErro( "Error reading obj" );
		return E_ERROR;
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
	else if ( type == "PLINE" )
	{
		readPolyLine();
	}
	else if ( type == "CIRCLE" )
	{
		readCircle();
	}
	else
		CErrosMsg::getInstance()->novoErro( "Unsupported type " + type );
	
	return E_OK;
}



void CweLoader::readText()
{
	string line, key, value;
	
    TTexto texto;

	while (true)
	{
		getline(_readFile, line);
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
			sscanf( value.c_str(), "%lf", &(texto.origem.y) );
		}
		else if ( key == "WIDTH" )
		{
			sscanf( value.c_str(), "%d", &(texto.FatorAltura) );
			texto.FatorAltura /= 1000.0;
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



void CweLoader::readCircle()
{
	string line, key, value;
    TArco arco; 

	while (true)
	{
		getline(_readFile, line);
		if ( line == "END_OBJ" )
			break;

		breakLine( line, key, value );
		if( key == "LAYER" )
		{
			arco.Nivel = _userParams->getTipoElemento( value );
		}
		else if( key == "DIAMETER" )
		{
			sscanf( value.c_str(), "%lf", &(arco.EixoPrimario) );
			arco.EixoSecundario = arco.EixoPrimario;
		}
		else if( key == "CENTER_X" )
		{
			sscanf( value.c_str(), "%lf", &(arco.Centro.x) );

			getKeyValue( key, value );
			
			if( key != "CENTER_Y" )
				CErrosMsg::getInstance()->novoErro( "Error reading " + key );
			
			sscanf( value.c_str(), "%lf", &(arco.Centro.y) );
		}
		else
			CErrosMsg::getInstance()->novoErro( "Error reading " + key );
	}
	
	_dados->Arcos.push_back( arco );
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



void CweLoader::readPolyLine()
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
		else if ( key == "NUM_VERTEX" )
		{
			int numVertex;	
			sscanf( value.c_str(), "%d", &numVertex );

			for( int i = 0; i < numVertex; ++i )
			{
				TPonto point; 

				getKeyValue( key, value );
				if( key != "X" )
					CErrosMsg::getInstance()->novoErro( "Error reading " + key );
				sscanf( value.c_str(), "%lf", &(point.x) );

				getKeyValue( key, value );
				if( key != "Y" )
					CErrosMsg::getInstance()->novoErro( "Error reading " + key );
				sscanf( value.c_str(), "%lf", &(point.y) );

				multipoint.pontos.push_back( point );
			}
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

		eReadElementResult result;
		while ( true )
		{
			result = readElement();
			if (result == E_CLOSE_GROUP)
				break;
		}
			
		_dados->InfoCelula.FechaCelula();

		if (result == E_END_GROUPS)
			break;
    }
}



void CweLoader::readElements()
{
	while (readElement() == E_OK);
}


