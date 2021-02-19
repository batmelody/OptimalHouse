#include <Eigen/Core>
#include <Eigen/Dense>
#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "se3.hpp"
#include "so3.hpp"
#include<fstream>
#include<stdlib.h>
#include<vector>
#include<string>
#include"BundleAdjustment.h"

using namespace std;
using namespace Eigen;
typedef vector<Vector3d, Eigen::aligned_allocator<Vector3d>> VecVector3d;





int main(int argc, char **argv) {
	VecVector3d p3d;
	VecVector3d p3dd;
	Vector3d phi(1, 1, 1);
	Matrix<double, 3, 3> A = Matrix<double, 3, 3>::Zero();

	Matrix<double, 6, 1>Phi = Matrix<double, 6, 1>::Zero();
	Phi(3, 0) = 1;
	Phi(4, 0) = 1;
	Phi(5, 0) = 1;
	Sophus::SE3d t_gt_ = Sophus::SE3d::exp(Phi);


	p3d = BA_Read("C:/Users/jd/Desktop/1.txt");
	p3dd = BA_Read("C:/Users/jd/Desktop/2.txt");


	//for (int i = 0; i < 100; i++)
	//{
	//	Vector3d P1(rand(), rand(), rand());
	//	Vector3d P2 = t_gt_ * P1;
	//	p3d.push_back(P1);
	//	p3dd.push_back(P2);
	//}

	BA_R(p3d, p3dd);

	return 0;
}

