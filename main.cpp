#include <iostream>
#include <stdlib.h>
#include <windows.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "imageloader.h"
#include "vec3f.h"

using namespace std;

static float _angle = 250.0;//250
static int viewx = 0;
static int viewy = 0;
static int viewz = 0;
//terrain
//Represents a terrain, by storing a set of heights and normals at 2D locations

class Terrain {
private:
    int w; //Width
    int l; //Length
    float** hs; //Heights
    Vec3f** normals;
    bool computedNormals; //Whether normals is up-to-date
public:

    Terrain(int w2, int l2) {
        w = w2;
        l = l2;

        hs = new float*[l];
        for (int i = 0; i < l; i++) {
            hs[i] = new float[w];
        }

        normals = new Vec3f*[l];
        for (int i = 0; i < l; i++) {
            normals[i] = new Vec3f[w];
        }

        computedNormals = false;
    }

    ~Terrain() {
        for (int i = 0; i < l; i++) {
            delete[] hs[i];
        }
        delete[] hs;

        for (int i = 0; i < l; i++) {
            delete[] normals[i];
        }
        delete[] normals;
    }

    int width() {
        return w;
    }

    int length() {
        return l;
    }

    //Sets the height at (x, z) to y

    void setHeight(int x, int z, float y) {
        hs[z][x] = y;
        computedNormals = false;
    }

    //Returns the height at (x, z)

    float getHeight(int x, int z) {
        return hs[z][x];
    }

    //Computes the normals, if they haven't been computed yet

    void computeNormals() {
        if (computedNormals) {
            return;
        }

        //Compute the rough version of the normals
        Vec3f** normals2 = new Vec3f*[l];
        for (int i = 0; i < l; i++) {
            normals2[i] = new Vec3f[w];
        }

        for (int z = 0; z < l; z++) {
            for (int x = 0; x < w; x++) {
                Vec3f sum(0.0f, 0.0f, 0.0f);

                Vec3f out;
                if (z > 0) {
                    out = Vec3f(0.0f, hs[z - 1][x] - hs[z][x], -1.0f);
                }
                Vec3f in;
                if (z < l - 1) {
                    in = Vec3f(0.0f, hs[z + 1][x] - hs[z][x], 1.0f);
                }
                Vec3f left;
                if (x > 0) {
                    left = Vec3f(-1.0f, hs[z][x - 1] - hs[z][x], 0.0f);
                }
                Vec3f right;
                if (x < w - 1) {
                    right = Vec3f(1.0f, hs[z][x + 1] - hs[z][x], 0.0f);
                }

                if (x > 0 && z > 0) {
                    sum += out.cross(left).normalize();
                }
                if (x > 0 && z < l - 1) {
                    sum += left.cross(in).normalize();
                }
                if (x < w - 1 && z < l - 1) {
                    sum += in.cross(right).normalize();
                }
                if (x < w - 1 && z > 0) {
                    sum += right.cross(out).normalize();
                }

                normals2[z][x] = sum;
            }
        }

        //Smooth out the normals
        const float FALLOUT_RATIO = 0.5f;
        for (int z = 0; z < l; z++) {
            for (int x = 0; x < w; x++) {
                Vec3f sum = normals2[z][x];

                if (x > 0) {
                    sum += normals2[z][x - 1] * FALLOUT_RATIO;
                }
                if (x < w - 1) {
                    sum += normals2[z][x + 1] * FALLOUT_RATIO;
                }
                if (z > 0) {
                    sum += normals2[z - 1][x] * FALLOUT_RATIO;
                }
                if (z < l - 1) {
                    sum += normals2[z + 1][x] * FALLOUT_RATIO;
                }

                if (sum.magnitude() == 0) {
                    sum = Vec3f(0.0f, 1.0f, 0.0f);
                }
                normals[z][x] = sum;
            }
        }

        for (int i = 0; i < l; i++) {
            delete[] normals2[i];
        }
        delete[] normals2;

        computedNormals = true;
    }

    //Returns the normal at (x, z)

    Vec3f getNormal(int x, int z) {
        if (!computedNormals) {
            computeNormals();
        }
        return normals[z][x];
    }
};

//gazibu
void gazibu(){
    glPushMatrix();
    glScaled(0.8, 0.04, 0.8);
    glTranslatef(0.0, -30.2, 0.0);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(0.5, 0, 0);
    glutSolidCube(7.0);
    glPopMatrix();

    //atap
    glPushMatrix();
    glScaled(0.8, 1.0, 0.8);
    glTranslatef(0.0, 0.9, 0.0);
    glRotated(45, 0, 1, 0);
    glRotated(-90, 1, 0, 0);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(0.5450980392156863, 0.2705882352941176, 0.0745098039215686);
    glutSolidCone(4.5, 2, 4, 1);
    glPopMatrix();

    //atap2
    glPushMatrix();
    glScaled(0.1, 0.2, 0.1);
    glTranslatef(0.0, 13.8, 0.0);
    glRotated(45, 0, 1, 0);
    glRotated(-90, 1, 0, 0);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3d(0.803921568627451, 0.5215686274509804, 0.2470588235294118);
    glutSolidCone(4.5, 2, 4, 1);
    glPopMatrix();


    glPushMatrix();
    glScaled(0.01, 0.12, 0.01);
    glTranslatef(-100, -6, 240);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(0.5450980392156863, 0.2705882352941176, 0.0745098039215686);
    glutSolidCube(7.0);
    glPopMatrix();

    glPushMatrix();
    glScaled(0.01, 0.12, 0.01);
    glTranslatef(100, -6, 240);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(0.5450980392156863, 0.2705882352941176, 0.0745098039215686);
    glutSolidCube(7.0);
    glPopMatrix();

    glPushMatrix();
    glScaled(0.01, 0.12, 0.01);
    glTranslatef(-100, -6, -240);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(0.5450980392156863, 0.2705882352941176, 0.0745098039215686);
    glutSolidCube(7.0);
    glPopMatrix();

    glPushMatrix();
    glScaled(0.01, 0.12, 0.01);
    glTranslatef(100, -6, -240);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(0.5450980392156863, 0.2705882352941176, 0.0745098039215686);
    glutSolidCube(7.0);
    glPopMatrix();
    //depan kanan
    glPushMatrix();
    glScaled(0.21, 0.01, 0.01);
    glTranslatef(8.0, -30.0, 240.0);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(0.5450980392156863, 0.2705882352941176, 0.0745098039215686);
    glutSolidCube(7.0);
    glTranslatef(0.0, -73, 0.0);
    glutSolidCube(7.0);
    glPopMatrix();
    //bulat depan kanan
    glPushMatrix();
    glScaled(0.4, 0.4, 0.4);
    glTranslatef(4.8, -1.7, 6.0);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(0.5450980392156863, 0.2705882352941176, 0.0745098039215686);
    glutSolidTorus(0.1, 0.85, 100, 100);
    glTranslatef(-1.4, 0.0, 0.0);
    glutSolidTorus(0.1, 0.85, 100, 100);
    glPopMatrix();

    //depan kiri
    glPushMatrix();
    glScaled(0.21, 0.01, 0.01);
    glTranslatef(-8.0, -30.0, 240.0);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(0.5450980392156863, 0.2705882352941176, 0.0745098039215686);
    glutSolidCube(7.0);
    glTranslatef(0.0, -73, 0.0);
    glutSolidCube(7.0);
    glPopMatrix();
    //bulat depan kiri
    glPushMatrix();
    glScaled(0.4, 0.4, 0.4);
    glTranslatef(-3.5, -1.7, 6.0);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(0.5450980392156863, 0.2705882352941176, 0.0745098039215686);
    glutSolidTorus(0.1, 0.85, 100, 100);
    glTranslatef(-1.4, 0.0, 0.0);
    glutSolidTorus(0.1, 0.85, 100, 100);
    glPopMatrix();

    //belakang kanan
    glPushMatrix();
    glScaled(0.21, 0.01, 0.01);
    glTranslatef(8.0, -30.0, -240.0);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(0.5450980392156863, 0.2705882352941176, 0.0745098039215686);
    glutSolidCube(7.0);
    glTranslatef(0.0, -73, 0.0);
    glutSolidCube(7.0);
    glPopMatrix();
    //bulat belakang kanan
    glPushMatrix();
    glScaled(0.4, 0.4, 0.4);
    glTranslatef(4.8, -1.7, -6.0);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(0.5450980392156863, 0.2705882352941176, 0.0745098039215686);
    glutSolidTorus(0.1, 0.85, 100, 100);
    glTranslatef(-1.4, 0.0, 0.0);
    glutSolidTorus(0.1, 0.85, 100, 100);
    glPopMatrix();

    //belakang kiri
    glPushMatrix();
    glScaled(0.21, 0.01, 0.01);
    glTranslatef(-8.0, -30.0, -240.0);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(0.5450980392156863, 0.2705882352941176, 0.0745098039215686);
    glutSolidCube(7.0);
    glTranslatef(0.0, -73, 0.0);
    glutSolidCube(7.0);
    glPopMatrix();
    //bulat belakang kiri
    glPushMatrix();
    glScaled(0.4, 0.4, 0.4);
    glTranslatef(-3.5, -1.7, -6.0);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(0.5450980392156863, 0.2705882352941176, 0.0745098039215686);
    glutSolidTorus(0.1, 0.85, 100, 100);
    glTranslatef(-1.4, 0.0, 0.0);
    glutSolidTorus(0.1, 0.85, 100, 100);
    glPopMatrix();

    //samping kanan
    glPushMatrix();
    glScaled(0.01, 0.01, 0.7);
    glTranslatef(240.0, -30.0, 0.0);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(0.5450980392156863, 0.2705882352941176, 0.0745098039215686);
    glutSolidCube(7.0);
    glTranslatef(0, -73, 0.0);
    glScaled(1.0, 1.0, 0.98);
    glutSolidCube(7.0);
    glPopMatrix();

    //bulat kanan
    glPushMatrix();
    glRotated(90, 0, 1, 0);
    glScaled(0.4, 0.4, 0.4);
    glTranslatef(4.8, -1.7, 6.0);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(0.5450980392156863, 0.2705882352941176, 0.0745098039215686);
    glutSolidTorus(0.1, 0.85, 100, 100);
    glTranslatef(-1.4, 0.0, 0.0);
    glutSolidTorus(0.1, 0.85, 100, 100);
    glTranslatef(-1.4, 0.0, 0.0);
    glutSolidTorus(0.1, 0.85, 100, 100);
    glTranslatef(-1.4, 0.0, 0.0);
    glutSolidTorus(0.1, 0.85, 100, 100);
    glTranslatef(-1.4, 0.0, 0.0);
    glutSolidTorus(0.1, 0.85, 100, 100);
    glTranslatef(-1.4, 0.0, 0.0);
    glutSolidTorus(0.1, 0.85, 100, 100);
    glTranslatef(-1.4, 0.0, 0.0);
    glutSolidTorus(0.1, 0.85, 100, 100);
    glTranslatef(-1.4, 0.0, 0.0);
    glutSolidTorus(0.1, 0.85, 100, 100);
    glPopMatrix();

    //samping kiri
    glPushMatrix();
    glScaled(0.01, 0.01, 0.7);
    glTranslatef(-240.0, -30.0, 0.0);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(0.5450980392156863, 0.2705882352941176, 0.0745098039215686);
    glutSolidCube(7.0);
    glTranslatef(0, -73, 0.0);
    glScaled(1.0, 1.0, 0.98);
    glutSolidCube(7.0);
    glPopMatrix();

    //bulat kiri
    glPushMatrix();
    glRotated(90, 0, 1, 0);
    glScaled(0.4, 0.4, 0.4);
    glTranslatef(4.8, -1.7, -6.0);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(0.5450980392156863, 0.2705882352941176, 0.0745098039215686);
    glutSolidTorus(0.1, 0.85, 100, 100);
    glTranslatef(-1.4, 0.0, 0.0);
    glutSolidTorus(0.1, 0.85, 100, 100);
    glTranslatef(-1.4, 0.0, 0.0);
    glutSolidTorus(0.1, 0.85, 100, 100);
    glTranslatef(-1.4, 0.0, 0.0);
    glutSolidTorus(0.1, 0.85, 100, 100);
    glTranslatef(-1.4, 0.0, 0.0);
    glutSolidTorus(0.1, 0.85, 100, 100);
    glTranslatef(-1.4, 0.0, 0.0);
    glutSolidTorus(0.1, 0.85, 100, 100);
    glTranslatef(-1.4, 0.0, 0.0);
    glutSolidTorus(0.1, 0.85, 100, 100);
    glTranslatef(-1.4, 0.0, 0.0);
    glutSolidTorus(0.1, 0.85, 100, 100);
    glPopMatrix();

    glTranslatef(0.0, 0.4, 0.0);
    glScaled(1.2, 0.25, 1.2);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(0, 0.5, 0);


    //tiang1
    glPushMatrix();
    glTranslated(2, -1.9, 2);
    glScaled(.2, 10.5, .2);
    glutSolidSphere(0.4, 80, 120);
    glPopMatrix();


    //tiang2
    glPushMatrix();
    glTranslated(-2, -1.9, 2);
    glScaled(.2, 10.5, .2);
    glutSolidSphere(0.4, 80, 120);
    glPopMatrix();


    //tiang3
    glPushMatrix();
    glTranslated(-2, -1.9, -2);
    glScaled(.2, 10.5, .2);
    glutSolidSphere(0.4, 80, 120);
    glPopMatrix();


    //tiang4
    glPushMatrix();
    glTranslated(2, -1.9, -2);
    glScaled(.2, 10.5, .2);
    glutSolidSphere(0.4, 80, 120);
    glPopMatrix();
}

//bouncy castle
void bouncyCastle(){
    //sisi kanan
        
    
        glColor3f(1, 0.58,0);
        glPushMatrix();
        glScaled(5.80, 1.0, 5.80);
        glTranslatef(1,0.5,0);
        glutSolidCube(2);
        glPopMatrix();
        glPushMatrix();;
        glScaled(1.0, 1.0, 15.0);
        glutSolidTorus(0.4,0.4,60,80);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.0f, 1.0f, 0.0f);
        glScaled(1.0, 1.0, 15.0);
        glutSolidTorus(0.4,0.4,60,80);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.0f, 2.0f, 0.0f);
        glScaled(1.0, 1.0, 15.0);
        glutSolidTorus(0.4,0.4,60,80);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.0f, 3.0f, 0.0f);
        glScaled(1.0, 1.0, 15.0);
        glutSolidTorus(0.4,0.4,60,80);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.0f, 4.0f, 0.0f);
        glScaled(1.0, 1.0, 15.0);
        glutSolidTorus(0.4,0.4,60,80);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.0f, 5.0f, 0.0f);
        glScaled(1.0, 1.0, 15.0);
        glutSolidTorus(0.4,0.4,60,80);
        glPopMatrix();
        
        //sisi belakang
        glPushMatrix();
        glRotatef(90,0.0,1.0,0.0);
        glTranslatef(-6.0f, 0.0f, 6.0f);
        glScaled(1.0, 1.0, 15.0);
        glutSolidTorus(0.4,0.4,60,80);
        glPopMatrix();
        glPushMatrix();
        glRotatef(90,0.0,1.0,0.0);
        glTranslatef(-6.0f, 1.0f, 6.0f);
        glScaled(1.0, 1.0, 15.0);
        glutSolidTorus(0.4,0.4,60,80);
        glPopMatrix();
        glPushMatrix();
        glRotatef(90,0.0,1.0,0.0);
        glTranslatef(-6.0f, 2.0f, 6.0f);
        glScaled(1.0, 1.0, 15.0);
        glutSolidTorus(0.4,0.4,60,80);
        glPopMatrix();     
        glPushMatrix();
        glRotatef(90,0.0,1.0,0.0);
        glTranslatef(-6.0f, 3.0f, 6.0f);
        glScaled(1.0, 1.0, 15.0);
        glutSolidTorus(0.4,0.4,60,80);
        glPopMatrix();      
        glPushMatrix();
        glRotatef(90,0.0,1.0,0.0);
        glTranslatef(-6.0f, 4.0f, 6.0f);
        glScaled(1.0, 1.0, 15.0);
        glutSolidTorus(0.4,0.4,60,80);
        glPopMatrix();
        glPushMatrix();
        glRotatef(90,0.0,1.0,0.0);
        glTranslatef(-6.0f, 5.0f, 6.0f);
        glScaled(1.0, 1.0, 15.0);
        glutSolidTorus(0.4,0.4,60,80);
        glPopMatrix();
        
        //sisi kiri
        glPushMatrix();
        glTranslatef(12.0f, 0.0f, 0.0f);
        glScaled(1.0, 1.0, 15.0);
        glutSolidTorus(0.4,0.4,60,80);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(12.0f, 1.0f, 0.0f);
        glScaled(1.0, 1.0, 15.0);
        glutSolidTorus(0.4,0.4,60,80);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(12.0f, 2.0f, 0.0f);
        glScaled(1.0, 1.0, 15.0);
        glutSolidTorus(0.4,0.4,60,80);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(12.0f, 3.0f, 0.0f);
        glScaled(1.0, 1.0, 15.0);
        glutSolidTorus(0.4,0.4,60,80);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(12.0f, 4.0f, 0.0f);
        glScaled(1.0, 1.0, 15.0);
        glutSolidTorus(0.4,0.4,60,80);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(12.0f, 5.0f, 0.0f);
        glScaled(1.0, 1.0, 15.0);
        glutSolidTorus(0.4,0.4,60,80);
        glPopMatrix();
        
        //tiang 1
        glColor3f(0.8,0.2,0.3);
        glPushMatrix();
        glRotatef(90,1,0,0);
        glTranslatef(12.0f, 6.0f, -3.5f);
        glScaled(1.4, 1.4, 11.0);
        glutSolidTorus(0.4,0.4,60,80);
        glPopMatrix();
        
        //tiang 2
        glPushMatrix();
        glRotatef(90,1,0,0);
        glTranslatef(0.0f, 6.0f, -3.5f);
        glScaled(1.4, 1.4, 11.0);
        glutSolidTorus(0.4,0.4,60,80);
        glPopMatrix();
        
       //tiang 3
        glPushMatrix();
        glRotatef(90,1,0,0);
        glTranslatef(0.0f, -6.0f, -3.5f);
        glScaled(1.4, 1.4, 11.0);
        glutSolidTorus(0.4,0.4,60,80);
        glPopMatrix();
        
        //tiang 4
        glPushMatrix();
        glRotatef(90,1,0,0);
        glTranslatef(12.0f, -6.0f, -3.5f);
        glScaled(1.4, 1.4, 11.0);
        glutSolidTorus(0.4,0.4,60,80);
        glPopMatrix();
        
        //tangga
        glColor3f(0.2,0.3,1);
        glPushMatrix();
        glRotatef(90,0.0,1.0,0.0);
        glTranslatef(6.0f, 0.6f, 6.0f);
        glScaled(1.0, 1.0, 13.0);
        glutSolidTorus(0.4,0.4,60,80);
        glPopMatrix();
        glPushMatrix();
        glRotatef(90,0.0,1.0,0.0);
        glTranslatef(7.0f, 0.3f, 6.0f);
        glScaled(1.0, 1.0, 13.0);
        glutSolidTorus(0.4,0.4,60,80);
        glPopMatrix();
        glPushMatrix();
        glRotatef(90,0.0,1.0,0.0);
        glTranslatef(8.0f, 0.0f, 6.0f);
        glScaled(1.0, 1.0, 13.0);
        glutSolidTorus(0.4,0.4,60,80);
        glPopMatrix();
        
        //gate
        glColor3f(0.8,0.2,0.3);
        glPushMatrix();
        glRotatef(90,0.0,1.0,0.0);
        glRotatef(15,1.0,0.0,0.0);
        glTranslatef(6.0f, 10.0f, 6.0f);
        glScaled(1.0, 1.0, 8.0);
        glutSolidTorus(0.4,0.4,60,80);
        glPopMatrix();
        glPushMatrix();
        glRotatef(90,0.0,1.0,0.0);
        glRotatef(-15,1.0,0.0,0.0);
        glTranslatef(6.0f, 7.0f, 5.0f);
        glScaled(1.0, 1.0, 8.0);
        glutSolidTorus(0.4,0.4,60,80);
        glPopMatrix();
        glPushMatrix();
        glRotatef(90,0.0,1.0,0.0);
        glRotatef(90,1.0,0.0,0.0);
        glTranslatef(6.0f, 5.8f, -9.0f);
        glScaled(1.2, 1.2, 5.0);
        glutSolidTorus(0.4,0.4,60,80);
        glPopMatrix();
        
        //kerucut
        glColor3f(0.55,0.34,0.114);
        glPushMatrix();
        glRotatef(-90,1.0,0.0,0.0);
        glTranslatef(5.8f, 6.0f, 10.8f);
        glutSolidCone(0.7,1.5,10,10);
        glPopMatrix();
        glPushMatrix();
        glRotatef(-90,1.0,0.0,0.0);
        glTranslatef(12.0f, 6.0f, 7.8f);
        glutSolidCone(0.7,1.5,10,10);
        glPopMatrix();
        glPushMatrix();
        glRotatef(-90,1.0,0.0,0.0);
        glTranslatef(12.0f, -6.0f, 7.8f);
        glutSolidCone(0.7,1.5,10,10);
        glPopMatrix();
        glPushMatrix();
        glRotatef(-90,1.0,0.0,0.0);
        glTranslatef(0.0f, 6.0f, 7.8f);
        glutSolidCone(0.7,1.5,10,10);
        glPopMatrix();
        glPushMatrix();
        glRotatef(-90,1.0,0.0,0.0);
        glTranslatef(0.0f, -6.0f, 7.8f);
        glutSolidCone(0.7,1.5,10,10);
        glPopMatrix();
}

void doubleSwing(){
     
     GLUquadricObj *pObj;
     pObj =gluNewQuadric();
     gluQuadricNormals(pObj, GLU_SMOOTH); 
     
     //tiang kanan
     glPushMatrix();
     glColor3ub(215,215,215);
     glTranslatef(-2,0,0);
     glRotatef(290,1.0,0.0,0.0);
     gluCylinder(pObj, 0.2, 0.2, 10, 25, 25);
     glPopMatrix();
     
     glPushMatrix();
     glColor3ub(215,215,215);
     glTranslatef(-2,0,7);
     glRotatef(250,1.0,0.0,0.0);
     gluCylinder(pObj, 0.2, 0.2, 10, 25, 25);
     glPopMatrix();
     
     //tiang kiri
     glPushMatrix();
     glColor3ub(215,215,215);
     glTranslatef(12,0,0);
     glRotatef(290,1.0,0.0,0.0);
     gluCylinder(pObj, 0.2, 0.2, 10, 25, 25);
     glPopMatrix();
     
     glPushMatrix();
     glColor3ub(215,215,215);
     glTranslatef(12,0,7);
     glRotatef(250,1.0,0.0,0.0);
     gluCylinder(pObj, 0.2, 0.2, 10, 25, 25);
     glPopMatrix();
     
     //balokatas
     glPushMatrix();
     glColor3ub(46,79,219);
     glTranslatef(5,9,3.5);
     glScaled(4,0.4,0.1);
     glutSolidCube(4);
     glPopMatrix();
     
     //tali
     glPushMatrix();
     glColor3ub(215,215,215);
     glTranslatef(10,3,3.5);
     glRotatef(-90,1.0,0.0,0.0);
     gluCylinder(pObj, 0.05, 0.05, 6, 25, 25);
     glPopMatrix();
     
     glPushMatrix();
     glColor3ub(215,215,215);
     glTranslatef(7,3,3.5);
     glRotatef(-90,1.0,0.0,0.0);
     gluCylinder(pObj, 0.05, 0.05, 6, 25, 25);
     glPopMatrix();
     
     glPushMatrix();
     glColor3ub(215,215,215);
     glTranslatef(3,3,3.5);
     glRotatef(-90,1.0,0.0,0.0);
     gluCylinder(pObj, 0.05, 0.05, 6, 25, 25);
     glPopMatrix();
     
     glPushMatrix();
     glColor3ub(215,215,215);
     glTranslatef(0,3,3.5);
     glRotatef(-90,1.0,0.0,0.0);
     gluCylinder(pObj, 0.05, 0.05, 6, 25, 25);
     glPopMatrix();
     
     //tempat duduk
     glPushMatrix();
     glColor3ub(105,87,15);
     glTranslatef(1.5,3,3.5);
     glScaled(1.5,0.1,0.6);
     glutSolidCube(2);
     glPopMatrix();
     
     glPushMatrix();
     glColor3ub(105,87,15);
     glTranslatef(8.5,3,3.5);
     glScaled(1.5,0.1,0.6);
     glutSolidCube(2);
     glPopMatrix();
     
}

void roundAbout(){
     
     GLUquadricObj *pObj;
     pObj =gluNewQuadric();
     gluQuadricNormals(pObj, GLU_SMOOTH); 
     
     
     
     //alas
     glPushMatrix();
     glColor3ub(0,0,0);
     glTranslatef(0,-1,-1);
     glScaled(1,0.1,1);
     glutSolidSphere(5, 25, 25);
     glPopMatrix();
     
     glPushMatrix();
     glColor3ub(46,79,219);
     glTranslatef(0,0,-1);
     glScaled(1,0.1,1);
     glutSolidSphere(6, 25, 25);
     glPopMatrix();
     
     //tiang pertama
     
     glTranslatef(0,0,4);
     glRotatef(25,0.0,1.0,0.0);
     
     glPushMatrix();
     glColor3ub(215,215,215);
     glRotatef(-90,1.0,0.0,0.0);
     glTranslatef(0,0,0);
     gluCylinder(pObj, 0.2, 0.2, 5, 25, 25);
     glTranslatef(4,0,0);
     gluCylinder(pObj, 0.2, 0.2, 5, 25, 25);
     glPopMatrix();
     
     glPushMatrix();
     glColor3ub(215,215,215);
     glTranslatef(0,5,0);
     glRotatef(90,0.0,1.0,0.0);
     gluCylinder(pObj, 0.2, 0.2, 4, 25, 25);
     glPopMatrix();
     
     glPushMatrix();
     glColor3ub(215,215,215);
     glTranslatef(2.0,3.0,-3.5);
     glRotatef(-30,1.0,0.0,0.0);
     gluCylinder(pObj, 0.2, 0.2, 4, 25, 25);
     glPopMatrix();
     
     glPushMatrix();
     glColor3ub(215,215,215);
     glTranslatef(2.0,0.8,-3.5);
     glRotatef(-90,1.0,0.0,0.0);
     gluCylinder(pObj, 0.2, 0.2, 2.3, 25, 25);
     glPopMatrix();
     
     //tiang kedua
     
     glTranslatef(-4,0,-4);
     glRotatef(-50,0.0,1.0,0.0);
     
     glPushMatrix();
     glColor3ub(215,215,215);
     glRotatef(-90,1.0,0.0,0.0);
     glTranslatef(0,0,0);
     gluCylinder(pObj, 0.2, 0.2, 5, 25, 25);
     glTranslatef(4,0,0);
     gluCylinder(pObj, 0.2, 0.2, 5, 25, 25);
     glPopMatrix();
     
     glPushMatrix();
     glColor3ub(215,215,215);
     glTranslatef(0,5,0);
     glRotatef(90,0.0,1.0,0.0);
     gluCylinder(pObj, 0.2, 0.2, 4, 25, 25);
     glPopMatrix();
     
     glPushMatrix();
     glColor3ub(215,215,215);
     glTranslatef(2.0,3.0,-3.5);
     glRotatef(-30,1.0,0.0,0.0);
     gluCylinder(pObj, 0.2, 0.2, 4, 25, 25);
     glPopMatrix();
     
     glPushMatrix();
     glColor3ub(215,215,215);
     glTranslatef(2.0,0.8,-3.5);
     glRotatef(-90,1.0,0.0,0.0);
     gluCylinder(pObj, 0.2, 0.2, 2.3, 25, 25);
     glPopMatrix();
     
     //tiang ketiga
     
     glTranslatef(8,0,-8);
     glRotatef(150,0.0,1.0,0.0);
     
     glPushMatrix();
     glColor3ub(215,215,215);
     glRotatef(-90,1.0,0.0,0.0);
     glTranslatef(0,0,0);
     gluCylinder(pObj, 0.2, 0.2, 5, 25, 25);
     glTranslatef(4,0,0);
     gluCylinder(pObj, 0.2, 0.2, 5, 25, 25);
     glPopMatrix();
     
     glPushMatrix();
     glColor3ub(215,215,215);
     glTranslatef(0,5,0);
     glRotatef(90,0.0,1.0,0.0);
     gluCylinder(pObj, 0.2, 0.2, 4, 25, 25);
     glPopMatrix();
     
     glPushMatrix();
     glColor3ub(215,215,215);
     glTranslatef(2.0,3.0,-3.5);
     glRotatef(-30,1.0,0.0,0.0);
     gluCylinder(pObj, 0.2, 0.2, 4, 25, 25);
     glPopMatrix();
     
     glPushMatrix();
     glColor3ub(215,215,215);
     glTranslatef(2.0,0.8,-3.5);
     glRotatef(-90,1.0,0.0,0.0);
     gluCylinder(pObj, 0.2, 0.2, 2.3, 25, 25);
     glPopMatrix();
     
     //tiang keeempat
     
     glTranslatef(8,0,-4);
     glRotatef(100,0.0,1.0,0.0);
     
     glPushMatrix();
     glColor3ub(215,215,215);
     glRotatef(-90,1.0,0.0,0.0);
     glTranslatef(0,0,0);
     gluCylinder(pObj, 0.2, 0.2, 5, 25, 25);
     glTranslatef(4,0,0);
     gluCylinder(pObj, 0.2, 0.2, 5, 25, 25);
     glPopMatrix();
     
     glPushMatrix();
     glColor3ub(215,215,215);
     glTranslatef(0,5,0);
     glRotatef(90,0.0,1.0,0.0);
     gluCylinder(pObj, 0.2, 0.2, 4, 25, 25);
     glPopMatrix();
     
     glPushMatrix();
     glColor3ub(215,215,215);
     glTranslatef(2.0,3.0,-3.5);
     glRotatef(-30,1.0,0.0,0.0);
     gluCylinder(pObj, 0.2, 0.2, 4, 25, 25);
     glPopMatrix();
     
     glPushMatrix();
     glColor3ub(215,215,215);
     glTranslatef(2.0,0.8,-3.5);
     glRotatef(-90,1.0,0.0,0.0);
     gluCylinder(pObj, 0.2, 0.2, 2.3, 25, 25);
     glPopMatrix();
     
}

void monkeyLadder(){
    //tiang kiri kanan depan
    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0);
    glPushMatrix();
    glScalef(1.0,20.0,1.0);
    glTranslatef(-5,0,35.0);
    glutSolidCube(2);
    glPopMatrix();    
    
    glPushMatrix();
    glScalef(1.0,20.0,1.0);
    glTranslatef(5,0,35.0);
    glutSolidCube(2);
    glPopMatrix(); 
    
    glPopMatrix();

//tiang kiri kanan belakang
    glPushMatrix();
    
    glPushMatrix();
    glScalef(1.0,20.0,1.0);
    glTranslatef(-5,0,-35.0);
    glutSolidCube(2);
    glPopMatrix();    
    
    glPushMatrix();
    glScalef(1.0,20.0,1.0);
    glTranslatef(5,0,-35.0);
    glutSolidCube(2);
    glPopMatrix(); 
    
    glPopMatrix();

//tiang atas
  glPushMatrix();
glTranslatef(0.0,19.0,0.0);
 glScalef(1.0,1.0,1.7);
glRotatef(90,1.0,0.0,0.0);


    glPushMatrix();
    
    glPushMatrix();
    glScalef(1.0,20.0,1.0);
glTranslatef(-5.0,0.0,0.0);
    glutSolidCube(2);
    glPopMatrix();    
    
    glPushMatrix();
    glScalef(1.0,20.0,1.0);
glTranslatef(5.0,0.0,0.0);
    glutSolidCube(2);
    glPopMatrix(); 
    
    glPopMatrix();

    glPopMatrix();





//tiang tangga depan
    glPushMatrix();
    glScalef(5.0,1.0,1.0);
    glTranslatef(0.0,-15.0,35.0);
    glutSolidCube(2);
    glPopMatrix(); 


    glPushMatrix();
    glScalef(5.0,1.0,1.0);
    glTranslatef(0.0,-5.0,35.0);
    glutSolidCube(2);
    glPopMatrix(); 
    
    glPushMatrix();
    glScalef(5.0,1.0,1.0);
    glTranslatef(0.0,5.0,35.0);
    glutSolidCube(2);
    glPopMatrix(); 
    
//tiang tangga belakang
    glPushMatrix();
    glScalef(5.0,1.0,1.0);
    glTranslatef(0.0,-15.0,-35.0);
    glutSolidCube(2);
    glPopMatrix(); 


    glPushMatrix();
    glScalef(5.0,1.0,1.0);
    glTranslatef(0.0,-5.0,-35.0);
    glutSolidCube(2);
    glPopMatrix(); 
    
    glPushMatrix();
    glScalef(5.0,1.0,1.0);
    glTranslatef(0.0,5.0,-35.0);
    glutSolidCube(2);
    glPopMatrix(); 
    
    
//tiang tangga atas depan
    glPushMatrix();
    glTranslatef(0.0,-16.0,16.0);
    glRotatef(90,1.0,0.0,0.0);
    
    
    glPushMatrix();
    glScalef(5.0,1.0,1.0);
    glTranslatef(0.0,-10.0,-35.0);
    glutSolidCube(2);
    glPopMatrix(); 


    glPushMatrix();
    glScalef(5.0,1.0,1.0);
    glTranslatef(0.0,0.0,-35.0);
    glutSolidCube(2);
    glPopMatrix(); 
    
    glPushMatrix();
    glScalef(5.0,1.0,1.0);
    glTranslatef(0.0,10.0,-35.0);
    glutSolidCube(2);
    glPopMatrix();     

    glPopMatrix();        
//tiang tangga atas depan
    glPushMatrix();
    glTranslatef(0.0,-16.0,-16.0);
    glRotatef(90,1.0,0.0,0.0);
    
    
    glPushMatrix();
    glScalef(5.0,1.0,1.0);
    glTranslatef(0.0,-10.0,-35.0);
    glutSolidCube(2);
    glPopMatrix(); 


    glPushMatrix();
    glScalef(5.0,1.0,1.0);
    glTranslatef(0.0,0.0,-35.0);
    glutSolidCube(2);
    glPopMatrix(); 
    
    glPushMatrix();
    glScalef(5.0,1.0,1.0);
    glTranslatef(0.0,10.0,-35.0);
    glutSolidCube(2);
    glPopMatrix();     

    glPopMatrix();    
    
    
   glPopMatrix();  
}
void Seesaw(){
     
     //bawah
    glPushMatrix();
    glColor3ub(90,49,0);
    glRotatef(20,1.0,0.0,1.0);
    glScalef(0.2,1,0.2);
    glutSolidCube(2);
    glPopMatrix();
    
    glPushMatrix();
    glColor3ub(90,49,0);
    glTranslatef(-1.0,0.0,0.0);
    glRotatef(20,1.0,0.0,-1.0);
    glScalef(0.2,1,0.2);
    glutSolidCube(2);
    glPopMatrix();
    
    glPushMatrix();
    glColor3ub(90,49,0);
    glTranslatef(0.0,0.0,1.0);
    glRotatef(20,-1.0,0.0,1.0);
    glScalef(0.2,1,0.2);
    glutSolidCube(2);
    glPopMatrix();
    
    glPushMatrix();
    glColor3ub(90,49,0);
    glTranslatef(-1.0,0.0,1.0);
    glRotatef(20,-1.0,0.0,-1.0);
    glScalef(0.2,1,0.2);
    glutSolidCube(2);
    glPopMatrix();
    
    //atas
    glPushMatrix();
    glColor3ub(41,86,254);
    glTranslatef(0.0,1.0,0.5);
    glRotatef(15,0.0,0.0,-1.0);
    glScalef(4.0,0.1,0.4);
    glutSolidCube(4);
    glPopMatrix(); 
    
    //pegangan
    glPushMatrix();
    glColor3ub(255,249,0);
    glTranslatef(5.0,0.3,0.5);
    glRotatef(15,0.0,0.0,-1.0);
    glScalef(0.1,0.2,0.4);
    glutSolidCube(4);
    glPopMatrix();
    
    glPushMatrix();
    glColor3ub(255,249,0);
    glTranslatef(-5.0,3.0,0.5);
    glRotatef(15,0.0,0.0,-1.0);
    glScalef(0.1,0.2,0.4);
    glutSolidCube(4);
    glPopMatrix();
}
void kursi(void){
    
// Batang Tiang Kanan
    glPushMatrix();
    glScaled(0.06, 0.2,0.06);
   glTranslatef(43, 0,380.5); 
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(1,1,1); 
    glutSolidCube(5.0);
    glPopMatrix(); 
    
    
    // Batang Tiang Kiri
    glPushMatrix();
    glScaled(0.06, 0.2,0.06);
   glTranslatef(3, 0,380.5); 
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(1,1,1); 
    glutSolidCube(5.0);
    glPopMatrix(); 
    
    // Batang depan knan
    glPushMatrix();
    glScaled(0.06, 0.2,0.06);
   glTranslatef(43, 0,390.5); 
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(1,1,1);
    glutSolidCube(5.0);
    glPopMatrix(); 
    
    // Batang Depan Kiri
    glPushMatrix();
    glScaled(0.06, 0.2,0.06);
   glTranslatef(3, 0,390.5); 
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(1,1,1); 
    glutSolidCube(5.0);
    glPopMatrix();     

    // atas kursi
    glPushMatrix();
    glScaled(0.6, 0.05,0.3);
   glTranslatef(2.4,8,77); 
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
     glColor3f(1.0000, 0.5252, 0.0157);
    glutSolidCube(5.0);
    glPopMatrix();  
   
}
//Loads a terrain from a heightmap.  The heights of the terrain range from
//-height / 2 to height / 2.

Terrain* loadTerrain(const char* filename, float height) {
    Image* image = loadBMP(filename);
    Terrain* t = new Terrain(image->width, image->height);
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            unsigned char color =
                    (unsigned char) image->pixels[3 * (y * image->width + x)];
            float h = height * ((color / 255.0f) - 0.5f);
            t->setHeight(x, y, h);
        }
    }

    delete image;
    t->computeNormals();
    return t;
}

void terrain(Terrain *terrain, GLfloat r, GLfloat g, GLfloat b) {

    float scale = 80.0f / max(terrain->width() - 1, terrain->length() - 1);
    glScalef(scale, scale, scale);
    glTranslatef(-(float) (terrain->width() - 1) / 2, 0.0f,
            -(float) (terrain->length() - 1) / 2);

    glColor3f(r, g, b);
    for (int z = 0; z < terrain->length() - 1; z++) {
        //Makes OpenGL draw a triangle at every three consecutive vertices
        glBegin(GL_TRIANGLE_STRIP);
        for (int x = 0; x < terrain->width(); x++) {
            Vec3f normal = terrain->getNormal(x, z);
            glNormal3f(normal[0], normal[1], normal[2]);
            glVertex3f(x, terrain->getHeight(x, z), z);
            normal = terrain->getNormal(x, z + 1);
            glNormal3f(normal[0], normal[1], normal[2]);
            glVertex3f(x, terrain->getHeight(x, z + 1), z + 1);
        }
        glEnd();
    }

}


//tanah

 //The rotation of the box
GLuint _textureId; //The OpenGL id of the texture
GLuint _textureId1;
GLuint _displayListGazibu;
GLuint _displayListBouncy;
GLuint _displayListRumput;
GLuint _displayListDoubleSwing;
GLuint _displayListRoundAbout;
GLuint _displayListMonkeyLadder;
GLuint _displayListSeeSaw;

Terrain* _terrainBeruang;
Terrain* _terrainAir;
Terrain* _terrainJalan;
Terrain* _terrainGunung;

void cleanup() {
    delete _terrainBeruang;
    delete _terrainAir;
    delete _terrainJalan;
    delete _terrainGunung;
}
void putarkanan() {
    _angle+=30;
    glutPostRedisplay();
}
static void handleKeypress(unsigned char key, int x, int y) {


if (key == 'c' || key == 'C') {
        putarkanan();
    }
    if (key == 27) {
        exit(0);
    }
}
static void kibor(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_HOME:
		viewy++;
		break;
	case GLUT_KEY_END:
		viewy--;
		break;
	case GLUT_KEY_UP:
		viewz--;
		break;
	case GLUT_KEY_DOWN:
		viewz++;
		break;

	case GLUT_KEY_RIGHT:
		viewx++;
		break;
	case GLUT_KEY_LEFT:
		viewx--;
		break;

	
	default:
		break;
	}
}
//Makes the image into a texture, and returns the id of the texture

GLuint loadTexture(Image* image) {
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D,
            0,
            GL_RGB,
            image->width, image->height,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            image->pixels);
    return textureId;
}

void rumput(GLuint textureId){
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_POLYGON);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-40.0, -40.0, 0.0);
    glTexCoord2f(4.0f, 0.0f);
    glVertex3f(40.0, -40.0, 0.0);
    glTexCoord2f(4.0f, 4.0f);
    glVertex3f(40.0, 40.0, 0.0);
    glTexCoord2f(0.0f, 4.0f);
    glVertex3f(-40.0, 40.0, 0.0);

    glEnd();
    
    
}

void initRendering() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    //glEnable(GL_LIGHT3);
    //glEnable(GL_LIGHT4);
    //glEnable(GL_LIGHT5);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    
    
    Image* image = loadBMP("images\\rumput.bmp");
    _textureId = loadTexture(image);
    Image* image1 = loadBMP("images\\awan1.bmp");
    _textureId1 = loadTexture(image1);
    
    
    _displayListGazibu = glGenLists(1);
    glNewList(_displayListGazibu, GL_COMPILE);
    gazibu();
    glEndList();
    
    _displayListBouncy= glGenLists(1);
    glNewList(_displayListBouncy, GL_COMPILE);
    bouncyCastle();
    glEndList();
    
    _displayListRumput= glGenLists(1);
    glNewList(_displayListRumput, GL_COMPILE);
    rumput(_textureId);
    glEndList();
    
    _displayListDoubleSwing= glGenLists(1);
    glNewList(_displayListDoubleSwing, GL_COMPILE);
    doubleSwing();
    glEndList();
    
    _displayListRoundAbout= glGenLists(1);
    glNewList(_displayListRoundAbout, GL_COMPILE);
    roundAbout();
    glEndList();
    
    _displayListMonkeyLadder= glGenLists(1);
    glNewList(_displayListMonkeyLadder, GL_COMPILE);
    monkeyLadder();
    glEndList();
    
    _displayListSeeSaw= glGenLists(1);
    glNewList(_displayListSeeSaw, GL_COMPILE);
    Seesaw();
    glEndList();
    
    GLfloat ambientLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

    GLfloat lightColor[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat lightPos[] = {0, 0, 200, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    
    GLfloat lightColor1[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat lightPos1[] = {0, 0, -200, 1.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    
    GLfloat lightColor2[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat lightPos2[] = {0, 200, 0, 1.0f};
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightColor2);
    glLightfv(GL_LIGHT2, GL_POSITION, lightPos2);
    
    GLfloat lightColor3[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat lightPos3[] = {0, -200, 0, 1.0f};
    glLightfv(GL_LIGHT3, GL_DIFFUSE, lightColor3);
    glLightfv(GL_LIGHT3, GL_POSITION, lightPos3);
    
    GLfloat lightColor4[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat lightPos4[] = {-200, 0, 0, 1.0f};
    glLightfv(GL_LIGHT4, GL_DIFFUSE, lightColor4);
    glLightfv(GL_LIGHT4, GL_POSITION, lightPos4);
    
    GLfloat lightColor5[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat lightPos5[] = {200, 0, 0, 1.0f};
    glLightfv(GL_LIGHT5, GL_DIFFUSE, lightColor5);
    glLightfv(GL_LIGHT5, GL_POSITION, lightPos5);
    //glEnable(GL_BLEND); //Enable alpha blending
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set the blend function

    
   
}

void handleResize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float) w / (float) h, 1.0, 200.0);
}

void drawScene() {
    glClearColor(0.0, 0.6, 0.8, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(viewx, viewy, viewz, 0.0, 0.0, -100.0, 0.0, 1.0, 0.0);
    glTranslatef(0.0f, 0.0f, -40.0f);

    
    
    
    
    
    glRotatef(-70, 1.0f, 0.0f, 0.0f);
    glRotatef(-_angle, 0.0f, 0.0f, 1.0f);
    glScalef(0.4,0.4,0.4);

    glEnable(GL_TEXTURE_2D);
    
    
    glCallList(_displayListRumput);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
    glEnable(GL_TEXTURE_GEN_T);
    glPushMatrix();
    glTranslatef(0,0,-16);
    glRotatef(90, 1, 0, 0);
    glScalef(3.8,1,3.8);
    terrain(_terrainGunung, 0.3f, 0.5f, 0.2f);//0.3f, 0.5f, 0.2f
    glPopMatrix();
    glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
    glDisable(GL_TEXTURE_GEN_T);
    //glutSolidCube(33);
    //glutSolidDodecahedron();
    glDisable(GL_TEXTURE_2D);
    
    glPushMatrix();
    glScalef(2,2,2);
    glRotatef(90, 1, 0, 0);
    glTranslatef(15.0,1.5,10.0);
    glCallList(_displayListGazibu);
    glPopMatrix();
    
    glPushMatrix();
    glScalef(0.5,0.5,0.5);
    glRotatef(90, 1, 0, 0);
    glRotatef(100, 0, 1, 0);
    glTranslatef(0.0,1.5,8.0);
    glCallList(_displayListBouncy);
    glPopMatrix();

    ///*
    //terrain beruang
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    terrain(_terrainBeruang, 0.494f, 0.494f, 0.494f);
    glPopMatrix();
//terrain air
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    terrain(_terrainAir, 0.5f, 0.71f, 0.757f);
    glPopMatrix();
    
    glPushMatrix();
    glScalef(0.5,0.5,0.5);
    glRotatef(90, 1, 0, 0);
    glRotatef(180, 0, 1, 0);
    glTranslatef(-50,1.5,0);
    glCallList(_displayListDoubleSwing);
    glPopMatrix();
    
    glPushMatrix();
    glScalef(0.5,0.5,0.5);
    glRotatef(90, 1, 0, 0);
    glRotatef(100, 0, 1, 0);
    glTranslatef(-60,1.5,-10);
    glCallList(_displayListRoundAbout);
    glPopMatrix();
    
    glPushMatrix();
    glScalef(0.5,0.5,0.5);
    glRotatef(90, 1, 0, 0);
    glRotatef(100, 0, 1, 0);
    glTranslatef(-60,1.5,10);
    glColor3f(0,0,1);
    glLineWidth(2);
    glutWireSphere(10,10,10);
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1,0.1,0.1);
    glRotatef(90, 1, 0, 0);
    glRotatef(100, 0, 1, 0);
    glTranslatef(80,20,-180);
    glCallList(_displayListMonkeyLadder);
    glPopMatrix();
    
    glPushMatrix();
    glScalef(0.5,0.5,0.5);
    glRotatef(90, 1, 0, 0);
    glRotatef(100, 0, 1, 0);
    glTranslatef(20,1.5,-60);
    glCallList(_displayListSeeSaw);
    glPopMatrix();
    
    glPushMatrix();
    glScalef(0.5,0.5,0.5);
    glRotatef(90, 1, 0, 0);
    glRotatef(100, 0, 1, 0);
    glTranslatef(20,1.5,-60);
    glCallList(_displayListSeeSaw);
    glPopMatrix();
    
    //kursi1
glPushMatrix();
glTranslatef(0,5,-15); 
glScalef(50, 50, 50);
//glBindTexture(GL_TEXTURE_2D, texture[0]);
kursi();
glPopMatrix();


//kursi2
glPushMatrix();
glTranslatef(-50,5,-15); 
glScalef(5, 5, 5);
//glBindTexture(GL_TEXTURE_2D, texture[0]);
kursi();
glPopMatrix();
    /*
    glPushMatrix();
    glTranslatef(0,0,-16);
    glRotatef(90, 1, 0, 0);
    glScalef(3.8,1,3.8);
    terrain(_terrainGunung, 0.3f, 0.5f, 0.2f);
    glPopMatrix();
    */
    //*/
    glutSwapBuffers();
}

//Called every 25 milliseconds

void update(int value) {
    _angle += 1.0f;
    if (_angle > 360) {
        _angle -= 360;
    }
    glutPostRedisplay();
    glutTimerFunc(25, update, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);

    glutCreateWindow("Taman Bermain");
    initRendering();
    _terrainBeruang = loadTerrain("images\\beruang.bmp", 0.7);
    _terrainAir = loadTerrain("images\\kolam.bmp", 0.1);
    _terrainJalan = loadTerrain("images\\jalan.bmp", 1.0);
    _terrainGunung = loadTerrain("images\\gunung.bmp", 45.0);
    glutDisplayFunc(drawScene);
    glutKeyboardFunc(handleKeypress);
    glutReshapeFunc(handleResize);
    //glutTimerFunc(25, update, 0);
    glutSpecialFunc(kibor);
    glutIdleFunc(drawScene);
    glutMainLoop();
    return 0;
}
