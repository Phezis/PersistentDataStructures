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
		constexpr size_t size = (1 << 12) + 1;
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

	TEST(PVectorCertainValueCreation, NoOne) {
		PersistentVector<size_t> pvector(0, 0);
		EXPECT_EQ(pvector.size(), 0);
		EXPECT_TRUE(pvector.empty());
	}

	TEST(PVectorCertainValueCreation, OnlyOne) {
		PersistentVector<size_t> pvector(1, 12345);
		EXPECT_EQ(pvector.size(), 1);
		EXPECT_EQ(pvector[0], 12345);
	}

	TEST(PVectorCertainValueCreation, Huge) {
		constexpr size_t size = (1 << 15) + 1;
		PersistentVector<size_t> pvector(size, 12345);
		EXPECT_EQ(pvector.size(), size);
		for (size_t i = 0; i < size; ++i) {
			EXPECT_EQ(pvector[i], 12345);
		}
	}

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


	/*
	*	Setting
	*/

	TEST(PVectorSetting, OnlyOne) {
		PersistentVector<size_t> srcpvector = { 12345 };
		auto pvector = srcpvector.set(0, 0);
		EXPECT_EQ(srcpvector.size(), 1);
		EXPECT_EQ(pvector.size(), 1);
		EXPECT_EQ(srcpvector[0], 12345);
		EXPECT_EQ(pvector[0], 0);
	}

	TEST(PVectorSetting, Some) {
		PersistentVector<size_t> pvector = { 1, 2, 3, 4, 5 };
		for (size_t i = 0; i < pvector.size(); ++i) {
			pvector = pvector.set(i, i);
		}
		EXPECT_EQ(pvector.size(), 5);
		for (size_t i = 0; i < pvector.size(); ++i) {
			EXPECT_EQ(pvector[i], i);
		}
	}

	TEST(PVectorSetting, Huge) {
		constexpr size_t size = (1 << 13) + 1;
		PersistentVector<size_t> pvector(size, 0);
		for (size_t i = 0; i < pvector.size(); ++i) {
			pvector = pvector.set(i, i + 1);
		}
		EXPECT_EQ(pvector.size(), size);
		for (size_t i = 0; i < pvector.size(); ++i) {
			EXPECT_EQ(pvector[i], i + 1);
		}
	}


	/*
	*	Iterators
	*/


	TEST(PVectorIterator, Empty) {
		PersistentVector<size_t> pvector;
		EXPECT_TRUE(pvector.cbegin() == pvector.cend());
		EXPECT_TRUE(pvector.crbegin() == pvector.crend());
	}

	
	TEST(PVectorIterator, OnlyOne) {
		PersistentVector<size_t> pvector = { 12345 };
		// const iterator
		{
			EXPECT_TRUE(pvector.cbegin() != pvector.cend());
			auto it = pvector.cbegin();
			EXPECT_EQ(*it, 12345);
			EXPECT_TRUE(++it == pvector.cend());
		}
		// const reversed iterator
		{
			EXPECT_TRUE(pvector.crbegin() != pvector.crend());
			auto it = pvector.crbegin();
			EXPECT_EQ(*it, 12345);
			EXPECT_TRUE(++it == pvector.crend());
		}
	}
	
	TEST(PVectorIterator, Some) {
		PersistentVector<size_t> pvector(10, 12345);
		// const iterator
		{
			EXPECT_TRUE(pvector.cbegin() != pvector.cend());
			for (auto it = pvector.cbegin(); it != pvector.cend(); ++it) {
				EXPECT_EQ(*it, 12345);
			}
		}
		// const reversed iterator
		{
			EXPECT_TRUE(pvector.crbegin() != pvector.crend());
			for (auto it = pvector.crbegin(); it != pvector.crend(); ++it) {
				EXPECT_EQ(*it, 12345);
			}
		}
	}

	TEST(PVectorIterator, Huge) {
		constexpr size_t size = (1 << 13) + 1;
		PersistentVector<size_t> pvector(10, 12345);
		// const iterator
		{
			EXPECT_TRUE(pvector.cbegin() != pvector.cend());
			for (auto it = pvector.cbegin(); it != pvector.cend(); ++it) {
				EXPECT_EQ(*it, 12345);
			}
		}
		// const reversed iterator
		{
			EXPECT_TRUE(pvector.crbegin() != pvector.crend());
			for (auto it = pvector.crbegin(); it != pvector.crend(); ++it) {
				EXPECT_EQ(*it, 12345);
			}
		}
	}
	
	TEST(PVectorIterator, ChangeAssign) {
#define PVECTOR_ITERATOR_CHANGE_ASSIGN(pvector, begin, end)	\
		auto it = pvector.begin();							\
		EXPECT_EQ(*it, 1);									\
		it += 0;											\
		EXPECT_EQ(*it, 1);									\
		it += 1;											\
		EXPECT_EQ(*it, 2);									\
		it += 2;											\
		EXPECT_EQ(*it, 4);									\
		it += 3;											\
		EXPECT_EQ(*it, 7);									\
		it += 4;											\
		EXPECT_TRUE(it == pvector.end());					\
		it -= 1;											\
		EXPECT_EQ(*it, 10);									\
		it -= 0;											\
		EXPECT_EQ(*it, 10);									\
		it -= 2;											\
		EXPECT_EQ(*it, 8);									\
		it -= 3;											\
		EXPECT_EQ(*it, 5);									\
		it -= 4;											\
		EXPECT_EQ(*it, 1);									\
		EXPECT_TRUE(it == pvector.begin());

		// const iterator
		{
			PersistentVector<size_t> pvector = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
			PVECTOR_ITERATOR_CHANGE_ASSIGN(pvector, cbegin, cend)
		}
		// const reversed iterator
		{
			PersistentVector<size_t> pvector = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
			PVECTOR_ITERATOR_CHANGE_ASSIGN(pvector, crbegin, crend)
		}
	}

	TEST(PVectorIterator, IncrementAndDecrement) {
#define PVECTOR_ITERATOR_INCREMENT_AND_DECREMENT(pvector, begin, end)	\
		auto it = pvector.begin();										\
		EXPECT_EQ(*it, 1);												\
		++it;															\
		EXPECT_EQ(*it, 2);												\
		it++;															\
		EXPECT_EQ(*it, 3);												\
		EXPECT_EQ(*(++it), 4);											\
		EXPECT_EQ(*(it++), 4);											\
		EXPECT_EQ(*it, 5);												\
		EXPECT_EQ(*(it++), 5);											\
		EXPECT_TRUE(it == pvector.end());								\
		--it;															\
		EXPECT_EQ(*it, 5);												\
		it--;															\
		EXPECT_EQ(*it, 4);												\
		EXPECT_EQ(*(--it), 3);											\
		EXPECT_EQ(*(it--), 3);											\
		EXPECT_EQ(*it, 2);												\
		EXPECT_EQ(*(it--), 2);											\
		EXPECT_TRUE(it == pvector.begin());


		// const iterator
		{
			PersistentVector<size_t> pvector = { 1, 2, 3, 4, 5 };
			PVECTOR_ITERATOR_INCREMENT_AND_DECREMENT(pvector, cbegin, cend)
		}
		// const reversed iterator
		{
			PersistentVector<size_t> pvector = { 5, 4, 3, 2, 1 };
			PVECTOR_ITERATOR_INCREMENT_AND_DECREMENT(pvector, crbegin, crend)
		}
	}


	TEST(PVectorIterator, GetById) {
#define PVECTOR_ITERATOR_GET_BY_ID(pvector, begin)	\
		auto it = pvector.begin();					\
		EXPECT_EQ(it[0], 1);						\
		EXPECT_EQ(it[9], 10);						\
		it += 4;									\
		EXPECT_EQ(it[-4], 1);						\
		EXPECT_EQ(it[0], 5);						\
		EXPECT_EQ(it[5], 10);						\
		it += 5;									\
		EXPECT_EQ(it[-9], 1);						\
		EXPECT_EQ(it[0], 10);


		// const iterator
		{
			PersistentVector<size_t> pvector = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
			PVECTOR_ITERATOR_GET_BY_ID(pvector, cbegin)
		}
		// const reversed iterator
		{
			PersistentVector<size_t> pvector = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
			PVECTOR_ITERATOR_GET_BY_ID(pvector, crbegin)
		}
	}

	TEST(PVectorIterator, BinaryPlusAndMinus) {
#define PVECTOR_ITERATOR_BINARY_PLUS_AND_MINUS(pvector, begin, end)	\
		auto it = pvector.begin();									\
		EXPECT_EQ(*(it + 0), 1);									\
		EXPECT_EQ(*(it + 1), 2);									\
		EXPECT_EQ(*(it + 2), 3);									\
		EXPECT_EQ(*(it + 5), 6);									\
		EXPECT_EQ(*(it + 9), 10);									\
		EXPECT_TRUE(it + 10 == pvector.end());						\
		it = it + 9;												\
		EXPECT_EQ(*(it - 0), 10);									\
		EXPECT_EQ(*(it - 1), 9);									\
		EXPECT_EQ(*(it - 2), 8);									\
		EXPECT_EQ(*(it - 5), 5);									\
		EXPECT_EQ(*(it - 9), 1);									\
		EXPECT_TRUE(it - 9 == pvector.begin());



		// const iterator
		{
			PersistentVector<size_t> pvector = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
			PVECTOR_ITERATOR_BINARY_PLUS_AND_MINUS(pvector, cbegin, cend)
		}
		// const reversed iterator
		{
			PersistentVector<size_t> pvector = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
			PVECTOR_ITERATOR_BINARY_PLUS_AND_MINUS(pvector, crbegin, crend)
		}
	}

	TEST(PVectorIterator, Difference) {
#define PVECTOR_ITERATOR_DIFFERENCE(pvector, begin, end)	\
		const auto begin = pvector.begin();					\
		const auto end = pvector.end();						\
		auto it = pvector.begin();							\
		EXPECT_EQ(it - begin, 0);							\
		EXPECT_EQ(begin - it, 0);							\
		it += 1;											\
		EXPECT_EQ(it - begin, 1);							\
		EXPECT_EQ(begin - it, -1);							\
		it += 2;											\
		EXPECT_EQ(it - begin, 3);							\
		EXPECT_EQ(begin - it, -3);							\
		it += 5;											\
		EXPECT_EQ(it - begin, 8);							\
		EXPECT_EQ(begin - it, -8);							\
		it += 2;											\
		EXPECT_EQ(it - begin, 10);							\
		EXPECT_EQ(begin - it, -10);							\
		EXPECT_EQ(it - end, 0);								\
		EXPECT_EQ(end - it, 0);								\
		it -= 1;											\
		EXPECT_EQ(it - end, -1);							\
		EXPECT_EQ(end - it, 1);								\
		it -= 2;											\
		EXPECT_EQ(it - end, -3);							\
		EXPECT_EQ(end - it, 3);								\
		it -= 5;											\
		EXPECT_EQ(it - end, -8);							\
		EXPECT_EQ(end - it, 8);								\
		it -= 2;											\
		EXPECT_EQ(it - end, -10);							\
		EXPECT_EQ(end - it, 10);							\
		EXPECT_EQ(it - begin, 0);							\
		EXPECT_EQ(begin - it, 0);


		// const iterator
		{
			PersistentVector<size_t> pvector = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
			PVECTOR_ITERATOR_DIFFERENCE(pvector, cbegin, cend)
		}
		// const reversed iterator
		{
			PersistentVector<size_t> pvector = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
			PVECTOR_ITERATOR_DIFFERENCE(pvector, crbegin, crend)
		}
	}

	TEST(PVectorIterator, Comparation) {
#define PVECTOR_ITERATOR_COMPARATION(pvector, begin, end)	\
			const auto begin = pvector.begin();				\
			const auto end = pvector.end();					\
			auto it = pvector.begin();						\
			EXPECT_TRUE(it == begin);						\
			EXPECT_TRUE(it + 5 == end);						\
			EXPECT_TRUE(it >= begin);						\
			EXPECT_TRUE(it + 5 >= end);						\
			EXPECT_TRUE(it <= begin);						\
			EXPECT_TRUE(it + 5 <= end);						\
			it += 2;										\
			EXPECT_FALSE(it == begin);						\
			EXPECT_FALSE(it == end);						\
			EXPECT_TRUE(it != begin);						\
			EXPECT_TRUE(it != end);							\
			EXPECT_TRUE(begin < it);						\
			EXPECT_TRUE(it < end);;							\
			EXPECT_TRUE(it > begin);						\
			EXPECT_TRUE(end > it);


		PersistentVector<size_t> pvector = { 1, 2, 3, 4, 5 };
		// const iterator
		{
			PVECTOR_ITERATOR_COMPARATION(pvector, cbegin, cend)
		}
		// const reversed iterator
		{
			PVECTOR_ITERATOR_COMPARATION(pvector, crbegin, crend)
		}
	}

}
