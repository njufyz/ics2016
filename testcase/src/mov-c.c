#include "trap.h"

int A[3][2];
int B[6]={0,1,1,2,2,3};
int main() {
/*	A[0][0] = 0;
	A[0][1] = 1;
	A[0][2] = 2;
	A[1][3] = 3;
	A[1][4] = 4;
    A[1][5] = 3;
//	b = A[3];
//	A[5] = b;
	b = A[1][4];
	nemu_assert(A[0][0] == 0);
	nemu_assert(A[0][1] == 1);
	nemu_assert(A[0][2] == 2);
	nemu_assert(A[1][3] == 3);
	nemu_assert(A[1][4] == 4);
	nemu_assert(b == 4);
	nemu_assert(A[1][5] == 3);
*/
  int i,j;
  i=j=0;
  int b = 0;
  for(;i<3;i++)
      for(;j<2;j++)
        {
            A[i][j]=i+j;
            nemu_assert(A[i][j]==B[b++]);
        }
	return 0;
}
