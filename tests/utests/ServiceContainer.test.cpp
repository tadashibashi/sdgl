#include "lib.h"
#include <sdgl/ServiceContainer.h>

TEST_CASE("ServiceContainer tests", "[sdgl::ServiceContainer]")
{
    ServiceContainer services{};

    SECTION("Can set and retrieve")
    {
        struct TestService
        {
            int x;
        };

        TestService s {
            .x = 10
        };

        REQUIRE(services.empty());
        services.provide(&s);
        REQUIRE(services.size() == 1);

        REQUIRE(services.getService<TestService>() == &s);
        REQUIRE(services.getService<TestService>()->x == 10);
    }

    SECTION("Can distinguish between inheriting types")
    {
        struct TestService
        {
            int x;
        };

        struct ChildService : public TestService
        {
            int y;
        };


        TestService s {
            .x = 15
        };

        ChildService c {
            .y = 20
        };
        c.x = 40;

        REQUIRE(services.empty());

        services.provide(&s);
        services.provide(&c);

        REQUIRE(services.size() == 2);

        REQUIRE(services.getService<TestService>() == &s);
        REQUIRE(services.getService<ChildService>() == &c);
        REQUIRE(services.getService<TestService>()->x == 15);
        REQUIRE(services.getService<ChildService>()->x == 40);
        REQUIRE(services.getService<ChildService>()->y == 20);
    }
}