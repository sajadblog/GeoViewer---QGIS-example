#ifndef GEOTEXTITEM_H
#define GEOTEXTITEM_H

#include <qgis/qgsmapcanvasitem.h>

#include "GeoViewer/common/mapstructures.h"
#include "geoitem.h"

#include <QFontMetrics>

namespace Software
{
namespace Library
{
namespace GisEngine {

class GeoTextItem : public QgsMapCanvasItem , public GeoItem
{
	friend class MapWidget;
	friend class GeoMarkerItem;
public:
	virtual eItemType type() override {return eItemType::Text;}
	void paint( QPainter *p ) override;

	QRectF boundingRect() const override;

	void updatePosition() override;

	QPointF Position() const;
	void setPosition(const QPointF &Position);

	QString text() const;
	void setText(const QString &text);
	QFont Font() const;
	void setFont(const QFont &Font);
	eHorizontalAlignment horizontalAlignment() const;
	void setHorizontalAlignment(const eHorizontalAlignment &horizontalAlignment);
	eVerticalAlignment VerticalAlignment() const;
	void setVerticalAlignment(const eVerticalAlignment &VerticalAlignment);
	void setZValue(int zValue);
	void setVisible(bool visible);
	quint16 textHeight() const;
	quint16 textWidth() const;



protected:
	GeoTextItem(MapWidget *mapWidget , quint64 id = 0);
private :
	void refreshText();

	QString m_text = "";
	QFont m_Font ;
	QFontMetrics m_FontMetrics = QFontMetrics(Font());
	quint16 m_textWidth = 0;
	quint16 m_textHeight = 0;
	quint16 m_textHalfWidth = 0;
	quint16 m_textHalfHeight = 0;
	QgsPointXY m_Position;
	eHorizontalAlignment m_horizontalAlignment = eHorizontalAlignment::Center;
	eVerticalAlignment m_VerticalAlignment = eVerticalAlignment::Center;
};
}
}
}
#endif // GEOTEXTITEM_H
