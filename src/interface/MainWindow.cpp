/*
 * MainWindow.cpp
 *
 *  Created on: 18/10/2014
 *      Author: Guilherme
 */

#include "MainWindow.h"
#include <QStandardItemModel>

#include <QFileDialog>
#include <string>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
{
    setupUi(this);

    connect( buttonOpenUserParams, SIGNAL(clicked()), this, SLOT(openUserParamsClicked() ) );
    connect( buttonExecute, SIGNAL( clicked() ), this, SIGNAL( executeButtonClicked() ) );

	
    _model = new QStandardItemModel();
	QStringList list;
	list.append( "Circuito" );
	list.append( "Origem" );
	list.append( "Destino" );
	list.append( "Rota" );
	list.append( "Cabo" );
	list.append( "Comprimento" );

	_model->setHorizontalHeaderLabels(list);
	tableView->setModel( _model );
}


MainWindow::~MainWindow()
{
}



void MainWindow::openUserParamsClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Selecionar o arquivo de configuração", "c:\\Projetos", tr("Excel (*.xls)"));
    lineEditUserParams->setText( fileName );
}



std::string MainWindow::getUserParamsFileName()
{
    return lineEditUserParams->text().toStdString();
}



std::string MainWindow::getInputCircuitsFileName()
{
    return lineEditInputCircuits->text().toStdString();
}



void MainWindow::setCircuit( int circuitIndex, std::string name, std::string source, std::string dest, std::string route, std::string cable )
{
	_model->setItem( circuitIndex, 0, new QStandardItem( name.c_str() ) );
	_model->setItem( circuitIndex, 1, new QStandardItem( source.c_str() ) );
	_model->setItem( circuitIndex, 2, new QStandardItem( dest.c_str() ) );
	_model->setItem( circuitIndex, 3, new QStandardItem( route.c_str() ) );
	_model->setItem( circuitIndex, 4, new QStandardItem( cable.c_str() ) );
}



void MainWindow::setCircuitLength( int circuitIndex, double length )
{
	_model->setItem( circuitIndex, 5, new QStandardItem( QString::number( length ) ) );
}