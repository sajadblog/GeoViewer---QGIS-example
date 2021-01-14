#ifndef GEODATACONTAINER_H
#define GEODATACONTAINER_H

#include <QObject>
#include <QMap>
#include <QPen>
#include <QFont>

#include <GeoViewer/common/mapstructures.h>
#include <GeoViewer/common/mapenums.h>



class QgsMapTool;

namespace Software
{
namespace Library
{
namespace GisEngine {
class GeoItem;
class GeoComponent;
class MapWidget;

class GeoDataContainer : public QObject
{
	Q_OBJECT
	friend class MapWidget;
	friend class GeoItem;

public:

	//Items
	GeoItem *getItem(eItemType type, quint64 id);
	QList<GeoItem *> getItemList(eItemType type);

	//Components
	GeoComponent *getComponent(eComponentType type);

	// tools

	//styles
	QList<double> getColorizeLevelValues() const {return m_colorizeLevelValues;}
	void setColorizeLevelValues(const QList<double> &colorizeLevelValues);
	quint16 getColorizePathLength() const {return m_colorizePathLength;}
	void setColorizePathLength(const quint16 &colorizePathLength);
	QPen getColorizePathPen() const{ return m_colorizePathPen;}
	void setColorizePathPen(const QPen &colorizePathPen);
	QPen getMeasuringToolPen() const { return m_measuringToolPen;}
	void setMeasuringToolPen(const QPen &measuringToolPen);
	QFont getGeneralFont() const { return m_generalFont;}
	void setGeneralFont(const QFont &generalFont);
	QPen getLinePen() const { return m_linePen;}
	void setLinePen(const QPen &linePen);
	QBrush getFillBrush() const { return m_fillBrush;}
	void setFillBrush(const QBrush &fillBrush);
	QBrush getMeasuringToolBrush() const { return m_measuringToolBrush;}
	void setMeasuringToolBrush(const QBrush &measuringToolBrush);

public slots:

signals:

private slots:
	void createConnections();
private :
	explicit GeoDataContainer(MapWidget *parent = nullptr);
	void createComponents();
	void createTools();
	void createGeneralValues();

	void addItem(GeoItem *item);
	bool removeItem(GeoItem * item);

	MapWidget *m_mapWidget;
	QMap< eItemType, QMap< quint64, GeoItem* > > m_itemContainers;
	//Styles
	QFont m_generalFont;
	QList<double> m_colorizeLevelValues;
	QPen m_colorizePathPen;
	quint16 m_colorizePathLength = 10;

	QPen m_linePen;
	QBrush m_fillBrush;
	QPen m_measuringToolPen;
	QBrush m_measuringToolBrush;

	//Components
	QMap<eComponentType, GeoComponent *> m_componentMap;

	// tools
	QMap<eToolType, QgsMapTool *> m_toolMap;


};
}
}
}
#endif // GEODATACONTAINER_H
