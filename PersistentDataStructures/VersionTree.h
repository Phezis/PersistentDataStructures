
#pragma once
#include <memory>
#include <array>
#include <vector>


namespace PersistentContainer {
    using version_t = std::uint64_t;

    class ListVersionTreeNode {
    public:
        ListVersionTreeNode() = delete;
        ListVersionTreeNode(cosnt ListVersionTreeNode& other) = delete;
        ListVersionTreeNode(ListVersionTreeNode&& other) = delete;
        ListVersionTreeNode(const FatNode_t* front, const FatNode_t* back, const ListVersionTreeNode* parent, version_t version) :
            front(front),
            back(back),
            parent(parent),
            version(version),
            children(std::vector()) {}
        ListVersionTreeNode(const FatNode_t* front, const FatNode_t* back, version_t version) : ListVersionTreeNode(front, back, nullptr, version) {}

        ListVersionTreeNode& operator=(cosnt ListVersionTreeNode& other) = delete;
        ListVersionTreeNode& operator=(ListVersionTreeNode&& other) = delete;

        ~ListVersionTreeNode() = default;

        const FatNode_t& getFront() {
            return *front;
        }

        const FatNode_t& getBack() {
            return *back;
        }

        const ListVersionTreeNode& getParent() {
            return *parent;
        }

        version_t getVersion() {
            return version;
        }

        ListVersionTreeNode& addChild(const FatNode_t* front, const FatNode_t* back, version_t childVersion) {
            children.emplace_back(front, back, this, childVersion);
            return children.back();
        }

    private:
        const FatNode_t* front;
        const FatNode_t* back;
        const ListVersionTreeNode* parent;
        const version_t version;
        std::vector<ListVersionTreeNode> children;
    };


    class ListVersionTree {
    public:
        ListVersionTree() {
            head = ListVersionTreeNode(nullptr, nullptr, 0);
        }
        ListVersionTree(cosnt ListVersionTree& other) = delete;
        ListVersionTree(ListVersionTree&& other) = delete;

        ListVersionTree& operator=(cosnt ListVersionTree& other) = delete;
        ListVersionTree& operator=(ListVersionTree&& other) = delete;

        ~ListVersionTree() = default;

        const ListVersionTreeNode& getHead() {
            return *(head.get());
        }

    private:
        std::unique_ptr<ListVersionTreeNode> head;
    };





    class VectorVersionTreeNode {
    public:
        VectorVersionTreeNode() = delete;
        VectorVersionTreeNode(cosnt VectorVersionTreeNode& other) = delete;
        VectorVersionTreeNode(VectorVersionTreeNode&& other) = delete;
        VectorVersionTreeNode(const ContainerNode_t* root, const VectorVersionTreeNode* parent, version_t version) :
            root(root),
            parent(parent),
            version(version),
            children(std::vector()) {}
        VectorVersionTreeNode(const ContainerNode_t* root, version_t version) : VectorVersionTreeNode(root, nullptr, version) {}

        VectorVersionTreeNode& operator=(cosnt VectorVersionTreeNode& other) = delete;
        VectorVersionTreeNode& operator=(VectorVersionTreeNode&& other) = delete;

        ~VectorVersionTreeNode() = default;

        const Root_t& getRoot() {
            return *root;
        }

        const VectorVersionTreeNode& getParent() {
            return *parent;
        }

        version_t getVersion() {
            return version;
        }

        VectorVersionTreeNode& addChild(const Root_t* root, version_t childVersion) {
            children.emplace_back(root, this, childVersion);
            return children.back();
        }

    private:
        const Root_t* root;
        const VectorVersionTreeNode* parent;
        const version_t version;
        std::vector<VectorVersionTreeNode> children;
    };


    class VectorVersionTree {
    public:
        VectorVersionTree() {
            head = VectorVersionTreeNode(nullptr, nullptr, 0);
        }
        VectorVersionTree(cosnt VectorVersionTree& other) = delete;
        VectorVersionTree(VectorVersionTree&& other) = delete;

        VectorVersionTree& operator=(cosnt VectorVersionTree& other) = delete;
        VectorVersionTree& operator=(VectorVersionTree&& other) = delete;

        ~VectorVersionTree() = default;

        const VectorVersionTreeNode& getHead() {
            return *(head.get());
        }

    private:
        std::unique_ptr<VectorVersionTreeNode> head;
    };

}
