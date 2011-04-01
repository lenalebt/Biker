
#include "opencyclemapadapter.h"
namespace qmapcontrol
{
    OpenCycleMapAdapter::OpenCycleMapAdapter()
        : TileMapAdapter("tile.opencyclemap.org", "/%1/%2/%3.png", 256, 0, 17)
    {   //TODO: evtl. falsche Adresse? Aus dem Kopf geraten...
    }

    OpenCycleMapAdapter::~OpenCycleMapAdapter()
    {
    }
}
