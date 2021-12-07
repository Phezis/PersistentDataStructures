﻿#pragma once
#include "VersionTree.h"
#include "Utils.h"

#include <memory>
#include <vector>
#include <stack>
#include <array>
#include <stdexcept>

namespace pds {
    /*
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

        vector_const_iterator& operator=(const vector_const_iterator& other);
        vector_const_iterator& operator=(vector_const_iterator&& other);

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
        using super = vector_const_iterator<T>;
    public:
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        vector_iterator();
        vector_iterator(const vector_iterator& other);
        vector_iterator(vector_iterator&& other);

        vector_iterator& operator=(const vector_iterator & other);
        vector_iterator& operator=(vector_iterator && other);

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
    */

    constexpr std::uint32_t m_primeTreeNodeSize = 5;

	template<typename T>
	class PersistentVector {
        template<std::uint32_t degreeOfTwo>
        class PrimeTreeNode;

        template<std::uint32_t degreeOfTwo>
        class PrimeTreeRoot;

        class VectorVersionTreeNode;

        class VectorVersionTree;

        class PrimeVectorTree;

	public:
        /*
        using iterator = vector_iterator<T>;
        using const_iterator = vector_const_iterator<T>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        */


        PersistentVector() :
            m_primeVectorTree(std::make_shared<PrimeVectorTree>()),
            m_version(0),
            m_undoStack(std::stack<version_t>()),
            m_versionTreeNode(m_primeVectorTree->getInitialVersion()) {}
        PersistentVector(const PersistentVector& other) = default;
        PersistentVector(PersistentVector&& other) noexcept = default;

        ~PersistentVector() = default;

        PersistentVector& operator=(const PersistentVector& other) = default;
        PersistentVector& operator=(PersistentVector&& other) = default;

        /*
        iterator begin();
        iterator end();
        const_iterator cbegin();
        const_iterator cend();
        reverse_iterator rbegin();
        reverse_iterator rend();
        const_reverse_iterator crbegin();
        const_reverse_iterator crend();
        */

        const T& operator[](std::size_t pos) const;

        const T& at(std::size_t pos) const;

        PersistentVector set(std::size_t pos, const T& value);

        /*
		bool operator==(const PersistentVector& other) const;
		bool operator!=(const PersistentVector& other) const;

		void swap(PersistentVector& other);
        */

		std::size_t size() const;
		bool empty() const;

        /*
        PersistentVector undo();
        PersistentVector redo();

        PersistentVector clear();
        */

        T& front();
        const T& front() const;

        T& back();
        const T& back() const;

        PersistentVector push_back(const T& value);

        /*
        template<class... Args>
        PersistentVector emplace_back(Args&&... args) const;
        PersistentVector pop_back() const;
        PersistentVector resize(std::size_t count) const;
        */

    private:
        PersistentVector(std::shared_ptr<PrimeVectorTree> primeVectorTree, version_t version, VectorVersionTreeNode* m_versionTreeNode) :
            m_primeVectorTree(primeVectorTree),
            m_version(version),
            m_undoStack(std::stack<version_t>()),
            m_versionTreeNode(m_versionTreeNode) {}


        using NodeCreationStatus = bool;
        static constexpr NodeCreationStatus NODE_DUPLICATE = true;
        static constexpr NodeCreationStatus NEW_NODE = false;


        /*
        *
        *   PrimeTreeNode - узел первичного дерева, которое эмулирует вектор;
        *       может быть как обычным узлом, у которого дети - узлы дерева,
        *       а также листом, у которого дети - элементы вектора
        * 
        */
        template<std::uint32_t degreeOfTwo>
        class PrimeTreeNode {
        public:
            PrimeTreeNode() = delete;
            PrimeTreeNode(std::shared_ptr<T>& insertingElement);
            PrimeTreeNode(std::shared_ptr<PrimeTreeNode<degreeOfTwo>>& child);
            PrimeTreeNode(std::shared_ptr<PrimeTreeNode<degreeOfTwo>>& oldChild, std::shared_ptr<PrimeTreeNode<degreeOfTwo>>& newChild);
            PrimeTreeNode(const PrimeTreeNode& other) = default;
            PrimeTreeNode(PrimeTreeNode&& other) = delete;

            PrimeTreeNode& operator=(const PrimeTreeNode& other) = delete;
            PrimeTreeNode& operator=(PrimeTreeNode&& other) = delete;

            ~PrimeTreeNode() = default;

            T& get(std::size_t pos, std::uint32_t level);

            NodeCreationStatus emplace_back(std::shared_ptr<T>&& value, std::shared_ptr<PrimeTreeNode>& primeTreeNode);

        private:
            static constexpr std::size_t ARRAY_SIZE = Utils::binPow(degreeOfTwo);
            using NodeType = bool;
            static constexpr NodeType NODE = true;
            static constexpr NodeType LEAF = false;

            union PrimeTreeNodeContent {
                std::array<std::shared_ptr<PrimeTreeNode>, ARRAY_SIZE> m_children;
                std::array<std::shared_ptr<T>, ARRAY_SIZE> m_elements;
            };

            NodeType m_type;
            PrimeTreeNodeContent m_content;
        };


        /*
        *
        *   PrimeTreeRoot - корень первичного дерева, которое эмулирует вектор;
        *       хранит указатель на узел дерева (который может быть листом),
        *       а также размер вектора; один корень соответствует одной версии вектора.
        *
        */
        template<std::uint32_t degreeOfTwo>
        class PrimeTreeRoot {
        public:
            PrimeTreeRoot() : m_child(nullptr), m_size(0), m_depth(0) {}
            PrimeTreeRoot(std::shared_ptr<PrimeTreeNode<degreeOfTwo>> child, std::size_t size) : m_child(child), m_size(size) {
                m_depth = 0;
                while (size) {
                    ++m_depth;
                    size >>= degreeOfTwo;
                }
            }
            PrimeTreeRoot(const PrimeTreeRoot& other) = delete;
            PrimeTreeRoot(PrimeTreeRoot&& other) = delete;

            PrimeTreeRoot& operator=(const PrimeTreeRoot& other) = delete;
            PrimeTreeRoot& operator=(PrimeTreeRoot&& other) = delete;

            ~PrimeTreeRoot() = default;

            T& operator[](std::size_t pos);
            const T& operator[](std::size_t pos) const;

            std::shared_ptr<PrimeTreeRoot> emplace_back(std::shared_ptr<T>&& value);

            std::size_t size() const;

        private:
            std::shared_ptr<PrimeTreeNode<degreeOfTwo>> m_child;
            std::size_t m_size;
            std::uint32_t m_depth;
        };


        /*
        *
        *   VectorVersionTreeNode - узел версионного дерева;
        *       хранит указатель на корень первичного дерева, соответствующего текущей версии, а также номер самой версии;
        *       хранит своего родителя для возможности откатиться к предыдущей версии;
        *       хранит всех своих детей - версии, созданные из текущей (сборка мусора);
        *
        */
        class VectorVersionTreeNode {
        public:
            VectorVersionTreeNode() = delete;
            VectorVersionTreeNode(const VectorVersionTreeNode& other) = delete;
            VectorVersionTreeNode(VectorVersionTreeNode&& other) = default;
            VectorVersionTreeNode(std::unique_ptr<PrimeTreeRoot<m_primeTreeNodeSize>> root, const VectorVersionTreeNode* parent, version_t version) :
                m_root(std::move(root)),
                m_parent(parent),
                m_version(version),
                m_children(std::vector<VectorVersionTreeNode>()) {}
            VectorVersionTreeNode(std::unique_ptr<PrimeTreeRoot<m_primeTreeNodeSize>> root, version_t version) : VectorVersionTreeNode(std::move(root), nullptr, version) {}

            VectorVersionTreeNode& operator=(const VectorVersionTreeNode& other) = delete;
            VectorVersionTreeNode& operator=(VectorVersionTreeNode&& other) = delete;

            ~VectorVersionTreeNode() = default;

            const PrimeTreeRoot<m_primeTreeNodeSize>& getRoot() const {
                return *m_root;
            }

            const VectorVersionTreeNode& getParent() const {
                return *m_parent;
            }

            version_t getVersion() const {
                return m_version;
            }

            VectorVersionTreeNode& addChild(std::shared_ptr<PrimeTreeRoot<m_primeTreeNodeSize>> root, version_t childVersion) {
                m_children.emplace_back(root, this, childVersion);
                return m_children.back();
            }

        private:
            std::unique_ptr<PrimeTreeRoot<m_primeTreeNodeSize>> m_root;
            const VectorVersionTreeNode* m_parent;
            const version_t m_version;
            std::vector<VectorVersionTreeNode> m_children;
        };


        /*
        *
        *   VectorVersionTree - версионное дерево (сборка мусора);
        *       хранит корень версионного дерева
        *
        */
        class VectorVersionTree {
        public:
            VectorVersionTree() = delete;
            VectorVersionTree(std::unique_ptr<PrimeTreeRoot<m_primeTreeNodeSize>> root) {
                m_head = std::make_unique<VectorVersionTreeNode>(std::move(root), nullptr, 0);
            }
            VectorVersionTree(const VectorVersionTree& other) = delete;
            VectorVersionTree(VectorVersionTree&& other) = default;

            VectorVersionTree& operator=(const VectorVersionTree& other) = delete;
            VectorVersionTree& operator=(VectorVersionTree&& other) = delete;

            ~VectorVersionTree() = default;

            VectorVersionTreeNode* getHead() {
                return m_head.get();
            }

        private:
            std::unique_ptr<VectorVersionTreeNode> m_head;
        };

        /*
        *
        *   PrimeVectorTree - первичное дерево (сборка мусора);
        *       хранит версионное дерево, из которого можно получить доступ к любой версии вектора;
        *       хранит номер последней версии;
        *
        */
        class PrimeVectorTree {
        public:
            PrimeVectorTree() : 
                m_versionTree(VectorVersionTree(std::make_unique<PrimeTreeRoot<m_primeTreeNodeSize>>())), 
                m_lastVersion(0) {}
            PrimeVectorTree(const PrimeVectorTree& other) = delete;
            PrimeVectorTree(PrimeVectorTree&& other) = default;

            PrimeVectorTree& operator=(const PrimeVectorTree&& other) = delete;
            PrimeVectorTree& operator=(PrimeVectorTree&& other) = delete;

            ~PrimeVectorTree() = default;

            VectorVersionTreeNode* getInitialVersion() { return m_versionTree.getHead(); }

            version_t getAndIncreaseLastVersion() { return m_lastVersion++; }

        private:
            VectorVersionTree m_versionTree;
            version_t m_lastVersion;
        };

        std::shared_ptr<PrimeVectorTree> m_primeVectorTree;
        version_t m_version;
        std::stack<version_t> m_undoStack;
        VectorVersionTreeNode* m_versionTreeNode;
	};

    /*
    *
    *   Implementation
    * 
    */

    template<typename T>
    inline std::size_t PersistentVector<T>::size() const {
        return m_versionTreeNode->getRoot().size();
    }

    template<typename T>
    inline bool PersistentVector<T>::empty() const {
        return 0 == size();
    }

    template<typename T>
    inline const T& PersistentVector<T>::operator[](std::size_t pos) const {
        return m_versionTreeNode->getRoot()[pos];
    }


    template<typename T>
    inline const T& PersistentVector<T>::at(std::size_t pos) const {
        if (pos >= size()) {
            throw std::out_of_range();
        }
        return (*this)[pos];
    }

    template<typename T>
    inline PersistentVector<T> PersistentVector<T>::set(std::size_t pos, const T& value) {
        
    }

    template<typename T>
    inline T& PersistentVector<T>::front() {
        return (*this)[0];
    }

    template<typename T>
    inline const T& PersistentVector<T>::front() const {
        return (*this)[0];
    }

    template<typename T>
    inline T& PersistentVector<T>::back() {
        return (*this)[size() - 1];
    }

    template<typename T>
    inline const T& PersistentVector<T>::back() const {
        return (*this)[size() - 1];
    }

    template<typename T>
    inline PersistentVector<T> PersistentVector<T>::push_back(const T& value) {
        auto newRoot = m_versionTreeNode->getRoot().emplace_back(std::move(std::make_shared(value)));
        // TODO: check if last version has changed delete newRoot and try again
        auto& newVersionTreeNode = m_versionTreeNode->addChild(newRoot, m_primeVectorTree->getAndIncreaseLastVersion());
        return newVector(m_primeVectorTree, newVersionTreeNode->getVersion(), newVersionTreeNode);
    }

    template<typename T>
    template<std::uint32_t degreeOfTwo>
    inline T& PersistentVector<T>::PrimeTreeRoot<degreeOfTwo>::operator[](std::size_t pos) {
        return m_child->get(pos, m_depth - 1);
    }

    template<typename T>
    template<std::uint32_t degreeOfTwo>
    inline const T& PersistentVector<T>::PrimeTreeRoot<degreeOfTwo>::operator[](std::size_t pos) const {
        return (*this)[pos];
    }

    template<typename T>
    template<std::uint32_t degreeOfTwo>
    inline std::shared_ptr<typename PersistentVector<T>::PrimeTreeRoot<degreeOfTwo>> 
        PersistentVector<T>::PrimeTreeRoot<degreeOfTwo>::emplace_back(std::shared_ptr<T>&& value)
    {
        std::shared_ptr<PrimeTreeNode<degreeOfTwo>> child;
        std::shared_ptr<PrimeTreeNode<degreeOfTwo>> childOfNewRoot;
        auto childCreationStatus = m_child->emplace_back(value, child);
        if (childCreationStatus == NEW_NODE) {
            childOfNewRoot = std::make_shared<PrimeTreeNode<degreeOfTwo>>(m_child, child);
        }
        // otherwise childCreationStatus == NODE_DUPLICATE
        else {
            childOfNewRoot = child;
        }
        return std::make_shared<PrimeTreeRoot<degreeOfTwo>>(childOfNewRoot, m_size + 1);
    }

    template<typename T>
    template<std::uint32_t degreeOfTwo>
    inline std::size_t PersistentVector<T>::PrimeTreeRoot<degreeOfTwo>::size() const {
        return m_size;
    }


    template<typename T>
    template<std::uint32_t degreeOfTwo>
    inline T& PersistentVector<T>::PrimeTreeNode<degreeOfTwo>::get(std::size_t pos, std::uint32_t level) {
        auto id = Utils::getId(pos, level, degreeOfTwo);
        auto mask = Utils::getMask(pos, level, degreeOfTwo);
        if (m_type == NODE) {
            return m_content.m_children[id]->get(pos & mask, level - 1);
        }
        // otherwise m_type == LEAF
        else {
            return *(m_content.m_elements[id]);
        }
    }

    template<typename T>
    template<std::uint32_t degreeOfTwo>
    inline typename PersistentVector<T>::NodeCreationStatus PersistentVector<T>::PrimeTreeNode<degreeOfTwo>::emplace_back(
            std::shared_ptr<T>&& value, 
            std::shared_ptr<PrimeTreeNode>& primeTreeNode) {
        PersistentVector<T>::NodeCreationStatus out;
        if (m_type == LEAF) {
            if (m_content.m_elements.size() < Utils::binPow(degreeOfTwo)) {
                primeTreeNode = std::make_shared(*this);
                primeTreeNode->m_content.m_elements[duplicate.m_content.m_elements.size()] = value;
                out = NODE_DUPLICATE;
            }
            else {
                primeTreeNode = std::make_shared(value);
                out = NEW_NODE;
            }
        }
        else {
            std::shared_ptr<PrimeTreeNode> child;
            auto childCreationStatus = m_content.m_children.back().emplace_back(value, child);
            if (childCreationStatus == NODE_DUPLICATE) {
                primeTreeNode = std::make_shared(*this);
                primeTreeNode->m_content.m_children.back() = child;
                out = NODE_DUPLICATE;
            }
            else {
                if (m_content.m_children.size() < Utils::binPow(degreeOfTwo)) {
                    primeTreeNode = std::make_shared(*this);
                    primeTreeNode->m_content.m_elements[duplicate.m_content.m_elements.size()] = value;
                    out = NODE_DUPLICATE;
                }
                else {
                    primeTreeNode = std::make_shared(child);
                    out = NEW_NODE;
                }
            }
        }
        return out;
    }

    template<typename T>
    template<std::uint32_t degreeOfTwo>
    inline PersistentVector<T>::PrimeTreeNode<degreeOfTwo>::PrimeTreeNode(std::shared_ptr<T>& insertingElement) : m_type(LEAF) {
        m_content.m_elements[0] = insertingElement;
    }

    template<typename T>
    template<std::uint32_t degreeOfTwo>
    inline PersistentVector<T>::PrimeTreeNode<degreeOfTwo>::PrimeTreeNode(std::shared_ptr<PrimeTreeNode<degreeOfTwo>>& child) : m_type(NODE) {
        m_content.m_children[0] = child;
    }

    template<typename T>
    template<std::uint32_t degreeOfTwo>
    inline PersistentVector<T>::PrimeTreeNode<degreeOfTwo>::PrimeTreeNode(std::shared_ptr<PrimeTreeNode<degreeOfTwo>>& oldChild, std::shared_ptr<PrimeTreeNode<degreeOfTwo>>& newChild) :
        m_type(NODE) {
        m_content.m_children[0] = oldChild;
        m_content.m_children[1] = newChild;

    }

}
