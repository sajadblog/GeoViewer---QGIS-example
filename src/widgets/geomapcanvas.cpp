#include "widgets/geomapcanvas.h"
#include "mapwidget.h"

namespace Software
{
namespace Library
{
namespace GisEngine {

GeoMapCanvas::GeoMapCanvas(QWidget *parent):
	QgsMapCanvas(parent)
{
	setDefaultMode(true);
}

bool GeoMapCanvas::setActiveTool(QgsMapTool * tool)
{
	setDefaultMode(false);
	setMapTool(tool);
	return true;
}

void GeoMapCanvas::mouseReleaseEvent(QMouseEvent *e)
{
	if(scene()->mouseGrabberItem())
		QGraphicsView::mouseReleaseEvent(e);
	else
		QgsMapCanvas::mouseReleaseEvent(e);
}

void GeoMapCanvas::mousePressEvent(QMouseEvent *e)
{
	if(( (e->button() == Qt::LeftButton) || (e->button() == Qt::RightButton) ) && m_defaultMode)
	{
			QGraphicsView::mousePressEvent(e);
			if(!scene()->mouseGrabberItem())
				emit dynamic_cast<MapWidget *>(parent())->clearedSelection();
	}
	else
		QgsMapCanvas::mousePressEvent(e);

}

void GeoMapCanvas::setDefaultMode(bool defaultMode)
{
	m_defaultMode = defaultMode;

	setInteractive(defaultMode);
}

void GeoMapCanvas::resetMapTool()
{
	setDefaultMode(true);

	this->unsetMapTool(this->mapTool());
}

}
}
}
