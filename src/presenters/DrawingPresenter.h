/*
 * DrawingPresenter.h
 *
 *  Created on: 19/10/2014
 *      Author: Guilherme
 */

#ifndef DRAWINGPRESENTER_H_
#define DRAWINGPRESENTER_H_

#include <QtCore/QObject>

class DrawingWindow;
class CGrafoDesenho;
class CInfoCircuitos;

class DrawingPresenter : public QObject
{
    Q_OBJECT

public:
    DrawingPresenter(CGrafoDesenho *grafoDesenho, CInfoCircuitos *infoCircuitos);
    virtual ~DrawingPresenter();

private:
    DrawingWindow *_window;
};

#endif /* DRAWINGPRESENTER_H_ */
