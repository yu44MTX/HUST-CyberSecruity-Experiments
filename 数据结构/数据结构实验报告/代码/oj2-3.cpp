#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAXVEX 21
#define INFINITY 65535
//���ĸ���������ɴ˹ؿ� 
typedef struct Node          //˫������ڵ� 
{
	char name[20];
	double distance;
	struct Node* pre; 
	struct Node* next;
}Node,*NodePtr;

typedef struct  DoubleLinklist     //˫��ѭ������ 
{
	int line;                //������·�� 
	NodePtr head;
	NodePtr end;
}DoubleLinklist,*DoubleLinklistPtr;

typedef char VertexType[20];             //��������Ϊ�ַ�������ΪҪ����վ�� 
typedef float EdgeType;
typedef struct                         //���ڽӾ�����ͼ 
{
	VertexType vexs[MAXVEX];
	EdgeType arc[MAXVEX][MAXVEX];
	int numVertexes,numEdges;	 
}MGraph;

typedef int Patharc[MAXVEX];                    //���ڴ洢���·���±������
typedef float ShortPathTable[MAXVEX];          //���ڴ洢���������·����Ȩֵ�� 

DoubleLinklistPtr CreatDoubleLinklist(int line)        //���������·�� ,����һ���յ�˫������ 
{
	DoubleLinklistPtr q=(DoubleLinklistPtr)malloc(sizeof(DoubleLinklist));
	q->line=line;
	NodePtr head=(NodePtr)malloc(sizeof(Node));
	head->distance=0;
	head->next=NULL;
	head->pre==NULL;
	q->head=head; 
	q->end=head;
	return q;
}

void AddNode(DoubleLinklistPtr q,char name[20],double distance)     //˫�������м���ڵ� 
{
	NodePtr p=(NodePtr)malloc(sizeof(Node));
	strcpy(p->name,name);
	p->distance=distance;
	q->end->next=p;
	p->pre=q->end;
	p->next=q->head;
	q->head->pre=p;
	q->end=p;
}

void read(DoubleLinklistPtr q)            //�����ݶ���q 
{
	char name[20];
	double distance;
	while(1)
	{
		scanf("%s %lf",name,&distance);
		AddNode(q,name,distance);          //����ڵ� 
		if(distance==0) break;
	}
}

void transfer(int* i,int* j,char a[],char b[],VertexType vexs[])  //��a��b�ڶ�����е��±��ҵ�������i��j���� 
{
	int k=0;
	for(k=0;k<MAXVEX;k++)
	{
		if(!strcmp(a,vexs[k])) *i=k;
		if(!strcmp(b,vexs[k])) *j=k;
	} 
}

void CreatMGraph(MGraph *G,DoubleLinklistPtr q[])         //���ĸ�������һ��ͼ 
{
	G->numEdges=0;
	G->numVertexes=0;
	int i,j,k;
	int flag=1;                 //flag=0���ʾ֮ǰ�����վ�ˣ����ü��붥��� 
	//��������� 
	for(i=0;i<MAXVEX;)        
	{
		for(j=0;j<4;j++)         //���ν��ĸ�������� 
		{
			NodePtr head=q[j]->head;
			NodePtr temp=q[j]->head->next;
			while(temp!=head)
			{
				for(k=0;k<G->numVertexes;k++)
				{
					if(!strcmp(G->vexs[k],temp->name))
					{
						flag=0; 
						break;
					} 
				}
				if(flag==1)                  //֮ǰû����ȵ�վ 
				{
					strcpy(G->vexs[i],temp->name);
					G->numVertexes++;
					i++;
				}
				temp=temp->next;
				flag=1;
			}
		} 
	}
	//��ʼ���ڽӾ���
	for(i=0;i<G->numVertexes;i++)
	{
		for(j=0;j<G->numVertexes;j++)
		{
			G->arc[i][j]=INFINITY;	
		}	
	}
	//�����ڽӾ���
	for(k=0;k<4;k++)        //���ĸ�����ı��ǹ�ϵ�����ڽӾ����� 
	{
		NodePtr head=q[k]->head;
		NodePtr temp=q[k]->head->next;
		while(temp->next!=head) 
		{
			transfer(&i,&j,temp->name,temp->next->name,G->vexs);
			G->arc[i][j]=temp->distance;
			G->arc[j][i]=temp->distance;
			G->numEdges++;
			temp=temp->next;
		}
	}	
}

void ShortestPath_Dijkstra(MGraph G, int v0, Patharc *P, ShortPathTable *D)
{    
	int v,w,k,min;    
	int final[MAXVEX];                        // final[w]=1��ʾ��ö���v0��vw�����·�� 
	for(v=0; v<G.numVertexes; v++)             //��ʼ������ 
	{        
		final[v] = 0;		             	//ȫ�������ʼ��Ϊδ֪���·��״̬ 
		(*D)[v] = G.arc[v0][v];             //����v0�������ߵĶ������Ȩֵ 
		(*P)[v] = -1;			          	//��ʼ��·������PΪ-1   
	}

	(*D)[v0] = 0;                         //v0��v0·��Ϊ0 
	final[v0] = 1;                         // v0��v0����Ҫ��·��        
    //��ʼ��ѭ����ÿ�����v0��ĳ��v��������·��  
	for(v=1; v<G.numVertexes; v++)   
	{
		min=INFINITY;                          //��ǰ��֪��v0������������       
		for(w=0; w<G.numVertexes; w++)           //Ѱ����v0����Ķ���    
		{            
			if(!final[w] && (*D)[w]<min)             
			{                   
				k=w;                    
				min = (*D)[w];    //w������v0�������             
			}        
		}        
		final[k] = 1;                                //��Ŀǰ�ҵ�������Ķ�����Ϊ1 
		for(w=0; w<G.numVertexes; w++)               // ������ǰ���·�������� 
		{
			//�������v�����·������������·���ĳ��ȶ̵Ļ� 
			if(!final[w] && (min+G.arc[k][w]<(*D)[w]))   
			{                                            // ˵���ҵ��˸��̵�·�����޸�D[w]��P[w] 
				(*D)[w] = min + G.arc[k][w];             //�޸ĵ�ǰ·������                
				(*P)[w]=k;        
			}       
		}   
	}
}
//Dijkstra�㷨���õ��Ľ��Ϊ��
//����a���±꣩�����ඥ��v�����·��P[v]�Լ���Ȩ����D[v]
// P[v]��ֵΪǰ�������±꣬D[v]��ʾa��v�����·��֮�� 

void InvertedSequence(int a[],int n)           //����n��Ԫ�ص������������� 
{
	int temp=0;
	int i=0;
	for(;i<n-1;i++,n--)
	{
		temp=a[i];
		a[i]=a[n-1];
		a[n-1]=temp;	
	}	
} 

int main()
{	
	int n,line;              //��n����·,��·����Ϊline 
	scanf("%d",&n); 
	DoubleLinklistPtr q[n];       
	int i=0;                //i����һ������������ 
	while(i<n)
	{
		scanf("%d",&line);
		q[i]=CreatDoubleLinklist(line);      //���ν�n����·����n��˫�������� 
		read(q[i]); 
		i++;
	}
	char origin[20],terminal[20];           //�����յ� 
	scanf("%s%s",origin,terminal);
	MGraph G;
	CreatMGraph(&G,q);
	int a,b;                  //��a��b������ʼ���յ��Ӧ�������±� 
	Patharc P;                //���ڴ洢���·���±������
	ShortPathTable D;         //���ڴ洢���������·����Ȩֵ��
	transfer(&a,&b,origin,terminal,G.vexs);   
	ShortestPath_Dijkstra(G,a,&P,&D);
	printf("%.2f ",D[b]);         //����b����·����� 
	int m=0;
	int b_temp1=b;                //b��ֵ��Ҫ�������������Ժ�������Ҫ�õ�b�ĵط����ֱ���b_temp1��b_temp2 
	int b_temp2=b; 
	while(P[b_temp1]!=-1)                //�ж��м侭���˼���վ�� ,��¼��m�� 
		{
			m++;
			b_temp1=P[b_temp1];
		}
	int index[m];
	i=0; 
	while(P[b_temp2]!=-1)              //��i��վ���index�������������   
		{
			index[i]=P[b_temp2];
			b_temp2=P[b_temp2];
			i++;
		}
	InvertedSequence(index,m);      //���������򣬱��������˳�� 
	printf("%s %.2f ",origin,G.arc[a][index[0]]);
	for(i=0;i<m-1;i++)
	{
		printf("%s %.2f ",G.vexs[index[i]],G.arc[index[i]][index[i+1]]);
	}
	printf("%s %.2f ",G.vexs[index[m-1]],G.arc[index[m-1]][b]);
	printf("%s",terminal);
	return 0;
}
