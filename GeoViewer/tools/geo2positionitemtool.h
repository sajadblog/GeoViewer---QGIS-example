#ifndef GEOCIRCLETOOL_H
#define GEOCIRCLETOOL_H

#include "tools/geomultipositiontool.h"

#include <qgis/qgspointxy.h>

namespace Software
{
namespace Library
{
namespace GisEngine {
class Geo2PositionItemTool: public GeoMultiPositionTool
{
    Q_OBJECT
  public:
	Geo2PositionItemTool(MapWidget *mapWidget ,e2PositionItemType itemType);

	virtual void canvasMoveEvent( QgsMapMouseEvent *e ) override;
	virtual void canvasReleaseEvent( QgsMapMouseEvent *e ) override;
	virtual void resetItems();

  private:
	void redraw(QgsPointXY start, QgsPointXY end, Qt::KeyboardModifiers modifier = Qt::NoModifier);

	QgsPointXY	m_firstClickPosition = QgsPointXY(0,0);
	e2PositionItemType m_itemType;


};
}
}
}
#endif // GEOCIRCLETOOL_H
