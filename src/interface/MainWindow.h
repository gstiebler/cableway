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

    std::string getInputDirectory();
	void setCircuit( int circuitIndex, std::string name, std::string source, std::string dest, std::string userRoute, std::string route, std::string cable, 
								double length, std::string errors );
	void getCircuit( int &circuitIndex, std::string &name, std::string &source, std::string &dest );
	void setInputFolder( std::string inputFolder );

signals:

    void executeButtonClicked();
	void circuitGridClicked( const QModelIndex &index );

private slots:

	void selectFolder();

private:
	QStandardItemModel *_model;

};

#endif /* MAINWINDOW_H_ */
