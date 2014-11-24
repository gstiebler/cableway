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

class QStandardItemModel;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    std::string getUserParamsFileName();
    std::string getInputCircuitsFileName();
	void setCircuit( int circuitIndex, std::string name, std::string source, std::string dest, std::string route, std::string cable, 
								double length, std::string errors );

signals:

    void executeButtonClicked();
	void circuitGridClicked( const QModelIndex &index );


private slots:

	void openUserParamsClicked();

private:
	QStandardItemModel *_model;

};

#endif /* MAINWINDOW_H_ */
