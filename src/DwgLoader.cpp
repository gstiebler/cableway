/*
 * DwgLoader.cpp
 *
 *  Created on: 15/09/2014
 *      Author: Guilherme
 */

#include "DwgLoader.h"

#include <stdio.h>
#include <string.h>
#include <dwg.h>

using namespace std;

void print_obj(Dwg_Object *obj);

void add_line(double x1, double y1, double x2, double y2)
{
    printf( "line: %f %f %f %f\n", x1, y1, x2, y2 );
}

void add_circle(double x, double y, double R)
{
    printf( "circle: %f %f %f\n", x, y, R );
}

void add_text(double x, double y, unsigned char *txt)
{
    printf( "text: %f %f %s\n", x, y, txt );
}

void add_group( Dwg_Object_GROUP *group )
{
    printf( "group: %d, %s\n", group->num_handles, group->str );
    for(int i(0); i < group->num_handles; ++i)
        print_obj(group->group_entries[i]->obj);
    printf("end group\n");
}

void add_insert(Dwg_Entity_INSERT *insert)
{
    printf( "insert: %d\n", insert->owned_obj_count );
    if(insert->block_header)
        print_obj( insert->block_header->obj );
    if(insert->first_attrib)
        print_obj( insert->first_attrib->obj );
    if(insert->seqend)
        print_obj( insert->seqend->obj );
    printf( "end insert\n" );
}

void add_lwpline(Dwg_Entity_LWPLINE *lwpline)
{
    printf( "lwpline: %d\n", lwpline->num_points );
    for(int i(0); i < lwpline->num_points; ++i)
        printf( "%f %f\n", lwpline->points[i].x, lwpline->points[i].y );
    printf( "end lwpline\n" );
}


void print_obj(Dwg_Object *obj)
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

    printf( "%x\n", obj->type );
    //dwg_print_object(&dwg.object[i]);

    switch (obj->type)
    {
        case DWG_TYPE_LINE:
          line = obj->tio.entity->tio.LINE;
          add_line(line->start.x, line->end.x, line->start.y, line->end.y);
          break;
        case DWG_TYPE_CIRCLE:
          circle = obj->tio.entity->tio.CIRCLE;
          add_circle(circle->center.x, circle->center.y, circle->radius);
          break;
        case DWG_TYPE_TEXT:
          text = obj->tio.entity->tio.TEXT;
          add_text(text->insertion_pt.x, text->insertion_pt.y, text->text_value);
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
   }
}


DwgLoader::DwgLoader( string fileName )
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

