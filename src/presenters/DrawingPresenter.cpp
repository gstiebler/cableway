/*
 * DrawingPresenter.cpp
 *
 *  Created on: 19/10/2014
 *      Author: Guilherme
 */

#include "DrawingPresenter.h"
#include "UMostraDesenho.h"

#include <interface/DrawingWindow.h>

DrawingPresenter::DrawingPresenter(CGrafoDesenho *grafoDesenho, CInfoCircuitos *infoCircuitos, int circuitIndex )
{
    _window = new DrawingWindow(grafoDesenho, infoCircuitos);
    _window->show();

	if( circuitIndex >= 0 )
		_window->_mostraDesenho->MostraCircuito( circuitIndex );

    connect( _window, SIGNAL( dialogClose() ), this, SLOT( windowClosed() ) );
}



DrawingPresenter::~DrawingPresenter()
{
}



void DrawingPresenter::windowClosed()
{
	delete this;
}

