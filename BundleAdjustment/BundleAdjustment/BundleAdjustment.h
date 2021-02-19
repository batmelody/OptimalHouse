#pragma once
#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <vector>
#include <fstream>
#include <iomanip>
#include<fstream>
#include<stdlib.h>
#include<vector>
#include<string>
#include "se3.hpp"
#include "so3.hpp"

using namespace std;
using namespace Eigen;
typedef vector<Vector3d, Eigen::aligned_allocator<Vector3d>> VecVector3d;

void BA_R(VecVector3d p3d, VecVector3d p3dd);
void BA_R_T(VecVector3d p3d, VecVector3d p3dd);
VecVector3d BA_Read(string str_Path);


