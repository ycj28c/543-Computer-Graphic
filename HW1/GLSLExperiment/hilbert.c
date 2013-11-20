#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include "Angel.h"

void Initial(int* x, int, int);
void Hilbert(int**, int ,int, int);
//direction: 0=0; 1=90 2=180 3=270
void Rot(int* x, int, int, int);

int main(int argc, char* argv[])
{

  int * x = NULL;
  int row = 2;
  int col = 2;
  int i, j;
  int layer = 3;

  if(argc >= 2)
    layer = atoi(argv[1]);

  x = (int*)malloc(row * col * sizeof(int));
  memset(x, 0, sizeof(int) * row * col);

  Initial(x, 2, 2);

  Hilbert(&x, row, col, layer);

  printf("Hilbert list:\n");
  for(i = 0;i < row * pow(2, layer - 1);i ++)
  {
    for(j = 0;j < col * pow(2, layer - 1);j ++)
      printf("%d \t", x[i * col * (int)pow(2, layer - 1) + j]);
    printf("\n");
  }
  printf("\nEnd Hilbert layer = %d.\n", layer);

  free(x);
  x = NULL;
  
  getchar();

  return 0;
}

void Initial(int* x, int row, int col)
{
  x[0] = 2;
  x[1] = -1;
  x[2] = 3;
  x[3] = 1;
}

void Rot(int* x, int row, int col, int direction)
{
  int i, j;
  int* y = NULL;
  int* z = NULL;
  int temp;
  if(direction == 0)
    return;

  y = (int*)malloc(row * col * sizeof(int)); 
  z = (int*)malloc(row * col * sizeof(int));

  printf("Before Rot %d row %d col %d: ", direction, row, col);
  for(i = 0;i < row;i ++)
    for(j = 0;j < col;j ++)
    {
      printf("%d ", *(x + i * col + j));
    }
  printf("\n");

  for(i = 0;i < row;i ++)
    for(j = 0;j < col;j ++)
    {
      if(direction == 1)
      {
        *(z + i * col + j) = j * col + i;
      }
      else if(direction == 2)
      {

      }
      else if(direction == 3)
      {
        *(z + i * col + j) = (row - j - 1) * col + col - i - 1;
      }
      else
        break;      
    
    }

  for(i = 0;i < row;i ++)
    for(j = 0;j < col;j ++)
    {
        temp = *(x + i * col + j);
        if(temp == -1)
          *(y + *(z + i * col + j)) = -1;          
        else
          *(y + *(z + i * col + j)) = *(z + temp);
    }

  memcpy(x, y, row * col * sizeof(int));

  printf("After Rot %d row %d col %d: ", direction, row, col);
  for(i = 0;i < row;i ++)
    for(j = 0;j < col;j ++)
    {
      printf("%d ", *(x + i * col + j));
    }
  printf("\n");

  free(y);
  free(z);
  z = NULL;
  y = NULL;
  
}

void Hilbert(int** px, int row, int col, int layer)
{
  int * y = NULL;
  int * z = NULL;
  int * t;
  int i, j;
  int c, r;
  layer --;
  if(layer <= 0)
    return;

  y = (int *)malloc(row * col * 4 * sizeof(int));
  z = (int *)malloc(row * col * sizeof(int));

  memset(y, 0, row * col * 4 * sizeof(int));

  memcpy(z, *px, row * col * sizeof(int));
  Rot(z, row, col, 1);
  for(i = 0;i < row;i ++)
    for(j = 0;j < col;j ++)
    {
      r = (int)(*(z + i * col + j) / col);
      c = *(z + i * col + j) % col;
      *(y + i * col * 2 + j) = r * col * 2 + c;
    }

  memcpy(z, *px, row * col * sizeof(int));
  Rot(z, row, col, 0);
  for(i = 0;i < row;i ++)
    for(j = 0;j < col;j ++)
    {
      r = (int)(*(z + i * col + j) / col);
      c = *(z + i * col + j) % col;
      *(y + (i + row) * col * 2 + j) = r * col * 2 + row * col * 2 + c;
    }

  memcpy(z, *px, row * col * sizeof(int));
  Rot(z, row, col, 3);
  for(i = 0;i < row;i ++)
    for(j = 0;j < col;j ++)
    {
      r = (int)(*(z + i * col + j) / col);
      c = *(z + i * col + j) % col;
      *(y + i * col * 2 + j + col) = r * col * 2 + col + c;
    }

  memcpy(z, *px, row * col * sizeof(int));
  Rot(z, row, col, 0);
  for(i = 0;i < row;i ++)
    for(j = 0;j < col;j ++)
    {
      r = (int)(*(z + i * col + j) / col);
      c = *(z + i * col + j) % col;
      *(y + (i + row) * col * 2 + j + col) = r * col * 2 + row * col * 2 + c + col;
    }

  *(y + row * col * 2 - col * 2) = row * col * 2;
  *(y + row * col * 2 + col * 2 - 1) = row * col * 2 - 1;
  *(y + row * col * 2 + col - 1) = row * col * 2 + col;
  *(y + col * 2 - 1) = -1;

  t = *px;
  *px = y;
  y = t;
 
  Hilbert(px, row * 2, col * 2, layer);

  free(y);
  free(z);

}
