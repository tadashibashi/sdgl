#include "content.h"

#include <stb_image.h>
#include <sdgl/io/io.h>
#include <sdgl/logging.h>

bool sdgl::io::convert::writeImageToSbc(const string &imageData, const string &filepath)
{
    int width, height, bytesPerPixel;
    const auto data = stbi_load_from_memory(
        reinterpret_cast<const stbi_uc *>(imageData.data()),
        static_cast<int>(imageData.size()),
        &width, &height, &bytesPerPixel, 0);

    if (!data)
    {
        SDGL_ERROR("stb_image failed to load image: {}", stbi_failure_reason());
        return false;
    }

    writeFile(filepath, data, width * height * bytesPerPixel);
    return true;
}
