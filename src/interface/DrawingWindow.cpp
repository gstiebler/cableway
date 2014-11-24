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

    _mostraDesenho = new CMostraDesenho( grafoDesenho, infoCircuitos );
    verticalLayout->addWidget( _mostraDesenho );

	setAttribute( Qt::WA_DeleteOnClose );
}



DrawingWindow::~DrawingWindow()
{
    // TODO Auto-generated destructor stub
}


void DrawingWindow::closeEvent( QCloseEvent * event )
{
	emit dialogClose();
}

