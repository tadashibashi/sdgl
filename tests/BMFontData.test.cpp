#include "lib.h"
#include <sdgl/graphics/font/BMFontData.h>

using sdgl::BMFontData;

TEST_CASE("BMFontData tests", "sdgl::graphics::BMFontData")
{
    SECTION("Read binary file")
    {
        BMFontData data;
        REQUIRE(BMFontData::fromFile("assets/bmfont/arial.fnt", &data));

        REQUIRE(data.info.fontName == "Arial");


        REQUIRE(data.pages.size() == 1);
        REQUIRE(data.pages[0].file == "font_0.png");
    }
}