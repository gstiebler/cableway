/*
 * MainPresenter.h
 *
 *  Created on: 18/10/2014
 *      Author: Guilherme
 */

#ifndef MAINPRESENTER_H_
#define MAINPRESENTER_H_

#include <QtCore/QObject>

class MainWindow;

class MainPresenter : public QObject
{
    Q_OBJECT

public:
    MainPresenter();
    virtual ~MainPresenter();

private:
    MainWindow *_window;


private slots:
    void execute();

};

#endif /* MAINPRESENTER_H_ */
