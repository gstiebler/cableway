/*
 * MainWindow.h
 *
 *  Created on: 18/10/2014
 *      Author: Guilherme
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QtGui/QMainWindow>

#include "ui_MainWindow.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

private slots:

};

#endif /* MAINWINDOW_H_ */
