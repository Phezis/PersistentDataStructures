#pragma once
#include "VersionTree.h"

#include <memory>
#include <stack>


namespace pds {
    /*
    template<typename T>
    class list_const_iterator {
        T* ptr;
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        list_const_iterator();
        list_const_iterator(const list_const_iterator& other);
        list_const_iterator(list_const_iterator&& other);

        list_const_iterator& operator=(const list_const_iterator & other);
        list_const_iterator& operator=(list_const_iterator&& other);

        ~list_const_iterator();

        inline T& operator*() const;
        inline T* operator->() const;

        inline list_const_iterator& operator++();
        inline list_const_iterator& operator--();
        inline list_const_iterator operator++(int) const;
        inline list_const_iterator operator--(int) const;

        inline bool operator==(const list_const_iterator& other) const;
        inline bool operator!=(const list_const_iterator& other) const;
    };

    template<typename T>
    class list_iterator : public list_const_iterator<T> {
        using super = vector_const_iterator<T>;
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        list_iterator();
        list_iterator(const list_iterator& other);
        list_iterator(list_iterator&& other);

        list_iterator& operator=(const list_iterator & other);
        list_iterator& operator=(list_iterator && other);

        ~list_iterator();

        inline T& operator*() const;
        inline T* operator->() const;

        inline list_iterator& operator++();
        inline list_iterator& operator--();
        inline list_iterator operator++(int) const;
        inline list_iterator operator--(int) const;
    };

    template<typename T, typename Allocator = std::allocator<T>>
    class PersistentList {
    public:
        using iterator = list_iterator<T>;
        using const_iterator = list_const_iterator<T>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        PersistentList();
        PersistentList(const PersistentList& other);
        PersistentList(PersistentList&& other);

        PersistentList& operator=(const PersistentList& other);
        PersistentList& operator=(PersistentList&& other);

        ~PersistentList();

        iterator begin();
        iterator end();
        const_iterator cbegin();
        const_iterator cend();
        reverse_iterator rbegin();
        reverse_iterator rend();
        const_reverse_iterator crbegin();
        const_reverse_iterator crend();

        bool operator==(const PersistentList& other) const;
        bool operator!=(const PersistentList& other) const;

        void swap(PersistentList& other);

        std::size_t size() const;
        bool empty() const;

        void undo();
        void redo();

        void clear();

        T& front();
        const T& front() const;

        T& back();
        const T& back() const;

        void push_back(const T& value);
        // template<class... Args>
        // void emplace_back(Args&&... args);
        void pop_back();

        void push_front(const T& value);
        // template<class... Args>
        // void emplace_front(Args&&... args);
        void pop_front();

    private:
        std::shared_ptr<PrimeList_t> primeList;
        version_t version;
        std::stack<version_t> undoStack;
        ListVersionTree* versionTreeNode;
    };
    */

}
