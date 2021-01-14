#ifndef GEOBROWSERComponent_H
#define GEOBROWSERComponent_H

#include "geocomponent.h"
#include <qgis/qgsbrowserdockwidget.h>
#include <qgis/qgsbrowserguimodel.h>

namespace Software
{
namespace Library
{
namespace GisEngine {
class GeoBrowserComponent : public  GeoComponent
{
	Q_OBJECT
public:
	GeoBrowserComponent(MapWidget *parent = nullptr);
private :
	QgsBrowserDockWidget *m_browserDockWidget;
	QgsBrowserGuiModel *m_browserGuiModel;

private slots:
	//! Process the list of URIs that have been dropped in QGIS
	void handleDropUriList( const QgsMimeDataUtils::UriList &lst );

};
}
}
}
#endif // GEOBROWSERComponent_H
