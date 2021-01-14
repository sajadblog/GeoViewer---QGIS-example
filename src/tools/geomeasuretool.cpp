#include "tools/geomeasuretool.h"
#include "items/geotextitem.h"
#include "items/geomultipositionItem.h"
#include "common/geodatacontainer.h"
#include "common/geounitmanager.h"
#include "mapwidget.h"

#include <qgis/qgsdistancearea.h>
#include "widgets/geomapcanvas.h"
#include <qgis/qgsproject.h>
#include <qgis/qgsmapmouseevent.h>

#include <QtMath>

namespace Software
{
namespace Library
{
namespace GisEngine {

GeoMeasureTool::GeoMeasureTool(MapWidget *mapWidget, eMeasureType measureType)
	: GeoMultiPositionTool( mapWidget , eGeometryType::Line),
	  m_measureType(measureType)
{
	mToolName = tr( "Measure" );

	m_pathItem = dynamic_cast<GeoMultiPositionItem *>(m_mapWidget->addItem(eItemType::MultiPosition, 0, false));
	m_currentTextItem = dynamic_cast<GeoTextItem *>(m_mapWidget->addItem(eItemType::Text, 0, false));

	m_distanceArea = new QgsDistanceArea();

	connect( mapWidget->mapCanvas(), &GeoMapCanvas::destinationCrsChanged,
			 this, &GeoMeasureTool::configureDistanceArea );
}

GeoMeasureTool::~GeoMeasureTool()
{
}

void GeoMeasureTool::activate()
{
	configureDistanceArea();
	GeoMultiPositionTool::activate();
}

void GeoMeasureTool::canvasMoveEvent(QgsMapMouseEvent *e)
{
	GeoMultiPositionTool::canvasMoveEvent(e);
	if(!freeze)
	{
		updateMeasureValues();
		updateMeasurePosition();
	}
}

void GeoMeasureTool::canvasReleaseEvent(QgsMapMouseEvent *e)
{
	GeoMultiPositionTool::canvasReleaseEvent(e);

	if(m_measureType == eMeasureType::Angle)
	{
		if ( (m_pathItem->partSize(0) == 3 ) && !freeze) {
			m_pathItem->removePoint(3, true, 0);
			freeze = true;
		}
		return;
	}

	m_totalDistance += m_segmentDistance;

	float textRotation = (m_segmentAzimuth < 180) ? m_segmentAzimuth + 180 : m_segmentAzimuth ;
	textRotation += 90;
	m_currentTextItem->setRotation(textRotation);

	m_textItemList.append(m_currentTextItem);
	m_currentTextItem = dynamic_cast<GeoTextItem *>(m_mapWidget->addItem(eItemType::Text, 0, false));

}

void GeoMeasureTool::resetItems()
{
	for(GeoTextItem *textItem : m_textItemList)
		m_mapWidget->removeItem(textItem);
	m_textItemList.clear();
	m_currentTextItem->setText("");
	m_segmentDistance = 0;
	m_totalDistance = 0;

	GeoMultiPositionTool::resetItems();
}

void GeoMeasureTool::createNewMultiPosition()
{
	// do nothing
}

void GeoMeasureTool::updateMeasureValues()
{
	if(m_pathItem->partSize(0) < 2)
		return;

	QgsPointXY begin	= *m_pathItem->getPoint(0 , m_pathItem->partSize(0) - 2 );
	QgsPointXY end		= *m_pathItem->getPoint(0 , m_pathItem->partSize(0) - 1 );

	m_segmentAzimuth	= m_distanceArea->bearing(      begin, end );
	m_segmentDistance	= m_distanceArea->measureLine(  begin, end );

	m_segmentAzimuth = qRadiansToDegrees(m_segmentAzimuth);
	if ( m_segmentAzimuth < 0 )
		m_segmentAzimuth =  m_segmentAzimuth + 360;

	m_segmentAzimuth = m_mapWidget->getGeoUnitManager()->getAngle(m_segmentAzimuth);
	m_segmentDistance = m_mapWidget->getGeoUnitManager()->getDistance(m_segmentDistance);

	QString measureString;
	switch(m_measureType)
	{
	case eMeasureType::Angle :
		measureString = QString("%1 , %2").
				arg(m_mapWidget->getGeoUnitManager()->formatAngle(m_segmentAzimuth, 2)).
				arg(m_mapWidget->getGeoUnitManager()->formatDistance(m_segmentDistance, 2));
		break;
	case eMeasureType::Distance :
		measureString = QString("%1 , %2").
				arg(m_mapWidget->getGeoUnitManager()->formatDistance(m_segmentDistance, 2)).
				arg(m_mapWidget->getGeoUnitManager()->formatDistance(m_totalDistance + m_segmentDistance, 2));
		break;
	}

	m_currentTextItem->setText(measureString);
}

void GeoMeasureTool::updateMeasurePosition()
{
	if(m_pathItem->partSize(0) < 2)
		return;

	QgsPointXY begin	= *m_pathItem->getPoint(0 , m_pathItem->partSize(0) - 2 );
	QgsPointXY end		= *m_pathItem->getPoint(0 , m_pathItem->partSize(0) - 1 );

	QPointF textPosition ;
	textPosition.setX((begin.x() + end.x()) / 2.0);
	textPosition.setY((begin.y() + end.y()) / 2.0);

	m_currentTextItem->setPosition(textPosition);
}

void GeoMeasureTool::configureDistanceArea()
{
	QString ellipsoidId = QgsProject::instance()->ellipsoid();
	m_distanceArea->setSourceCrs( m_mapWidget->mapCanvas()->mapSettings().destinationCrs(), QgsProject::instance()->transformContext() );
	m_distanceArea->setEllipsoid( ellipsoidId );
	updateMeasureValues();
}
}
}
}
