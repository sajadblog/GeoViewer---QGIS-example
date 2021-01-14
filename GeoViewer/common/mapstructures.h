#ifndef MAPSTRUCTURES_H
#define MAPSTRUCTURES_H
#include <QPointF>

namespace Software
{
namespace Library
{
namespace GisEngine {

struct sZoneInfo{
	QPointF center;
	double minRange;
	double maxRange;
	float minAzimuth;
	float maxAzimuth;
	float stepValue;

	void operator=(sZoneInfo a)
	{
		this->center = a.center;
		this->minRange = a.minRange;
		this->maxRange = a.maxRange;
		this->minAzimuth = a.minAzimuth;
		this->maxAzimuth = a.maxAzimuth;
		this->stepValue = a.stepValue;
	}
	bool operator==(sZoneInfo a)
	{
		if((a.center == center )
				&&(a.minRange == minRange )
				&&(a.maxRange == maxRange )
				&&(a.minAzimuth == minAzimuth )
				&&(a.maxAzimuth == maxAzimuth )
				&&(a.stepValue == stepValue )
				)
			return true ;
		return false ;
	}
	bool sameShape(sZoneInfo a)
	{
		if((a.minRange == minRange )
				&&(a.maxRange == maxRange )
				&&(a.minAzimuth == minAzimuth )
				&&(a.maxAzimuth == maxAzimuth )
				&&(a.stepValue == stepValue )
				)
			return true ;
		return false ;
	}
};
}
}
}
#endif // MAPSTRUCTURES_H
