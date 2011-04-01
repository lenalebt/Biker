#ifndef OEPNVKARTEMAPADAPTER_H
#define OEPNVKARTEMAPADAPTER_H

#include "tilemapadapter.h"
namespace qmapcontrol
{
    //! MapAdapter for OEPNVKarte
    /*!
     * This is a conveniece class, which extends and configures a TileMapAdapter
     *	@author Lena Brüder <lena_lebt@gmx.de>
     */
    class OEPNVKarteMapAdapter : public TileMapAdapter
    {
        Q_OBJECT
                public:
        //! constructor
        /*!
         * This construct a OEPNVKarte Adapter
         */
        OEPNVKarteMapAdapter();
        virtual ~OEPNVKarteMapAdapter();
    };
}
#endif // OEPNVKARTEMAPADAPTER_H
