#include "GeoViewer/items/geosectoritem.h"

#include <QGeoCoordinate>

#include "widgets/geomapcanvas.h"
#include <qgis/qgsproject.h>
#include <qgis/qgsdatumtransformdialog.h>


namespace Software
{
namespace Library
{
namespace GisEngine {

GeoSectorItem::GeoSectorItem(MapWidget *mapWidget , quint64 id):
	GeoMultiPositionItem(mapWidget,eGeometryType::Polygon,id)
{
	setColor(QColor(255,0,0,100));
	setFillColor(QColor(255,255,0,100));
}

void GeoSectorItem::clear()
{
	reset(m_geometryType);
}

sZoneInfo GeoSectorItem::zoneInfo() const
{
	return m_zoneInfo;
}

void GeoSectorItem::setZoneInfo(const sZoneInfo &zoneInfo)
{
	if(m_zoneInfo == zoneInfo)
		return;

	m_zoneInfo = zoneInfo;
	refresh();
}

void GeoSectorItem::refresh()
{
	if(m_zoneInfo.minAzimuth == m_zoneInfo.maxAzimuth) {
		return;
	}
	if(m_zoneInfo.minRange == m_zoneInfo.maxRange) {
		return;
	}

	clear();

	QGeoCoordinate  cen(m_zoneInfo.center.x(),m_zoneInfo.center.y());
	float minAz = ((m_zoneInfo.minAzimuth * -1) + 90) ;
	float maxAz = ((m_zoneInfo.maxAzimuth * -1) + 90) ;

	if(minAz > maxAz)
	{
		float tmpValue;
		tmpValue = minAz;
		minAz = maxAz;
		maxAz = tmpValue;
	}

	for(float tmpDegree = minAz; tmpDegree <= maxAz ; tmpDegree += 10)
	{
		QGeoCoordinate newCord = cen.atDistanceAndAzimuth(m_zoneInfo.minRange, (double)tmpDegree);
		addPoint(QgsPointXY(newCord.latitude(),newCord.longitude()));
	}

	for(float tmpDegree2 = maxAz ; tmpDegree2 >= minAz ; tmpDegree2 -= 10 )
	{
		QGeoCoordinate newCord = cen.atDistanceAndAzimuth(m_zoneInfo.maxRange, (double)tmpDegree2);
		addPoint(QgsPointXY(newCord.latitude(),newCord.longitude()));
	}
}

void GeoSectorItem::setZValue(int zValue)
{
	GeoItem::setZValue(zValue);
}
}
}
}
