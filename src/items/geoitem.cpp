#include "items/geoitem.h"
#include "mapwidget.h"
#include "widgets/geomapcanvas.h"

#include <qgis/qgsmapcanvasitem.h>

namespace Software
{
namespace Library
{
namespace GisEngine {

GeoItem::GeoItem(MapWidget *mapWidget ,quint64 id):
	m_mapWidget(mapWidget),
	m_id(id)
{
	m_uuid = quint64(qrand()) | (quint64(qrand()) << 32);
	m_id = ( m_id == 0) ? m_uuid : m_id ;
}

GeoItem::~GeoItem()
{
	if(m_canvasItem)
		m_mapWidget->mapCanvas()->scene()->removeItem(m_canvasItem);

	for(GeoItem * item : m_itemList){
		if(item)
		{
			delete item;
		}
	}
}

bool GeoItem::selectable() const
{
	return m_selectable;
}

void GeoItem::setSelectable(bool selectable)
{
	m_selectable = selectable;
}

bool GeoItem::selected() const
{
	return m_selected;
}

void GeoItem::setSelected(bool selected)
{
	if(selected && !m_selectable)
		return;
	m_selected = selected;
	foreach (GeoItem *item, m_itemList) {
		item->setSelected(selected);
	}

}

bool GeoItem::visible() const
{
	return m_visible;
}

void GeoItem::setVisible(bool visible)
{
	m_visible = visible;
	if(m_canvasItem)
		m_canvasItem->setVisible(visible);

	foreach (GeoItem *item, m_itemList) {
		item->setVisible(visible);
	}
}

int GeoItem::zValue() const
{
	return m_zValue;
}

void GeoItem::setZValue(int zValue)
{
	m_zValue = zValue;
	if(m_canvasItem)
		m_canvasItem->setZValue(zValue);

	foreach (GeoItem *item, m_itemList) {
		item->setZValue( --zValue);
	}
}

quint64 GeoItem::uuid() const
{
	return m_uuid;
}

quint64 GeoItem::id() const
{
	return m_id;
}

QList<GeoItem *> GeoItem::itemList() const
{
	return m_itemList;
}

void GeoItem::addItem(GeoItem *newItem)
{
	m_itemList.append(newItem);
}

void GeoItem::removeItem(GeoItem *newItem)
{
	m_itemList.removeOne(newItem);
}

QgsMapCanvasItem *GeoItem::canvasItem() const
{
	return m_canvasItem;
}
}
}
}
