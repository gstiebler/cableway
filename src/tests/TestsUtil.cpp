/*
 * TestsUtil.cpp
 *
 *  Created on: 04/09/2014
 *      Author: Guilherme
 */

#include "TestsUtil.h"


std::string TestsUtil::exeFileName;


std::string TestsUtil::getExePath()
{
    int index = exeFileName.find_last_of( '/' );
    return exeFileName.substr( 0, index );
}
