/*
 * DrawingWindow.cpp
 *
 *  Created on: 19/10/2014
 *      Author: Guilherme
 */

#include "DrawingWindow.h"

#include "ScreenDrawing/UOpenGL.h"

using namespace std;

DrawingWindow::DrawingWindow( shared_ptr<CDadosGenerico> dados, shared_ptr<CInfoCircuitos> infoCircuitos, std::string drawingName)
{
    setupUi(this);

	_mostraDesenho = new COpenGL( 600, 600, this, dados, infoCircuitos );
    verticalLayout->addWidget( _mostraDesenho );

	setWindowTitle( drawingName.c_str() );

	cbShowOriginalColors->setChecked( true );
	cbShowBandeirolas->setChecked( true );
	cbShowDisconnectedCableEndings->setChecked( true );

	setAttribute( Qt::WA_DeleteOnClose );

	connect( cbShowOriginalColors, SIGNAL(stateChanged(int)), this, SIGNAL(optionsChanged()) );
	connect( cbShowDisconnectedCableEndings, SIGNAL(stateChanged(int)), this, SIGNAL(optionsChanged()) );
	connect( cbShowBandeirolas, SIGNAL(stateChanged(int)), this, SIGNAL(optionsChanged()) );
	connect( cbShowEquipmentEnding, SIGNAL(stateChanged(int)), this, SIGNAL(optionsChanged()) );
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

