#include "components/geobrowsercomponent.h"
#include "mapwidget.h"

#include <qgis/qgspluginlayer.h>
#include <qgis/qgsapplication.h>
#include <qgis/qgspluginlayerregistry.h>
#include <qgis/qgsproject.h>
#include <qgis/qgslogger.h>

#include <qgsrasterlayer.h>



namespace Software
{
namespace Library
{
namespace GisEngine {
GeoBrowserComponent::GeoBrowserComponent(MapWidget *parent):
	GeoComponent(parent)
{
	m_browserGuiModel = new QgsBrowserGuiModel( this );
	m_browserDockWidget = new QgsBrowserDockWidget("Browser",m_browserGuiModel , parent);
	m_dockWidget = m_browserDockWidget ;

	connect( m_browserDockWidget, &QgsBrowserDockWidget::handleDropUriList, this, &GeoBrowserComponent::handleDropUriList );

}

void GeoBrowserComponent::handleDropUriList(const QgsMimeDataUtils::UriList &lst)
{
	for ( int i = lst.size() - 1 ; i >= 0 ; i--)
	{
	  const QgsMimeDataUtils::Uri &Ur = lst.at( i );

	  if(Ur.providerKey == "wms")
		  m_parent->addLayer(Ur.uri, eLayerType::XYZ, Ur.name);
	  else if(Ur.layerType == "raster")
		  m_parent->addLayer(Ur.uri, eLayerType::Raster, Ur.name);
	  else if(Ur.layerType == "vector")
		  m_parent->addLayer(Ur.uri, eLayerType::Vector, Ur.name);

	}
}

}
}
}
