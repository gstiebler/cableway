/*
 * DrawingWindow.h
 *
 *  Created on: 19/10/2014
 *      Author: Guilherme
 */

#ifndef DRAWINGWINDOW_H_
#define DRAWINGWINDOW_H_

#include <QDialog>
#include <memory>

#include "ui_DrawingWindow.h"

class CGrafoDesenho;
class CInfoCircuitos;	
class COpenGL;

class DrawingWindow : public QDialog, private Ui::Dialog
{
    Q_OBJECT

public:
    DrawingWindow( std::shared_ptr<CGrafoDesenho> grafoDesenho, std::shared_ptr<CInfoCircuitos> infoCircuitos, std::string drawingName);
    virtual ~DrawingWindow();

	void getOptions( bool &showOriginalColors, bool &showDisconnectedCables, bool &showBandeirolas, bool &showEquipmentEnding );

	COpenGL *_mostraDesenho;

signals:
	void dialogClose();
	void optionsChanged();

private:
	void closeEvent( QCloseEvent * event );
};

#endif /* DRAWINGWINDOW_H_ */
