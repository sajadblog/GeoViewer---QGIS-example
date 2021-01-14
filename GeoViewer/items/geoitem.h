#ifndef GEOITEM_H
#define GEOITEM_H
#include <QtCore>
#include "GeoViewer/common/mapstructures.h"
#include "GeoViewer/common/mapenums.h"

#pragma clang diagnostic ignored "-Woverloaded-virtual"
#pragma clang diagnostic ignored "-Wold-style-cast"

class QgsMapCanvasItem;
namespace Software
{
namespace Library
{
namespace GisEngine {

class MapWidget;
class GeoItem
{
	friend class MapWidget;
public:

	quint64 uuid() const;
	quint64 id() const;

	virtual eItemType type() = 0;

	QList<GeoItem *> itemList() const;
	void addItem(GeoItem *newItem);
	void removeItem(GeoItem *newItem);

	QgsMapCanvasItem *canvasItem() const;

	int zValue() const;
	virtual void setZValue(int zValue);
	bool visible() const;
	virtual void setVisible(bool visible);
	bool selected() const;
	virtual void setSelected(bool selected) ;
	bool selectable() const;
	virtual void setSelectable(bool selectable);



protected:
	GeoItem(MapWidget *mapWidget, quint64 id = 0);
	virtual ~GeoItem();

	QgsMapCanvasItem *m_canvasItem = nullptr;
	MapWidget *m_mapWidget = nullptr;
	QList<GeoItem *> m_itemList;

	quint64 m_uuid;
	quint64 m_id;
	int m_zValue = 10;
	bool m_visible = true;
	bool m_selected = false;
	bool m_selectable = false;

private :

};
}
}
}

#endif // GEOITEM_H
