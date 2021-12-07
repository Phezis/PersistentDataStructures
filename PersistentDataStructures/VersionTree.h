
#pragma once
#include <memory>
#include <array>
#include <vector>


namespace pds {
    using version_t = std::uint64_t;
    /*
    class ListVersionTreeNode {
    public:
        ListVersionTreeNode() = delete;
        ListVersionTreeNode(const ListVersionTreeNode& other) = delete;
        ListVersionTreeNode(ListVersionTreeNode&& other) = delete;
        ListVersionTreeNode(const FatNode_t* front, const FatNode_t* back, const ListVersionTreeNode* parent, version_t version) :
            front(front),
            back(back),
            parent(parent),
            version(version),
            children(std::vector()) {}
        ListVersionTreeNode(const FatNode_t* front, const FatNode_t* back, version_t version) : ListVersionTreeNode(front, back, nullptr, version) {}

        ListVersionTreeNode& operator=(const ListVersionTreeNode& other) = delete;
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
        ListVersionTree(const ListVersionTree& other) = delete;
        ListVersionTree(ListVersionTree&& other) = delete;

        ListVersionTree& operator=(const ListVersionTree& other) = delete;
        ListVersionTree& operator=(ListVersionTree&& other) = delete;

        ~ListVersionTree() = default;

        const ListVersionTreeNode& getHead() {
            return *(head.get());
        }

    private:
        std::unique_ptr<ListVersionTreeNode> head;
    };
    
    */

}
