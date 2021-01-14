#ifndef GEOLAYERSCOMPONENT_H
#define GEOLAYERSCOMPONENT_H

#include "geocomponent.h"
#include "common/mapenums.h"

#include <qgis/qgslayertreeview.h>
#include <qgis/qgslayertreemapcanvasbridge.h>

namespace Software
{
namespace Library
{
namespace GisEngine {
class GeoLayersComponent: public  GeoComponent
{
	Q_OBJECT
public:
	GeoLayersComponent(MapWidget *parent = nullptr);
	void addLayerByDialog(QString &Address, eLayerType type, QString &layerName);

private :
	QgsLayerTreeView *m_LayerTreeView = nullptr;
	QgsLayerTreeModel *m_layerTreeModel;
	QgsLayerTreeMapCanvasBridge *m_LayerTreeCanvasBridge = nullptr;

signals:

private slots:
};
}
}
}
#endif // GEOLAYERSCOMPONENT_H
