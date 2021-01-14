#include "components/geolayerscomponent.h"
#include "mapwidget.h"
#include "widgets/geomapcanvas.h"


#include "qgis/qgslayertreemapcanvasbridge.h"
#include "qgis/qgslayertreemodel.h"
#include <qgis/qgslayertreenode.h>
#include <qgis/qgslayertree.h>
#include <qgis/qgsproviderregistry.h>
#include <qgis/qgsproject.h>
#include <qgis/qgsxyzconnectiondialog.h>
#include <qgis/qgsxyzconnection.h>
#include <qgis/qgslayertreeviewdefaultactions.h>

#include <QAction>
#include <QDockWidget>
#include <QFileDialog>
#include <qgsapplication.h>

namespace Software
{
namespace Library
{
namespace GisEngine {
GeoLayersComponent::GeoLayersComponent(MapWidget *parent):
	GeoComponent(parent)
{
	m_LayerTreeView = new QgsLayerTreeView( parent );
	m_LayerTreeView->setObjectName( QStringLiteral( "theLayerTreeView" ) );
//	m_LayerTreeView->setMenuProvider( new QgsAppLayerTreeViewMenuProvider( m_LayerTreeView, canvas ) );



	m_dockWidget = new QDockWidget(parent);
	m_dockWidget->setWidget(m_LayerTreeView);

	m_layerTreeModel = new QgsLayerTreeModel( QgsProject::instance()->layerTreeRoot(), parent );
	m_layerTreeModel->setFlag( QgsLayerTreeModel::AllowNodeReorder );
	m_layerTreeModel->setFlag( QgsLayerTreeModel::AllowNodeRename );
	m_layerTreeModel->setFlag( QgsLayerTreeModel::AllowNodeChangeVisibility );
	m_layerTreeModel->setFlag( QgsLayerTreeModel::ShowLegendAsTree );
	m_layerTreeModel->setFlag( QgsLayerTreeModel::UseEmbeddedWidgets );
	m_layerTreeModel->setAutoCollapseLegendNodes( 10 );

	m_LayerTreeView->setModel( m_layerTreeModel );

	m_LayerTreeCanvasBridge = new QgsLayerTreeMapCanvasBridge( QgsProject::instance()->layerTreeRoot(), parent->mapCanvas(), this );

	// add group action
	QAction *actionAddGroup = new QAction( tr( "Add Group" ), this );
	actionAddGroup->setIcon( QgsApplication::getThemeIcon( QStringLiteral( "/mActionAddGroup.svg" ) ) );
	actionAddGroup->setToolTip( tr( "Add Group" ) );
	connect( actionAddGroup, &QAction::triggered, m_LayerTreeView->defaultActions(), &QgsLayerTreeViewDefaultActions::addGroup );

}

void GeoLayersComponent::addLayerByDialog(QString &Address, eLayerType type, QString &layerName)
{
	switch (type) {
	case eLayerType::Raster:
	case eLayerType::Vector:
	{
		Address = QFileDialog::getOpenFileName(m_parent,
													   "Add New Layer",
													   "",
													   (type == eLayerType::Raster) ? QgsProviderRegistry::instance()->fileRasterFilters() :
																					  QgsProviderRegistry::instance()->fileVectorFilters());
		QFileInfo fileInfo(Address);
		layerName = fileInfo.baseName();
		break;
	}
	case eLayerType::XYZ :
	{
		QgsXyzConnectionDialog dlg;
		if ( !dlg.exec() )
		  return;
		Address = dlg.connection().url;
		layerName = dlg.connection().name;
		break;
	}
	default:
		break;
	}
}
}
}
}
