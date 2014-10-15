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

class CContainerDesenhos;

struct CircuitResult
{
    std::string route;
    double length;
};

class MainExecution
{
public:
    MainExecution( std::string userParametersFileName );
    virtual ~MainExecution();

    std::vector<CircuitResult> execute( std::string inputCircuitsFileName );

private:

    CContainerDesenhos *_containerDesenhos;

};

#endif /* MAINEXECUTION_H_ */
