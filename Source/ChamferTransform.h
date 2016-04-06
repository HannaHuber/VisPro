#ifndef CHAMFERTRANSFORM_H
#define CHAMFERTRANSFORM_H

#include "vtkRenderer.h"
#define N 300

class ChamferTransform{

public:
	ChamferTransform(vtkRenderer *r);
	
private:
	vtkRenderer *ren;
	float m;
	unsigned int q[9][2];
	float maxC[N][N];
	void fillQ(unsigned int i);
	float calculateC(unsigned int px, unsigned int py, unsigned int qx, unsigned int qy);
};

#endif
