#include "tools/geo2positionitemtool.h"
#include "items/geomultipositionItem.h"
#include "common/geodatacontainer.h"
#include "mapwidget.h"
#include "widgets/geomapcanvas.h"


#include "widgets/geomapcanvas.h"
#include <qgis/qgsmapmouseevent.h>
#include <qgis/qgscircle.h>

namespace Software
{
namespace Library
{
namespace GisEngine {

Geo2PositionItemTool::Geo2PositionItemTool(MapWidget *mapWidget, e2PositionItemType itemType)
	: GeoMultiPositionTool( mapWidget, eGeometryType::Polygon),
	  m_itemType(itemType)
{
	mToolName = tr( "2 Position Item Generator" );
}
void Geo2PositionItemTool::canvasMoveEvent( QgsMapMouseEvent *e )
{
//	m_mapWidget->mapCanvas()->mouseMoveEvent(e);

	if(m_pathItem->partSize(0) < 1)
		return;
	m_pathItem->reset();

	redraw(m_firstClickPosition, e->snapPoint(), e->modifiers());
}

void Geo2PositionItemTool::canvasReleaseEvent(QgsMapMouseEvent *e)
{
	if(e->button() == Qt::LeftButton)
	{
		if(m_pathItem->partSize(0) == 0) {
			m_firstClickPosition = e->snapPoint();
		}
		else
		{
			createNewMultiPosition();
			resetItems();
			return;
		}
	}else{

	}
	GeoMultiPositionTool::canvasReleaseEvent(e);
}

void Geo2PositionItemTool::resetItems()
{
	m_firstClickPosition = QgsPointXY(0,0);
	GeoMultiPositionTool::resetItems();
}

void Geo2PositionItemTool::redraw(QgsPointXY start, QgsPointXY end, Qt::KeyboardModifiers modifier)
{
	switch (m_itemType) {
	case e2PositionItemType::Circle:
	{
		QgsPointSequence points;
		if(modifier & Qt::ControlModifier)
		{
			points = QgsCircle().fromCenterPoint(QgsPoint(start), QgsPoint(end)).points();
		}else{
			points = QgsEllipse().fromCenterPoint(QgsPoint(start), QgsPoint(end)).points();
		}
		for(QgsPoint point : points)
			m_pathItem->addPoint(point);
		//	m_pathItem->addPoint(m_center);
		//	m_pathItem->addPoint(points.first());
		break;
	}
	case e2PositionItemType::Rectangle:
	{
		QRectF geometryRect;
		geometryRect = QgsRectangle(QgsPoint(start), QgsPoint(end)).toRectF();
		if(modifier & Qt::ControlModifier)
		{
			qreal diffx = (end.x() - start.x());
			qreal diffy = (end.y() - start.y());

			if(fabs(diffx) > fabs(diffy))
			{
				if(diffy * diffx < 0)
					diffx *= -1 ;
				end = QgsPointXY(end.x(),start.y() + diffx);
			}
			else{
				if(diffy * diffx < 0)
					diffy *= -1 ;
				end = QgsPointXY(start.x() + diffy,end.y());
			}
			geometryRect = QgsRectangle(QgsPoint(start), QgsPoint(end)).toRectF();
		}
		m_pathItem->addPoint(geometryRect.topLeft());
		m_pathItem->addPoint(geometryRect.topRight());
		m_pathItem->addPoint(geometryRect.bottomRight());
		m_pathItem->addPoint(geometryRect.bottomLeft());
		break;
	}
	default:
		break;
	}

}
}
}
}
