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

    /*template<typename T>
    class vector_const_iterator {
        std::size_t m_id;
        const PersistentVector<T>* m_pvector;
    public:
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = const T*;
        using reference = const T&;

        vector_const_iterator() = delete;
        vector_const_iterator(std::size_t id, const PersistentVector<T>* pvector) : m_id(id), m_pvector(pvector) {}
        vector_const_iterator(const vector_const_iterator& other) = default;
        vector_const_iterator(vector_const_iterator&& other) = default;

        vector_const_iterator& operator=(const vector_const_iterator& other) = default;
        vector_const_iterator& operator=(vector_const_iterator&& other) = default;

        ~vector_const_iterator() = default;

        inline vector_const_iterator& operator+=(const difference_type shift);
        inline vector_const_iterator& operator-=(const difference_type shift);
        inline const T& operator*() const;
        inline const T* operator->() const;
        inline const T& operator[](const difference_type shift) const;

        inline vector_const_iterator& operator++();
        inline vector_const_iterator& operator--();
        inline vector_const_iterator operator++(int);
        inline vector_const_iterator operator--(int);

        inline difference_type operator-(const vector_const_iterator& other) const;
        inline vector_const_iterator operator+(const difference_type shift) const;
        inline vector_const_iterator operator-(const difference_type shift) const;

        inline bool operator==(const vector_const_iterator& other) const;
        inline bool operator!=(const vector_const_iterator& other) const;
        inline bool operator>(const vector_const_iterator& other) const;
        inline bool operator<(const vector_const_iterator& other) const;
        inline bool operator>=(const vector_const_iterator& other) const;
        inline bool operator<=(const vector_const_iterator& other) const;
    };*/

    /*template<typename T>
    inline vector_const_iterator<T> operator+(const typename vector_const_iterator<T>::difference_type lhs, const vector_const_iterator<T>& rhs);
    template<typename T>
    inline vector_const_iterator<T> operator-(const typename vector_const_iterator<T>::difference_type lhs, const vector_const_iterator<T>& rhs);*/

    template<typename Key, typename T, typename Hash>
    class PersistentMap {
        static constexpr std::size_t DEFAULT_INITIAL_SIZE = 256;
    public:
        /*using const_iterator = vector_const_iterator<T>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;*/

        PersistentMap(std::size_t initial_size, const Hash& hash) :
            m_hash(hash),
            m_size(0),
            m_vector(std::make_shared<PersistentVector<PersistentVector<TWrapper>>>(initial_size)) {}

        PersistentMap() : PersistentMap(DEFAULT_INITIAL_SIZE, Hash()) {}
        PersistentMap(const PersistentMap& other) = default;
        PersistentMap(PersistentMap&& other) noexcept = default;


        PersistentMap(std::size_t initial_size) : PersistentMap(initial_size, Hash()) {}

        template<typename InputIt, typename std::enable_if<is_iterator<InputIt>, bool>::type = false>
        PersistentMap(InputIt first, InputIt last, std::size_t initial_size, Hash& hash);

        template<typename InputIt, typename std::enable_if<is_iterator<InputIt>, bool>::type = false>
        PersistentMap(InputIt first, InputIt last, std::size_t initial_size) : PersistentMap(first, last, initial_size, Hash()) {}

        PersistentMap(std::initializer_list<T> init, std::size_t initial_size, Hash& hash) : PersistentMap(init.begin(), init.end(), initial_size, hash) {}
        PersistentMap(std::initializer_list<T> init, std::size_t initial_size) : PersistentMap(init.begin(), init.end(), initial_size) {}

        ~PersistentMap() = default;

        PersistentMap& operator=(const PersistentMap& other) = default;
        PersistentMap& operator=(PersistentMap&& other) = default;

        // const_iterator cbegin() const;
        // const_iterator cend() const;

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

        PersistentMap erase(const Key& key) const;

    private:
        struct TWrapper;

        PersistentMap(const Hash& hash, std::size_t size, std::shared_ptr<PersistentVector<PersistentVector<TWrapper>>> vector) :
            m_hash(hash),
            m_size(size),
            m_vector(vector) {}

        std::vector<PersistentVector<TWrapper>> getReallocatedVector(const Key& key, const T& value) const;

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

    //template<typename T>
    //inline vector_const_iterator<T>& vector_const_iterator<T>::operator+=(const difference_type shift) {
    //    m_id += shift;
    //    return *this;
    //}

    //template<typename T>
    //inline vector_const_iterator<T>& vector_const_iterator<T>::operator-=(const difference_type shift) {
    //    m_id -= shift;
    //    return *this;
    //}

    //template<typename T>
    //inline const T& vector_const_iterator<T>::operator*() const {
    //    return (*m_pvector)[m_id];
    //}

    //template<typename T>
    //inline const T* vector_const_iterator<T>::operator->() const {
    //    return &(*m_pvector)[m_id];
    //}

    //template<typename T>
    //inline const T& vector_const_iterator<T>::operator[](const difference_type shift) const {
    //    return (*m_pvector)[m_id + shift];
    //}

    //template<typename T>
    //inline vector_const_iterator<T>& vector_const_iterator<T>::operator++() {
    //    ++m_id;
    //    return *this;
    //}

    //template<typename T>
    //inline vector_const_iterator<T>& vector_const_iterator<T>::operator--() {
    //    --m_id;
    //    return *this;
    //}

    //template<typename T>
    //inline vector_const_iterator<T> vector_const_iterator<T>::operator++(int) {
    //    auto copy = *this;
    //    ++m_id;
    //    return copy;
    //}

    //template<typename T>
    //inline vector_const_iterator<T> vector_const_iterator<T>::operator--(int) {
    //    auto copy = *this;
    //    --m_id;
    //    return copy;
    //}

    //template<typename T>
    //inline typename vector_const_iterator<T>::difference_type vector_const_iterator<T>::operator-(const vector_const_iterator& other) const {
    //    return static_cast<vector_const_iterator<T>::difference_type>(m_id - other.m_id);
    //}

    //template<typename T>
    //inline vector_const_iterator<T> vector_const_iterator<T>::operator+(const difference_type shift) const {
    //    auto copy = *this;
    //    copy += shift;
    //    return copy;
    //}

    //template<typename T>
    //inline vector_const_iterator<T> vector_const_iterator<T>::operator-(const difference_type shift) const {
    //    auto copy = *this;
    //    copy -= shift;
    //    return copy;
    //}

    //template<typename T>
    //inline bool vector_const_iterator<T>::operator==(const vector_const_iterator<T>& other) const {
    //    return m_id == other.m_id;
    //}

    //template<typename T>
    //inline bool vector_const_iterator<T>::operator!=(const vector_const_iterator<T>& other) const {
    //    return !(*this == other);
    //}

    //template<typename T>
    //inline bool vector_const_iterator<T>::operator<(const vector_const_iterator<T>& other) const {
    //    return m_id < other.m_id;
    //}

    //template<typename T>
    //inline bool vector_const_iterator<T>::operator>(const vector_const_iterator<T>& other) const {
    //    return other < *this;
    //}

    //template<typename T>
    //inline bool vector_const_iterator<T>::operator>=(const vector_const_iterator<T>& other) const {
    //    return !(*this < other);
    //}

    //template<typename T>
    //inline bool vector_const_iterator<T>::operator<=(const vector_const_iterator<T>& other) const {
    //    return !(other < *this);
    //}

    //template<typename T>
    //inline vector_const_iterator<T> operator+(const typename vector_const_iterator<T>::difference_type lhs, const vector_const_iterator<T>& rhs) {
    //    return rhs + lhs;
    //}

    //template<typename T>
    //inline vector_const_iterator<T> operator-(const typename vector_const_iterator<T>::difference_type lhs, const vector_const_iterator<T>& rhs) {
    //    return rhs - lhs;
    //}


    /*
    *
    *   Persistent map
    *
    */

    template<typename Key, typename T, typename Hash>
    template<typename InputIt, typename std::enable_if<is_iterator<InputIt>, bool>::type>
    PersistentMap<Key, T, Hash>::PersistentMap(InputIt first, InputIt last, std::size_t initial_size, Hash& hash) : PersistentMap(initial_size, hash) {

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
            throw std::out_of_range();
        }
        auto it = std::find_if(subseq.cbegin(), subseq.cend(), [&key](const TWrapper& wrapper) { return key == wrapper.key; });
        if (it == subseq.cend()) {
            throw std::out_of_range();
        }
        return *(it->value);
    }

    template<typename Key, typename T, typename Hash>
    inline PersistentMap<Key, T, Hash> PersistentMap<Key, T, Hash>::set(const Key& key, const T& value) const {
        std::size_t newSize = m_size;
        auto hash = m_hash(key) % m_vector->size();
        std::shared_ptr<PersistentVector<PersistentVector<TWrapper>>> outVector;
        if ((*m_vector)[hash].empty()) {
            ++newSize;
            if (newSize > m_vector->size() / 2) {
                auto reallocatedVector = getReallocatedVector(key, value);
                outVector = std::make_shared<PersistentVector<PersistentVector<TWrapper>>>(
                    m_vector->reset(reallocatedVector.cbegin(), reallocatedVector.cend()));
            }
            else {
                outVector = std::make_shared<PersistentVector<PersistentVector<TWrapper>>>(
                    m_vector->set(hash, (*m_vector)[hash].push_back(std::move(TWrapper(hash, key, std::move(std::make_shared<T>(value)))))));
            }
        }
        else {
            auto collided = std::find_if((*m_vector)[hash].cbegin(), (*m_vector)[hash].cend(), [&key](const TWrapper& wrapper) { return key == wrapper.key; });
            if (collided == (*m_vector)[hash].cend()) {
                ++newSize;
                if (newSize > m_vector->size() / 2) {
                    auto reallocatedVector = getReallocatedVector(key, value);
                    outVector = std::make_shared<PersistentVector<PersistentVector<TWrapper>>>(
                        m_vector->reset(reallocatedVector.cbegin(), reallocatedVector.cend()));
                }
                else {
                    outVector = std::make_shared<PersistentVector<PersistentVector<TWrapper>>>(
                        m_vector->set(hash, (*m_vector)[hash].push_back(std::move(TWrapper(hash, key, std::move(std::make_shared<T>(value)))))));
                }
            }
            else {
                outVector = std::make_shared<PersistentVector<PersistentVector<TWrapper>>>(
                    m_vector->set(hash, (*m_vector)[hash].set(collided.getId(), std::move(TWrapper(hash, key, std::move(std::make_shared<T>(value)))))));
            }
        }
        return PersistentMap<Key, T, Hash>(m_hash, newSize, outVector);
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
    inline bool PersistentMap<Key, T, Hash>::canUndo() const
    {
        return false;
    }

    template<typename Key, typename T, typename Hash>
    inline bool PersistentMap<Key, T, Hash>::canRedo() const
    {
        return false;
    }

    template<typename Key, typename T, typename Hash>
    inline PersistentMap<Key, T, Hash> PersistentMap<Key, T, Hash>::undo() const
    {
        return PersistentMap();
    }

    template<typename Key, typename T, typename Hash>
    inline PersistentMap<Key, T, Hash> PersistentMap<Key, T, Hash>::redo() const
    {
        return PersistentMap();
    }

    template<typename Key, typename T, typename Hash>
    inline PersistentMap<Key, T, Hash> PersistentMap<Key, T, Hash>::clear() const
    {
        return PersistentMap();
    }

    template<typename Key, typename T, typename Hash>
    inline std::size_t PersistentMap<Key, T, Hash>::count(const Key& key) const
    {
        return std::size_t();
    }

    template<typename Key, typename T, typename Hash>
    inline bool PersistentMap<Key, T, Hash>::contains(const Key& key) const
    {
        return false;
    }

    template<typename Key, typename T, typename Hash>
    inline PersistentMap<Key, T, Hash> PersistentMap<Key, T, Hash>::erase(const Key& key) const
    {
        return PersistentMap();
    }

    template<typename Key, typename T, typename Hash>
    inline std::vector<PersistentVector<typename PersistentMap<Key, T, Hash>::TWrapper>> PersistentMap<Key, T, Hash>::getReallocatedVector(const Key& key, const T& value) const {
        std::vector<std::vector<TWrapper>> resetVector(m_vector->size() * 2);
        for (auto it = m_vector->cbegin(); it != m_vector->cend(); ++it) {
            for (auto it_in = it->cbegin(); it_in != it->cend(); ++it_in) {
                auto hash = m_hash(it_in->key) % (m_vector->size() * 2);
                resetVector[hash].emplace_back(hash, it_in->key, it_in->value);
            }
        }
        auto hash = m_hash(key) % (m_vector->size() * 2);
        if (!resetVector[hash].empty()) {
            bool found = false;
            for (auto& wrapper : resetVector[hash]) {
                if (wrapper.key == key) {
                    wrapper.value = std::move(std::make_shared<T>(value));
                    found = true;
                    break;
                }
            }
            if (!found) {
                resetVector[hash].emplace_back(hash, key, std::move(std::make_shared<T>(value)));
            }
        }
        else {
            resetVector[hash].emplace_back(hash, key, std::move(std::make_shared<T>(value)));
        }
        std::vector<PersistentVector<TWrapper>> out(m_vector->size() * 2);
        for (std::size_t i = 0; i < resetVector.size(); ++i) {
            out[i] = PersistentVector<TWrapper>(resetVector[i].cbegin(), resetVector[i].cend());
        }
        return out;
    }
}