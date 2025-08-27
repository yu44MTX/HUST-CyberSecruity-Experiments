#ifndef HYU202112146_SAVE_CLASSIFY_H
#define HYU202112146_SAVE_CLASSIFY_H

#include "stdio.h"
#include "string.h"
#include "math.h"
#include "stdlib.h"

/****************************以上表面用到了那些库**********************************/

#define MAX_JZ_NUMBER 10000          //一次读入的最大基站个数
#define MAX_YD_NUMBER 20             //一次读入的终端移动最多线路
#define MAX_WZ_NUMBER 20             //一次读入的伪基站移动最多线路
#define MAX_JZ_DISTRICT_NUMBER 200   //分块后，每块最多基站个数
#define MAX_DISTRICT_LENGTH_X_ALL 20      //将所有基站分块,x轴分的最大个数
#define MAX_DISTRICT_LENGTH_Y_ALL 10      //将所有基站分块,y轴分的最大个数
#define MAX_DISTRICT_LENGTH_X_GS 30      //将高速分块，x轴分的最大个数
#define MAX_DISTRICT_LENGTH_Y_GS 10      //将高速分块，y轴分的最大个数
#define X_ALL_LENGTH 6000                //分块大小
#define Y_ALL_LENGTH 6000                //分块大小
#define X_GS_LENGTH 5000                 //分块大小
#define Y_GS_LENGTH 5000                 //分块大小
#define T1 0.01                           //查找时时间间隔
#define T2 0.1                           //最后一个功能的时间间隔

/*********************************以上为宏定义************************************/

typedef struct Jz{
    int x;
    int y;
    char type[5];
    float strength;
    int order;
    float Effective_radius;
}Jz;                          //将每个基站的信息存入

typedef struct District{
    int number;                      //number代表这个区块中有多少个基站
    int jzOrder[MAX_JZ_DISTRICT_NUMBER];
}District;              //将坐标系分区,且分区后，保证每个区块有不超过100个基站

typedef struct Yd{
    int Xs;
    int Ys;
    int Xe;
    int Ye;
    int velocity;
    int hour;
    int minute;
}Yd;                     //移动终端的移动轨迹信息

typedef struct Wz{
    int Xs;
    int Ys;
    int Xe;
    int Ye;
    int velocity;
    int hour;
    int minute;
    int order;
    double length;
    double time_start;
    double time_end;
}Wz;                    //伪基站的移动轨迹信息

/********************************以上定义用到的结构体**********************************/

static Jz jz[MAX_JZ_NUMBER];                //基站信息
static Yd yd[MAX_YD_NUMBER];                //终端信息
static Wz wz[MAX_WZ_NUMBER];                //伪基站信息
static District All[MAX_DISTRICT_LENGTH_X_ALL][MAX_DISTRICT_LENGTH_Y_ALL];    //将所有基站分块
static District Gs[MAX_DISTRICT_LENGTH_X_GS][MAX_DISTRICT_LENGTH_Y_GS];       //将高速基站分块
static int count_jz=0;               //读入基站个数
static int count_yd=0;               //读入终端路线个数
static int count_wz=0;               //读入伪基站路线个数
static int X_max=0;                  //读入基站X的最大值
static int X_min=0;                  //读入基站X的最小值
static int Y_max=0;                  //读入基站Y的最大值
static int Y_min=0;                  //读入基站Y的最小值
static int X_all_order_max=0;        //对于所有基站，分块的X的最大值
static int Y_all_order_max=0;        //对于所有基站，分块的Y的最大值
static int X_gs_order_max=0;         //对于高速基站，分块的X的最大值
static int Y_gs_order_max=0;         //对于高速基站，分块的Y的最大值

/********************************以上定义全局变量*****************************************/

void Save_JZ();                   //读入基站信息
void Save_YD();                   //读入移动端信息
void Save_WZ();                   //读入伪基站信息
void Extreme_value();             //获取基站坐标X，Y的最值，便于分块
void Partition1();                //将所有基站按一定方式分块
void Partition2();                //将高速基站分块
void function1();                 //实现功能1
void function2();                 //实现功能2
void function3_4();               //实现功能3,4
int Get_max_power(double x,double y,double*power);   //找到距离（x，y）最强信号的基站序号n，并返回
int Search_max_power(int x,int y,int X,int Y,double* power);  //找到某块区域中距离（x，y）信号最大的基站
double Get_distance(int x1,int y1,int x2,int y2);             //求距离
void function5();                 //实现功能5
void Get_direction_yd(double* sin,double* cos,int track,double yd_length);    //得到终端轨迹移动方向
void Get_direction_wz(double* sin,double* cos,int track,double wz_length);    //得到伪基站轨迹移动方向
void function6();                 //实现功能6
void function7();                 //实现功能7
void function8_9();               //实现功能8和9

/**********************************以上定义函数*********************************************/
#endif //HYU202112146_SAVE_CLASSIFY_H
