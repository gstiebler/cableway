/*
 * MainWindow.cpp
 *
 *  Created on: 18/10/2014
 *      Author: Guilherme
 */

#include "MainWindow.h"
#include <QStandardItemModel>
#include <QFileDialog>
#include <QMessageBox>
#include <string>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
{
    setupUi(this);

    connect( buttonExecute, SIGNAL( clicked() ), this, SIGNAL( executeButtonClicked() ) );
    connect( btInputDirectory, SIGNAL( clicked() ), this, SLOT( selectFolder() ) );
	connect( menuSobre, SIGNAL(aboutToShow()), this, SLOT(aboutClicked()) );

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

	statusBar()->show();
	
    connect( tableView, SIGNAL( clicked(QModelIndex) ), this, SIGNAL( circuitGridClicked(QModelIndex) ) );
}


MainWindow::~MainWindow()
{
}



std::string MainWindow::getInputDirectory()
{
	return lineEditInputDirectory->text().toStdString();
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



void MainWindow::getCircuit( int &circuitIndex, string &name, string &source, string &dest )
{
	name = _model->item( circuitIndex, 0 )->text().toStdString();
	source = _model->item( circuitIndex, 1 )->text().toStdString();
	dest = _model->item( circuitIndex, 2 )->text().toStdString();
}

 

void MainWindow::selectFolder()
{
	QFileDialog dialog(NULL);
	dialog.setFileMode(QFileDialog::Directory);
	dialog.setOption(QFileDialog::ShowDirsOnly);
	dialog.setDirectory( lineEditInputDirectory->text() );
	if ( dialog.exec() )
		lineEditInputDirectory->setText( dialog.selectedFiles()[0] );
}



void MainWindow::setInputFolder( std::string inputFolder )
{
	lineEditInputDirectory->setText( QString::fromLatin1( inputFolder.c_str() ) );
}


void MainWindow::showStatusMessage( std::string text )
{
	statusBar()->showMessage( QString::fromLatin1( text.c_str() ) );
}


void MainWindow::aboutClicked()
{
	QMessageBox msgBox;
	msgBox.setText("Propriedade intelectual de Guilherme Stiebler");
	msgBox.exec();
}