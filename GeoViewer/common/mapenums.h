#ifndef MAPENUMS_H
#define MAPENUMS_H
#include <QPointF>

namespace Software
{
namespace Library
{
namespace GisEngine {
enum class eCoordinateAppearanceType
{
	DD = 0,
	DDM ,
	DMS
};
enum class eDateUnit{
	Gregorian = 0,
	Jalali
};

enum class eSpeedUnit{
	MPS = 0,
	KMPH
};
enum class eHorizontalAlignment{
	Right,
	Left,
	Center
};
enum class eVerticalAlignment{
	Up,
	Down,
	Center
};

enum class eItemType
{
	marker = 0,
	Text ,
	Sector ,
	SimpleMarker,
	Target ,
	MultiPosition,
	ColorizePath,
	CV,
	Interrogator
};

enum class e2PositionItemType{
	Circle,
	Rectangle,
};

enum class eToolType{
	NoneTool = 0,
	PanTool ,
	ZoomTool,
	MultipointTool,
	PolylineTool,
	PolygonTool,
	CircleTool,
	RectangleTool,
	DistanceTool,
	AreaTool,
	BearingTool,
	pointedCirve,
	SectorEditor
};

enum class eMeasureType
{
	Angle,
	Distance,
//	Area
};

enum class eComponentType
{
	none = 0,
	ToolBar,
	Layers,
	Browser,
	DrawingToolBar,
	AppearanceList,
	Compass,
	Zoom,
	Navigators,
	ScaleBar,
	Overview,
	MessageBar,
	MouseCoordinate,
	Information,
	Drawing,
	Table,
	Chart

};

enum class eToolBarItemType{
	LayerManager = 0,
	PanTool ,
	ZoomTool,
	DrawingToolBar,
	DistanceTool,
	BearingTool,
	AppearanceCoordinate,
	table,
	drawing
};

enum class eSimpleMarkerSymbolType
{
	None,
	Cross,
	X,
	Box,
	Circle,
	DoubleTrianle
};

enum class eGeometryType
{
	Point,
	Line,
	Polygon
};

enum class eCvSectorType{
	Near = 0,
	Far,
	Tracking,
	Firing,
	Killing
};

enum class eLayerType
{
    None,
    Raster,
    Vector,
    XYZ
};

}
}
}
#endif // MAPENUMS_H
