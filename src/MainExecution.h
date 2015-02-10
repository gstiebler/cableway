/*
 * MainExecution.h
 *
 *  Created on: 15/10/2014
 *      Author: Guilherme
 */

#ifndef MAINEXECUTION_H_
#define MAINEXECUTION_H_

#include <string>
#include <vector>
#include "InputCircuitsReader.h"

class CContainerDesenhos;

struct CircuitResult
{
    std::vector<std::string> route;
    double length;
	std::string errorMessage;
};

class MainExecution
{
public:
    MainExecution( const std::string &userParametersFileName );
    virtual ~MainExecution();

    static std::string exeFileName;
    static std::string getExePath();

    void execute( std::string inputCircuitsFileName );

    CContainerDesenhos *_containerDesenhos;

	
   std::vector<InputCircuit> _inputCircuits;
   std::vector<CircuitResult> _resultCircuits;

private:

	std::string ErrosDoCircuito( std::string Origem, std::string Destino, std::vector<std::string> &route );

};

#endif /* MAINEXECUTION_H_ */
