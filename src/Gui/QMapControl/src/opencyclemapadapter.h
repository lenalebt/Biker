#ifndef OPENCYCLEMAPADAPTER_H
#define OPENCYCLEMAPADAPTER_H

#include "tilemapadapter.h"
namespace qmapcontrol
{
    //! MapAdapter for OpenCycleMap
    /*!
     * This is a conveniece class, which extends and configures a TileMapAdapter
     *	@author Lena Brüder <lena_lebt@gmx.de>
     */
    class OpenCycleMapAdapter : public TileMapAdapter
    {
        Q_OBJECT
                public:
        //! constructor
        /*!
         * This construct a OEPNVKarte Adapter
         */
        OpenCycleMapAdapter();
        virtual ~OpenCycleMapAdapter();
    };
}
#endif // OPENCYCLEMAPADAPTER_H
