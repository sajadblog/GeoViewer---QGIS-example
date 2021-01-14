#ifndef GEOCOLORIZEPATHITEM_H
#define GEOCOLORIZEPATHITEM_H

#include "items/geoitem.h"

#include <QPen>

namespace Software
{
namespace Library
{
namespace GisEngine {
class GeoMultiPositionItem;
class GeoColorizePathItem : public GeoItem
{
	friend class MapWidget;
	friend class GeoMarkerItem;
public:
	virtual eItemType type() override{return eItemType::ColorizePath;}

	virtual void setVisible(bool visible);

	void addPoint(QVector3D position);
	void clear();

	QList<double> valueLevel() const {return m_valueLevel;}
	void setValueLevel(const QList<double> &valueLevel);
	QList<QVector3D> pointList() const{return m_pointList;}
	void setPointList(const QList<QVector3D> &pointList);
	quint8 width() const{return m_width;}
	void setWidth(const quint8 &width);
	Qt::PenStyle penStyle() const{return m_penStyle;}
	void setPenStyle(const Qt::PenStyle &penStyle);
	quint16 length() const{return m_length;}
	void setLength(const quint16 &length);

protected:
	GeoColorizePathItem(MapWidget *mapWidget, quint64 id = 0);

private:
	void clearItems();
	void updateLastPositionOfLevels(QVector3D position);
	void checkRemoveFirstPositions();
	void repaint();
	QColor calculateLevelColor(double value);

	QList<GeoMultiPositionItem *> m_levelItemList;
	QMutex m_levelRefreshMutex;

	QList<QVector3D> m_pointList;
	QList<double> m_valueLevel;
	quint8 m_width = 0;
	quint16 m_length = 0;
	Qt::PenStyle m_penStyle = Qt::NoPen;
};
}
}
}

#endif // GEOCOLORIZEPATHITEM_H
