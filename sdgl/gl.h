#pragma once
#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif

#include <GLES3/gl3.h>

#ifdef SDGL_DEBUG

#include <stdexcept>
#define GL_ERR_CHECK() do { if (glGetError() != GL_NO_ERROR) \
{ \
SDGL_ERROR("GL Error: {}", glGetError()); \
throw std::runtime_error("GL Error"); \
} } while(false)
#else
#define GL_ERR_CHECK()
#endif
