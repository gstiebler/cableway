/*
 * DwgLoader.h
 *
 *  Created on: 15/09/2014
 *      Author: Guilherme
 */

#ifndef CWE_LOADER_H_
#define CWE_LOADER_H_


#include <fstream>
#include <iostream>


class CDadosGenerico;
class TListaItensCelula;
struct UserParams;
class TMultipoint;
class TTexto;

#include <memory>

enum eReadElementResult { E_OK, E_CLOSE_GROUP, E_END_GROUPS, E_ERROR, E_END_FILE };

class CweLoader
{
public:
    CweLoader( std::string fileName, std::shared_ptr<CDadosGenerico> dados, UserParams *userParams );
    virtual ~CweLoader();

private:

	void readGroups();
	void readElements();

	eReadElementResult readElement();
	void readText();
	void readDBText();
	void readLine();
	void readPolyLine();
	void readCircle();
	void readArc();

	void writeError();

	static void breakLine(std::string line, std::string &first, std::string &second );
	void getKeyValue( std::string &key, std::string &value );

	std::shared_ptr<CDadosGenerico> _dados;
    UserParams *_userParams;
	std::ifstream _readFile;

	//Temporary items list of current group
	TListaItensCelula *ListaItensCelula;
};

#endif /* CWE_LOADER_H_ */
