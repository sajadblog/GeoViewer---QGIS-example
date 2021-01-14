#include "items/geosimplemarkeritem.h"

#include <QPen>
#include <QBrush>
#include <QPainter>

namespace Software
{
namespace Library
{
namespace GisEngine {

GeoSimpleMarkerItem::GeoSimpleMarkerItem(MapWidget *mapWidget, quint64 id):
	GeoMarkerItem (mapWidget, id)
{

}

void GeoSimpleMarkerItem::setIconType(eSimpleMarkerSymbolType iconType)
{
	m_iconType = iconType;
	update();
}

void GeoSimpleMarkerItem::setColor(const QColor &color)
{
	m_color = color;
	update();
}

void GeoSimpleMarkerItem::setFillColor(const QColor &fillColor)
{
	m_fillColor = fillColor;
	update();
}

void GeoSimpleMarkerItem::setPenWidth(int width)
{
	m_penWidth = width;
	update();
}

void GeoSimpleMarkerItem::drawContent(QPainter *p)
{
	qreal s = ( m_iconSize - 1 ) / 2.0;

	QPen pen( m_color );
	pen.setWidth( m_penWidth );
	p->setPen( pen );
	QBrush brush( m_fillColor );
	p->setBrush( brush );

	switch ( m_iconType )
	{
	  case eSimpleMarkerSymbolType::None:
		break;

	  case eSimpleMarkerSymbolType::Cross:
		p->drawLine( QLineF( -s, 0, s, 0 ) );
		p->drawLine( QLineF( 0, -s, 0, s ) );
		break;

	  case eSimpleMarkerSymbolType::X:
		p->drawLine( QLineF( -s, -s, s, s ) );
		p->drawLine( QLineF( -s, s, s, -s ) );
		break;

	  case eSimpleMarkerSymbolType::Box:
		p->drawLine( QLineF( -s, -s, s, -s ) );
		p->drawLine( QLineF( s, -s, s, s ) );
		p->drawLine( QLineF( s, s, -s, s ) );
		p->drawLine( QLineF( -s, s, -s, -s ) );
		break;

	  case eSimpleMarkerSymbolType::Circle:
		p->drawEllipse( QPointF( 0, 0 ), s, s );
		break;

	  case eSimpleMarkerSymbolType::DoubleTrianle:
		p->drawLine( QLineF( -s, -s,  s, -s ) );
		p->drawLine( QLineF( -s,  s,  s,  s ) );
		p->drawLine( QLineF( -s, -s,  s,  s ) );
		p->drawLine( QLineF( s, -s, -s,  s ) );
		break;
	}
}

QRectF GeoSimpleMarkerItem::boundingRect() const
{
	qreal s = qreal( m_iconSize + m_penWidth ) / 2.0;
	return QRectF( -s, -s, 2.0 * s, 2.0 * s );
}
}
}
}
