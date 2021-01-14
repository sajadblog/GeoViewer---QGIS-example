#ifndef GEOSIMPLEMARKERITEM_H
#define GEOSIMPLEMARKERITEM_H

#include "geomarkeritem.h"
#include "GeoViewer/common/mapenums.h"

namespace Software
{
namespace Library
{
namespace GisEngine {

class GeoSimpleMarkerItem: public GeoMarkerItem
{
	friend class MapWidget;
public:
	virtual eItemType type() override {return eItemType::SimpleMarker;}

	void setIconType( eSimpleMarkerSymbolType iconType );
	void setColor( const QColor &color );
	QColor color() const { return m_color; }
	void setFillColor( const QColor &color );
	QColor fillColor() const { return m_fillColor; }
	void setPenWidth( int width );

	virtual void drawContent( QPainter *p );
	QRectF boundingRect() const override;
protected:
	GeoSimpleMarkerItem(MapWidget *mapWidget ,quint64 id = 0);

private :
	int m_penWidth = 1;
	QColor m_color = QColor( 255, 0, 0 );
	QColor m_fillColor = QColor( 0, 0, 0, 0 );
	eSimpleMarkerSymbolType m_iconType = eSimpleMarkerSymbolType::X;

};
}
}
}

#endif // GEOSIMPLEMARKERITEM_H
