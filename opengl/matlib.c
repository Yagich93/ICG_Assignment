#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <GL/glew.h>


typedef struct Vertex
{
        float Position[4];
        float Color[4];
	float Normal[4];
} Vertex;


void CrossProd( float *res, float a[], float b[]) {

    float ares[3];
    ares[0] = a[1]*b[2]-a[2]*b[1];
    ares[1] = a[2]*b[0]-a[0]*b[2];
    ares[2] = a[0]*b[1]-a[1]*b[0];
    memcpy(res, ares, 3 * sizeof(float));

}

void Prod( float *res, float a[], float b[]) {

    float ares[4];
    ares[0] = a[0]*b[0]; ares[1] = a[1]*b[1];
    ares[2] = a[2]*b[2]; ares[3] = a[3]*b[3];
    memcpy(res, ares, 4 * sizeof(float));

}

void Normalize(float *vec) {

    float ed, ares[3];

    ed  = sqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]);
    ares[0] = vec[0]/ed; ares[1] = vec[1]/ed; ares[2] = vec[2]/ed;
    memcpy(vec, ares, 3 * sizeof(float));

}

void IdentMat4( float *mat) {

    int i, size = 4;
    float ares[16];
    for (i=0; i<size*size; ++i) ares[i]=0.0f;
    for (i=0; i<size; ++i) ares[i+i*size]=1.0f;
    memcpy(mat, ares, 16 * sizeof(float));

}

void MultMat4(float *mat, float a[], float b[]) {

    int i, j, k;
    float ares[16];

    for (i=0; i<4; ++i) {
        for (j=0; j<4; ++j) {
            ares[j*4+i] = 0.0f;
            for (k=0; k<4; ++k) {
                ares[j*4+i] += a[k*4+i]*b[j*4+k];
            }
        }
    }
    memcpy(mat, ares, 16 * sizeof(float));
}

void MultMatV(float *vec, float a[], float v[] ) {

    int i, j;
    float ares[4];

    for (i=0; i<4; ++i) {
        ares[i] = 0.0f;
        for (j=0; j<4; ++j) {
            ares[i] += a[j*4+i]*v[j];
        }
    }
    memcpy(vec, ares, 4 * sizeof(float));

}

void Translate(float *mat, float x, float y, float z) {

    IdentMat4(mat);
    mat[12] = x;
    mat[13] = y;
    mat[14] = z;

}

void Scale(float *mat,  float x, float y, float z) {

    float ares[16];

    IdentMat4(ares);
    ares[0] = x;
    ares[5] = y;
    ares[10] = z;
    memcpy(mat, ares, 16 * sizeof(float));

}

void RotateX(float *mat, float angledeg) {
    float anglerad = angledeg*M_PI/180;
    float ares[16];

    IdentMat4(ares);
    ares[5] =  cos(anglerad);
    ares[6] =  sin(anglerad);
    ares[9] = -ares[6];
    ares[10] = ares[5];
    memcpy(mat, ares, 16 * sizeof(float));

}

void RotateY(float *mat, float angledeg) {
    float anglerad = angledeg*M_PI/180;
    float ares[16];

    IdentMat4(mat);
    ares[0] =  cos(anglerad);
    ares[2] = -sin(anglerad);
    ares[8] = -ares[2];
    ares[10] = ares[0];
    memcpy(mat, ares, 16 * sizeof(float));

}

void RotateZ(float *mat, float angledeg) {
    float anglerad = angledeg*M_PI/180;
    float ares[16];

    IdentMat4(mat);
    ares[0] =  cos(anglerad);
    ares[1] =  sin(anglerad);
    ares[4] = -ares[1];
    ares[5] =  ares[0];
    memcpy(mat, ares, 16 * sizeof(float));

}

void LookAt( GLfloat *mat,
	float cax, float cay, float caz,
	float lax, float lay, float laz,
	float upx, float upy, float upz) {

    float upvec[3], nvec[3], uvec[3], vvec[3];
    float m1[16], m2[16];

    upvec[0] = upx; upvec[1] = upy; upvec[2] = upz;

    nvec[0] =  (cax-lax);
    nvec[1] =  (cay-lay);
    nvec[2] =  (caz-laz);
    Normalize(nvec);

    CrossProd(uvec, upvec, nvec); Normalize(uvec);
    CrossProd(vvec, nvec, uvec); Normalize(vvec);

    IdentMat4(m1);

    m1[0]  = uvec[0];
    m1[4]  = uvec[1];
    m1[8]  = uvec[2];

    m1[1]  = vvec[0];
    m1[5]  = vvec[1];
    m1[9]  = vvec[2];

    m1[2]  = nvec[0];
    m1[6]  = nvec[1];
    m1[10] = nvec[2];

    IdentMat4(m2);
    m2[12] = -cax;
    m2[13] = -cay;
    m2[14] = -caz;

    MultMat4(m1, m1, m2);

    memcpy(mat, m1, 16 * sizeof(GLfloat));
}

void Ortho(GLfloat *mat, float l, float r, float b, float t, float zn, float zf)
{
    float m1[16];

    IdentMat4(m1);

    m1[0] =  2/(r-l);
    m1[5] =  2/(t-b);
    m1[10] = -2/(zf-zn);
    m1[12] = -(r+l)/(r-l);
    m1[13] = -(t+b)/(t-b);
    m1[14] = -(zf+zn)/(zf-zn);

    memcpy(mat, m1, 16 * sizeof(GLfloat));
}

void Frustum(GLfloat *mat, float l, float r, float b, float t, float zn, float zf)
{

    float m1[16];

    IdentMat4(m1);

    m1[0] =  2.0*zn/(r-l);
    m1[5] =  2.0*zn/(t-b);
    m1[8] =  (r+l)/(r-l);
    m1[9] =  (t+b)/(t-b);
    m1[10] = -(zf+zn)/(zf-zn);
    m1[11] = -1.0;
    m1[14] = -2.0*(zf*zn)/(zf-zn);
    m1[15] = 0.0;

    memcpy(mat, m1, 16 * sizeof(GLfloat));

}
