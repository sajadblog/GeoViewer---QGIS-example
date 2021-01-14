#ifndef GEOMULTIPOSITIONITEM_H
#define GEOMULTIPOSITIONITEM_H

#include "QtCore"
#include "qgis/qgsrubberband.h"

#include "geoitem.h"
#include "GeoViewer/common/mapstructures.h"

namespace Software
{
namespace Library
{
namespace GisEngine {

class GeoMultiPositionItem: public QgsRubberBand, public GeoItem
{
	friend class MapWidget;
public:
	virtual eItemType type() override;
	virtual void setVisible(bool visible);

	eGeometryType geometryType() const;
	void reset();
	void reset(const eGeometryType &geometryType);

protected:
	GeoMultiPositionItem(MapWidget *mapWidget , eGeometryType geometryType = eGeometryType::Line ,quint64 id = 0);
protected:
	eGeometryType m_geometryType = eGeometryType::Line;
};
}
}
}

#endif // GEOMULTIPOSITIONITEM_H
