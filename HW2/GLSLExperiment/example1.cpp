// Two-Dimensional Sierpinski Gasket       
// Generated using randomly selected vertices and bisection

#include "Angel.h"


//----------------------------------------------------------------------------
#define  GLUT_WHEEL_UP 3           //install mouse mid button
#define  GLUT_WHEEL_DOWN 4
#define	 window_width 512
#define	 window_height 512

//////////
typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
struct face3{
	int face;
	int point1;
	int point2;
	int point3;
};
struct cuboidborder{
	float xmin;
	float xmax;
	float ymin;
	float ymax;
	float zmax;
	float zmin;
};
typedef struct{             
	int  facenum;
	int	 vertexnum;
	point4 *flyvertex; 
	face3 *flyface; 
}Plyfile; 

//define constant valve
using namespace std;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)
//const int NumVerticeshw2 = 120000; // change mothed, use pointer to init the array
int width = 0;
int height = 0;
float xaxis,yaxis,zaxis;
float yrotate,xrotate,currentrotate;
int stage;
bool isbigger;
bool isclockwise;
int xtranslate, Xtranslate,ytranslate, Ytranslate,ztranslate, Ztranslate, canyrotate;
float normalscale; 
char *allfiles[43] =  {"airplane.ply", "ant.ply", "apple.ply", "balance.ply", "beethoven.ply", "big_atc.ply", "big_dodge.ply", "big_porsche.ply",
     "big_spider.ply", "canstick.ply", "chopper.ply", "cow.ply", "dolphins.ply", "egret.ply", "f16.ply", "footbones.ply", "fracttree.ply",
     "galleon.ply", "hammerhead.ply", "helix.ply", "hind.ply", "kerolamp.ply", "ketchup.ply", "mug.ply", "part.ply", "pickup_big.ply",
	 "pump.ply", "pumpa_tb.ply", "sandal.ply", "saratoga.ply","scissors.ply", "shark.ply","steeringweel.ply","stratocaster.ply","street_lamp.ply",
	 "teapot.ply","tennis_shoe.ply","tommygun.ply","trashcan.ply","turbine.ply","urn2.ply","walkman.ply","weathervane.ply"};
int currentmodle;
char *plypath = "ply_files\\\\"; //the file path
int globaltempnumber,globalcuboidnumber;
int isnormalmode,isbreathmode,iscuboidmode,iscuboidmodeextend; 

// handle to program
GLuint program;
point4 *pointshw2; 
color4 *colorshw2;
point4 *currentpointshw2; 
//color4 *currentcolorshw2;
point4 points[NumVertices];
color4 colors[NumVertices];

// remember to prototype
void generateGeometry( void );
void display( void );
void mouse(int button,int state, int x, int y);
void processMouseActiveMotion (int x, int y);
void keyboard( unsigned char key, int x, int y );
void quad( int a, int b, int c, int d );
void colorcube(void);
void drawCube(void);
Plyfile readplyfile( char *);
int  filldrawing(void);
void drawply(void);
void initGPUAndShader( void );
void displayhw2( void );
void idlehandle( void );
void drawnext( void );
void drawpre( void );
void initpro( void ); //init the graphic
void dotranslate( void );
void resetstate( void );
void transpoint2ordinate(point4 *, int );
point4 translatepoint(float left, float right, float bottom, float top, float znear, float zfar, float x, float y, float z);
point4 newell( void );
void drawnormal( void );
void dobreathing( void );
void drawcuboid( void );
void formcuboid(point4, point4, point4, point4 );
void formcuboidpoint(float, float, float, float, float, float);
void drawbigcuboid( void );

// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices[8] = {
    point4( -0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5, -0.5, -0.5, 1.0 ),
    point4( -0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5, -0.5, -0.5, 1.0 )
};
// RGBA olors
color4 vertex_colors[8] = {
    color4( 0.0, 0.0, 0.0, 1.0 ),  // black
    color4( 1.0, 0.0, 0.0, 1.0 ),  // red
    color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
    color4( 0.0, 1.0, 0.0, 1.0 ),  // green
    color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
    color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
    color4( 1.0, 1.0, 1.0, 1.0 ),  // white
    color4( 0.0, 1.0, 1.0, 1.0 )   // cyan
};

Plyfile readplyfile(char *filename)
{
	char *name;
	FILE *fp = NULL;
	char filetype[10];
	char temp1[20],temp2[20],temp3[20],temp4[20],temp5[20];
	Plyfile plyf;
	int icount;

	name = (char *) malloc (sizeof (char) * (strlen (filename) + 5));
	strcpy (name, filename);
	if (strlen (name) < 4 ||strcmp (name + strlen (name) - 4, ".ply") != 0) 
		strcat (name, ".ply"); 

	fp = fopen(filename,"r");
	if(fp == NULL){
		printf("can not open the file.\n");
		exit(0);
	}

	fscanf(fp, "%s",&filetype); //ply
	if(strcmp(filetype,"ply")!=0){
		printf("ERROR! the file type is not existed\n");
		exit(0);
	}
	fscanf(fp, "%s %s %f",&temp1,&temp2,&temp3);//format ascii 1.0
	fscanf(fp, "%s %s %d",&temp1,&temp2,&plyf.vertexnum); //element vertex 1335
	fscanf(fp, "%s %s %s",&temp1,&temp2,&temp3);//property float32 x
	fscanf(fp, "%s %s %s",&temp1,&temp2,&temp3);//property float32 y
	fscanf(fp, "%s %s %s",&temp1,&temp2,&temp3);//property float32 z
	fscanf(fp, "%s %s %d",&temp1,&temp2,&plyf.facenum);//element face 2452
	fscanf(fp, "%s %s %s %s %s\n",&temp1,&temp2,&temp3,&temp4,&temp5);//property list uint8 int32 vertex_indices //uint8 和int32属于int类型?
	fscanf(fp, "%s",&temp1);//end_header

	plyf.flyvertex = (point4 *)calloc(1,sizeof(point4)*plyf.vertexnum);//dynamic allocate momery
	for(icount=0;icount<plyf.vertexnum;icount++){
		fscanf(fp, "%f %f %f", &plyf.flyvertex[icount].x, &plyf.flyvertex[icount].y, &plyf.flyvertex[icount].z);
	}
	plyf.flyface = (face3 *)calloc(1,sizeof(face3)*plyf.facenum);
	for(icount=0;icount<plyf.facenum;icount++){
		fscanf(fp, "%d %d %d %d", &plyf.flyface[icount].face, &plyf.flyface[icount].point1, &plyf.flyface[icount].point2, &plyf.flyface[icount].point3);
	}
	
	fclose(fp);
	return plyf;
}

int filldrawing() //
{
	char tempname[50]; //limit lenth 50
	int i;
	int index = 0;
	i=0;

	strcpy(tempname,plypath);
	strcat(tempname,allfiles[currentmodle]);
	Plyfile plyread = readplyfile(tempname);
	int inumber = plyread.facenum*3;
	pointshw2 = (point4 *) malloc (sizeof(point4)*inumber);
	colorshw2 = (color4 *) malloc (sizeof(color4)*inumber);

	while(i<plyread.facenum){
		pointshw2[index] = plyread.flyvertex[plyread.flyface[i].point1];
		pointshw2[index].w = 1.0;
		colorshw2[index] = vertex_colors[4]; //blue
		pointshw2[index+1] = plyread.flyvertex[plyread.flyface[i].point2];
		pointshw2[index+1].w = 1.0;
		colorshw2[index+1] = vertex_colors[4]; //blue
		pointshw2[index+2] = plyread.flyvertex[plyread.flyface[i].point3];
		pointshw2[index+2].w = 1.0;
		colorshw2[index+2] = vertex_colors[4]; //blue
		index+=3;
		i++;
	}
	return index; 
}

void initGPUAndShader( void )
{
	//GPU init
    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

	//Shader Init
	// Load shaders and use the resulting shader program
    program = InitShader( "vshader1.glsl", "fshader1.glsl" );
    glUseProgram( program );

	// sets the default color to clear screen
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
}

void drawply(void)
{
	int tempnumber;
	tempnumber = filldrawing();
	globaltempnumber = tempnumber;

	transpoint2ordinate(pointshw2,tempnumber);

	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	glBufferData( GL_ARRAY_BUFFER, sizeof(point4)*globaltempnumber + sizeof(color4)*globaltempnumber, NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4)*globaltempnumber, pointshw2 );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*globaltempnumber, sizeof(color4)*globaltempnumber, colorshw2 );
	GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    GLuint vColor = glGetAttribLocation( program, "vColor" ); 
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point4)*globaltempnumber)); 
	glEnable( GL_DEPTH_TEST );
	glDrawArrays( GL_TRIANGLES, 0, globaltempnumber );
	glDisable( GL_DEPTH_TEST ); 

	free(currentpointshw2);
	currentpointshw2 = (point4 *) malloc (sizeof(point4)*globaltempnumber);
	memcpy(currentpointshw2,pointshw2,sizeof(point4)*globaltempnumber);
}

void dobreathing(){
	int tempnumber;
	tempnumber = filldrawing();
	globaltempnumber = tempnumber;

	transpoint2ordinate(pointshw2,tempnumber);

	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	point4  facenormals[1];
	int index=0;

	while(index<globaltempnumber){
		facenormals[0].x = (pointshw2[index].y-pointshw2[index+1].y)*(pointshw2[index].z+pointshw2[index+1].z)+
						(pointshw2[index+1].y-pointshw2[index+2].y)*(pointshw2[index+1].z+pointshw2[index+2].z)+
						(pointshw2[index+2].y-pointshw2[index].y)*(pointshw2[index+2].z+pointshw2[index].z);
		facenormals[0].y = (pointshw2[index].z-pointshw2[index+1].z)*(pointshw2[index].x+pointshw2[index+1].x)+
						(pointshw2[index+1].z-pointshw2[index+2].z)*(pointshw2[index+1].x+pointshw2[index+2].x)+
						(pointshw2[index+2].z-pointshw2[index].z)*(pointshw2[index+2].x+pointshw2[index].x);
		facenormals[0].z = (pointshw2[index].x-pointshw2[index+1].x)*(pointshw2[index].y+pointshw2[index+1].y)+
						(pointshw2[index+1].x-pointshw2[index+2].x)*(pointshw2[index+1].y+pointshw2[index+2].y)+
						(pointshw2[index+2].x-pointshw2[index].x)*(pointshw2[index+2].y+pointshw2[index].y);
		facenormals[0].w = 1.0f;

		float normalunit = sqrt(facenormals[0].x*facenormals[0].x+facenormals[0].y*facenormals[0].y+facenormals[0].z*facenormals[0].z);
		facenormals[0].x = facenormals[0].x/normalunit;
		facenormals[0].y = facenormals[0].y/normalunit;
		facenormals[0].z = facenormals[0].z/normalunit;

		pointshw2[index].x = pointshw2[index].x+ facenormals[0].x*normalscale;
		pointshw2[index].y = pointshw2[index].y+ facenormals[0].y*normalscale;
		pointshw2[index].z = pointshw2[index].z+ facenormals[0].z*normalscale;
		pointshw2[index].w = 1.0f;
		colorshw2[index] = vertex_colors[4]; //blue

		pointshw2[index+1].x = pointshw2[index+1].x+ facenormals[0].x*normalscale;
		pointshw2[index+1].y = pointshw2[index+1].y+ facenormals[0].y*normalscale;
		pointshw2[index+1].z = pointshw2[index+1].z+ facenormals[0].z*normalscale;
		pointshw2[index+1].w = 1.0f;
		colorshw2[index+1] = vertex_colors[4]; //blue

		pointshw2[index+2].x = pointshw2[index+2].x+ facenormals[0].x*normalscale;
		pointshw2[index+2].y = pointshw2[index+2].y+ facenormals[0].y*normalscale;
		pointshw2[index+2].z = pointshw2[index+2].z+ facenormals[0].z*normalscale;
		pointshw2[index+2].w = 1.0f;
		colorshw2[index+2] = vertex_colors[4]; //blue

		index = index +3;
	}

	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glBufferData( GL_ARRAY_BUFFER, sizeof(point4)*globaltempnumber + sizeof(color4)*globaltempnumber, NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4)*globaltempnumber, pointshw2 );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*globaltempnumber, sizeof(color4)*globaltempnumber, colorshw2 );
	GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    GLuint vColor = glGetAttribLocation( program, "vColor" ); 
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point4)*globaltempnumber));

	glEnable( GL_DEPTH_TEST );
	glDrawArrays( GL_TRIANGLES, 0, index); 
	glDisable( GL_DEPTH_TEST ); 

	free(currentpointshw2);
	currentpointshw2 = (point4 *) malloc (sizeof(point4)*globaltempnumber);
	memcpy(currentpointshw2,pointshw2,sizeof(point4)*globaltempnumber);
}

void drawnormal(){
	point4  *facenormals;
	int icount=0,index=0,indexnormal;
	
	indexnormal = globaltempnumber/3*2;
	facenormals = (point4 *)malloc(sizeof(point4)*indexnormal);

	while(index<globaltempnumber){
		facenormals[icount].x = (currentpointshw2[index].x+currentpointshw2[index+1].x+currentpointshw2[index+2].x)/3;
		facenormals[icount].y = (currentpointshw2[index].y+currentpointshw2[index+1].y+currentpointshw2[index+2].y)/3;
		facenormals[icount].z = (currentpointshw2[index].z+currentpointshw2[index+1].z+currentpointshw2[index+2].z)/3;
		facenormals[icount].w = 1.0f;

		facenormals[icount+1].x = (currentpointshw2[index].y-currentpointshw2[index+1].y)*(currentpointshw2[index].z+currentpointshw2[index+1].z)+
						(currentpointshw2[index+1].y-currentpointshw2[index+2].y)*(currentpointshw2[index+1].z+currentpointshw2[index+2].z)+
						(currentpointshw2[index+2].y-currentpointshw2[index].y)*(currentpointshw2[index+2].z+currentpointshw2[index].z);
		facenormals[icount+1].y = (currentpointshw2[index].z-currentpointshw2[index+1].z)*(currentpointshw2[index].x+currentpointshw2[index+1].x)+
						(currentpointshw2[index+1].z-currentpointshw2[index+2].z)*(currentpointshw2[index+1].x+currentpointshw2[index+2].x)+
						(currentpointshw2[index+2].z-currentpointshw2[index].z)*(currentpointshw2[index+2].x+currentpointshw2[index].x);
		facenormals[icount+1].z = (currentpointshw2[index].x-currentpointshw2[index+1].x)*(currentpointshw2[index].y+currentpointshw2[index+1].y)+
						(currentpointshw2[index+1].x-currentpointshw2[index+2].x)*(currentpointshw2[index+1].y+currentpointshw2[index+2].y)+
						(currentpointshw2[index+2].x-currentpointshw2[index].x)*(currentpointshw2[index+2].y+currentpointshw2[index].y);
		float normalunit = sqrt(facenormals[icount+1].x*facenormals[icount+1].x+
			facenormals[icount+1].y*facenormals[icount+1].y+facenormals[icount+1].z*facenormals[icount+1].z);
		facenormals[icount+1].x = facenormals[icount+1].x/normalunit;
		facenormals[icount+1].y = facenormals[icount+1].y/normalunit;
		facenormals[icount+1].z = facenormals[icount+1].z/normalunit;
		facenormals[icount+1].x = float(facenormals[icount].x + facenormals[icount+1].x*0.1);
		facenormals[icount+1].y = float(facenormals[icount].y + facenormals[icount+1].y*0.1);
		facenormals[icount+1].z = float(facenormals[icount].z + facenormals[icount+1].z*0.1);
		facenormals[icount+1].w = 1.0f;

		icount = icount+2;
		index = index+3;
	}

	free(pointshw2);
	free(colorshw2);
	pointshw2 = (point4 *)malloc(sizeof(point4)*indexnormal);
	colorshw2 = (color4 *)malloc(sizeof(color4)*indexnormal);

	for(int i=0;i<indexnormal;i++){
		pointshw2[i] = facenormals[i];
		colorshw2[i] = vertex_colors[1]; //red
	}
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glBufferData( GL_ARRAY_BUFFER, sizeof(point4)*indexnormal + sizeof(color4)*indexnormal, NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4)*indexnormal, pointshw2);
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*indexnormal, sizeof(color4)*indexnormal, colorshw2 );
	GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    GLuint vColor = glGetAttribLocation( program, "vColor" ); 
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point4)*indexnormal));  
	glEnable( GL_DEPTH_TEST );
	glDrawArrays( GL_LINES, 0, indexnormal);
	glDisable( GL_DEPTH_TEST ); 

	//recycle
	free(facenormals);
}

void formcuboid(point4 a,point4 b,point4 c,point4 d ){
	colorshw2[globalcuboidnumber] = vertex_colors[3]; pointshw2[globalcuboidnumber] = a; globalcuboidnumber++;
    colorshw2[globalcuboidnumber] = vertex_colors[3]; pointshw2[globalcuboidnumber] = b; globalcuboidnumber++;
    colorshw2[globalcuboidnumber] = vertex_colors[3]; pointshw2[globalcuboidnumber] = c; globalcuboidnumber++;
    colorshw2[globalcuboidnumber] = vertex_colors[3]; pointshw2[globalcuboidnumber] = d; globalcuboidnumber++;
}

void formcuboidpoint(float xmax, float xmin, float ymax, float ymin, float zmax, float zmin){
	point4 cuboidpoint[8];
	//xmin,ymin,zmax
	cuboidpoint[0].x=xmin; cuboidpoint[0].y=ymin; cuboidpoint[0].z=zmax; cuboidpoint[0].w=1.0f;
	//xmax,ymin,zmax
	cuboidpoint[1].x=xmax; cuboidpoint[1].y=ymin; cuboidpoint[1].z=zmax; cuboidpoint[1].w=1.0f;
	//xmax,ymax,zmax
	cuboidpoint[2].x=xmax; cuboidpoint[2].y=ymax; cuboidpoint[2].z=zmax; cuboidpoint[2].w=1.0f;
	//xmin,ymax,zmax
	cuboidpoint[3].x=xmin; cuboidpoint[3].y=ymax; cuboidpoint[3].z=zmax; cuboidpoint[3].w=1.0f;
	//xmin,ymin,zmin
	cuboidpoint[4].x=xmin; cuboidpoint[4].y=ymin; cuboidpoint[4].z=zmin; cuboidpoint[4].w=1.0f;
	//xmax,ymin,zmin
	cuboidpoint[5].x=xmax; cuboidpoint[5].y=ymin; cuboidpoint[5].z=zmin; cuboidpoint[5].w=1.0f;
	//xmax,ymax,zmin
	cuboidpoint[6].x=xmax; cuboidpoint[6].y=ymax; cuboidpoint[6].z=zmin; cuboidpoint[6].w=1.0f;
	//xmin,ymax,zmin
	cuboidpoint[7].x=xmin; cuboidpoint[7].y=ymax; cuboidpoint[7].z=zmin; cuboidpoint[7].w=1.0f;

	formcuboid(cuboidpoint[0],cuboidpoint[1],cuboidpoint[2],cuboidpoint[3]);
	formcuboid(cuboidpoint[1],cuboidpoint[5],cuboidpoint[6],cuboidpoint[2]);
	formcuboid(cuboidpoint[5],cuboidpoint[4],cuboidpoint[7],cuboidpoint[6]);
	formcuboid(cuboidpoint[4],cuboidpoint[0],cuboidpoint[3],cuboidpoint[7]);
	formcuboid(cuboidpoint[2],cuboidpoint[6],cuboidpoint[7],cuboidpoint[3]);
	formcuboid(cuboidpoint[1],cuboidpoint[5],cuboidpoint[4],cuboidpoint[0]);
}

void drawcuboid(){
	int i,index=0,vcount=0;
	globalcuboidnumber = 0;  
	float xmin,xmax,ymin,ymax,zmax,zmin;
	cuboidborder *tempcuboid;

	tempcuboid = (cuboidborder* )malloc(sizeof(cuboidborder)*(globaltempnumber/3));
	while(vcount<globaltempnumber){
		xmin = currentpointshw2[vcount].x;
		xmax = currentpointshw2[vcount].x;
		ymin = currentpointshw2[vcount].y;
		ymax = currentpointshw2[vcount].y;
		zmin = currentpointshw2[vcount].z;
		zmax = currentpointshw2[vcount].z;

		for(i=vcount;i<vcount+3;i++){
			if(currentpointshw2[i].x>xmax) xmax=currentpointshw2[i].x;
			if(currentpointshw2[i].x<xmin) xmin=currentpointshw2[i].x;
			if(currentpointshw2[i].y>ymax) ymax=currentpointshw2[i].y;
			if(currentpointshw2[i].y<ymin) ymin=currentpointshw2[i].y;
			if(currentpointshw2[i].z>zmax) zmax=currentpointshw2[i].z;
			if(currentpointshw2[i].z<zmin) zmin=currentpointshw2[i].z;
		}
		tempcuboid[index].xmax = xmax;
		tempcuboid[index].xmin = xmin;
		tempcuboid[index].ymax = ymax;
		tempcuboid[index].ymin = ymin;
		tempcuboid[index].zmin = zmin;
		tempcuboid[index].zmax = zmax;

		vcount=vcount+3;
		index++;
	}

	int inumber = index*6*4;
	free(pointshw2);
	free(colorshw2);
	pointshw2 = (point4 *)malloc(sizeof(point4)*inumber);
	colorshw2 = (color4 *)malloc(sizeof(color4)*inumber);

	for(i=0;i<index;i++){ 
		formcuboidpoint(tempcuboid[i].xmax,tempcuboid[i].xmin,tempcuboid[i].ymax,
			tempcuboid[i].ymin,tempcuboid[i].zmax,tempcuboid[i].zmin);
	}
		

	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glBufferData( GL_ARRAY_BUFFER, sizeof(point4)*inumber + sizeof(color4)*inumber, NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4)*inumber, pointshw2 );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*inumber, sizeof(color4)*inumber, colorshw2 );

	GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    GLuint vColor = glGetAttribLocation( program, "vColor" ); 
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point4)*inumber));  

	glEnable( GL_DEPTH_TEST );
	glDrawArrays( GL_QUADS, 0, inumber); 
	glDisable( GL_DEPTH_TEST ); 

	//recycle
	free(tempcuboid);
}

void drawbigcuboid(){
	int index=0,vcount=0;
	globalcuboidnumber = 0;  
	float xmin,xmax,ymin,ymax,zmax,zmin;

	xmin = currentpointshw2[vcount].x;
	xmax = currentpointshw2[vcount].x;
	ymin = currentpointshw2[vcount].y;
	ymax = currentpointshw2[vcount].y;
	zmin = currentpointshw2[vcount].z;
	zmax = currentpointshw2[vcount].z;

	while(vcount<globaltempnumber){
		if(currentpointshw2[vcount].x>xmax) xmax=currentpointshw2[vcount].x;
		if(currentpointshw2[vcount].x<xmin) xmin=currentpointshw2[vcount].x;
		if(currentpointshw2[vcount].y>ymax) ymax=currentpointshw2[vcount].y;
		if(currentpointshw2[vcount].y<ymin) ymin=currentpointshw2[vcount].y;
		if(currentpointshw2[vcount].z>zmax) zmax=currentpointshw2[vcount].z;
		if(currentpointshw2[vcount].z<zmin) zmin=currentpointshw2[vcount].z;
	
		vcount=vcount++;
	}
	int inumber = 6*4; 
	free(pointshw2);
	free(colorshw2);
	pointshw2 = (point4 *)malloc(sizeof(point4)*inumber);
	colorshw2 = (color4 *)malloc(sizeof(color4)*inumber);

	formcuboidpoint(xmax,xmin,ymax,ymin,zmax,zmin);
		
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glBufferData( GL_ARRAY_BUFFER, sizeof(point4)*inumber + sizeof(color4)*inumber, NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4)*inumber, pointshw2 );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*inumber, sizeof(color4)*inumber, colorshw2 );

	GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    GLuint vColor = glGetAttribLocation( program, "vColor" ); 
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point4)*inumber));  

	glEnable( GL_DEPTH_TEST );
	glDrawArrays( GL_QUADS, 0, inumber); 
	glDisable( GL_DEPTH_TEST ); 
}

//Martin Newell 
point4 newell(){ 
	point4  facenormals[10];
	int index=0;
	while(index<globaltempnumber){  
		facenormals[index].x = (pointshw2[index].y-pointshw2[index+1].y)*(pointshw2[index].z+pointshw2[index+1].z)+
			(pointshw2[index+1].y-pointshw2[index+2].y)*(pointshw2[index+1].z+pointshw2[index+2].z)+
			(pointshw2[index+2].y-pointshw2[index].y)*(pointshw2[index+2].z+pointshw2[index].z);
		facenormals[index].y = (pointshw2[index].z-pointshw2[index+1].z)*(pointshw2[index].x+pointshw2[index+1].x)+
			(pointshw2[index+1].z-pointshw2[index+2].z)*(pointshw2[index+1].x+pointshw2[index+2].x)+
			(pointshw2[index+2].z-pointshw2[index].z)*(pointshw2[index+2].x+pointshw2[index].x);
		facenormals[index].z = (pointshw2[index].x-pointshw2[index+1].x)*(pointshw2[index].y+pointshw2[index+1].y)+
			(pointshw2[index+1].x-pointshw2[index+2].x)*(pointshw2[index+1].y+pointshw2[index+2].y)+
			(pointshw2[index+2].x-pointshw2[index].x)*(pointshw2[index+2].y+pointshw2[index].y);
		index = index+3;
	}
	return facenormals[10];
}

point4 translatepoint(float left, float right, float bottom, float top, float znear, float zfar, float x, float y, float z){
	float xzoom = 2/(right - left);
	float yzoom = 2/(top - bottom);
	float zzoom = 2/(znear - zfar);
	float xoffset = (right + left)/(right - left);
	float yoffset = (top + bottom)/(top - bottom);
	float zoffset = (zfar + znear)/(zfar - znear);
	float newx = x * xzoom - xoffset;
	float newy = y * xzoom - yoffset;
	float newz = z * xzoom- zoffset;
	//float newy = y * yzoom- yoffset;
	//float newz = z * zzoom- zoffset;
	return point4( newx, newy, newz, 1.0 );
}

void transpoint2ordinate(point4 ppoint[], int pointnumber){
	float ratiox,ratioy,ratioz;
	float xmin,xmax,ymin,ymax,zmax,zmin;
	float xoffset,yoffset,zoffset;

	xmin = ppoint[0].x;
	xmax = ppoint[0].x;
	ymin = ppoint[0].y;
	ymax = ppoint[0].y;
	zmin = ppoint[0].z;
	zmax = ppoint[0].z;

	for(int i=0;i<pointnumber;i++){
		if(ppoint[i].x>xmax) xmax=ppoint[i].x;
		if(ppoint[i].x<xmin) xmin=ppoint[i].x;
		if(ppoint[i].y>ymax) ymax=ppoint[i].y;
		if(ppoint[i].y<ymin) ymin=ppoint[i].y;
		if(ppoint[i].z>zmax) zmax=ppoint[i].z;
		if(ppoint[i].z<zmin) zmin=ppoint[i].z;
	}

	ratiox = 2/(xmax-xmin);
	ratioy = 2/(ymax-ymin);
	ratioz = 2/(zmax-zmin);

	xoffset = xmax - (xmax-xmin)/2;
	yoffset = ymax - (ymax-ymin)/2;
	zoffset = zmax - (zmax-zmin)/2;

	if(ratiox<ratioy){
		for(int i=0;i<pointnumber;i++){
			ppoint[i].x = (ppoint[i].x-xoffset)*ratiox;
			ppoint[i].y = (ppoint[i].y-yoffset)*ratiox;
			ppoint[i].z = (ppoint[i].z)*ratiox;
			//ppoint[i].z = (ppoint[i].z-zoffset)*ratiox;
		}
	}
	
	else{
		for(int i=0;i<pointnumber;i++){
			ppoint[i].x = (ppoint[i].x-xoffset)*ratioy;
			ppoint[i].y = (ppoint[i].y-yoffset)*ratioy;
			ppoint[i].z = (ppoint[i].z)*ratioy;
			//ppoint[i].z = (ppoint[i].z-zoffset)*ratioy;
		}
	}
}


void drawnext()
{
	if(currentmodle<42){
		currentmodle++;
	}
	else currentmodle = 0;
	
	printf("now drawing the %d %s modle\n",currentmodle,allfiles[currentmodle]);
}

void drawpre()
{
	if(currentmodle>0){
		currentmodle--;
	}
	else currentmodle = 42;
	
	printf("now drawing the %d %s modle\n",currentmodle,allfiles[currentmodle]);
}

void displayhw2( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );     // clear the window
	Angel::mat4 perspectiveMat = Angel::Perspective((GLfloat)45.0, (GLfloat)width/(GLfloat)height, (GLfloat)0.1, (GLfloat) 100.0); //mat4是Angel成员函数

	float viewMatrixf[16];
	viewMatrixf[0] = perspectiveMat[0][0];viewMatrixf[4] = perspectiveMat[0][1];
	viewMatrixf[1] = perspectiveMat[1][0];viewMatrixf[5] = perspectiveMat[1][1];
	viewMatrixf[2] = perspectiveMat[2][0];viewMatrixf[6] = perspectiveMat[2][1];
	viewMatrixf[3] = perspectiveMat[3][0];viewMatrixf[7] = perspectiveMat[3][1];

	viewMatrixf[8] = perspectiveMat[0][2];viewMatrixf[12] = perspectiveMat[0][3];
	viewMatrixf[9] = perspectiveMat[1][2];viewMatrixf[13] = perspectiveMat[1][3];
	viewMatrixf[10] = perspectiveMat[2][2];viewMatrixf[14] = perspectiveMat[2][3];
	viewMatrixf[11] = perspectiveMat[3][2];viewMatrixf[15] = perspectiveMat[3][3];
	
	Angel::mat4 modelMat = Angel::identity();
	modelMat = modelMat * Angel::Translate(xaxis, yaxis, zaxis) * Angel::RotateY(yrotate) * Angel::RotateX(xrotate);
	float modelMatrixf[16];
	modelMatrixf[0] = modelMat[0][0];modelMatrixf[4] = modelMat[0][1];
	modelMatrixf[1] = modelMat[1][0];modelMatrixf[5] = modelMat[1][1];
	modelMatrixf[2] = modelMat[2][0];modelMatrixf[6] = modelMat[2][1];
	modelMatrixf[3] = modelMat[3][0];modelMatrixf[7] = modelMat[3][1];

	modelMatrixf[8] = modelMat[0][2];modelMatrixf[12] = modelMat[0][3];
	modelMatrixf[9] = modelMat[1][2];modelMatrixf[13] = modelMat[1][3];
	modelMatrixf[10] = modelMat[2][2];modelMatrixf[14] = modelMat[2][3];
	modelMatrixf[11] = modelMat[3][2];modelMatrixf[15] = modelMat[3][3];

	// set up projection matricies
	GLuint modelMatrix = glGetUniformLocationARB(program, "model_matrix");
	glUniformMatrix4fv( modelMatrix, 1, GL_FALSE, modelMatrixf );
	GLuint viewMatrix = glGetUniformLocationARB(program, "projection_matrix");
	glUniformMatrix4fv( viewMatrix, 1, GL_FALSE, viewMatrixf);

	//begin drawing
	if(isbreathmode){
		dobreathing();
	}
	else{
		drawply();
	}
	if(isnormalmode){
		drawnormal();
	}
	if(iscuboidmodeextend){
		drawcuboid();
	}
	if(iscuboidmode){
		drawbigcuboid();
	}
	glFlush(); // force output to graphics hardware
	free(pointshw2);
	free(colorshw2);

	// use this call to double buffer
	glutSwapBuffers();
	// you can implement your own buffers with textures
}

//----------------------------------------------------------------------------

// keyboard handler
void keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
    case 033:
        exit( EXIT_SUCCESS );
        break;
	case 'W':
		stage = 10;
		initpro(); 
		resetstate(); 
		break;
	case 'N':
		//stage = 11;
		currentrotate = yrotate;
		drawnext();
		break;
	case 'P':
		//stage = 12;
		currentrotate = yrotate;
		drawpre();
		break;
	case 'x':
		stage = 1;
		resetstate();
		dotranslate();
		break;
	case 'X':
		stage = 2;
		resetstate();
		dotranslate();
		break;
	case 'y':
		stage = 3;
		resetstate();
		dotranslate();
		break;
	case 'Y':
		stage = 4;
		resetstate();
		dotranslate();
		break;
	case 'z':
		stage = 5;
		resetstate();
		dotranslate();
		break;
	case 'Z':
		stage = 6;
		resetstate();
		dotranslate();
		break;
	case 'R':
		stage = 7;
		currentrotate = yrotate;
		resetstate();
		dotranslate();
		break;
	case 'B':
		//stage = 8;
		if(isbreathmode) isbreathmode = 0;
		else isbreathmode =1;
		break;
	case 'm':
		if(isnormalmode) isnormalmode = 0;
		else isnormalmode = 1;
		break;
	case 'e':
		if(iscuboidmode) iscuboidmode = 0;
		else iscuboidmode = 1;
		break;
	case 'E': //extent function
		if(iscuboidmodeextend) iscuboidmodeextend = 0;
		else iscuboidmodeextend = 1;
		break;
	}
}
void resetstate()
{
	if(stage == 1){
		Xtranslate =FALSE;
		ytranslate =FALSE;
		Ytranslate =FALSE;
		ztranslate =FALSE;
		Ztranslate =FALSE;
		canyrotate =FALSE;
		//printf("stage %d\n",stage);
		//printf("Xtranslate %s\n",Xtranslate==FALSE?"FALSE":"TRUE");
	}
	else if(stage == 2){
		xtranslate =FALSE;
		ytranslate =FALSE;
		Ytranslate =FALSE;
		ztranslate =FALSE;
		Ztranslate =FALSE;
		canyrotate =FALSE;
	}
	else if(stage == 3){
		xtranslate =FALSE;
		Xtranslate =FALSE;
		Ytranslate =FALSE;
		ztranslate =FALSE;
		Ztranslate =FALSE;
		canyrotate =FALSE;
	}
	else if(stage == 4){
		xtranslate =FALSE;
		Xtranslate =FALSE;
		ytranslate =FALSE;
		ztranslate =FALSE;
		Ztranslate =FALSE;
		canyrotate =FALSE;
	}
	else if(stage == 5){
		xtranslate =FALSE;
		Xtranslate =FALSE;
		ytranslate =FALSE;
		Ytranslate =FALSE;
		Ztranslate =FALSE;
		canyrotate =FALSE;
	}
	else if(stage == 6){
		xtranslate =FALSE;
		Xtranslate =FALSE;
		ytranslate =FALSE;
		Ytranslate =FALSE;
		ztranslate =FALSE;
		canyrotate =FALSE;
	}
	else if(stage == 7){
		xtranslate =FALSE;
		Xtranslate =FALSE;
		ytranslate =FALSE;
		Ytranslate =FALSE;
		ztranslate =FALSE;
		Ztranslate =FALSE;
	}
	else{
		xtranslate =FALSE;
		Xtranslate =FALSE;
		ytranslate =FALSE;
		Ytranslate =FALSE;
		ztranslate =FALSE;
		Ztranslate =FALSE;
		canyrotate =FALSE;
	}

}
void dotranslate()
{
	if(stage == 1){
		if(xtranslate==TRUE) xtranslate = FALSE;
		else xtranslate = TRUE;
	}
	if(stage == 2){
		if(Xtranslate==TRUE) Xtranslate = FALSE;
		else Xtranslate = TRUE;
	}
	if(stage == 3){
		if(ytranslate==TRUE) ytranslate = FALSE;
		else ytranslate = TRUE;
	}
	if(stage == 4){
		if(Ytranslate==TRUE) Ytranslate = FALSE;
		else Ytranslate = TRUE;
	}
	if(stage == 5){
		if(ztranslate==TRUE) ztranslate = FALSE;
		else ztranslate = TRUE;
	}
	if(stage == 6){
		if(Ztranslate==TRUE) Ztranslate = FALSE;
		else Ztranslate = TRUE;
	}
	if(stage == 7){
		if(canyrotate==TRUE) canyrotate = FALSE;
		else canyrotate = TRUE;
	}
	
}

void mouse(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		//yaxis = yaxis-20.0f;
	}
	if(button == GLUT_WHEEL_UP){
		zaxis = zaxis-0.1f;
	}
	if(button== GLUT_WHEEL_DOWN) {
		zaxis = zaxis+0.1f;
	}
	glutPostRedisplay(); 
}

void processMouseActiveMotion (int x, int y){
	glutPostRedisplay();
}

void idlehandle() 
{
	if(isbreathmode){ 
		if(isbigger){ 
			normalscale +=0.0005f;
			if(normalscale>0.05) isbigger=0;
		}
		else{
			normalscale -=0.0005f;
			if(normalscale<0.001) isbigger=1;
		}
	}
	if(stage == 1){
		if(xtranslate){ 
			xaxis = xaxis+0.01f; 
			if(xaxis >2.0f) xaxis = -2.0f; 
		}
		else xaxis = xaxis;
	}
	else if(stage == 2){
		if(Xtranslate){
			xaxis = xaxis-0.01f; 
			if(xaxis <-2.0f) xaxis = 2.0f; 
		}
		else xaxis = xaxis;
	}
	else if(stage == 3){
		if(ytranslate){
			yaxis = yaxis+0.01f; 
			if(yaxis >2.0f) yaxis = -2.0f; 
		}
		else yaxis = yaxis;
	}
	else if(stage == 4){
		if(Ytranslate){
			yaxis = yaxis-0.01f; 
			if(yaxis <-2.0f) yaxis = 2.0f; 
		}
		else yaxis = yaxis;
	}
	else if(stage == 5){
		if(ztranslate) {
			zaxis = zaxis+0.02f;  
			if(zaxis >2.0f) zaxis = -4.0f; 
		}
		else zaxis = zaxis;
	}
	else if(stage == 6){
		if(Ztranslate){
			zaxis = zaxis-0.02f; 
			if(zaxis <-4.0f) zaxis = 1.0f; 
		}
		else zaxis = zaxis;
	}
	else if(stage == 7){
		if(canyrotate){
			if(isclockwise){
				yrotate = yrotate+3.0f;
				if( yrotate - currentrotate > 360.0f ){
					yrotate -= 360.0f;
					drawnext(); 
					isclockwise = 0;
				}
			}
			else{
				yrotate = yrotate-3.0f;
				if( currentrotate-yrotate > 360.0f ){
					yrotate += 360.0f;
					drawnext(); 
					isclockwise = 1;
				}
			}
		}
		else yrotate = yrotate;
	}
	glutPostRedisplay(); 
}

void initpro(){
	yaxis = 0.0f;
	xaxis = 0.0f;
	zaxis = -2.0f;
	yrotate = 0.0f;
	xrotate = 0.0f;
	normalscale = 0.0f;
	isclockwise = 1;
	//glEnable(GL_DEPTH_TEST); 
	currentmodle = 11;  //init windows is cow
}
//----------------------------------------------------------------------------
// entry point
int main( int argc, char **argv )
{
	// init glut
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
	width = 512;
	height = 512;
    // If you are using freeglut, the next two lines will check if 
    // the code is truly 3.2. Otherwise, comment them out
    
	// should run a test here 
	// with different cases
	// this is a good time to find out information about
	// your graphics hardware before you allocate any memory
    glutInitContextVersion( 3, 1 );
    glutInitContextProfile( GLUT_CORE_PROFILE );

	// create window
	// opengl can be incorperated into other packages like wxwidgets, fltoolkit, etc.
    glutCreateWindow( "Color Cube" );

	// init glew
    glewInit();

    //generateGeometry();
	initGPUAndShader();

	// assign handlers
	initpro();
	
    glutDisplayFunc( displayhw2 );
	glutIdleFunc(idlehandle);
	glutMouseFunc(mouse);
	glutMotionFunc(processMouseActiveMotion);
    glutKeyboardFunc( keyboard );
	// should add menus
	// add mouse handler
	// add resize window functionality (should probably try to preserve aspect ratio)

	// enter the drawing loop
	// frame rate can be controlled with 
    glutMainLoop();
    return 0;
}
