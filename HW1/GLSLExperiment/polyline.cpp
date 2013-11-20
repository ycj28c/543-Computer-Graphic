#include "stdio.h"
#include "string.h"
#include "malloc.h"
#include "Angel.h"

void readin(char* filename);
void drawPolyLineFile(char* );
void setWindow(GLdouble , GLdouble , GLdouble , GLdouble );

int main()
{

  drawPolyLineFile("dragon.dat");
  return 0;
}

void setWindow(GLdouble left, GLdouble right, GLdouble buttom, GLdouble top)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(left, right, buttom, top);
}

void drawPolyLineFile(char* filename)
{
  char line[256];
  FILE* fp = NULL;
  
  float top, buttom, left, right;
  int total_poly;
  int i, j;
  int * polygens = NULL;
  float x, y;

  fp = fopen(filename, "r");
  if(fp == NULL)
  {
    printf("can not open the file.\n");
    return;
  }

// omit all lines before *
  while(!feof(fp))
  {
     memset(line, 0, 256);
     fscanf(fp, "%s", line);
     if(line[0] == '*')
       break;
     else
       continue;
  }

// read in the four directions of extents
  fscanf(fp, "%f %f %f %f\n", &left, &top, &right, &buttom);
  setWindow(left, right, buttom, top);

// read in the total polygens
  fscanf(fp, "%d", &total_poly);
  
 polygens = (int*)malloc(total_poly * sizeof(int));

// read in the polygens
  for(i = 0;i < total_poly;i ++)
  {
     fscanf(fp, "%d", &polygens[i]);

     glBegin(GL_LINE_STRIP);	     // draw the next polyline
     for(j = 0;j < polygens[i];j ++)
     {
// read in the x and y coordinates
// let's do it later
        fscanf(fp, "%f %f", &x, &y);
	glVertex3d(x, y, 0);
     }
     glEnd();
  }

  glFlush();

  printf("total polygens: %d\n", total_poly);
  for(i = 0;i < total_poly;i ++)
    printf("polygen %d : %d\n", polygens[i]);

  free(polygens);
  polygens = NULL;
}
