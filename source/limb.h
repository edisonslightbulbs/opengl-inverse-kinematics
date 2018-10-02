#ifndef JOINT_H
#define JOINT_H

#ifdef WIN32
 #define NOMINMAX
 #include <Windows.h>
#endif

#include <GL/glut.h>
#include <vector>
#include "color.h"

class Limb
{
public:
  
    Limb(glColor c)
    {
        mObj = gluNewQuadric();
        gluQuadricDrawStyle(mObj, GLU_FILL);
        gluQuadricOrientation(mObj, GLU_OUTSIDE);
        gluQuadricNormals(mObj, GLU_SMOOTH);
	
	color = c;
    }
    
    float length;
    float angle;
    GLUquadricObj *mObj;
    glColor color;
};

#endif // JOINT_H
