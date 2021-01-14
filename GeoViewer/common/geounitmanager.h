#ifndef GEOUNITMANAGER_H
#define GEOUNITMANAGER_H

#include <QObject>

#include <qgis/qgsunittypes.h>

#include "lib/qdateconvertor/qdateconvertor.h"
#include "mapenums.h"

namespace Software
{
namespace Library
{
namespace GisEngine {
class GeoUnitManager : public QObject
{
	Q_OBJECT
public:
	explicit GeoUnitManager(QObject *parent = nullptr);

	//convert to global style
	double getDistance(double distance ,QgsUnitTypes::DistanceUnit inputUnit = QgsUnitTypes::DistanceMeters);
	double getHeight(double height ,QgsUnitTypes::DistanceUnit inputUnit = QgsUnitTypes::DistanceMeters);
	double getAngle(double angle ,QgsUnitTypes::AngleUnit inputUnit = QgsUnitTypes::AngleDegrees);
	double getSpeed(double speed ,eSpeedUnit inputUnit = eSpeedUnit::MPS);
	QString getDate(quint64 date );
	QString getDateTime(quint64 dateTime );
	QString getTime(quint64 time );
	QString getCoordinate(QString coordinate, eCoordinateAppearanceType inputUnit = eCoordinateAppearanceType::DD);
	QString getCoordinateFromDD(double coordinate);

	//units strings
	QString formatDistance(double distance, int decimals = 4, QgsUnitTypes::DistanceUnit unit = QgsUnitTypes::DistanceMeters);
	QString formatHeight(double Height, int decimals = 4, QgsUnitTypes::DistanceUnit unit  = QgsUnitTypes::DistanceMeters);
	QString formatAngle(double angle, int decimals = 4, QgsUnitTypes::AngleUnit unit = QgsUnitTypes::AngleDegrees);
	QString formatSpeed();
	QString formatDate();

	//convert methods
	static QString degreeToDegreeMinute(double coordinate);
	static QString degreeToDegreeMinuteSecondString(double coordinate);
	static QList<float> degreeToDegreeMinuteSecond(double coordinate);
	static double degreeFromDegreeMinute(QString coordinate);
	static double degreeFromDegreeMinuteSecond(QString coordinate);
	static double degreeFromDegreeMinuteSecond(QList<float> coordinate);
	QString converCoordinate(QString inputValue, eCoordinateAppearanceType inputUnit, eCoordinateAppearanceType outputUnit);
	double converDistance(double inputValue, QgsUnitTypes::DistanceUnit inputUnit, QgsUnitTypes::DistanceUnit outputUnit);
	double converAngle(double inputValue, QgsUnitTypes::AngleUnit inputUnit, QgsUnitTypes::AngleUnit outputUnit);
	double converSpeed(double inputValue, eSpeedUnit inputUnit, eSpeedUnit outputUnit);

	//accessors
	QgsUnitTypes::DistanceUnit distanceUnit() const;
	void setDistanceUnit(const QgsUnitTypes::DistanceUnit &distanceUnit);
	eCoordinateAppearanceType coordinateAppearanceType() const;
	void setCoordinateAppearanceType(const eCoordinateAppearanceType &coordinateAppearanceType);
	QgsUnitTypes::DistanceUnit getHeightUnit() const;
	void setHeightUnit(const QgsUnitTypes::DistanceUnit &heightUnit);
	eSpeedUnit getSpeedUnit() const;
	void setSpeedUnit(const eSpeedUnit &speedUnit);
	eDateUnit getDateUnit() const;
	void setDateUnit(const eDateUnit &dateUnit);
	QgsUnitTypes::AngleUnit getAngleUnit() const;
	void setAngleUnit(const QgsUnitTypes::AngleUnit &angleUnit);

signals:
	void distanceUnitChanged(QgsUnitTypes::DistanceUnit);
	void heightUnitChanged(QgsUnitTypes::DistanceUnit);
	void angleUnitChanged(QgsUnitTypes::AngleUnit);
	void speedUnitChanged(eSpeedUnit);
	void coordinateAppearanceTypeChanged(eCoordinateAppearanceType);
	void dateUnitChanged(eDateUnit);

private slots:


private:

	//distance
	double mpsConvertor(eSpeedUnit secondaryUnit,double value, bool toMps);

	QgsUnitTypes::DistanceUnit m_distanceUnit = QgsUnitTypes::DistanceMeters;
	QgsUnitTypes::DistanceUnit m_heightUnit = QgsUnitTypes::DistanceMeters;
	QgsUnitTypes::AngleUnit m_angleUnit = QgsUnitTypes::AngleDegrees;
	eSpeedUnit m_speedUnit = eSpeedUnit::MPS;
	eDateUnit m_dateUnit = eDateUnit::Gregorian;
	eCoordinateAppearanceType m_coordinateAppearanceType = eCoordinateAppearanceType::DD;

	QDateConvertor m_dateConvertor;
};
}
}
}
#endif // GEOUNITMANAGER_H
