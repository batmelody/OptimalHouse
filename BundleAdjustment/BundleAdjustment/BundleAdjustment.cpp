#include <Eigen/Core>
#include <Eigen/Dense>
#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include<fstream>
#include<stdlib.h>
#include<vector>
#include<string>
#include "se3.hpp"
#include "so3.hpp"
#include "BundleAdjustment.h"

using namespace std;
using namespace Eigen;
typedef vector<Vector3d, Eigen::aligned_allocator<Vector3d>> VecVector3d;

void BA_R(VecVector3d p3d, VecVector3d p3dd)
{
	int iterations = 30;
	double cost = 0, lastCost = 0;
	int nPoints = p3d.size();
	Sophus::SO3d T_esti; //

	for (int iter = 0; iter < iterations; iter++) {
		Matrix<double, 3, 3> H = Matrix<double, 3, 3>::Zero();
		Vector3d b = Vector3d::Zero();

		cost = 0;
		for (int i = 0; i < nPoints; i++) {
			Vector3d p1 = p3d[i];
			Vector3d p2 = p3dd[i];
			Vector3d p_ = T_esti * p1;
			Vector3d e = p2 - p_;
			cost += (e[0] * e[0] + e[1] * e[1]);

			Matrix<double, 3, 3> J;
			J = Sophus::SO3d::hat(p_);
			H += J.transpose() * J;
			b += -J.transpose() * e;
		}

		Vector3d dx;
		dx = H.ldlt().solve(b);

		if (isnan(dx[0])) {
			cout << "result is nan!" << endl;
			break;
		}

		if (iter > 0 && cost >= lastCost) {
			cout << "cost: " << cost << ", last cost: " << lastCost << endl;
			break;
		}

		T_esti = Sophus::SO3d::exp(dx)*T_esti;

		lastCost = cost;

		cout << "iteration " << iter << " cost=" << cout.precision(12) << cost << endl;
	}

	cout << "estimated pose by BA: \n" << T_esti.matrix() << endl;
}


void BA_R_T(VecVector3d p3d, VecVector3d p3dd)
{
	int iterations = 100;
	double cost = 0, lastCost = 0;
	int nPoints = p3d.size();
	Sophus::SE3d T_esti;

	for (int iter = 0; iter < iterations; iter++) {
		Matrix<double, 6, 6> H = Matrix<double, 6, 6>::Zero();
		Matrix<double, 6, 1> b = Matrix<double, 6, 1>::Zero();
		cost = 0;
		for (int i = 0; i < nPoints; i++) {
			Vector3d p1 = p3d[i];
			Vector3d p2 = p3dd[i];
			Vector3d p_ = T_esti * p1;
			Vector3d e = p2 - p_;
			cost += (e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);

			Matrix<double, 3, 6> J = Matrix<double, 3, 6>::Zero();
			double x = p_[0];
			double y = p_[1];
			double z = p_[2];
			J(0, 0) = 1;
			J(0, 4) = z;
			J(0, 5) = -y;
			J(1, 1) = 1;
			J(1, 3) = -z;
			J(1, 5) = x;
			J(2, 2) = 1;
			J(2, 3) = y;
			J(2, 4) = -x;
			J = -J;
			H += J.transpose() * J;
			b += -J.transpose() * e;
		}

		Matrix<double, 6, 1> dx;
		dx = H.ldlt().solve(b);

		if (isnan(dx[0])) {
			cout << "result is nan!" << endl;
			break;
		}

		if (iter > 0 && cost >= lastCost) {
			cout << "cost: " << cost << ", last cost: " << lastCost << endl;
			break;
		}

		T_esti = Sophus::SE3d::exp(dx)*T_esti;
		lastCost = cost;

		cout << "iteration " << iter << " cost=" << cout.precision(12) << cost << endl;
	}

	cout << T_esti.matrix() << endl;

}

VecVector3d BA_Read(string str_Path)
{
	ifstream infile;
	infile.open(str_Path);
	VecVector3d Pts;
	if (!infile)
	{
		cout << "error" << endl;
		return Pts;
	}
	string str;
	double t1, t2, t3;
	Vector3d Pt;
	while (infile >> t1 >> t2 >> t3)
	{
		Pt(0, 0) = t1;
		Pt(1, 0) = t2;
		Pt(2, 0) = t3;
		Pts.push_back(Pt);
	}
	return Pts;
}