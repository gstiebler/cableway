/*
 * DrawingWindow.h
 *
 *  Created on: 19/10/2014
 *      Author: Guilherme
 */

#ifndef DRAWINGWINDOW_H_
#define DRAWINGWINDOW_H_

#include <QtGui/QDialog>

#include "ui_DrawingWindow.h"

class DrawingWindow : public QDialog, private Ui::Dialog
{
    Q_OBJECT

public:
    DrawingWindow();
    virtual ~DrawingWindow();
};

#endif /* DRAWINGWINDOW_H_ */
