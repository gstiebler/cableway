
#ifndef __INTERFACE_FEEDBACK_H
#define __INTERFACE_FEEDBACK_H

#include <string>
#include <memory>

class MainWindow;

class InterfaceFeedback
{
public:
	InterfaceFeedback( MainWindow* mainWindow );
	void showMessage( std::string text );

private:
	MainWindow* _mainWindow;
};

#endif