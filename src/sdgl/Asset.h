#pragma once
#include "sdglib.h"

namespace sdgl {

    class Asset {
    public:
        virtual ~Asset() = default;

        // Asset just needs to be unloadable, loading logic should be added to content management classes
        virtual void unload() = 0;
    };

} // sdgl
