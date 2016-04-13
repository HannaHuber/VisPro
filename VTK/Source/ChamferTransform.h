#ifndef CHAMFERTRANSFORM_H
#define CHAMFERTRANSFORM_H

#include "vtkRenderer.h"
#define N 300

class ChamferTransform{

public:
	ChamferTransform(vtkRenderer *r, float phi);
	
private:
	vtkRenderer *ren;
	float tanPhi;
	float PMsz;
	unsigned int q[9][2];
	float maxC[N][N];
	void fillQ(unsigned int i);
	float calculateC(unsigned int px, unsigned int py, unsigned int qx, unsigned int qy);
	float calculateDepth(float clipZ);
};

#endif
