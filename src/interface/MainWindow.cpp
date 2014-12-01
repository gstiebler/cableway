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
	list.append( "Rota desejada" );
	list.append( "Rota" );
	list.append( "Cabo" );
	list.append( "Comprimento" );
	list.append( "Erros" );

	_model->setHorizontalHeaderLabels( list );
	tableView->setModel( _model );

	tableView->setColumnWidth( 7, 800 );
	
    connect( tableView, SIGNAL( clicked(QModelIndex) ), this, SIGNAL( circuitGridClicked(QModelIndex) ) );
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



void MainWindow::setCircuit( int circuitIndex, std::string name, std::string source, std::string dest, std::string userRoute, std::string route, std::string cable, 
								double length, std::string errors )
{
	_model->setItem( circuitIndex, 0, new QStandardItem( name.c_str() ) );
	_model->setItem( circuitIndex, 1, new QStandardItem( source.c_str() ) );
	_model->setItem( circuitIndex, 2, new QStandardItem( dest.c_str() ) );
	_model->setItem( circuitIndex, 3, new QStandardItem( userRoute.c_str() ) );
	_model->setItem( circuitIndex, 4, new QStandardItem( route.c_str() ) );
	_model->setItem( circuitIndex, 5, new QStandardItem( cable.c_str() ) );
	_model->setItem( circuitIndex, 6, new QStandardItem( QString::number( length ) ) );
	_model->setItem( circuitIndex, 7, new QStandardItem( QString::fromLatin1( errors.c_str() ) ) );
}