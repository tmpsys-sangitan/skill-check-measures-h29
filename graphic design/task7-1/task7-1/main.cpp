//
//  main.cpp
//  task6-1
//
//  Created by PIC019 on 2017/05/29.
//  Copyright (c) 2017年 PIC019. All rights reserved.
//

/*
 * main.c
 * 画像読み込み・出力
 *
 * Created by ladmin on 2017/04/17.
 * Copyright (c) 2017年 ladmin. All rights reserved.
 */

#include <iostream>
#include <algorithm>
#include <cmath>

#define H 682 //hight
#define W 1024 //width
#define HEADER_SIZE 54
#define B 0
#define G 1
#define R 2
#define RGB 3
#define KAICHO 256

using namespace std;

void load_image(unsigned char inputpic[H][W][RGB], unsigned char head[HEADER_SIZE], FILE *fp);
void save_image(unsigned char inputpic[H][W][RGB], unsigned char head[HEADER_SIZE], FILE *fp);
void conversion_image(unsigned char inputpic[H][W][RGB]);
void FilterConversion(unsigned char inputpic[H][W][RGB]);

int main(int argc, const char * argv[])
{
    FILE *fp_in, *fp_out;
    unsigned char data[H][W][RGB], head[HEADER_SIZE];
    
    //画像の読み込み
    fp_in = fopen("/Users/PIC019/Desktop/t1620/b.bmp", "rb");
    load_image(data, head, fp_in);
    
    //画像の変換
    //conversion_image(data);
    
    //度数分布のヒストグラム描画
    FilterConversion(data);
    
    //画像の書き込み
    fp_out = fopen("/Users/PIC019/Desktop/t1620/a.bmp", "wb");
    save_image(data, head, fp_out);
    
    cout << "処理完了"<<endl;
    return 0;
}

void load_image(unsigned char inputpic[H][W][RGB], unsigned char head[HEADER_SIZE], FILE *fp)
{
    int i, j, k;
    
    //ヘッダ情報の読み込み
    for(i = 0; i < HEADER_SIZE; i++)
    {
        head[i] = fgetc(fp);
    }
    
    //データ部分の読み込み
    for(i = 0; i < H; i++)
    {
        for(j = 0; j < W; j++){
            for(k = 0; k < RGB; k++)
            {
                inputpic[i][j][k] = fgetc(fp);
            }
        }
    }
}

void save_image(unsigned char inputpic[H][W][RGB], unsigned char head[HEADER_SIZE], FILE *fp)
{
    int i, j, k;
    
    //ヘッダ情報の書き込み
    for(i = 0; i < HEADER_SIZE; i++)
    {
        fputc(head[i], fp);
    }
    
    //データ部分の書き込み
    for(i = 0; i < H; i++)
    {
        for(j = 0; j < W; j++)
        {
            for(k = 0; k < RGB; k++)
            {
                fputc(inputpic[i][j][k], fp);
            }
        }
    }
}

void conversion_image(unsigned char inputpic[H][W][RGB])
{
    int cov; // 変換後の値
    int i, j, k; // ループ制御
    
    for(i = 0; i < H; i++)
    {
        for(j = 0; j < W; j++)
        {
            // HDTV係数による加重平均法
            
            double red = pow((double)inputpic[i][j][R], 2.2) * 0.222015;
            double gre = pow((double)inputpic[i][j][G], 2.2) * 0.706655;
            double ble = pow((double)inputpic[i][j][B], 2.2) * 0.071330;
            cov = pow((double)red+gre+ble, 1/2.2);
            
            for(k = 0; k < RGB; k++)
            {
                inputpic[i][j][k] = cov;
            }
        }
    }
}

void FilterConversion(unsigned char inputpic[H][W][RGB])
{
    int i, j, k, l, m; // ループ制御
    int cov;     // 変換後の値
    #define K 9
    double filter[3][3] = {{1, 1, 1}, {1, -8, 1}, {1, 1, 1}};
    unsigned char keep[H][W][RGB];
    
    //inputpicをkeepに複製
    for(i = 0; i < H; i++)
    {
        for(j = 0; j < W; j++)
        {
            for(k = 0; k < RGB; k++)
            {
                keep[i][j][k] = inputpic[i][j][k];
            }
        }
    }
    
    // フィルタ作成
    cov = - K / 9;
    for (i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            filter[i][j] = cov;
        }
    }
    filter[1][1] = 1 + 8 * K / 9;
    
    for(i = 0; i < H; i++)
    {
        for(j = 0; j < W; j++)
        {
            for(k = 0; k < RGB; k++)
            {
                cov = 0;
                // フィルタ適用
                for (l = 0; l < 3; l++)
                {
                    if((i + l - 1) < 0 || H < (i + l - 1))
                    {
                        continue;
                    }
                    for(m = 0; m < 3; m++)
                    {
                        if((j + l - 1) < 0 || W < (j + m - 1))
                        {
                            continue;
                        }
                        cov += keep[i + l - 1][j + m - 1][k] * filter[l][m];
                    }
                }
                if(cov < 0){
                    cov = 0;
                }
                if(255 < cov){
                    cov = 255;
                }
                inputpic[i][j][k] = cov;
            }
        }
    }
}
