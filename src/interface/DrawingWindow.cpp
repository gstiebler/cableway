/*
 * DrawingWindow.cpp
 *
 *  Created on: 19/10/2014
 *      Author: Guilherme
 */

#include "DrawingWindow.h"

#include <UMostraDesenho.h>

using namespace std;

DrawingWindow::DrawingWindow(shared_ptr<CGrafoDesenho> grafoDesenho, CInfoCircuitos *infoCircuitos)
{
    setupUi(this);

    _mostraDesenho = new CMostraDesenho( grafoDesenho, infoCircuitos );
    verticalLayout->addWidget( _mostraDesenho );

	cbShowOriginalColors->setChecked( true );
	cbShowBandeirolas->setChecked( true );
	cbShowDisconnectedCableEndings->setChecked( true );

	setAttribute( Qt::WA_DeleteOnClose );

	connect( cbShowOriginalColors, SIGNAL(stateChanged(int)), this, SIGNAL(optionsChanged()) );
	connect( cbShowDisconnectedCableEndings, SIGNAL(stateChanged(int)), this, SIGNAL(optionsChanged()) );
	connect( cbShowBandeirolas, SIGNAL(stateChanged(int)), this, SIGNAL(optionsChanged()) );
	connect( cbShowEquipmentEnding, SIGNAL(stateChanged(int)), this, SIGNAL(optionsChanged()) );
}



DrawingWindow::~DrawingWindow()
{
    // TODO Auto-generated destructor stub
}


void DrawingWindow::closeEvent( QCloseEvent * event )
{
	emit dialogClose();
}



void DrawingWindow::getOptions( bool &showOriginalColors, bool &showDisconnectedCables, bool &showBandeirolas, bool &showEquipmentEnding )
{
	showOriginalColors = cbShowOriginalColors->isChecked();
	showDisconnectedCables = cbShowDisconnectedCableEndings->isChecked();
	showBandeirolas = cbShowBandeirolas->isChecked();
	showEquipmentEnding = cbShowEquipmentEnding->isChecked();
}

