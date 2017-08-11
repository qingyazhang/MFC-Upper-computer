#include<stdio.h> 
#include<math.h> 
#define MAX 100 
void main() 
{
	int i,j,k,m,N,mi; //i,j,k,mi;
					 //m是多项式次数;N是数据组数
	float tmp,mx,SSE; 
	float X[MAX][MAX],Y[MAX],x[MAX],y[MAX],a[MAX]; 
	//X[MAX][MAX]是矩阵乘数因子
	//Y[MAX]是矩阵因变量,x[MAX]
	//x[MAX],y[MAX]是中间变量
	//a[MAX]是多项式系数，从零次到高次;
	printf("\n 输入拟合多项式的次数:\n");  
	scanf("%d",&m); 
	printf("\n 输入给定点的个数n及坐标(x,y):\n"); 
	scanf("%d",&N); 
	if (m>N)
	{
		printf("\n多项式次数大于给定点，错误！\n");//m
	} 
	else
	{
		//printf("\n"); 
		for(i=0;i<N;i++)  
		{			
			scanf("%f %f",&x[i],&y[i]);
		}
		for(i=0;i<=m;i++) //求矩阵乘数因子
		{ 
			for(j=i;j<=m;j++)  
			{  
				tmp=0;  
				for(k=0;k<N;k++) 
					tmp=tmp+pow(x[k],(i+j)); 
				X[i][j]=tmp;  
				X[j][i]=X[i][j]; 
			}  
		} 
		for(i=0;i<=m;i++) //求矩阵因变量
		{  
			tmp=0; 
			for(k=0;k<N;k++)  
			{
				tmp=tmp+y[k]*pow(x[k],i); 
			}
			Y[i]=tmp; 
		} 
		for(j=0;j<m;j++) //求矩阵因变量和系数的增广矩阵
		{ 
			for(i=j+1,mi=j,mx=fabs(X[j][j]);i<=m;i++)  
				if(fabs(X[i][j])>mx) 
				{  
					mi=i;  
					mx=fabs(X[i][j]);  
				} 
				if(j<mi) //矩阵X和Y改变行号
				{   
					tmp=Y[j];
					Y[j]=Y[mi]; 
					Y[mi]=tmp;  
					for(k=j;k<=m;k++) 
					{  
						tmp=X[j][k]; 
						X[j][k]=X[mi][k];  
						X[mi][k]=tmp; 
					}
				}  
				for(i=j+1;i<=m;i++)  //矩阵变换
				{  
					tmp=-X[i][j]/X[j][j]; 
					Y[i]+=Y[j]*tmp; 
					for(k=j;k<=m;k++) 
					{
						X[i][k]+=X[j][k]*tmp; 
					}
				}
		}  
		a[m]=Y[m]/X[m][m]; 
		for(i=m-1;i>=0;i--) 
		{  
			a[i]=Y[i];
			for(j=i+1;j<=m;j++)
			{
				a[i]-=X[i][j]*a[j];
			}
			a[i]/=X[i][i]; 
		}
		printf("\n 所求的二次多项式为	:\n"); 
		printf("P(x)=%f",a[0]); 
		for(i=1;i<=m;i++) 
		{
			printf("+(%f)*x^%d:\n",a[i],i);		
		}
		//求该拟合函数的SSE；m
		for (i=0,SSE=0;i<N;i++)
		{
			for (j=0,tmp=0;j<=m;j++)
			{		
				tmp+=a[j]*pow(x[i],j);
			}
			SSE+=y[i]*y[i]-tmp*tmp;
		}
		printf("SSE: %f \n",SSE);	
		printf("SSE: %f \n",SSE);	
	} 
	return ;
}
