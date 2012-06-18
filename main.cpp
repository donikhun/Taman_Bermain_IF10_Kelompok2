#include <iostream>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "imageloader.h"
#include "vec3f.h"

using namespace std;

static float _angle = 0.0;
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
Terrain* _terrainBeruang;
Terrain* _terrainAir;
Terrain* _terrainJalan;

void cleanup() {
    delete _terrainBeruang;
    delete _terrainAir;
    delete _terrainJalan;
}
void putarkanan() {
    _angle+=30;
    glutPostRedisplay();
}
void handleKeypress(unsigned char key, int x, int y) {

    if (key == 'c' || key == 'C') {
        putarkanan();
    }

    if (key == 27) {
        exit(0);
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

void initRendering() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
    glEnable(GL_LIGHT4);
    glEnable(GL_LIGHT5);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    //glEnable(GL_BLEND); //Enable alpha blending
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set the blend function

    Image* image = loadBMP("images\\rumput.bmp");
    _textureId = loadTexture(image);
    delete image;
}

void handleResize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float) w / (float) h, 1.0, 200.0);
}

void drawScene() {
    glClearColor(1.0, 1.0, 1.0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -100.0f);

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
    
    glRotatef(-40, 1.0f, 0.0f, 0.0f);
    glRotatef(-_angle, 0.0f, 0.0f, 1.0f);


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId);
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
    //glutSolidCube(33);
    //glutSolidDodecahedron();
    glDisable(GL_TEXTURE_2D);
    
    glPushMatrix();
    glScalef(2,2,2);
    glRotatef(90, 1, 0, 0);
    glTranslatef(15.0,1.5,10.0);
     //lantai
    gazibu();
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

    //terrain jalan
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    terrain(_terrainJalan, 0.247f, 0.247f, 0.247f);
    glPopMatrix();
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
    _terrainBeruang = loadTerrain("images\\beruang.bmp", 1.0);
    _terrainAir = loadTerrain("images\\kolam.bmp", 0.1);
    _terrainJalan = loadTerrain("images\\jalan.bmp", 1.0);

    glutDisplayFunc(drawScene);
    glutKeyboardFunc(handleKeypress);
    glutReshapeFunc(handleResize);
    //glutTimerFunc(25, update, 0);

    glutMainLoop();
    return 0;
}


