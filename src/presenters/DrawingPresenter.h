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

class DrawingPresenter : public QObject
{
    Q_OBJECT

public:
    DrawingPresenter();
    virtual ~DrawingPresenter();

private:
    DrawingWindow *_window;
};

#endif /* DRAWINGPRESENTER_H_ */
