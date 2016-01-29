#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

GLuint  VertexShaderId, FragmentShaderId, ProgramId;

const GLchar* VertexShader =
{
	"#version 330\n"\

	"layout(location=0) in vec4 vPos;\n"\
	"layout(location=1) in vec4 vCol;\n"\
	"layout(location=2) in vec4 vNor;\n"\
	"layout(location=3) in vec2 vtexcoord;\n"\
	
	"out vec4 ex_Color;\n"\
	"out vec2 tex_coord;\n"\

	"uniform mat4 ModelViewMatrix;\n"\
	"uniform mat4 ProjectionMatrix;\n"\

	"uniform ivec4 lights;\n"\
	"uniform vec4 C_L0_Amb;\n"\
	"uniform vec4 L1_Dir, C_L1_Amb, C_L1_Dif;\n"\
	"uniform vec4 L2_Pos, C_L2_Amb, C_L2_Dif, C_L2_Spe;\n"\
	"uniform float L2_Shin;\n"\
	"vec4 zer = vec4(0.0, 0.0, 0.0, 0.0);\n"\

	"void main(void)\n"\
	"{\n"\
	"	gl_Position = ProjectionMatrix*ModelViewMatrix*vPos;\n"\
	"	mat4 NMatrix = transpose(inverse(ModelViewMatrix));\n"\
	"	vec3 N = normalize((NMatrix*vNor).xyz);\n"\
	"	vec3 L_l1 = normalize((NMatrix*L1_Dir).xyz);\n"\
	"	vec3 L_l2 = normalize((ModelViewMatrix*L2_Pos).xyz-(ModelViewMatrix*vPos).xyz);\n"\
	"	vec3 E_l2 = vec3(0.0, 0.0, 1.0);\n"\
	"	vec3 H_l2 = normalize(L_l2+E_l2);\n"\
	"	float Kd_l1 = max(dot(L_l1,N), 0.0);\n"\
	"	float Kd_l2 = max(dot(L_l2,N), 0.0);\n"\
	"	float Ks_l2 = pow(max(dot(N,H_l2), 0.0), L2_Shin);\n"\
	"	float D_l2 = distance(L2_Pos.xyz, vPos.xyz);\n"\
	"	float Att_l2 = 1.0+0.1*D_l2+0.1*D_l2*D_l2;\n"\
	"	Att_l2 = 1.0/Att_l2;\n"\
	"	if (lights.x==0 && lights.y==0 && lights.z==0 && lights.w == 0) zer = vCol;\n"\
	"	if (lights.x==1) zer = zer + vCol*C_L0_Amb;\n"\
	"	if (lights.y==1) zer = zer + vCol*(C_L1_Amb + Kd_l1*C_L1_Dif);\n"\
	"	if (lights.z==1) zer = zer + vCol*C_L2_Amb + Att_l2*(vCol*Kd_l2*C_L2_Dif + Ks_l2*C_L2_Spe);\n"\
	"	if (lights.w==1) zer = vCol;\n"\
	"	ex_Color = zer;\n"\
	"	tex_coord = vtexcoord;\n"\
	"}\n"



	// "#version 330\n"\

	// "layout(location=0) in vec4 in_Pos;\n"\
	// "layout(location=1) in vec4 in_Color;\n"\
	// "layout(location=2) in vec4 vNor;\n"\
	// "layout(location=3) in vec2 vtexcoord;\n"\

	// "out vec4 ex_Color;\n"\
	// "out vec2 tex_coord;\n"\

	// "uniform mat4 ModelViewMatrix;\n"\
	// "uniform mat4 ProjectionMatrix;\n"\

	// "void main(void)\n"\
	// "{\n"\
	// "	gl_Position = ProjectionMatrix*ModelViewMatrix*in_Pos;\n"\
	// "	ex_Color = in_Color;\n"\
	// "	tex_coord = vtexcoord;\n"\
	// "}\n"
};

const GLchar* FragmentShader =
{
	"#version 330\n"\

	"in vec4 ex_Color;\n"\
	"in vec2 tex_coord;\n"\

	"uniform int VAONumber;\n"\
	"uniform sampler2D mytexS; /* Snow */ \n"\
	"uniform sampler2D mytexR; /* Roof  */ \n"\

	"out vec4 out_Color;\n"\

	"void main(void)\n"\
	"{\n"\
	"	if (VAONumber==0) out_Color = ex_Color;\n"\
	"	if (VAONumber==1) out_Color = ex_Color*texture(mytexR, tex_coord);\n"\
	"	if (VAONumber==2) out_Color = ex_Color*texture(mytexS, tex_coord);\n"\
	"	if (VAONumber==3) out_Color = ex_Color;\n"\
	"}\n"

	// "#version 330\n"\

	// "in vec4 ex_Color;\n"\
	// "in vec2 tex_coord;\n"\

	// "uniform int VAONumber;\n"\
	// "uniform sampler2D mytexP; /* Plane */ \n"\
	// "uniform sampler2D mytexR; /* Roof  */ \n"\

	// "out vec4 out_Color;\n"\

	// "void main(void)\n"\
	// "{\n"\
	// "	if (VAONumber==0) out_Color = ex_Color;\n"\
	// "	if (VAONumber==1) out_Color = texture(mytexR, tex_coord);\n"\
	// "	if (VAONumber==2) out_Color = texture(mytexP, tex_coord);\n"\
	// "	if (VAONumber==3) out_Color = ex_Color;\n"\
	// "}\n"
};

const GLuint CreateShaders(void)
{
        VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(VertexShaderId, 1, &VertexShader, NULL);
        glCompileShader(VertexShaderId);

        FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(FragmentShaderId, 1, &FragmentShader, NULL);
        glCompileShader(FragmentShaderId);

        ProgramId = glCreateProgram();
        glAttachShader(ProgramId, VertexShaderId);
        glAttachShader(ProgramId, FragmentShaderId);
        glLinkProgram(ProgramId);
        glUseProgram(ProgramId);

	return(ProgramId);
}

void DestroyShaders(void)
{

        glUseProgram(0);

        glDetachShader(ProgramId, VertexShaderId);
        glDetachShader(ProgramId, FragmentShaderId);

        glDeleteShader(FragmentShaderId);
        glDeleteShader(VertexShaderId);

        glDeleteProgram(ProgramId);

}

