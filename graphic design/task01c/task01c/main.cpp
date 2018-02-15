//
//  main.cpp
//  task01c
//
//  Created by PIC019 on 2017/04/10.
//  Copyright (c) 2017年 PIC019. All rights reserved.
//

#include <iostream>

int main(int argc, const char * argv[])
{
    int array[] = {1, 0, 1, 1, 0, 0, 1, 0, 1};
    int p = 0;
    
    for(int i = 0; i < 9; i++){
        std::cout << array[i];
        std::cout << ", ";
        if(array[i] == 1){
            p++;
        }
    }
    std::cout << p % 2;
    
    return 0;
}

