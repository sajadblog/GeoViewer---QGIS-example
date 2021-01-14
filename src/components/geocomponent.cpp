#include "components/geocomponent.h"
#include "mapwidget.h"

#include <QDockWidget>

namespace Software
{
namespace Library
{
namespace GisEngine {
GeoComponent::GeoComponent(MapWidget *parent):
	QObject(parent), m_parent(parent)
{

}

QWidget *GeoComponent::getWidgetObject()
{
	if(!m_widget){
		if(m_dockWidget)
			if(m_dockWidget->widget())
				return m_dockWidget->widget();
		throw std::logic_error( "Requested panel has not widget implementation " );
	}
	return m_widget;
}

QDockWidget *GeoComponent::getDockObject()
{
	if(!m_dockWidget)
		throw std::logic_error( "Requested panel has not Component widget implementation " );
	return m_dockWidget;
}
}
}
}
