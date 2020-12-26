//
//  main.cpp
//  RBTWithRank
//
//  Created by Erin Jane Ge on 12/26/20.
//  Copyright © 2020 Erin Jane Ge. All rights reserved.
//

#include <iostream>
#include "RBTWithRank.hpp"

int main(int argc, const char * argv[]) {
    RBT tree;
    for (int i = 1; i < 7; ++i) {
        tree.insert(i);
    }
    return 0;
}
