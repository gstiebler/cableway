/*
 * UserParams.cpp
 *
 *  Created on: 13/10/2014
 *      Author: Guilherme
 */
#include "UserParams.h"

TTipoElemento UserParams::getTipoElemento( std::string level )
{
    if ( cableLevels.find( level ) != cableLevels.end() )
        return CABO;
    else if ( equipmentLevels.find( level ) != equipmentLevels.end() )
        return INSTRUMENTO;
    else if ( tagLevels.find( level ) != tagLevels.end() )
        return TAG;
    else if ( bandeirolaLevels.find( level ) != bandeirolaLevels.end() )
        return BANDEIROLA;

    return NADA;
}
