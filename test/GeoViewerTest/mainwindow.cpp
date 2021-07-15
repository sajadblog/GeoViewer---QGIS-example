#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "GeoViewer/items/geosimplemarkeritem.h"
#include "GeoViewer/items/geomarkeritem.h"
#include "GeoViewer/items/geotextitem.h"
#include "GeoViewer/items/geomultipositionItem.h"
#include "GeoViewer/items/geosectoritem.h"
#include "GeoViewer/common/mapstructures.h"
#include "GeoViewer/common/geodatacontainer.h"
#include "GeoViewer/components/geocomponent.h"

#include <qgis/qgsbrowserdockwidget.h>
#include <qgis/qgsbrowsermodel.h>
#include <qgis/qgsgui.h>
#include <qgis/qgsnative.h>


#define TESTCASE
#define AIRPLANE_COUNT 1
#define SIMULATION_INTERVAL 1000

using namespace Software::Library::GisEngine;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->mapWidget->addLayer("https://tile.openstreetmap.org/{z}/{x}/{y}.png", eLayerType::XYZ, "OSM");

	addDockWidget(Qt::RightDockWidgetArea, ui->mapWidget->getGeoDataContainer()->getComponent(eComponentType::Layers)->getDockObject());
	addDockWidget(Qt::LeftDockWidgetArea, ui->mapWidget->getGeoDataContainer()->getComponent(eComponentType::Browser)->getDockObject());
	connect(ui->mapWidget, &MapWidget::mouseCoodinateChanged, [&](double x, double y){
		ui->statusBar->showMessage(QString::number(x) + " : " + QString::number(y));
	});


#ifdef TESTCASE
	//polyline or polygon or multi point
	GeoMultiPositionItem *m_rubberBand = static_cast<GeoMultiPositionItem *>(ui->mapWidget->addItem(eItemType::MultiPosition));
	m_rubberBand->setFillColor(Qt::blue);
	m_rubberBand->setStrokeColor(Qt::green);
	m_rubberBand->setWidth(3);
	m_rubberBand->setIcon(QgsRubberBand::ICON_BOX);
	m_rubberBand->setIconSize(20);
	m_rubberBand->addPoint(QgsPointXY(0,0));
	m_rubberBand->addPoint(QgsPointXY(10,10));
	m_rubberBand->addPoint(QgsPointXY(0,30));

	//simple marker
	GeoSimpleMarkerItem *m_simpleMarker = static_cast<GeoSimpleMarkerItem *>(ui->mapWidget->addItem(eItemType::SimpleMarker,10));
	m_simpleMarker->setColor(Qt::blue);
	m_simpleMarker->setIconType(eSimpleMarkerSymbolType::X);
	m_simpleMarker->setIconSize(10);
	m_simpleMarker->setPenWidth(3);
	m_simpleMarker->setFillColor(Qt::green);
	m_simpleMarker->setPosition(QVector3D(51,34,10));
//	m_simpleMarker->textItem()->setText("test Simple marker object ");

	//Text Item
	GeoTextItem *m_textItem = static_cast<GeoTextItem *>(ui->mapWidget->addItem(eItemType::Text));
	m_textItem->setPosition(QPointF(56,26));
	m_textItem->setText("test");
	m_textItem->setHorizontalAlignment(eHorizontalAlignment::Center);
	m_textItem->setVerticalAlignment(eVerticalAlignment::Down);
	ui->mapWidget->removeItem(m_textItem);

	//Sector Item
	GeoSectorItem *m_sectorItem = static_cast<GeoSectorItem *>(ui->mapWidget->addItem(eItemType::Sector));
	sZoneInfo zoneInfo ;
	zoneInfo.center = QPointF(0,0);
	zoneInfo.minRange = 1000;
	zoneInfo.maxRange = 100000;
	zoneInfo.minAzimuth = 90;
	zoneInfo.maxAzimuth = 135;
	m_sectorItem->setZoneInfo(zoneInfo);

	//Pixmap Marker
	for(int i = 0 ;i < AIRPLANE_COUNT; i++)
	{
		GeoMarkerItem *m_itemTest = static_cast<GeoMarkerItem *>(ui->mapWidget->addItem(eItemType::marker));
		m_itemTest->setPosition(QVector3D((qrand() / 10000)%100,(qrand() / 10000)%100, qrand() % 13000));
        m_itemTest->setPixmap(QPixmap(":/icons/airplane-black.png"));
		m_itemTest->textItem()->setText("simulator");
		markerList.append(m_itemTest);
	}
	testTimer = new QTimer(this);
	connect(testTimer,&QTimer::timeout,[&](){
		for(int i = 0 ;i < markerList.size() ; i++)
		{
			QVector3D Pos((qrand() / 10000)%100,(qrand() / 10000)%100, qrand() % 13000);
			markerList[i]->setPosition(Pos, (qrand() % 3) + 1, true);
			markerList[i]->setRotation(qrand()%360);
		}
	});
	connect(ui->pushButton,&QPushButton::clicked,[&](){
		testTimer->stop();
		if(testValue)
		{
			ui->mapWidget->setActiveTool(eToolType::RectangleTool);
			for(int i = 0 ;i < 1 ; i++)
			{
				GeoMarkerItem *m_itemTest = static_cast<GeoMarkerItem *>(ui->mapWidget->addItem(eItemType::marker));
				m_itemTest->textItem()->setText("10");
				for(int j = 0 ; j < 10 ; j++)
					m_itemTest->setPosition(QVector3D((qrand() / 10000)%100,(qrand() / 10000)%100, qrand() % 13000));
                m_itemTest->setPixmap(QPixmap(":/icons/airplane-black.png"));
			}

		}else{
			ui->mapWidget->resetMapTool();
			ui->mapWidget->removeAllItems();
		}
		testValue = (testValue == false);

		int index = qrand() % 3;
		markerList.first()->setVisibleDatasourceTrajectoy(index, !markerList.first()->visibleDatasourceTrajectoy(index));
	});


#endif

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAdd_Layer_triggered()
{
	ui->mapWidget->addLayerByDialog(eLayerType::XYZ);
}

void MainWindow::on_actionSimulation_triggered()
{
#ifdef TESTCASE
	testTimer->start(SIMULATION_INTERVAL);
#endif

}
