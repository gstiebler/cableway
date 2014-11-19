/*
 * MainWindow.h
 *
 *  Created on: 18/10/2014
 *      Author: Guilherme
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QMainWindow>

#include "ui_MainWindow.h"

#include <string>

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    std::string getUserParamsFileName();
    std::string getInputCircuitsFileName();

signals:

    void executeButtonClicked();


private slots:

        void openUserParamsClicked();

};

#endif /* MAINWINDOW_H_ */
