/*
 * DwgLoader.h
 *
 *  Created on: 15/09/2014
 *      Author: Guilherme
 */

#ifndef DWGLOADER_H_
#define DWGLOADER_H_

#include <string>

class DwgLoader
{
public:
    DwgLoader( std::string fileName );
    virtual ~DwgLoader();
};

#endif /* DWGLOADER_H_ */
