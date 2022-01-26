#include <gtest/gtest.h>
#include <PersistentList.h>

namespace {
    using namespace pds;
    using namespace std;

    TEST(PListCreation, Creation) {

        pds::persistent_linked_list<int> list;
        //auto fat = std::make_shared<pds::list_fat_node<int>>();
        //auto n = std::make_shared<pds::list_node<int>>(1, 2, fat, fat);

        auto b = list.push_back(1);
        std::cout << list.size() << " " << b.size() << " " << b.push_front(12).size() << std::endl;
        std::cout << b.front() << " " << b.back() << std::endl;
        std::cout << b.pop_back().size() << " " << b.pop_front().size() << std::endl;

        auto c = b.set(0, 123);
        std::cout << c.size() << " " << c.front() << " " << c.back() << std::endl;

        auto d = c.push_back(555).push_back(666).insert(2, 444);
        std::cout << d.size() << " " << d.front() << " " << d.back() << std::endl;

        for (auto it = d.cbegin(); it != d.cend(); it++)
        {
            std::cout << *it << std::endl;
        }

        std::cout << d.undo().size() << " " << d.undo().redo().size() << std::endl;

        std::vector<int> v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        pds::persistent_linked_list<int> f_range(v.begin(), v.end());

        for (auto it = f_range.cbegin(); it != f_range.cend(); ++it)
        {
            std::cout << *it << std::endl;
        }

       // persistent_linked_list<int> list;
        EXPECT_EQ(list.size(), 0);
        EXPECT_TRUE(list.empty());
    }

    TEST(PListCreation, EmptyCopyCreation) {
        persistent_linked_list<int> pList_base;
        persistent_linked_list<int> pList = pList_base;
        EXPECT_EQ(pList.size(), 0);
        EXPECT_TRUE(pList.empty());
    }

    TEST(PListCreation, EmptyMoveCreation) {
        persistent_linked_list<int> pList_base;
        persistent_linked_list<int> pList = std::move(pList_base);
        EXPECT_EQ(pList.size(), 0);
        EXPECT_TRUE(pList.empty());
    }
}
