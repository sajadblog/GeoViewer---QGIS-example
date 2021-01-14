#include "common/geodatacontainer.h"

#include "mapwidget.h"
#include "widgets/geomapcanvas.h"

#include "items/geoitem.h"
#include "components/geocomponent.h"
#include "components/geolayerscomponent.h"
#include "components/geobrowsercomponent.h"

#include "tools/geomeasuretool.h"
#include "tools/geomultipositiontool.h"
#include "tools/geo2positionitemtool.h"

#include <qgis/qgsmaptool.h>
#include <qgis/qgsmaptoolpan.h>
#include <qgis/qgsmaptoolzoom.h>
namespace Software
{
namespace Library
{
namespace GisEngine {

GeoDataContainer::GeoDataContainer(MapWidget *parent) :
	QObject(parent),
	m_mapWidget(parent)
{
	createComponents();
	createTools();
	createGeneralValues();
	createConnections();
}

void GeoDataContainer::createConnections()
{
	connect(m_mapWidget, &MapWidget::clearedSelection, [&](){
		for(QMap< quint64, GeoItem* > itemContainer : m_itemContainers.values())
		{
			for(GeoItem * item : itemContainer.values())
			{
				item->setSelected(false);
			}
		}
	});
}

void GeoDataContainer::createComponents()
{
	m_componentMap.insert(eComponentType::Layers	, new GeoLayersComponent(  m_mapWidget));
	m_componentMap.insert(eComponentType::Browser	, new GeoBrowserComponent( m_mapWidget));
}

void GeoDataContainer::createTools()
{
	m_toolMap.insert(eToolType::PanTool			, new QgsMapToolPan(		m_mapWidget->mapCanvas()));
	m_toolMap.insert(eToolType::ZoomTool		, new QgsMapToolZoom(		m_mapWidget->mapCanvas(), false));
	m_toolMap.insert(eToolType::MultipointTool	, new GeoMultiPositionTool( m_mapWidget, eGeometryType::Point));
	m_toolMap.insert(eToolType::CircleTool		, new Geo2PositionItemTool(	m_mapWidget, e2PositionItemType::Circle));
	m_toolMap.insert(eToolType::RectangleTool	, new Geo2PositionItemTool(	m_mapWidget, e2PositionItemType::Rectangle));
	m_toolMap.insert(eToolType::PolylineTool	, new GeoMultiPositionTool( m_mapWidget, eGeometryType::Line));
	m_toolMap.insert(eToolType::PolygonTool		, new GeoMultiPositionTool( m_mapWidget, eGeometryType::Polygon));
	m_toolMap.insert(eToolType::BearingTool		, new GeoMeasureTool(		m_mapWidget, eMeasureType::Angle));
	m_toolMap.insert(eToolType::DistanceTool	, new GeoMeasureTool(		m_mapWidget, eMeasureType::Distance));
}

void GeoDataContainer::createGeneralValues()
{
	m_generalFont = QFont("DejaVu Sans", 10);

	m_colorizeLevelValues.append(1000);
	m_colorizeLevelValues.append(2000);
	m_colorizeLevelValues.append(3000);
	m_colorizeLevelValues.append(4000);
	m_colorizeLevelValues.append(6000);
	m_colorizeLevelValues.append(8000);
	m_colorizeLevelValues.append(10000);
	m_colorizeLevelValues.append(11000);
	m_colorizeLevelValues.append(13000);
	setColorizeLevelValues(m_colorizeLevelValues);

	m_linePen.setColor(Qt::darkRed);
	m_linePen.setWidth(2);
	m_linePen.setStyle(Qt::SolidLine);
	m_fillBrush.setColor(QColor(0, 255, 0, 100));
	m_fillBrush.setStyle(Qt::SolidPattern);


	m_measuringToolPen.setColor(QColor(255, 0, 0, 100));
	m_measuringToolPen.setWidth(2);
	m_measuringToolPen.setStyle(Qt::DashLine);
	m_measuringToolBrush.setColor(QColor(0, 0, 255, 100));
	m_measuringToolBrush.setStyle(Qt::CrossPattern);

}

void GeoDataContainer::addItem(GeoItem *item)
{
	if(!m_itemContainers.contains(item->type())) {
		QMap< quint64, GeoItem* > map;
		m_itemContainers.insert(item->type(),map);
	}
	m_itemContainers[item->type()].insert(item->id(),item);
}

bool GeoDataContainer::removeItem(GeoItem *item)
{
	if(!m_itemContainers.contains(item->type()))
		return false;
	if(!m_itemContainers[item->type()].contains(item->id()))
		return false;
	m_itemContainers[item->type()].remove(item->id());
	return true;
}

void GeoDataContainer::setMeasuringToolBrush(const QBrush &measuringToolBrush)
{
	m_measuringToolBrush = measuringToolBrush;
}

void GeoDataContainer::setFillBrush(const QBrush &fillBrush)
{
	m_fillBrush = fillBrush;
}

void GeoDataContainer::setLinePen(const QPen &linePen)
{
	m_linePen = linePen;
}

void GeoDataContainer::setGeneralFont(const QFont &generalFont)
{
	m_generalFont = generalFont;
}

void GeoDataContainer::setMeasuringToolPen(const QPen &measuringToolPen)
{
	m_measuringToolPen = measuringToolPen;
	dynamic_cast<GeoMeasureTool *>(m_toolMap.value(eToolType::BearingTool))->updateRubberBandStyle();
	dynamic_cast<GeoMeasureTool *>(m_toolMap.value(eToolType::DistanceTool))->updateRubberBandStyle();
//	dynamic_cast<GeoMeasureTool *>(m_toolMap.value(eToolType::AreaTool))->updateRubberBandStyle();
}

void GeoDataContainer::setColorizePathPen(const QPen &colorizePathPen)
{
	m_colorizePathPen = colorizePathPen;
}

void GeoDataContainer::setColorizePathLength(const quint16 &colorizePathLength)
{
	m_colorizePathLength = colorizePathLength;
}

void GeoDataContainer::setColorizeLevelValues(const QList<double> &colorizeLevelValues)
{
	m_colorizeLevelValues = colorizeLevelValues;
	qSort(m_colorizeLevelValues.begin(), m_colorizeLevelValues.end(), [=](const int p1, const int p2) {
		return p1  < p2 ;
	});

}

GeoItem *GeoDataContainer::getItem(eItemType type, quint64 id)
{
	if(!m_itemContainers.contains(type))
		return nullptr;
	if(!m_itemContainers[type].contains(id))
		return nullptr;
	return m_itemContainers[type][id];
}

QList<GeoItem *> GeoDataContainer::getItemList(eItemType type)
{
	if(!m_itemContainers.contains(type)) {
		QList<GeoItem *>emptyList;
		return  emptyList;
	}
	return m_itemContainers[type].values();
}

GeoComponent *GeoDataContainer::getComponent(eComponentType type)
{
	if(!m_componentMap.contains(type))
		return nullptr;

	return m_componentMap[type];
}
}
}
}
