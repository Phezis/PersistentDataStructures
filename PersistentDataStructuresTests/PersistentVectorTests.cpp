#include <gtest/gtest.h>
#include <PersistentVector.h>

namespace {
	using namespace pds;

	TEST(PVectorTest, Creation) {
		PersistentVector<int> pvector;
		EXPECT_EQ(pvector.size(), 0);
		EXPECT_TRUE(pvector.empty());
	}

	TEST(PVectorTest, EmptyCopyCreation) {
		PersistentVector<int> pvector_base;
		PersistentVector<int> pvector = pvector_base;
		EXPECT_EQ(pvector.size(), 0);
		EXPECT_TRUE(pvector.empty());
	}

	TEST(PVectorTest, EmptyMoveCreation) {
		PersistentVector<int> pvector_base;
		PersistentVector<int> pvector = std::move(pvector_base);
		EXPECT_EQ(pvector.size(), 0);
		EXPECT_TRUE(pvector.empty());
	}
}
