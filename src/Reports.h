#ifndef __REPORTS_H
#define __REPORTS_H

#include <string>
#include "InputCircuitsReader.h"
#include "MainExecution.h"

class Reports
{
public:
	static void generateCirtuisReport( std::string fileName, const std::vector<InputCircuit> &inputCircuits, const std::vector<CircuitResult> &resultCircuits );
};

#endif