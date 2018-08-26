#ifndef LINKEDSTRUCTURE_H
#define LINKEDSTRUCTURE_H

#include "color.h"
#include <vector>
#include <Eigen/Dense>


using namespace Eigen;

class Link;


class Arm
{
private:

    std::vector<Link*> mList;
    VectorXf mBasePosition;
    VectorXf mPosition;
    VectorXf mTargetPosition;
    float mStep;
    bool mResolveTarget;
    Color mColor;
    
    void calculatePosition();
    MatrixXf jacobian();
    MatrixXf pseudoInverse();

    
public:

    Arm()
	{
		mBasePosition = VectorXf::Zero(2, 1);
		mPosition = VectorXf::Zero(2, 1);
		mTargetPosition = VectorXf::Zero(2, 1);
		mStep = 0.5f;
		mResolveTarget = false;
    }

    void draw();
    void moveToPoint(const VectorXf position);
    void moveBy(float dx, float dy);
    void update();
    bool isTargetResolved();
    VectorXf getPointWithinRange();
    
    inline void setStep(float step) { step = mStep; }
    inline void addLink(Link *link) { mList.push_back(link); calculatePosition();}
    inline VectorXf getPosition() { return mPosition; }
};

#endif // LINKEDSTRUCTURE_H
