#include "mapwidget.h"
#include "ui_mapwidget.h"
#include "widgets/geomapcanvas.h"
#include "items/geoitem.h"
#include "items/geomarkeritem.h"
#include "items/geosimplemarkeritem.h"
#include "items/geotextitem.h"
#include "items/geomultipositionItem.h"
#include <items/geosectoritem.h>
#include <items/geocolorizepathitem.h>

#include "common/geodatacontainer.h"
#include "common/geounitmanager.h"

#include <components/geobrowsercomponent.h>
#include <components/geolayerscomponent.h>

#include "QFileInfo"
#include "QFileDialog"

#include "qgis/qgsproject.h"
#include "qgis/qgsrasterlayer.h"
#include "qgis/qgsvectorlayer.h"
#include "qgis/qgsproviderregistry.h"
#include "qgis/qgsmapcanvasitem.h"
#include "qgis/qgsrubberband.h"
#include "qgis/qgsvertexmarker.h"
#include "qgis/qgslayertree.h"
#include <qgis/qgsxyzconnection.h>
#include <qgis/qgsmaptool.h>

#include <qgsapplication.h>


namespace Software
{
namespace Library
{
namespace GisEngine {

MapWidget::MapWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MapWidget)
{
	ui->setupUi(this);
	registerMetaTypes();
	init();

	createConnections();
}

MapWidget::~MapWidget()
{
	m_settings->sync();
	removeAllItems();
	delete ui;
}

void MapWidget::setRotation(double rotation)
{
	m_mapCanvas->setRotation(rotation);
}

double MapWidget::rotation()
{
	return m_mapCanvas->rotation();
}

bool MapWidget::addLayer(QString Address, eLayerType layerType, QString LayerName)
{
	if(Address.isEmpty())
		return false;

	QgsMapLayer *newLayer = nullptr;
	switch(layerType)
	{
	case eLayerType::None:
		return false;
	case eLayerType::Raster:
		newLayer = new QgsRasterLayer(Address,LayerName);
		break;
	case eLayerType::Vector:
	{
		newLayer = new QgsVectorLayer(Address,LayerName);
	}
		break;
	case eLayerType::XYZ:
	{
		if(!Address.left(4).contains("type"))
		{
			QgsXyzConnection connection;
			connection.url = Address;
			connection.name = LayerName;
			connection.zMax = 24;
			connection.zMin = 0;
			connection.hidden = false;
			connection.authCfg = "";
			connection.referer = "";
			connection.password = "";
			connection.username = "";
			Address = connection.encodedUri();
		}
		newLayer = new QgsRasterLayer(Address, LayerName, "wms");
		break;
	}
	}

	if(!newLayer)
		return false;

	if(!newLayer->isValid())
		return false;

	QgsProject::instance()->addMapLayer(newLayer);
	return true;
}

bool MapWidget::addLayerByDialog(eLayerType type)
{
	QString address;
	QString layerName;
	dynamic_cast<GeoLayersComponent *>(m_geoDataContainer->m_componentMap[eComponentType::Layers])->addLayerByDialog(address, type, layerName);
	return addLayer(address, type, layerName);
}

void MapWidget::registerMetaTypes()
{
}
GeoItem *MapWidget::addItem(eItemType type, quint64 id, bool isPrimary)
{
	GeoItem *item;
	switch (type) {
	case eItemType::SimpleMarker:
		item = new GeoSimpleMarkerItem(this, id);break;
	case eItemType::marker:
		item = new GeoMarkerItem(this, id);break;
	case eItemType::Text:
		item = new GeoTextItem(this, id);break;
	case eItemType::MultiPosition:
		item = new GeoMultiPositionItem(this,eGeometryType::Line, id);break;
	case eItemType::Sector:
		item = new GeoSectorItem(this, id);break;
	case eItemType::ColorizePath:
		item = new GeoColorizePathItem(this, id);break;
	default:
		return nullptr;
	}
	item->setSelectable(isPrimary);

	if(isPrimary)
		m_geoDataContainer->addItem(item);
	return item;
}
bool MapWidget::removeItem(GeoItem * item, bool isPrimary)
{
	m_mapCanvas->freeze(true);
	if(isPrimary)
		m_geoDataContainer->removeItem(item);

	delete item ;
	m_mapCanvas->freeze(false);
	return true;
}

void MapWidget::removeAllItems()
{
	m_mapCanvas->freeze(true);
	for(QMap< quint64, GeoItem* > itemContainer : m_geoDataContainer->m_itemContainers.values())
	{
		for(GeoItem * item : itemContainer.values())
		{
			removeItem(item);
		}
	}
	m_geoDataContainer->m_itemContainers.clear();
	m_mapCanvas->freeze(false);
}

bool MapWidget::setActiveTool(eToolType tool)
{
	if(!m_geoDataContainer->m_toolMap.contains(tool))
		return false;
	m_mapCanvas->setActiveTool(m_geoDataContainer->m_toolMap[tool]);
	return true;
}

void MapWidget::resetMapTool()
{
	m_mapCanvas->resetMapTool();
}

GeoDataContainer *MapWidget::getGeoDataContainer()
{
	return m_geoDataContainer ;
}

void MapWidget::init()
{
	initQGis();
	m_settings = new QSettings("GisEngineSettings" + QString::number(((rand() % 899)+100)));
	m_geoDataContainer = new GeoDataContainer(this);
	m_geoUnitManager = new GeoUnitManager(this);
}

void MapWidget::initQGis()
{
#ifdef QGISDEBUG
	setenv("QGIS_DEBUG","3",1);
#endif
	m_mapCanvas = new GeoMapCanvas(this);
	layout()->addWidget(m_mapCanvas);

	QgsApplication::setPluginPath("/usr/lib/qgis/plugins");
	QgsApplication::setPkgDataPath("/usr/share/qgis"); // set qgis files to user crs DB address
	QgsApplication::initQgis();

	//Set CRS
	QgsCoordinateReferenceSystem m_crs;
	m_crs = QgsCoordinateReferenceSystem::fromEpsgId(4326) ;
	QgsProject::instance()->setCrs(m_crs);
	QgsProject::instance()->setEllipsoid( m_crs.ellipsoidAcronym() );
	m_mapCanvas->setDestinationCrs(m_crs);
	if(!m_crs.isValid())
		QgsDebugMsg("can not load crs database");
}

void MapWidget::createConnections()
{
	connect(m_mapCanvas, &GeoMapCanvas::xyCoordinates,[&](QgsPointXY pos){
		emit mouseCoodinateChanged(pos.x(), pos.y());
	});
}

GeoUnitManager *MapWidget::getGeoUnitManager() const
{
	return m_geoUnitManager;
}

QSettings *MapWidget::getSettings() const
{
	return m_settings;
}

}
}
}
