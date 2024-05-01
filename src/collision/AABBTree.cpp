#include "AABBTree.h"

#include "Graphics.h"
#include "components/Collider.h"
#include "components/Physics.h"

#include <queue>
#include <iostream>
#include <float.h>

namespace oge
{


AABBTree::AABBTree(Scene* scene) : 
    SceneGraph(scene), root_(AABBNode::none), removed_(AABBNode::none)
{
    
}

void AABBTree::insert(aecs::Entity ent, const Rect& box)
{
    auto widen = [](const Rect& box, float f) -> Rect
    {
        float px = box.w * f;
        float py = box.h * f;

        return Rect(box.x - px, box.y - py, box.w + px * 2, box.h + py * 2);
    };

    Rect widened = widen(box, 0.2);

    uint32_t added = addNode(AABBNode{widened, ent});
    if(root_ == AABBNode::none)
    {
        root_ = added;
        return;
    }

    uint32_t sibling = findBest(added);
    uint32_t branch = addNode(AABBNode 
    {
        .box = unionAABB(added, sibling),
        .entity = Entity(),
        .parent = nodes_[sibling].parent,
        .lchild = sibling,
        .rchild = added
    });

    uint32_t oldParent = nodes_[sibling].parent;
    nodes_[sibling].parent = branch;
    nodes_[added  ].parent = branch;
    if(oldParent == AABBNode::none)
    {
        root_ = branch;
    }
    else
    {
        AABBNode& pnode = nodes_[oldParent];
        if(pnode.lchild == sibling)
            pnode.lchild = branch;
        else
            pnode.rchild = branch;    
    }

    // Refit the boxes after inserting a new node
    uint32_t next = branch;
    while(next != AABBNode::none)
    {
        AABBNode& node = nodes_[next];
        node.box = unionAABB(node.lchild, node.rchild);

        uint32_t temp = next;
        next = node.parent;
        rotate(temp);
    }
}

void AABBTree::rotate(uint32_t idx)
{
    AABBNode& leafNode = nodes_[idx];

    // We're considering rotating current node with grandparent's
    // other child so our node needs to have both a parent and a grandparent
    uint32_t parent = leafNode.parent;
    if(parent == AABBNode::none) 
        return;

    uint32_t grandparent = nodes_[parent].parent;
    if(grandparent == AABBNode::none) 
        return;

    AABBNode& parentNode  = nodes_[parent];
    AABBNode& gparentNode = nodes_[grandparent];

    uint32_t otherChild = parentNode.lchild == idx ?
        parentNode.rchild :
        parentNode.lchild;

    uint32_t candidate = gparentNode.lchild == parent ?
        gparentNode.rchild :
        gparentNode.lchild;

    AABBNode& candidateNode = nodes_[candidate];
    AABBNode& otherChildNode = nodes_[otherChild];

    // Calculate current surface and surface after a swap
    float currentSurface = parentNode.box.area();
    float newSurface = otherChildNode.box.area() + candidateNode.box.area();

    // If the swap results in a lower surface area
    if(newSurface < currentSurface)
    {
        candidateNode.parent = parent;
        if(parentNode.lchild == idx)
        {
            parentNode.lchild = candidate;
        }
        else
        {
            parentNode.rchild = candidate;
        }

        leafNode.parent = grandparent;
        if(gparentNode.lchild == parent)
        {
            gparentNode.rchild = idx;
        }
        else
        {
            gparentNode.lchild = idx;
        }
    }
}

void AABBTree::remove(uint32_t idx)
{
    OGE_ASSERT(nodes_[idx].isLeaf(), "The deleted node must be a leaf!");

    nodes_[idx].nextRemoved = removed_;
    removed_ = idx;

    // If the removed leaf node was the root, it was the 
    // only node in the tree and we can prune it safely
    if(idx == root_)
    {
        root_ = AABBNode::none;
        return;
    }

    // We also need to remove the deleted node's parent because it's an internal node
    uint32_t parent = nodes_[idx].parent;
    uint32_t grandparent = nodes_[parent].parent;

    uint32_t otherChild = (nodes_[parent].lchild == idx) ? 
            nodes_[parent].rchild : 
            nodes_[parent].lchild;

    nodes_[parent].nextRemoved = removed_;
    removed_ = parent;

    nodes_[otherChild].parent = grandparent;
    if(grandparent == AABBNode::none)
    {
        root_ = otherChild;
        return;
    }

    if(nodes_[grandparent].lchild == parent)
    {
        nodes_[grandparent].lchild = otherChild;
    }
    else
    {
        nodes_[grandparent].rchild = otherChild;
    }
}

std::vector<aecs::Entity> AABBTree::query(const Rect& box)
{
    std::vector<aecs::Entity> collisions;
    collisions.reserve(8);
    
    std::stack<uint32_t> s;
    s.push(root_);
    while(!s.empty())
    {
        uint32_t idx = s.top();
        s.pop();

        if(idx == AABBNode::none)
            continue;

        if(nodes_[idx].isLeaf())
        {
            if(getHitbox(*scene_, nodes_[idx].entity).intersects(box))
                collisions.push_back(nodes_[idx].entity);
        }
        else if(box.intersects(nodes_[idx].box))
        {
            s.push(nodes_[idx].lchild);
            s.push(nodes_[idx].rchild);
        }
    }

    collisions.shrink_to_fit();
    return collisions;
}

std::vector<SceneGraph::RaycastPair> AABBTree::raycast(const Ray& ray)
{
    std::vector<RaycastPair> collisions;
    collisions.reserve(8);

    std::stack<uint32_t> s;
    s.push(root_);
    while(!s.empty())
    {
        uint32_t idx = s.top();
        s.pop();

        if(idx == AABBNode::none)
            continue;

        if(nodes_[idx].isLeaf())
        {
            Rect hitbox = getHitbox(*scene_, nodes_[idx].entity);
            RaycastResult result = ray.intersects(hitbox);
            if(result)
            {
                RaycastPair pair(nodes_[idx].entity, result);
                collisions.push_back(pair);
            }
        }
        else if(ray.intersects(nodes_[idx].box))
        {
            s.push(nodes_[idx].lchild);
            s.push(nodes_[idx].rchild);
        }
    }

    collisions.shrink_to_fit();
    return collisions;
}

std::vector<SceneGraph::RaycastPair> AABBTree::raycastBox(const Ray& ray, const Rect& box)
{
    std::vector<RaycastPair> collisions;
    collisions.reserve(8);

    std::stack<uint32_t> s;
    s.push(root_);
    while(!s.empty())
    {
        uint32_t idx = s.top();
        s.pop();

        if(idx == AABBNode::none)
            continue;

        if(nodes_[idx].isLeaf())
        {
            Rect hitbox = getHitbox(*scene_, nodes_[idx].entity);
            RaycastResult result = ray.sweepRect(box, hitbox);
            if(result)
            {
                RaycastPair pair(nodes_[idx].entity, result);
                collisions.push_back(pair);
            }
        }
        else if(ray.sweepRect(box, nodes_[idx].box))
        {
            s.push(nodes_[idx].lchild);
            s.push(nodes_[idx].rchild);
        }
    }

    collisions.shrink_to_fit();
    return collisions;
}

uint32_t AABBTree::addNode(const AABBNode& node)
{
    uint32_t idx = nodes_.size();
    if(removed_ != AABBNode::none)
    {
        idx = removed_;
        removed_ = nodes_[removed_].nextRemoved;
        nodes_[idx] = node;
    }
    else
    {
        nodes_.push_back(node);
    }
    return idx;
}

uint32_t AABBTree::findBest(uint32_t inserted)
{
    AABBNode& insertedNode = nodes_[inserted];

    uint32_t idx = root_;
    while(!nodes_[idx].isLeaf())
    {
        AABBNode& currentNode = nodes_[idx];
        AABBNode& leftNode = nodes_[currentNode.lchild];
        AABBNode& rightNode = nodes_[currentNode.rchild];

        Rect combined = currentNode.box.unionRect(insertedNode.box);

        float newParentCost = 2.0f * combined.area();
        float pushCost = 2.0f * (combined.area() - currentNode.box.area());

        float lcost, rcost;
        if(leftNode.isLeaf())
        {
            lcost = insertedNode.box.unionRect(leftNode.box).area() + pushCost;
        }
        else
        {
            Rect newLeftBox = insertedNode.box.unionRect(leftNode.box);
            lcost = (newLeftBox.area() - leftNode.box.area()) + pushCost;
        }

        if(rightNode.isLeaf())
        {
            rcost = insertedNode.box.unionRect(rightNode.box).area() + pushCost;
        }
        else
        {
            Rect newRightBox = insertedNode.box.unionRect(rightNode.box);
            rcost = (newRightBox.area() - rightNode.box.area()) + pushCost;
        }

        if(newParentCost < lcost && newParentCost < rcost)
        {
            break;
        }

        if(lcost < rcost)
            idx = currentNode.lchild;
        else
            idx = currentNode.rchild;
    }
    return idx;
}

void AABBTree::update()
{
    for(uint32_t i = 0; i < nodes_.size(); i++)
    {
        if(isRemoved(i) || !nodes_[i].isLeaf()) continue;

        Rect hitbox = getHitbox(*scene_, nodes_[i].entity);
        if(!nodes_[i].box.contains(hitbox))
        {
            this->remove(i);
            this->insert(nodes_[i].entity, hitbox);
        }
    }
}

void AABBTree::cleanupRemoved()
{
    for(uint32_t i = 0; i< nodes_.size(); i++)
    {
        if(isRemoved(i) || !nodes_[i].isLeaf()) continue;

        Collider* col = scene_->try_get<Collider>(nodes_[i].entity);
        Transform* tran = scene_->try_get<Transform>(nodes_[i].entity);

        if(col == nullptr || tran == nullptr)
        {
            this->remove(i);
        }
    }
}

void AABBTree::draw(const Vector2f& camera)
{
    Graphics& graphics = Graphics::instance();

    this->traverse([&](auto& elem)
    {
        Rect draw = scene_->worldToScreen(elem.box);

        if(elem.isLeaf()) 
        {
            graphics.drawRect(draw, Color::green);
            Rect actual = getHitbox(*scene_, elem.entity);
            graphics.drawRect(scene_->worldToScreen(actual), Color::red);
        }
        else graphics.drawRect(draw.widen(10), Color::blue);
    });
}

bool AABBTree::isRemoved(uint32_t idx)
{
    /// TODO: Optimize this
    uint32_t next = removed_;
    while(next != AABBNode::none)
    {
        if(idx == next)
            return true;
        
        next = nodes_[next].nextRemoved;
    }
    return false;
}

float AABBTree::computeCost(uint32_t inserted, uint32_t sibling)
{
    float cost = unionAABB(inserted, sibling).area();
    
    uint32_t next = nodes_[sibling].parent;
    while(next != AABBNode::none)
    {
        cost += computeResizeCost(inserted, next);
        next = nodes_[next].parent;
    }

    return cost;
}

float AABBTree::computeResizeCost(uint32_t inserted, uint32_t resized)
{
    return unionAABB(inserted, resized).area() - nodes_[resized].box.area();;
}

Rect AABBTree::unionAABB(uint32_t a, uint32_t b)
{
    return nodes_[a].box.unionRect(nodes_[b].box);
}


} // namespace oge