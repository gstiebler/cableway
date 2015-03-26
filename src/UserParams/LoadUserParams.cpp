
#include "LoadUserParams.h"

#include <libxls/xls.h>
#include <libxls/xlsstruct.h>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include "UErros.h"

#include "UserParams.h"

using namespace xls;
using namespace std;

static string encoding = "UTF-8";

xlsWorkSheet* sheetByName( xlsWorkBook* pWB, const char* sheetName )
{
    for (int i = 0; i < pWB->sheets.count; i++)
    {
        if (strcmp(sheetName, (char *)pWB->sheets.sheet[i].name) == 0) {
            return xls_getWorkSheet(pWB, i);
        }
    }

    return 0;
}



void loadDrawingParams( xlsWorkSheet *pWS, vector<DrawingParams> &drawingsParams )
{
    for (int cellRow = 1; cellRow <= pWS->rows.lastrow; cellRow++)
    {
        DrawingParams drawingParams;

        xlsCell *cell = xls_cell(pWS, cellRow, 0);
        if( cell->str )
            drawingParams.fileName = string( (char*) cell->str );
        else
            break;

        cell = xls_cell(pWS, cellRow, 1);
        drawingParams.elevation = cell->d;

        drawingsParams.push_back( drawingParams );
    }
}



void loadLevelsParams(xlsWorkSheet *pWS, int col, set<string> &levels )
{
    const int STRING_FORMAT = 253;
    const int NUMBER_FORMAT = 638;
    for (int cellRow = 1; cellRow <= pWS->rows.lastrow; cellRow++)
    {
        xlsCell *cell = xls_cell(pWS, cellRow, col);
		if( !cell )
			continue;

        if( cell->id == STRING_FORMAT )
            levels.insert( string((char*)cell->str) );
        else if ( cell->id == NUMBER_FORMAT )
        {
            int value = floor( cell->d );
            char temp[256] = {};
            sprintf( temp, "%d", value );
            levels.insert( string(temp) );
        }
    }
}



void loadUserParams( const std::string excelFileName, UserParams *userParams )
{
    xlsWorkBook *pWB;
    struct st_row_data *row;
    pWB = xls_open(excelFileName.c_str(), encoding.c_str());

	if( !pWB )
	{
		CErrosMsg::getInstance()->novoErro( "Error opening xls file " + excelFileName );
        return;
	}

    xlsWorkSheet *drawingsSheet = sheetByName( pWB, "desenhos" );
	if(!drawingsSheet)
	{
		CErrosMsg::getInstance()->novoErro( "Aba \"desenhos\" não encontrada." );
        return;
	}
    xls_parseWorkSheet(drawingsSheet);
    loadDrawingParams( drawingsSheet, userParams->drawingsParams );
    xls_close_WS( drawingsSheet );

    xlsWorkSheet *levelsSheet = sheetByName( pWB, "niveis" );
	if(!levelsSheet)
	{
		CErrosMsg::getInstance()->novoErro( "Aba \"níveis\" não encontrada." );
        return;
	}
    xls_parseWorkSheet(levelsSheet);
    loadLevelsParams( levelsSheet, 0, userParams->equipmentLevels );
    loadLevelsParams( levelsSheet, 1, userParams->cableLevels );
    loadLevelsParams( levelsSheet, 2, userParams->tagLevels );
    loadLevelsParams( levelsSheet, 3, userParams->bandeirolaLevels );
    xls_close_WS( levelsSheet );

    xls_close( pWB );
}
