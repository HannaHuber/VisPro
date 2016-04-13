#include "vtkCamera.h"
#include <math.h>
#include <complex>
#include "ChamferTransform.h"

ChamferTransform::ChamferTransform(vtkRenderer *r, float phi)
: ren(r)
, tanPhi(tan(phi))
{
	// TODO: calculate angle m
	double* range = ren->GetActiveCamera()->GetClippingRange();
	PMsz = (range[0] + range[1]) / (range[0] - range[1]);

	// Iterate over all pixels p
	for (unsigned int px = 0; px < N; px++)
	{
		for (unsigned int py = 0; py < N; py++)
		{
			// Initialize with depth value
			maxC[px][py] = ren->GetZ(px, py);

			// Iterate over all pixels q
			for (unsigned int i = 1; i <= log2(N); i++)
			{
				// Calculate q values for this iteration
				fillQ(i);

				// Calculate C(p,q)
				for (unsigned int j = 0; j < 9; j++)
				{
					float C = calculateC(px, py, q[j][0], q[j][1]);

					// Store maximum
					if (maxC[px][py] < C)
						maxC[px][py] = C;
				}
			}

			//cout << maxC[px][py] << endl;
		}
	}

}

void ChamferTransform::fillQ(unsigned int i)
{
	// Calculate offset
	unsigned k = N / pow(2, i);

	// Fill array of 9 candidate points
	q[0][0] = k;
	q[0][1] = k;
	q[1][0] = 0;
	q[1][1] = 0;
	q[2][0] = -k;
	q[2][1] = -k;
	q[3][0] = k;
	q[3][1] = 0;
	q[4][0] = k;
	q[4][1] = -k;
	q[5][0] = 0;
	q[5][1] = k;
	q[6][0] = 0;
	q[6][1] = -k;
	q[7][0] = -k;
	q[7][1] = k;
	q[8][0] = -k;
	q[8][1] = 0;

}

float ChamferTransform::calculateC(unsigned int px, unsigned int py, unsigned int qx, unsigned int qy)
{
	float z = calculateDepth(ren->GetZ(px, py));
	float pqNorm = std::norm(std::complex<float>(px - qx, py - qx));
	float m = -(PMsz + z) / tanPhi;
	return  (z - m*pqNorm);
}

float ChamferTransform::calculateDepth(float clipZ)
{
	double near, far;
	ren->GetActiveCamera()->GetClippingRange(near, far);
	return (near + clipZ * (far - near));
}