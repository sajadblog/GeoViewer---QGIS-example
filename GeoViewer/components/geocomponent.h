#ifndef GEOComponent_H
#define GEOComponent_H
#include <QWidget>
class QDockWidget;

namespace Software
{
namespace Library
{
namespace GisEngine {
class MapWidget;
class GeoComponent : public QObject
{
	Q_OBJECT
public:
	GeoComponent(MapWidget *parent = nullptr);
	virtual QWidget * getWidgetObject();
	virtual QDockWidget * getDockObject();

protected :
	QDockWidget * m_dockWidget = nullptr;
	QWidget * m_widget = nullptr;
	MapWidget * m_parent = nullptr;
};
}
}
}
#endif // GEOComponent_H
