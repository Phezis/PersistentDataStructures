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
			pvector = pvector.push_back(i);
			for (size_t j = 0; j <= i; ++j) {
				EXPECT_EQ(pvector[j], j);
			}
		}
		for (size_t i = 0; i < size; ++i) {
			EXPECT_EQ(pvector[i], i);
		}
	}

	TEST(PVectorGetting, Huge) {
		PersistentVector<size_t> pvector;
		constexpr size_t size = (1 << 13) + 1;
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


	/*
	*	Extended creation
	*/

	TEST(PVectorIteratorCreation, NoOne) {
		std::vector<size_t> srcvector;
		PersistentVector<size_t> pvector(srcvector.begin(), srcvector.end());
		EXPECT_EQ(pvector.size(), 0);
		EXPECT_TRUE(pvector.empty());
	}

	TEST(PVectorIteratorCreation, OnlyOne) {
		std::vector<size_t> srcvector = { 12345 };
		PersistentVector<size_t> pvector(srcvector.begin(), srcvector.end());
		EXPECT_EQ(pvector.size(), 1);
		EXPECT_EQ(pvector[0], 12345);
	}

	TEST(PVectorIteratorCreation, Some) {
		std::vector<size_t> srcvector = { 1, 2, 3, 4, 5 };
		PersistentVector<size_t> pvector(srcvector.begin(), srcvector.end());
		EXPECT_EQ(pvector.size(), srcvector.size());
		for (size_t i = 0; i < srcvector.size(); ++i) {
			EXPECT_EQ(pvector[i], srcvector[i]);
		}
	}

	TEST(PVectorIteratorCreation, Huge) {
		constexpr size_t size = (1 << 15) + 1;
		std::vector<size_t> srcvector(size);

		for (size_t i = 0; i < srcvector.size(); ++i) {
			srcvector[i] = i + 1;
		}
		PersistentVector<size_t> pvector(srcvector.begin(), srcvector.end());
		EXPECT_EQ(pvector.size(), srcvector.size());
		for (size_t i = 0; i < srcvector.size(); ++i) {
			EXPECT_EQ(pvector[i], srcvector[i]);
		}
	}

	TEST(PVectorInitListCreation, NoOne) {
		PersistentVector<size_t> pvector = {};
		EXPECT_EQ(pvector.size(), 0);
		EXPECT_TRUE(pvector.empty());
	}

	TEST(PVectorInitListCreation, OnlyOne) {
		PersistentVector<size_t> pvector = { 12345 };
		EXPECT_EQ(pvector.size(), 1);
		EXPECT_EQ(pvector[0], 12345);
	}

	TEST(PVectorInitListCreation, Some) {
		PersistentVector<size_t> pvector = { 1, 2, 3, 4, 5 };
		EXPECT_EQ(pvector.size(), 5);
		for (size_t i = 0; i < 5; ++i) {
			EXPECT_EQ(pvector[i], i + 1);
		}
	}
}
