/*
 * InputCircuitsReader.cpp
 *
 *  Created on: 15/10/2014
 *      Author: Guilherme
 */

#include "InputCircuitsReader.h"

#include <libxls/xls.h>
#include <libxls/xlsstruct.h>
#include <iostream>
#include <sstream>
#include <string>

using namespace xls;
using namespace std;


std::string InputCircuit::getFormatedRoute() const
{
	string result;
	for( string routeItem : route )
		result += routeItem + " / ";

	return result;
}



std::vector<std::string> processRoute(string s) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, '/'))
        elems.push_back(item);

    return elems;
}



string readString( xlsCell *cell )
{
    string result;
    if( cell->str )
        result = (char*) cell->str;
    return result;
}



void loadCircuits( xlsWorkSheet *circuitsSheet, vector< InputCircuit > &circuits )
{
    for (int cellRow = 1; cellRow <= circuitsSheet->rows.lastrow; cellRow++)
    {
        InputCircuit circuit;

        xlsCell *cell = xls_cell(circuitsSheet, cellRow, 0);
        circuit.name = readString( cell );

        cell = xls_cell(circuitsSheet, cellRow, 1);
        circuit.source = readString( cell );

        cell = xls_cell(circuitsSheet, cellRow, 2);
        circuit.dest = readString( cell );

        cell = xls_cell(circuitsSheet, cellRow, 3);
        circuit.route = processRoute( readString( cell ) );

        cell = xls_cell(circuitsSheet, cellRow, 4);
        circuit.cable = readString( cell );

        circuits.push_back( circuit );
    }
}



void readInputCircuits( string fileName, vector< InputCircuit > &circuits )
{
    static string ENCODING = "UTF-8";

    xlsWorkBook *pWB;
    struct st_row_data *row;
    pWB = xls_open(fileName.c_str(), ENCODING.c_str());
	if( !pWB )
		return;

    xlsWorkSheet *circuitsSheet = xls_getWorkSheet(pWB, 0);
    xls_parseWorkSheet( circuitsSheet );
    loadCircuits( circuitsSheet, circuits );
    xls_close_WS( circuitsSheet );
}
