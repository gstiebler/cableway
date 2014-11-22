/*
 * InputCircuitsReader.h
 *
 *  Created on: 15/10/2014
 *      Author: Guilherme
 */

#ifndef INPUTCIRCUITSREADER_H_
#define INPUTCIRCUITSREADER_H_

#include <string>
#include <vector>

struct InputCircuit
{
    std::string name;
    std::string source;
    std::string dest;
    std::vector<std::string> route;
    std::string cable;

	std::string getFormatedRoute() const;
};




void readInputCircuits( std::string fileName, std::vector< InputCircuit > &circuits );

#endif
