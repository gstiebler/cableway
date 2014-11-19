/*
 * MainWindow.cpp
 *
 *  Created on: 18/10/2014
 *      Author: Guilherme
 */

#include "MainWindow.h"

#include <QFileDialog>
#include <string>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
{
    setupUi(this);

    connect( buttonOpenUserParams, SIGNAL(clicked()), this, SLOT(openUserParamsClicked() ) );
    connect( buttonExecute, SIGNAL( clicked() ), this, SIGNAL( executeButtonClicked() ) );
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
