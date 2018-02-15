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

#define H 734 //hight
#define W 1200 //width
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
void binarization(unsigned char inputpic[H][W][RGB]);

int main(int argc, const char * argv[])
{
    FILE *fp_in, *fp_out;
    unsigned char data[H][W][RGB], head[HEADER_SIZE];
    
    //画像の読み込み
    fp_in = fopen("/Users/PIC019/Desktop/t1620/b.bmp", "rb");
    load_image(data, head, fp_in);
    
    //画像の変換
    conversion_image(data);
    
    //度数分布のヒストグラム描画
    binarization(data);
    
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

void binarization(unsigned char inputpic[H][W][RGB])
{
    int i, j, k, l, m; // ループ制御
    int cov;     // 変換後の値
    long lim;    // 閾値
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
    
    for(i = 0; i < H; i++)
    {
        for(j = 0; j < W; j++)
        {
            for (k = -1, m = 0, lim = 0; k < 2; k++)
            {
                for (l = -1; l < 2; l++)
                {
                    if((i+k >= 0) && i+k < H && j+l >= 0 && j+l < W)
                    {
                        lim += keep[i + k][j + l][0];
                        m++;
                    }
                }
            }
            lim = lim / m + 1;
            cov = keep[i][j][0]/lim;
            for(k = 0; k < RGB; k++)
            {
                inputpic[i][j][k] = cov > 0 ? 255 : 0;
            }
        }
    }
}
