// tree.h                                                       -*-C++-*-
#ifndef INCLUDED_TREE
#define INCLUDED_TREE

#include <memory>
#include <variant>
#include <vector>

namespace tree {

template <typename Data>
class Node {
  private:
    Data                  d_;
    int                   height_;
    std::shared_ptr<Node> left_;
    std::shared_ptr<Node> right_;

  public:
    Node()            = default;
    Node(Node const&) = default;
    Node(Node&&)      = default;
    Node(Data                  d,
         std::shared_ptr<Node> left  = nullptr,
         std::shared_ptr<Node> right = nullptr)
        : d_(d), left_(left), right_(right) {
        recalcHeight();
    }

    Node& operator=(Node const&) = default;
    Node& operator=(Node&&)      = default;

    std::shared_ptr<Node>& left() { return left_; }
    std::shared_ptr<Node>& right() { return right_; }
    std::shared_ptr<Node>  setLeft(std::shared_ptr<Node> l) {
        return left_ = l;
    }
    std::shared_ptr<Node> setRight(std::shared_ptr<Node> r) {
        return right_ = r;
    }
    Data data() { return d_; };
    void setData(Data d) { d_ = d; };
    int  height() { return height_; }
    int  recalcHeight() {
        int leftHeight  = left_ ? left_->height() : 0;
        int rightHeight = right_ ? right_->height() : 0;
        height_         = 1 + std::max(leftHeight, rightHeight);
        return height_;
    }
};

template <typename Data>
using NodePtr = std::shared_ptr<Node<Data>>;

template <typename Data>
int tree_height(NodePtr<Data> tree) {
    if (tree == nullptr) {
        return 0;
    }
    return tree->height();
}

template <typename Data>
class Tree {
  public:
    using NodePtr = std::shared_ptr<Node<Data>>;

  private:
    NodePtr root_;

  public:
    Tree()            = default;
    Tree(Tree const&) = default;
    Tree(Tree&&)      = default;

    Tree(NodePtr root) : root_(root){};

    Tree& operator=(Tree const&) = default;
    Tree& operator=(Tree&&)      = default;

    int height() { return tree_height(root_); }

    static void singleRotateLeft(NodePtr& t) {
        NodePtr r = t->right();
        t->setRight(r->left());
        t->recalcHeight();

        r->setLeft(t);
        t = r;
        t->recalcHeight();
    }

    static void singleRotateRight(NodePtr& t) {
        NodePtr l = t->left();
        t->setLeft(l->right());
        t->recalcHeight();

        l->setRight(t);
        t = l;
        t->recalcHeight();
    }

    static void doubleRotateLeft(NodePtr& t) {
        singleRotateRight(t->right());
        singleRotateLeft(t);
    }

    static void doubleRotateRight(NodePtr& t) {
        singleRotateLeft(t->left());
        singleRotateRight(t);
    }

    static void rotateLeft(NodePtr& t) {
        NodePtr r   = t->right();
        int     zag = tree_height(r->left());
        int     zig = tree_height(r->right());

        if (zig > zag) {
            singleRotateLeft(t);
        } else {
            doubleRotateLeft(t);
        }
    }

    static void rotateRight(NodePtr& t) {
        NodePtr l   = t->left();
        int     zig = tree_height(l->left());
        int     zag = tree_height(l->right());

        if (zig > zag) {
            singleRotateRight(t);
        } else {
            doubleRotateRight(t);
        }
    }

    static void rebalance(NodePtr& t) {
        int hl = tree_height(t->left());
        int hr = tree_height(t->right());

        if (hr > hl + 1) {
            rotateLeft(t);
        } else if (hl > hr + 1) {
            rotateRight(t);
        } else {
            t->recalcHeight();
        }
    }

    static void insert(Data d, NodePtr& t) {
        if (t == nullptr) {
            t = std::make_shared<Node<Data>>(d);
        } else if (d < t->data()) {
            insert(d, t->left());
            rebalance(t);
        } else if (d > t->data()) {
            insert(d, t->right());
            rebalance(t);
        }
    }

    static Data removeLeast(NodePtr& t) {
        if (t->left() == nullptr) {
            Data    data = t->data();
            NodePtr p    = t;
            t            = t->right();
            return data;
        } else {
            Data data = removeLeast(t->left());
            rebalance(t);
            return data;
        }
    }

    static void remove(Data data, NodePtr& t) {
        if (t != nullptr) {
            if (data < t->data()) {
                remove(data, t->left());
                rebalance(t);
            } else if (data > t->data()) {
                remove(data, t->right());
                rebalance(t);
            } else if (t->left() == nullptr) {
                NodePtr p = t;
                t         = t->right();
            } else if (t->right() == nullptr) {
                NodePtr p = t;
                t         = t->left();
            } else {
                t->setData(removeLeast(t->right()));
                rebalance(t);
            }
        }
    }
};

} // namespace tree

#endif
