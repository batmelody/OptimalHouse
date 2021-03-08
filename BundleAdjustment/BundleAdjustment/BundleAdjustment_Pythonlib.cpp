#include <Eigen/Core>
#include <Eigen/Dense>
#include <Python.h> 
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

using namespace std;
using namespace Eigen;
typedef vector<Vector3d, Eigen::aligned_allocator<Vector3d>> VecVector3d;


Matrix<double, 3, 3> BA_R_ONLY(VecVector3d p3d, VecVector3d p3dd)
{
	int iterations = 30;
	double cost = 0, lastCost = 0;
	int nPoints = p3d.size();
	cout << "nPoints:" << nPoints << endl;
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
	Matrix<double, 3, 3> res_M = T_esti.matrix();
	return res_M;
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


void BA_writte(string str_Path, Matrix<double,3,3> M)
{
	ofstream outfile;      //终端输入--》内存--》文本
	outfile.open(str_Path);//(输入流) （变量）（输出文件流）
	if (!outfile) cout << "error" << endl;

	string str;
	int i = 0;
	for (int r = 0; r < 3; r++)
	{
		for (int c = 0; c < 3; c++)
		{
			double data = M(r, c);    //读取数据，at<type> - type 是矩阵元素的具体数据格式
			outfile << data << " ";    //每列数据用 tab 隔开
		}
		outfile << endl;    //换行
	}

	outfile.close();
}




PyObject *
dataC2P(Matrix<double, 3, 3> * c_data_point)
{
	int argSize = c_data_point->size();
	printf("c++ data size:%d\n", argSize);
	PyObject *py_data = PyList_New(argSize);
	for (int i = 0; i < 3; ++i) {
		PyObject *pObj = PyList_New(3);
		for (int j = 0; j < 3; ++j) {
			PyList_SetItem(pObj, j, Py_BuildValue("d", (*c_data_point)(i,j)));
		}
		PyList_SetItem(py_data, i, Py_BuildValue("O", pObj));
	}
	cout << "finishC2P" << endl;
	return py_data;
}

typedef struct data {
	VecVector3d c_data;
	VecVector3d c_data_;
}Function_Struct;


Function_Struct dataP2C(PyObject *py_data)
{
	Function_Struct cpp_data;
	int row = PyList_Size(py_data);
	int gap = row / 2;
	for (int i = 0; i < row; i++)
	{	
		Vector3d Pt;
		PyObject *pItem = PyList_GetItem(py_data, i);
		for (int j = 0; j < 3; j++)
		{
			PyObject *pItem_value = PyList_GetItem(pItem, j);
			double value = PyFloat_AsDouble(pItem_value);
			Pt(j, 0) = value;
		}

		if (i < gap)
		{
			cpp_data.c_data.push_back(Pt);

		}
		else
		{
			cpp_data.c_data_.push_back(Pt);
		}

	}
	return cpp_data;
	cout << "this function complited!" << endl;
	cout << "nPoints :" << cpp_data.c_data.size() << endl;
}




static PyObject * BA_Python(PyObject* self, PyObject* args)
{	
	cout << "at this step" << endl;
	
	PyObject *py_data; //the first parameters

	if (!PyArg_ParseTuple(args, "O", &py_data)) // 获取到python文件中的输入参数（python 2 c++）
	{
		return NULL;
	}
	Function_Struct cpp_data;
	cpp_data = dataP2C(py_data);
	cout << "nPoints output:" << cpp_data.c_data.size() << endl;
	cout << "nPoints output_:" << cpp_data.c_data_[0] << endl;

	Matrix<double, 3, 3> M = BA_R_ONLY(cpp_data.c_data, cpp_data.c_data_);
	cout << "finish the optimization" << endl;
	PyObject* tmp = dataC2P(&M); // c++ 2 python
	return Py_BuildValue("O", tmp);  // （c++ 2 python）
}



static PyMethodDef BAPython_methods[] = {
	{ "BA", (PyCFunction)BA_Python, METH_VARARGS, "MyfirstFunction" },
	{ NULL, NULL , 0, NULL }
};

static struct PyModuleDef PCmodule = {
	PyModuleDef_HEAD_INIT,
	"Py2Cpp",   /* name of module */
	NULL, /* module documentation, may be NULL */
	-1,       /* size of per-interpreter state of the module,
			  or -1 if the module keeps state in global variables. */
	BAPython_methods
};

PyMODINIT_FUNC
PyInit_Py2Cpp(void)
{
	return PyModule_Create(&PCmodule);
}

