#include "arm.h"
#include <cmath>
#include <iostream>
#include "limb.h"

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
    currentPosition = VectorXf::Zero(2,1);

    // computing new position similar to jacobian construction
    for (int i = 0; i < limbs.size(); i++){
        angle += limbs[i]->angle;
        currentPosition(0) += limbs[i]->length*cos(angle);
        currentPosition(1) += -limbs[i]->length*sin(angle);
    }
}

MatrixXf Arm::jacobian()
{
  // constructing the jacobian for the linked structure
    MatrixXf j = MatrixXf::Zero(2,limbs.size());

    for (int col = 0; col < limbs.size(); col++){
        float angle = 0;
        for (int i = 0; i < limbs.size(); i++){
            angle += limbs[i]->angle;
            if (i >= col){
                j(0, col) += (-limbs[i]->length*sin(angle));
                j(1, col) += (limbs[i]->length*cos(angle));
            }
        }
    }
    return j;
}

void Arm::moveToPoint(const VectorXf position)
{
  // specifying the new position and indicate that an update is now true
  targetPosition = position;
  resolveTarget = true;
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

void Arm::draw() 
{
    glPushMatrix();  // Creates matrix 1 on the top
    glTranslatef(armBase(0),armBase(1),0.0f);  // Applies translation to matrix 1
	glutSolidSphere(3.0f, 8, 8);  // base | root joint

    // represent the joints by a sphere (on the tips of every arm-structure)
    for (int i = 0; i < limbs.size(); i++){
        glRotatef(degrees(limbs[i]->angle), 1.0f, 0.0f, 0.0f);
		limbs[i]->color.apply();
        gluCylinder(limbs[i]->mObj, 1, 1, limbs[i]->length, 20, 20);
        glTranslatef(0, 0, limbs[i]->length);
		glColor color = {0.5f, 0.5f, 0.5f};
		color.apply();
	
		// drawing the sphere-joints
        glutSolidSphere(1.5, 20, 20);
    }
    glPopMatrix();  // Deletes matrix 1
}

void Arm::moveBy(float dx, float dy)
{
	// Here we move by dx an dy the linked structure's tip
    VectorXf angleDifferential = VectorXf::Zero(limbs.size(), 1);
    VectorXf positionDifferential = VectorXf::Zero(2, 1);

	positionDifferential(0) = dx;
	positionDifferential(1) = dy;
  
    angleDifferential = pseudoInverse() * positionDifferential;
    
    // Adding the difference required to move to the new position to the angles
    for (int i = 0; i < limbs.size(); i++){
        limbs[i]->angle += angleDifferential(i);
	}
    calculatePosition();
}

void Arm::update()
{
  if(isTargetResolved()) return;
  
  float x = (targetPosition(0) - currentPosition(0) > 1) ? effectorStep : -effectorStep;
  float y = (targetPosition(1) + currentPosition(1) > 1) ? effectorStep : -effectorStep;

  moveBy(x, y);
}

bool Arm::isTargetResolved()
{
  if(fabs(currentPosition(0) - targetPosition(0)) <= 2.0f && fabs(currentPosition(1) + targetPosition(1)) <= 2.0f){
      resolveTarget = false;
      return true;
  }
  return false;
}

VectorXf Arm::getPointWithinRange()
{
  // Returning a random point within range of the arm
   VectorXf point = VectorXf::Zero(2, 1);
   float length = 0;
   for(int i = 0; i < limbs.size(); i++){
     length += limbs[i]->length;
   }

   point(0) = ((rand() % 2 == 0) ? -1 : 1) * (rand() % (int)(length*0.7));
   point(1) = ((rand() % 2 == 0) ? -1 : 1) * (rand() % (int)(length*0.7));
   
   return point;
}
