#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <GeoViewer/common/mapstructures.h>
#include <GeoViewer/common/mapenums.h>

#include <QWidget>
#include <QMap>
#include <qgis/qgsrubberband.h>

class QSettings;

namespace Ui {
class MapWidget;
}
namespace Software
{
namespace Library
{
namespace GisEngine {
class GeoItem;
class GeoDataContainer;
class GeoUnitManager;
class GeoMapCanvas;

class MapWidget : public QWidget
{
	Q_OBJECT
	Q_ENUM(eLayerType)
public:
	explicit MapWidget(QWidget *parent = nullptr);
	~MapWidget();

	//map
	GeoMapCanvas *mapCanvas() { return m_mapCanvas;}
	void setRotation(double rotation);
	double rotation();

	//Items
	GeoItem *addItem(eItemType type, quint64 id = 0, bool isPrimary = true);
	bool removeItem(GeoItem * item, bool isPrimary = true);
	QList<GeoItem *> getItemList(eItemType type);
	void removeAllItems();

	//Tools
	bool setActiveTool(eToolType tool);
	void resetMapTool();


	GeoDataContainer *getGeoDataContainer() ;
	GeoUnitManager *getGeoUnitManager() const;
	QSettings *getSettings() const;


public slots:
	//layers
	bool addLayer(QString Address, eLayerType type, QString LayerName = "");
	bool addLayerByDialog(eLayerType type);

private:
	void registerMetaTypes();
	void init();
	void initQGis();
	void createConnections();

	Ui::MapWidget *ui = nullptr;
	GeoMapCanvas *m_mapCanvas = nullptr;
	GeoDataContainer *m_geoDataContainer = nullptr;
	GeoUnitManager *m_geoUnitManager = nullptr;
	QSettings *m_settings = nullptr;


signals:
		void mouseCoodinateChanged(double x, double y);
		void clearedSelection();

};
}
}
}

#endif // MAPWIDGET_H
