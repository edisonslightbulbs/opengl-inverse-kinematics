#include "arm.h"
#include <cmath>
#include <iostream>
#include "link.h"

#define degrees(x) x*180/3.1415926


MatrixXf Arm::pseudoInverse()
{
	// computing the pseudo-inverse
    MatrixXf j = jacobian();
    MatrixXf jjtInv = (j * j.transpose());
    jjtInv = jjtInv.inverse();
    return (j.transpose() * jjtInv);
}

void Arm::calculatePosition()
{
    float angle = 0;
    mPosition = VectorXf::Zero(2,1);

    // computing new position similar to jacobian construction
    for (int i = 0; i < mList.size(); i++){
        angle += mList[i]->mAngle;
        mPosition(0) += mList[i]->mLength*cos(angle);
        mPosition(1) += -mList[i]->mLength*sin(angle);
    }
}

MatrixXf Arm::jacobian()
{
  // constructing the jacobian for the linked structure
    MatrixXf j = MatrixXf::Zero(2,mList.size());

    for (int col = 0; col < mList.size(); col++){
        float angle = 0;
        for (int i = 0; i < mList.size(); i++){
            angle += mList[i]->mAngle;
            if (i >= col){
                j(0, col) += (-mList[i]->mLength*sin(angle));
                j(1, col) += (mList[i]->mLength*cos(angle));
            }
        }
    }
    return j;
}

void Arm::moveToPoint(const VectorXf position)
{
  // specifying the new position and indicate that an update is now true
  mTargetPosition = position;
  mResolveTarget = true;
}

/* n.b. :: 
 * OpenGL keeps a stack of matrices to quickly apply and remove transformations. 
 * 
 * - glPushMatrix : copies the top matrix and pushes it onto the stack,
 * - glPopMatrix  : pops the top matrix off the stack. 
 *
 * All transformation functions (glScaled, etc.) function on the top matrix, and the top matrix 
 * is what all rendering commands use to transform their data. By pushing and popping matrices, one 
 * can control what transformations apply to which objects, as well as apply transformations to groups 
 * of objects, and easily reverse the transformations so that they don't affect other objects.
 */

void Arm::draw() // drawing the arm position   << Review as at 28. August 2018
{
    glPushMatrix();  // Creates matrix 1 on the top
    glTranslatef(mBasePosition(0),mBasePosition(1),0.0f);  // Applies translation to matrix 1
    glutSolidSphere(2.0f, 20, 20);  // Draws a sphere with translation <2.0f, 20, 20>

    // represent the joints by a sphere (on the tips of every arm-structure)
    for (int i = 0; i < mList.size(); i++){
        glRotatef(degrees(mList[i]->mAngle), 1.0f, 0.0f, 0.0f);
		mList[i]->mColor.apply();
        gluCylinder(mList[i]->mObj, 1, 1, mList[i]->mLength, 20, 20);
        glTranslatef(0, 0, mList[i]->mLength);
		Color c = { 1.0, 0, 0};
		c.apply();
	
		// drawing the sphere-joints
        glutSolidSphere(1.5, 20, 20);
    }
    glPopMatrix();  // Deletes matrix 1
}

void Arm::moveBy(float dx, float dy)
{
	// Here we move by dx an dy the linked structure's tip
    VectorXf dAngles = VectorXf::Zero(mList.size(), 1);
    VectorXf dPosition = VectorXf::Zero(2, 1);

	dPosition(0) = dx;
	dPosition(1) = dy;
  
    dAngles = pseudoInverse() * dPosition;
    
    // Adding the difference required to move to the new position to the angles
    for (int i = 0; i < mList.size(); i++){
        mList[i]->mAngle += dAngles(i);
	}
    calculatePosition();
}

void Arm::update()
{
  // updating the arm-structure
  if(isTargetResolved()) return;
  
   float x = (mTargetPosition(0) - mPosition(0) > 1) ? mStep : -mStep;
   float y = (mTargetPosition(1) + mPosition(1) > 1) ? mStep : -mStep;

  moveBy(x, y);
}

bool Arm::isTargetResolved()
{
  // verify that the current possition is the target position
  if(fabs(mPosition(0) - mTargetPosition(0)) <= 2.0f && fabs(mPosition(1) + mTargetPosition(1)) <= 2.0f){
      mResolveTarget = false;
      return true;
  }
  return false;
}

VectorXf Arm::getPointWithinRange()
{
  // Returning a random point within the range that the linkedstructure
  // can reach. This method is just for demo purposes
   VectorXf point = VectorXf::Zero(2, 1);
   float length = 0;
   for(int i = 0; i < mList.size(); i++){
     length += mList[i]->mLength;
   }
   
   point(0) = ((rand() % 2 == 0) ? -1 : 1) * (rand() % (int)(length*0.7));
   point(1) = ((rand() % 2 == 0) ? -1 : 1) * (rand() % (int)(length*0.7));
   
   return point;
}
