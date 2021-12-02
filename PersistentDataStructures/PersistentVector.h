#pragma once
#include "VersionTree.h"

#include <memory>
#include <vector>

namespace PersistentContainer {
    template<typename T>
    class vector_const_iterator {
        std::size_t id;
    public:
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        vector_const_iterator();
        vector_const_iterator(const vector_const_iterator& other);
        vector_const_iterator(vector_const_iterator&& other);

        vector_const_iterator& opeartor=(const vector_const_iterator & other);
        vector_const_iterator& opeartor=(vector_const_iterator && other);

        ~vector_const_iterator();

        inline vector_const_iterator& operator+=(const difference_type other);
        inline vector_const_iterator& operator-=(const difference_type other);
        inline T& operator*() const;
        inline T* operator->() const;
        inline T& operator[](const difference_type other) const;

        inline vector_const_iterator& operator++();
        inline vector_const_iterator& operator--();
        inline vector_const_iterator operator++(int) const;
        inline vector_const_iterator operator--(int) const;

        inline difference_type operator-(const vector_const_iterator& other) const;
        inline vector_const_iterator operator+(const difference_type other) const;
        inline vector_const_iterator operator-(const difference_type other) const;
        friend inline vector_const_iterator operator+(const difference_type lhs, const vector_const_iterator& rhs);
        friend inline vector_const_iterator operator-(const difference_type lhs, const vector_const_iterator& rhs);

        inline bool operator==(const vector_const_iterator& other) const;
        inline bool operator!=(const vector_const_iterator& other) const;
        inline bool operator>(const vector_const_iterator& other) const;
        inline bool operator<(const vector_const_iterator& other) const;
        inline bool operator>=(const vector_const_iterator& other) const;
        inline bool operator<=(const vector_const_iterator& other) const;
    };

    template<typename T>
    class vector_iterator : public vector_const_iterator<T> {
        using super = vector_vector_const_iterator<T>;
    public:
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        vector_iterator();
        vector_iterator(const vector_iterator& other);
        vector_iterator(vector_iterator&& other);

        vector_iterator& opeartor=(const vector_iterator & other);
        vector_iterator& opeartor=(vector_iterator && other);

        ~vector_iterator();

        inline vector_iterator& operator+=(const difference_type other);
        inline vector_iterator& operator-=(const difference_type other);
        inline T& operator*() const;
        inline T* operator->() const;
        inline T& operator[](const difference_type other) const;

        inline vector_iterator& operator++();
        inline vector_iterator& operator--();
        inline vector_iterator operator++(int) const;
        inline vector_iterator operator--(int) const;

        inline difference_type operator-(const vector_iterator& other) const;
        inline vector_iterator operator+(const difference_type other) const;
        inline vector_iterator operator-(const difference_type other) const;
        friend inline vector_iterator operator+(const difference_type lhs, const vector_iterator& rhs);
        friend inline vector_iterator operator-(const difference_type lhs, const vector_iterator& rhs);
    };

	template<typename T, typename Allocator = std::allocator<T>>
	class PersistentVector {
	public:
        using iterator = vector_iterator<T>;
        using const_iterator = vector_const_iterator<T>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        

		PersistentVector();
		PersistentVector(const PersistentVector& other);
        PersistentVector(PersistentVector&& other);

		PersistentVector& operator=(const PersistentVector& other);
		PersistentVector& operator=(PersistentVector&& other);

        T& operator[](std::size_t pos);
        const T& operator[](std::size_t pos) const;

		~PersistentVector();

        iterator begin();
        iterator end();
        const_iterator cbegin();
        const_iterator cend();
        reverse_iterator rbegin();
        reverse_iterator rend();
        const_reverse_iterator crbegin();
        const_reverse_iterator crend();

		bool operator==(const PersistentVector& other) const;
		bool operator!=(const PersistentVector& other) const;

		void swap(PersistentVector& other);

		std::size_t size() const;
		bool empty() const;

        void undo();
        void redo();

        void clear();

        T& front();
        const T& front() const;

        T& back();
        const T& back() const;

        void push_back(cosnt T& value);
        template<class... Args>
        void emplace_back(Args&&... args);
        void pop_back();
        void resize(std::size_t count);

        T& at(std::size_t pos);
        const T& at(std::size_t pos) const;

    private:
        std::shared_ptr<PrimeVectorTree_t> primeVectorTree;
        version_t version;
        std::stack<version_t> undoStack;
        VectorVersionTree* versionTreeNode;
	};

}
