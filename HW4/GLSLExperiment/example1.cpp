// Two-Dimensional Sierpinski Gasket       
// Generated using randomly selected vertices and bisection

#include "Angel.h"
#include "bmpread.h"

//----------------------------------------------------------------------------
#define  GLUT_WHEEL_UP 3           //mouse wheel 
#define  GLUT_WHEEL_DOWN 4

//////////
typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
typedef Angel::vec3  point3;
struct face3{
	int face;
	int point1;
	int point2;
	int point3;
};
typedef struct{             
	int  facenum;
	int	 vertexnum;
	point4 *flyvertex;  
	face3 *flyface; 
}Plyfile;  
typedef struct{             
	char repold;
	char repnew;
}repstr; 
typedef struct{             
	char ruleold;
	char rulenew[100];
}rulestr;  //difine rep
typedef struct{             
	int  len;
	int	 iter;
	point4 rot; 
	repstr rep[10];
	int	repcount;
	char lstart;
	rulestr lrule[10];
	int rulecount;
}Lsysfile;  //define Lsysfile
typedef struct{             
	mat4 cmatrix;
	point4 cpoints;
	point4 corientation;
}cstate;  //the stuctrue for push state
//define constant valve
using namespace std;

int width = 0;
int height = 0;
float xaxis,yaxis,zaxis;
float yrotate,xrotate,currentrotate;
char *allfiles[45] =  {"airplane.ply", "ant.ply", "apple.ply", "balance.ply", "beethoven.ply", "big_atc.ply", "big_dodge.ply", "big_porsche.ply",
     "big_spider.ply", "canstick.ply", "chopper.ply", "cow.ply", "dolphins.ply", "egret.ply", "f16.ply", "footbones.ply", "fracttree.ply",
     "galleon.ply", "hammerhead.ply", "helix.ply", "hind.ply", "kerolamp.ply", "ketchup.ply", "mug.ply", "part.ply", "pickup_big.ply",
	 "pump.ply", "pumpa_tb.ply", "sandal.ply", "saratoga.ply","scissors.ply", "shark.ply","steeringweel.ply","stratocaster.ply","street_lamp.ply",
	 "teapot.ply","tennis_shoe.ply","tommygun.ply","trashcan.ply","turbine.ply","urn2.ply","walkman.ply","weathervane.ply","cylinder.ply","sphere.ply"};
int currentmodle;
char *plypath = "ply_files\\\\"; 
int globaltempnumber,globalcuboidnumber;
int globalmodelmatrix = 0;
mat4 modlematrixstack[1000]; //most 1000 stack state
int globalstate = 0;
cstate statestack[1000]; //most 1000 stack state
char *allsysf[5] = {"lsys1.txt","lsys2.txt","lsys3.txt","lsys4.txt","lsysmyown"};
char *lsysplypath = "lsys_files\\\\";
float displayglobal = 85;
int drawtarget = 0; //1:sphere, 2:cylinder else:0
int globalcolor;
char currenttree;
mat4 global_matrix;
GLuint BmpProjection;
Angel::mat4 lookMat;
float lookMatrixf[16];
GLuint lookMatrix; 
GLuint modelMatrix;
Angel::mat4 perspectiveMat;
char *plain_texture = "grass";
int shadermode = 1;
GLuint projcetMatrix;
bmpread_t bitmap[6];
bool fogflag = false;
int irandom,irandom2;

// handle to program
GLuint program;
GLuint global_program;
GLuint global_program1;
GLuint global_program2;
GLuint global_program3;
GLuint global_program4;
GLuint global_program9;
point4 *pointshw2; 
vec4 *normalhw2;
color4 *colorshw2;
point4 normals[100];
point4 points[100];
point4 texcube[100];
float ddddd = -20;
bool shadowmode = TRUE;
GLuint reflect_flag = false;
GLuint refract_flag = false;
GLuint ground_flag = false;

// remember to prototype
void generateGeometry( void );
void keyboard( unsigned char key, int x, int y );
Plyfile readplyfile( char *);
Lsysfile readlsysfile( char *);
int  filldrawing(void);
void drawply(void);
void initGPUAndShader( void );
void displayhw3( void );
void initpro( void ); //init state
void transpoint2ordinate(point4 *, int );
point4 translatepoint(float left, float right, float bottom, float top, float znear, float zfar, float x, float y, float z);
char* analyzelsys(Lsysfile );
void PushMatrix(mat4 );
mat4 PopMatrix();
void PushState(cstate );
cstate PopState();
void drawtree(char, float, float, float );
void drawplain();
void Draw_Polygon(int, float, float, float );
void Draw_Polygon_Re(int, float, float, float );
void drawpark();
void quad(int a, int b, int c, int d);
void DrawPlain( void );
void DrawPlain2( void );
void SetShader(void);
void SetShader1(void);
void envmap(void);


// RGBA olors
color4 vertex_colorshw3[7] = {
    color4( 0.0, 0.0, 0.0, 1.0 ),  // black
    color4( 1.0, 0.0, 0.0, 1.0 ),  // red
    color4( 1.0, 1.0, 0.0, 1.0),  // yellow
    color4( 0.0, 1.0, 0.0, 1.0 ),  // green
    color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
    color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
    color4( 0.0, 1.0, 1.0, 1.0 )   // cyan
};

color4 vertex_colors[10] = {
    color4( 0.0, 0.0, 0.0, 1.0 ),  // black
    color4( 1.0, 0.0, 0.0, 1.0 ),  // red
    color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
    color4( 0.0, 1.0, 0.0, 1.0 ),  // green
    color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
    color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
    color4( 1.0, 1.0, 1.0, 1.0 ),  // white
    color4( 0.0, 1.0, 1.0, 1.0 ),   // cyan
	color4( 1.0, 0.5, 0.0, 1.0 ), // brown
	color4( 0.5, 0.5, 0.5, 1.0 ) // the plain of park,gray
};

Lsysfile readlsysfile(char *filename)
{
	char *name;
	FILE *fp = NULL;
	Lsysfile lsysf;
	int icount;
	int repcount,rulecount;
	char line[256];

	//init
	repcount = 0;
	rulecount = 0;
	icount = 0;

	name = (char *) malloc (sizeof (char) * (strlen (filename) + 5));
	strcpy (name, filename);
	if (strlen (name) < 4 ||strcmp (name + strlen (name) - 4, ".txt") != 0) 
		strcat (name, ".txt"); //if need txt, complete it

	fp = fopen(filename,"r");
	if(fp == NULL){
		printf("can not open the file.\n");
		exit(0);
	}

	while(!feof(fp)){
		memset(line,0,256);
		fscanf(fp,"%s",&line);
		if(strcmp(line,"#")==0){
			fgets(line,255,fp);
		}
		else if(strcmp(line,"len:")==0){
			fscanf(fp,"%d",&lsysf.len);
		}
		else if(strcmp(line,"iter:")==0){
			fscanf(fp,"%d",&lsysf.iter);
		}
		else if(strcmp(line,"start:")==0){
			fgetc(fp); //remove blank
			fscanf(fp,"%c",&lsysf.lstart);
		}
		else if(strcmp(line,"rot:")==0){
			fscanf(fp,"%f %f %f",&lsysf.rot.x,&lsysf.rot.y,&lsysf.rot.z);
			lsysf.rot.w = 1.0f;
		}
		else if(strcmp(line,"rep:")==0){
			fgetc(fp); //remove blank
			fscanf(fp,"%c",&lsysf.rep[repcount].repold);
			fgetc(fp); //remove ,
			fscanf(fp,"%c",&lsysf.rep[repcount].repnew);
			if(lsysf.rep[repcount].repnew == '\n'){
				printf("The new rep is empty\n");
			}
			repcount++;
		}
		else{ //left right tree
			if(line[0] == '\n'||line[0] == '\t'||line[0] == '\0'){ //the real last element is \0
				printf("End reading\n");
				break;
			}
			lsysf.lrule[rulecount].ruleold = line[0];
			fscanf(fp,"%s",&lsysf.lrule[rulecount].rulenew);
			rulecount++;
		}
	}
	
	lsysf.repcount = repcount;
	lsysf.rulecount = rulecount;

	fclose(fp);
	return lsysf;
}

char* analyzelsys(Lsysfile lsysf){ //analyze L,R,A,B,C,D
	int i,j,k;
	bool changeflag;
	char tempstring[20000]="\0";
	char finalstring[20000]="\0";

	for(i=0;i<lsysf.iter;i++){
		//init
		k=0;
		finalstring[0] = lsysf.lstart;
		changeflag = FALSE;
		//loop
		while(finalstring[k]!=NULL){
			for(j=0;j<lsysf.rulecount;j++){
				if(finalstring[k]==lsysf.lrule[j].ruleold){
					strcat(tempstring,lsysf.lrule[j].rulenew);
					changeflag = TRUE;
					break;
				}	
			}
			if(!changeflag){ //if no change happened
				tempstring[strlen(tempstring)]=finalstring[k];
			}
			changeflag = FALSE;
			k++;
		}
		strcpy(finalstring,tempstring);
		memset(tempstring,0,20000);
	}
	//printf("The finalstring after rule replace: %s\n",finalstring);

	if(lsysf.repcount>0){
		//init
		k=0;
		changeflag = FALSE;
		memset(tempstring,0,20000);
		//loop
		while(finalstring[k]!=NULL){
			for(j=0;j<lsysf.repcount;j++){
				if(finalstring[k]==lsysf.rep[j].repold){
					if(lsysf.rep[j].repnew == '\n'){//if empty,just go
						changeflag = TRUE;
						break;
					}
					else{
						tempstring[strlen(tempstring)] = lsysf.rep[j].repnew;
						changeflag = TRUE;
						break;
					}	
				}	
			}
			if(!changeflag){ //if no change
				tempstring[strlen(tempstring)] = finalstring[k];
			}
			changeflag = FALSE;
			k++;
		}
		strcpy(finalstring,tempstring);
	}
	//printf("The finalstring after rep replace: %s\n",finalstring);
	return finalstring;
}

void displayhw3(void){
	//clear screen
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	//glClear( GL_COLOR_BUFFER_BIT);

	perspectiveMat = Angel::Perspective((GLfloat)65, (GLfloat)width/(GLfloat)height, (GLfloat)0.1, (GLfloat) 100.0); 
	projcetMatrix = glGetUniformLocationARB(global_program, "projection_matrix");
	//glUniformMatrix4fv( viewMatrix, 1, GL_TRUE, perspectiveMat);

	lookMat = Angel::LookAt( //no use, nothing happened
		Angel::vec3(0,0.5,2), // Camera is at (4,3,3), in World Space
		Angel::vec3(0,0,0), // and looks at the origin
		Angel::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	lookMatrixf[0] = lookMat[0][0];lookMatrixf[4] = lookMat[0][1];
	lookMatrixf[1] = lookMat[1][0];lookMatrixf[5] = lookMat[1][1];
	lookMatrixf[2] = lookMat[2][0];lookMatrixf[6] = lookMat[2][1];
	lookMatrixf[3] = lookMat[3][0];lookMatrixf[7] = lookMat[3][1];

	lookMatrixf[8] = lookMat[0][2];lookMatrixf[12] = lookMat[0][3];
	lookMatrixf[9] = lookMat[1][2];lookMatrixf[13] = lookMat[1][3];
	lookMatrixf[10] = lookMat[2][2];lookMatrixf[14] = lookMat[2][3];
	lookMatrixf[11] = lookMat[3][2];lookMatrixf[15] = lookMat[3][3];
	
	//lookMatrix = glGetUniformLocationARB(program, "look_matrix");
	//glUniformMatrix4fv( lookMatrix, 1, GL_FALSE, lookMatrixf);


	
	//envmap();

	Draw_Polygon_Re(7,0,0,-0.2);
	Draw_Polygon_Re(5, -3.5, 0.0, -6.0);
	drawtree('d', -1.3, 0.0, -1.5);
	drawtree('c', 3.5, 0.0, -4.0);
	DrawPlain2();
	//Draw_Polygon_Re(5, -3.5, 0.0, -6.0);
	/*if(shadermode ==2||shadermode==3){
		//envmap();
		Draw_Polygon_Re(7,0,0,-1);
		Draw_Polygon_Re(5, -3.5, 0.0, -6.0);
	}
	//Draw_Polygon(5, -3.0, 0.0, -4.0);
	//Draw_Polygon(6, 2.0, 0.0, 3.0);
	else{
		Draw_Polygon(7, 0.0, 0.0, -1);
		Draw_Polygon(5, -3.5, 0.0, -6.0);
	}*/
	
	//Draw_Polygon(11, -1.0, 0.0, -6.0);
	
	
	
	//glFlush();
	glutSwapBuffers(); 

	//glDeleteProgram(program);
	//glDeleteProgram(global_program);
	//glDisable(GL_TEXTURE_CUBE_MAP);
}

void Draw_Polygon_Re(int polynum, float startx, float starty, float startz){
	GLuint refleOn;
	GLuint refraOn;
	if(shadermode == 2){
		refleOn = glGetUniformLocationARB(global_program, "refleOn");
		glUniform1i(refleOn, true );
	}
	else if(shadermode == 3){
		refraOn = glGetUniformLocationARB(global_program, "refraOn");
		glUniform1i(refraOn, true );
	}
	else if(shadermode == 4){
		irandom = rand()%5;
		if(irandom == 1||irandom==3||irandom==5){
			refraOn = glGetUniformLocationARB(global_program, "refraOn");
			glUniform1i(refraOn, true );
		}
		else{
			refleOn = glGetUniformLocationARB(global_program, "refleOn");
			glUniform1i(refleOn, true );
		}
	}
	

	/*//glUseProgram( global_program );
	
	//glCullFace(GL_BACK);                        //背面裁剪(背面不可见)
    //glEnable(GL_CULL_FACE);                        //启用裁剪
	//glEnable(GL_TEXTURE_CUBE_MAP); //启用立方体贴图
	//glDisable(GL_TEXTURE_CUBE_MAP); // 禁用立方体贴图

	GLuint  tex[6];
	glGenTextures( 6, tex ); // 分配6个纹理对象的编号

	//如果不使用glBindTexture函数，那么glTexImage*、glTexParameter*、glTexCoord*系列函数默认在一个编号为0的纹理对象上进行操作
	//函数有两个参数，第一个参数是需要使用纹理的目标,第二个参数是所使用的纹理的编号
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex[0]);
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);// 啥用
	//active
	glActiveTexture(GL_TEXTURE10); //为什么要这行
   
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X ,0,GL_RGB,bitmap[0].width,bitmap[0].height,0,GL_RGB,GL_UNSIGNED_BYTE, bitmap[1].rgb_data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X ,0,GL_RGB,bitmap[1].width,bitmap[1].height,0,GL_RGB,GL_UNSIGNED_BYTE, bitmap[0].rgb_data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y ,0,GL_RGB,bitmap[2].width,bitmap[2].height,0,GL_RGB,GL_UNSIGNED_BYTE, bitmap[2].rgb_data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y ,0,GL_RGB,bitmap[3].width,bitmap[3].height,0,GL_RGB,GL_UNSIGNED_BYTE, bitmap[3].rgb_data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z ,0,GL_RGB,bitmap[4].width,bitmap[4].height,0,GL_RGB,GL_UNSIGNED_BYTE, bitmap[5].rgb_data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z ,0,GL_RGB,bitmap[5].width,bitmap[5].height,0,GL_RGB,GL_UNSIGNED_BYTE, bitmap[4].rgb_data);
	// 设定缩放器的过滤参数
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,GL_NEAREST); //GL_LINEAR有计算，效果好
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//GL_TEXTURE_MAG_FILTER指当纹理图象被使用到一个大于它的形状上时，GL_TEXTURE_MIN_FILTER指当纹理图象被使用到一个小于（或等于）它的形状上时
	//glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    
	//bmpread_free(bitmap);

	GLuint texMapLocation = glGetUniformLocation(global_program, "texMap");
	glUniform1i(texMapLocation, tex[0]);

	//glActiveTexture(GL_TEXTURE11);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, tex[0]);*/

	GLuint projcetMatrix = glGetUniformLocationARB(global_program, "projection_matrix");
	glUniformMatrix4fv( projcetMatrix, 1, GL_TRUE, perspectiveMat);
	GLuint lookMatrix = glGetUniformLocationARB(global_program, "look_matrix");
	glUniformMatrix4fv( lookMatrix, 1, GL_TRUE, lookMat);
	Angel::mat4 modelMat = Angel::identity() * Angel::Translate(startx, starty, startz) * Angel::RotateY(0.0f) * Angel::Scale(1) ;
	GLuint modelMatrix = glGetUniformLocationARB(global_program, "model_matrix");
	glUniformMatrix4fv( modelMatrix, 1, GL_TRUE, modelMat );

	//draw car
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	int tempnumber;
	currentmodle = polynum; //5,6,7,11
	tempnumber = filldrawing();
	globaltempnumber = tempnumber;

	transpoint2ordinate(pointshw2,tempnumber);
	glBufferData( GL_ARRAY_BUFFER, sizeof(point4)*globaltempnumber + sizeof(color4)*globaltempnumber+sizeof(vec4)*globaltempnumber, NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4)*globaltempnumber, pointshw2 );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*globaltempnumber, sizeof(color4)*globaltempnumber, colorshw2 );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*globaltempnumber+sizeof(color4)*globaltempnumber, sizeof(vec4)*globaltempnumber, normalhw2 );
	GLuint vPosition = glGetAttribLocation( global_program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    GLuint vColor = glGetAttribLocation( global_program, "vColor" ); 
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point4)*globaltempnumber)); 
	GLuint vNormal = glGetAttribLocation( global_program, "vNormal" ); 
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point4)*globaltempnumber+sizeof(color4)*globaltempnumber));
	glEnable( GL_DEPTH_TEST );
	glDrawArrays( GL_TRIANGLES, 0, globaltempnumber );
	glDisable( GL_DEPTH_TEST );

	//draw car shadow
	if(shadowmode == TRUE){
		GLuint drawshadow = glGetUniformLocationARB(global_program, "drawshadow");
		glUniform1i(drawshadow, true );

		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE); //用line画影子比较像
		float light[3] = {5,5,5}; // location of light
		mat4 m = Angel::identity();
		m[3][1] = -1.0/light[1];
		m[3][3] = 0.0;
		mat4 tempview = lookMat;
		tempview = tempview * Angel::Translate(light[0],light[1],light[2]) * m * Angel::Translate(-light[0],-light[1],-light[2]);
		lookMatrix = glGetUniformLocationARB(global_program, "look_matrix");
		glUniformMatrix4fv( lookMatrix, 1, GL_TRUE, tempview );

		//怎么把影子在fog的shader里变黑
		for(int icount=0;icount<globaltempnumber;icount++){
			colorshw2[icount] = vertex_colors[0];
		}
		glBufferData( GL_ARRAY_BUFFER, sizeof(point4)*globaltempnumber + sizeof(color4)*globaltempnumber+sizeof(vec4)*globaltempnumber, NULL, GL_STATIC_DRAW );
		glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4)*globaltempnumber, pointshw2 );
		glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*globaltempnumber, sizeof(color4)*globaltempnumber, colorshw2 );
		glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*globaltempnumber+sizeof(color4)*globaltempnumber, sizeof(vec4)*globaltempnumber, normalhw2 );
		vPosition = glGetAttribLocation( global_program, "vPosition" );
		glEnableVertexAttribArray( vPosition );
		glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
		vColor = glGetAttribLocation( global_program, "vColor" ); 
		glEnableVertexAttribArray( vColor );
		glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point4)*globaltempnumber)); 
		vNormal = glGetAttribLocation( global_program, "vNormal" ); 
		glEnableVertexAttribArray( vNormal );
		glVertexAttribPointer( vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point4)*globaltempnumber+sizeof(color4)*globaltempnumber));
		//draw shadow
		glEnable( GL_DEPTH_TEST );
		glDrawArrays( GL_TRIANGLES, 0, globaltempnumber );
		glDisable( GL_DEPTH_TEST );

		glUniform1i(drawshadow, false );
	}
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	//glutSwapBuffers(); 
	//glFlush();

	//recycle
	free(pointshw2);
	free(colorshw2);
	free(normalhw2);
	glUniformMatrix4fv( lookMatrix, 1, GL_TRUE, lookMat );

	refleOn = glGetUniformLocationARB(global_program, "refleOn");
	glUniform1i(refleOn, false );
	refraOn = glGetUniformLocationARB(global_program, "refraOn");
	glUniform1i(refraOn, false );

}

void Draw_Polygon(int polynum, float startx, float starty, float startz){
	GLuint projcetMatrix = glGetUniformLocationARB(global_program, "projection_matrix");
	glUniformMatrix4fv( projcetMatrix, 1, GL_TRUE, perspectiveMat);
	GLuint lookMatrix = glGetUniformLocationARB(global_program, "look_matrix");
	glUniformMatrix4fv( lookMatrix, 1, GL_TRUE, lookMat);
	Angel::mat4 modelMat = Angel::identity() * Angel::Translate(startx, starty, startz) * Angel::RotateY(0.0f) * Angel::Scale(1) ;
	GLuint modelMatrix = glGetUniformLocationARB(global_program, "model_matrix");
	glUniformMatrix4fv( modelMatrix, 1, GL_TRUE, modelMat );

	//draw car
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	int tempnumber;
	currentmodle = polynum; //5,6,7,11
	tempnumber = filldrawing();
	globaltempnumber = tempnumber;

	transpoint2ordinate(pointshw2,tempnumber);
	glBufferData( GL_ARRAY_BUFFER, sizeof(point4)*globaltempnumber + sizeof(color4)*globaltempnumber+sizeof(vec4)*globaltempnumber, NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4)*globaltempnumber, pointshw2 );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*globaltempnumber, sizeof(color4)*globaltempnumber, colorshw2 );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*globaltempnumber+sizeof(color4)*globaltempnumber, sizeof(vec4)*globaltempnumber, normalhw2 );
	GLuint vPosition = glGetAttribLocation( global_program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    GLuint vColor = glGetAttribLocation( global_program, "vColor" ); 
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point4)*globaltempnumber)); 
	GLuint vNormal = glGetAttribLocation( global_program, "vNormal" ); 
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point4)*globaltempnumber+sizeof(color4)*globaltempnumber));
	glEnable( GL_DEPTH_TEST );
	glDrawArrays( GL_TRIANGLES, 0, globaltempnumber );
	glDisable( GL_DEPTH_TEST );

	//draw car shadow
	if(shadowmode == TRUE){
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL); //用line画影子比较像
		float light[3] = {5,5,5}; // location of light
		mat4 m = Angel::identity();
		m[3][1] = -1.0/light[1];
		m[3][3] = 0.0;
		mat4 tempview = lookMat;
		tempview = tempview * Angel::Translate(light[0],light[1],light[2]) * m * Angel::Translate(-light[0],-light[1],-light[2]);
		lookMatrix = glGetUniformLocationARB(global_program, "look_matrix");
		glUniformMatrix4fv( lookMatrix, 1, GL_TRUE, tempview );

		//怎么把影子在fog的shader里变黑
		for(int icount=0;icount<globaltempnumber;icount++){
			colorshw2[icount] = vertex_colors[0];
		}
		glBufferData( GL_ARRAY_BUFFER, sizeof(point4)*globaltempnumber + sizeof(color4)*globaltempnumber+sizeof(vec4)*globaltempnumber, NULL, GL_STATIC_DRAW );
		glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4)*globaltempnumber, pointshw2 );
		glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*globaltempnumber, sizeof(color4)*globaltempnumber, colorshw2 );
		glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*globaltempnumber+sizeof(color4)*globaltempnumber, sizeof(vec4)*globaltempnumber, normalhw2 );
		vPosition = glGetAttribLocation( global_program, "vPosition" );
		glEnableVertexAttribArray( vPosition );
		glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
		vColor = glGetAttribLocation( global_program, "vColor" ); 
		glEnableVertexAttribArray( vColor );
		glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point4)*globaltempnumber)); 
		vNormal = glGetAttribLocation( global_program, "vNormal" ); 
		glEnableVertexAttribArray( vNormal );
		glVertexAttribPointer( vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point4)*globaltempnumber+sizeof(color4)*globaltempnumber));
		//draw shadow
		glEnable( GL_DEPTH_TEST );
		glDrawArrays( GL_TRIANGLES, 0, globaltempnumber );
		glDisable( GL_DEPTH_TEST );
	}
	
	//glutSwapBuffers(); 
	//glFlush();

	//recycle
	free(pointshw2);
	free(colorshw2);
	free(normalhw2);
	glUniformMatrix4fv( lookMatrix, 1, GL_TRUE, lookMat );
}

void drawpark(){
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
	char drawchar;
	int randomchar;

	drawplain();

	//drawtree('d');
	//drawtree('c');
	for(int i=0;i<5;i++){
		randomchar = rand()%5;
		if(randomchar == 0) drawchar='a';
		else if(randomchar == 1) drawchar='b';
		else if(randomchar == 2) drawchar='c';
		else if(randomchar == 3) drawchar='d';
		else if(randomchar == 4) drawchar='e';

		//drawtree(drawchar);
	}
	for(int i=0;i<5;i++){
		//drawcar();
	}
	//glFlush();
}

void drawplain(){
	Angel::mat4 modelMat = Angel::identity();//load 1,1,1,1 unit matrix
	//modelMat = modelMat*Angel::Translate(0, 0, -2.0f)*Angel::RotateZ(0.0f) * Angel::RotateX(28.0f)*Angel::Scale(1);
	modelMat = modelMat* Angel::RotateX(0.0f)*Angel::Scale(1);
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
	glUniformMatrix4fv( modelMatrix, 1, GL_FALSE, modelMat );

	int sizesize = 50;
	pointshw2 = (point4 *) malloc (sizeof(point4)*sizesize);
	colorshw2 = (color4 *) malloc (sizeof(color4)*sizesize);
	for(int icount=0;icount<sizesize;icount++){ 
		colorshw2[icount] = vertex_colors[9]; 
	}
	//why why -3
	pointshw2[0].x = -10.0f; pointshw2[0].y = 0.0f; pointshw2[0].z = -20.0f; pointshw2[0].w = 1.0f;
	pointshw2[1].x = -10.0f; pointshw2[1].y = 0.0f; pointshw2[1].z =1.0f; pointshw2[1].w = 1.0f;
	pointshw2[2].x = 10.0f; pointshw2[2].y = 0.0f; pointshw2[2].z = 1.0f; pointshw2[2].w = 1.0f;
	pointshw2[3].x = 10.0f; pointshw2[3].y = 0.0f; pointshw2[3].z = -20.0; pointshw2[3].w = 1.0f;

	//glPolygonMode(GL_FRONT, GL_FILL); // fill mode      
    //glPolygonMode(GL_BACK, GL_LINE);  // line mode
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); 
	////read from buffer
	glBufferData( GL_ARRAY_BUFFER, sizeof(point4)*sizesize + sizeof(color4)*sizesize, NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4)*sizesize, pointshw2 );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*sizesize, sizeof(color4)*sizesize, colorshw2 );
	//reset buffer position
	GLuint vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	GLuint vColor = glGetAttribLocation( program, "vColor" ); 
	glEnableVertexAttribArray( vColor );
	glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point4)*sizesize));  //dynamic allocate memeory
	//glVertexAttrib4f(vColor, 1.0f, 0.0f, 0.0f, 1.0f);
	//glEnable( GL_DEPTH_TEST ); 
	glDrawArrays( GL_QUADS, 0, sizesize );
	//glDisable( GL_DEPTH_TEST ); 
	//glFlush(); // force output to graphics hardware
 
	//recollect memeory
	free(pointshw2);
	free(colorshw2);
	//free(normalhw2);
}

void drawbet2point(point4 ppoint1, point4 ppoint2)  
{  

	float scalesize = 0.002;

	Angel::mat4 modelMat = Angel::identity();//load 1,1,1,1 unit matrix
	//if(currenttree == 'e'||currenttree == 'a'){ //a tree and e tree lose weight
	//	scalesize = 0.001;
	//}
	modelMat = modelMat * Angel::Translate(ppoint1)* Angel::Scale(scalesize*5);//pay attention to order
	//scalesize = 0.002;

	float modelMatrixf[16];
	modelMatrixf[0] = modelMat[0][0];modelMatrixf[4] = modelMat[0][1];
	modelMatrixf[1] = modelMat[1][0];modelMatrixf[5] = modelMat[1][1];
	modelMatrixf[2] = modelMat[2][0];modelMatrixf[6] = modelMat[2][1];
	modelMatrixf[3] = modelMat[3][0];modelMatrixf[7] = modelMat[3][1];
	modelMatrixf[8] = modelMat[0][2];modelMatrixf[12] = modelMat[0][3];
	modelMatrixf[9] = modelMat[1][2];modelMatrixf[13] = modelMat[1][3];
	modelMatrixf[10] = modelMat[2][2];modelMatrixf[14] = modelMat[2][3];
	modelMatrixf[11] = modelMat[3][2];modelMatrixf[15] = modelMat[3][3];
	modelMatrix = glGetUniformLocationARB(global_program, "model_matrix");
	glUniformMatrix4fv( modelMatrix, 1, GL_FALSE, modelMatrixf );
	global_matrix = modelMat;
	//global_matrix = Angel::identity()*Angel::Translate(ppoint1);

	currentmodle = 44;
	drawtarget = 1;
	drawply(); //the diameter of sphere from -1 to 1

	//caculate target vector 
	vec3 targetvector;
	targetvector.x = ppoint2.x - ppoint1.x;  
	targetvector.y = ppoint2.y - ppoint1.y;
	targetvector.z = ppoint2.z - ppoint1.z;

	vec3 axisZStart = vec3(0, 0, -1.0f);
	vec3 axisZEnd = targetvector;
    float moveangle = Angel::dot(normalize(axisZStart), normalize(axisZEnd));
	double moveradius =  acos(moveangle);
	vec3 axisRotate = Angel::cross(axisZStart,axisZEnd);
	float veclength = Angel::length(targetvector);
	float veczoom = veclength/2;
	float axislength = Angel::length(axisRotate);
	/***********************************************************************
			|	x^2*(1-c)+c	x*y*(1-c)-z*s	x*z*(1-c)+y*s	0	|
	R = 	|	y*x*(1-c)+z*s	y^2*(1-c)+c	y*z*(1-c)-x*s	0	|
			|	x*z*(1-c)-y*s	y*z*(1-c)+x*s	z^2*(1-c)+c	0	|
 			|	0				0				0			1	|
	[x, y, z] - a vector about which the rotation should be done
	c - cos(alpha)
	s - sin(alpha)
	alpha - rotation angle
	***********************************************************************/
	mat4 rotate_matrix;
	float c = cos(moveradius);
	float s = sin(moveradius);
	float x = axisRotate.x/axislength;
	float y = axisRotate.y/axislength;
	float z = axisRotate.z/axislength;
	rotate_matrix[0][0]= x*x*(1-c)+c;
	rotate_matrix[0][1]= x*y*(1-c)-z*s;
	rotate_matrix[0][2]= x*z*(1-c)+y*s;
	rotate_matrix[0][3]= 0;
	rotate_matrix[1][0]= y*x*(1-c)+z*s;
	rotate_matrix[1][1]= y*y*(1-c)+c;
	rotate_matrix[1][2]= y*z*(1-c)-x*s;
	rotate_matrix[1][3]= 0;
	rotate_matrix[2][0]= x*z*(1-c)-y*s;
	rotate_matrix[2][1]= y*z*(1-c)+x*s;
	rotate_matrix[2][2]= z*z*(1-c)+c;
	rotate_matrix[2][3]= 0;
	rotate_matrix[3][0]= 0;
	rotate_matrix[3][1]= 0;
	rotate_matrix[3][2]= 0;
	rotate_matrix[3][3]= 1;

	//scalesize = 0.003;
	//if(currenttree!='b'){//highlight tree truck effect except b tree
	//	if(abs(targetvector.x)<=0.01&&abs(targetvector.z)<=0.01){//highlight tree truck 
	//		scalesize = 0.007;
	//	}
	//}
	//if(currenttree == 'e'||currenttree == 'a'){//a tree and e tree lose weight
	//	scalesize = 0.001;
	//}
	modelMat = Angel::identity();//load 1,1,1,1 unit matrix
	modelMat = modelMat* Angel::Translate(ppoint1) * rotate_matrix
		* Angel::Translate(0,0,-1*veczoom)*Angel::Scale(scalesize,scalesize,veczoom);
	modelMatrixf[0] = modelMat[0][0];modelMatrixf[4] = modelMat[0][1];
	modelMatrixf[1] = modelMat[1][0];modelMatrixf[5] = modelMat[1][1];
	modelMatrixf[2] = modelMat[2][0];modelMatrixf[6] = modelMat[2][1];
	modelMatrixf[3] = modelMat[3][0];modelMatrixf[7] = modelMat[3][1];
	modelMatrixf[8] = modelMat[0][2];modelMatrixf[12] = modelMat[0][3];
	modelMatrixf[9] = modelMat[1][2];modelMatrixf[13] = modelMat[1][3];
	modelMatrixf[10] = modelMat[2][2];modelMatrixf[14] = modelMat[2][3];
	modelMatrixf[11] = modelMat[3][2];modelMatrixf[15] = modelMat[3][3];
	glUniformMatrix4fv( modelMatrix, 1, GL_FALSE, modelMatrixf );
	global_matrix = modelMat;
	//global_matrix = Angel::identity() * Angel::Translate(ppoint1) * rotate_matrix * Angel::Translate(0,0,-1*veczoom);

	currentmodle = 43; //cylinder ,draw alone with z axis
	drawtarget = 2;
	drawply(); //the height of cylinder z from -1 to 1

	//scalesize = 0.002;
	//draw second sphere
	//modelMat = Angel::identity();//load 1,1,1,1 unit matrix
	//if(currenttree == 'e'||currenttree == 'a'){ //a tree and e tree lose weight
	//	scalesize = 0.001;
	//}
	modelMat = modelMat * Angel::Translate(ppoint2)* Angel::Scale(scalesize*5);//the order
	//scalesize = 0.002;

	modelMatrixf[0] = modelMat[0][0];modelMatrixf[4] = modelMat[0][1];
	modelMatrixf[1] = modelMat[1][0];modelMatrixf[5] = modelMat[1][1];
	modelMatrixf[2] = modelMat[2][0];modelMatrixf[6] = modelMat[2][1];
	modelMatrixf[3] = modelMat[3][0];modelMatrixf[7] = modelMat[3][1];
	modelMatrixf[8] = modelMat[0][2];modelMatrixf[12] = modelMat[0][3];
	modelMatrixf[9] = modelMat[1][2];modelMatrixf[13] = modelMat[1][3];
	modelMatrixf[10] = modelMat[2][2];modelMatrixf[14] = modelMat[2][3];
	modelMatrixf[11] = modelMat[3][2];modelMatrixf[15] = modelMat[3][3];
	glUniformMatrix4fv( modelMatrix, 1, GL_FALSE, modelMatrixf );
	global_matrix = modelMat;
	//global_matrix = Angel::identity() * Angel::Translate(ppoint2);

	currentmodle = 44;
	drawtarget = 1;
	drawply(); //the sphere diameter from -1 to 1

	//draw
	//glFlush();
	//recycle
	modelMat = Angel::identity();
	drawtarget = 0;
}  

void drawtree(char treeflag, float startx, float starty, float startz){
	GLuint refleOn;
	GLuint refraOn;
	if(shadermode == 2){
		refleOn = glGetUniformLocationARB(global_program, "refleOn");
		glUniform1i(refleOn, true );
	}
	else if(shadermode == 3){
		refraOn = glGetUniformLocationARB(global_program, "refraOn");
		glUniform1i(refraOn, true );
	}

	GLuint projcetMatrix = glGetUniformLocationARB(global_program, "projection_matrix");
	glUniformMatrix4fv( projcetMatrix, 1, GL_TRUE, perspectiveMat);
	GLuint lookMatrix = glGetUniformLocationARB(global_program, "look_matrix");
	glUniformMatrix4fv( lookMatrix, 1, GL_TRUE, lookMat);

	float suitsize;
	char *sstring;
	Lsysfile lsysf;

	if(treeflag == 'a'){
		lsysf = readlsysfile("lsys_files\\\\lsys1.txt");
		sstring = analyzelsys(lsysf);
		suitsize = 0.01f; //adjust the tree length
		currenttree = 'a';
	}
	else if(treeflag == 'b'){
		lsysf = readlsysfile("lsys_files\\\\lsys2.txt");
		sstring = analyzelsys(lsysf);
		suitsize = 0.04f;
		currenttree = 'b';
	}
	else if(treeflag == 'c'){
		lsysf = readlsysfile("lsys_files\\\\lsys3.txt");
		sstring = analyzelsys(lsysf);
		suitsize = 0.1f;
		currenttree = 'c';
	}
	else if(treeflag == 'd'){
		lsysf = readlsysfile("lsys_files\\\\lsys4.txt");
		sstring = analyzelsys(lsysf);
		suitsize = 0.1f;
		currenttree = 'd';
	}
	else if(treeflag == 'e'){
		lsysf = readlsysfile("lsys_files\\\\lsysmyown.txt");
		sstring = analyzelsys(lsysf);
		suitsize = 0.02f;
		currenttree = 'e';
	}

	Angel::mat4 modelMat2 = Angel::identity();//load 1,1,1,1 unit matrix
	//modelMat2 = modelMat2 * Angel::RotateY(0.0f)* Angel::RotateX(0.0f) * Angel::RotateZ(0.0f); //matrix for caculate the position

	//init
	int scount=0;
	
	point4 startpoint;
	startpoint.x = startx; 
	startpoint.y = starty; //for shadow sake, everything need to be above y=0
	startpoint.z = startz; //-3 to 3
	startpoint.w = 1.0f;
	
	vec4 orientation; //start vertex
	orientation.x = 0.0f;
	orientation.y = 1.0f;  //vector forward y axis
	orientation.z = 0.0f;
	orientation.w = 0.0f;

	//int sizesize = 10000;
	int sizesize = strlen(sstring);
	pointshw2 = (point4 *) malloc (sizeof(point4)*sizesize);
	colorshw2 = (color4 *) malloc (sizeof(color4)*sizesize);
	globalcolor = rand()%7;

	int index = 0;
	point4 currentpoint = startpoint;
	point4 temppp[2];
	cstate currentstate;
	while(sstring[scount]!=NULL){ //cause gl_Position = projection_matrix*model_matrix*vPosition
		if(sstring[scount]=='F'){ //Move forward a step of length len, drawing a line (or cylinder) to the new point.
			//draw cylinder tree
			temppp[0] = currentpoint;
			orientation = modelMat2 * orientation;
			float unito = Angel::length(orientation);
			//the point move 1 unit as vertex oritation
			temppp[1].x = currentpoint.x + orientation.x/unito*lsysf.len*suitsize; //tempority use 0.1
			temppp[1].y = currentpoint.y + orientation.y/unito*lsysf.len*suitsize; 
			temppp[1].z = currentpoint.z + orientation.z/unito*lsysf.len*suitsize;
			temppp[1].w = 1.0f;
			drawbet2point(temppp[0],temppp[1]);
			
			modelMat2 = Angel::identity();
			currentpoint = temppp[1];
			currentstate.corientation = orientation;
			currentstate.cpoints = currentpoint;
			currentstate.cmatrix = modelMat2;
		}
		else if(sstring[scount]=='+'){//Apply a positive rotatation about the X-axis of xrot degrees.
			modelMat2 = modelMat2 * Angel::RotateZ(lsysf.rot.x);
			currentstate.cmatrix = modelMat2;
		}
		else if(sstring[scount]=='-'){//Apply a negative rotatation about the X-axis of xrot degrees.
			modelMat2 = modelMat2 * Angel::RotateZ(-lsysf.rot.x);
			currentstate.cmatrix = modelMat2;
		}
		else if(sstring[scount]=='&'){//Apply a positive rotatation about the Y-axis of yrot degrees.
			modelMat2 = modelMat2 * Angel::RotateY(lsysf.rot.y);
			currentstate.cmatrix = modelMat2;
		}
		else if(sstring[scount]=='^'){//Apply a negative rotatation about the Y-axis of yrot degrees.
			modelMat2 = modelMat2 * Angel::RotateY(-lsysf.rot.y);
			currentstate.cmatrix = modelMat2;
		}
		else if(sstring[scount]=='\\'){//Apply a positive rotatation about the Z-axis of zrot degrees.
			modelMat2 = modelMat2 * Angel::RotateX(lsysf.rot.z);
			currentstate.cmatrix = modelMat2;
		}
		else if(sstring[scount]=='/'){//Apply a negative rotatation about the Z-axis of zrot degrees.
			modelMat2 = modelMat2 * Angel::RotateX(-lsysf.rot.z);
			currentstate.cmatrix = modelMat2;
		}
		else if(sstring[scount]=='|'){//Turn around 180 degrees.
			modelMat2 = modelMat2 * Angel::RotateX(180.0f);
			currentstate.cmatrix = modelMat2;
		}
		else if(sstring[scount]=='['){//Push the current state of the turtle onto a pushdown stack.
			PushState(currentstate); //store the current point, matrix and orientation as well
		}
		else if(sstring[scount]==']'){//Pop the state from the top of the turtle stack, and make it the current turtle stack.
			currentstate = PopState();
			currentpoint = currentstate.cpoints;
			orientation = currentstate.corientation;
			modelMat2 = currentstate.cmatrix;
		}
		scount++;
	}
	//glFlush(); // force output to graphics hardware
	
	//recycle
	free(pointshw2);
	free(colorshw2);
	free(normalhw2);
	globalcolor = 0;
	// use this call to double buffer
	//glutSwapBuffers(); 
	// you can implement your own buffers with textures

	refleOn = glGetUniformLocationARB(global_program, "refleOn");
	glUniform1i(refleOn, false );
	refraOn = glGetUniformLocationARB(global_program, "refraOn");
	glUniform1i(refraOn, false );
}

void PushState(cstate currentstate){
	if(globalmodelmatrix>=999){
		printf("ERROR! the state limited is 1000\n");
		exit(0);
	}
	else{
		statestack[globalstate] =  currentstate;
		globalstate++;
	}
}

cstate PopState(){ 
	cstate returnstate;
	if(globalstate<=0){
		printf("ERROR! can't pop cause there are no state in stack\n");
		exit(0);
	}
	else{
		returnstate = statestack[globalstate-1];
		globalstate--;
	}
	return returnstate;
}

Plyfile readplyfile(char *filename)
{
	//char *filename;
	char *name;
	//char line[256];
	FILE *fp = NULL;
	char filetype[10];
	char temp1[20],temp2[20],temp3[20],temp4[20],temp5[20];
	Plyfile plyf;
	int icount;

	//filename = "ply_files\\cow.ply";
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
	fscanf(fp, "%s %s %s %s %s\n",&temp1,&temp2,&temp3,&temp4,&temp5);//property list uint8 int32 vertex_indices
	fscanf(fp, "%s",&temp1);//end_header

	plyf.flyvertex = (point4 *)calloc(1,sizeof(point4)*plyf.vertexnum);
	for(icount=0;icount<plyf.vertexnum;icount++){
		fscanf(fp, "%f %f %f", &plyf.flyvertex[icount].x, &plyf.flyvertex[icount].y, &plyf.flyvertex[icount].z);
		//plyf.flyvertex[icount].w = 1.0; 
		//printf("%f,%f,%f\n",plyf.flyvertex[icount].x,plyf.flyvertex[icount].y,plyf.flyvertex[icount].z);
	}
	plyf.flyface = (face3 *)calloc(1,sizeof(face3)*plyf.facenum);
	for(icount=0;icount<plyf.facenum;icount++){
		fscanf(fp, "%d %d %d %d", &plyf.flyface[icount].face, &plyf.flyface[icount].point1, &plyf.flyface[icount].point2, &plyf.flyface[icount].point3);
		//printf("%d,%d,%d,%d\n", plyf.flyface[icount].face, plyf.flyface[icount].point1, plyf.flyface[icount].point2, plyf.flyface[icount].point3);
	}
	
	fclose(fp);
	return plyf;
}

int filldrawing() 
{
	char tempname[50]; //name length limit 50
	int i=0;
	int index = 0;
	//color4 colortarget;

	strcpy(tempname,plypath);
	strcat(tempname,allfiles[currentmodle]);
	//Plyfile plyread = readplyfile("ply_files\\\\cylinder.ply");
	Plyfile plyread = readplyfile(tempname);

	int inumber = plyread.facenum*3;
	pointshw2 = (point4 *) malloc (sizeof(point4)*inumber);
	colorshw2 = (color4 *) malloc (sizeof(color4)*inumber);
	normalhw2 = (point4 *) malloc (sizeof(point4)*inumber);

	while(i<plyread.facenum){
		vec4 normal;

		point4 a = plyread.flyvertex[plyread.flyface[i].point1];
		point4 b = plyread.flyvertex[plyread.flyface[i].point2];
		point4 c = plyread.flyvertex[plyread.flyface[i].point3];

		normal = normalize(cross(b - a,	c - b));

		pointshw2[index] = plyread.flyvertex[plyread.flyface[i].point1];
		pointshw2[index].w = 1.0;
		normalhw2[index] = normal;
		colorshw2[index] = vertex_colors[5];

		pointshw2[index+1] = plyread.flyvertex[plyread.flyface[i].point2];
		pointshw2[index+1].w = 1.0;
		normalhw2[index+1] = normal;
		colorshw2[index+1] = vertex_colors[5];
	
		pointshw2[index+2] = plyread.flyvertex[plyread.flyface[i].point3];
		pointshw2[index+2].w = 1.0;
		normalhw2[index+2] = normal;
		colorshw2[index+2] = vertex_colors[5];

		index+=3;
		i++;
	}
	return index;
	//free(tempname);
}

void SetShader(void){
	//GLuint groundOn = glGetUniformLocationARB(global_program, "groundOn");
	//glUniform1i(groundOn, ground_flag );
	//GLuint refleOn = glGetUniformLocationARB(global_program, "refleOn");
	//glUniform1i(refleOn, reflect_flag );
	GLuint refracOn = glGetUniformLocationARB(global_program, "refracOn");
	glUniform1i(refracOn, reflect_flag );

	/*if(shadermode ==0){
		global_program = InitShader( "vshader4expfog.glsl", "fshader4expfog.glsl" );
		glUseProgram( global_program );
	}
	else if(shadermode ==1){
		global_program = InitShader( "vshader4linefog.glsl", "fshader4linefog.glsl" );
		glUseProgram( global_program );
	}
	else if(shadermode ==2){
		global_program = InitShader( "vshader4reflect.glsl", "fshader4reflect.glsl" );
		glUseProgram( global_program );
	}
	else if(shadermode ==3){
		global_program = InitShader( "vshader4refract.glsl", "fshader4refract.glsl" );
		glUseProgram( global_program );
	}
	else if(shadermode ==9){
		global_program = InitShader( "vshader1.glsl", "fshader1.glsl" );
		glUseProgram( global_program );
	}
	else{
		global_program = InitShader( "vshader4expfog.glsl", "fshader4expfog.glsl" );
		glUseProgram( global_program );
	}*/
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



		/*global_program1 = InitShader( "vshader4expfog.glsl", "fshader4expfog.glsl" );
		global_program2 = InitShader( "vshader4linefog.glsl", "fshader4linefog.glsl" );
		global_program3 = InitShader( "vshader4reflect.glsl", "fshader4reflect.glsl" );
		global_program4 = InitShader( "vshader4refract.glsl", "fshader4refract.glsl" );
		global_program9 = InitShader( "vshader1.glsl", "fshader1.glsl" );*/
	global_program = InitShader( "vshader4all.glsl", "fshader4all.glsl" );
	glUseProgram( global_program );

	envmap();
	// sets the default color to clear screen
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
}

void drawply(void)
{
	int tempnumber;
	tempnumber = filldrawing();
	globaltempnumber = tempnumber;
	glUniformMatrix4fv( lookMatrix, 1, GL_TRUE, lookMat);

	//draw polygen
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );//用fill来画树比较像
	glBufferData( GL_ARRAY_BUFFER, sizeof(point4)*globaltempnumber + sizeof(color4)*globaltempnumber+sizeof(vec4)*globaltempnumber, NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4)*globaltempnumber, pointshw2 );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*globaltempnumber, sizeof(color4)*globaltempnumber, colorshw2 );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*globaltempnumber+sizeof(color4)*globaltempnumber, sizeof(vec4)*globaltempnumber, normalhw2 );
	GLuint vPosition = glGetAttribLocation( global_program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    GLuint vColor = glGetAttribLocation( global_program, "vColor" ); 
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point4)*globaltempnumber)); 
	GLuint vNormal = glGetAttribLocation( global_program, "vNormal" ); 
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point4)*globaltempnumber+sizeof(color4)*globaltempnumber));
	glEnable( GL_DEPTH_TEST );
	glDrawArrays( GL_TRIANGLES, 0, globaltempnumber );
	glDisable( GL_DEPTH_TEST );

	if(shadowmode == TRUE){
		GLuint drawshadow = glGetUniformLocationARB(global_program, "drawshadow");
		glUniform1i(drawshadow, true );

		//draw shadow
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE); //用line画影子比较像
		float light[3] = {5,5,5}; // location of light
		mat4 m = Angel::identity();
		m[3][1] = -1.0/light[1]; 
		m[3][3] = 0.0;
		mat4 tempview = lookMat;
		tempview = tempview*Angel::Translate(light[0], light[1],light[2])*m*Angel::Translate(-light[0], -light[1], -light[2]);
		lookMatrix = glGetUniformLocationARB(global_program, "look_matrix");
		glUniformMatrix4fv( lookMatrix, 1, GL_TRUE, tempview );
		//fill black
		for(int icount=0;icount<globaltempnumber;icount++){
			colorshw2[icount] = vertex_colors[0];
		}
		//fill buffer
		glBufferData( GL_ARRAY_BUFFER, sizeof(point4)*globaltempnumber + sizeof(color4)*globaltempnumber+sizeof(vec4)*globaltempnumber, NULL, GL_STATIC_DRAW );
		glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4)*globaltempnumber, pointshw2 );
		glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*globaltempnumber, sizeof(color4)*globaltempnumber, colorshw2 );
		glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*globaltempnumber+sizeof(color4)*globaltempnumber, sizeof(vec4)*globaltempnumber, normalhw2 );
		vPosition = glGetAttribLocation( global_program, "vPosition" );
		glEnableVertexAttribArray( vPosition );
		glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
		vColor = glGetAttribLocation( global_program, "vColor" ); 
		glEnableVertexAttribArray( vColor );
		glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point4)*globaltempnumber)); 
		vNormal = glGetAttribLocation( global_program, "vNormal" ); 
		glEnableVertexAttribArray( vNormal );
		glVertexAttribPointer( vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point4)*globaltempnumber+sizeof(color4)*globaltempnumber));
		//draw 
		glEnable( GL_DEPTH_TEST );
		glDrawArrays( GL_TRIANGLES, 0, globaltempnumber );
		glDisable( GL_DEPTH_TEST );

		glUniform1i(drawshadow, false );
	}
	//glFlush();
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	//recycle
	glUniformMatrix4fv( lookMatrix, 1, GL_TRUE, lookMat);
}

point4 translatepoint(float left, float right, float bottom, float top, float znear, float zfar, float x, float y, float z){
	int tranrate = 2;

	float xzoom = tranrate/(right - left);
	float yzoom = tranrate/(top - bottom);
	float zzoom = tranrate/(znear - zfar);
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
	int tranrate = 2;
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

	ratiox = tranrate/(xmax-xmin);
	ratioy = tranrate/(ymax-ymin);
	ratioz = tranrate/(zmax-zmin);
	xoffset = xmax - (xmax-xmin)/tranrate;
	yoffset = ymax - (ymax-ymin)/tranrate;
	zoffset = zmax - (zmax-zmin)/tranrate;

	if(ratiox<ratioy){
		for(int i=0;i<pointnumber;i++){
			ppoint[i].x = (ppoint[i].x-xoffset)*ratiox;
			if(currentmodle == 5){
				ppoint[i].y = (ppoint[i].y-yoffset)*ratiox+0.55;
			}
			if(currentmodle == 6){
				ppoint[i].y = (ppoint[i].y-yoffset)*ratiox+0.4;
			}
			if(currentmodle == 7){
				ppoint[i].y = (ppoint[i].y-yoffset)*ratiox+0.4;
			}
			if(currentmodle == 11){
				ppoint[i].y = (ppoint[i].y-yoffset)*ratiox+0.6;
			}
			ppoint[i].z = (ppoint[i].z)*ratiox;
			//ppoint[i].z = (ppoint[i].z-zoffset)*ratiox; 
		}
	}
	
	else{
		for(int i=0;i<pointnumber;i++){
			ppoint[i].x = (ppoint[i].x-xoffset)*ratioy;
			if(currentmodle == 5){
				ppoint[i].y = (ppoint[i].y-yoffset)*ratiox+0.55;
			}
			if(currentmodle == 6){
				ppoint[i].y = (ppoint[i].y-yoffset)*ratiox+0.4;
			}
			if(currentmodle == 7){
				ppoint[i].y = (ppoint[i].y-yoffset)*ratiox+0.4;
			}
			if(currentmodle == 11){
				ppoint[i].y = (ppoint[i].y-yoffset)*ratiox+0.6;
			}
			ppoint[i].z = (ppoint[i].z)*ratioy;
			//ppoint[i].z = (ppoint[i].z-zoffset)*ratioy;
		}
	}
	/*else if((ratioz<ratiox)&&(ratioz<ratioy)){
		for(int i=0;i<pointnumber;i++){
			ppoint[i].x = (ppoint[i].x-xoffset)*ratioz;
			ppoint[i].y = (ppoint[i].y-yoffset)*ratioz;
			ppoint[i].z = (ppoint[i].z-zoffset)*ratioz;
		}
	}*/
}

// keyboard handler
void keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
	case 'D':
		if(shadowmode == TRUE) shadowmode = FALSE;
		else shadowmode = TRUE;
		break;
	case 'A':
		if(strcmp(plain_texture,"grass")==0) plain_texture ="stone";
		else plain_texture ="grass";
		break;
	case 'F': //fog
		GLuint fogtype;
		fogtype = glGetUniformLocationARB(global_program, "fogtype");
		if(fogflag == true) fogflag = false;
		else fogflag = true;
		glUniform1i(fogtype, fogflag);
		break;
	case 'T'://reflect
		irandom = 1;
		irandom2 = 1;
		if(shadermode != 2) shadermode = 2;
		else shadermode = 1;
		break;
	case 'V'://refract
		irandom = 2;
		irandom2 = 2;
		if(shadermode != 3) shadermode = 3;
		else shadermode = 1;
		break;
	case 'K':
		shadermode = 4;
		break;
    case 033:
        exit( EXIT_SUCCESS );
        break;
	}
	glutPostRedisplay(); 
}

void initpro(){
	//bmpread_t bitmap[6];
	for(int icount=0;icount<6;icount++){
		char *bmpfile[6] ={"env\\\\nvnegx.bmp","env\\\\nvposx.bmp","env\\\\nvposy.bmp",
			"env\\\\nvnegy.bmp","env\\\\nvposz.bmp","env\\\\nvnegz.bmp"};
		//char *bmpfile[6] ={"grass.bmp","grass.bmp","grass.bmp","grass.bmp","grass.bmp","grass.bmp"};
		printf("Loading %s\n",bmpfile[icount]);
		if(!bmpread(bmpfile[icount], 0, &bitmap[icount]))
		{
			fprintf(stderr, "%s:error loading bitmap file\n", bitmap[icount]);
			exit(1);
		}
	}

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void displaybmp()
{
    //glClear( GL_COLOR_BUFFER_BIT );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable( GL_DEPTH_TEST );
    //glDrawArrays( GL_TRIANGLES, 0, 6 );
	glDrawArrays( GL_QUADS, 0, 12);
	glDisable( GL_DEPTH_TEST ); 
    glutSwapBuffers();
}

void displaymap()
{
	//color4 red = vec4(1.0, 0.0, 0.0, 1.0);
	//GLuint color_loc = glGetUniformLocation(program, "fcolor");
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glUniform4fv(color_loc, 1, red);
	//transpoint2ordinate(pointshw2,10000);
	glEnable( GL_DEPTH_TEST );
	//glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   // glClear( GL_COLOR_BUFFER_BIT );
	glDrawArrays( GL_TRIANGLES, 0, 20000);
	glDisable( GL_DEPTH_TEST ); 
    glutSwapBuffers();
	
}

void reshape( int width, int height )
{
    glViewport( 0, 0, width, height );
    //mat4 projection = Ortho( 0.0, 1.0, 0.0, 1.0, -1.0, 1.0 );
	//mat4 projection = Ortho( -2.0, 2.0, -2.0, 2.0, -2.0, 2.0 );
	perspectiveMat = Angel::Perspective((GLfloat)65, (GLfloat)width/(GLfloat)height, (GLfloat)0.1, (GLfloat) 100.0); 
	//GLuint viewMatrix = glGetUniformLocationARB(program, "projection_matrix");
	glUniformMatrix4fv( projcetMatrix, 1, GL_TRUE, perspectiveMat);
	//Angel::mat4 projection = Angel::Perspective((GLfloat)125, (GLfloat)width/(GLfloat)height, (GLfloat)0.01, (GLfloat) 100.0);
    //glUniformMatrix4fv( viewMatrix, 1, GL_TRUE, perspectiveMat ); //true or false
}

//为什么modelmat在shader里面乘没效果,难道显卡不支持
point4 pointplain[4]={
	point4(-5.0,0.0,0.0,1.0),
	point4(5.0,0.0,0.0,1.0),
	point4(5.0,0.0,-10.0,1.0),
	point4(-5.0,0.0,-10.0,1.0),
};
//贴图坐标0-1
point4 pointbmp[4]={
	point4(0.0,0.0,0.0,1.0),
	point4(0.0,1.0,0.0,1.0),
	point4(1.0,1.0,0.0,1.0),
	point4(1.0,0.0,0.0,1.0),
};

point4 pointplain2[12]={
	point4(-10.0,-10.0,0.0,1.0),
	point4(-10.0,10.0,0.0,1.0),
	point4(10.0,10.0,0.0,1.0),
	point4(10.0,-10.0,0.0,1.0),

	point4(-10.0,-10.0,-10.0,1.0),
	point4(-10.0,-10.0,0.0,1.0),
	point4(-10.0,10.0,0.0,1.0),
	point4(-10.0,10.0,-10.0,1.0),

	point4(10.0,-10.0,-10.0,1.0),
	point4(10.0,-10.0,0.0,1.0),
	point4(10.0,10.0,0.0,1.0),
	point4(10.0,10.0,-10.0,1.0),
};

point4 pointbmp2[12]={
	point4(0.0,0.0,0.0,1.0),
	point4(0.0,1.0,0.0,1.0),
	point4(1.0,1.0,0.0,1.0),
	point4(1.0,0.0,0.0,1.0),

	point4(0.0,0.0,0.0,1.0),
	point4(0.0,1.0,0.0,1.0),
	point4(1.0,1.0,0.0,1.0),
	point4(1.0,0.0,0.0,1.0),

	point4(0.0,0.0,0.0,1.0),
	point4(0.0,1.0,0.0,1.0),
	point4(1.0,1.0,0.0,1.0),
	point4(1.0,0.0,0.0,1.0),
};

void DrawPlain2( void ){
	GLuint groundOn = glGetUniformLocationARB(global_program, "groundOn");
    glUniform1i(groundOn, true );

	bmpread_t bitmap;
	GLuint  textures[2];

	//load shader
    //program = InitShader( "vshader4bmp.glsl", "fshader4bmp.glsl" );
    //glUseProgram( program );
	//set local matrix
	GLuint viewMatrix = glGetUniformLocationARB(global_program, "projection_matrix");
	glUniformMatrix4fv( viewMatrix, 1, GL_TRUE, perspectiveMat);
	GLuint lookMatrix = glGetUniformLocationARB(global_program, "look_matrix");
	glUniformMatrix4fv( lookMatrix, 1, GL_TRUE, lookMat);
	Angel::mat4 modelMat = Angel::identity()* Angel::RotateY(0);//pay attention to order
	GLuint modelMatrix = glGetUniformLocationARB(global_program, "model_matrix");
	glUniformMatrix4fv( modelMatrix, 1, GL_TRUE, modelMat );

	//read bmp into shader
	glGenTextures( 2, textures );
	printf("Loading grass.bmp\n");
	if(!bmpread("grass.bmp", 0, &bitmap)){
		fprintf(stderr, "%s:error loading bitmap file\n", "grass.bmp");
		exit(1);
	}
    glBindTexture( GL_TEXTURE_2D, textures[0] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, bitmap.width, bitmap.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmap.rgb_data );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	bmpread_free(&bitmap);
	printf("Loading stones.bmp\n");
	if(!bmpread("stones.bmp", 0, &bitmap)){
		fprintf(stderr, "%s:error loading bitmap file\n", "stones.bmp");
		exit(1);
	}
    glBindTexture( GL_TEXTURE_2D, textures[1] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, bitmap.width, bitmap.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmap.rgb_data );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	bmpread_free(&bitmap);
	
	//set position
	point4 plain_points[4]={
		point4(-10.0,0.10,0.0,1.0),
		point4(10.0,0.0,10.0,1.0),
		point4(10.0,0.0,-10.0,1.0),
		point4(-10.0,0.0,-10.0,1.0),
	};
	point4 bmp_position[4]={
		point4(0.0,0.0,0.0,1.0),
		point4(0.0,50.0,0.0,1.0),
		point4(50.0,50.0,0.0,1.0),
		point4(50.0,0.0,0.0,1.0),
	};

	//set buffer
	glBufferData( GL_ARRAY_BUFFER, sizeof(plain_points)+sizeof(bmp_position), NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(plain_points), plain_points );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(plain_points), sizeof(bmp_position), bmp_position );
	GLuint vPosition = glGetAttribLocation( global_program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    GLuint vColor = glGetAttribLocation( global_program, "vColor" ); 
    glEnableVertexAttribArray( vColor );
    GLuint vTexCoord = glGetAttribLocation( global_program, "vTexCoord" ); 
    glEnableVertexAttribArray( vTexCoord );
    glVertexAttribPointer( vTexCoord, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(plain_points)) );
    // Set our texture samples to the active texture unit
    glUniform1i( glGetUniformLocation(global_program, "texture"), 0 );

	//change texture
	glActiveTexture( GL_TEXTURE0 );
	//glBindTexture(GL_TEXTURE_2D, textures[0]); //测试用
	if(strcmp(plain_texture,"grass") == 0){
		glBindTexture(GL_TEXTURE_2D, textures[0]);
	}
	else if(strcmp(plain_texture,"stone") == 0){
		glBindTexture(GL_TEXTURE_2D, textures[1]);
	}

	glEnable( GL_DEPTH_TEST );
	glDrawArrays( GL_QUADS, 0, 4 );
	glDisable( GL_DEPTH_TEST ); 
    //glutSwapBuffers();

	//glUseProgram(0);
	groundOn = glGetUniformLocationARB(global_program, "groundOn");
	glUniform1i(groundOn, false );
}

void DrawPlain( void ){
	bmpread_t bitmap;
	GLuint  textures[2];

	//load shader
    program = InitShader( "vshader4bmp.glsl", "fshader4bmp.glsl" );
    glUseProgram( program );
	//set local matrix
	GLuint viewMatrix = glGetUniformLocationARB(program, "projection_matrix");
	glUniformMatrix4fv( viewMatrix, 1, GL_TRUE, perspectiveMat);
	GLuint lookMatrix = glGetUniformLocationARB(program, "look_matrix");
	glUniformMatrix4fv( lookMatrix, 1, GL_TRUE, lookMat);
	Angel::mat4 modelMat = Angel::identity()* Angel::RotateX(0);//pay attention to order
	GLuint modelMatrix = glGetUniformLocationARB(program, "model_matrix");
	glUniformMatrix4fv( modelMatrix, 1, GL_TRUE, modelMat );

	//read bmp into shader
	glGenTextures( 2, textures );
	printf("Loading grass.bmp\n");
	if(!bmpread("grass.bmp", 0, &bitmap)){
		fprintf(stderr, "%s:error loading bitmap file\n", "grass.bmp");
		exit(1);
	}
    glBindTexture( GL_TEXTURE_2D, textures[0] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, bitmap.width, bitmap.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmap.rgb_data );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	bmpread_free(&bitmap);
	printf("Loading stones.bmp\n");
	if(!bmpread("stones.bmp", 0, &bitmap)){
		fprintf(stderr, "%s:error loading bitmap file\n", "stones.bmp");
		exit(1);
	}
    glBindTexture( GL_TEXTURE_2D, textures[1] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, bitmap.width, bitmap.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmap.rgb_data );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	bmpread_free(&bitmap);
	
	//set position
	point4 plain_points[4]={
		point4(-10.0,0.10,0.0,1.0),
		point4(10.0,0.0,10.0,1.0),
		point4(10.0,0.0,-10.0,1.0),
		point4(-10.0,0.0,-10.0,1.0),
	};
	point4 bmp_position[4]={
		point4(0.0,0.0,0.0,1.0),
		point4(0.0,50.0,0.0,1.0),
		point4(50.0,50.0,0.0,1.0),
		point4(50.0,0.0,0.0,1.0),
	};

	//set buffer
	glBufferData( GL_ARRAY_BUFFER, sizeof(plain_points)+sizeof(bmp_position), NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(plain_points), plain_points );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(plain_points), sizeof(bmp_position), bmp_position );
	GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    GLuint vColor = glGetAttribLocation( program, "vColor" ); 
    glEnableVertexAttribArray( vColor );
    GLuint vTexCoord = glGetAttribLocation( program, "vTexCoord" ); 
    glEnableVertexAttribArray( vTexCoord );
    glVertexAttribPointer( vTexCoord, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(plain_points)) );
    // Set our texture samples to the active texture unit
    glUniform1i( glGetUniformLocation(program, "texture"), 0 );

	//change texture
	glActiveTexture( GL_TEXTURE0 );
	if(strcmp(plain_texture,"grass") == 0){
		glBindTexture(GL_TEXTURE_2D, textures[0]);
	}
	else if(strcmp(plain_texture,"stone") == 0){
		glBindTexture(GL_TEXTURE_2D, textures[1]);
	}

	glEnable( GL_DEPTH_TEST );
	glDrawArrays( GL_QUADS, 0, 4 );
	glDisable( GL_DEPTH_TEST ); 
    //glutSwapBuffers();

	//glUseProgram(0);
}

point4 vertices[10] = {
	point4( -0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5, -0.5, -0.5, 1.0 ),
    point4( -0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5, -0.5, -0.5, 1.0 ),
	//point4(  0.5,  -0.3, -0.5, 1.0 ),
    //point4(  -0.5, -0.3, -0.5, 1.0 )
};

void colorcube()  
{
	quad( 2, 6, 7, 3 );//posx
    quad( 5, 1, 0, 4 );//ngax
    quad( 2, 6, 5, 1 );//posy
    quad( 3, 0, 4, 7 );//ngay
    quad( 0, 1, 2, 3 );//poxz
    quad( 4, 5, 6, 7 );//ngaz

	//quad( 0, 3, 8, 9 );

	/*quad( 1, 0, 3, 2 );//posx
    quad( 2, 3, 7, 6 );//ngax
    quad( 3, 0, 4, 7 );//posy
    quad( 6, 5, 1, 2 );//ngay
    quad( 4, 5, 6, 7 );//poxz
    quad( 5, 4, 0, 1 );//ngaz*/
}

int cubefill() 
{
	int i;
	int index = 0;
	i=0;

	//strcpy(tempname,plypath);
	//strcat(tempname,allfiles[currentmodle]);
	Plyfile plyread = readplyfile("ply_files\\\\cylinder.ply");
	int inumber = plyread.facenum*3;
	pointshw2 = (point4 *) malloc (sizeof(point4)*inumber);
	normalhw2 = (color4 *) malloc (sizeof(vec4)*inumber);

	while(i<plyread.facenum){
		vec4 normal;

		point4 a = plyread.flyvertex[plyread.flyface[i].point1];
		point4 b = plyread.flyvertex[plyread.flyface[i].point2];
		point4 c = plyread.flyvertex[plyread.flyface[i].point3];

		normal = normalize(cross(b - a,	c - b));

		pointshw2[index] = plyread.flyvertex[plyread.flyface[i].point1];
		pointshw2[index].w = 1.0;
		normalhw2[index] = normal;

		pointshw2[index+1] = plyread.flyvertex[plyread.flyface[i].point2];
		pointshw2[index+1].w = 1.0;
		normalhw2[index+1] = normal;
	
		pointshw2[index+2] = plyread.flyvertex[plyread.flyface[i].point3];
		pointshw2[index+2].w = 1.0;
		normalhw2[index+2] = normal;

		index+=3;
		i++;
	}
	return index;
	//free(tempname);
}

void quad(int a, int b, int c, int d)
{
	vec4 normal;

	static int ii =0;
	normal = normalize(cross(vertices[b] - vertices[a],	vertices[c] - vertices[b]));
	normals[ii] = normal;
	points[ii] = vertices[a];
	//texcube[ii] = vertices[a];
	ii++;
	normals[ii] = normal;
	points[ii] = vertices[b];
	//texcube[ii] = vertices[b];
	ii++;
	normals[ii] = normal;
	points[ii] = vertices[c];
	//texcube[ii] = vertices[c];
	ii++;
	normals[ii] = normal;
	points[ii] = vertices[a];
	//texcube[ii] = vertices[a];
	ii++;
	normals[ii] = normal;
	points[ii] = vertices[c];
	//texcube[ii] = vertices[c];
	ii++;
	normals[ii] = normal;
	points[ii] = vertices[d];
	//texcube[ii] = vertices[d];
	ii++;
}

void envmap( void ){
	//global_program = InitShader( "vshader4reflect.glsl", "fshader4reflect.glsl" );
    glUseProgram( global_program );

	bmpread_t bitmap[6];
	for(int icount=0;icount<6;icount++){
		char *bmpfile[6] ={"env\\\\nvnegx.bmp","env\\\\nvposx.bmp","env\\\\nvposy.bmp",
			"env\\\\nvnegy.bmp","env\\\\nvposz.bmp","env\\\\nvnegz.bmp"};
		//char *bmpfile[6] ={"grass.bmp","grass.bmp","grass.bmp","grass.bmp","grass.bmp","grass.bmp"};
		printf("Loading %s\n",bmpfile[icount]);
		if(!bmpread(bmpfile[icount], 0, &bitmap[icount]))
		{
			fprintf(stderr, "%s:error loading bitmap file\n", bitmap[icount]);
			exit(1);
		}
	}
	
	//glCullFace(GL_BACK);                        //背面裁剪(背面不可见)
    //glEnable(GL_CULL_FACE);                        //启用裁剪
	//glEnable(GL_TEXTURE_CUBE_MAP); //启用立方体贴图
	//glDisable(GL_TEXTURE_CUBE_MAP); // 禁用立方体贴图

	GLuint  tex[6];
	glGenTextures( 6, tex ); // 分配6个纹理对象的编号

	//如果不使用glBindTexture函数，那么glTexImage*、glTexParameter*、glTexCoord*系列函数默认在一个编号为0的纹理对象上进行操作
	//函数有两个参数，第一个参数是需要使用纹理的目标,第二个参数是所使用的纹理的编号
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex[0]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);// 啥用
	//active
	glActiveTexture(GL_TEXTURE3); //为什么要这行
   
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X ,0,GL_RGB,bitmap[0].width,bitmap[0].height,0,GL_RGB,GL_UNSIGNED_BYTE, bitmap[1].rgb_data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X ,0,GL_RGB,bitmap[1].width,bitmap[1].height,0,GL_RGB,GL_UNSIGNED_BYTE, bitmap[0].rgb_data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y ,0,GL_RGB,bitmap[2].width,bitmap[2].height,0,GL_RGB,GL_UNSIGNED_BYTE, bitmap[2].rgb_data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y ,0,GL_RGB,bitmap[3].width,bitmap[3].height,0,GL_RGB,GL_UNSIGNED_BYTE, bitmap[3].rgb_data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z ,0,GL_RGB,bitmap[4].width,bitmap[4].height,0,GL_RGB,GL_UNSIGNED_BYTE, bitmap[5].rgb_data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z ,0,GL_RGB,bitmap[5].width,bitmap[5].height,0,GL_RGB,GL_UNSIGNED_BYTE, bitmap[4].rgb_data);
	// 设定缩放器的过滤参数
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,GL_NEAREST); //GL_LINEAR有计算，效果好
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//GL_TEXTURE_MAG_FILTER指当纹理图象被使用到一个大于它的形状上时，GL_TEXTURE_MIN_FILTER指当纹理图象被使用到一个小于（或等于）它的形状上时
	//glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    
	bmpread_free(bitmap);

	GLuint texMapLocation = glGetUniformLocation(global_program, "texMap");
	glUniform1i(texMapLocation, tex[0]);

	//glDisable(GL_TEXTURE_CUBE_MAP); // 禁用立方体贴图
}

void idlehandle() 
{
	glutPostRedisplay(); 
}
//----------------------------------------------------------------------------
// entry point
int main( int argc, char **argv )
{
	// init glut
    glutInit( &argc, argv );
    //glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitDisplayMode( GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH ); //we don't use double buffer in hw3
	//glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
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
	//glEnable(GL_BLEND);
	//bmpbmp();
	//envmap();
	//point4 point1 = point4 (0, 0.5f, 0,1.0f);
	//point4 point2 = point4 (0, 1.0f, 0,1.0f);
	//RenderBone(point1, point2)  ;
	//glutDisplayFunc( RenderBone );
    glutDisplayFunc( displayhw3 );
	//glutDisplayFunc( displaymap );
	//glutReshapeFunc( reshape );
	//glutIdleFunc(idlehandle);
	//glutMouseFunc(mouse);
	//glutMotionFunc(processMouseActiveMotion);
    glutKeyboardFunc( keyboard );
	// should add menus
	// add mouse handler
	// add resize window functionality (should probably try to preserve aspect ratio)

	// enter the drawing loop
	// frame rate can be controlled with 
    glutMainLoop();
    return 0;
}
