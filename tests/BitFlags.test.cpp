#include "lib.h"
#include <sdgl/BitFlags.h>

TEST_CASE("BitFlags test", "sdgl::BitFlags")
{
    SECTION("uin32_t block count aligns with number of flags")
    {
        REQUIRE(BitFlags(32).size() == 1);
        REQUIRE(BitFlags(8).size() == 1);
        REQUIRE(BitFlags(0).size() == 0);
        REQUIRE(BitFlags(64).size() == 2);
        REQUIRE(BitFlags(44).size() == 2);
        REQUIRE(BitFlags(64000).size() == 2000);
    }

    SECTION("flags are settable and retrievable, one block")
    {
        auto flags = BitFlags(32);

        flags.set(0, 1);
        flags.set(15, 1);

        auto data = *flags.data();

        REQUIRE(data & (1));
        REQUIRE(data & (1 << 15));
        REQUIRE((data & (1 << 31)) == 0);
        REQUIRE((data & (1 << 16)) == 0);
        REQUIRE((data & (1 << 14)) == 0);
        REQUIRE((data & (1 << 2)) == 0);

        // get equivalent
        REQUIRE(flags.get(0));
        REQUIRE(flags.get(15));
    }

    SECTION("flags are settable and retrievable, multiple blocks")
    {
        constexpr auto bitCount = 32 * 4;
        auto flags = BitFlags(bitCount);

        flags.set(32 * 3 + 4, 1);
        flags.set(32 * 2, 1);

        auto data = flags.data();

        REQUIRE((data[0] & 1) == 0);
        REQUIRE(data[3] & (1 << 4));
        REQUIRE(data[2] & (1));

        // get equivalent
        REQUIRE(flags.get(32 * 3 + 4));
        REQUIRE(flags.get(32 * 2));
    }

    SECTION("Test copy operator")
    {
        auto flagsA = BitFlags(32);
        flagsA.set(10, true);

        REQUIRE(flagsA.get(10));

        auto flagsB = BitFlags(10);
        flagsB = flagsA;

        REQUIRE(flagsB.get(10));
        REQUIRE(flagsA.get(10)); // check that no alteration of original data occurred
        REQUIRE(flagsB.data() != flagsA.data()); // data has been copied not the pointer
    }

    SECTION("Test copy constructor")
    {
        auto flagsA = BitFlags(32);
        flagsA.set(10, true);

        REQUIRE(flagsA.get(10));

        BitFlags flagsB(flagsA);

        REQUIRE(flagsB.get(10));
        REQUIRE(flagsA.get(10)); // check that no alteration of original data occurred
        REQUIRE(flagsB.data() != flagsA.data()); // data has been copied not the pointer
    }

    SECTION("Test move operator")
    {
        auto flagsA = BitFlags(32);
        flagsA.set(4, true);

        REQUIRE(flagsA.get(4));

        auto flagsB = BitFlags(0);
        flagsB = std::move(flagsA);

        REQUIRE(flagsB.get(4));
        REQUIRE(flagsA.size() == 0);
        REQUIRE(flagsA.data() == nullptr);
    }

    SECTION("Test move constructor")
    {
        auto flagsA = BitFlags(32);
        flagsA.set(4, true);

        REQUIRE(flagsA.get(4));

        BitFlags movedHere(std::move(flagsA));

        REQUIRE(movedHere.get(4));
        REQUIRE(flagsA.size() == 0);
        REQUIRE(flagsA.data() == nullptr);
    }
}
