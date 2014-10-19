/*
 * DrawingWindow.cpp
 *
 *  Created on: 19/10/2014
 *      Author: Guilherme
 */

#include "DrawingWindow.h"

#include <UMostraDesenho.h>

DrawingWindow::DrawingWindow(CGrafoDesenho *grafoDesenho, CInfoCircuitos *infoCircuitos)
{
    setupUi(this);

    CMostraDesenho *mostraDesenho = new CMostraDesenho( grafoDesenho, infoCircuitos );
    verticalLayout->addWidget(mostraDesenho);
}

DrawingWindow::~DrawingWindow()
{
    // TODO Auto-generated destructor stub
}

