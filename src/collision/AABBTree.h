#ifndef __AABBTREE_H__
#define __AABBTREE_H__

#include "aecs/Entity.h"
#include "math/Rect.h"
#include "math/Ray.h"
#include "SceneGraph.h"

#include <stdint.h>
#include <vector>
#include <stack>
#include <iostream>
#include <utility>

namespace oge
{


class Scene;

struct AABBNode
{
    Rect box = Rect(0, 0, 0, 0);
    aecs::Entity entity = aecs::Entity();
    uint32_t parent = none;
    uint32_t lchild = none;
    uint32_t rchild = none;
    uint32_t nextRemoved = none;

    bool isLeaf()
    {
        return lchild == none;
    }

    static constexpr uint32_t none = UINT32_MAX;
};

class AABBTree : public SceneGraph
{
public:
    AABBTree(Scene* scene);

    void insert(aecs::Entity ent, const Rect& box);
    void update();
    std::vector<aecs::Entity> query(const Rect& box); 
    std::vector<RaycastPair> raycast(const Ray& ray);
    std::vector<RaycastPair> raycastBox(const Ray& ray, const Rect& box);
    void cleanupRemoved() override;

    void draw(const Vector2f& camera) override;

    template<typename T>
    void traverse(T l)
    {
        std::stack<uint32_t> s;
        s.push(root_);
        while(!s.empty())
        {
            uint32_t idx = s.top();
            s.pop();

            if(idx == AABBNode::none) continue;

            l(nodes_[idx]);
            s.push(nodes_[idx].rchild);
            s.push(nodes_[idx].lchild);
        }
    }

    void reserve(size_t n)
    {
        nodes_.reserve(n);
    }

    int getDepth()
    {
        return getDepth(root_);
    }

    int getDepth(uint32_t n)
    {
        if(n == AABBNode::none)
            return 0;

        uint32_t left = nodes_[n].lchild;
        uint32_t right = nodes_[n].rchild;

        return std::max(getDepth(left), getDepth(right)) + 1;
    }
    
    void print()
    {
        print(root_, 0);
    }

    void print(uint32_t node, int depth)
    {
        if(node == AABBNode::none) return;

        for(int i = 0; i < depth; i++)
            std::cout << "|-";
        std::cout << (nodes_[node].isLeaf() ? 'L' : 'I') << node << std::endl;

        this->print(nodes_[node].lchild, depth + 1);
        this->print(nodes_[node].rchild, depth + 1);
    }
private:
    void rotate(uint32_t idx);

    uint32_t addNode(const AABBNode& node);
    uint32_t findBest(uint32_t inserted);
    void remove(uint32_t idx);

    float computeCost(uint32_t inserted, uint32_t sibling);
    float computeResizeCost(uint32_t inserted, uint32_t resized);
    Rect unionAABB(uint32_t a, uint32_t b);

    bool isRemoved(uint32_t idx);

    uint32_t root_;
    uint32_t removed_;

    std::vector<AABBNode> nodes_;
};


} // namespace oge
#endif // __AABBTREE_H__