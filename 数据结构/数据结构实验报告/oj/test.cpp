 //Your code begin.
//示例仅供参考，你也可以??修改设计。
////2.1
//
//#include<stdio.h>
//#include<stdlib.h>
//#include<string.h>
//typedef struct node{
// float x;
// char *s;
// struct node *next;
//}lnode ,*list;
//int ls[100];
//int len[100];
//char str[10][100][100];
//int n,m;
//float num;
//int main(){
// scanf("%d",&n);
// list l[n];
// for (int i=0;i<n;i++)
// l[i] = (list)malloc(sizeof(lnode));
// for(int i=0;i<n;i++){
// scanf("%d",&ls[i]);
// int cnt = 0;
// list p=NULL;
// p = l[i];
// while (1) {
// scanf("%s%f",str[i][cnt],&num);
// p->s=str[i][cnt];
// cnt++;
// if(num==0){break;}
// p->x=num;
// list new;
// new =(list)malloc(sizeof(lnode));
// p->next = new;
// p = p->next;
// }
// len[i]=cnt;
// }
// for(int i=0;i<n;i++){
// printf("%d ",ls[i]);
// list k = l[i];
// for(int j=0;j<len[i]-1;j++){
// printf("%s ",k->s);
// printf("%.2f ",k->x);
// k=k->next;
// }
// printf("%s\n",k->s);
// }
//}
//
//
//#include<stdio.h>
//#include<stdlib.h>
//#include<string.h>
//typedef struct node{
// float x;
// char *s;
// struct node *next;
//}lnode ,*list;
//int ls[100];
//int len[100];
//char str[10][100][100];
//int n,m;
//float num;
//int main(){
// scanf("%d",&n);
// list l[n];
// for (int i=0;i<n;i++)
// l[i] = (list)malloc(sizeof(lnode));
// for(int i=0;i<n;i++){
// scanf("%d",&ls[i]);
// int cnt = 0;
// list p=NULL;
// p = l[i];
// while (1) {
// scanf("%s%f",str[i][cnt],&num);
// p->s=str[i][cnt];
// cnt++;
// if(num==0){break;}
// p->x=num;
// list new;
// new =(list)malloc(sizeof(lnode));
// p->next = new;
// p = p->next;
// }
// len[i]=cnt;
// }
// for(int i=0;i<n;i++){
// printf("%d ",ls[i]);
// list k = l[i];
// for(int j=0;j<len[i]-1;j++){
// printf("%s ",k->s);
// printf("%.2f ",k->x);
// k=k->next;
// }
// printf("%s\n",k->s);
// }
// int flag=1;
// int mum=0;
// float x1=0,x2=0;
//list ed;
//ed =(list)malloc(sizeof(lnode));
// char ptr[100];
// char ssr1[100];
// char ssr2[100];
// scanf("%s",ptr);
// if(ptr[0]=='a')
//{
// scanf("%d",&mum);
// for(int i=0;i<n;i++){
// if(mum==ls[i]){
// scanf("%f%f",&x1,&x2);
// if(x1)
// {scanf("%s",ssr1);
// scanf("%s",ssr2);}
// else {
// scanf("%s",ssr1);
// }
// list k1 =l[i];
// list k2 =k1;
// list k3 =k1;
////针对于已经有了同名站点
// for(int j=0;j<len[i];j++){
// if(strcmp(ssr2, k2->s)==0){
// printf("增加失败，已有同名站点");
// return 0;
// }
// k2 = k2->next;
// }
////针对于没有同名前驱站点
// for(int j=0;j<len[i]-1;j++){
// if(x1==0){flag=0;break;}
// if(strcmp(ssr1,k3->s)==0){
// flag=0;
// break;
// }
// k3=k3->next;
// }
// if(flag){
// printf("增加失败，没有与输?的增加位置前?站点同名的站点");
// return 0;
// }
////针对于正常的添加，开头?0；
// if(x1){
// for(int j=0;j<len[i]-1;j++){
// if(strcmp(ssr1,k1->s)==0){
// k1->x=x1;
// list lk = k1->next;
// list new;
// new = (list)malloc(sizeof(lnode));
// k1->next=new;
// k1 = new;
// k1->x=x2;
// k1->s=ssr2;
// k1->next = lk;
//
//
// printf("%d ",mum);
// list km=l[i];
// for(int q=0;q<len[i];q++){
// printf("%s ",km->s);
// printf("%.2f ",km->x);
// km=km->next;
// }
// printf("%s\n",km->s);
// }
//
// k1=k1->next;
// }}
////这是开头是0的情况；
// else
// { list lk=NULL;
// list new;
// new = (list)malloc(sizeof(lnode));
// lk=new;
// lk->next =k1;
// lk->s=ssr1;
// lk->x=x2;
// printf("%d ",mum);
// list km=lk;
// for(int q=0;q<len[i];q++){
// printf("%s ",km->s);
// printf("%.2f ",km->x);
// km=km->next;
// }
// printf("%s\n",km->s);
// }
//
// }
// }
//}
// else{
// scanf("%d",&mum);
// scanf("%s",ssr1);
// for(int i=0;i<n;i++){
// int flag=1;
// if(mum==ls[i]){
// list k1=l[i];
// list k2=k1;
// list k3 = k1;
//
// for(int j=0;j<len[i]-1;j++){
// if(strcmp(ssr1,k2->s)==0){
// flag=0;
// break;
// }
// k2=k2->next;
// }
// if(flag){
// printf("删除失败，没有同名站点");
// return 0;
// }
//
// for(int j=0;j<len[i]-1;j++){
// if(strcmp(ssr1,k1->s)==0){
// float tem1 =k1->x;
// while (k3->next!=k1) {
// k3=k3->next;
// }
// float tem2 =k3->x;
// float ans =tem1+tem2;
// k3->x = ans;
// k3->next = k1->next;
//
// list km=l[i];
// printf("%d ",mum);
// for(int q=0;q<len[i]-2;q++){
// printf("%s ",km->s);
// printf("%.2f ",km->x);
// km=km->next;
// }
// printf("%s\n",km->s);
//
//
// }
// k1=k1->next;
// }
// }
// }
// }
//
// return 0;
//}
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct node{
 float x;
 char *s;
 int spot;
 struct node *next;
 struct node *pre;
}lnode ,*list;
int s1[100];int d1[100];
int ls[100];
int len[100];
char str[10][100][100];
int n,m;
int my_l;
float num;
int spt;

int main(){
 scanf("%d",&n);
 list l[n];
 for (int i=0;i<n;i++)
 l[i] = (list)malloc(sizeof(lnode));
 for(int i=0;i<n;i++){
 scanf("%d",&ls[i]);
 int cnt = 0;
 list p=NULL;
 list pp=NULL;
 p = l[i];
 while (1) {
 scanf("%s%f",str[i][cnt],&num);
 p->spot = spt;spt++;//确定站数，?便?较
 p->s=str[i][cnt];
 cnt++;
 if(num==0){break;}
 p->x=num;
 list new=(list)malloc(sizeof(lnode));
 p->next = new;
 pp=p;
 p = p->next;
 p->pre=pp;
 p->next = NULL;
 
 }
 len[i]=cnt;
 spt = 0;//归零
 }
// for(int i=0;i<n;i++){
// printf("%d ",ls[i]);
// list k = l[i];
// for(int j=0;j<len[i]-1;j++){
// printf("%s ",k->s);
// printf("%.2f ",k->x);
// k=k->next;
// }
// printf("%s\n",k->s);
// }
 int abc[100];
 int nba[100];
 char ssr1[100];
 char ssr2[100];
 scanf("%s",ssr1);
 scanf("%s",ssr2);
 list f1[100];list f2[100];
 int num1=0;int num2=0;
 for(int i=0;i<n;i++){
 list lsp =l[i];
 while(lsp){
 if(strcmp(ssr1, lsp->s)==0){
 f1[i]=lsp;
 abc[num1]=i;
 num1++;
 }
 if(strcmp(ssr2, lsp->s)==0){
 f2[i]=lsp;
 nba[num2]=i;
 num2++;
 
 }
 lsp=lsp->next;
 }
 }
// printf("%d",num1);
// puts("");
// printf("%d",num2);
 //遍历所有从A到B的情况
 int zflag = 0;
 float answer[100];int ansx = 0;
 for(int i=0;i<num1;i++){
 for(int j=0;j<num2;j++){
// list k0 = f1[abc[i]];
// list k1 = f2[nba[j]];
 if(abc[i]==nba[j]){
//从前?站到后?站
 my_l = abc[i];
 list k0 = f1[abc[i]];
 list k1 = f2[nba[j]];
 if(k0->spot<k1->spot)
 {while (k0!=k1) {
 answer[ansx]+=k0->x;
 k0=k0->next;
 }
 ansx++;}
//从后?站到前?站
 else {zflag =1;
 list k1 = f1[abc[i]];
 list k0 = f2[nba[j]];
 while (k1!=k0) {
 answer[ansx]+=k1->x;
 k1=k1->pre;
 }
 answer[ansx]+=k1->x;
 ansx++;
 }
 
 }
 
 }
 }
//冒泡排序得出最短路径
 float temp=0;
 for (int i=0;i<ansx-1;i++){
 for(int j=0;j<ansx;j++){
 if(answer[i]>answer[j]){
 temp =answer[i];
 answer[i]=answer[j];
 answer[j]=temp;
 }
 }
 }
 printf("%.2f ",answer[0]);
//倒着输出
 if(zflag){
 list k1=f1[my_l];
 list k0 =f2[my_l];
 while (k1!=k0) {
 printf("%s ",k1->s);
 k1=k1->pre;
 printf("%.2f ",k1->x);
 }
 printf("%s",k1->s);
 }
////正着输出
 
else
 {
 list k0=f1[my_l];
 list k1 =f2[my_l];
 while (k0!=k1) {
 printf("%s %.2f ",k0->s,k0->x);
 k0=k0->next;
 }
 printf("%s",k0->s);
}
 
 return 0;
}
//Your code end.
