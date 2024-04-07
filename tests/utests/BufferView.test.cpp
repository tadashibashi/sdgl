#include "lib.h"
#include <sdgl/io/BufferView.h>

TEST_CASE("BufferView tests", "sdgl::io::BufferView")
{
    SECTION("Can read strings")
    {
        auto buffer = "Hello world!";
        io::BufferView view(buffer);

        string value;
        REQUIRE(view.read(value));

        REQUIRE(buffer == value);
    }

    SECTION("Void * constructor")
    {
        auto buffer = "Hello world!";
        auto view = io::BufferView(buffer, std::strlen(buffer) + 1);
        string value;
        REQUIRE(view.read(value));

        REQUIRE(buffer == value);
    }

    SECTION("vector<char> constructor")
    {
        auto buffer = vector<char> ({'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' });
        auto view = io::BufferView(buffer);

        string value;
        REQUIRE(view.read(value));

        REQUIRE(value == "Hello world!");
    }

    SECTION("vector<ubyte> constructor")
    {
        auto buffer = vector<ubyte> ({'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' });
        auto view = io::BufferView(buffer);

        string value;
        REQUIRE(view.read(value));

        REQUIRE(value == "Hello world!");
    }

    SECTION("Can read into cstring")
    {
        auto buffer = "Hello world!";

        io::BufferView view(buffer);

        char value[64];
        REQUIRE(view.read(value, 64) == std::strlen(buffer) + 1);
        REQUIRE(std::strcmp(value, buffer) == 0);
    }

    SECTION("Can read two strings")
    {
        auto buffer = "Hello world!\0Another world!";
        io::BufferView view(buffer, 28);

        string value1, value2;

        REQUIRE(view.read(value1));
        REQUIRE(view.read(value2));

        REQUIRE(value1 == "Hello world!");
        REQUIRE(value2 == "Another world!");
    }

    SECTION("Can read a string + number + string + number")
    {
        auto buffer = "Apples\0\x15Oranges\0\x16";
        io::BufferView view(buffer, 17);

        string value1;
        ubyte value2;
        string value3;
        ubyte value4;

        REQUIRE(view.read(value1));
        REQUIRE(view.read(value2));
        REQUIRE(view.read(value3));
        REQUIRE(view.read(value4));

        REQUIRE(value1 == "Apples");
        REQUIRE((int)value2 == 0x15);
        REQUIRE(value3 == "Oranges");
        REQUIRE((int)value4 == 0x16);
    }

    SECTION("Can flip endianness")
    {
        // System architecture must be either big or little endian for this test to pass
        REQUIRE(io::SystemEndian != io::Endian::Unknown);
        constexpr auto endian = io::SystemEndian == io::Endian::Little ? io::Endian::Big : io::Endian::Little;

        auto buffer = "\x10\x00\x00\x00";
        auto view = io::BufferView(buffer, 4, endian);

        uint value;
        REQUIRE(view.read(value));

        // Flipped endianness
        REQUIRE(value == (io::SystemEndian == io::Endian::Big ? 0x00000010u : 0x10000000u));
    }

    SECTION("Protects against overreading: on end")
    {
        auto buffer = "Apples"; // only one string is available to read
        auto view = io::BufferView(buffer);

        string value1;
        int value2 = 123456;

        REQUIRE(view.read(value1));

        REQUIRE(value1 == "Apples");
        REQUIRE(!view.read(value2)); // can't read this int

        REQUIRE(value2 == 123456); // maintains its value
    }

    SECTION("Protects against overreading: type too big")
    {
        auto buffer = "Apples\0\x12\x34"; // string + 16-bit number
        auto view = io::BufferView(buffer);

        string value1;
        uint value2 = 123456; // 32-bit is too big

        REQUIRE(view.read(value1));

        REQUIRE(value1 == "Apples");
        REQUIRE(!view.read(value2)); // fail here

        REQUIRE(value2 == 123456); // maintains its value
    }

    SECTION("When reading string, size is guarded")
    {
        auto buffer = "Apples\0\x12\x34"; // string + 16-bit number
        auto view = io::BufferView(buffer, 9);

        string value;
        REQUIRE(view.read(value, 3));

        REQUIRE(value == "App");
    }

    SECTION("When reading into cstring, size is guarded")
    {
        auto buffer = "Apples\0\x12\x34"; // string + 16-bit number
        auto view = io::BufferView(buffer, 9);

        char value[4];
        REQUIRE(view.read(value, 4));

        REQUIRE(std::strcmp(value, "App") == 0);
    }
}
