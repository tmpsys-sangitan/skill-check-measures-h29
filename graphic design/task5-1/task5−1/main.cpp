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
void ffine_transformation(unsigned char inputpic[H][W][RGB]);

int main(int argc, const char * argv[])
{
    FILE *fp_in, *fp_out;
    unsigned char data[H][W][RGB], head[HEADER_SIZE];
    
    //画像の読み込み
    fp_in = fopen("/Users/PIC019/Desktop/t1620/b.bmp", "rb");
    load_image(data, head, fp_in);
    
    //画像の変換
    ffine_transformation(data);
    
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

void ffine_transformation(unsigned char inputpic[H][W][RGB])
{
    #define PI 3.141592653589793
    
    int i, j, k, n; // ループ制御
    int ax, ay;
     double M[3][6] =   {{1.5, 0, 0, 1, 0, 0}, {0, 1, 0, 0, 1, 0}, {0, 0, 1, 0, 0, 1}};
    // double M[3][6] =   {{cos(PI/6), sin(PI/6), 0, 1, 0, 0}, {-sin(PI/6), cos(PI/6), 0, 0, 1, 0}, {0, 0, 1, 0, 0, 1}};
    double pivot, mul;
    unsigned char keep[H][W][RGB] = {};
    
    for (i = 0; i < 3; ++i)
    {
        pivot = M[i][i];
        for (j = 0; j < 3 * 2; ++j)
        {
            M[i][j] = (1 / pivot) * M[i][j];
        }
        for (k = i + 1; k < 3; ++k)
        {
            mul = M[k][i];
            for (n = i; n < 6; ++n)
            {
                M[k][n] = M[k][n] -  mul * M[i][n];
            }
        }
    }
    for (i = 2; i > 0; --i)
    {
        for (k = i - 1; k >= 0; --k)
        {
            mul = M[k][i];
            for (n = i; n < 6; ++n)
            {
                M[k][n] = M[k][n] - mul * M[i][n];
            }
        }
    }
    
    //アフィン変換
    for(i = 0; i < H; i++)
    {
        for(j = 0; j < W; j++)
        {
            /*
            // 右40px、下30px平行移動
            ay = (1 * i) + (0 * j) + 30;
            ax = (0 * i) + (1 * j) + 40;
            */
            
            /*
            // x軸1.5倍
            ay = (1 * i) + (0 * j) + 0;
            ax = (0 * i) + (1.5 * j) + 0;
            */
            
            /*
            // 反時計回り30度
            ay = (cos(-PI * 1 / 6) * i) + (sin(-PI * 1 / 6) * j) + 0;
            ax = (-sin(-PI * 1 / 6) * i) + (cos(-PI * 1 / 6) * j) + 0;
            */
            
            /*
            // 反転
            ay = (-1 * i) + (0 * j) + H;
            ax = (0 * i) + (-1 * j) + W;
            */
            
            /*
            // 平行5度 鉛直10度
            ay = (1 * i) + (tan(PI / 18) * j) + 0;
            ax = (tan(PI / 36) * i) + (1 * j) + 0;
            */
            
            ay = M[1][3] * j + M[1][4] * i + M[1][5];
            ax = M[0][3] * j + M[0][4] * i + M[0][5];
            if(ay < 0 || H <= ay || ax < 0 || W <= ax){
                continue;
            }
            for(k = 0; k < RGB; k++)
            {
                // keep[ay][ax][k] = inputpic[i][j][k];
                keep[i][j][k] = inputpic[ay][ax][k];
            }
        }
    }
    
    //keepをinputpicに複製
    for(i = 0; i < H; i++)
    {
        for(j = 0; j < W; j++)
        {
            for(k = 0; k < RGB; k++)
            {
                inputpic[i][j][k] = keep[i][j][k];
            }
        }
    }
}
