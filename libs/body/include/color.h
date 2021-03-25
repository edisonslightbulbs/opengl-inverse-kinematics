#ifndef COLOR_H
#define COLOR_H

#include <GL/gl.h>

struct glColor {
    float r;
    float g;
    float b;
    float a;

    inline void apply() const { glColor4f(r, g, b, a); }
};

#endif // COLOR_H
