/*
 * DrawingPresenter.cpp
 *
 *  Created on: 19/10/2014
 *      Author: Guilherme
 */

#include "DrawingPresenter.h"

#include <interface/DrawingWindow.h>

DrawingPresenter::DrawingPresenter(CGrafoDesenho *grafoDesenho, CInfoCircuitos *infoCircuitos)
{
    _window = new DrawingWindow(grafoDesenho, infoCircuitos);
    _window->show();

    connect( _window, SIGNAL( dialogClose() ), this, SLOT( windowClosed() ) );
}



DrawingPresenter::~DrawingPresenter()
{
}



void DrawingPresenter::windowClosed()
{
	delete this;
}

