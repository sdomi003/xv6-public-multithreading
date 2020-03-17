#include "types.h"
#include "user.h"


#define N 21
int *sem;

struct matrices
{
	int mat1[N][N];
	int mat2[N][N];
	int result[N][N];
	int i;
};

void test4(void *arg)
{
	struct matrices *mat = (struct matrices*)arg;
	
	int j, k, temp;
	
	for (j=0; j< N; j++)
	{
		for (k=0; k< N; k++)
		{
			temp = mat->mat1[j][k] * mat->mat2[k][mat->i];
			mat->result[mat->i][j] += temp;
		}
	}
	exit();
}

int main(int argc, char **argv)
{	 
  printf(0,"test matrix multiplication: setting up\n");

  struct matrices *mat[N];
  int *mstack[N];
  int result_1[N][N], result_2[N][N];
  
  int i, j, k, num;
  
  for(i=0; i < N; i++)
  {
	  mat[i] = malloc(sizeof(struct matrices));
  }
  
  for(i=0; i < N; i++)
  {
	  for(j=0; j < N; j++)
	  {
		  for(k=0; k < N; k++)
		  { 
			  mat[k]->mat1[i][j] = i + j + 1;
			  mat[k]->mat2[i][j] = i * j + 1;
			  mat[k]->result[i][j] = 0;
		  }
	  }
  }
  
  printf(0,"ready\n");
  printf(0,"running single process\n");
  for(i=0; i < N; i++)
  {
	  for(j=0; j < N; j++)
	  {
		  for(k=0; k < N; k++)
		  {
			  result_1[i][j] += mat[0]->mat1[j][k] * mat[0]->mat2[k][i];
		  }
	  }
  }
  printf(0,"single process complete, time = %d\n");
  
  num = 0;
  printf(0,"running multi threaded\n");
  
  for(i=0; i < N; i++)
  {
	 mat[i]->i = i;
	 mstack[i] = malloc(4096);
	 kthread_create(&test4,mat[i],mstack[i]);
	 num++;
  }
	  
  for (k=0; k < N; k++)
  {
	  kthread_join(0);
  }
  
  for (i=0; i< N; i++)
  {
	for (j=0; j < N; j++)
	{
		result_2[j][i] = mat[j]->result[j][i];
	}
  }
  
  for (j=0;j < N; j++)
  {
	 free(mstack[j]);
  }
  
  printf(0,"multi threaded complete\n");
  printf(0,"number of threads = %d\n",num);
  
  for(i=0; i < N; i++)
  {
	  for(j=0; j < N; j++)
	  {
		  //printf(0,"result_1[%d][%d] = %d\n",i,j, result_1[i][j]); 
		  //printf(0,"result_2[%d][%d] = %d\n",i,j, result_2[i][j]); 
		  if (result_1[i][j] != result_2[i][j])
		  { 
			  printf(0,"FAIL due to wrong results\n"); 
			  
			  printf(0,"location [%d][%d] = %d, it should = %d\n",i,j,result_2[i][j],result_1[i][j]); 
			  
			  
			  for(k=0; k < N; k++)
			  {
				  free(mat[k]);
			  }
			  
			  exit();
		  }
	  }
  }
  printf(0,"test matrix multiplication: PASSED\n");
  
  for(i=0; i < N; i++)
  {
	  free(mat[i]);
  }
  exit();
}
