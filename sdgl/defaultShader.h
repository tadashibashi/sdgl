#pragma once
#include "graphics/Shader.h"

namespace sdgl {
    /**
     *
     * @return pointer to the default shader, it's owned by the compiler, so no need to delete it
     */
    const Shader *getDefaultShader();
}
