#ifndef JOINT_H
#define JOINT_H

#include "color.h"
#include <GL/glut.h>
#include <vector>

class Limb {
public:
    Limb(glColor c)
    {
        mObj = gluNewQuadric();
        gluQuadricDrawStyle(mObj, GLU_FILL);
        gluQuadricOrientation(mObj, GLU_OUTSIDE);
        gluQuadricNormals(mObj, GLU_SMOOTH);

        color = c;
    }

    float length{};
    float angle{};
    GLUquadricObj* mObj;
    glColor color{};
};

#endif // JOINT_H
