#ifndef GEOMULTIPOSITIONTOOL_H
#define GEOMULTIPOSITIONTOOL_H

#include "qgsmaptool.h"
#include "common/mapenums.h"

class QgsDisplayAngle;
class QgsRubberBand;
class QgsDistanceArea;

namespace Software
{
namespace Library
{
namespace GisEngine {
class GeoTextItem;
class GeoMultiPositionItem;
class MapWidget;

class GeoMultiPositionTool: public QgsMapTool
{
    Q_OBJECT
  public:
	GeoMultiPositionTool(MapWidget *mapWidget , Software::Library::GisEngine::eGeometryType geometryType);
	~GeoMultiPositionTool() override;

    Flags flags() const override { return QgsMapTool::AllowZoomRect; }

	virtual void canvasMoveEvent( QgsMapMouseEvent *e ) override;
	virtual void canvasReleaseEvent( QgsMapMouseEvent *e ) override;
	virtual void activate() override;
	virtual void deactivate() override;

	virtual void updateRubberBandStyle();

	eGeometryType geometryType() const;

protected :
	virtual void resetItems();
	virtual void createNewMultiPosition();

	eGeometryType m_geometryType = eGeometryType::Line;
	MapWidget *m_mapWidget;
	bool freeze = false;
	GeoMultiPositionItem *m_pathItem = nullptr;

  private:

};
}
}
}
#endif // GEOMULTIPOSITIONTOOL_H
