#include "lib.h"
#include <sdgl/graphics/atlas/CrunchAtlasData.h>

using sdgl::graphics::CrunchAtlasData;

TEST_CASE("CrunchAtlasData tests", "sdgl::graphics::CrunchAtlasData")
{
    SECTION("Open and verify binary test file")
    {
        CrunchAtlasData data;
        REQUIRE(CrunchAtlasData::loadBinaryFile("atlas.bin", true, true, &data));

        REQUIRE(data.textures.size() == 1);
        REQUIRE(data.textures[0].images.size() == 42);
        REQUIRE(data.textures[0].name == "atlas0");

        {
            auto it = std::ranges::find_if(data.textures[0].images, [](const CrunchAtlasData::Image &img) { return img.name == "compy/5"; });
            REQUIRE(it != data.textures[0].images.end());

            it = std::ranges::find_if(data.textures[0].images, [](const CrunchAtlasData::Image &img) { return img.name == "balloon/2"; });
            REQUIRE(it != data.textures[0].images.end());

            it = std::ranges::find_if(data.textures[0].images, [](const CrunchAtlasData::Image &img) { return img.name == "frog/4"; });
            REQUIRE(it != data.textures[0].images.end());
        }
    }
}