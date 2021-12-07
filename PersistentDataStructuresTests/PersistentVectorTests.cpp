#include <gtest/gtest.h>
#include <PersistentVector.h>


namespace {
	using namespace pds;
	using namespace std;

	/*
	*	Creation
	*/

	TEST(PVectorCreation, Creation) {
		PersistentVector<int> pvector;
		EXPECT_EQ(pvector.size(), 0);
		EXPECT_TRUE(pvector.empty());
	}

	TEST(PVectorCreation, EmptyCopyCreation) {
		PersistentVector<int> pvector_base;
		PersistentVector<int> pvector = pvector_base;
		EXPECT_EQ(pvector.size(), 0);
		EXPECT_TRUE(pvector.empty());
	}

	TEST(PVectorCreation, EmptyMoveCreation) {
		PersistentVector<int> pvector_base;
		PersistentVector<int> pvector = std::move(pvector_base);
		EXPECT_EQ(pvector.size(), 0);
		EXPECT_TRUE(pvector.empty());
	}


	/*
	*	Inserting
	*/

	TEST(PVectorInserting, push_back_onlyOneLeaf) {
		PersistentVector<size_t> pvector;
		constexpr size_t size = (1 << 5);
		for (size_t i = 0; i < size; ++i) {
			pvector = pvector.push_back(i);
			EXPECT_EQ(pvector.size(), i + 1);
		}
	}

	TEST(PVectorInserting, push_back_twoLeafs) {
		PersistentVector<size_t> pvector;
		constexpr size_t size = 2*(1 << 5);
		for (size_t i = 0; i < size; ++i) {
			pvector = pvector.push_back(i);
			EXPECT_EQ(pvector.size(), i + 1);
		}
	}

	TEST(PVectorInserting, push_back_twoLevels) {
		PersistentVector<size_t> pvector;
		constexpr size_t size = ((1 << 5) << 5) + 1;
		for (size_t i = 0; i < size; ++i) {
			pvector = pvector.push_back(i);
			EXPECT_EQ(pvector.size(), i + 1);
		}
	}


	/*
	*	Getting
	*/

	TEST(PVectorGetting, onlyOneElement) {
		PersistentVector<size_t> pvector;
		pvector = pvector.push_back(0);
		EXPECT_EQ(pvector[0], 0);
	}

	TEST(PVectorGetting, onlyOneLeaf) {
		PersistentVector<size_t> pvector;
		constexpr size_t size = (1 << 5);
		for (size_t i = 0; i < size; ++i) {
			pvector = pvector.push_back(i);
			for (size_t j = 0; j <= i; ++j) {
				EXPECT_EQ(pvector[j], j);
			}
		}
		for (size_t i = 0; i < size; ++i) {
			EXPECT_EQ(pvector[i], i);
		}
	}

	TEST(PVectorGetting, TwoLeafs) {
		PersistentVector<size_t> pvector;
		constexpr size_t size = (1 << 5) + 1;
		for (size_t i = 0; i < size; ++i) {
			pvector = pvector.push_back(i);
			for (size_t j = 0; j <= i; ++j) {
				EXPECT_EQ(pvector[j], j);
			}
		}
		for (size_t i = 0; i < size; ++i) {
			EXPECT_EQ(pvector[i], i);
		}
	}

	TEST(PVectorGetting, TwoLelves) {
		PersistentVector<size_t> pvector;
		constexpr size_t size = (1 << 10) + 1;
		for (size_t i = 0; i < size; ++i) {
			if (i >= 1023) {
				//std::cout << "A" << std::endl;
			}
			pvector = pvector.push_back(i);
			if (i > 64) {
				for (size_t j = 0; j <= i; ++j) {
					//std::cout << pvector[j] << " = " << j << std::endl;
				}
			}
			for (size_t j = 0; j <= i; ++j) {
				if (i >= 1023 && j >= 32) {
					//std::cout << "A" << std::endl;
				}
				EXPECT_EQ(pvector[j], j);
			}
		}
		for (size_t i = 0; i < size; ++i) {
			EXPECT_EQ(pvector[i], i);
		}
	}

	TEST(PVectorGetting, Huge) {
		PersistentVector<size_t> pvector;
		constexpr size_t size = (1 << 11) + 1;
		for (size_t i = 0; i < size; ++i) {
			pvector = pvector.push_back(i);
			for (size_t j = 0; j <= i; ++j) {
				EXPECT_EQ(pvector[j], j);
			}
		}
		for (size_t i = 0; i < size; ++i) {
			EXPECT_EQ(pvector[i], i);
		}
	}
}
