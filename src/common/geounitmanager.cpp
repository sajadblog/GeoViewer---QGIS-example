#include "common/geounitmanager.h"
#include <QDateTime>

#include "qmath.h"
#include "mapwidget.h"

#define KMPHRATIO  3.6

namespace Software
{
namespace Library
{
namespace GisEngine {

GeoUnitManager::GeoUnitManager(QObject *parent) : QObject(parent)
{
	qRegisterMetaType<QgsUnitTypes::DistanceUnit>("QgsUnitTypes::DistanceUnit");
}

double GeoUnitManager::getDistance(double distance, QgsUnitTypes::DistanceUnit inputUnit)
{
	return converDistance(distance , inputUnit, m_distanceUnit);
}

double GeoUnitManager::getSpeed(double speed ,eSpeedUnit inputUnit)
{
	return converSpeed(speed , inputUnit, m_speedUnit);
}

double GeoUnitManager::getHeight(double height, QgsUnitTypes::DistanceUnit inputUnit)
{
	return converDistance(height , inputUnit, m_heightUnit);
}

double GeoUnitManager::getAngle(double angle, QgsUnitTypes::AngleUnit inputUnit)
{
	return converAngle(angle , inputUnit, m_angleUnit);
}

QString GeoUnitManager::getDate(quint64 date)
{
	if(date == 0)
		return "----/--/--";
	switch (m_dateUnit) {
	case eDateUnit::Gregorian:
	{
		return QDateTime::fromMSecsSinceEpoch(date).toString("yyyy/MM/dd");
	}
	case eDateUnit::Jalali:
	{
		QDateTime currentTime = QDateTime::fromMSecsSinceEpoch(date);
		QStringList jalaliDate = m_dateConvertor.ToJalali(QString::number(currentTime.date().year()),
								 QString::number(currentTime.date().month()),
								 QString::number(currentTime.date().day()));
		return jalaliDate.at(0) + "/" + jalaliDate.at(1) + "/" + jalaliDate.at(2) ;
	}

	}
	return "----/--/--";
}

QString GeoUnitManager::getDateTime(quint64 dateTime)
{
	return getDate(dateTime) + " " + getTime(dateTime);
}

QString GeoUnitManager::getTime(quint64 time)
{
	if(time == 0)
		return "--:--:--";
	return QDateTime::fromMSecsSinceEpoch(time).toString("hh:mm:ss");
}

QString GeoUnitManager::getCoordinate(QString coordinate, eCoordinateAppearanceType inputUnit)
{
	return converCoordinate(coordinate , inputUnit, m_coordinateAppearanceType);
}

QString GeoUnitManager::getCoordinateFromDD(double coordinate)
{
	switch (m_coordinateAppearanceType) {
	case eCoordinateAppearanceType::DDM:
		return degreeToDegreeMinute(coordinate);
	case eCoordinateAppearanceType::DMS:
		return degreeToDegreeMinuteSecondString(coordinate);
	default:
		return QString::number(coordinate );
	}
}

QString GeoUnitManager::formatDistance(double distance, int decimals, QgsUnitTypes::DistanceUnit unit )
{
	return QgsUnitTypes::formatDistance(distance, decimals, unit);
}

QString GeoUnitManager::formatHeight(double Height, int decimals, QgsUnitTypes::DistanceUnit unit )
{
	return QgsUnitTypes::formatDistance(Height, decimals, unit);
}

QString GeoUnitManager::formatAngle(double angle, int decimals, QgsUnitTypes::AngleUnit unit )
{
	return QgsUnitTypes::formatAngle(angle, decimals, unit);
}

QString GeoUnitManager::formatSpeed()
{
	switch(m_speedUnit)
	{
	case eSpeedUnit::KMPH :return tr("kmph");
	case eSpeedUnit::MPS :
	default :
		return tr("mps");
	}
}

QString GeoUnitManager::formatDate()
{
	switch(m_dateUnit)
	{
	case eDateUnit::Jalali :return tr("Jalali");
	case eDateUnit::Gregorian :
	default :
		return tr("Gregorian");
	}
}

QgsUnitTypes::DistanceUnit GeoUnitManager::distanceUnit() const
{
	return m_distanceUnit;
}

void GeoUnitManager::setDistanceUnit(const QgsUnitTypes::DistanceUnit &distanceUnit)
{
	if(distanceUnit == m_distanceUnit)
		return;
	m_distanceUnit = distanceUnit;

	emit distanceUnitChanged(m_distanceUnit);
}

eCoordinateAppearanceType GeoUnitManager::coordinateAppearanceType() const
{
	return m_coordinateAppearanceType;
}

void GeoUnitManager::setCoordinateAppearanceType(const eCoordinateAppearanceType &coordinateAppearanceType)
{
	if(coordinateAppearanceType == m_coordinateAppearanceType)
		return;
	m_coordinateAppearanceType = coordinateAppearanceType;

	emit coordinateAppearanceTypeChanged(m_coordinateAppearanceType);
}

QString GeoUnitManager::degreeToDegreeMinuteSecondString(double coordinate)
{
	QString retValue;
	int sec = (int)round(coordinate * 3600);
	retValue.append(QString::number(sec / 3600) + "° ");
	sec = abs(sec % 3600);
	retValue.append(QString::number(sec / 60) + "\'");
	retValue.append(QString::number(sec % 60) + "\"");
	return retValue ;
}

QList<float> GeoUnitManager::degreeToDegreeMinuteSecond(double coordinate)
{
	QList<float> retList;
	int sec = (int)round(coordinate * 3600);
	retList.append(sec / 3600);
	sec = abs(sec % 3600);
	retList.append(sec / 60);
	retList.append(sec % 60);
	return retList;
}

double GeoUnitManager::degreeFromDegreeMinute(QString coordinate)
{
	QString min = coordinate.split(' ').last() ;
	coordinate.remove(min);
	min.remove('\'');

	QString degree = coordinate.split(' ').last() ;
	degree.remove("°");

	return degree.toDouble() + (min.toDouble() / 60.0);
}

double GeoUnitManager::degreeFromDegreeMinuteSecond(QString coordinate)
{
	QString sec = coordinate.split(' ').last() ;
	coordinate.remove(sec);
	sec.remove('\"');

	QString min = coordinate.split(' ').last() ;
	coordinate.remove(min);
	min.remove('\'');

	QString degree = coordinate.split(' ').last() ;
	degree.remove("°");

	return degree.toDouble() + ((min.toDouble() * 60 + sec.toDouble()) / 3600.0);
}

double GeoUnitManager::degreeFromDegreeMinuteSecond(QList<float> coordinate)
{
	return coordinate.first() + ((coordinate.at(1) * 60 + coordinate.at(2)) / 3600.0);
}

QString GeoUnitManager::converCoordinate(QString inputValue, eCoordinateAppearanceType inputUnit, eCoordinateAppearanceType outputUnit)
{
	double retValue ;
	switch (inputUnit) {
	case eCoordinateAppearanceType::DDM:
		retValue = degreeFromDegreeMinute(inputValue);
		break;
	case eCoordinateAppearanceType::DMS:
		retValue = degreeFromDegreeMinuteSecond(inputValue);
		break;
	default:
		retValue = inputValue.toDouble() ;
		break;
	}


	switch (outputUnit) {
	case eCoordinateAppearanceType::DDM:
		return degreeToDegreeMinute(retValue);
	case eCoordinateAppearanceType::DMS:
		return degreeToDegreeMinuteSecondString(retValue);
	default:
		return QString::number(retValue );
	}

}

double GeoUnitManager::mpsConvertor(eSpeedUnit secondaryUnit, double value, bool toMps)
{
	double ratio;

	switch(secondaryUnit)
	{
	case eSpeedUnit::KMPH:
		ratio = KMPHRATIO;
		break;
	default:
		return value;
	}

	if(toMps)
		return value / ratio;
	else
		return value * ratio;
}

QgsUnitTypes::AngleUnit GeoUnitManager::getAngleUnit() const
{
	return m_angleUnit;
}

void GeoUnitManager::setAngleUnit(const QgsUnitTypes::AngleUnit &angleUnit)
{
	if(m_angleUnit == angleUnit)
		return;
	m_angleUnit = angleUnit;
	emit angleUnitChanged(angleUnit);
}

eSpeedUnit GeoUnitManager::getSpeedUnit() const
{
	return m_speedUnit;
}

void GeoUnitManager::setSpeedUnit(const eSpeedUnit &speedUnit)
{
	if(speedUnit == m_speedUnit)
		return;

	m_speedUnit = speedUnit;

	emit speedUnitChanged(m_speedUnit);
}

eDateUnit GeoUnitManager::getDateUnit() const
{
	return m_dateUnit;
}

void GeoUnitManager::setDateUnit(const eDateUnit &dateUnit)
{

	if(dateUnit == m_dateUnit)
		return;

	m_dateUnit = dateUnit;

	emit dateUnitChanged(m_dateUnit);
}

QgsUnitTypes::DistanceUnit GeoUnitManager::getHeightUnit() const
{
	return m_heightUnit;
}

void GeoUnitManager::setHeightUnit(const QgsUnitTypes::DistanceUnit &heightUnit)
{
	if(heightUnit == m_heightUnit)
		return;

	m_heightUnit = heightUnit;

	emit heightUnitChanged(m_heightUnit);
}

double GeoUnitManager::converDistance(double inputValue, QgsUnitTypes::DistanceUnit inputUnit, QgsUnitTypes::DistanceUnit outputUnit)
{
	if(inputUnit == outputUnit)
		return inputValue ;

	return inputValue * QgsUnitTypes::fromUnitToUnitFactor( inputUnit, outputUnit );
}

double GeoUnitManager::converAngle(double inputValue, QgsUnitTypes::AngleUnit inputUnit, QgsUnitTypes::AngleUnit outputUnit)
{
	if(inputUnit == outputUnit)
		return inputValue ;

	return inputValue * QgsUnitTypes::fromUnitToUnitFactor( inputUnit, outputUnit );
}

double GeoUnitManager::converSpeed(double inputValue, eSpeedUnit inputUnit, eSpeedUnit outputUnit)
{
	if(inputUnit == outputUnit)
		return inputValue ;

	double retValue;

	retValue = mpsConvertor(inputUnit, inputValue, true);
	retValue = mpsConvertor(outputUnit, retValue, false);

	return retValue;
}

QString GeoUnitManager::degreeToDegreeMinute(double coordinate)
{
	QString retValue;
	int sec = (int)round(coordinate * 3600);
	retValue.append(QString::number(sec / 3600) + "° ");
	sec = abs(sec % 3600);
	retValue.append(QString::number((float)sec / 60.0) + "\'");
	return retValue ;

}
}
}
}
