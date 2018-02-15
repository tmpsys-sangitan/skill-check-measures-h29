//
//  main.cpp
//  task01
//
//  Created by PIC019 on 2017/04/10.
//  Copyright (c) 2017年 PIC019. All rights reserved.
//

#include <iostream>
int main(int argc, const char * argv[])
{
    int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int i, j, out;
    
    for (i = 0; i < 9;i++) {
        std::cout << array[i] * 2;
        if(i < 8){
            std::cout << ", ";
        }
    }
    std::cout << "\n";
    
    for(out = 0, i = 0; i < 9; i++){
        for(j = 2; j < array[i]; j++){
            if(array[i] % j == 0){
                break;
            }
        }
        if(j == array[i]){
            if(out){
                std::cout << ", ";
            }
            std::cout << array[i];
            out++;
        }
    }

}

