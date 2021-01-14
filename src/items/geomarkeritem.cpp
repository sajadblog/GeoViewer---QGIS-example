#include "items/geomarkeritem.h"
#include "items/geotextitem.h"
#include "items/geomultipositionItem.h"
#include "items/geocolorizepathitem.h"
#include "mapwidget.h"
#include "widgets/geomapcanvas.h"
#include "common/geodatacontainer.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>

namespace Software
{
namespace Library
{
namespace GisEngine {

GeoMarkerItem::GeoMarkerItem(MapWidget *mapWidget , quint64 id) :
	QgsMapCanvasItem(mapWidget->mapCanvas()),GeoItem(mapWidget ,id)
{
	m_canvasItem = this;

	m_colorizeTrajectoryItem = dynamic_cast<GeoColorizePathItem *>(m_mapWidget->addItem(eItemType::ColorizePath, 0, false));
	m_colorizeTrajectoryItem->setVisible(m_visibleColorizeTrajectoy);
	m_itemList.append(m_colorizeTrajectoryItem);
	setFlags(flags() | QGraphicsItem::ItemIsSelectable);

	QBrush selectBrush;
	selectBrush.setColor(QColor(0, 255, 0));
	m_selectPen.setBrush(selectBrush);
	m_selectPen.setColor(Qt::red);
	m_selectPen.setWidth(2);
}

void GeoMarkerItem::updateTextPosition(QPoint iconPosition)
{
	if(m_textItem == nullptr)
		return;
	iconPosition.setY( iconPosition.y() + qCos(qDegreesToRadians(m_mapWidget->rotation())) * m_iconSize);
	iconPosition.setX( iconPosition.x() + qSin(qDegreesToRadians(m_mapWidget->rotation())) * m_iconSize);
	m_textItem->setPosition(toMapCoordinates(iconPosition).toQPointF());
}

void GeoMarkerItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QgsMapCanvasItem::mousePressEvent(event);
	setSelected(true);
}

void GeoMarkerItem::addNewTrajectory(quint64 DataSource)
{
	if(DataSource == TrajectoryMainPathID)
		return;
	if(m_trajectoryMap.contains(DataSource))
		return;
	GeoMultiPositionItem *newTrajectory = dynamic_cast<GeoMultiPositionItem *>(m_mapWidget->addItem(eItemType::MultiPosition, 0, false));
	newTrajectory->reset(eGeometryType::Line);
	newTrajectory->setColor(QColor(qrand() % 255,qrand() % 255,qrand() % 255));
	newTrajectory->setVisible(false);
	m_itemList.append(newTrajectory);
	m_trajectoryMap.insert(DataSource, newTrajectory);
	m_trajectoryVisibilityMap.insert(DataSource, newTrajectory->visible());
}

void GeoMarkerItem::setVisibleColorizeTrajectoy(bool visibleColorizeTrajectoy)
{
	m_visibleColorizeTrajectoy = visibleColorizeTrajectoy;
	if(isVisible())
		m_colorizeTrajectoryItem->setVisible(visibleColorizeTrajectoy);
}

void GeoMarkerItem::setVisibleDatasourceTrajectoy(quint64 Datasource, bool visibleDatasourceTrajectoy)
{
	if(!m_trajectoryVisibilityMap.contains(Datasource))
		return;
	m_trajectoryVisibilityMap[Datasource] = visibleDatasourceTrajectoy;
	if(isVisible())
		m_trajectoryMap[Datasource]->setVisible(visibleDatasourceTrajectoy);
}

bool GeoMarkerItem::visibleDatasourceTrajectoy(quint64 Datasource)
{
	if(!m_trajectoryVisibilityMap.contains(Datasource))
		return false;
	return m_trajectoryVisibilityMap[Datasource];
}

GeoMultiPositionItem *GeoMarkerItem::getDatasourceTrajectory(quint64 Datasource)
{
	if(!m_trajectoryMap.contains(Datasource))
		return nullptr;
	return m_trajectoryMap[Datasource];
}


void GeoMarkerItem::setVisibleTextItem(bool visibleTextItem)
{
	m_visibleTextItem = visibleTextItem;
}

GeoTextItem *GeoMarkerItem::textItem()
{
	if(m_textItem == nullptr)
		createTextItem();
	return m_textItem;
}

void GeoMarkerItem::createTextItem()
{
	m_textItem = dynamic_cast<GeoTextItem *>(m_mapWidget->addItem(eItemType::Text, 0, false));
	m_textItem->setHorizontalAlignment(eHorizontalAlignment::Center);
	m_textItem->setVerticalAlignment(eVerticalAlignment::Down);
	m_textItem->setVisible(m_visibleTextItem);

	m_itemList.append(m_textItem);
}

void GeoMarkerItem::paint(QPainter *p)
{
	if(m_selected)
	{
		QPen pPen = p->pen();
		p->setPen(m_selectPen);
		p->drawEllipse(boundingRect().toRect());
		p->setPen(pPen);
	}

	drawContent(p);
}

void GeoMarkerItem::drawContent(QPainter *p)
{
	p->drawPixmap(boundingRect().toRect(),m_pixmap);
}

QRectF GeoMarkerItem::boundingRect() const
{
	qreal s = qreal( m_iconSize ) / 2.0;
	return QRectF( -s, -s, 2.0 * s, 2.0 * s );
}

void GeoMarkerItem::updatePosition()
{
	QPointF pt = toCanvasCoordinates( m_xyPosition );
	setPos( pt );

	updateTextPosition(pt.toPoint());
}

void GeoMarkerItem::setSelected(bool selected)
{
	if(selected == m_selected)
		return;

	if(selected && !m_selectable)
		return;

	GeoItem::setSelected(selected);
	update();
}

void GeoMarkerItem::setVisible(bool visible)
{
	GeoItem::setVisible(visible);
	if(visible)
	{
		if(m_textItem)
			m_textItem->setVisible(m_visibleTextItem);
		m_colorizeTrajectoryItem->setVisible(m_visibleColorizeTrajectoy);
		for(quint64 datasource : m_trajectoryMap.keys())
			m_trajectoryMap[datasource]->setVisible(m_trajectoryVisibilityMap[datasource]);
	}
}

void GeoMarkerItem::setPixmap(const QPixmap &pixmap)
{
	m_pixmap = pixmap;
}

void GeoMarkerItem::setPosition(QVector3D Position, quint64 DataSource, bool PushToTrajectory)
{
	m_xyPosition = QgsPointXY(Position.toPointF());
	QPointF pt = toCanvasCoordinates( m_xyPosition );
	setPos( pt );

	updateTextPosition(pt.toPoint());

	if(PushToTrajectory)
	{
		if(DataSource == TrajectoryMainPathID)
		{
			m_colorizeTrajectoryItem->addPoint(Position);
		}else{
			if(!m_trajectoryMap.contains(DataSource))
				addNewTrajectory(DataSource);
			m_trajectoryMap[DataSource]->addPoint(Position.toPointF());
			setVisibleDatasourceTrajectoy(DataSource,m_trajectoryVisibilityMap[DataSource]);

			while(m_trajectoryMap[DataSource]->partSize(0) > m_mapWidget->getGeoDataContainer()->getColorizePathLength())
				m_trajectoryMap[DataSource]->removePoint();
		}
	}
}

void GeoMarkerItem::setIconSize(int iconSize)
{
	m_iconSize = iconSize;
}
}
}
}
