//
//  main.cpp
//  task01d
//
//  Created by PIC019 on 2017/04/10.
//  Copyright (c) 2017年 PIC019. All rights reserved.
//

#include <iostream>

int main(int argc, const char * argv[])
{
    int array[][9] = {{1, 0, 1, 0, 1, 0, 1, 0, 1},
                      {1, 1, 1, 0, 0, 0, 0, 0, 1},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0}};
    int yp[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    int xp = 0;
    
    for(int i = 0; i < 3; i++){
        xp = 0;
        for(int j = 0; j < 9; j++){
            std::cout << array[i][j];
            std::cout << ", ";
            if(array[i][j]){
                yp[j]++;
                xp++;
            }
        }
        std::cout << xp % 2;
        std::cout << "\n";
    }
    
    for(int i = 0; i < 9; i++){
        std::cout << yp[i] % 2;
        std::cout << ", ";
        if(yp[i] % 2){
            xp++;
        }
    }
    std::cout << xp % 2;
    std::cout << "\n";
    return 0;
}

