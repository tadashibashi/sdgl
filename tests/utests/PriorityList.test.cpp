#include "lib.h"
#include "sdgl/ecs/PriorityList.h"

int idCounter = 0;
TEST_CASE("PriorityList tests", "[sdgl::PriorityList]")
{
    idCounter = 0;
    struct PriorityItem
    {
        int priority = 0;
        int id = idCounter++;

        [[nodiscard]]
        bool operator==(const PriorityItem &other) const { return id == other.id; }
        bool operator!=(const PriorityItem &other) const { return id != other.id; }
    };

    vector<PriorityItem> destroyedItems;
    PriorityList<PriorityItem> list([](const PriorityItem &item) { return item.priority; }, [&destroyedItems](PriorityItem &item) {
        destroyedItems.emplace_back(item);
    });

    SECTION("Indexer and size works as expected")
    {
        PriorityItem item0 = {.priority=10}, item1 {.priority=30}, item2 {.priority=20};
        REQUIRE(list.empty());
        REQUIRE(list.size() == 0);
        REQUIRE_THROWS(list.at(0));
        REQUIRE_THROWS(list.at(-1));
        REQUIRE_NOTHROW(list[0]);
        REQUIRE_NOTHROW(list[-1]);

        list.add(item0);

        REQUIRE_THROWS(list.at(1));
        REQUIRE_NOTHROW(list[1]);

        REQUIRE(list.at(0).priority == 10);
        REQUIRE(list[0].priority == 10);

        list.add(item1);
        REQUIRE_THROWS(list.at(2));
        REQUIRE_NOTHROW(list[2]);

        REQUIRE(list.at(0).priority == 30);
        REQUIRE(list[0].priority == 30);
        REQUIRE(list.at(1).priority == 10);
        REQUIRE(list[1].priority == 10);
    }

    SECTION("Can place items in order")
    {
        PriorityItem item0 = {.priority=10}, item1 {.priority=30}, item2 {.priority=20};

        list.add(item0);
        REQUIRE(!list.empty());
        REQUIRE(list.size() == 1);
        REQUIRE(list.at(0).priority == 10);

        list.add(item1);
        REQUIRE(list.size() == 2);
        REQUIRE(list.at(0).priority == 30);
        REQUIRE(list.at(1).priority == 10);

        list.add(item2);
        REQUIRE(list.size() == 3);
        REQUIRE(list.at(0).priority == 30);
        REQUIRE(list.at(1).priority == 20);
        REQUIRE(list.at(2).priority == 10);

        list.add(item2);
        REQUIRE(list.size() == 4);
        REQUIRE(list.at(0).priority == 30);
        REQUIRE(list.at(1).priority == 20);
        REQUIRE(list.at(2).priority == 20);
        REQUIRE(list.at(3).priority == 10);

        list.add(item1);
        list.add(item0);

        REQUIRE(list.size() == 6);
        REQUIRE(list.at(0).priority == 30);
        REQUIRE(list.at(1).priority == 30);
        REQUIRE(list.at(2).priority == 20);
        REQUIRE(list.at(3).priority == 20);
        REQUIRE(list.at(4).priority == 10);
        REQUIRE(list.at(5).priority == 10);
    }

    SECTION("Can remove items, and call destroyed callback")
    {
        PriorityItem item0 = {.priority=10}, item1 {.priority=30}, item2 {.priority=20};

        list.add(item0);
        REQUIRE(list.size() == 1);
        list.eraseItem(item0);
        REQUIRE(list.empty());

        REQUIRE(destroyedItems.size() == 1);
        REQUIRE(destroyedItems[0].id == item0.id);
    }
}
