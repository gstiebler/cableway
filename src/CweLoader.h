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

class CweLoader
{
public:
    CweLoader( std::string fileName, std::shared_ptr<CDadosGenerico> dados, UserParams *userParams );
    virtual ~CweLoader();

private:

	void readGroups();
	void readElements();

	bool readElement();
	void readText();

	static void breakLine(std::string line, std::string &first, std::string &second );

	std::shared_ptr<CDadosGenerico> _dados;
    UserParams *_userParams;
	std::ifstream _readFile;
};

#endif /* CWE_LOADER_H_ */
