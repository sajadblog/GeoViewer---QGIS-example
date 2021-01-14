#ifndef GEOMEASURETOOL_H
#define GEOMEASURETOOL_H

#include "tools/geomultipositiontool.h"

class QgsDistanceArea;

namespace Software
{
namespace Library
{
namespace GisEngine {
class GeoTextItem;
class MapWidget;

class GeoMeasureTool: public GeoMultiPositionTool
{
    Q_OBJECT
  public:
		GeoMeasureTool(MapWidget *mapWidget , eMeasureType measureType);
	~GeoMeasureTool();
    void activate() override;

protected :
	void canvasMoveEvent( QgsMapMouseEvent *e ) override;
	void canvasReleaseEvent( QgsMapMouseEvent *e ) override;
	virtual void resetItems();
	virtual void createNewMultiPosition();

private:
	GeoTextItem * m_currentTextItem ;
	QList<GeoTextItem *> m_textItemList ;
	QgsDistanceArea *m_distanceArea;
	eMeasureType m_measureType;


	float m_segmentAzimuth = 0;
	double m_segmentDistance = 0;
	double m_totalDistance = 0;

  public slots:

  private slots:
	void updateMeasureValues();
	void updateMeasurePosition();
	void configureDistanceArea();

};
}
}
}
#endif // GEOMEASURETOOL_H
