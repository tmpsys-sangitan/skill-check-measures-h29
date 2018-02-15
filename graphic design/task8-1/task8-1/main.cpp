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

#define H 720 //hight
#define W 1280 //width
#define HEADER_SIZE 54
#define B 0
#define G 1
#define R 2
#define RGB 3
#define KAICHO 256

#define fil_n 30

using namespace std;

void load_image(unsigned char inputpic[H][W][RGB], unsigned char head[HEADER_SIZE], FILE *fp);
void save_image(unsigned char inputpic[H][W][RGB], unsigned char head[HEADER_SIZE], FILE *fp);
void ColerFilter(unsigned char inputpic[H][W][RGB]);
void SmoothingFilter(unsigned char inputpic[H][W][RGB], int loop);

int main(int argc, const char * argv[])
{
    FILE *fp_in, *fp_out;
    unsigned char data[H][W][RGB], head[HEADER_SIZE];
    
    //画像の読み込み
    fp_in = fopen("/Users/PIC019/Desktop/t1620/task8-1/b5.bmp", "rb");
    load_image(data, head, fp_in);
    
    //鮮やかさフィルタ
    //ColerFilter(data);
    
    //線形平滑化フィルタ
    for (int i = 0;i < 30; i++){
        SmoothingFilter(data, i);
    }
    
    //画像の書き込み
    fp_out = fopen("/Users/PIC019/Desktop/t1620/task8-1/a5.bmp", "wb");
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

void ColerFilter(unsigned char inputpic[H][W][RGB])
{
    unsigned char keep[H][W][RGB];
    double filter[3][3] = {{2.0, -0.5, -0.5}, {-0.5, 2.0, -0.5}, {-0.5, -0.5, 2.0}};
    
    //inputpicをkeepに複製
    for(int i = 0; i < H; i++)
    {
        for(int j = 0; j < W; j++)
        {
            for(int k = 0; k < RGB; k++)
            {
                keep[i][j][k] = inputpic[i][j][k];
            }
        }
    }
    
    for(int i = 0; i < H; i++)
    {
        for(int j = 0; j < W; j++)
        {
            for(int k = 0; k < RGB; k++)
            {
                int cov = 0;
                for (int l = 0; l < 3; l++)
                {
                    cov += keep[i][j][l] * filter[k][l];
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

void SmoothingFilter(unsigned char inputpic[H][W][RGB], int loop)
{
    unsigned char keep[H][W][RGB];
    unsigned char filter[3][3] = {{0, 1, 0}, {1, 2, 1}, {0, 1, 0}};
    
    //inputpicをkeepに複製
    for(int i = 0; i < H; i++)
    {
        for(int j = 0; j < W; j++)
        {
            for(int k = 0; k < RGB; k++)
            {
                keep[i][j][k] = inputpic[i][j][k];
            }
        }
    }
    
    // 全体画素y
    for(int i = 0; i < H; i++)
    {
        int threshold;
        // 中央部なら中断
        if((H / 3) < i && i < (H * 2 / 3))
        {
            continue;
        }
        // 上部なら
        else if(i < (H / 3))
        {
            threshold = i - (H / 3);
        }
        // 下部なら
        else
        {
            threshold = i - (H * 2 / 3);
        }
        if(threshold * threshold * (9 * fil_n) / (H * H) <= loop){
            continue;
        }
        // 全体画素x
        for(int j = 0; j < W; j++)
        {
            // 画素
            for(int k = 0; k < RGB; k++)
            {
                int cov = 0;
                int cnt = 0;
                // 周辺画素y
                for (int l = 0; l < 3; l++)
                {
                    // 画像範囲はみ出し
                    if((i + l - 1) < 0 || H <= (i + l - 1))
                    {
                        continue;
                    }
                    // 周辺画素x
                    for(int m = 0; m < 3; m++)
                    {
                        // 画像範囲はみ出し
                        if((j + m - 1) < 0 || W <= (j + m - 1))
                        {
                            continue;
                        }
                        cov += keep[i + l - 1][j + m - 1][k] * filter[l][m];
                        cnt += filter[l][m];
                    }
                }
                cov /= cnt;
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

