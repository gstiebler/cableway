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

using namespace std;

void DwgLoader::add_line(Dwg_Entity_LINE *line)
{
    printf( "line: %f %f %f %f\n", line->start.x, line->end.x, line->start.y, line->end.y );
    if( _currCell )
    {
        TItemCelula itemCelula;
        itemCelula.Indice = _pointerToMultipointIndex[line];
        itemCelula.TipoVetorCW = VMULTIPOINT;
        _currCell->Adiciona( itemCelula );
    }
    else
    {
        if(!_insideModelSpace)
            return;

        TMultipoint multipoint;
         multipoint.pontos.push_back( TPonto(line->start.x, line->start.y) );
         multipoint.pontos.push_back( TPonto(line->end.x, line->end.y) );
        _dados->Multipoint.push_back( multipoint );

        _pointerToMultipointIndex[line] = _dados->Multipoint.size() - 1;
    }
}

void DwgLoader::add_circle(Dwg_Entity_CIRCLE *circle)
{
    printf( "circle: %f %f %f\n", circle->center.x, circle->center.y, circle->radius );

    TArco arco;
    arco.EixoPrimario = circle->radius;
    arco.EixoSecundario = circle->radius;
    arco.Centro.x = circle->center.x;
    arco.Centro.y = circle->center.y;
    _dados->Arcos.push_back( arco );
}

void DwgLoader::add_text( Dwg_Entity_TEXT *text )
{
    printf( "text: %f %f %s\n", text->insertion_pt.x, text->insertion_pt.y, text->text_value );
    if( _currCell )
    {
        int textIndex = _pointerToTextIndex[text];
        _currCell->iTextos.push_back( textIndex );
    }
    else
    {
        if(!_insideModelSpace)
            return;

        TTexto texto;
        texto.origem.x = text->insertion_pt.x;
        texto.origem.y = text->insertion_pt.y;
        texto.texto = string( (char*) text->text_value );
        _dados->Textos.push_back( texto );
        _pointerToTextIndex[text] = _dados->Textos.size() - 1;
    }
}

void DwgLoader::add_group( Dwg_Object_GROUP *group )
{
    printf( "group: %d, %s, depth: %d\n", group->num_handles, group->str, _objDepth );

    _objDepth++;
    _currCell = new TListaItensCelula();
    for(int i(0); i < group->num_handles; ++i)
        print_obj(group->group_entries[i]->obj);
    _dados->InfoCelula.ListaCelulasInstrumentos->Adiciona( *_currCell );
    delete _currCell;
    _currCell = NULL;
    _objDepth--;

    printf("end group %s, depth: %d\n", group->str, _objDepth);
}

void DwgLoader::add_insert(Dwg_Entity_INSERT *insert)
{
    printf( "insert: %d, depth: %d\n", insert->owned_obj_count, _objDepth );

    _objDepth++;
    if(insert->block_header)
        print_obj( insert->block_header->obj );
    if(insert->first_attrib)
        print_obj( insert->first_attrib->obj );
    if(insert->seqend)
        print_obj( insert->seqend->obj );
    _objDepth--;

    printf("pointer: %p\n", insert->attrib_handles);
    printf( "end insert, depth: %d\n", _objDepth );
}

void DwgLoader::add_lwpline(Dwg_Entity_LWPLINE *lwpline)
{
    printf( "lwpline: %d\n", lwpline->num_points );
    for(int i(0); i < lwpline->num_points; ++i)
        printf( "%f %f\n", lwpline->points[i].x, lwpline->points[i].y );

    if( _currCell )
    {
        TItemCelula itemCelula;
        itemCelula.Indice = _pointerToMultipointIndex[lwpline];
        itemCelula.TipoVetorCW = VMULTIPOINT;
        _currCell->Adiciona( itemCelula );
    }
    else
    {
        if(!_insideModelSpace)
            return;

        TMultipoint multipoint;
        for(int i(0); i < lwpline->num_points; ++i)
        {
            multipoint.pontos.push_back( TPonto(lwpline->points[i].x, lwpline->points[i].y) );
            //printf( "%f %f\n", lwpline->points[i].x, lwpline->points[i].y );
        }
        _dados->Multipoint.push_back( multipoint );

        printf( "end lwpline\n" );

        _pointerToMultipointIndex[lwpline] = _dados->Multipoint.size() - 1;
    }
}

void DwgLoader::add_block_header( Dwg_Object_BLOCK_HEADER *block_header )
{
    printf("block header: %s, %d, %d, depth: %d\n", block_header->entry_name, block_header->insert_count, block_header->owned_object_count, _objDepth);

    bool isModelSpace = !strcmp( "*Model_Space*", (char*) block_header->entry_name );

    if( isModelSpace )
        _insideModelSpace = true;

    _objDepth++;
    for(int i(0); i < block_header->owned_object_count; ++i)
        print_obj(block_header->entities[i]->obj);
    _objDepth--;

    if( isModelSpace )
        _insideModelSpace = false;

    printf("end block header %s, depth: %d\n", block_header->entry_name, _objDepth);
}


void DwgLoader::print_obj(Dwg_Object *obj)
{
    if(!obj)
        return;

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
        default: printf( "%x\n", obj->type );
   }
}


DwgLoader::DwgLoader( string fileName, CDadosGenerico* dados ) :
    _dados( dados ),
    _currCell( NULL ),
    _objDepth( 0 ),
    _insideModelSpace( false )
{
    unsigned int i;
    int success;
    Dwg_Data dwg;

    dwg.num_objects = 0;
    char filename[256];
    strcpy( filename, fileName.c_str() );
    success = dwg_read_file(filename, &dwg);
    printf( "%d %s\n", success, filename );
    for (i = 0; i < dwg.num_objects; i++)
    {
        print_obj(&(dwg.object[i]));
    }
    dwg_free(&dwg);
}

DwgLoader::~DwgLoader()
{
}

