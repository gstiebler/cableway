/*
 * DwgLoader.h
 *
 *  Created on: 15/09/2014
 *      Author: Guilherme
 */

#ifndef DWGLOADER_H_
#define DWGLOADER_H_

#include <string>
#include <dwg.h>

class CDadosGenerico;

class DwgLoader
{
public:
    DwgLoader( std::string fileName, CDadosGenerico* dados );
    virtual ~DwgLoader();

private:

    void print_obj(Dwg_Object *obj);
    void add_block_header( Dwg_Object_BLOCK_HEADER *block_header );
    void add_lwpline(Dwg_Entity_LWPLINE *lwpline);
    void add_insert(Dwg_Entity_INSERT *insert);
    void add_group( Dwg_Object_GROUP *group);
    void add_text( Dwg_Entity_TEXT *text );
    void add_circle(Dwg_Entity_CIRCLE *circle);
    void add_line(Dwg_Entity_LINE *line);

    CDadosGenerico* _dados;
};

#endif /* DWGLOADER_H_ */
