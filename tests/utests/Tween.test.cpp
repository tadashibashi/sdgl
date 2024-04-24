#include "lib.h"
#include <sdgl/Tween.h>

TEST_CASE("Tween tests", "[sdgl::Tween]")
{
    SECTION("Can update value")
    {
        Tween t(10, easings::linear);

        REQUIRE(t.getCurrentValue() == 0);

        t.update(5);

        REQUIRE(t.getCurrentValue() == .5);

        t.update(5);

        REQUIRE(t.getCurrentValue() == 1.0);
        REQUIRE(t.isPaused());
    }

    SECTION("Callback sets value as expected")
    {
        Tween t(100, easings::linear);

        float value = 0;
        t.onStep([&](const float current) {
            value = current;
        });

        t.update(50);

        REQUIRE(value == .5);

        t.update(50);

        REQUIRE(value == 1);
    }

    SECTION("Yoyo behavior works as expected")
    {
        Tween t(10, easings::linear);
        t.setYoyo(true);

        REQUIRE(t.isYoyo());

        t.update(5);

        REQUIRE(t.getCurrentValue() == .5);
        REQUIRE(t.isReversing() == false);

        t.update(5);

        REQUIRE(t.getCurrentValue() == 1);
        REQUIRE(t.isPaused() == false);

        t.update(5);

        REQUIRE(t.getCurrentValue() == .5);
        REQUIRE(t.isReversing() == true);

        t.update(5);

        REQUIRE(t.getCurrentValue() == 0);
        REQUIRE(t.isReversing() == false); // no longer reversing once it reaches home
        REQUIRE(t.isPaused() == true);     // done
    }

    SECTION("Scales values correctly with helper")
    {
        Tween t(10, easings::linear);
        t.update(5);

        REQUIRE(t.calculateValue(0, 10) == 5);
        REQUIRE(t.calculateValue(0, 100) == 50);
        REQUIRE(t.calculateValue(0, -100) == -50);
        REQUIRE(t.calculateValue(-4, 2) == -1);
        REQUIRE(t.calculateValue(10, 2) == 6); // higher value coming first still works
    }
}
