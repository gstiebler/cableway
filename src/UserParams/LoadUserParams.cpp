
#include "LoadUserParams.h"

#include <libxls/xls.h>
#include <libxls/xlsstruct.h>
#include <cstring>
#include <string>
#include <vector>

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
    for (int cellRow = 0; cellRow <= pWS->rows.lastrow; cellRow++)
    {
        DrawingParams drawingParams;

        xlsCell *cell = xls_cell(pWS, cellRow, 0);
        drawingParams.fileName = string( (char*) cell->str );

        cell = xls_cell(pWS, cellRow, 1);
        drawingParams.elevation = cell->d;

        drawingsParams.push_back( drawingParams );
    }
}



void loadLevelsParams(xlsWorkSheet *pWS, int col, vector<int> &levels )
{

}



void loadUserParams( const std::string excelFileName, UserParams *userParams )
{
    xlsWorkBook *pWB;
    struct st_row_data *row;
    pWB = xls_open(excelFileName.c_str(), encoding.c_str());

    xlsWorkSheet *drawingsSheet = sheetByName( pWB, "desenhos" );
    xls_parseWorkSheet(drawingsSheet);
    loadDrawingParams( drawingsSheet, userParams->drawingsParams );
    xls_close_WS( drawingsSheet );

    xlsWorkSheet *levelsSheet = sheetByName( pWB, "niveis" );
    xls_parseWorkSheet(levelsSheet);
    loadLevelsParams( levelsSheet, 0, userParams->equipmentLevels );
    loadLevelsParams( levelsSheet, 1, userParams->cableLevels );
    loadLevelsParams( levelsSheet, 2, userParams->tagLevels );
    loadLevelsParams( levelsSheet, 3, userParams->bandeirolaLevels );
    xls_close_WS( levelsSheet );

    xls_close( pWB );
}
