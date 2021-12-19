#pragma once
#include "PersistentVector.h"
#include "Utils.h"

#include <memory>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iterator>
#include <utility>

namespace pds {
    template<typename Key, typename T, typename Hash = std::hash<Key>>
    class PersistentMap;

    template<typename Key, typename T, typename Hash>
    class PersistentMap {
        static constexpr std::size_t DEFAULT_INITIAL_SIZE = 256;
        struct TWrapper;
    public:
        class map_const_iterator;
        using const_iterator = map_const_iterator;

        class map_const_iterator {
            const typename PersistentVector<PersistentVector<TWrapper>>::const_iterator m_outer_end;
            typename PersistentVector<PersistentVector<TWrapper>>::const_iterator m_outer;
            typename PersistentVector<TWrapper>::const_iterator m_inner;
        public:
            using iterator_category = std::random_access_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = const T*;
            using reference = const T&;

            map_const_iterator() = delete;
            map_const_iterator(typename PersistentVector<PersistentVector<TWrapper>>::const_iterator m_outer_end,
                                typename PersistentVector<PersistentVector<TWrapper>>::const_iterator m_outer,
                                typename PersistentVector<TWrapper>::const_iterator m_inner)
                                : m_outer_end(m_outer_end), m_outer(m_outer), m_inner(m_inner) {}
            map_const_iterator(const map_const_iterator& other) = default;
            map_const_iterator(map_const_iterator&& other) = default;

            map_const_iterator& operator=(const map_const_iterator& other) = default;
            map_const_iterator& operator=(map_const_iterator&& other) = default;

            ~map_const_iterator() = default;

            inline const T& operator*() const;
            inline const T* operator->() const;

            inline map_const_iterator& operator++();
            inline map_const_iterator operator++(int);

            inline bool operator==(const map_const_iterator& other) const;
            inline bool operator!=(const map_const_iterator& other) const;
        };

        PersistentMap(std::size_t initial_size, const Hash& hash) :
            m_hash(hash),
            m_size(0),
            m_vector(std::make_shared<PersistentVector<PersistentVector<TWrapper>>>(initial_size)) {}

        PersistentMap() : PersistentMap(DEFAULT_INITIAL_SIZE, Hash()) {}
        PersistentMap(const PersistentMap& other) = default;
        PersistentMap(PersistentMap&& other) noexcept = default;


        PersistentMap(std::size_t initial_size) : PersistentMap(initial_size, Hash()) {}

        template<typename InputIt, typename std::enable_if<is_iterator<InputIt>, bool>::type = false>
        PersistentMap(InputIt first, InputIt last, std::size_t initial_size, const Hash& hash);

        template<typename InputIt, typename std::enable_if<is_iterator<InputIt>, bool>::type = false>
        PersistentMap(InputIt first, InputIt last, std::size_t initial_size) : PersistentMap(first, last, initial_size, Hash()) {}

        PersistentMap(std::initializer_list<std::pair<const Key, T>> init, std::size_t initial_size, Hash& hash) : PersistentMap(init.begin(), init.end(), initial_size, hash) {}
        PersistentMap(std::initializer_list<std::pair<const Key, T>> init, std::size_t initial_size) : PersistentMap(init.begin(), init.end(), initial_size) {}
        PersistentMap(std::initializer_list<std::pair<const Key, T>> init) : PersistentMap(init.begin(), init.end(), DEFAULT_INITIAL_SIZE) {}

        ~PersistentMap() = default;

        PersistentMap& operator=(const PersistentMap& other) = default;
        PersistentMap& operator=(PersistentMap&& other) = default;

        const_iterator cbegin() const;
        const_iterator cend() const;

        const T& operator[](const Key& key) const;

        const T& at(const Key& key) const;

        PersistentMap set(const Key& key, const T& value) const;

        bool operator==(const PersistentMap& other) const;
        bool operator!=(const PersistentMap& other) const;

        void swap(PersistentMap& other);


        std::size_t size() const;
        bool empty() const;

        bool canUndo() const;
        bool canRedo() const;

        PersistentMap undo() const;
        PersistentMap redo() const;

        PersistentMap clear() const;

        std::size_t count(const Key& key) const;
        // iterator find() const;
        bool contains(const Key& key) const;

        // if element does not exist throws out_of_range
        PersistentMap erase(const Key& key) const;

    private:
        PersistentMap(const Hash& hash, std::size_t size, std::shared_ptr<PersistentVector<PersistentVector<TWrapper>>> vector) :
            m_hash(hash),
            m_size(size),
            m_vector(vector) {}

        PersistentMap set(const Key& key, std::shared_ptr<T> value) const;

        // returns true if that was unique Key and false if existed key was updated
        static bool insertToSequenceAsHash(std::vector<std::vector<TWrapper>>& sequence, const Key& key, std::shared_ptr<T> value, std::size_t hash);

        template<typename InputIt, typename std::enable_if<is_iterator<InputIt>, bool>::type = false>
        static std::vector<std::vector<TWrapper>> getReallocatedVector(InputIt begin, InputIt end, std::size_t size, const Hash& hashFunc);

        static std::vector<PersistentVector<TWrapper>> getReallocatedVectorOfPersistentVectors(std::vector<std::vector<TWrapper>> resetVector, std::size_t size);

        std::vector<PersistentVector<TWrapper>> getReallocatedVectorOfPersistentVectors(const Key& key, std::shared_ptr<T> value) const;

        struct TWrapper {
            TWrapper() = default;
            TWrapper(std::size_t hash, Key key, std::shared_ptr<T> value) : hash(hash), key(key), value(value) {}

            std::size_t hash;
            Key key;
            std::shared_ptr<T> value;
        };

        Hash m_hash;
        std::size_t m_size;
        std::shared_ptr<PersistentVector<PersistentVector<TWrapper>>> m_vector;
    };

    /*
    *
    *   Implementation
    *
    */


    /*
    *
    *   Iterators
    *
    */

    template<typename Key, typename T, typename Hash>
    inline const T& PersistentMap<Key, T, Hash>::map_const_iterator::operator*() const {
        return *(m_inner->value);
    }

    template<typename Key, typename T, typename Hash>
    inline const T* PersistentMap<Key, T, Hash>::map_const_iterator::operator->() const {
        return &(*(m_inner->value));
    }

    template<typename Key, typename T, typename Hash>
    inline typename PersistentMap<Key, T, Hash>::map_const_iterator& PersistentMap<Key, T, Hash>::map_const_iterator::operator++() {
        ++m_inner;
        if (m_inner == m_outer->cend()) {
            m_outer = std::find_if(++m_outer, m_outer_end, [](const PersistentVector<TWrapper>& pvector) { return !pvector.empty(); });
            if (m_outer != m_outer_end) {
                m_inner = m_outer->cbegin();
            }
        }
        return *this;
    }

    template<typename Key, typename T, typename Hash>
    inline typename PersistentMap<Key, T, Hash>::map_const_iterator PersistentMap<Key, T, Hash>::map_const_iterator::operator++(int) {
        auto copy = *this;
        operator++();
        return copy;
    }

    template<typename Key, typename T, typename Hash>
    inline bool PersistentMap<Key, T, Hash>::map_const_iterator::operator==(const map_const_iterator& other) const {
        return m_outer == other.m_outer;
    }

    template<typename Key, typename T, typename Hash>
    inline bool PersistentMap<Key, T, Hash>::map_const_iterator::operator!=(const map_const_iterator& other) const {
        return !(*this == other);
    }


    /*
    *
    *   Persistent map
    *
    */

    template<typename Key, typename T, typename Hash>
    template<typename InputIt, typename std::enable_if<is_iterator<InputIt>, bool>::type>
    PersistentMap<Key, T, Hash>::PersistentMap(InputIt first, InputIt last, std::size_t initial_size, const Hash& hashFunc) : PersistentMap(initial_size, hashFunc) {
        std::vector<std::vector<TWrapper>> resetVector(initial_size);
        m_size = 0;
        for (auto it = first; it != last; ++it) {
            auto hash = hashFunc(it->first) % (initial_size);
            if (insertToSequenceAsHash(resetVector, it->first, std::make_shared<T>(it->second), hash)) {
                ++m_size;
            }
        }
        auto vectorOfPersistentVectors = getReallocatedVectorOfPersistentVectors(resetVector, initial_size);
        m_vector = std::make_shared<PersistentVector<PersistentVector<TWrapper>>>(vectorOfPersistentVectors.cbegin(), vectorOfPersistentVectors.cend());
    }

    template<typename Key, typename T, typename Hash>
    inline typename PersistentMap<Key, T, Hash>::const_iterator PersistentMap<Key, T, Hash>::cbegin() const {
        auto it = std::find_if(m_vector->cbegin(), m_vector->cend(), [](const PersistentVector<TWrapper>& pvector) { return !pvector.empty(); });
        return const_iterator(m_vector->cend(), it, it == m_vector->cend() ? m_vector->cbegin()->cbegin() : it->cbegin());
    }

    template<typename Key, typename T, typename Hash>
    inline typename PersistentMap<Key, T, Hash>::const_iterator PersistentMap<Key, T, Hash>::cend() const {
        return const_iterator(m_vector->cend(), m_vector->cend(), m_vector->cbegin()->cbegin());
    }

    template<typename Key, typename T, typename Hash>
    inline const T& PersistentMap<Key, T, Hash>::operator[](const Key& key) const {
        auto hash = m_hash(key) % m_vector->size();
        auto it = std::find_if((*m_vector)[hash].cbegin(), (*m_vector)[hash].cend(), [&key](const TWrapper& wrapper) { return key == wrapper.key; });
        return *(it->value);
    }

    template<typename Key, typename T, typename Hash>
    inline const T& PersistentMap<Key, T, Hash>::at(const Key& key) const {
        auto hash = m_hash(key) % m_vector->size();
        const auto& subseq = (*m_vector)[hash];
        if (subseq.empty()) {
            throw std::out_of_range("Key not found");
        }
        auto it = std::find_if(subseq.cbegin(), subseq.cend(), [&key](const TWrapper& wrapper) { return key == wrapper.key; });
        if (it == subseq.cend()) {
            throw std::out_of_range("Key not found");
        }
        return *(it->value);
    }

    template<typename Key, typename T, typename Hash>
    inline PersistentMap<Key, T, Hash> PersistentMap<Key, T, Hash>::set(const Key& key, const T& value) const {
        return set(key, std::make_shared<T>(value));
    }

    template<typename Key, typename T, typename Hash>
    inline bool PersistentMap<Key, T, Hash>::operator==(const PersistentMap& other) const
    {
        return false;
    }

    template<typename Key, typename T, typename Hash>
    inline bool PersistentMap<Key, T, Hash>::operator!=(const PersistentMap& other) const
    {
        return false;
    }

    template<typename Key, typename T, typename Hash>
    inline void PersistentMap<Key, T, Hash>::swap(PersistentMap& other)
    {
    }

    template<typename Key, typename T, typename Hash>
    inline std::size_t PersistentMap<Key, T, Hash>::size() const {
        return m_size;
    }

    template<typename Key, typename T, typename Hash>
    inline bool PersistentMap<Key, T, Hash>::empty() const {
        return 0 == size();
    }

    template<typename Key, typename T, typename Hash>
    inline bool PersistentMap<Key, T, Hash>::canUndo() const {
        return m_vector->canUndo();
    }

    template<typename Key, typename T, typename Hash>
    inline bool PersistentMap<Key, T, Hash>::canRedo() const {
        return m_vector->canRedo();
    }

    template<typename Key, typename T, typename Hash>
    inline PersistentMap<Key, T, Hash> PersistentMap<Key, T, Hash>::undo() const {
        auto outVector = std::make_shared<PersistentVector<PersistentVector<TWrapper>>>(m_vector->undo());
        std::size_t newSize = 0;
        for (auto it = outVector.cbegin(); it != outVector.cend(); ++it) {
            newSize += it->size();
        }
        return PersistentMap<Key, T, Hash>(m_hash, newSize, outVector);
    }

    template<typename Key, typename T, typename Hash>
    inline PersistentMap<Key, T, Hash> PersistentMap<Key, T, Hash>::redo() const {
        auto outVector = std::make_shared<PersistentVector<PersistentVector<TWrapper>>>(m_vector->redo());
        std::size_t newSize = 0;
        for (auto it = outVector.cbegin(); it != outVector.cend(); ++it) {
            newSize += it->size();
        }
        return PersistentMap<Key, T, Hash>(m_hash, newSize, outVector);
    }

    template<typename Key, typename T, typename Hash>
    inline PersistentMap<Key, T, Hash> PersistentMap<Key, T, Hash>::clear() const {
        auto outVector = std::make_shared<PersistentVector<PersistentVector<TWrapper>>>(m_vector->clear());
        std::size_t newSize = 0;
        return PersistentMap<Key, T, Hash>(m_hash, newSize, outVector);
    }

    template<typename Key, typename T, typename Hash>
    inline std::size_t PersistentMap<Key, T, Hash>::count(const Key& key) const {
        auto hash = m_hash(key) % m_vector->size();
        auto it = std::find_if((*m_vector)[hash].cbegin(), (*m_vector)[hash].cend(), [&key](const TWrapper& wrapper) { return key == wrapper.key; });
        return it == (*m_vector)[hash].cend() ? 0 : 1;
    }

    template<typename Key, typename T, typename Hash>
    inline bool PersistentMap<Key, T, Hash>::contains(const Key& key) const {
        return count(key) > 0;
    }

    template<typename Key, typename T, typename Hash>
    inline PersistentMap<Key, T, Hash> PersistentMap<Key, T, Hash>::erase(const Key& key) const {
        auto hash = m_hash(key) % m_vector->size();
        std::vector<TWrapper> v((*m_vector)[hash].cbegin(), (*m_vector)[hash].cend());
        auto target = std::find_if(v.begin(), v.end(), [&key](const TWrapper& wrapper) { return key == wrapper.key; });
        if (target == v.end()) {
            throw std::out_of_range("Key not found");
        }
        std::size_t newSize = m_size - 1;
        v.erase(target);
        PersistentVector<TWrapper> newSeq(v.begin(), v.end());
        auto outVector = std::make_shared<PersistentVector<PersistentVector<TWrapper>>>(m_vector->set(hash, newSeq));
        return PersistentMap<Key, T, Hash>(m_hash, newSize, outVector);
    }

    template<typename Key, typename T, typename Hash>
    inline PersistentMap<Key, T, Hash> PersistentMap<Key, T, Hash>::set(const Key& key, std::shared_ptr<T> value) const {
        std::size_t newSize = m_size;
        auto hash = m_hash(key) % m_vector->size();
        std::shared_ptr<PersistentVector<PersistentVector<TWrapper>>> outVector;
        if ((*m_vector)[hash].empty()) {
            ++newSize;
            if (newSize > m_vector->size() / 2) {
                auto reallocatedVector = getReallocatedVectorOfPersistentVectors(key, value);
                outVector = std::make_shared<PersistentVector<PersistentVector<TWrapper>>>(
                    m_vector->reset(reallocatedVector.cbegin(), reallocatedVector.cend()));
            }
            else {
                outVector = std::make_shared<PersistentVector<PersistentVector<TWrapper>>>(
                    m_vector->set(hash, (*m_vector)[hash].push_back(std::move(TWrapper(hash, key, std::move(value))))));
            }
        }
        else {
            auto collided = std::find_if((*m_vector)[hash].cbegin(), (*m_vector)[hash].cend(), [&key](const TWrapper& wrapper) { return key == wrapper.key; });
            if (collided == (*m_vector)[hash].cend()) {
                ++newSize;
                if (newSize > m_vector->size() / 2) {
                    auto reallocatedVector = getReallocatedVectorOfPersistentVectors(key, value);
                    outVector = std::make_shared<PersistentVector<PersistentVector<TWrapper>>>(
                        m_vector->reset(reallocatedVector.cbegin(), reallocatedVector.cend()));
                }
                else {
                    outVector = std::make_shared<PersistentVector<PersistentVector<TWrapper>>>(
                        m_vector->set(hash, (*m_vector)[hash].push_back(std::move(TWrapper(hash, key, std::move(value))))));
                }
            }
            else {
                outVector = std::make_shared<PersistentVector<PersistentVector<TWrapper>>>(
                    m_vector->set(hash, (*m_vector)[hash].set(collided.getId(), std::move(TWrapper(hash, key, std::move(value))))));
            }
        }
        return PersistentMap<Key, T, Hash>(m_hash, newSize, outVector);
    }

    template<typename Key, typename T, typename Hash>
    inline bool PersistentMap<Key, T, Hash>::insertToSequenceAsHash(std::vector<std::vector<TWrapper>>& sequence, const Key& key, std::shared_ptr<T> value, std::size_t hash) {
        bool found = false;
        if (sequence[hash].empty()) {
            sequence[hash].emplace_back(hash, key, value);
        }
        else {
            for (auto& wrapper : sequence[hash]) {
                if (wrapper.key == key) {
                    wrapper.value = std::move(value);
                    found = true;
                    break;
                }
            }
            if (!found) {
                sequence[hash].emplace_back(hash, key, value);
            }
        }
        return !found;
    }

    template<typename Key, typename T, typename Hash>
    template<typename InputIt, typename std::enable_if<is_iterator<InputIt>, bool>::type>
    inline std::vector<std::vector<typename PersistentMap<Key, T, Hash>::TWrapper>> PersistentMap<Key, T, Hash>::getReallocatedVector(InputIt begin, InputIt end, std::size_t size, const Hash& hashFunc) {
        std::vector<std::vector<TWrapper>> resetVector(size);
        for (auto it = begin; it != end; ++it) {
            for (auto it_in = it->cbegin(); it_in != it->cend(); ++it_in) {
                auto hash = hashFunc(it_in->key) % (size);
                insertToSequenceAsHash(resetVector, it_in->key, it_in->value, hash);
            }
        }
        return resetVector;
    }

    template<typename Key, typename T, typename Hash>
    inline std::vector<PersistentVector<typename PersistentMap<Key, T, Hash>::TWrapper>> 
        PersistentMap<Key, T, Hash>::getReallocatedVectorOfPersistentVectors(std::vector<std::vector<typename PersistentMap<Key, T, Hash>::TWrapper>> resetVector, std::size_t size)
    {
        std::vector<PersistentVector<TWrapper>> out(size);
        for (std::size_t i = 0; i < resetVector.size(); ++i) {
            out[i] = PersistentVector<TWrapper>(resetVector[i].cbegin(), resetVector[i].cend());
        }
        return out;
    }

    template<typename Key, typename T, typename Hash>
    inline std::vector<PersistentVector<typename PersistentMap<Key, T, Hash>::TWrapper>> PersistentMap<Key, T, Hash>::getReallocatedVectorOfPersistentVectors(const Key& key, std::shared_ptr<T> value) const {
        auto resetVector = getReallocatedVector(m_vector->cbegin(), m_vector->cend(), m_vector->size() * 2, m_hash);
        auto hash = m_hash(key) % (m_vector->size() * 2);
        if (!resetVector[hash].empty()) {
            bool found = false;
            for (auto& wrapper : resetVector[hash]) {
                if (wrapper.key == key) {
                    wrapper.value = std::move(value);
                    found = true;
                    break;
                }
            }
            if (!found) {
                resetVector[hash].emplace_back(hash, key, std::move(value));
            }
        }
        else {
            resetVector[hash].emplace_back(hash, key, std::move(value));
        }
        return getReallocatedVectorOfPersistentVectors(resetVector, m_vector->size() * 2);
    }
}