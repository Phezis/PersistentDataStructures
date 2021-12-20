#include <gtest/gtest.h>
#include <PersistentMap.h>
#include <thread>
#include <chrono>


namespace {
	using namespace pds;
	using namespace std;

	/*
	*	Creation
	*/

	TEST(PMapCreation, Creation) {
		PersistentMap<size_t, size_t> pmap;
		EXPECT_EQ(pmap.size(), 0);
		EXPECT_TRUE(pmap.empty());
	}

	TEST(PMapCreation, EmptyCopyCreation) {
		PersistentMap<size_t, size_t> pmap_base;
		PersistentMap<size_t, size_t> pmap = pmap_base;
		EXPECT_EQ(pmap.size(), 0);
		EXPECT_TRUE(pmap.empty());
	}

	TEST(PMapCreation, EmptyMoveCreation) {
		PersistentMap<size_t, size_t> pmap_base;
		PersistentMap<size_t, size_t> pmap = std::move(pmap_base);
		EXPECT_EQ(pmap.size(), 0);
		EXPECT_TRUE(pmap.empty());
	}


	/*
	*	Setting
	*/

	TEST(PMapSetting, OnlyOne) {
		PersistentMap<size_t, size_t> pmap;
		EXPECT_TRUE(pmap.empty());
		EXPECT_EQ(pmap.size(), 0);
		pmap = pmap.set(0, 0);
		EXPECT_FALSE(pmap.empty());
		EXPECT_EQ(pmap.size(), 1);
	}

	TEST(PMapSetting, Some) {
		PersistentMap<size_t, size_t> pmap;
		constexpr size_t size = 1 << 5;
		for (size_t i = 0; i < size; ++i) {
			pmap = pmap.set(i, i);
			EXPECT_EQ(pmap.size(), i + 1);
		}
	}

	TEST(PMapSetting, OneKey) {
		PersistentMap<size_t, size_t> pmap;
		constexpr size_t size = 1 << 5;
		for (size_t i = 0; i < size; ++i) {
			pmap = pmap.set(0, i);
			EXPECT_EQ(pmap.size(), 1);
		}
	}

	TEST(PMapSetting, SomeWithCollision) {
		constexpr size_t size = 1 << 5;
		PersistentMap<size_t, size_t> pmap(8*size);
		for (size_t i = 0; i < size; ++i) {
			pmap = pmap.set(i + 8*size, i);
			EXPECT_EQ(pmap.size(), i + 1);
		}
	}

	TEST(PMapSetting, KeyIsUnique) {
		constexpr size_t size = 1 << 5;
		PersistentMap<size_t, size_t> pmap;
		for (size_t i = 0; i < size; ++i) {
			pmap = pmap.set(0, i);
			EXPECT_EQ(pmap.size(), 1);
		}
	}

	TEST(PMapSetting, Huge) {
		constexpr size_t size = 1 << 13;
		PersistentMap<size_t, size_t> pmap;
		for (size_t i = 0; i < size; ++i) {
			pmap = pmap.set(i, i);
			EXPECT_EQ(pmap.size(), i + 1);
		}
		for (size_t i = 0; i < size >> 3; ++i) {
			pmap = pmap.set(i, 0);
			EXPECT_EQ(pmap.size(), size);
		}
	}


	/*
	*	Getting
	*/

	TEST(PMapGetting, OnlyOneElement) {
		PersistentMap<size_t, size_t> pmap;
		pmap = pmap.set(5, 13);
		EXPECT_EQ(pmap[5], 13);
	}

	TEST(PMapGetting, Some) {
		PersistentMap<size_t, size_t> pmap;
		constexpr size_t size = (1 << 5);
		for (size_t i = 0; i < size; ++i) {
			pmap = pmap.set(i, i);
			for (size_t j = 0; j <= i; ++j) {
				EXPECT_EQ(pmap[j], j);
			}
		}
		for (size_t i = 0; i < size; ++i) {
			EXPECT_EQ(pmap[i], i);
		}
	}

	TEST(PMapGetting, SomeWithCollision) {
		PersistentMap<size_t, size_t> pmap(512);
		constexpr size_t size = (1 << 5);
		for (size_t i = 0; i < size; ++i) {
			pmap = pmap.set(i, i);
			pmap = pmap.set(i + 512, i);
			for (size_t j = 0; j <= i; ++j) {
				EXPECT_EQ(pmap[j], j);
				EXPECT_EQ(pmap[j + 512], j);
			}
		}
		for (size_t i = 0; i < size; ++i) {
			EXPECT_EQ(pmap[i], i);
			EXPECT_EQ(pmap[i + 512], i);
		}
	}

	TEST(PMapGetting, SomeWithResetting) {
		PersistentMap<size_t, size_t> pmap(512);
		constexpr size_t size = (1 << 5);
		for (size_t i = 0; i < size; ++i) {
			pmap = pmap.set(i, 0);
			for (size_t j = 0; j <= i; ++j) {
				EXPECT_EQ(pmap[j], 0);
			}
		}
		for (size_t i = 0; i < size; ++i) {
			pmap = pmap.set(i, i);
			for (size_t j = 0; j <= i; ++j) {
				EXPECT_EQ(pmap[j], j);
			}
		}
		for (size_t i = 0; i < size; ++i) {
			EXPECT_EQ(pmap[i], i);
		}
	}

	TEST(PMapGetting, Huge) {
		PersistentMap<size_t, size_t> pmap;
		constexpr size_t size = (1 << 13);
		for (size_t i = 0; i < size; ++i) {
			pmap = pmap.set(i, 0);
			pmap = pmap.set(i, i);
		}
		for (size_t i = 0; i < size; ++i) {
			EXPECT_EQ(pmap[i], i);
		}
	}


	/*
	*	Extended creation
	*/

	TEST(PMamIteratorCreation, NoOne) {
		vector<pair<size_t, size_t>> v;
		PersistentMap<size_t, size_t> pmap(v.cbegin(), v.cend(), 16);
		EXPECT_EQ(pmap.size(), 0);
		EXPECT_TRUE(pmap.empty());
	}

	TEST(PMamIteratorCreation, OnlyOne) {
		vector<pair<size_t, size_t>> v = { { 3, 12 } };
		PersistentMap<size_t, size_t> pmap(v.cbegin(), v.cend(), 16);
		EXPECT_EQ(pmap.size(), 1);
		EXPECT_EQ(pmap[3], 12);
	}

	TEST(PMamIteratorCreation, Some) {
		vector<pair<size_t, size_t>> v = { { 3, 12 }, { 8, 22 }, { 400, 65 }, { 35453, 0 }, { 555, 555 } };
		PersistentMap<size_t, size_t> pmap(v.cbegin(), v.cend(), 16);
		EXPECT_EQ(pmap.size(), 5);
		EXPECT_EQ(pmap[3], 12);
		EXPECT_EQ(pmap[8], 22);
		EXPECT_EQ(pmap[400], 65);
		EXPECT_EQ(pmap[35453], 0);
		EXPECT_EQ(pmap[555], 555);
	}

	TEST(PMamIteratorCreation, Huge) {
		constexpr size_t size = (1 << 15);
		vector<pair<size_t, size_t>> v;
		for (size_t i = 0; i < size; ++i) {
			v.emplace_back(pair<size_t, size_t>(i, i + 1));
		}
		PersistentMap<size_t, size_t> pmap(v.cbegin(), v.cend(), 2*size);
		EXPECT_EQ(pmap.size(), size);
		for (size_t i = 0; i < size; ++i) {
			EXPECT_EQ(pmap[i], i + 1);
		}
	}

	TEST(PMamInitListCreation, NoOne) {
		PersistentMap<size_t, size_t> pmap({  }, 16);
		EXPECT_EQ(pmap.size(), 0);
		EXPECT_TRUE(pmap.empty());
	}

	TEST(PVectorIteratorCreation, OnlyOne) {
		PersistentMap<size_t, size_t> pmap({ { 3, 12 }, { 8, 22 }, { 400, 65 }, { 35453, 0 }, { 555, 555 } }, 16);
		EXPECT_EQ(pmap.size(), 5);
		EXPECT_EQ(pmap[3], 12);
		EXPECT_EQ(pmap[8], 22);
		EXPECT_EQ(pmap[400], 65);
		EXPECT_EQ(pmap[35453], 0);
		EXPECT_EQ(pmap[555], 555);
	}
	namespace {
		struct MyHash {
			std::size_t operator()(size_t key) const {
				return key;
			}
		};

		TEST(PVectorOwnHash, IteratorCreation) {
			vector<pair<size_t, size_t>> v = { { 3, 12 }, { 8, 22 }, { 400, 65 }, { 35453, 0 }, { 555, 555 } };
			PersistentMap<size_t, size_t, MyHash> pmap(v.cbegin(), v.cend(), 16, MyHash());
			EXPECT_EQ(pmap.size(), 5);
			EXPECT_EQ(pmap[3], 12);
			EXPECT_EQ(pmap[8], 22);
			EXPECT_EQ(pmap[400], 65);
			EXPECT_EQ(pmap[35453], 0);
			EXPECT_EQ(pmap[555], 555);
		}

		TEST(PVectorOwnHash, InitListCreation) {
			vector<pair<size_t, size_t>> v = { { 3, 12 }, { 8, 22 }, { 400, 65 }, { 35453, 0 }, { 555, 555 } };
			PersistentMap<size_t, size_t, MyHash> pmap(v.cbegin(), v.cend(), 16, MyHash());
			EXPECT_EQ(pmap.size(), 5);
			EXPECT_EQ(pmap[3], 12);
			EXPECT_EQ(pmap[8], 22);
			EXPECT_EQ(pmap[400], 65);
			EXPECT_EQ(pmap[35453], 0);
			EXPECT_EQ(pmap[555], 555);
		}
	}


	/*
	*	Erasing
	*/

	TEST(PVectorErasing, Existed) {
		vector<pair<size_t, size_t>> v = { { 0, 0 }, { 1, 1 }, { 2, 2 }, { 3, 3 }, { 4, 4 } };
		PersistentMap<size_t, size_t, MyHash> pmap(v.cbegin(), v.cend(), 16, MyHash());
		size_t size = 5;
		for (size_t j = 0; j < 5; ++j, --size) {
			EXPECT_EQ(pmap.size(), size);
			for (size_t i = 0; i < size; ++i) {
				EXPECT_EQ(pmap[i], i);
			}
			pmap = pmap.erase(4 - j);
		}
		EXPECT_TRUE(pmap.empty());
	}

	TEST(PVectorErasing, NotExisted) {
		vector<pair<size_t, size_t>> v = { { 0, 0 }, { 1, 1 }, { 2, 2 }, { 3, 3 }, { 4, 4 } };
		PersistentMap<size_t, size_t, MyHash> pmap(v.cbegin(), v.cend(), 16, MyHash());
		for (size_t j = 5; j < 10; ++j) {
			EXPECT_THROW(pmap.erase(j), out_of_range);
			EXPECT_EQ(pmap.size(), 5);
		}
	}


	/*
	*	Iterators
	*/


	TEST(PMapIterator, Empty) {
		PersistentMap<size_t, size_t, MyHash> pmap;
		EXPECT_TRUE(pmap.cbegin() == pmap.cend());
	}

	TEST(PMapIterator, OnlyOne) {
		PersistentMap<size_t, size_t, MyHash> pmap = { { 12345, 12345 } };

		EXPECT_TRUE(pmap.cbegin() != pmap.cend());
		auto it = pmap.cbegin();
		EXPECT_EQ(it->second, 12345);
		EXPECT_TRUE(++it == pmap.cend());
	}

	TEST(PMapIterator, Some) {
		constexpr size_t size = 10;
		PersistentMap<size_t, size_t, MyHash> pmap;
		for (size_t i = 0; i < size; ++i) {
			pmap = pmap.set(12345 + i, 54321);
		}
		EXPECT_TRUE(pmap.cbegin() != pmap.cend());
		for (auto it = pmap.cbegin(); it != pmap.cend(); ++it) {
			EXPECT_EQ(it->second, 54321);
		}
	}

	TEST(PMapIterator, Huge) {
		constexpr size_t size = (1 << 13) + 1;
		PersistentMap<size_t, size_t, MyHash> pmap;
		for (size_t i = 0; i < size; ++i) {
			pmap = pmap.set(12345 + i, 54321);
		}
		EXPECT_TRUE(pmap.cbegin() != pmap.cend());
		for (auto it = pmap.cbegin(); it != pmap.cend(); ++it) {
			EXPECT_EQ(it->second, 54321);
		}
	}

	TEST(PVectorIterator, Comparation) {
		constexpr size_t size = 10;
		PersistentMap<size_t, size_t, MyHash> pmap;
		for (size_t i = 0; i < size; ++i) {
			pmap = pmap.set(12345 + i, 54321 * i);
		}
		EXPECT_TRUE(pmap.cbegin() != pmap.cend());
		auto it1 = pmap.cbegin();
		auto it2 = pmap.cbegin();
		EXPECT_TRUE(pmap.cbegin() != pmap.cend());
		EXPECT_FALSE(pmap.cbegin() == pmap.cend());
		EXPECT_TRUE(it1 == it2);
		EXPECT_FALSE(it1 != it2);
		++it1;
		EXPECT_FALSE(it1 == it2);
		EXPECT_TRUE(it1 != it2);
		++it2;
		EXPECT_TRUE(it1 == it2);
		EXPECT_FALSE(it1 != it2);
		++it2;
		EXPECT_FALSE(it1 == it2);
		EXPECT_TRUE(it1 != it2);


		EXPECT_TRUE(it1 != pmap.cbegin());
		EXPECT_TRUE(it1 != pmap.cend());
		EXPECT_TRUE(it2 != pmap.cbegin());
		EXPECT_TRUE(it2 != pmap.cend());
	}


	/*
	*
	*	Comparation
	*
	*/

	TEST(PMapComparation, Empty) {
		PersistentMap<size_t, size_t, MyHash> pmap;
		PersistentMap<size_t, size_t, MyHash> pmap1;
		PersistentMap<size_t, size_t, MyHash> pmap2 = pmap;
		EXPECT_TRUE(pmap == pmap);
		EXPECT_FALSE(pmap != pmap);
		EXPECT_TRUE(pmap == pmap1);
		EXPECT_FALSE(pmap != pmap1);
		EXPECT_TRUE(pmap == pmap2);
		EXPECT_FALSE(pmap != pmap2);
	}

	TEST(PMapComparation, NonEmptyConstructed) {
		PersistentMap<size_t, size_t, MyHash> pmap = { { 0, 0 }, { 1, 2 }, { 2, 4 } };
		PersistentMap<size_t, size_t, MyHash> pmap1 = { { 0, 0 }, { 2, 4 }, { 1, 2 } };
		PersistentMap<size_t, size_t, MyHash> pmap2 = pmap;
		EXPECT_TRUE(pmap == pmap);
		EXPECT_FALSE(pmap != pmap);
		EXPECT_TRUE(pmap == pmap1);
		EXPECT_FALSE(pmap != pmap1);
		EXPECT_TRUE(pmap == pmap2);
		EXPECT_FALSE(pmap != pmap2);
	}

	TEST(PMapComparation, SameLength) {
		constexpr size_t size = (1 << 13) + 1;
		PersistentMap<size_t, size_t, MyHash> pmap;
		PersistentMap<size_t, size_t, MyHash> pmap1;
		for (size_t i = 0; i < size; ++i) {
			pmap = pmap.set(i, i + 1);
			pmap1 = pmap1.set(i, i + 1);
		}
		EXPECT_TRUE(pmap == pmap1);
		EXPECT_FALSE(pmap != pmap1);
		auto pmap2 = pmap1.set(size - 1, 54321);
		EXPECT_TRUE(pmap == pmap1);
		EXPECT_FALSE(pmap != pmap1);
		EXPECT_FALSE(pmap == pmap2);
		EXPECT_TRUE(pmap != pmap2);
		pmap2 = pmap2.set(size - 1, size);
		EXPECT_TRUE(pmap2 == pmap1);
		EXPECT_FALSE(pmap2 != pmap1);
		EXPECT_TRUE(pmap2 == pmap);
		EXPECT_FALSE(pmap2 != pmap);
		pmap2 = pmap2.set(size - 5, 54321);
		EXPECT_TRUE(pmap == pmap1);
		EXPECT_FALSE(pmap != pmap1);
		EXPECT_FALSE(pmap == pmap2);
		EXPECT_TRUE(pmap != pmap2);
		pmap2 = pmap2.set(size - 5, size - 4);
		EXPECT_TRUE(pmap2 == pmap1);
		EXPECT_FALSE(pmap2 != pmap1);
		EXPECT_TRUE(pmap2 == pmap);
		EXPECT_FALSE(pmap2 != pmap);
	}

	TEST(PMapComparation, DifferentLength) {
		constexpr size_t size = (1 << 13) + 1;
		PersistentMap<size_t, size_t, MyHash> pmap;
		PersistentMap<size_t, size_t, MyHash> pmap1;
		for (size_t i = 0; i < size - 1; ++i) {
			pmap = pmap.set(i, i + 1);
			pmap1 = pmap1.set(i, i + 1);
		}
		pmap = pmap.set(size - 1, size);
		EXPECT_FALSE(pmap == pmap1);
		EXPECT_TRUE(pmap != pmap1);
		auto pmap2 = pmap1.set(size - 2, 54321);
		EXPECT_FALSE(pmap == pmap1);
		EXPECT_TRUE(pmap != pmap1);
		EXPECT_FALSE(pmap1 == pmap2);
		EXPECT_TRUE(pmap1 != pmap2);
		pmap2 = pmap2.set(size - 2, size - 1);
		EXPECT_FALSE(pmap == pmap1);
		EXPECT_TRUE(pmap != pmap1);
		EXPECT_TRUE(pmap1 == pmap2);
		EXPECT_FALSE(pmap1 != pmap2);
	}


	/*
	*
	*	Swap
	*
	*/

	TEST(PMapSwap, SwapItself) {
		PersistentMap<size_t, size_t, MyHash> pmap = { { 0, 0 }, { 1, 2 }, { 2, 4 } };
		pmap.swap(pmap);
		EXPECT_TRUE(pmap == pmap);
		std::swap(pmap, pmap);
		EXPECT_TRUE(pmap == pmap);
	}

	TEST(PMapSwap, SwapDifferent) {
		PersistentMap<size_t, size_t, MyHash> pmap1 = { { 0, 0 }, { 1, 2 }, { 2, 4 } };
		PersistentMap<size_t, size_t, MyHash> pmap2 = { { 0, 4 }, { 1, 2 }, { 2, 0 } };
		PersistentMap<size_t, size_t, MyHash> pmap1_copy = pmap1;
		PersistentMap<size_t, size_t, MyHash> pmap2_copy = pmap2;
		EXPECT_TRUE(pmap1 != pmap2);
		EXPECT_TRUE(pmap1 == pmap1_copy);
		EXPECT_TRUE(pmap2 == pmap2_copy);
		pmap1.swap(pmap2);
		EXPECT_TRUE(pmap1 != pmap2);
		EXPECT_TRUE(pmap2 == pmap1_copy);
		EXPECT_TRUE(pmap1 == pmap2_copy);
		std::swap(pmap1, pmap2);
		EXPECT_TRUE(pmap1 != pmap2);
		EXPECT_TRUE(pmap1 == pmap1_copy);
		EXPECT_TRUE(pmap2 == pmap2_copy);
	}


	/*
	*	Clear
	*/

	TEST(PMapClear, Empty) {
		PersistentMap<size_t, size_t, MyHash> pmap;
		EXPECT_TRUE(pmap.empty());
		auto pmap1 = pmap.clear();
		EXPECT_TRUE(pmap.empty());
		EXPECT_TRUE(pmap1.empty());
	}

	TEST(PMapClear, One) {
		PersistentMap<size_t, size_t, MyHash> pmap;
		pmap = pmap.set(123, 123);
		auto pmap1 = pmap.clear();
		EXPECT_EQ(pmap.size(), 1);
		EXPECT_TRUE(pmap1.empty());
	}

	TEST(PMapClear, Some) {
		constexpr size_t size = 6;
		PersistentMap<size_t, size_t, MyHash> pmap;
		for (size_t i = 0; i < size; ++i) {
			pmap = pmap.set(i, i);
		}
		auto pmap1 = pmap.clear();
		EXPECT_EQ(pmap.size(), size);
		EXPECT_TRUE(pmap1.empty());
	}

	TEST(PMapClear, Huge) {
		constexpr size_t size = 1 << 13;
		PersistentMap<size_t, size_t, MyHash> pmap;
		for (size_t i = 0; i < size; ++i) {
			pmap = pmap.set(i, i);
		}
		auto pmap1 = pmap.clear();
		EXPECT_EQ(pmap.size(), size);
		EXPECT_TRUE(pmap1.empty());
	}


	/*
	*	Searching
	*/

	TEST(PMapSearching, Empty) {
		PersistentMap<size_t, size_t, MyHash> pmap;
		EXPECT_EQ(pmap.find(0), pmap.cend());
	}

	TEST(PMapSearching, Exists) {
		PersistentMap<size_t, size_t, MyHash> pmap;
		pmap = pmap.set(123, 123);
		EXPECT_EQ(pmap.find(123), pmap.cbegin());
	}

	TEST(PMapSearching, DoesntExist) {
		PersistentMap<size_t, size_t, MyHash> pmap;
		pmap = pmap.set(123, 123);
		EXPECT_EQ(pmap.find(0), pmap.cend());
	}

	TEST(PMapSearching, ExistsHuge) {
		constexpr size_t size = 1 << 13;
		PersistentMap<size_t, size_t, MyHash> pmap;
		for (size_t i = 0; i < size; ++i) {
			pmap = pmap.set(i, i);
		}
		auto it = pmap.cbegin();
		++(++(++it));
		EXPECT_EQ(pmap.find(it->first), it);
	}

	TEST(PMapSearching, DoesntExistHuge) {
		constexpr size_t size = 1 << 13;
		PersistentMap<size_t, size_t, MyHash> pmap;
		for (size_t i = 0; i < size; ++i) {
			pmap = pmap.set(i, i);
		}
		EXPECT_EQ(pmap.find(size + 1), pmap.cend());
	}


	/*
	*	UNDO & REDO
	*/

//	TEST(PVectorUndoRedo, linear) {
//		PersistentVector<size_t> pvector;
//		constexpr size_t size = (1 << 5);
//		for (size_t i = 0; i < size; ++i) {
//			pvector = pvector.push_back(i);
//		}
//		auto pvector1 = pvector;
//		for (size_t i = 0; i < size; ++i) {
//			EXPECT_EQ(pvector1.size(), size - i);
//			EXPECT_EQ(pvector1.back(), size - i - 1);
//			pvector1 = pvector1.undo();
//			EXPECT_EQ(pvector1.size(), size - i - 1);
//		}
//		EXPECT_TRUE(pvector1.empty());
//		for (size_t i = 0; i < size; ++i) {
//			EXPECT_EQ(pvector1.size(), i);
//			pvector1 = pvector1.redo();
//			EXPECT_EQ(pvector1.back(), i);
//			EXPECT_EQ(pvector1.size(), i + 1);
//		}
//	}
//
//	TEST(PVectorUndoRedo, linearHuge) {
//		PersistentVector<size_t> pvector;
//		constexpr size_t size = (1 << 13);
//		for (size_t i = 0; i < size; ++i) {
//			pvector = pvector.push_back(i);
//		}
//		auto pvector1 = pvector;
//		for (size_t i = 0; i < size; ++i) {
//			EXPECT_EQ(pvector1.size(), size - i);
//			EXPECT_EQ(pvector1.back(), size - i - 1);
//			pvector1 = pvector1.undo();
//			EXPECT_EQ(pvector1.size(), size - i - 1);
//		}
//		EXPECT_TRUE(pvector1.empty());
//		for (size_t i = 0; i < size; ++i) {
//			EXPECT_EQ(pvector1.size(), i);
//			pvector1 = pvector1.redo();
//			EXPECT_EQ(pvector1.back(), i);
//			EXPECT_EQ(pvector1.size(), i + 1);
//		}
//		EXPECT_EQ(pvector, pvector1);
//	}
//
//	TEST(PVectorUndoRedo, linearHugeDestruction) {
//		PersistentVector<size_t> pvector;
//		constexpr size_t size = (1 << 13);
//		for (size_t i = 0; i < size; ++i) {
//			pvector = pvector.push_back(i);
//		}
//		for (size_t i = 0; i < size; ++i) {
//			EXPECT_EQ(pvector.size(), size - i);
//			EXPECT_EQ(pvector.back(), size - i - 1);
//			pvector = pvector.undo();
//			EXPECT_EQ(pvector.size(), size - i - 1);
//		}
//		pvector = pvector.push_back(0);
//	}
//
//	TEST(PVectorUndoRedo, LinearWithUndoRedo) {
//		PersistentVector<size_t> pvector;
//		constexpr size_t size = (1 << 5);
//		for (size_t i = 0; i < size; ++i) {
//			pvector = pvector.push_back(i);
//			pvector = pvector.undo();
//			EXPECT_EQ(pvector.size(), i);
//			EXPECT_TRUE(pvector.empty() || pvector.back() == i - 1);
//			pvector = pvector.redo();
//			EXPECT_EQ(pvector.size(), i + 1);
//			EXPECT_EQ(pvector.back(), i);
//		}
//	}
//
//	TEST(PVectorUndoRedo, NotLinearWithUndoRedo) {
//		PersistentVector<size_t> pvector;
//		constexpr size_t size = (1 << 13);
//		for (size_t i = 0; i < size; i += 2) {
//			pvector = pvector.push_back(i);
//			pvector = pvector.push_back(i * 2);
//			pvector = pvector.undo();
//			pvector = pvector.push_back(i + 1);
//			pvector = pvector.undo();
//			pvector = pvector.redo();
//			EXPECT_EQ(pvector.back(), i + 1);
//		}
//		EXPECT_EQ(pvector.size(), size);
//		for (size_t i = 0; i < size; ++i) {
//			EXPECT_EQ(pvector[i], i);
//		}
//	}
//
//	TEST(PVectorUndoRedo, NotLinearWithUndoRedoTwo) {
//		PersistentVector<size_t> pvector;
//		constexpr size_t size = (1 << 13);
//		for (size_t i = 0; i < size; i += 2) {
//			pvector = pvector.push_back(i);
//			pvector = pvector.push_back(i * 2);
//			pvector = pvector.push_back(i * 3);
//			pvector = pvector.undo();
//			pvector = pvector.undo();
//			pvector = pvector.push_back(i + 1);
//			pvector = pvector.undo();
//			pvector = pvector.redo();
//			EXPECT_EQ(pvector.back(), i + 1);
//		}
//		EXPECT_EQ(pvector.size(), size);
//		for (size_t i = 0; i < size; ++i) {
//			EXPECT_EQ(pvector[i], i);
//		}
//	}
//
//	/*
//	*	canUndo & canRedo
//	*/
//
//	TEST(PVectorCanUndoRedo, NewCreated) {
//		PersistentVector<size_t> pvector;
//		EXPECT_FALSE(pvector.canUndo());
//		EXPECT_FALSE(pvector.canRedo());
//	}
//
//	TEST(PVectorCanUndoRedo, CanUndoCantRedo) {
//		PersistentVector<size_t> pvector;
//		pvector = pvector.push_back(0);
//		EXPECT_TRUE(pvector.canUndo());
//		EXPECT_FALSE(pvector.canRedo());
//	}
//
//	TEST(PVectorCanUndoRedo, CantUndoCanRedo) {
//		PersistentVector<size_t> pvector;
//		pvector = pvector.push_back(0);
//		pvector = pvector.undo();
//		EXPECT_FALSE(pvector.canUndo());
//		EXPECT_TRUE(pvector.canRedo());
//	}
//
//	TEST(PVectorCanUndoRedo, CanUndoCanRedo) {
//		PersistentVector<size_t> pvector;
//		pvector = pvector.push_back(0).push_back(1);
//		pvector = pvector.undo();
//		EXPECT_TRUE(pvector.canUndo());
//		EXPECT_TRUE(pvector.canRedo());
//	}
//
//
//
//	/*
//	*	Concurrency
//	*/
//
//	namespace {
//		void PushBackFunc(PersistentVector<size_t>& pvector) {
//			auto id = std::hash<std::thread::id>{}(std::this_thread::get_id());
//			auto myPvector = pvector.push_back(id);
//			std::this_thread::sleep_for(std::chrono::milliseconds(10));
//			myPvector = myPvector.push_back(id + 1);
//			for (size_t i = 0; i < 5; ++i) {
//				EXPECT_EQ(myPvector[i], i + 1);
//			}
//			for (size_t i = 0; i < 2; ++i) {
//				EXPECT_EQ(myPvector[i + 5], id + i);
//			}
//		}
//
//		TEST(PVectorConcurrency, PushBack) {
//			constexpr size_t repeats_number = 100;
//			constexpr size_t threads_number = 16;
//			PersistentVector<size_t> pvector = { 1, 2, 3, 4, 5 };
//			std::thread threads[threads_number];
//			for (size_t repeat = 0; repeat < repeats_number; ++repeat) {
//				for (size_t i = 0; i < threads_number; ++i) {
//					threads[i] = std::thread(&PushBackFunc, std::ref(pvector));
//				}
//				for (size_t i = 0; i < threads_number; ++i) {
//					threads[i].join();
//				}
//			}
//		}
//	}
//
//	namespace {
//		void UndoRedoFunc(PersistentVector<size_t>& pvector) {
//			auto id = std::hash<std::thread::id>{}(std::this_thread::get_id());
//			auto myPvector = pvector.undo();
//			std::this_thread::sleep_for(std::chrono::milliseconds(1));
//			myPvector = myPvector.undo();
//			std::this_thread::sleep_for(std::chrono::milliseconds(1));
//			myPvector = myPvector.undo();
//			std::this_thread::sleep_for(std::chrono::milliseconds(1));
//			myPvector = myPvector.undo();
//			std::this_thread::sleep_for(std::chrono::milliseconds(1));
//			myPvector = myPvector.undo();
//			std::this_thread::sleep_for(std::chrono::milliseconds(1));
//			myPvector = myPvector.redo();
//			std::this_thread::sleep_for(std::chrono::milliseconds(1));
//			myPvector = myPvector.redo();
//			std::this_thread::sleep_for(std::chrono::milliseconds(1));
//			myPvector = myPvector.redo();
//			std::this_thread::sleep_for(std::chrono::milliseconds(1));
//			myPvector = myPvector.push_back(id);
//			std::this_thread::sleep_for(std::chrono::milliseconds(1));
//			myPvector = myPvector.push_back(id + 1);
//			for (size_t i = 0; i < 3; ++i) {
//				EXPECT_EQ(myPvector[i], i + 1);
//			}
//			for (size_t i = 0; i < 2; ++i) {
//				EXPECT_EQ(myPvector[i + 3], id + i);
//			}
//		}
//
//		TEST(PVectorConcurrency, UndoRedo) {
//			constexpr size_t repeats_number = 100;
//			constexpr size_t threads_number = 16;
//			PersistentVector<size_t> pvector;
//			pvector = pvector.push_back(1).push_back(2).push_back(3).push_back(4).push_back(5);
//			std::thread threads[threads_number];
//			for (size_t repeat = 0; repeat < repeats_number; ++repeat) {
//				for (size_t i = 0; i < threads_number; ++i) {
//					threads[i] = std::thread(&UndoRedoFunc, std::ref(pvector));
//				}
//				for (size_t i = 0; i < threads_number; ++i) {
//					threads[i].join();
//				}
//			}
//		}
//	}

}
