#ifndef GEOMAPCANVAS_H
#define GEOMAPCANVAS_H
#include "common/mapenums.h"
#include <qgis/qgsmapcanvas.h>

namespace Software
{
namespace Library
{
namespace GisEngine {

class GeoMapCanvas : public QgsMapCanvas
{
public:
	GeoMapCanvas( QWidget *parent = nullptr );

	bool setActiveTool(QgsMapTool *tool);
	void resetMapTool();

protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

private :
	void setDefaultMode(bool defaultMode);

	bool m_defaultMode = true;


};
}
}
}

#endif // GEOMAPCANVAS_H
