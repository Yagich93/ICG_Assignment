#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#define WindowTitle "Assignment Lushin Iakov"

typedef struct Vertex
{
	float Position[4];
	float Color[4];
	float Normal[4];
} Vertex;

static const double PI = 3.14159265358979323846;

int WindowWidth = 500, WindowHeight = 500;

/* vertex arrays and buffers */
GLuint VBOIds[3];
GLuint VAOIds[3];
GLuint ProgramID;

/* uniform variables */
GLuint ModelViewMatrixLocation;
GLfloat ModelViewMatrix[16];
GLuint ProjectionMatrixLocation;
GLfloat ProjectionMatrix[16];

/* uniform variables for lighting */
GLint lights[4]={0,1,1,0}; /* ambient, x, z, up; now all off */
GLuint lightsLoc;
GLfloat C_L0_Ambient[4];
GLuint C_L0_AmbientLoc;
GLfloat L1_Direction[4], C_L1_Ambient[4], C_L1_Diffuse[4];
GLuint L1_DirectionLoc, C_L1_AmbientLoc, C_L1_DiffuseLoc;
GLfloat L2_Position[4], C_L2_Ambient[4], C_L2_Diffuse[4], C_L2_Specular[4];
GLuint L2_PositionLoc, C_L2_AmbientLoc, C_L2_DiffuseLoc, C_L2_SpecularLoc;
GLfloat L2_Shininess;
GLuint L2_ShininessLoc;

/* function prototypes */
void Initialize(int, char*[]);
void InitWindow(int, char*[]);
void ResizeFunction(int, int);
void KeyboardFunction(unsigned char, int, int);
void RenderFunction(void);
void MouseFunction(int, int, int, int);
void Cleanup(void);
void CreateVBO(void);
void DestroyVBO(void);
GLuint CreateShaders(void);
void DestroyShaders(void);
void Add_Lights();

void Updatetransformation(int);
void IdleFunction(void);

/* function prototypes for matrix-transformation-library */
void CrossProd(float *, float [],float []);
void Normalize(float *);
void IdentMat4(float *);
void MultMat4(float *, float [], float []);
void MultMatV(float *, float [], float []);
void Translate(float *, float, float, float);
void Scale(float *, float, float, float);
void RotateX(float *, float);
void RotateY(float *, float);
void RotateZ(float *, float);
void LookAt(GLfloat *, float, float, float, float, float, float, float, float, float);
void Ortho(GLfloat *, float, float, float, float, float, float);
void Frustum(GLfloat *, float, float, float, float, float, float);
void Prod(GLfloat *, float[], float[]);

GLfloat cax=2.0, cay= 9.0, caz= 12.0;
GLfloat lax=5.0, lay= 0.0, laz=-2.0;
GLfloat upx=0.0, upy= 1.0, upz=0.0;
GLfloat l=-7.0, r=5.0, b=-6.0, t=6.0, zn=11.0, zf=23.0;

int proj = 1;

/* house/cube */
Vertex Cube_Ver[] = {
0.0, 0.0, 0.0, 1.0, 0.11, 0.57, 0.78, 1.0, 0.0, 0.0, 0.0, 1.0,
2.0, 0.0, 0.0, 1.0, 0.11, 0.57, 0.78, 1.0, 0.0, 0.0, 0.0, 1.0,
2.0, 2.0, 0.0, 1.0, 0.11, 0.57, 0.78, 1.0, 0.0, 0.0, 0.0, 1.0,
0.0, 2.0, 0.0, 1.0, 0.11, 0.57, 0.78, 1.0, 0.0, 0.0, 0.0, 1.0,
0.0, 0.0, -2.0, 1.0, 0.11, 0.57, 0.78, 1.0, 0.0, 0.0, 0.0, 1.0,
2.0, 0.0, -2.0, 1.0, 0.11, 0.57, 0.78, 1.0, 0.0, 0.0, 0.0, 1.0,
2.0, 2.0, -2.0, 1.0, 0.11, 0.57, 0.78, 1.0, 0.0, 0.0, 0.0, 1.0,
0.0, 2.0, -2.0, 1.0, 0.11, 0.57, 0.78, 1.0, 0.0, 0.0, 0.0, 1.0};
GLint Cube_Ind[] = {
0,1,2, 0,2,3, 1,5,6, 1,6,2, 5,4,7, 5,7,6, 4,0,3, 4,3,7,
3,2,6, 3,6,7, 0,4,5, 0,5,1};

/* house from cube, now roof */
Vertex Roof_Ver[] = {
-0.5, 1.5,  0.5, 1.0, 0.78, 0.57, 0.11, 1.0, 0.0, 0.0, 0.0, 1.0,
 1.0, 3.0,  0.5, 1.0, 0.78, 0.57, 0.11, 1.0, 0.0, 0.0, 0.0, 1.0,
 1.0, 3.0, -2.5, 1.0, 0.78, 0.57, 0.11, 1.0, 0.0, 0.0, 0.0, 1.0,
-0.5, 1.5, -2.5, 1.0, 0.78, 0.57, 0.11, 1.0, 0.0, 0.0, 0.0, 1.0,
 2.5, 1.5,  0.5, 1.0, 0.78, 0.57, 0.11, 1.0, 0.0, 0.0, 0.0, 1.0,
 2.5, 1.5, -2.5, 1.0, 0.78, 0.57, 0.11, 1.0, 0.0, 0.0, 0.0, 1.0};
GLint Roof_Ind[] = {0,1,2, 0,2,3, 4,5,2, 4,2,1};

int Num_of_Houses;

int main(int argc, char* argv[])
{
	Initialize(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

void Initialize(int argc, char* argv[])
{
	GLenum GlewInitResult;

	InitWindow(argc, argv);
	glewExperimental = GL_TRUE;
	GlewInitResult = glewInit();

	ProgramID = CreateShaders();
	printf("ProgramID %d\n", ProgramID);
	ModelViewMatrixLocation = glGetUniformLocation(ProgramID, "ModelViewMatrix");
	ProjectionMatrixLocation = glGetUniformLocation(ProgramID, "ProjectionMatrix");
	lightsLoc = glGetUniformLocation(ProgramID, "lights");
	C_L0_AmbientLoc = glGetUniformLocation(ProgramID, "C_L0_Amb");
	L1_DirectionLoc = glGetUniformLocation(ProgramID, "L1_Dir");
	C_L1_AmbientLoc = glGetUniformLocation(ProgramID, "C_L1_Amb");
	C_L1_DiffuseLoc = glGetUniformLocation(ProgramID, "C_L1_Dif");

	L2_PositionLoc = glGetUniformLocation(ProgramID, "L2_Pos");
	C_L2_AmbientLoc = glGetUniformLocation(ProgramID, "C_L2_Amb");
	C_L2_DiffuseLoc = glGetUniformLocation(ProgramID, "C_L2_Dif");
	C_L2_SpecularLoc = glGetUniformLocation(ProgramID, "C_L2_Spe");
	L2_ShininessLoc = glGetUniformLocation(ProgramID, "L2_Shin");

        LookAt(ModelViewMatrix, cax,cay,caz, lax,lay,laz, upx,upy,upz);
	//Ortho(ProjectionMatrix, l,r, b,t, zn, zf);
	Frustum(ProjectionMatrix, l,r, b,t, zn, zf);

	CreateVBO();
	Add_Lights();

	glClearColor(0.2f, 0.2f, 0.2f, 0.2f);

        printf(" cx  cy  cz  lx  ly  lz  ux  uy  uz   l  r  b  t  zn  zf \n");

}

void InitWindow(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS );
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGBA);

	glutCreateWindow(WindowTitle);

	glutReshapeFunc(ResizeFunction);
        glutKeyboardFunc(KeyboardFunction);
	glutDisplayFunc(RenderFunction);
	
	glutIdleFunc(IdleFunction);
	glutTimerFunc(0, Updatetransformation, 0); /* ms, call, value */

	glutCloseFunc(Cleanup);
}

void ResizeFunction(int Width, int Height)
{
	WindowWidth = Width;
	WindowHeight = Height;
	glViewport(0, 0, WindowWidth, WindowHeight);
}

void KeyboardFunction(unsigned char key, int x, int y)
{
        switch (key)
        {
        case 27:
	case 'q': Cleanup(); exit(0); break;
	case 'x': cax -= 0.5; break;
	case 'X': cax += 0.5; break;
	case 'y': cay -= 0.5; break;
	case 'Y': cay += 0.5; break;
	case 'z': caz -= 0.5; break;
	case 'Z': caz += 0.5; break;
	case 'a': lax -= 0.5; break;
	case 'A': lax += 0.5; break;
	case 's': lay -= 0.5; break;
	case 'S': lay += 0.5; break;
	case 'd': laz -= 0.5; break;
	case 'D': laz += 0.5; break;
	case 'i': l+=0.5; r-=0.5; b+=0.5; t-=0.5; break;
	case 'o': l-=0.5; r+=0.5; b-=0.5; t+=0.5; break;
	case 'n': zn -= 0.5; break;
	case 'N': zn += 0.5; break;
	case 'f': zf -= 0.5; break;
	case 'F': zf += 0.5; break;
	case 'p': proj = (proj==0)? 1: 0; break;
	case '0': lights[0] = (lights[0]==0)? 1: 0; break;
	case '1': lights[1] = (lights[1]==0)? 1: 0; break;
	case '2': lights[2] = (lights[2]==0)? 1: 0; break;
	case '3': lights[3] = (lights[3]==0)? 1: 0; break;
        }
	LookAt(ModelViewMatrix, cax,cay,caz, lax,lay,laz, upx,upy,upz);
	if (proj==1) Frustum(ProjectionMatrix, l,r, b,t, zn, zf);
	if (proj==0) Ortho(ProjectionMatrix, l,r, b,t, zn, zf);
	printf("\r%3.0f %3.0f %3.0f %3.0f %3.0f %3.0f %3.0f %3.0f %3.0f %3.0f %3.0f %3.0f %3.0f %3.0f %3.0f %d %d %d %d",
	cax,cay,caz,lax,lay,laz,upx,upy,upz, l,r,b,t,zn,zf, lights[0], lights[1], lights[2], lights[3]);
	fflush(stdout);
	glutPostRedisplay();
}

void RenderFunction(void)
{

	int i,j;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glUniformMatrix4fv(ModelViewMatrixLocation, 1, 0, ModelViewMatrix);
	glUniformMatrix4fv(ProjectionMatrixLocation, 1, 0, ProjectionMatrix);
	glUniform4iv(lightsLoc, 1, lights);
	glUniform4fv(C_L0_AmbientLoc, 1, C_L0_Ambient);

	glUniform4fv(L1_DirectionLoc, 1, L1_Direction);
	glUniform4fv(C_L1_AmbientLoc, 1, C_L1_Ambient);
	glUniform4fv(C_L1_DiffuseLoc, 1, C_L1_Diffuse);

	glUniform4fv(L2_PositionLoc, 1, L2_Position);
	glUniform4fv(C_L2_AmbientLoc, 1, C_L2_Ambient);
	glUniform4fv(C_L2_DiffuseLoc, 1, C_L2_Diffuse);
	glUniform4fv(C_L2_SpecularLoc, 1, C_L2_Specular);
	glUniform1f(L2_ShininessLoc, L2_Shininess);

	glBindVertexArray(VAOIds[0]);
	glDrawArrays(GL_TRIANGLES, 0, 3*48);

        glBindVertexArray(VAOIds[1]);
        glDrawArrays(GL_LINES, 0, 6);
	
	glBindVertexArray(VAOIds[2]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	glFlush();

	glBindVertexArray(0);
}

void IdleFunction(void)
{
    glutPostRedisplay();
}

void Updatetransformation(int Value)
{
	float l1_dif[] = {1, 0, 0, 1,
			  1, 1, 0, 1,
			  0, 1, 0, 1,
			  0, 1, 1, 1,
			  0, 0, 1, 1,
			  1, 0, 1, 1};
	float r1_dif[] = {1, 1, 1, 1};
	static int r = 1;
	static int clr_sh = 0;

	GLfloat wait = 1000;
	if (Value==1) {
		/*r = (r+1)%2;
		l1_dif[0] = r;
		Prod(C_L1_Diffuse, l1_dif, r1_dif);*/
		Prod(C_L2_Diffuse, l1_dif+4*clr_sh, r1_dif);
		Prod(C_L2_Specular, l1_dif+4*clr_sh, r1_dif);
		clr_sh += 1;
		clr_sh %= sizeof(l1_dif)/sizeof(float)/4;
	}
	glutTimerFunc(wait, Updatetransformation, 1);
}

void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
}

void CreateVBO(void)
{

	Vertex Cube_Tri[36]; /* num_of_triangles*numf_of_vertics in each */
			     /* 12 * 3 = 36 */

	Vertex House_Tri[48]; /* cube triangles + 4 triangles for the roof */
			      /* 12 * 3 + 4 * 3 = 48 */

	Vertex All_Houses[3*48];

	Vertex Axes[] = {
	 0.0,  0.0,  0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0,
	 4.0,  0.0,  0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0,

	 0.0,  0.0,  0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0,
	 0.0,  4.0,  0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0,

	 0.0,  0.0,  0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0,
	 0.0,  0.0,  2.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0,
	};

	Vertex Snow[] = {
	 -2.0,  0.0,  -8.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0,
	 -2.0,  0.0,   2.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0,
	  8.0,  0.0,   2.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0,
	 
	  8.0,  0.0,   2.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0,
	  8.0,  0.0,  -8.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0,
	 -2.0,  0.0,  -8.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0,};

	int i,j,k, ind;

	float m1[16], m2[16], M[16];
	float a1[4], a2[4];

	float v1[3], v2[3], v3[3], w1[3], w2[3], n1[3], n2[3], n3[3];

	/* Cube triangles */
	ind = 0;
	for (i=0; i<12; i++) { /* 12:  number of triangles on a cube surface*/
		for (j=0; j<3; j++) { /* three vertices in each triangle */
			Cube_Tri[ind] = Cube_Ver[Cube_Ind[i*3+j]];
			House_Tri[ind] = Cube_Ver[Cube_Ind[i*3+j]];
			ind = ind + 1;
		}
	}

	/* ... more House triangles (roof) */

	ind = 36;
	for (i=0; i<4; i++) {
		for (j=0; j<3; j++) {
			House_Tri[ind] = Roof_Ver[Roof_Ind[i*3+j]];
			ind = ind + 1;
		}
	}

	/* ... normals for house vertices */
	for (i=0; i<16; i++) {
		ind = 3*i;
		memcpy(v1, House_Tri[ind].Position, 3*sizeof(float));
		memcpy(v2, House_Tri[ind+1].Position, 3*sizeof(float));
		memcpy(v3, House_Tri[ind+2].Position, 3*sizeof(float));
		for (j=0;j<3;j++) w1[j]=v2[j]-v1[j];
		for (j=0;j<3;j++) w2[j]=v3[j]-v1[j];
		CrossProd(n1, w1, w2); Normalize(n1);
		for (j=0;j<3;j++) w1[j]=v3[j]-v2[j];
		for (j=0;j<3;j++) w2[j]=v1[j]-v2[j];
		CrossProd(n2, w1, w2); Normalize(n2);
		for (j=0;j<3;j++) w1[j]=v1[j]-v3[j];
                for (j=0;j<3;j++) w2[j]=v2[j]-v3[j];
		CrossProd(n3, w1, w2); Normalize(n3);
		memcpy(House_Tri[ind].Normal, n1, 3*sizeof(float));
		memcpy(House_Tri[ind+1].Normal, n2, 3*sizeof(float));
		memcpy(House_Tri[ind+2].Normal, n3, 3*sizeof(float));
		if(i == 0) {
			printf("Positions for first triangle:\n");
			printf("%3f %3f %3f %3f \n %3f %3f %3f %3f \n %3f %3f %3f %3f\n\n",
			House_Tri[ind].Position[0], House_Tri[ind].Position[1],
			House_Tri[ind].Position[2], House_Tri[ind].Position[3],
			House_Tri[ind+1].Position[0], House_Tri[ind+1].Position[1],
			House_Tri[ind+1].Position[2], House_Tri[ind+1].Position[3],
			House_Tri[ind+2].Position[0], House_Tri[ind+2].Position[1],
			House_Tri[ind+2].Position[2], House_Tri[ind+2].Position[3]);
		
			printf("Normals for first triangle:\n");
			printf("%3f %3f %3f %3f \n %3f %3f %3f %3f \n %3f %3f %3f %3f\n\n",
			House_Tri[ind].Normal[0], House_Tri[ind].Normal[1],
			House_Tri[ind].Normal[2], House_Tri[ind].Normal[3],
			House_Tri[ind+1].Normal[0], House_Tri[ind+1].Normal[1],
			House_Tri[ind+1].Normal[2], House_Tri[ind+1].Normal[3],
			House_Tri[ind+2].Normal[0], House_Tri[ind+2].Normal[1],
			House_Tri[ind+2].Normal[2], House_Tri[ind+2].Normal[3]);
		}
	}


        /* Move Houses to proper locations  */

        Num_of_Houses = 0;
        for (i=0; i<2; i++) {
            for (j=0; j<i+1; j++) {
                IdentMat4(m1); IdentMat4(M);
                Translate(m2, i*4.0, 0.0, -j*4.0);
                MultMat4(M, m1, m2);
                ind = Num_of_Houses*48;
                for (k=0; k<48; k++) {
                        memcpy(a2, House_Tri[k].Position, 4*sizeof(float));
                        MultMatV(a1, M, a2);
                        memcpy(All_Houses[ind+k].Position, a1, 4*sizeof(float));
                        memcpy(All_Houses[ind+k].Color, House_Tri[k].Color, 4*sizeof(float));
			memcpy(All_Houses[ind+k].Normal, House_Tri[k].Normal, 4*sizeof(float));
/*
			printf("%3f %3f %3f %3f   %3f %3f %3f %3f   %3f %3f %3f %3f\n",
			House_Tri[k].Position[0], House_Tri[k].Position[1], 
			House_Tri[k].Position[2], House_Tri[k].Position[3],
			House_Tri[k].Color[0], House_Tri[k].Color[1],
                        House_Tri[k].Color[2], House_Tri[k].Color[3],
                        House_Tri[k].Normal[0], House_Tri[k].Normal[1],
                        House_Tri[k].Normal[2], House_Tri[k].Normal[3]);
*/
                }
                Num_of_Houses += 1;
            }
        }

/*      2 VAOs: first for the house and second for the grid */
/*      3 VBOs: the first one (0) for vertices, the second one (1) for colors, third one (2) for normals */

	glGenVertexArrays(3, &VAOIds[0]);
	glBindVertexArray(VAOIds[0]);
	glGenBuffers(3, &VBOIds[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOIds[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(All_Houses), All_Houses, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*12, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4*12, (GLvoid*)16);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4*12, (GLvoid*)32);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindVertexArray(VAOIds[1]);
	glGenBuffers(3, &VBOIds[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOIds[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Axes), Axes, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*12, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4*12, (GLvoid*)16);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4*12, (GLvoid*)32);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	
	// Snow
	glBindVertexArray(VAOIds[2]);
	glGenBuffers(3, &VBOIds[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOIds[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Snow), Snow, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*12, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4*12, (GLvoid*)16);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4*12, (GLvoid*)32);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

        glBindVertexArray(0);

}

void Add_Lights() 
{

/* light 0: ambient light */
	float l0_amb[4] = {0.05, 0.05, 0.05, 1};  /* add the color components of the light here */
	float r0_amb[4] = {1, 1, 1, 1};  /* add the color components of the reflection here */

/* light 1: blueish light in infinity in x-axis: amb and dif components */

	float l1_dir[4] = {1, 0.2, 0.5, 1}; /* ? direction */
	float l1_amb[4] = {0, 0, 0, 1}; /* ? ambient component */
	float l1_dif[4] = {0.15, 0.15, 0.15, 1}; /* ? diffuse component */
	float r1_amb[4] = {1, 1, 1, 1}; /* ? ambient component */
	float r1_dif[4] = {1, 1, 1, 1}; /* ? diffuse component */

/* light 2: greenish light in z-axis, not very far: amb, dif, and spec components */

	float l2_pos[4] = {0, 2, 3, 1}; /* ? position */
	float l2_amb[4] = {0, 0, 0, 1}; /* ? ambient component */
	float l2_dif[4] = {1, 0, 1, 1}; /* ? diffuse component */
	float l2_spe[4] = {1, 0, 1, 1}; /* ? specular component */
	float r2_amb[4] = {1, 1, 1, 1}; /* ? ambient component */
	float r2_dif[4] = {1, 1, 1, 1}; /* ? diffuse component {0.11, 0.57, 0.78, 1.0}; */
	float r2_spe[4] = {1, 1, 1, 1}; /* ? specular component */

/* light 3: spotlight above the houses:  */
/*   How to implement this? */

/* finally : uniforms for the shader */

	Prod(C_L0_Ambient, l0_amb, r0_amb);

	memcpy(L1_Direction, l1_dir, 4*sizeof(float));
	Prod(C_L1_Ambient, l1_amb, r1_amb);
	Prod(C_L1_Diffuse, l1_dif, r1_dif);

	memcpy(L2_Position, l2_pos, 4*sizeof(float)); /* ? position */
	Prod(C_L2_Ambient, l2_amb, r2_amb); /* ? ambient */
	Prod(C_L2_Diffuse, l2_dif, r2_dif); /* ? diffuse */
	Prod(C_L2_Specular, l2_spe, r2_spe); /* ? specular */
	L2_Shininess = 100;

        printf("Ambient components for Light 1: %f %f %f %f\n", C_L1_Ambient[0],
	C_L1_Ambient[1], C_L1_Ambient[2], C_L1_Ambient[3]);

}

void DestroyVBO(void)
{

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(3, &VBOIds[0]);
	glBindVertexArray(0);
	glDeleteVertexArrays(2, &VAOIds[0]);

}

