
#include "items/geocolorizepathitem.h"
#include <items/geomultipositionItem.h>
#include "mapwidget.h"

#include "common/geodatacontainer.h"

#include <QVector3D>

namespace Software
{
namespace Library
{
namespace GisEngine {
GeoColorizePathItem::GeoColorizePathItem(Software::Library::GisEngine::MapWidget *mapWidget, quint64 id):
	GeoItem (mapWidget, id)
{

}

void GeoColorizePathItem::clearItems()
{
	QListIterator<GeoMultiPositionItem *> itemIterator(m_levelItemList);
	while (itemIterator.hasNext()){
		GeoMultiPositionItem *pol = itemIterator.next();
		for(GeoItem *item : pol->itemList()) {
			m_itemList.removeOne(item);
		}
		m_mapWidget->removeItem(pol, false);
	}
	m_levelItemList.clear();
}

void GeoColorizePathItem::updateLastPositionOfLevels(QVector3D newPosition)
{
	if(m_pointList.size() < 2)
		return;

	QMutexLocker lock(&m_levelRefreshMutex);

	quint8 pathWidth = m_width ? m_width :m_mapWidget->getGeoDataContainer()->getColorizePathPen().width();
	Qt::PenStyle pathPenStyle = m_width ? m_penStyle :m_mapWidget->getGeoDataContainer()->getColorizePathPen().style();

	QColor m_currentTargetLevel = calculateLevelColor((double)newPosition.z()) ;
	QColor m_lastTargetLevel = calculateLevelColor((double)m_pointList.at(m_pointList.size() - 2).z());
	GeoMultiPositionItem *newItem = nullptr;
	if((m_currentTargetLevel != m_lastTargetLevel )||(m_pointList.size() == 2)){
		newItem = dynamic_cast<GeoMultiPositionItem *>(m_mapWidget->addItem(eItemType::MultiPosition, 0 ,false));
		newItem->reset(eGeometryType::Line);
		m_levelItemList.append(newItem);
		m_itemList.append(newItem);
		newItem->setColor(m_currentTargetLevel);
		newItem->setWidth(pathWidth);
		newItem->setLineStyle(pathPenStyle);
		m_lastTargetLevel = m_currentTargetLevel ;
		newItem->addPoint(QPointF((double)m_pointList.at(m_pointList.size() - 2).x(),(double)m_pointList.at(m_pointList.size() - 2 ).y()));
	}
	if(!m_levelItemList.isEmpty()) {
		m_levelItemList.last()->addPoint(QPointF((double)newPosition.x(),(double)newPosition.y()));
		m_levelItemList.last()->setVisible(m_visible);
	}

	checkRemoveFirstPositions();
}

void GeoColorizePathItem::checkRemoveFirstPositions()
{
	quint16 pathLength = m_length ? m_length :m_mapWidget->getGeoDataContainer()->getColorizePathLength();
	while(m_pointList.size() > pathLength)
	{
		GeoMultiPositionItem *firstPath = m_levelItemList.first();
		firstPath->removePoint(0);
		if(firstPath->numberOfVertices() == 1)
		{
			m_levelItemList.removeFirst();
			m_itemList.removeOne(firstPath);
			m_mapWidget->removeItem(firstPath, false);
		}
		m_pointList.removeFirst();
	}
}

void GeoColorizePathItem::repaint()
{

	QMutexLocker lock(&m_levelRefreshMutex);
	clearItems();

	for(QVector3D position : m_pointList)
		updateLastPositionOfLevels(position);
}
void GeoColorizePathItem::setPointList(const QList<QVector3D> &pointList)
{
	m_pointList = pointList;

	quint16 pathLength = m_length ? m_length :m_mapWidget->getGeoDataContainer()->getColorizePathLength();
	while(m_pointList.size() > pathLength){
		m_pointList.removeFirst();
	}

	repaint();
}
void GeoColorizePathItem::setValueLevel(const QList<double> &valueLevel)
{
	m_valueLevel = valueLevel;
	repaint();
}

void GeoColorizePathItem::setVisible(bool visible)
{
	GeoItem::setVisible(visible);
}

void GeoColorizePathItem::addPoint(QVector3D position)
{
	m_pointList.append(position);
	updateLastPositionOfLevels(position);

}

void GeoColorizePathItem::clear()
{
	clearItems();
	m_pointList.clear();
}

QColor GeoColorizePathItem::calculateLevelColor(double value)
{
	QList<double > levelValue = m_valueLevel.isEmpty() ? m_mapWidget->getGeoDataContainer()->getColorizeLevelValues() : m_valueLevel;
	if(levelValue.isEmpty())
		return Qt::black;

	double minHueColor = 60.0;
	double maxHueColor = 359.0;

	double retValue = minHueColor ;

	if(value >= levelValue.last())
		retValue = maxHueColor ;
	else if(value < 0)
		retValue = minHueColor ;
	else{
		for(int i = 0 ; i < levelValue.size() - 1; i++)
		{
			if((value >= levelValue.at(i))&&
					(value < levelValue.at(i + 1)))
			{
				retValue = (i * 1.0 / levelValue.size()) * (maxHueColor - minHueColor);
				retValue += minHueColor;
			}
		}
	}
	return QColor::fromHsv((int)retValue,255,255);
}
void GeoColorizePathItem::setLength(const quint16 &length)
{
	m_length = length;
}
void GeoColorizePathItem::setPenStyle(const Qt::PenStyle &penStyle)
{
	m_penStyle = penStyle;
	repaint();
}
void GeoColorizePathItem::setWidth(const quint8 &width)
{
	m_width = width;
	repaint();
}
}
}
}
