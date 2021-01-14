#include "tools/geomultipositiontool.h"
#include "items/geomultipositionItem.h"
#include "common/geodatacontainer.h"
#include "mapwidget.h"

#include "widgets/geomapcanvas.h"
#include <qgis/qgsmapmouseevent.h>

namespace Software
{
namespace Library
{
namespace GisEngine {

GeoMultiPositionTool::GeoMultiPositionTool( MapWidget *mapWidget, eGeometryType geometryType)
  : QgsMapTool( mapWidget->mapCanvas() ),
	m_mapWidget(mapWidget),
	m_geometryType(geometryType)
{
  mToolName = tr( "Multi Position Generator" );

  m_pathItem = dynamic_cast<GeoMultiPositionItem *>(m_mapWidget->addItem(eItemType::MultiPosition, 0, false));
  m_pathItem->reset(m_geometryType);

}

GeoMultiPositionTool::~GeoMultiPositionTool()
{
}

void GeoMultiPositionTool::canvasMoveEvent( QgsMapMouseEvent *e )
{
	if (( m_pathItem->partSize(0) < 2 ) || ( freeze ) )
	  return;

	m_pathItem->movePoint( e->snapPoint() );
}

void GeoMultiPositionTool::canvasReleaseEvent( QgsMapMouseEvent *e )
{
	if ( e->button() == Qt::RightButton )
	{
		if(m_pathItem->partSize(0) == 0)
			m_mapWidget->resetMapTool();
		else{
			createNewMultiPosition();
			resetItems();
		}
	}
	else if ( e->button() == Qt::LeftButton )
	{
		if( freeze )
		{
			freeze = false;
			resetItems();
		}
		m_pathItem->addPoint( e->snapPoint() );
	}
}

void GeoMultiPositionTool::activate()
{
	updateRubberBandStyle();
	QgsMapTool::activate();
}

void GeoMultiPositionTool::deactivate()
{
  resetItems();

  QgsMapTool::deactivate();
}

void GeoMultiPositionTool::updateRubberBandStyle()
{
	QPen pen = m_mapWidget->getGeoDataContainer()->getMeasuringToolPen();
	m_pathItem->setColor( pen.color() );
	m_pathItem->setWidth( pen.width());
	m_pathItem->setLineStyle(pen.style());

	QBrush brush = m_mapWidget->getGeoDataContainer()->getMeasuringToolBrush();
	m_pathItem->setFillColor(brush.color());
	m_pathItem->setBrushStyle(brush.style());
}

void GeoMultiPositionTool::resetItems()
{
	m_pathItem->reset();
}

eGeometryType GeoMultiPositionTool::geometryType() const
{
	return m_geometryType;
}

void GeoMultiPositionTool::createNewMultiPosition()
{
	if(m_pathItem->partSize(0) == 0)
		return ;

	GeoMultiPositionItem *newItem = dynamic_cast<GeoMultiPositionItem *>(m_mapWidget->addItem(eItemType::MultiPosition));
	newItem->reset(m_geometryType);
	for(int i = 0 ; i <  m_pathItem->partSize(0) ; i++)
		newItem->addPoint(m_pathItem->getPoint(0 , i)->toQPointF());
}
}
}
}
