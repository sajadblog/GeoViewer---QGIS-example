#include "items/geotextitem.h"
#include "common/geodatacontainer.h"
#include "mapwidget.h"
#include "widgets/geomapcanvas.h"


#include <QPainter>
namespace Software
{
namespace Library
{
namespace GisEngine {

GeoTextItem::GeoTextItem(MapWidget *mapWidget , quint64 id) :
	QgsMapCanvasItem(mapWidget->mapCanvas()),GeoItem(mapWidget ,id)
{
	m_canvasItem = this;

	if(m_mapWidget->getGeoDataContainer())
	{
		setFont(m_mapWidget->getGeoDataContainer()->getGeneralFont());
	}

}

void GeoTextItem::refreshText()
{
	m_textWidth = qreal( m_FontMetrics.width(m_text) ) ;
	m_textHeight = qreal( m_FontMetrics.height() ) ;
	m_textHalfWidth = m_textWidth / 2.0;
	m_textHalfHeight = m_textHeight / 2.0;
	update();
}

quint16 GeoTextItem::textWidth() const
{
	return m_textWidth;
}

quint16 GeoTextItem::textHeight() const
{
	return m_textHeight;
}

void GeoTextItem::paint(QPainter *p)
{
	p->setFont(m_Font);
	p->drawText(boundingRect(), m_text);
}

QRectF GeoTextItem::boundingRect() const
{
	return QRectF( -m_textHalfWidth, -m_textHalfHeight, m_textWidth, m_textHeight );
}

void GeoTextItem::updatePosition()
{
	QPointF pt = toCanvasCoordinates( m_Position );
	switch (m_horizontalAlignment) {
	case eHorizontalAlignment::Left:
		pt.setX(pt.x() - m_textHalfWidth);break;
	case eHorizontalAlignment::Right:
		pt.setX(pt.x() + m_textHalfWidth);break;
	case eHorizontalAlignment::Center:break;
	}
	switch (m_VerticalAlignment) {
	case eVerticalAlignment::Up:
		pt.setY(pt.y() - m_textHalfHeight);break;
	case eVerticalAlignment::Down:
		pt.setY(pt.y() + m_textHalfHeight);break;
	case eVerticalAlignment::Center:break;
	}
	setPos( pt );
}

QString GeoTextItem::text() const
{
	return m_text;
}

void GeoTextItem::setText(const QString &text)
{
	m_text = text;
	refreshText();
}

QFont GeoTextItem::Font() const
{
	return m_Font;
}

void GeoTextItem::setFont(const QFont &Font)
{
	m_Font = Font;
	m_FontMetrics = QFontMetrics(m_Font);
	refreshText();
}

eHorizontalAlignment GeoTextItem::horizontalAlignment() const
{
	return m_horizontalAlignment;
}

void GeoTextItem::setHorizontalAlignment(const eHorizontalAlignment &horizontalAlignment)
{
	m_horizontalAlignment = horizontalAlignment;
	refreshText();
}

eVerticalAlignment GeoTextItem::VerticalAlignment() const
{
	return m_VerticalAlignment;
}

void GeoTextItem::setVerticalAlignment(const eVerticalAlignment &VerticalAlignment)
{
	m_VerticalAlignment = VerticalAlignment;
	refreshText();
}

void GeoTextItem::setZValue(int zValue)
{
	GeoItem::setZValue(zValue);
}

void GeoTextItem::setVisible(bool visible)
{
	GeoItem::setVisible(visible);
}

QPointF GeoTextItem::Position() const
{
	return m_Position.toQPointF();
}

void GeoTextItem::setPosition(const QPointF &Position)
{
	m_Position = QgsPointXY(Position);
	updatePosition();
}

}
}
}
