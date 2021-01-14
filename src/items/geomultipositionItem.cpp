#include "items/geomultipositionItem.h"
#include "common/geodatacontainer.h"
#include "widgets/geomapcanvas.h"

#include "mapwidget.h"

namespace Software
{
namespace Library
{
namespace GisEngine {

GeoMultiPositionItem::GeoMultiPositionItem(MapWidget *mapWidget, eGeometryType geometryType, quint64 id):
	QgsRubberBand(mapWidget->mapCanvas(),(geometryType == eGeometryType::Line)?QgsWkbTypes::LineGeometry:
															   ((geometryType == eGeometryType::Point)?QgsWkbTypes::PointGeometry:
																									   QgsWkbTypes::PolygonGeometry))
  ,GeoItem(mapWidget,id)
  ,m_geometryType(geometryType)
{
	m_canvasItem = this;
	if(m_mapWidget->getGeoDataContainer())
	{
		setColor(m_mapWidget->getGeoDataContainer()->getLinePen().color());
		setWidth(m_mapWidget->getGeoDataContainer()->getLinePen().width());
		setLineStyle(m_mapWidget->getGeoDataContainer()->getLinePen().style());
		setFillColor(m_mapWidget->getGeoDataContainer()->getFillBrush().color());
		setBrushStyle(m_mapWidget->getGeoDataContainer()->getFillBrush().style());
	}
}

eItemType GeoMultiPositionItem::type()
{
	return eItemType::MultiPosition;
}

void GeoMultiPositionItem::setVisible(bool visible)
{
	GeoItem::setVisible(visible);
}

eGeometryType GeoMultiPositionItem::geometryType() const
{
	return m_geometryType;
}

void GeoMultiPositionItem::reset()
{
	reset(m_geometryType);
}

void GeoMultiPositionItem::reset(const eGeometryType &geometryType)
{
	m_geometryType = geometryType;
	switch (m_geometryType) {
	case eGeometryType::Point:
		QgsRubberBand::reset(QgsWkbTypes::PointGeometry);break;
	case eGeometryType::Line:
		QgsRubberBand::reset(QgsWkbTypes::LineGeometry);break;
	case eGeometryType::Polygon:
		QgsRubberBand::reset(QgsWkbTypes::PolygonGeometry);break;
	}
}

}
}
}
