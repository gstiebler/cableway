/*
 * DrawingPresenter.cpp
 *
 *  Created on: 19/10/2014
 *      Author: Guilherme
 */

#include "DrawingPresenter.h"

#include <interface/DrawingWindow.h>

DrawingPresenter::DrawingPresenter()
{
    _window = new DrawingWindow;
    _window->show();
}

DrawingPresenter::~DrawingPresenter()
{
    delete _window;
}

