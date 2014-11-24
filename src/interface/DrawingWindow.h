/*
 * DrawingWindow.h
 *
 *  Created on: 19/10/2014
 *      Author: Guilherme
 */

#ifndef DRAWINGWINDOW_H_
#define DRAWINGWINDOW_H_

#include <QDialog>

#include "ui_DrawingWindow.h"

class CGrafoDesenho;
class CInfoCircuitos;	

class DrawingWindow : public QDialog, private Ui::Dialog
{
    Q_OBJECT

public:
    DrawingWindow(CGrafoDesenho *grafoDesenho, CInfoCircuitos *infoCircuitos);
    virtual ~DrawingWindow();

signals:
	void dialogClose();

private:
	void closeEvent( QCloseEvent * event );
};

#endif /* DRAWINGWINDOW_H_ */
