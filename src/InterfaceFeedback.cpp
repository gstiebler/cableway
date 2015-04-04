#include "InterfaceFeedback.h"
#include "interface/MainWindow.h"

InterfaceFeedback::InterfaceFeedback( MainWindow* mainWindow ) :
	_mainWindow( mainWindow )
{
}


void InterfaceFeedback::showMessage( std::string text )
{
	if( _mainWindow )
		_mainWindow->showStatusMessage( text );
}