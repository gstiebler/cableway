/*
 * TestsUtil.cpp
 *
 *  Created on: 04/09/2014
 *      Author: Guilherme
 */

#include "TestsUtil.h"
#include <Shlwapi.h>


std::string TestsUtil::exeFileName;

void TestsUtil::initExeFileName()
{
	HMODULE hModule = GetModuleHandleW(NULL);
	WCHAR path[MAX_PATH];
	GetModuleFileNameW(hModule, path, MAX_PATH);

	char ch[260];
    char DefChar = ' ';
    WideCharToMultiByte(CP_ACP,0, path,-1, ch,260,&DefChar, NULL);

    exeFileName = std::string( ch );
}


std::string TestsUtil::getExePath()
{
    int index = exeFileName.find_last_of( '\\' );
    return exeFileName.substr( 0, index );
}

std::string TestsUtil::getDataPath()
{
	return getExePath() + "\\..\\..\\data";
}
