//
//  main.cpp
//  task01b
//
//  Created by PIC019 on 2017/04/10.
//  Copyright (c) 2017年 PIC019. All rights reserved.
//

#include <iostream>

int main(int argc, const char * argv[])
{
    int array[] = {2, 6, 4, 8, 1, 9, 5, 3, 7};
    
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 8 - i; j++){
            if(array[j] > array[j + 1]){
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
    for(int i = 0; i < 9; i++){
        std::cout << array[i];
        if(i < 8) std::cout << ", ";
    }
    return 0;
}

