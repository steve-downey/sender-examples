#include <tree/tree.h>

#include <gtest/gtest.h>

#include <memory>

using namespace tree;

TEST(TreeTest, TestGTest) {
    ASSERT_EQ(1, 1);
}

TEST(TreeTest, Breathing) {
    tree::Node<int> n1;
    tree::Node<int> n1a(n1);
    tree::Node<int> n1b(std::move(n1a));

    tree::Node<int> n2;
    tree::Node<int> n3;

    n2 = n1;
    n3 = std::move(n2);

    tree::Tree<int> t1;
    tree::Tree<int> t1a(t1);
    tree::Tree<int> t1b(std::move(t1a));

    tree::Tree<int> t2;
    tree::Tree<int> t3;

    t2 = t1;
    t3 = std::move(t2);
}

TEST(TreeTest, construct) {
    tree::NodePtr<int> t = std::make_shared<Node<int>>();

    Tree<int>::insert(0, t);

    EXPECT_EQ(0, t->data());
    Tree<int>::insert(1, t);
    Tree<int>::insert(2, t);
    Tree<int>::insert(3, t);
    Tree<int>::insert(4, t);

    /*

           1
        0     3
            2   4

     */
    EXPECT_EQ(1, t->data());
    EXPECT_EQ(3, t->height());
    EXPECT_EQ(1, t->left()->height());
    EXPECT_EQ(0, t->left()->data());
    EXPECT_EQ(2, t->right()->height());
    EXPECT_EQ(3, t->right()->data());

    EXPECT_NE(nullptr, t->right()->left());
    EXPECT_EQ(2, t->right()->left()->data());
    EXPECT_EQ(1, t->right()->left()->height());

    EXPECT_NE(nullptr, t->right()->right());
    EXPECT_EQ(4, t->right()->right()->data());
    EXPECT_EQ(1, t->right()->right()->height());

    EXPECT_EQ(nullptr, t->right()->right()->right());

    EXPECT_EQ(nullptr, t->right()->right()->left());

    Tree<int>::remove(4, t);
    EXPECT_EQ(nullptr, t->right()->right());
}
template class tree::Tree<int>;
template class tree::Tree<std::string>;
