/*
 * DwgLoader.cpp
 *
 *  Created on: 15/09/2014
 *      Author: Guilherme
 */

#include "DwgLoader.h"

#include <stdio.h>
#include <string.h>
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

void DwgLoader::add_line(Dwg_Entity_LINE *line)
{
    //printf( "line: %f %f %f %f\n", line->start.x, line->end.x, line->start.y, line->end.y );
	if( _dados->InfoCelula.DentroCelula )
    {
        TItemCelula itemCelula;
        itemCelula.Indice = getMultipointIndex( line );
        itemCelula.TipoVetorCW = VMULTIPOINT;
		_dados->InfoCelula.ListaItensCelula->Adiciona( itemCelula );
    }
    else
    {
        if(!_insideModelSpace)
            return;

        TMultipoint multipoint;
        multipoint.Nivel = _currLayer;
        multipoint.pontos.push_back( TPonto(line->start.x, line->start.y) );
        multipoint.pontos.push_back( TPonto(line->end.x, line->end.y) );

		addMultipoint( &multipoint, line );
    }
}

void DwgLoader::add_circle(Dwg_Entity_CIRCLE *circle)
{
    //printf( "circle: %f %f %f\n", circle->center.x, circle->center.y, circle->radius );

    TArco arco;
    arco.EixoPrimario = circle->radius;
    arco.EixoSecundario = circle->radius;
    arco.Centro.x = circle->center.x;
    arco.Centro.y = circle->center.y;
    _dados->Arcos.push_back( arco );
}

void DwgLoader::add_text( Dwg_Entity_TEXT *text )
{
    if( _dados->InfoCelula.DentroCelula )
    {
        int textIndex = geTextIndex( text );
		_dados->InfoCelula.AdicionaTexto( textIndex, _dados->Textos[textIndex].texto, _dados->Textos[textIndex].Nivel );
    }
    else
    {
        if(!_insideModelSpace)
            return;

        TTexto texto;
		dwg2cwText( text, &texto );
        _dados->Textos.push_back( texto );
        _pointerToTextIndex[text] = _dados->Textos.size() - 1;
    }
}

void DwgLoader::add_group( Dwg_Object_GROUP *group )
{
    //printf( "group: %d, %s, depth: %d\n", group->num_handles, group->str, _objDepth );

    _objDepth++;
	_ASSERT( !_dados->InfoCelula.DentroCelula );
	_dados->InfoCelula.EntraCelula( 0, false );
    for(int i(0); i < group->num_handles; ++i)
        print_obj(group->group_entries[i]->obj);
	_dados->InfoCelula.FechaCelula();
    _objDepth--;

    //printf("end group %s, depth: %d\n", group->str, _objDepth);
}

void DwgLoader::add_insert(Dwg_Entity_INSERT *insert)
{
    //printf( "insert: %d, depth: %d\n", insert->owned_obj_count, _objDepth );

    _objDepth++;
    if(insert->block_header)
        print_obj( insert->block_header->obj );
    if(insert->first_attrib)
        print_obj( insert->first_attrib->obj );
    if(insert->seqend)
        print_obj( insert->seqend->obj );
    _objDepth--;

    //printf("pointer: %p\n", insert->attrib_handles);
    //printf( "end insert, depth: %d\n", _objDepth );
}

void DwgLoader::add_lwpline(Dwg_Entity_LWPLINE *lwpline)
{
    //printf( "lwpline: %d\n", lwpline->num_points );
    //for(int i(0); i < lwpline->num_points; ++i)
    //    printf( "%f %f\n", lwpline->points[i].x, lwpline->points[i].y );

    if( _dados->InfoCelula.DentroCelula )
    {
        TItemCelula itemCelula;
        itemCelula.Indice = getMultipointIndex( lwpline );
        itemCelula.TipoVetorCW = VMULTIPOINT;
        _dados->InfoCelula.ListaItensCelula->Adiciona( itemCelula );
    }
    else
    {
        if(!_insideModelSpace)
            return;

        TMultipoint multipoint;
        multipoint.Nivel = _currLayer;
        for(int i(0); i < lwpline->num_points; ++i)
        {
            multipoint.pontos.push_back( TPonto(lwpline->points[i].x, lwpline->points[i].y) );
            //printf( "%f %f\n", lwpline->points[i].x, lwpline->points[i].y );
        }

		addMultipoint( &multipoint, lwpline );
    }
}

void DwgLoader::addMultipoint(const TMultipoint *multipoint, void *line)
{
        _dados->Multipoint.push_back( *multipoint );
        _pointerToMultipointIndex[line] = _dados->Multipoint.size() - 1;
}

int DwgLoader::getMultipointIndex(Dwg_Entity_LINE *line)
{
	map<void*, int>::iterator it = _pointerToMultipointIndex.find( line );
	if( it != _pointerToMultipointIndex.end() )
		return it->second;
	else
	{
        TMultipoint multipoint;
        multipoint.Nivel = _currLayer;
        multipoint.pontos.push_back( TPonto(line->start.x, line->start.y) );
        multipoint.pontos.push_back( TPonto(line->end.x, line->end.y) );

		addMultipoint( &multipoint, line );

		return _dados->Multipoint.size() - 1;
	}
}

int DwgLoader::getMultipointIndex(Dwg_Entity_LWPLINE *lwpline)
{
	map<void*, int>::iterator it = _pointerToMultipointIndex.find( lwpline );
	if( it != _pointerToMultipointIndex.end() )
		return it->second;
	else
	{
        TMultipoint multipoint;
        multipoint.Nivel = _currLayer;
        for(int i(0); i < lwpline->num_points; ++i)
            multipoint.pontos.push_back( TPonto(lwpline->points[i].x, lwpline->points[i].y) );

		addMultipoint( &multipoint, lwpline );

		return _dados->Multipoint.size() - 1;
	}
}

int DwgLoader::geTextIndex(Dwg_Entity_TEXT *text)
{
	map<void*, int>::iterator it = _pointerToTextIndex.find( text );
	if( it != _pointerToTextIndex.end() )
		return it->second;
	else
	{
        TTexto texto;
		dwg2cwText( text, &texto );
        _dados->Textos.push_back( texto );
        _pointerToTextIndex[text] = _dados->Textos.size() - 1;

		return _dados->Textos.size() - 1;
	}
}



void DwgLoader::dwg2cwText( Dwg_Entity_TEXT *dwgText, TTexto *cwText )
{
	string textStr = string( (char*) dwgText->text_value );
	textStr = textStr.substr( 0, textStr.length() - 1 );

    cwText->Nivel = _currLayer;
    cwText->origem.x = dwgText->insertion_pt.x;
    cwText->origem.y = dwgText->insertion_pt.y;
    cwText->texto = textStr;
}



void DwgLoader::add_block_header( Dwg_Object_BLOCK_HEADER *block_header )
{
    //printf("block header: %s, %d, %d, depth: %d\n", block_header->entry_name, block_header->insert_count, block_header->owned_object_count, _objDepth);

    bool isModelSpace = !strcmp( "*Model_Space*", (char*) block_header->entry_name );

    if( isModelSpace )
        _insideModelSpace = true;

    _objDepth++;
    for(int i(0); i < block_header->owned_object_count; ++i)
        print_obj(block_header->entities[i]->obj);
    _objDepth--;

    if( isModelSpace )
        _insideModelSpace = false;

    //printf("end block header %s, depth: %d\n", block_header->entry_name, _objDepth);
}


void DwgLoader::print_obj(Dwg_Object *obj)
{
    if(!obj)
        return;

    //obj->tio.object->tio.LAYER->values
    if(obj->supertype == DWG_SUPERTYPE_ENTITY && obj->tio.entity->layer)
    {
        Dwg_Object *layer = obj->tio.entity->layer->obj;
        Dwg_Object_LAYER *LAYER = layer->tio.object->tio.LAYER;
        string strLayer = string( (char*)LAYER->entry_name );
        strLayer = strLayer.substr( 0, strLayer.length() - 1 );
        _currLayer = _userParams->getTipoElemento( strLayer );
    }
    else
        _currLayer = -1;

    Dwg_Entity_LINE *line;
    Dwg_Entity_CIRCLE *circle;
    Dwg_Entity_TEXT *text;
    Dwg_Object_GROUP *group;
    Dwg_Entity_INSERT *insert;
    Dwg_Entity_LWPLINE *lwpline;
    Dwg_Entity_BLOCK *block;
    Dwg_Object_BLOCK_HEADER *block_header;

    switch (obj->type)
    {
        case DWG_TYPE_LINE:
          line = obj->tio.entity->tio.LINE;
          add_line(line);
          break;
        case DWG_TYPE_CIRCLE:
          circle = obj->tio.entity->tio.CIRCLE;
          add_circle(circle);
          break;
        case DWG_TYPE_TEXT:
          text = obj->tio.entity->tio.TEXT;
          add_text(text);
          break;
        case DWG_TYPE_GROUP:
            add_group(obj->tio.object->tio.GROUP);
            break;

        case DWG_TYPE_INSERT:
            add_insert(obj->tio.entity->tio.INSERT);
            break;
        case DWG_TYPE_LWPLINE:
            add_lwpline(obj->tio.entity->tio.LWPLINE);
            break;
        case DWG_TYPE_BLOCK_HEADER:
            add_block_header(obj->tio.object->tio.BLOCK_HEADER);
            break;
        //default: printf( "%x\n", obj->type );
   }
}


DwgLoader::DwgLoader( string fileName, std::shared_ptr<CDadosGenerico> dados, UserParams *userParams ) :
    _dados( dados ),
    _objDepth( 0 ),
    _insideModelSpace( false ),
    _currLayer( -1 ),
    _userParams( userParams )
{
    unsigned int i;
    int success;
    Dwg_Data dwg;

    dwg.num_objects = 0;
    char filename[256];
    strcpy( filename, fileName.c_str() );
    success = dwg_read_file(filename, &dwg);

    if ( success != 0 )
	{
		printf( "Error opening file %s\n", filename );
        return;
	}

    for (i = 0; i < dwg.num_objects; i++)
        print_obj(&(dwg.object[i]));

    dwg_free(&dwg);
}

DwgLoader::~DwgLoader()
{
}

