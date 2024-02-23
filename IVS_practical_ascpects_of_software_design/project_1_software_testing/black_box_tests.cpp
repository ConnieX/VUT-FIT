//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     Monika Rosinska <xrosin00@stud.fit.vutbr.cz>
// $Date:       $2019-04-03
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author Monika Rosinska
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//

class EmptyTree : public ::testing::Test
{
  protected:
    BinaryTree tree;
};

class NonEmptyTree : public ::testing::Test
{
  protected:
    virtual void SetUp()
    {
        int values[] = {10, 85, 15, 35, 20, 63, 'a', 65, 22, 987, 64};

        for (int i = 0; i < 14; ++i)
            tree.InsertNode(values[i]);
    }
    BinaryTree tree;
};

class TreeAxioms : public ::testing::Test
{
  protected:
    virtual void SetUp()
    {
        int values[] = {10, 85, 15, 35, 20, 63, 21, 68, 8, 52};

        for (int i = 0; i < 14; ++i)
            tree.InsertNode(values[i]);
    }
    BinaryTree tree;
};

TEST_F(EmptyTree, InsertNode)
{
    std::pair<bool, Node_t *> node = tree.InsertNode(0);
    EXPECT_TRUE(node.first);
    EXPECT_TRUE(node.second != NULL);
    EXPECT_TRUE((node.second)->key == 0);
    EXPECT_TRUE(node.second->pParent == NULL);

    std::pair<bool, Node_t *> node2 = tree.InsertNode(0);
    EXPECT_FALSE(node2.first);
    EXPECT_TRUE(node2.second == node.second);

    node = tree.InsertNode('a');
    EXPECT_TRUE(node.first);
    EXPECT_TRUE((node.second)->key == 'a');

    node = tree.InsertNode(-2);
    EXPECT_TRUE(node.first);
    EXPECT_TRUE((node.second)->key == -2);
}

TEST_F(EmptyTree, DeleteNode)
{
    bool node = tree.DeleteNode(0);
    EXPECT_FALSE(node);

    node = tree.DeleteNode('a');
    EXPECT_FALSE(node);
}

TEST_F(EmptyTree, FindNode)
{
    Node_t *node = tree.FindNode(0);
    EXPECT_TRUE(node == NULL);
}

TEST_F(NonEmptyTree, InsertNode)
{
    std::pair<bool, Node_t *> node = tree.InsertNode(0);
    EXPECT_TRUE(node.first);
    EXPECT_TRUE(node.second != NULL);
    EXPECT_TRUE((node.second)->key == 0);

    node = tree.InsertNode(10);
    EXPECT_FALSE(node.first);
    EXPECT_TRUE(node.second != NULL);
    EXPECT_TRUE((node.second)->key == 10);
}

TEST_F(NonEmptyTree, DeleteNode)
{
    bool node = tree.DeleteNode(0);
    EXPECT_FALSE(node);

    node = tree.DeleteNode(15);
    EXPECT_TRUE(node);

    node = tree.DeleteNode(15);
    EXPECT_FALSE(node);
}

TEST_F(NonEmptyTree, FindNode)
{
    Node_t *node = tree.FindNode(0);
    EXPECT_TRUE(node == NULL);

    node = tree.FindNode(15);
    EXPECT_FALSE(node == NULL);
    EXPECT_TRUE(node->key == 15);
}

TEST_F(TreeAxioms, Axiom1)
{
    std::vector<Node_t *> leafNodes;
    tree.GetLeafNodes(leafNodes);

    for (int i = 0; i < leafNodes.size(); i++)
    {
        EXPECT_TRUE(leafNodes[i]->pLeft == NULL);
        EXPECT_TRUE(leafNodes[i]->pRight == NULL);
        EXPECT_TRUE(leafNodes[i]->color == BLACK);
    }
}

TEST_F(TreeAxioms, Axiom2)
{
    std::vector<Node_t *> nodes;
    tree.GetAllNodes(nodes);

    for (int i = 0; i < nodes.size(); i++)
    {
        if (nodes[i]->color == RED)
        {
            EXPECT_TRUE(nodes[i]->pLeft->color == BLACK);
            EXPECT_TRUE(nodes[i]->pRight->color == BLACK);
        }
    }
}

TEST_F(TreeAxioms, Axiom3)
{
    std::vector<Node_t *> leafNodes;
    tree.GetLeafNodes(leafNodes);
    Node_t *nextNode = leafNodes[0];

    int blackNodesFirst = 0;
    int blackNodesSecond = 0;

    while (nextNode->pParent != NULL)
    {
        nextNode = nextNode->pParent;
        if (nextNode->color == BLACK)
        {
            blackNodesFirst++;
        }
    }

    for (int i = 1; i < leafNodes.size(); i++)
    {
        blackNodesSecond = 0;
        nextNode = leafNodes[i];
        while (nextNode->pParent != NULL)
        {
            nextNode = nextNode->pParent;
            if (nextNode->color == BLACK)
            {
                blackNodesSecond++;
            }
        }
        EXPECT_TRUE(blackNodesFirst == blackNodesSecond);
    }
}
/*** Konec souboru black_box_tests.cpp ***/
