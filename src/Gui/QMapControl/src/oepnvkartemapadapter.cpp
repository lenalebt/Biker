
#include "oepnvkartemapadapter.h"
namespace qmapcontrol
{
    OEPNVKarteMapAdapter::OEPNVKarteMapAdapter()
        : TileMapAdapter("tile.xn--pnvkarte-m4a.de", "/tilegen/%1/%2/%3.png", 256, 0, 17)
    {   //TODO: evtl. falsche Adresse? Aus dem Kopf geraten...
    }

    OEPNVKarteMapAdapter::~OEPNVKarteMapAdapter()
    {
    }
}
