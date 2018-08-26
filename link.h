#ifndef LINK_H
#define LINK_H

#ifdef WIN32
 #define NOMINMAX
 #include <Windows.h>
#endif

#include <GL/glut.h>
#include <vector>
#include "color.h"

class Link
{
public:
  
    Link(Color c)
    {
        mObj = gluNewQuadric();
        gluQuadricDrawStyle(mObj, GLU_FILL);
        gluQuadricOrientation(mObj, GLU_OUTSIDE);
        gluQuadricNormals(mObj, GLU_SMOOTH);
	
	mColor = c;
    }
    
    float mLength;
    float mAngle;
    GLUquadricObj *mObj;
    Color mColor;
};

#endif // LINK_H
