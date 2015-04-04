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
#include <memory>
#include "file/InputCircuitsReader.h"

class CContainerDesenhos;
class InterfaceFeedback;

struct CircuitResult
{
    std::vector<std::string> route;
    double length;
	std::string errorMessage;
};

class MainExecution
{
public:
    MainExecution( const std::string &userParametersFileName, std::shared_ptr<InterfaceFeedback> interfaceFeedback );
    virtual ~MainExecution();

    static std::string exeFileName;
    static std::string getExePath();

    void execute( std::string inputCircuitsFileName );

    std::shared_ptr<CContainerDesenhos> _containerDesenhos;

	
   std::vector<InputCircuit> _inputCircuits;
   std::vector<CircuitResult> _resultCircuits;

private:

	std::string ErrosDoCircuito( std::string Origem, std::string Destino, std::vector<std::string> &route );
	std::shared_ptr<InterfaceFeedback> _interfaceFeedback;

};

#endif /* MAINEXECUTION_H_ */
