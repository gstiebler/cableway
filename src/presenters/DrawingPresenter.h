/*
 * DrawingPresenter.h
 *
 *  Created on: 19/10/2014
 *      Author: Guilherme
 */

#ifndef DRAWINGPRESENTER_H_
#define DRAWINGPRESENTER_H_

#include <QtCore/QObject>
#include <memory>

class DrawingWindow;
class CGrafoDesenho;
class CInfoCircuitos;
struct TArestasCircuito;

class DrawingPresenter : public QObject
{
    Q_OBJECT

public:
    DrawingPresenter( std::shared_ptr<CGrafoDesenho> grafoDesenho, std::shared_ptr<CInfoCircuitos> infoCircuitos, std::shared_ptr<TArestasCircuito> arestasCircuito );
    virtual ~DrawingPresenter();

    DrawingWindow *_window;

	
private slots:
    void windowClosed();

	void optionsChanged();
};

#endif /* DRAWINGPRESENTER_H_ */
