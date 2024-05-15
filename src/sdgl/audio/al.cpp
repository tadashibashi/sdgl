#include "al.h"
#include <sdgl/logging.h>

bool sdgl::audio::detail::alCheckImpl(const char *filename, const uint_fast32_t line)
{
    const auto error = alGetError();
    if (error != AL_NO_ERROR)
    {
        const auto msgStart = format("*** OpenAL Error *** ({}:{})", filename, line);
        switch(error)
        {
            case AL_INVALID_NAME:
                SDGL_ERROR("{}\nAL_INVALID_NAME: bad name (id) was passed to an OpenAL function", msgStart);
            break;
            case AL_INVALID_ENUM:
                SDGL_ERROR("{}\nAL_INVALID_ENUM: invalid enum value was passed to an OpenAL function", msgStart);
            break;
            case AL_INVALID_VALUE:
                SDGL_ERROR("{}\nAL_INVALID_VALUE: invalid value was passed to an OpenAL function", msgStart);
            break;
            case AL_INVALID_OPERATION:
                SDGL_ERROR("{}\nAL_INVALID_OPERATION: operation is not valid", msgStart);
            break;
            case AL_OUT_OF_MEMORY:
                SDGL_ERROR("{}\nAL_OUT_OF_MEMORY: operation resulted in OpenAL running out of memory", msgStart);
            break;
            default:
                SDGL_ERROR("{}\nUnknown OpenAL error occured", msgStart);
            break;
        }

        return false;
    }

    return true;
}

bool sdgl::audio::detail::alcCheckImpl(const char *filename, const uint_fast32_t line, ALCdevice *device)
{
    ALCenum result = alcGetError(device);
    if(result != ALC_NO_ERROR)
    {
        const auto msgStart = format("***ERROR*** ({}:{})", filename, line);
        switch(result)
        {
        case ALC_INVALID_VALUE:
            SDGL_ERROR("{}\nALC_INVALID_VALUE: an invalid value was passed to an OpenAL function", msgStart);
            break;
        case ALC_INVALID_DEVICE:
            SDGL_ERROR("{}\nALC_INVALID_DEVICE: a bad device was passed to an OpenAL function", msgStart);
            break;
        case ALC_INVALID_CONTEXT:
            SDGL_ERROR("{}\nALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function", msgStart);
            break;
        case ALC_INVALID_ENUM:
            SDGL_ERROR("{}\nALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function", msgStart);
            break;
        case ALC_OUT_OF_MEMORY:
            SDGL_ERROR("{}\nALC_OUT_OF_MEMORY: an unknown enum value was passed to an OpenAL function", msgStart);
            break;
        default:
            SDGL_ERROR("{}\nUnknown ALC Error: {}", msgStart, result);
        }

        return false;
    }
    return true;
}
