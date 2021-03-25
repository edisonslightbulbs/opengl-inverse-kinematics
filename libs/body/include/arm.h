#ifndef ARM_H
#define ARM_H

#include <Eigen/Dense>
#include "color.h"
#include <vector>

class Limb;
using namespace Eigen;

class Arm {
private:
    glColor color{};
    VectorXf armBase;
    std::vector<Limb*> limbs;

    VectorXf targetPosition;
    VectorXf currentPosition;

    float effectorStep;
    bool resolveTarget; // todo: resolve or resolved?

    MatrixXf jacobian();
    MatrixXf pseudoInverse();
    void calculatePosition();

public:
    Arm()
    {
        effectorStep = 0.5f;
        resolveTarget = false;
        armBase = VectorXf::Zero(2, 1);
        currentPosition = VectorXf::Zero(2, 1);
        targetPosition = VectorXf::Zero(2, 1);
    }

    void draw();
    void update();
    bool isTargetResolved();
    VectorXf getPointWithinRange();
    void moveBy(float dx, float dy);
    void moveToPoint(const VectorXf& position);

    inline void setStep(float step) { step = effectorStep; }
    inline VectorXf getPosition() { return currentPosition; }

    inline void addLimb(Limb* link)
    {
        limbs.push_back(link);
        calculatePosition();
    }
};

#endif // ARM_H
