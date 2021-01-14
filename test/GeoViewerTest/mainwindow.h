#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <items/geomarkeritem.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionAdd_Layer_triggered();

    void on_actionSimulation_triggered();

private:
    Ui::MainWindow *ui;
	QTimer *testTimer;
	QList<Software::Library::GisEngine::GeoMarkerItem *> markerList;
	bool testValue = true;


};

#endif // MAINWINDOW_H
