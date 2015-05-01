#ifndef __REPORTS_H
#define __REPORTS_H

#include <string>
#include "file/InputCircuitsReader.h"
#include "MainExecution.h"

class Reports
{
public:
	static void generateCirtuitsReport( std::string fileName, const std::vector<InputCircuit> &inputCircuits, const std::vector<CircuitResult> &resultCircuits );
	static void generateBandeirolaReport( std::string fileName, const std::vector<InputCircuit> &inputCircuits, const std::vector<CircuitResult> &resultCircuits );
	static void generateCableReport( std::string fileName, const std::vector<InputCircuit> &inputCircuits, const std::vector<CircuitResult> &resultCircuits );
};

#endif