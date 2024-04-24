#pragma once
#if defined(SDGL_GRAPHICS_GLES3)
#   ifndef GL_SILENCE_DEPRECATION
#       define GL_SILENCE_DEPRECATION 1
#   endif

#   include <GLES3/gl3.h>

#   ifdef SDGL_DEBUG

#       include <stdexcept>

/// A call to this after invoking a gl function throws / logs if there were any errors
/// In debug mode it will throw an exception and log an error message, otherwise in release mode this is a noop
#       define GL_ERR_CHECK() do { if (glGetError() != GL_NO_ERROR) \
{ \
SDGL_ERROR("GL Error: {}: on line `{}`, in function `{}`, in file \"{}\"", glGetError(), __LINE__, __func__, __FILE__); \
throw std::runtime_error("GL Error"); \
} } while(false)
#   else
#       define GL_ERR_CHECK()
#   endif
#endif
