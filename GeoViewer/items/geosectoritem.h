#ifndef GEOSECTORITEM_H
#define GEOSECTORITEM_H

#include "geomultipositionItem.h"

namespace Software
{
namespace Library
{
namespace GisEngine {

class GeoSectorItem : public GeoMultiPositionItem
{
	friend class MapWidget;
public:
	virtual eItemType type() override {return eItemType::Sector;}

	sZoneInfo zoneInfo() const;
	void setZoneInfo(const sZoneInfo &zoneInfo);
	void refresh();

	void setZValue(int zValue);

protected:
	GeoSectorItem(MapWidget *mapWidget ,quint64 id = 0);
private :
	sZoneInfo m_zoneInfo;

	void clear();
};

}
}
}
#endif // GEOSECTORITEM_H
