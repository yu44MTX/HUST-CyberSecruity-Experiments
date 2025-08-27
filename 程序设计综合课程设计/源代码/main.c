#include"stdio.h"
#include"math.h"
int gcd(int m,int n){
    int c;
    c=m%n;
    while(c){
        m=n;
        n=c;
        c=m%n;
    }
    return n;
}
int f(int x){
    int i = 0;
    for (int j = 1; j <=x; ++j) {
        if(gcd(j,x)==1) i++;
    }return i;
}

int main(){
    int N,n,c,d,i=0;
    scanf("%d%d",&N,&n);
    int a[N];
    int b[N];
    while(i<N)
    {
        scanf("%d",&a[i]);
        i++;
    }
    i=0;
    while(i<N)
    {
        if(i==0) b[i]=n;
        else b[i]=f(b[i-1]);
        i++;
    }
    c=N-1;
    while(c>=0)
    {
        if(c!=0) a[c]=a[c]%b[c]+b[c];
        if(c==0)
        {
            a[0]=a[0]%b[0];
            while(a[0]<0)
            {
                a[0]+=b[0];
            }
            printf("%d",a[0]);
            return 0;
        }
        a[c-1]=pow(a[c-1],a[c]);
        c--,d--;
    }
}
