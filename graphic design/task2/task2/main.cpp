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

#define H 184 //hight
#define W 184 //width
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

int main(int argc, const char * argv[])
{
    FILE *fp_in, *fp_out;
    unsigned char data[H][W][RGB], head[HEADER_SIZE];
    
    //画像の読み込み
    fp_in = fopen("/Users/PIC019/Desktop/t1620/r.bmp", "rb");
    load_image(data, head, fp_in);
    
    //画像の変換
    conversion_image(data);
    
    //画像の書き込み
    fp_out = fopen("/Users/PIC019/Desktop/t1620/r2.bmp", "wb");
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
            // 単純平均法
            /*
            cov = (inputpic[i][j][R] + inputpic[i][j][G] + inputpic[i][j][B] ) /3;
             */
            
            // Gチャネル法
            /*
            cov = inputpic[i][j][G];
             */
            
            // 中間値法
            /*
            int cmax = max(max(inputpic[i][j][R], inputpic[i][j][G]), inputpic[i][j][B]);
            int cmin = min(min(inputpic[i][j][R], inputpic[i][j][G]), inputpic[i][j][B]);
            cov = (cmax + cmin) /2;
             */
            
            // NTSC係数による加重平均法
            /*
            cov = 0.298912 * inputpic[i][j][R] + 0.586611 * inputpic[i][j][G] + 0.114478*inputpic[i][j][B];
             */
            
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
