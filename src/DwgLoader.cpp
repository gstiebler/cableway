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
#include "UItemCelula.h"
#include "UListaItensCelula.h"
#include "UListaV.h"

using namespace std;

void DwgLoader::add_line(Dwg_Entity_LINE *line)
{
    printf( "line: %f %f %f %f\n", line->start.x, line->end.x, line->start.y, line->end.y );

    TMultipoint multipoint;
     multipoint.pontos.push_back( TPonto(line->start.x, line->start.y) );
     multipoint.pontos.push_back( TPonto(line->end.x, line->end.y) );
    _dados->Multipoint.push_back( multipoint );

    if( _currCell )
    {
        TItemCelula itemCelula;
        itemCelula.Indice = _pointerToMultipointIndex[line];
        itemCelula.TipoVetorCW = VMULTIPOINT;
        _currCell->Adiciona( itemCelula );
    }
    else
        _pointerToMultipointIndex[line] = _dados->Multipoint.size() - 1;
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
    TTexto texto;
    texto.origem.x = text->insertion_pt.x;
    texto.origem.y = text->insertion_pt.y;
    texto.texto = string( (char*) text->text_value );
    _dados->Textos.push_back( texto );

    if( _currCell )
    {
        int textIndex = _pointerToTextIndex[text];
        _currCell->iTextos.push_back( textIndex );
    }
    else
        _pointerToTextIndex[text] = _dados->Textos.size() - 1;
}

void DwgLoader::add_group( Dwg_Object_GROUP *group )
{
    printf( "group: %d, %s\n", group->num_handles, group->str );
    for(int i(0); i < group->num_handles; ++i)
        print_obj(group->group_entries[i]->obj);
    printf("end group\n");
}

void DwgLoader::add_insert(Dwg_Entity_INSERT *insert)
{
    printf( "insert: %d\n", insert->owned_obj_count );
    if(insert->block_header)
        print_obj( insert->block_header->obj );
    if(insert->first_attrib)
        print_obj( insert->first_attrib->obj );
    if(insert->seqend)
        print_obj( insert->seqend->obj );
    printf("pointer: %p\n", insert->attrib_handles);
    printf( "end insert\n" );
}

void DwgLoader::add_lwpline(Dwg_Entity_LWPLINE *lwpline)
{
    printf( "lwpline: %d\n", lwpline->num_points );
    TMultipoint multipoint;
    for(int i(0); i < lwpline->num_points; ++i)
    {
        multipoint.pontos.push_back( TPonto(lwpline->points[i].x, lwpline->points[i].y) );
        printf( "%f %f\n", lwpline->points[i].x, lwpline->points[i].y );
    }
    _dados->Multipoint.push_back( multipoint );


    if( _currCell )
    {
        TItemCelula itemCelula;
        itemCelula.Indice = _pointerToMultipointIndex[lwpline];
        itemCelula.TipoVetorCW = VMULTIPOINT;
        _currCell->Adiciona( itemCelula );
    }
    else
        _pointerToMultipointIndex[lwpline] = _dados->Multipoint.size() - 1;

    printf( "end lwpline\n" );
}

void DwgLoader::add_block_header( Dwg_Object_BLOCK_HEADER *block_header )
{
    printf("block header: %s, %d, %d\n", block_header->entry_name, block_header->insert_count, block_header->owned_object_count);

    _currCell = new TListaItensCelula();
    for(int i(0); i < block_header->owned_object_count; ++i)
        print_obj(block_header->entities[i]->obj);
    delete _currCell;
    _currCell = NULL;


    printf("end block header\n");
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

    //dwg_print_object(&dwg.object[i]);

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
    _currCell( NULL )
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

