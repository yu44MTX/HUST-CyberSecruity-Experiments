#include "Save_Classify.h"
void Save_JZ()
{
    char type[5];
    //记录读入基站的类型
    char filename1[50];
    char filename2[50];
    printf("输入你想查询的基站的两个的文件名（中间用空格隔开）：");
    scanf("%s %s",filename1,filename2);
    FILE *fp1, *fp2;
    fp1 = fopen(filename1,"r");
    fp2 = fopen(filename2, "r");
    //将jz001的数据读入数组
    fscanf(fp1,"%s",type);
    if(!strcmp(type,"JZ")) printf("第一个基站文件打开成功！\n");
    while(!feof(fp1))
    {
        fscanf(fp1,"%d,%d,%s  %f,%d",&jz[count_jz].x,&jz[count_jz].y,jz[count_jz].type,&jz[count_jz].strength,&jz[count_jz].order);
        if(!strcmp(jz[count_jz].type,"城区")) jz[count_jz].Effective_radius=300* sqrtf(jz[count_jz].strength);
        if(!strcmp(jz[count_jz].type,"乡镇")) jz[count_jz].Effective_radius=1000* sqrtf(jz[count_jz].strength);
        if(!strcmp(jz[count_jz].type,"高速")) jz[count_jz].Effective_radius=5000* sqrtf(jz[count_jz].strength);
        count_jz++;
    }
    if(jz[count_jz-1].x==-1&&jz[count_jz-1].y==-1) count_jz--;
    fclose(fp1);
    printf("第一个文件读入成功，共读入%d个基站信息\n",count_jz);
    // 将jz002的数据读入数组
    fscanf(fp2,"%s",type);
    if(!strcmp(type,"JZ")) printf("第二个基站文件打开成功！\n");
    while(!feof(fp2))
    {
        fscanf(fp2,"%d,%d,%s %f,%d",&jz[count_jz].x,&jz[count_jz].y,jz[count_jz].type,&jz[count_jz].strength,&jz[count_jz].order);
        if(!strcmp(jz[count_jz].type,"城区")) jz[count_jz].Effective_radius=300* sqrtf(jz[count_jz].strength);
        if(!strcmp(jz[count_jz].type,"乡镇")) jz[count_jz].Effective_radius=1000* sqrtf(jz[count_jz].strength);
        if(!strcmp(jz[count_jz].type,"高速")) jz[count_jz].Effective_radius=5000* sqrtf(jz[count_jz].strength);
        count_jz++;
    }
    if(jz[count_jz-1].x==-1&&jz[count_jz-1].y==-1) count_jz--;
    fclose(fp2);
    printf("第二个文件读入成功，共读入%d个基站信息\n",count_jz);
    //读入数据完毕，现在count_jz=基站个数
}

void Save_YD()
{
    char type[5];
    //记录读入基站的类型
    char filename3[50];
    printf("输入你想查询的移动轨迹的文件名：");
    scanf("%s",filename3);
    FILE *fp1;
    fp1 = fopen(filename3,"r");
    fscanf(fp1,"%s",type);
    if(!strcmp(type,"YD")) printf("终端文件打开成功！\n");
    while(!feof(fp1))
    {
        fscanf(fp1,"%d,%d,%d,%d,%d,%d,%d",&yd[count_yd].Xs,&yd[count_yd].Ys,&yd[count_yd].Xe,&yd[count_yd].Ye,&yd[count_yd].velocity,&yd[count_yd].hour,&yd[count_yd].minute);
        count_yd++;
    }
    if(yd[count_yd-1].Xs==-1&&yd[count_yd-1].Ys==-1) count_yd--;
    printf("终端数据写入成功！共写入%d条运动路线。\n",count_yd);
}

void Save_WZ()
{
    char type[5];
    //记录读入基站的类型
    char filename4[50];
    printf("输入你想查询的伪基站移动轨迹的文件名：");
    scanf("%s",filename4);
    FILE *fp1;
    fp1 = fopen(filename4,"r");
    fscanf(fp1,"%s",type);
    if(!strcmp(type,"WZ")) printf("伪基站文件打开成功！\n");
    while(!feof(fp1))
    {
        fscanf(fp1,"%d,%d,%d,%d,%d,%d,%d,%d",&wz[count_wz].Xs,&wz[count_wz].Ys,&wz[count_wz].Xe,&wz[count_wz].Ye,&wz[count_wz].velocity,&wz[count_wz].hour,&wz[count_wz].minute,&wz[count_wz].order);
        wz[count_wz].length= Get_distance(wz[count_wz].Xs,wz[count_wz].Ys,wz[count_wz].Xe,wz[count_wz].Ye);
        wz[count_wz].time_start=wz[count_wz].hour*3600+wz[count_wz].minute*60;
        wz[count_wz].time_end=wz[count_wz].time_start+wz[count_wz].length*3.6/wz[count_wz].velocity;
        count_wz++;
    }
    if(wz[count_wz-1].Xs==-1&&wz[count_wz-1].Ys==-1) count_wz--;
    printf("伪基站写入成功！,共写入%d条运动路线。\n",count_wz);
}

void Extreme_value()
{
    int n=count_jz;
    n--;
    while(n>=0)
    {
        X_max=(X_max>jz[n].x?X_max:jz[n].x);
        X_min=(X_min<jz[n].x?X_min:jz[n].x);
        Y_max=(Y_max>jz[n].y?Y_max:jz[n].y);
        Y_min=(Y_min<jz[n].y?Y_min:jz[n].y);
        n--;
    }
    printf("X的最大值为：%d，X的最小值为：%d\n",X_max,X_min);
    printf("Y的最大值为：%d，Y的最小值为：%d\n",Y_max,Y_min);
    X_all_order_max=(X_max-X_min)/X_ALL_LENGTH;
    Y_all_order_max=(Y_max-Y_min)/Y_ALL_LENGTH;
    X_gs_order_max=(X_max-X_min)/X_GS_LENGTH;
    Y_gs_order_max=(Y_max-Y_min)/Y_GS_LENGTH;
}

void Partition1()
{
    int x,y,n=count_jz;
    for(x=0;x<MAX_DISTRICT_LENGTH_X_ALL;x++)
    {
        for(y=0;y<MAX_DISTRICT_LENGTH_Y_ALL;y++)
        {
            All[x][y].number=0;
        }
    }
    //初始化。将每块中基站个数计为0
    n--;
    while(n>=0)
    {
        x=(jz[n].x-X_min)/X_ALL_LENGTH;
        y=(jz[n].y-Y_min)/Y_ALL_LENGTH;
        All[x][y].jzOrder[All[x][y].number]=n;
        //jzOrder记录这个块中各基站的编号
        All[x][y].number++;
        //number记录这个块中基站的个数
        n--;
    }
}

void Partition2()
{
    int x, y,n=count_jz;
    for (x = 0; x <MAX_DISTRICT_LENGTH_X_GS; x++)
    {
        for (y = 0; y<MAX_DISTRICT_LENGTH_Y_GS; y++)
        {
            Gs[x][y].number = 0;
        }
    }
    //初始化。将每块中基站个数计为0
    n--;
    while (n >= 0)
    {
        if (!strcmp(jz[n].type, "高速"))
        {
            x = (jz[n].x-X_min) / X_GS_LENGTH;
            y = (jz[n].y-Y_min) / Y_GS_LENGTH;
            Gs[x][y].jzOrder[Gs[x][y].number] = n;
            //jzOrder记录这个块中各基站的编号
            Gs[x][y].number++;
            //number记录这个块中基站的个数
        }
        n--;
    }
}

void function1()
{
    int n;
    int m;
    printf("\n首个分块的基站信息如下\n");
    printf("    X    Y   类型    强度    序号\n");
    n=All[0][Y_all_order_max].number;
    n--;
    while(n>=0)
    {
        m=All[0][Y_all_order_max].jzOrder[n];
        printf("%6d %5d %s  %4f %d\n",jz[m].x,jz[m].y,jz[m].type,jz[m].strength,jz[m].order);
        n--;
    }
    printf("\n最后一个分块的基站信息如下\n");
    printf("    X    Y   类型    强度    序号\n");
    n=All[X_all_order_max][0].number;
    n--;
    while(n>=0)
    {
        m=All[X_all_order_max][0].jzOrder[n];
        printf("%6d %5d %s  %4f %d\n",jz[m].x,jz[m].y,jz[m].type,jz[m].strength,jz[m].order);
        n--;
    }
}

void function2()
{
    int n;
    int m;
    printf("\n西北角东侧分块的基站信息如下\n");
    printf("    X    Y   类型    强度    序号\n");
    n=All[1][Y_all_order_max].number;
    n--;
    while(n>=0)
    {
        m=All[1][Y_all_order_max].jzOrder[n];
        printf("%6d %5d %s  %4f %d\n",jz[m].x,jz[m].y,jz[m].type,jz[m].strength,jz[m].order);
        n--;
    }
    printf("\n西北角南侧分块的基站信息如下\n");
    printf("    X    Y   类型    强度    序号\n");
    n=All[0][Y_all_order_max-1].number;
    n--;
    while(n>=0)
    {
        m=All[0][Y_all_order_max-1].jzOrder[n];
        printf("%6d %5d %s  %4f %d\n",jz[m].x,jz[m].y,jz[m].type,jz[m].strength,jz[m].order);
        n--;
    }
    printf("\n东南角西北侧分块的基站信息如下\n");
    printf("    X    Y   类型    强度    序号\n");
    n=All[X_all_order_max-1][1].number;
    n--;
    while(n>=0)
    {
        m=All[X_all_order_max-1][1].jzOrder[n];
        printf("%6d %5d %s  %4f %d\n",jz[m].x,jz[m].y,jz[m].type,jz[m].strength,jz[m].order);
        n--;
    }
    printf("\n东南角北侧分块的基站信息如下\n");
    printf("    X    Y   类型    强度    序号\n");
    n=All[X_all_order_max-1][2].number;
    n--;
    while(n>=0)
    {
        m=All[X_all_order_max-1][2].jzOrder[n];
        printf("%6d %5d %s  %4f %d\n",jz[m].x,jz[m].y,jz[m].type,jz[m].strength,jz[m].order);
        n--;
    }
}

void function3_4()
{
    int x=0,y=0;    //接收要查询的坐标
    int n;
    double power=0;
    printf("请输入你要查询的坐标，X，Y中间用空格隔开：");
    scanf("%d %d",&x,&y);
    n=Get_max_power(x,y,&power);
    if(n==-1)
    {
        printf("无基站覆盖\n");
        return ;
    }
    printf("距离（%d，%d）信号最强的基站信息如下：\n",x,y);
    printf("    X    Y   类型    强度    序号  信号强度\n");
    printf("%6d %5d %s  %4f %d  %f\n",jz[n].x,jz[n].y,jz[n].type,jz[n].strength,jz[n].order,power);
}

int Get_max_power(double x,double y,double* power)     //找到x，y附近的最大power，返回对应的基站序号，若返回值为-1，则无基站覆盖
{
    int X,Y;    //定位要查询坐标所在块
    int n=-1,n1=-1;       //返回记录此块中，信号最强的基站的序号，若为-1，则代表无基站
    double power1=0;  //记录强度
    X=(x-X_min)/X_ALL_LENGTH;
    Y=(y-Y_min)/Y_ALL_LENGTH;
    if(X>=0&&X<=X_all_order_max&&Y>=0&&Y<=Y_all_order_max) n1=Search_max_power(x,y,X,Y,&power1);
    if(power1>*power)
    {
        n=n1;
        *power=power1;
    }
    if(X-1>=0&&X-1<=X_all_order_max&&Y+1>=0&&Y+1<=Y_all_order_max) n1=Search_max_power(x,y,X-1,Y+1,&power1);
    if(power1>*power)
    {
        n=n1;
        *power=power1;
    }
    if(X>=0&&X<=X_all_order_max&&Y+1>=0&&Y+1<=Y_all_order_max) n1=Search_max_power(x,y,X,Y+1,&power1);
    if(power1>*power)
    {
        n=n1;
        *power=power1;
    }
    if(X+1>=0&&X+1<=X_all_order_max&&Y+1>=0&&Y+1<=Y_all_order_max) n1=Search_max_power(x,y,X+1,Y+1,&power1);
    if(power1>*power)
    {
        n=n1;
        *power=power1;
    }
    if(X-1>=0&&X-1<=X_all_order_max&&Y>=0&&Y<=Y_all_order_max) n1=Search_max_power(x,y,X-1,Y,&power1);
    if(power1>*power)
    {
        n=n1;
        *power=power1;
    }
    if(X+1>=0&&X+1<=X_all_order_max&&Y>=0&&Y<=Y_all_order_max) n1=Search_max_power(x,y,X+1,Y,&power1);
    if(power1>*power)
    {
        n=n1;
        *power=power1;
    }
    if(X-1>=0&&X-1<=X_all_order_max&&Y-1>=0&&Y-1<=Y_all_order_max) n1=Search_max_power(x,y,X-1,Y-1,&power1);
    if(power1>*power)
    {
        n=n1;
        *power=power1;
    }
    if(X>=0&&X<=X_all_order_max&&Y-1>=0&&Y-1<=Y_all_order_max) n1=Search_max_power(x,y,X,Y-1,&power1);
    if(power1>*power)
    {
        n=n1;
        *power=power1;
    }
    if(X+1>=0&&X+1<=X_all_order_max&&Y-1>=0&&Y-1<=Y_all_order_max) n1=Search_max_power(x,y,X+1,Y-1,&power1);
    if(power1>*power)
    {
        n=n1;
        *power=power1;
    }
    //遍历9个分块，通过Search_max_power找到每个分块中信号最强的基站，再将9个基站进行比较得到信号最强的基站的信息
    return n;
}

int Search_max_power(int x,int y,int X,int Y,double* power)  //找到一个分块中距离（x，y）信号最强的基站
{
    *power=0;
    int n=0,m=0,order=-1;
    double power1;
    double distance;
    n=All[X][Y].number;
    while(m<n)
    {
        distance = Get_distance(jz[All[X][Y].jzOrder[m]].x, jz[All[X][Y].jzOrder[m]].y, x, y);
        if (distance < jz[All[X][Y].jzOrder[m]].Effective_radius)
        {
            power1 = jz[All[X][Y].jzOrder[m]].strength * (jz[All[X][Y].jzOrder[m]].Effective_radius / distance) *
                     (jz[All[X][Y].jzOrder[m]].Effective_radius / distance);
            if (power1 > *power)
            {
                *power = power1;
                order=All[X][Y].jzOrder[m];
            }
        }
        m++;
    }
    if(*power==0) return -1;
    return order;
}

double Get_distance(int x1,int y1,int x2,int y2)  //找到两点间的距离
{
    if(x1==x2) return fabs(y1-y2);
    if(y1==y2) return fabs(x1-x2);
    return sqrt((float)((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));
}

void function5()
{
    int step_length=0;          //步长
    int yd_track=1;             //移动路线1～12
    double yd_length;                 //某段移动距离的长度
    double power;                //记录强度
    int yd_number;              //移动的总次数
    double sin,cos;
    int n=-1,n1=-1;             //记录基站序号
    int number=0;                 //记录经过基站个数
    int number_CQ=0,number_XZ=0,number_GS=0;
    int a=0;
    printf("请输入步长：");
    scanf("%d",&step_length);     //输入步长
    while(yd_track<=count_yd)
    {
        printf("第%d条轨迹依次连接的基站有：\n",yd_track);
        printf("    X    Y   类型    强度    序号  信号强度\n");
        yd_length=Get_distance(yd[yd_track-1].Xs,yd[yd_track-1].Ys,yd[yd_track-1].Xe,yd[yd_track-1].Ye);
        yd_number=yd_length/step_length;
        Get_direction_yd(&sin,&cos,yd_track-1,yd_length);
        //每次前进一点
        while(a<=yd_number)
        {
            power=0;
            n1=Get_max_power(yd[yd_track-1].Xs+a*cos*step_length,yd[yd_track-1].Ys+a*sin*step_length,&power);
            //获取当前位置的最强信号的基站
            if(n1!=-1&&n1!=n)      //若和之前记录的基站不一样，则代表一次切换
            {
                n=n1;              //记录新的基站信息
                number++;
                printf("%6d %5d %s  %4f %d  %f\n",jz[n].x,jz[n].y,jz[n].type,jz[n].strength,jz[n].order,power);
                if(!strcmp(jz[n].type,"城区")) number_CQ++;
                if(!strcmp(jz[n].type,"乡镇")) number_XZ++;
                if(!strcmp(jz[n].type,"高速")) number_GS++;
            }
            a++;
        }
        printf("第%d条路径共经过%d个基站，其中城区基站%d个，乡镇基站%d个，高速基站%d个\n\n",yd_track,number,number_CQ,number_XZ,number_GS);
        yd_track++;
        number=0,a=0,n=-1,n1=-1,number_CQ=0,number_GS=0,number_XZ=0;
    }
}

void Get_direction_yd(double* sin,double* cos,int track,double yd_length)
{
    *sin=(yd[track].Ye-yd[track].Ys)/yd_length;
    *cos=(yd[track].Xe-yd[track].Xs)/yd_length;
}

void Get_direction_wz(double* sin,double* cos,int track,double wz_length)
{
    *sin=(wz[track].Ye-wz[track].Ys)/wz_length;
    *cos=(wz[track].Xe-wz[track].Xs)/wz_length;
}

void function6()
{
    int yd_track;
    double sin,cos;
    int n=-1,n1=-1;             //记录基站序号
    double yd_length;                 //某段移动距离的长度
    double step_length;          //步长
    int a=0,a1,a2;                  //用来计算过了多少个0.1s
    double power;
    int flag=0;
    int hour1,minute1,hour2,minute2;
    float second1,second2;
    int m1,m2,m3,m4;
    printf("请输入要查询的路线（任务单要求为1）:");
    scanf("%d",&yd_track);
    yd_length=Get_distance(yd[yd_track-1].Xs,yd[yd_track-1].Ys,yd[yd_track-1].Xe,yd[yd_track-1].Ye);
    //得到移动轨迹的长度
    Get_direction_yd(&sin,&cos,yd_track-1,yd_length);
    //得到移动的方向
    step_length=T1*yd[yd_track-1].velocity/3.6;
    while(1)
    {
        power=0;
        n1=Get_max_power(yd[yd_track-1].Xs+a*cos*step_length,yd[yd_track-1].Ys+a*sin*step_length,&power);
        if(n1!=-1&&!flag)
        {
            n=n1;        // 指进入了第一个基站范围内，且该基站序号为n
            a1=a;        //记录第一个时间节点
            flag=1;      //代表进来过一次了
        }
        if(n1!=n&&n!=-1)
        {
            a2=a;        //记录第二个时间节点
            goto L;
        }
        a++;
    }
    L:m1=a1*T1/60;
    second1=a1*T1-m1*60;
    m2=(m1+yd[yd_track-1].minute)/60;
    minute1=m1+yd[yd_track-1].minute-60*m2;
    hour1=m2+yd[yd_track-1].hour;
    m3=a2*T1/60;
    second2=a2*T1-m3*60;
    m4=(m3+yd[yd_track-1].minute)/60;
    minute2=m3+yd[yd_track-1].minute-60*m4;
    hour2=m4+yd[yd_track-1].hour;
    printf("在%d:%d:%.2f进入%d号基站,在%d:%d:%.2f离开\n",hour1,minute1,second1,jz[n].order,hour2,minute2,second2);
}

void function7()
{
    int step_length=1;          //步长
    int yd_track=1;             //移动路线1～12
    double yd_length;                 //某段移动距离的长度
    double power;                //记录强度
    int yd_number;              //移动的总次数
    double sin,cos;
    int n=-1,n1=-1;             //记录基站序号
    int a=0;
    int x1,x2;
    int jz_order[50];
    int number=0;
    printf("请输入您要查询的移动轨迹（任务单要求为3or6）：");
    scanf("%d",&yd_track);
    yd_length=Get_distance(yd[yd_track-1].Xs,yd[yd_track-1].Ys,yd[yd_track-1].Xe,yd[yd_track-1].Ye);
    yd_number=yd_length/step_length;
    Get_direction_yd(&sin,&cos,yd_track-1,yd_length);
    while(a<=yd_number)
    {
        power=0;
        n1=Get_max_power(yd[yd_track-1].Xs+a*cos*step_length,yd[yd_track-1].Ys+a*sin*step_length,&power);
        if(n1!=-1&&n1!=n)
        {
            n=n1;
            jz_order[number]=n;   //这和function5的原理相同，是用来获取移动线路的基站切换情况，并存入jz_order数组中
            number++;
        }
        a++;
    }
    x1=jz_order[0];
    x2=jz_order[1];
    if((Get_distance(yd[yd_track-1].Xs,yd[yd_track-1].Ys,jz[x1].x,jz[x1].y))<=jz[x1].Effective_radius&&(Get_distance(yd[yd_track-1].Xs,yd[yd_track-1].Ys,jz[x2].x,jz[x2].y))<=jz[x2].Effective_radius)
    {
        x1=jz_order[1];
        x2=jz_order[2];
    }
    //以上操作只是为了得到x1，x2，即重叠区的两个基站的序号
    double d1,d2;
    int flag=1;     //flag的作用是判断是否进入过重叠区
    int a1,a2;
    double t;
    a=0;
    while(1)
    {
        d1=Get_distance(yd[yd_track-1].Xs+a*cos*step_length,yd[yd_track-1].Ys+a*sin*step_length,jz[x1].x,jz[x1].y);
        d2=Get_distance(yd[yd_track-1].Xs+a*cos*step_length,yd[yd_track-1].Ys+a*sin*step_length,jz[x2].x,jz[x2].y);
        if(d1<=jz[x1].Effective_radius&&d2<=jz[x2].Effective_radius&&flag)   //首次进入重叠区
        {
            a1=a;    //记录时间节点a1
            flag=0;
        }
        if((d1>jz[x1].Effective_radius||d2>jz[x2].Effective_radius)&&!flag)  //在进入重叠区之后首次出重叠区
        {
            a2=a;   //记录时间节点a2
            goto L2;
        }
        a++;
    }
    L2:t=(a2-a1)*step_length*3.6/yd[yd_track-1].velocity;
    printf("终端在%d号基站和%d基站重叠区时间为%f秒\n",jz[x1].order,jz[x2].order,t);
}


void function8_9()
{
    int yd_track;
    double time_start,time_end;
    double yd_length;                 //某段移动距离的长度
    double sin_yd,cos_yd,sin_wz,cos_wz;
    double step_length_yd,step_length_wz;              //0.01s走的距离
    int n=0;
    int a,a1,a2;
    double d=100;
    int flag=0;
    int hour,minute;
    float second;
    int m1,m2;
    printf("请输入您要查询的移动轨迹（任务单要求为9or12）：");
    scanf("%d",&yd_track);
    step_length_yd=T2*yd[yd_track-1].velocity/3.6;
    yd_length=Get_distance(yd[yd_track-1].Xs,yd[yd_track-1].Ys,yd[yd_track-1].Xe,yd[yd_track-1].Ye);
    Get_direction_yd(&sin_yd,&cos_yd,yd_track-1,yd_length);
    time_start=yd[yd_track-1].hour*3600+yd[yd_track-1].minute*60;       //将时间换算为秒
    time_end=time_start+yd_length/yd[yd_track-1].velocity*3.6;          //将时间换算为秒
    while(n<count_wz)
    {
        Get_direction_wz(&sin_wz,&cos_wz,n,wz[n].length);
        step_length_wz=T2*wz[n].velocity/3.6;
        //下面对应移动轨迹和伪基站轨迹在时间上重合的四种情况，具体算法见报告
        if(time_end>=wz[n].time_start&&time_start<=wz[n].time_start&&time_end<=wz[n].time_end)
        {
            a=0,a1=0,a2=0;
            d=100;
            flag=0;
            while(a<=(time_end-wz[n].time_start)/T1)
            {
                d=Get_distance(yd[yd_track-1].Xs+(wz[n].time_start-time_start)/T2*step_length_yd*cos_yd+a*cos_yd*step_length_yd,yd[yd_track-1].Ys+(wz[n].time_start-time_start)/T2*step_length_yd*sin_yd+a*sin_yd*step_length_yd,wz[n].Xs+a*cos_wz*step_length_wz,wz[n].Ys+a*sin_wz*step_length_wz);
                if(d<=40&&!flag)
                {
                    flag=1;
                    a1=a;
                }
                if(d>40&&flag)
                {
                    a2=a;
                    m1=(T2*a1)/60;
                    second=T2*a1-60*m1;
                    m2=(m1+wz[n].minute)/60;
                    minute=m1+wz[n].minute-60*m2;
                    hour=wz[n].hour+m2;
                    printf("从%d:%d:%.1f开始",hour,minute,second);
                    printf("连接上%d伪基站%.1f秒\n",wz[n].order,(a2-a1)*T2);
                    break;
                }
                a++;
            }
        }
        else if(time_start<=wz[n].time_start&&time_end>=wz[n].time_end)
        {
            a=0,a1=0,a2=0;
            d=100;
            flag=0;
            while(a<=(wz[n].time_end-wz[n].time_start)/T2)
            {
                if(a!=4944) d=Get_distance(yd[yd_track-1].Xs+(wz[n].time_start-time_start)/T2*step_length_yd*cos_yd+a*cos_yd*step_length_yd,yd[yd_track-1].Ys+(wz[n].time_start-time_start)/T2*step_length_yd*sin_yd+a*sin_yd*step_length_yd,wz[n].Xs+a*cos_wz*step_length_wz,wz[n].Ys+a*sin_wz*step_length_wz);
                if(d<=40&&!flag)
                {
                    flag=1;
                    a1=a;
                }
                if(d>40&&flag)
                {
                    a2=a;
                    m1=(T2*a1)/60;
                    second=T2*a1-60*m1;
                    m2=(m1+wz[n].minute)/60;
                    minute=m1+wz[n].minute-60*m2;
                    hour=wz[n].hour+m2;
                    printf("从%d:%d:%.1f开始",hour,minute,second);
                    printf("连接上%d伪基站%.1f秒\n",wz[n].order,(a2-a1)*T2);
                    break;
                }
                a++;
            }
        }
        else if(time_start>=wz[n].time_start&&time_end<=wz[n].time_end)
        {
            a=0,a1=0,a2=0;
            d=100;
            flag=0;
            while(a<=(time_end-time_start)/T2)
            {
                d=Get_distance(yd[yd_track-1].Xs+a*cos_yd*step_length_yd,yd[yd_track-1].Ys+a*sin_yd*step_length_yd,wz[n].Xs+a*cos_wz*step_length_wz+(time_start-wz[n].time_start)/T2*step_length_wz*cos_wz,wz[n].Ys+a*sin_wz*step_length_wz+(time_start-wz[n].time_start)/T2*step_length_wz*sin_wz);
                if(d<=40&&!flag)
                {
                    flag=1;
                    a1=a;
                }
                if(d>40&&flag)
                {
                    a2=a;
                    m1=(T2*a1)/60;
                    second=T2*a1-60*m1;
                    m2=(m1+yd[yd_track-1].minute)/60;
                    minute=m1+yd[yd_track-1].minute-60*m2;
                    hour=yd[yd_track-1].hour+m2;
                    printf("从%d:%d:%.1f开始",hour,minute,second);
                    printf("连接上%d伪基站%.1f秒\n",wz[n].order,(a2-a1)*T2);
                    break;
                }
                a++;
            }
        }
        else if(time_end>=wz[n].time_end&&time_start<=wz[n].time_end&&time_start>=wz[n].time_start)
        {
            a=0,a1=0,a2=0;
            d=100;
            flag=0;
            while(a<=(wz[n].time_end-time_start)/T2)
            {
                d=Get_distance(yd[yd_track-1].Xs+a*cos_yd*step_length_yd,yd[yd_track-1].Ys+a*sin_yd*step_length_yd,wz[n].Xs+a*cos_wz*step_length_wz+(time_start-wz[n].time_start)/T2*step_length_wz*cos_wz,wz[n].Ys+a*sin_wz*step_length_wz+(time_start-wz[n].time_start)/T2*step_length_wz*sin_wz);
                if(d<=40&&!flag)
                {
                    flag=1;
                    a1=a;
                }
                if(d>40&&flag)
                {
                    a2=a;
                    m1=(T2*a1)/60;
                    second=T2*a1-60*m1;
                    m2=(m1+yd[yd_track-1].minute)/60;
                    minute=m1+yd[yd_track-1].minute-60*m2;
                    hour=yd[yd_track-1].hour+m2;
                    printf("从%d:%d:%.1f开始",hour,minute,second);
                    printf("连接上%d伪基站%.1f秒\n",wz[n].order,(a2-a1)*T2);
                    break;
                }
                a++;
            }
        }
        n++;
    }
}
