#ifndef GEOMARKERITEM_H
#define GEOMARKERITEM_H

#include <qgis/qgsmapcanvasitem.h>
#include "geoitem.h"

#include <QPen>
#include <QVector3D>

#define TrajectoryMainPathID 0

namespace Software
{
namespace Library
{
namespace GisEngine {
class GeoTextItem;
class GeoMultiPositionItem;
class GeoColorizePathItem;
class GeoMarkerItem : public QgsMapCanvasItem , public GeoItem
{
	friend class MapWidget;
public:
	virtual eItemType type() override{return eItemType::marker;}


	QRectF boundingRect() const override;

	void updatePosition() override;
	virtual void setSelected(bool selected) ;
	virtual void setVisible(bool visible);

	QPixmap pixmap() const {return m_pixmap;}
	void setPixmap(const QPixmap &pixmap);
	void setPosition(QVector3D Position, quint64 DataSource  = TrajectoryMainPathID, bool PushToTrajectory = false);
	QVector3D position() const {return m_Position;}
	int iconSize() const { return m_iconSize;}
	void setIconSize(int iconSize);
	GeoTextItem *textItem();
	void createTextItem();
	bool visibleTextItem() const {return m_visibleTextItem;}
	void setVisibleTextItem(bool visibleTextItem);
	bool visibleColorizeTrajectoy() const { return m_visibleColorizeTrajectoy;}
	void setVisibleColorizeTrajectoy(bool visibleColorizeTrajectoy);
	void setVisibleDatasourceTrajectoy(quint64 Datasource, bool visibleDatasourceTrajectoy);
	bool visibleDatasourceTrajectoy(quint64 Datasource) ;
	GeoMultiPositionItem *getDatasourceTrajectory(quint64 Datasource);


protected:
	GeoMarkerItem(MapWidget *mapWidget, quint64 id = 0);
	void updateTextPosition(QPoint iconPosition);

	void paint( QPainter *p ) override;
	virtual void drawContent(QPainter *p);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)override;

	//Trajectory
	void addNewTrajectory(quint64 DataSource);

	QPixmap m_pixmap;
	int m_iconSize = 20;
	QPen m_selectPen;

	QgsPointXY m_xyPosition;
	QVector3D m_Position;
	GeoTextItem *m_textItem = nullptr;
	bool m_visibleTextItem = true;
	bool m_visibleMainTrajectoryItem = true;
	GeoColorizePathItem *m_colorizeTrajectoryItem = nullptr;
	bool m_visibleColorizeTrajectoy = false;
	QMap<quint64, GeoMultiPositionItem *> m_trajectoryMap;
	QMap<quint64, bool> m_trajectoryVisibilityMap;
private :
};
}
}
}
#endif // GEOMARKERITEM_H
