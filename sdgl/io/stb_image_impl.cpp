#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#ifdef SDGL_DEBUG
#   define STBI_FAILURE_USERMSG
#else
#   define STBI_NO_FAILURE_STRINGS
#endif

#include <stb_image.h>
