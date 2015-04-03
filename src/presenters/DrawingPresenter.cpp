/*
 * DrawingPresenter.cpp
 *
 *  Created on: 19/10/2014
 *      Author: Guilherme
 */

#include "DrawingPresenter.h"
#include "UMostraDesenho.h"

#include <interface/DrawingWindow.h>

DrawingPresenter::DrawingPresenter( shared_ptr<CGrafoDesenho> grafoDesenho, shared_ptr<CInfoCircuitos> infoCircuitos, shared_ptr<TArestasCircuito> arestasCircuito )
{
    _window = new DrawingWindow(grafoDesenho, infoCircuitos);
    _window->show();

	//mostraCircuito( circuitIndex );
	
	if( arestasCircuito.get() != 0 )
		_window->_mostraDesenho->MostraCircuito( arestasCircuito );

    connect( _window, SIGNAL( dialogClose() ), this, SLOT( windowClosed() ) );
    connect( _window, SIGNAL( optionsChanged() ), this, SLOT( optionsChanged() ) );
}



DrawingPresenter::~DrawingPresenter()
{
}



void DrawingPresenter::windowClosed()
{
	delete this;
}


void DrawingPresenter::optionsChanged()
{
	bool showOriginalColors, showDisconnectedCables, showBandeirolas, showEquipmentEnding;
	_window->getOptions( showOriginalColors, showDisconnectedCables, showBandeirolas, showEquipmentEnding );
	_window->_mostraDesenho->SetDestacaCores( showOriginalColors );
	_window->_mostraDesenho->SetMostrarPontasDeCaboDescon( showDisconnectedCables );
	_window->_mostraDesenho->SetDestacaBandeirolas( showBandeirolas );
	_window->_mostraDesenho->SetMostraChegaEquip( showEquipmentEnding );
	_window->_mostraDesenho->repaint();
}

