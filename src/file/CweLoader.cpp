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
#include <math.h>

#include "UDadosGenerico.h"
#include "UDefines.h"
#include "UInfoCelula.h"
#include "UListaItensCelula.h"
#include "UserParams/UserParams.h"
#include "UErros.h"
#include "InputCircuitsReader.h"

using namespace std;


CweLoader::CweLoader( std::string fileName, std::shared_ptr<CDadosGenerico> dados, UserParams *userParams ) :
	_dados( dados ),
	_userParams( userParams ),
	_readFile( fileName ),
	ListaItensCelula( NULL )
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
	else if(line == "END_FILE")
		return E_CLOSE_GROUP;

	breakLine( line, obj, type );
	if( obj != "OBJ" )
	{
		CErrosMsg::getInstance()->novoErro( "Error reading obj " + obj );
		return E_OK;
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
	else if ( type == "ARC" )
	{
		readArc();
	}
	else
		CErrosMsg::getInstance()->novoErro( "Unsupported type " + type );
	
	return E_OK;
}



void CweLoader::readText()
{
	string line, key, value;
	
	shared_ptr<TTexto> texto = make_shared<TTexto>();

	while (true)
	{
		getline(_readFile, line);
		breakLine( line, key, value );
		if( key == "LAYER" )
		{
			texto->layerName = value;
			texto->Nivel = _userParams->getTipoElemento( value );
		}
		else if( key == "TEXT" )
		{
			texto->texto = trimString( value );
		}
		else if( key == "X" )
		{
			sscanf( value.c_str(), "%lf", &(texto->origem.x) );
		}
		else if ( key == "Y" )
		{
			sscanf( value.c_str(), "%lf", &(texto->origem.y) );
		}
		else if ( key == "WIDTH" )
		{
			sscanf( value.c_str(), "%d", &(texto->FatorAltura) );
			texto->FatorAltura /= 1000.0;
		}
		else if ( key == "ID" )
		{
			sscanf( value.c_str(), "%d", &(texto->ID) );
		}
		else if ( key == "END_OBJ" )
			break;
		else
			CErrosMsg::getInstance()->novoErro( "Error reading text " + key );
	}
	
    _dados->Textos.push_back( texto );

	if( ListaItensCelula )
    {
		ListaItensCelula->_texts.push_back( _dados->Textos.back() );
    }
}



void CweLoader::readDBText()
{
	string line, key, value;
	
    shared_ptr<TTexto> texto = make_shared<TTexto>();

	while (true)
	{
		getline(_readFile, line);
		if ( line == "END_OBJ" )
			break;

		breakLine( line, key, value );
		if( key == "LAYER" )
		{
			texto->layerName = value;
			texto->Nivel = _userParams->getTipoElemento( value );
		}
		else if( key == "TEXT" ) 
		{
			texto->texto = trimString( value );;
		}
		else if( key == "X" )
		{
			sscanf( value.c_str(), "%lf", &(texto->origem.x) );
		}
		else if ( key == "Y" )
		{
			sscanf( value.c_str(), "%lf", &(texto->origem.y) );
		}
		else if ( key == "WIDTH_FACTOR" )
		{
			sscanf( value.c_str(), "%lf", &(texto->FatorAltura) );
		}
		else if ( key == "ID" )
		{
			sscanf( value.c_str(), "%d", &(texto->ID) );
		}
		else
			CErrosMsg::getInstance()->novoErro( "Error reading " + key );
	}
	
    _dados->Textos.push_back( texto );

    if( ListaItensCelula )
    {
		ListaItensCelula->_texts.push_back( _dados->Textos.back() );
    }
}



void CweLoader::readCircle()
{
	string line, key, value;
    shared_ptr<TArco> arco = make_shared<TArco>(); 

	while (true)
	{
		getline(_readFile, line);
		if ( line == "END_OBJ" )
			break;

		breakLine( line, key, value );
		if( key == "LAYER" )
		{
			arco->layerName = value;
			arco->Nivel = _userParams->getTipoElemento( value );
		}
		else if( key == "DIAMETER" )
		{
			sscanf( value.c_str(), "%lf", &(arco->EixoPrimario) );
			arco->EixoPrimario /= 2.0;
			arco->EixoSecundario = arco->EixoPrimario;
			arco->AngIni = 0.0;
			arco->AngEnd = M_PI * 2;
		}
		else if( key == "CENTER_X" )
		{
			sscanf( value.c_str(), "%lf", &(arco->Centro.x) );

			getKeyValue( key, value );
			
			if( key != "CENTER_Y" )
				CErrosMsg::getInstance()->novoErro( "Error reading " + key );
			
			sscanf( value.c_str(), "%lf", &(arco->Centro.y) );
		}
		else if ( key == "ID" )
		{
			sscanf( value.c_str(), "%d", &(arco->ID) );
		}
		else
			CErrosMsg::getInstance()->novoErro( "Error reading " + key );
	}
	
	_dados->Arcos.push_back( arco );

	if( ListaItensCelula )
		ListaItensCelula->_arcs.push_back( _dados->Arcos.back() );
}


void CweLoader::readArc()
{
	string line, key, value;
    shared_ptr<TArco> arco = make_shared<TArco>(); 

	while (true)
	{
		getline(_readFile, line);
		if ( line == "END_OBJ" )
			break;

		breakLine( line, key, value );
		if( key == "LAYER" )
		{
			arco->layerName = value;
			arco->Nivel = _userParams->getTipoElemento( value );
		}
		else if( key == "END_POINT_X" )
		{
			//TODO
			double x, y;
			sscanf( value.c_str(), "%lf", &x );

			getKeyValue( key, value );
			
			if( key != "END_POINT_Y" )
				CErrosMsg::getInstance()->novoErro( "Error reading " + key );
			
			//TODO
			sscanf( value.c_str(), "%lf", &y );
	
			//TODO
			double difX = arco->Centro.x - x;
			double difY = arco->Centro.y - y;
			arco->EixoPrimario = sqrt( difX * difX + difY * difY );
			arco->EixoSecundario = arco->EixoPrimario;
		}
		else if( key == "CENTER_X" )
		{
			sscanf( value.c_str(), "%lf", &(arco->Centro.x) );

			getKeyValue( key, value );
			
			if( key != "CENTER_Y" )
				CErrosMsg::getInstance()->novoErro( "Error reading " + key );
			
			sscanf( value.c_str(), "%lf", &(arco->Centro.y) );
		}
		else if( key == "START_ANGLE" )
		{
			sscanf( value.c_str(), "%lf", &(arco->AngIni) );

			getKeyValue( key, value );
			
			if( key != "END_ANGLE" )
				CErrosMsg::getInstance()->novoErro( "Error reading " + key );
			
			double angEnd;
			sscanf( value.c_str(), "%lf", &(arco->AngEnd) );
		}
		else if ( key == "ID" )
		{
			sscanf( value.c_str(), "%d", &(arco->ID) );
		}
		else
			CErrosMsg::getInstance()->novoErro( "Error reading " + key );
	}
	_dados->Arcos.push_back( arco );

	if( ListaItensCelula )
		ListaItensCelula->_arcs.push_back( _dados->Arcos.back() );
}




void CweLoader::readLine()
{
	string line, key, value;
	
    shared_ptr<TMultipoint> multipoint = make_shared<TMultipoint>();

	while (true)
	{
		getline(_readFile, line);
		if ( line == "END_OBJ" )
			break;

		breakLine( line, key, value );
		if( key == "LAYER" )
		{
			multipoint->layerName = value;
			multipoint->Nivel = _userParams->getTipoElemento( value );
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

			multipoint->pontos.push_back( startPoint );
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

			multipoint->pontos.push_back( startPoint );
		}
		else if ( key == "ID" )
		{
			sscanf( value.c_str(), "%d", &(multipoint->ID) );
		}
		else
			CErrosMsg::getInstance()->novoErro( "Error reading " + key );
	}
	
    _dados->Multipoint.push_back( multipoint );

	if( ListaItensCelula )
		ListaItensCelula->_multipoints.push_back( _dados->Multipoint.back() );
}



void CweLoader::readPolyLine()
{
	string line, key, value;
	
    shared_ptr<TMultipoint> multipoint = make_shared<TMultipoint>();

	while (true)
	{
		getline(_readFile, line);
		if ( line == "END_OBJ" )
			break;

		breakLine( line, key, value );
		if( key == "LAYER" )
		{
			multipoint->layerName = value;
			multipoint->Nivel = _userParams->getTipoElemento( value );
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

				multipoint->pontos.push_back( point );
			}
		}
		else if ( key == "ID" )
		{
			sscanf( value.c_str(), "%d", &(multipoint->ID) );
		}
		else
			CErrosMsg::getInstance()->novoErro( "Error reading " + key );
	}
	
    _dados->Multipoint.push_back( multipoint );

	if( ListaItensCelula )
		ListaItensCelula->_multipoints.push_back( _dados->Multipoint.back() );
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
		
		ListaItensCelula = new TListaItensCelula();

		eReadElementResult result;
		while ( true )
		{
			result = readElement();
			if (result == E_CLOSE_GROUP)
				break;
		}
			
		_dados->InfoCelula.FechaCelula( ListaItensCelula );
		delete ListaItensCelula;
		ListaItensCelula = NULL;

		if (result == E_END_GROUPS)
			break;
    }
}



void CweLoader::readElements()
{
	while (readElement() == E_OK);
}


