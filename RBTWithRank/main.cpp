//
//  main.cpp
//  RBTWithRank
//
//  Created by Erin Jane Ge on 12/26/20.
//  gtest frameword from: https://github.com/google/googletest
//  Copyright © 2020 Erin Jane Ge. All rights reserved.
//

#include <iostream>
#include "RBTWithRank.cpp"
#include<gtest/gtest.h>
using namespace std;

template<typename T>
RBT<T> CreateTree(T * data, int len) {
    RBT<T> tree;
    for (int i = 0; i < len; ++i) {
        tree.insert(*(data + i));
    }
    
    return tree;
}

TEST(RBTWithRankTest, TestRank) {
    int data[] = {3, 8, 9, 9, 9, 15};
    int len = sizeof(data) / sizeof(data[0]);
    RBT<int> tree = CreateTree(data, len);
    
    int * ranks = tree.rank(3);
    EXPECT_EQ(0, *ranks) << "rank(3) lower bound shoud be 0, actual output is " << *ranks << endl;
    EXPECT_EQ(0, *(ranks + 1)) << "rank(3) upper bound shoud be 0, actual output is " << *(ranks + 1) << endl;
    
    ranks = tree.rank(8);
    EXPECT_EQ(1, *ranks) << "rank(8) lower bound shoud be 1, actual output is " << *ranks << endl;
    EXPECT_EQ(1, *(ranks + 1)) << "rank(8) upper bound shoud be 1, actual output is " << *(ranks + 1) << endl;
    
    ranks = tree.rank(9);
    EXPECT_EQ(2, *ranks) << "rank(9) lower bound shoud be 2, actual output is " << *ranks << endl;
    EXPECT_EQ(4, *(ranks + 1)) << "rank(9) upper bound shoud be 4, actual output is " << *(ranks + 1) << endl;
    
    ranks = tree.rank(15);
    EXPECT_EQ(5, *ranks) << "rank(15) lower bound shoud be 5, actual output is " << *ranks << endl;
    EXPECT_EQ(5, *(ranks + 1)) << "rank(15) upper bound shoud be 5, actual output is " << *(ranks + 1) << endl;
}

TEST(RBTWithRankTest, TestIndex) {
    int data[] = {3, 8, 9, 9, 9, 15};
    int len = sizeof(data) / sizeof(data[0]);
    RBT<int> tree = CreateTree(data, len);
    
    for (int i = 0; i < len; ++i) {
        int output = tree.index(i);
        EXPECT_EQ(*(data + i), output) << "index(" << i << ") should be" << *(data + i) << ", actual output is  " << output << endl;
    }
}

TEST(RBTWithRankTest, TestDouble) {
    double data[] = {3.1, 8.1, 9.1, 9.1, 9.1, 15.1};
    int len = sizeof(data) / sizeof(data[0]);
    RBT<double> tree = CreateTree(data, len);
    
    int * ranks = tree.rank(9.1);
    EXPECT_EQ(2, *ranks) << "rank(9.1) lower bound shoud be 2, actual output is " << *ranks << endl;
    EXPECT_EQ(4, *(ranks + 1)) << "rank(9.1) upper bound shoud be 4, actual output is " << *(ranks + 1) << endl;
    
    for (int i = 0; i < len; ++i) {
        double output = tree.index(i);
        EXPECT_EQ(*(data + i), output) << "index(" << i << ") should be" << *(data + i) << ", actual output is  " << output << endl;
    }
}

int main(int argc, char * argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    
    return RUN_ALL_TESTS();
}


