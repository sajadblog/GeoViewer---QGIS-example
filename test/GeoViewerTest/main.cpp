#include "mainwindow.h"
#include <qgis/qgsapplication.h>
#include <iostream>
#include <qgis/qgslogger.h>

int main(int argc, char *argv[])
{

	QgsApplication myApp( argc, argv, true );
	myApp.init();

	MainWindow w;
	w.showMaximized();

	return myApp.exec();
}
