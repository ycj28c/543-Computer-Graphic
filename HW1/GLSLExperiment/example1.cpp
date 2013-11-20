// Angel.h is homegrown include file that also includes glew and freeglut
#include "Angel.h"

//Other reference
using namespace std;

//Define structure
typedef struct {        
	float top;
	float bottom;
	float left;
	float right;
	int  total_poly;          
	//float **polygens;
	int subpolynum[200];
	float polygens[200][3000]; //memory for grs file
}grsfile;
struct GLintPoint{
    GLint x, y;
};

//Define the global varible
#define window_height 480
#define window_width  640
const int NumPoints = 3;
typedef vec2 point2;
point2 points[NumPoints];
point2 pointshw1[3000];
GLuint program;
int stage = 3; //every key event has a stage
int stagelock = 3;//each stage has lock
bool isnewpoint; //if its new point, for b function
int drawsteps[100];
int drawstepscount;
int tempcount;
point2 pointstemp[100];
int pointstempcount;
point2 * tempmousepoint;
bool moveable;
bool onlyonce = 0;
//float etime;
GLuint ProjLoc;
GLuint loc;
double windowratiox=1;
double windowratioy=1;
int globalmanify;
int p[6][6]; //for 6*6 reshape

//Declare the routine
void generateGeometry(void );
void initGPUBuffers();
void shaderSetup(void );
void display(void );
void myInit(void );
void renderGL(void );
void Mousehw(int button, int state, int x, int y );
void processMouseActiveMotion (int, int );
void keyboarddown(unsigned char, int, int );
void keyboardup(unsigned char, int, int );
void magnify(int x, int y);
void drawsixsix(void );
void fillsixsix(void );
void gingerbread (void );
void readgrs(char *);
point2 translatepoint(float, float, float, float, float, float );
int translatepointbackx(float );
int translatepointbacky(float );
void drawmode(int, int );
void drawdelete(int, int );
void drawmoving(int, int );
void drawmodify(int, int );
void setWindow(GLfloat left,GLfloat right,GLfloat bottom,GLfloat top);

//********************************Begin the routine************************************//

void generateGeometry( void )  //the original test
{
	// ***************** Important note ***************** //
	// please refer to OpenGL documentation before coding
	// many old functions have been depricated
	// though they will probably still work on most machines others
	// will require compatiability mode to be used
	// Specifiy the vertices for a triangle
	// for 3d points use vec3 and change your vPosition attribute appropriately
	points[0] = point2( -0.5, -0.5 );
	points[1] = point2( 0.0, 0.5 );
	points[2] = point2( 0.5, -0.5 );
}

void initGPUBuffers()
{
	// Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
   // glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );
	glBufferData( GL_ARRAY_BUFFER, sizeof(pointshw1), pointshw1, GL_STATIC_DRAW ); //getbuffer
}

void shaderSetup( void )
{
	    // Load shaders and use the resulting shader program
    program = InitShader( "vshader1.glsl", "fshader1.glsl" );
    glUseProgram( program );

    // Initialize the vertex position attribute from the vertex shader
    loc = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	// sets the default color to clear screen
   // glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
}

//translate the float coord to coord we need
point2 translatepoint(float left, float top, float right, float bottom, float a, float b){
	float xzoom = 2/(right - left);
	float yzoom = 2/(top - bottom);
	float xoffset = (right + left)/(right - left);
	float yoffset = (top + bottom)/(top - bottom);
	float newx = a * xzoom - xoffset;
	float newy = b * yzoom- yoffset;
	return point2( newx, newy );
}
int translatepointbackx(float a){  //function from float coord to int x coord
	return (int)((a+1)/2*window_width*windowratiox);
}
int translatepointbacky(float a){  //function from float coord to int y coord
	return (int)((a+1)/2*window_height*windowratioy);
}

//File operate
void DrawFromFile(char* filename)
{
	char line[256];
	FILE* fp = NULL; 
	float top, bottom, left, right;
	int total_poly;
	int i, j;
	int * polygens = NULL;
	float x, y;

	printf(" now drawing %s\n", filename);

	fp = fopen(filename, "r");
	if(fp == NULL){
		printf("can not open the file.\n");
		return;
	}

	while(!feof(fp)){		// delete all context before *
		memset(line, 0, 256);
		fscanf(fp, "%s", line);
		if(line[0] == '*')	break;
		else	continue;
	}

	fscanf(fp, "%f %f %f %f\n", &left, &top, &right, &bottom); // read in the four directions of extents
	fscanf(fp, "%d", &total_poly); // read in the total polygens
	polygens = (int*)malloc(total_poly * sizeof(int));  

	//glClear(GL_COLOR_BUFFER_BIT);  //clear the screen

	for(i = 0;i < total_poly;i ++) 	// read in the polygens
	{
		fscanf(fp, "%d", &polygens[i]);
		for(j = 0;j < polygens[i];j ++)
		{
			fscanf(fp, "%f %f", &x, &y);
			if(strcmp(filename,"drawing\\dino.dat")==0){
				pointshw1[j] = translatepoint(0.0, 1.0, 1.0, 0.0, (float)x/window_width, (float)y/window_height);
			}
			else pointshw1[j] = translatepoint(left, top, right, bottom, x, y);
		}
		glBufferData( GL_ARRAY_BUFFER, sizeof(pointshw1), pointshw1, GL_STATIC_DRAW );
		glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();                      // reset the matrix to its default state
		glDrawArrays( GL_LINE_STRIP, 0, polygens[i] ); 
	}
	glFlush();
	
	//recycle
	free(polygens); //free the pointer space
	polygens = NULL;
	fclose(fp); //release the file
}

// display function below
void display( void )
{
	// TIP1: remember to enable depth buffering when drawing in 3d
	// TIP2: If you want to be sure your math functions are right, 
	//		 prototype your operations in Matlab or Mathematica first to verify correct behavior
	//		 both programs support some sort of runtime interface to C++ programs
	// TIP3: check your graphics specs. you have a limited number of loop iterations, storage, registers, texture space etc.
	// TIP4: avoid using glTranslatex, glRotatex, push and pop
	//		 pass your own view matrix to the shader directly
	//		 refer to the latest OpenGL documentation for implementation details

    glClear( GL_COLOR_BUFFER_BIT );     // clear the window
    glDrawArrays( GL_LINE_LOOP, 0, NumPoints );    // draw the points
    glFlush(); // force output to graphics hardware
}

void setWindow(GLfloat left,GLfloat right,GLfloat bottom,GLfloat top)
{
	
	ProjLoc = glGetUniformLocation( program, "Proj" );
	mat4 ortho = Ortho2D( left, right, bottom, top );
    glUniformMatrix4fv( ProjLoc, 1, GL_FALSE, ortho );
}

void renderGL()  
{  
		for (int loop=0; loop<10; loop++)                
		{  
			if (loop==0)	glViewport (0, window_height/10*9*windowratioy, window_width/10*windowratiox, window_height/10*windowratioy); 
			if (loop==1)	glViewport (window_width/10*windowratiox, window_height/10*9*windowratioy, window_width/10*windowratiox, window_height/10*windowratioy);  
			if (loop==2)	glViewport (window_width/10*2*windowratiox, window_height/10*9*windowratioy, window_width/10*windowratiox, window_height/10*windowratioy);  
			if (loop==3)	glViewport (window_width/10*3*windowratiox, window_height/10*9*windowratioy, window_width/10*windowratiox, window_height/10*windowratioy);            
			if (loop==4)	glViewport (window_width/10*4*windowratiox, window_height/10*9*windowratioy, window_width/10*windowratiox, window_height/10*windowratioy);             
			if (loop==5)	glViewport (window_width/10*5*windowratiox, window_height/10*9*windowratioy, window_width/10*windowratiox, window_height/10*windowratioy);             
			if (loop==6)	glViewport (window_width/10*6*windowratiox, window_height/10*9*windowratioy, window_width/10*windowratiox, window_height/10*windowratioy);        
			if (loop==7)	glViewport (window_width/10*7*windowratiox, window_height/10*9*windowratioy, window_width/10*windowratiox, window_height/10*windowratioy);             
			if (loop==8)	glViewport (window_width/10*8*windowratiox, window_height/10*9*windowratioy, window_width/10*windowratiox, window_height/10*windowratioy);           
			if (loop==9)	glViewport (window_width/10*9*windowratiox, window_height/10*9*windowratioy, window_width/10*windowratiox, window_height/10*windowratioy); 
   
			if (loop==0)	DrawFromFile("drawing\\dino.dat");
			if (loop==1)	DrawFromFile("drawing\\birdhead.dat");
			if (loop==2)	DrawFromFile("drawing\\dragon.dat");
			if (loop==3)	DrawFromFile("drawing\\house.dat"); 
			if (loop==4)	DrawFromFile("drawing\\knight.dat"); 
			if (loop==5)	DrawFromFile("drawing\\rex.dat"); 
			if (loop==6)	DrawFromFile("drawing\\scene.dat"); 
			if (loop==7)	DrawFromFile("drawing\\usa.dat"); 
			if (loop==8)	DrawFromFile("drawing\\vinci.dat"); 
			if (loop==9)	DrawFromFile("drawing\\vinci.dat"); 
		}  
		glFlush();
}  

void magnify(int x, int y){
	if(y<(window_height*windowratiox/10*windowratioy*windowratioy)){
		glEnable(GL_SCISSOR_TEST);           
		glScissor(0, 0, window_width*windowratiox, window_height/10*9*windowratioy);
		glClear (GL_COLOR_BUFFER_BIT);  
		glDisable(GL_SCISSOR_TEST);   
		glViewport (window_width/10*1.5*windowratiox, window_width/10*windowratioy, window_width/10*7*windowratiox, window_height/10*7*windowratioy); 
		if(0<x&&x<=window_width/10*windowratiox)										{DrawFromFile("drawing\\dino.dat");globalmanify=1;}
		else if(window_width/10*windowratiox<x&&x<=window_width/10*2*windowratiox)		{DrawFromFile("drawing\\birdhead.dat");globalmanify=2;}
		else if(window_width/10*2*windowratiox<x&&x<=window_width/10*3*windowratiox)	{DrawFromFile("drawing\\dragon.dat");globalmanify=3;}
		else if(window_width/10*3*windowratiox<x&&x<=window_width/10*4*windowratiox)	{DrawFromFile("drawing\\house.dat");globalmanify=4;}
		else if(window_width/10*4*windowratiox<x&&x<=window_width/10*5*windowratiox)	{DrawFromFile("drawing\\knight.dat");globalmanify=5;}
		else if(window_width/10*5*windowratiox<x&&x<=window_width/10*6*windowratiox)	{DrawFromFile("drawing\\rex.dat");globalmanify=6;}
		else if(window_width/10*6*windowratiox<x&&x<=window_width/10*7*windowratiox)	{DrawFromFile("drawing\\scene.dat");globalmanify=7;}
		else if(window_width/10*7*windowratiox<x&&x<=window_width/10*8*windowratiox)	{DrawFromFile("drawing\\usa.dat");globalmanify=8;}
		else if(window_width/10*8*windowratiox<x&&x<=window_width/10*9*windowratiox)	{DrawFromFile("drawing\\vinci.dat");globalmanify=9;}
		else if(window_width/10*9*windowratiox<x&&x<window_width*windowratiox)			{DrawFromFile("drawing\\vinci.dat");globalmanify=10;}
	}
	stagelock = 1; 
}

void drawsixsix(){
	glEnable(GL_SCISSOR_TEST);      
	glScissor(0, 0, window_width*windowratiox, window_height/10*9*windowratioy);
	glClear (GL_COLOR_BUFFER_BIT);  
	glDisable(GL_SCISSOR_TEST);  

	int count = 0; 
    //glLoadIdentity();   

	glViewport (window_width/8*windowratiox, 0, window_width/8*6*windowratiox, window_height/8*6*windowratioy);
	//gluOrtho2D(0.0,window_width,0.0,window_height);
	
	for (float x = -1; x<=1;) //draw 6*6
	{
		pointshw1[count] = point2( x, 1.0 );
		count++;
		pointshw1[count] = point2( x, -1.0 );
		count++;
		x = x+ 0.333;
	}
	for (float y = -1; y<=1;)  
	{
		pointshw1[count] = point2( 1.0, y );
		count++;
		pointshw1[count] = point2( -1.0, y );
		count++;
		y = y+ 0.333; //(float)2/6
	}

	glBufferData( GL_ARRAY_BUFFER, sizeof(pointshw1), pointshw1, GL_STATIC_DRAW );
	glDrawArrays( GL_LINES, 0, count);
	glFlush();

	//glLoadIdentity();
	fillsixsix();

	stagelock = 2;
} 

void fillsixsix(){
	for(int x = 0; x<6; x++)
		for(int y = 0; y<6; y++){
			//glLoadIdentity();
			glViewport (window_width/8*windowratiox+window_width/8*x*windowratiox, 0+window_height/8*y*windowratioy, window_width/8*windowratiox, window_height/8*windowratioy);
			int randomdraw = rand()%8;
			if (randomdraw == 0) {DrawFromFile("drawing\\dino.dat"); p[x][y] = randomdraw;}
			if (randomdraw == 1) {DrawFromFile("drawing\\birdhead.dat");p[x][y] = randomdraw;}
			if (randomdraw == 2) {DrawFromFile("drawing\\dragon.dat");p[x][y] = randomdraw;}
			if (randomdraw == 3) {DrawFromFile("drawing\\house.dat");p[x][y] = randomdraw;}
			if (randomdraw == 4) {DrawFromFile("drawing\\knight.dat");p[x][y] = randomdraw;}
			if (randomdraw == 5) {DrawFromFile("drawing\\rex.dat");p[x][y] = randomdraw;}
			if (randomdraw == 6) {DrawFromFile("drawing\\scene.dat");p[x][y] = randomdraw;}
			if (randomdraw == 7) {DrawFromFile("drawing\\usa.dat");p[x][y] = randomdraw;}
			if (randomdraw == 8) {DrawFromFile("drawing\\vinci.dat");p[x][y] = randomdraw;}
		}		
	glFlush();
}

void gingerbread ()
{
	glEnable(GL_SCISSOR_TEST);         
	glScissor(0, 0, window_width*windowratiox, window_height/10*9*windowratioy);
	glClear (GL_COLOR_BUFFER_BIT);  
	glDisable(GL_SCISSOR_TEST);  
	glViewport (0, 0, window_width*windowratiox, window_height/10*9*windowratioy);


	glMatrixMode( GL_MODELVIEW );  
    glLoadIdentity( ); 

	int times = 3000;
	int count;
	GLintPoint p, q;
	const int M = 40;
	const int L = 3;

	p.x = 121;
    p.y = 115;
	
	for (count = 0; count < times; count++)
	{
		q.x = M * (1 + 2 * L) - p.y + abs(p.x - L * M);
		q.y = p.x;
		p = q;
		pointshw1[count] = translatepoint(0.0, 1.0, 1.0, 0.0, (float)p.x/window_width, (float)p.y/window_height);
	}
 
	glBufferData( GL_ARRAY_BUFFER, sizeof(pointshw1), pointshw1, GL_STATIC_DRAW );
	glDrawArrays( GL_POINTS, 0, count);

	glFlush();
	stagelock = 3;
}

void drawmode(int x, int y){
	glViewport (0, 0, window_width*windowratiox, window_height*windowratioy);
	if(pointstempcount<=99){ //the limit of storage is 100
		pointstemp[pointstempcount] = translatepoint(0.0, 1.0, 1.0, 0.0, (float)x/(window_width*windowratiox), (float)(window_height*windowratioy-y)/(window_height*windowratioy));
		if(isnewpoint){
			drawsteps[drawstepscount] = pointstempcount - tempcount;
			drawstepscount++;
			tempcount = pointstempcount;	
			drawsteps[drawstepscount] = pointstempcount + 1 - tempcount;				
		}
		else{
			drawsteps[drawstepscount] = pointstempcount + 1 - tempcount;
		}
		pointstempcount++;
	}
	else printf("ERROR!the limit of storage is 100\n");
	
	//print the drawing
	int k = 0;
	for(int i = 0;i <=drawstepscount;i ++) 	// read in the polygens
	{
		for(int j = 0;j <drawsteps[i];j ++)
		{
			pointshw1[j] = pointstemp[k];
			k++;
		}

		glBufferData( GL_ARRAY_BUFFER, sizeof(pointshw1), pointshw1, GL_STATIC_DRAW );
		glDrawArrays( GL_LINE_STRIP, 0, drawsteps[i]); 
	}
	glFlush();

	stagelock = 4;
}

void drawmodify(int x,int y){
	glViewport (0, 0, window_width*windowratiox, window_height*windowratioy);
	moveable = FALSE;
	for(int k=0;k<pointstempcount;k++){
		int tempx = translatepointbackx(pointstemp[k].x); 
		int tempy = window_height*windowratioy-translatepointbacky(pointstemp[k].y); 
		if((-2<=x-tempx)&&(x-tempx<=2)&&(-2<=y-tempy)&&(y-tempy<=2)){  //mouse select range2
				printf("mouse gets point successful!!!!!\n");
				tempmousepoint = pointstemp+k;   
				moveable = TRUE;
		}
	}
	stagelock = 5;
}

void drawmoving(int x, int y){
	glViewport (0, 0, window_width*windowratiox, window_height*windowratioy);
	if(moveable){
		tempmousepoint->x = (float )translatepoint(0.0, 1.0, 1.0, 0.0, (float)x/(window_width*windowratiox), (float)(window_height*windowratioy-y)/(window_height*windowratioy)).x;
		tempmousepoint->y = (float )translatepoint(0.0, 1.0, 1.0, 0.0, (float)x/(window_width*windowratiox), (float)(window_height*windowratioy-y)/(window_height*windowratioy)).y;

		//redraw the graphic
		glEnable(GL_SCISSOR_TEST);        
		glScissor(0, 0, window_width*windowratiox, window_height/10*9*windowratioy);
		glClear (GL_COLOR_BUFFER_BIT);  
		glDisable(GL_SCISSOR_TEST);  

		int k = 0;
		for(int i = 0;i <=drawstepscount;i ++) 	// read in the polygens
		{
			for(int j = 0;j <drawsteps[i];j ++)
			{
				pointshw1[j] = pointstemp[k];
				k++;
			}
			glBufferData( GL_ARRAY_BUFFER, sizeof(pointshw1), pointshw1, GL_STATIC_DRAW );
			glDrawArrays( GL_LINE_STRIP, 0, drawsteps[i]); 
		}
		glFlush();
	}
	stagelock = 5;
}

void drawdelete(int x, int y){
	glViewport (0, 0, window_width*windowratiox, window_height*windowratioy);
	int temp = 0;
	for(int k=0;k<pointstempcount;){
		int tempx = translatepointbackx(pointstemp[k].x);  
		int tempy = window_height*windowratioy-translatepointbacky(pointstemp[k].y);
		if((-2<=x-tempx)&&(x-tempx<=2)&&(-2<=y-tempy)&&(y-tempy<=2)){ 
				printf("mouse gets point successful!!!!!\n");

				for(int m=k;m<=pointstempcount;m++){  //
					pointstemp[m] = pointstemp[m+1];
				}
				pointstempcount = pointstempcount -1;
				for(int m=0;m<=drawstepscount;m++){
					temp = temp + drawsteps[m];
					for(int n=1;n<=drawsteps[m];n++){
						if(temp>k){
							if(drawsteps[m]==1){  
								for(int t=m;t<drawstepscount;t++){
									drawsteps[m]=drawsteps[m+1];
								}
								drawstepscount = drawstepscount - 1;
								m = drawstepscount + 2; 
							}
							else{  
								drawsteps[m] = drawsteps[m] - 1;
								m = drawstepscount + 2;
							}
						}
					}			
				}
				k = pointstempcount + 2;
		}
		k++;
	}
	
	//redraw the graphic
	glEnable(GL_SCISSOR_TEST);          
	glScissor(0, 0, window_width*windowratiox, window_height/10*9*windowratioy);
	glClear (GL_COLOR_BUFFER_BIT);  
	glDisable(GL_SCISSOR_TEST);  

	int k = 0;
	for(int i = 0;i <=drawstepscount;i ++) 	// read in the polygens
	{
		for(int j = 0;j <drawsteps[i];j ++)
		{
			pointshw1[j] = pointstemp[k];
			k++;
		}
		glBufferData( GL_ARRAY_BUFFER, sizeof(pointshw1), pointshw1, GL_STATIC_DRAW );

		glDrawArrays( GL_LINE_STRIP, 0, drawsteps[i]); 
	}
	glFlush();

	stagelock = 6;
}
// keyboard handler
void keyboarddown(unsigned char theKey, int mouseX, int mouseY)
{
	switch(theKey)
	{
	case 'p':
		stage = 1;
		magnify((rand()%window_width)+1, (rand()%window_height/10)+1);
		break;
	case 't':
		stage = 2;
		drawsixsix();
		break;
	case 'g':
		stage = 3;
		gingerbread ();
		break;
	case 'e':
		stage = 4;
		//init the global variable
		memset(drawsteps, 0, sizeof(drawsteps));
		memset(pointstemp, 0, sizeof(pointstemp));
		pointstempcount = 0;
		tempmousepoint = NULL;
		tempcount = 0;
		//clear screen
		glEnable(GL_SCISSOR_TEST);         
		glScissor(0, 0, window_width*windowratiox, window_height/10*9*windowratioy);
		glClear (GL_COLOR_BUFFER_BIT);  
		glDisable(GL_SCISSOR_TEST);  
		glViewport (0, 0, window_width*windowratiox, window_height*windowratioy);
		break;
	case 'b':
		//stage = 4;
		isnewpoint = TRUE;
		break;
	case 'm':
		if(stagelock == 4||stagelock == 5||stagelock == 6) 
			stage = 5;
		break;
	case 'd':
		if(stagelock == 4||stagelock == 5||stagelock == 6) 
			stage = 6;
		break;
	case 'c': 
		if(stagelock == 4||stagelock == 5||stagelock == 6)
			stage = 4;
		break;
	case 033:
		exit( EXIT_SUCCESS );	
	default:
		break;
	}
	glutPostRedisplay();
}

void keyboardup(unsigned char theKey, int mouseX, int mouseY){
	switch(theKey)
	{
	case 'b':
		//stagelock = 4;
		isnewpoint = FALSE;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}
//Mouse handler
void Mousehw(int button, int state, int x, int y)
{
	if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
	{
		if(stage == 1)
			magnify(x,y);
		if(stage == 4)
			drawmode(x,y);
		if(stage == 5)
			drawmodify(x,y);
		if(stage == 6)
			drawdelete(x,y);
	}
	if (button==GLUT_LEFT_BUTTON && state==GLUT_UP)
	{
		if(stage == 5){
			moveable = FALSE;
		}	
	}
	glutPostRedisplay();
}
void processMouseActiveMotion (int x, int y){
	if(stage == 5){
		drawmoving(x,y);
	}	
	glutPostRedisplay();
}
//reshape windows
void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	printf("xxxxxxxxxx:%d,yyyyyyyyyyy:%d\n",w,h);
	glViewport( 0, 0, (GLint)window_width, (GLint)window_height );  
	windowratiox = (double)w/window_width;
	windowratioy = (double)h/window_height;
	printf("windowratiox:%d,windowratiox:%d\n",windowratiox,windowratioy);
	if(stage == 1){
		glEnable(GL_SCISSOR_TEST);         
		glScissor(0, 0, window_width*windowratiox, window_height/10*9*windowratioy);
		glClear (GL_COLOR_BUFFER_BIT);  
		glDisable(GL_SCISSOR_TEST);  
		glViewport (window_width/10*1.5*windowratiox, window_width/10*windowratioy, window_width/10*7*windowratiox, window_height/10*7*windowratioy); 
		if(globalmanify==1) DrawFromFile("drawing\\dino.dat");
		if(globalmanify==2) DrawFromFile("drawing\\birdhead.dat");
		if(globalmanify==3) DrawFromFile("drawing\\dragon.dat");
		if(globalmanify==4) DrawFromFile("drawing\\house.dat");
		if(globalmanify==5) DrawFromFile("drawing\\knight.dat");
		if(globalmanify==6) DrawFromFile("drawing\\rex.dat");
		if(globalmanify==7) DrawFromFile("drawing\\scene.dat");
		if(globalmanify==8) DrawFromFile("drawing\\usa.dat");
		if(globalmanify==9) DrawFromFile("drawing\\vinci.dat");
		if(globalmanify==10) DrawFromFile("drawing\\vinci.dat");
	}
	if(stage == 2){
		glEnable(GL_SCISSOR_TEST);         
		glScissor(0, 0, window_width*windowratiox, window_height/10*9*windowratioy);
		glClear (GL_COLOR_BUFFER_BIT);  
		glDisable(GL_SCISSOR_TEST);  

		int count = 0; 
		//glLoadIdentity();   

		glViewport (window_width/8*windowratiox, 0, window_width/8*6*windowratiox, window_height/8*6*windowratioy);
		//gluOrtho2D(0.0,window_width,0.0,window_height);
	
		for (float x = -1; x<=1;)  
		{
			pointshw1[count] = point2( x, 1.0 );
			count++;
			pointshw1[count] = point2( x, -1.0 );
			count++;
			x = x+ 0.333; //(float)2/6
		}
		for (float y = -1; y<=1;)  
		{
			pointshw1[count] = point2( 1.0, y );
			count++;
			pointshw1[count] = point2( -1.0, y );
			count++;
			y = y+ 0.333; //(float)2/6
		}

		glBufferData( GL_ARRAY_BUFFER, sizeof(pointshw1), pointshw1, GL_STATIC_DRAW );
		glDrawArrays( GL_LINES, 0, count);
		glFlush();

		//glLoadIdentity();
		for(int x = 0; x<6; x++)
			for(int y = 0; y<6; y++){
				glViewport (window_width/8*windowratiox+window_width/8*x*windowratiox, 0+window_height/8*y*windowratioy, window_width/8*windowratiox, window_height/8*windowratioy);
				if (p[x][y]  == 0) {DrawFromFile("drawing\\dino.dat"); }
				if (p[x][y]  == 1) {DrawFromFile("drawing\\birdhead.dat");}
				if (p[x][y]  == 2) {DrawFromFile("drawing\\dragon.dat");}
				if (p[x][y]  == 3) {DrawFromFile("drawing\\house.dat");}
				if (p[x][y]  == 4) {DrawFromFile("drawing\\knight.dat");}
				if (p[x][y]  == 5) {DrawFromFile("drawing\\rex.dat");}
				if (p[x][y]  == 6) {DrawFromFile("drawing\\scene.dat");}
				if (p[x][y]  == 7) {DrawFromFile("drawing\\usa.dat");}
				if (p[x][y]  == 8) {DrawFromFile("drawing\\vinci.dat");}
			}		
		glFlush();
	}
	if(stage == 3){
		gingerbread ();
	}
	if(stage == 4){
		glEnable(GL_SCISSOR_TEST);           
		glScissor(0, 0, window_width*windowratiox, window_height*windowratioy);
		glClear (GL_COLOR_BUFFER_BIT);  
		glDisable(GL_SCISSOR_TEST);  
		glViewport (0, 0, window_width*windowratiox, window_height*windowratioy);
		int k = 0;
		for(int i = 0;i <=drawstepscount;i ++) 	// read in the polygens
		{
			for(int j = 0;j <drawsteps[i];j ++)
			{
				pointshw1[j] = pointstemp[k];
				k++;
			}

			glBufferData( GL_ARRAY_BUFFER, sizeof(pointshw1), pointshw1, GL_STATIC_DRAW );
			//initGPUBuffers();
			//shaderSetup( );
			glDrawArrays( GL_LINE_STRIP, 0, drawsteps[i]); 
		}
		glFlush();
	}
	if(stage == 5){
		glEnable(GL_SCISSOR_TEST);        
		glScissor(0, 0, window_width*windowratiox, window_height*windowratioy);
		glClear (GL_COLOR_BUFFER_BIT);  
		glDisable(GL_SCISSOR_TEST);  
		glViewport (0, 0, window_width*windowratiox, window_height*windowratioy);
		int k = 0;
		for(int i = 0;i <=drawstepscount;i ++) 	// read in the polygens
		{
			for(int j = 0;j <drawsteps[i];j ++)
			{
				pointshw1[j] = pointstemp[k];
				k++;
			}

			glBufferData( GL_ARRAY_BUFFER, sizeof(pointshw1), pointshw1, GL_STATIC_DRAW );
			glDrawArrays( GL_LINE_STRIP, 0, drawsteps[i]); 
		}
		glFlush();
	}
	if(stage == 6){
		glEnable(GL_SCISSOR_TEST);         
		glScissor(0, 0, window_width*windowratiox, window_height*windowratioy);
		glClear (GL_COLOR_BUFFER_BIT);  
		glDisable(GL_SCISSOR_TEST);  
		glViewport (0, 0, window_width*windowratiox, window_height*windowratioy);
		int k = 0;
		for(int i = 0;i <=drawstepscount;i ++) 	// read in the polygens
		{
			for(int j = 0;j <drawsteps[i];j ++)
			{
				pointshw1[j] = pointstemp[k];
				k++;
			}

			glBufferData( GL_ARRAY_BUFFER, sizeof(pointshw1), pointshw1, GL_STATIC_DRAW );
			glDrawArrays( GL_LINE_STRIP, 0, drawsteps[i]); 
		}
		glFlush();
	}
}

//Init the graphic
void myInit(void)
{
	//float ratio = (float) window_width / (float) window_height;  
	glClearColor( 1.0, 1.0, 1.0, 1.0 ); 
	glColor3f(0.0,0.0,0.0);
	glPointSize(2.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective( 60.0, ratio, 1.0, 100.0 );
	glClear(GL_COLOR_BUFFER_BIT);  //未使用这一句会使绘图变慢;
}

// entry point
int main( int argc, char **argv )
{
	// init glut
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );
	glutInitWindowSize( window_width, window_height );
	//glutInitWindowPosition(100,150);

    // If you are using freeglut, the next two lines will check if 
    // the code is truly 3.2. Otherwise, comment them out
    //glutInitContextVersion( 3, 3 );
    //glutInitContextProfile( GLUT_CORE_PROFILE );

	// create GLUT window for drawing
    glutCreateWindow( "CS543 Homework1" );

	// init glew
    glewInit();

	initGPUBuffers();
	shaderSetup();
	// assign handlers
	myInit();
    glutDisplayFunc( renderGL );
	stage = 1; //从stage1开始
	//renderGL();
	//gingerbread();

	glutReshapeFunc ( reshape );

	// should add menus
	//glutCreateMenu(processMenuEvents4);

	// add mouse handler
	glutMouseFunc(Mousehw);
	glutMotionFunc(processMouseActiveMotion);

	// add keyboard handler
	glutKeyboardFunc( keyboarddown ); 
	glutKeyboardUpFunc( keyboardup );  
	//glutMotionFunc(Mousehw);
	//glutPassiveMotionFunc(myPassiveMotiontest3);

	// add resize window functionality (should probably try to preserve aspect ratio)

	// enter the drawing loop
	// frame rate can be controlled with 
    glutMainLoop();
	return 0;
}


