//
//  main.cpp
//  RBTWithRank
//
//  Created by Erin Jane Ge on 12/26/20.
//  Copyright © 2020 Erin Jane Ge. All rights reserved.
//

#include <iostream>
#include "RBTWithRank.hpp"
#include<gtest/gtest.h>

const int DATA[6] = {3, 8, 9, 9, 9, 15};

TEST(RBTWithRankTest, TestInsertInput) {
    RBT tree;
    int len = sizeof(DATA) / sizeof(DATA[0]);
    for (int i = 0; i < len; ++i) {
        tree.insert(DATA[i]);
    }
    
    EXPECT_EQ(15, tree.maximum());
}

int main(int argc, char * argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    
    return RUN_ALL_TESTS();
}


