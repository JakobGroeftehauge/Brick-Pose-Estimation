#include <iostream>
#include <fstream>
#include "../../NR_LIB/code/nr3.h"
#include "../../NR_LIB/code/ludcmp.h"
#include "../../NR_LIB/code/cholesky.h"
#include "../../NR_LIB/code/svd.h"
#include "../../utilities.h"
using namespace std;

double residual(MatDoub A, VecDoub x, VecDoub b)
{
	return (A * x - b).length() / b.length();
}

void read_data(string path, VecDoub &theta1, VecDoub &theta2, VecDoub &x, VecDoub &y)
{
	ifstream d(path);
	for (int i = 0; i < theta1.size(); i++) {
		d >> theta1[i];
		d >> theta2[i];
		d >> x[i];
		d >> y[i];
	}
}

void build_design_matrix(MatDoub& A, VecDoub& theta1, VecDoub& theta2)
{
	for (int i = 0; i < theta1.size(); i = i++)
	{
		int ii = i * 2;
		A[ii][0] = 1; A[ii][1] = 0; A[ii][2] = cos(theta1[i]); A[ii][3] = cos(theta1[i] + theta2[i]);
		A[ii + 1][0] = 0; A[ii + 1][1] = 1; A[ii + 1][2] = sin(theta1[i]); A[ii + 1][3] = sin(theta1[i] + theta2[i]);
	}
}

void build_z(VecDoub& z, VecDoub& x, VecDoub& y)
{
	for (int i = 0; i < x.size(); i = i++)
	{
		int ii = i * 2;
		z[ii] = x[i];
		z[ii + 1] = y[i];
	}
}

int main() {
	//------------------------------- READ THE DATA -----------------------------------
	
	VecDoub theta1_d1(500), theta2_d1(500), x_d1(500), y_d1(500);
	VecDoub theta1_d2(500), theta2_d2(500), x_d2(500), y_d2(500);
	read_data("../../Data/d1", theta1_d1, theta2_d1, x_d1, y_d1);
	read_data("../../Data/d2", theta1_d2, theta2_d2, x_d2, y_d2);
	util::print(y_d1);

	//*********************************************************************************

	//------------------------- CONTRUCT DESIGN MATRIX --------------------------------

	VecDoub q_d1(4), q_d2(4), z_d1(2 * theta1_d1.size(),0.0), z_d2(2 * theta1_d2.size(),0.0);
	MatDoub A_d1(2 * theta1_d1.size(), q_d1.size(),0.0), A_d2(2 * theta1_d1.size(), q_d2.size(),0.0);
	build_design_matrix(A_d1, theta1_d1, theta2_d1);
	build_design_matrix(A_d2, theta1_d2, theta2_d2);
	build_z(z_d1, x_d1, y_d1);
	build_z(z_d2, x_d2, y_d2);
	//cout << "A_d1: " << endl;
	//util::print(A_d1);
	//cout << "z_d1: " << endl;
	//util::print(z_d1);

	//**********************************************************************************

	//---------------------------------- SOLVE -----------------------------------------

	SVD svd_d1(A_d1);
	SVD svd_d2(A_d2);

	cout << "d1 W: " << endl;
	util::print(svd_d1.w);
	cout << "d1 inverse condition: " << svd_d1.inv_condition() << endl;

	cout << "d2 W: " << endl;
	util::print(svd_d2.w);
	cout << "d2 inverse condition: " << svd_d2.inv_condition() << endl;

	svd_d1.solve(z_d1, q_d1);
	cout << "d1 solution: " << endl;
	util::print(q_d1);
	cout << "d1 residual error: " << residual(A_d1, q_d1, z_d1) << endl;

	svd_d2.solve(z_d2, q_d2);
	cout << "d2 solution: " << endl;
	util::print(q_d2);
	cout << "d2 residual error: " << residual(A_d2, q_d2, z_d2) << endl;

	//*****************************************************************************



	//MatDoub A_Filip(82, 11);
	//for (int i = 0; i < A_Filip.nrows(); i++)
	//{
	//	for (int j = 0; j < A_Filip.ncols(); j++)
	//	{
	//		A_Filip[i][j] = pow(xFilip[i], j);
	//	}
	//}


	//VecDoub b_Filip = yFilip;
	//VecDoub x_Filip_SVD(11);
	//SVD svd(A_Filip);

	//cout << "W:" << endl;
	//util::print(svd.w);

	//cout << "Inverse Condition number:" << svd.inv_condition() << endl;

	//cout << "\n" << "Solve 1:" << endl;
	//svd.solve(b_Filip, x_Filip_SVD, svd.eps);
	//cout << "Solution for x: " << endl;
	//util::print(x_Filip_SVD);

	//cout << "Eps: " << svd.eps << " tsh: " << svd.tsh << endl;
	//cout << "Residuals: " << residual(A_Filip, x_Filip_SVD, b_Filip) << endl;

	//cout << "\n" << "Solve 2:" << endl;
	//svd.solve(b_Filip, x_Filip_SVD, 0.000008);
	//cout << "Solution for x: " << endl;
	//util::print(x_Filip_SVD);

	//cout << "Eps: " << svd.eps << " tsh: " << svd.tsh << endl;
	//cout << "Residuals: " << residual(A_Filip, x_Filip_SVD, b_Filip) << endl;

	//cout << "\n" << "Solve 3:" << endl;
	//svd.solve(b_Filip, x_Filip_SVD, 0.001);
	//cout << "Solution for x: " << endl;
	//util::print(x_Filip_SVD);

	//cout << "Eps: " << svd.eps << " tsh: " << svd.tsh << endl;
	//cout << "Residuals: " << residual(A_Filip, x_Filip_SVD, b_Filip) << endl;

	////---------------------- New A and b for error estimation --------------
	//double sigma = 1 / 0.00335;
	//MatDoub A_with_sigma = A_Filip * sigma;
	//VecDoub b_with_sigma = b_Filip * sigma;
	//VecDoub x_with_sigma(11);

	//cout << "A_with_sigma " << endl;
	//util::print(A_with_sigma);


	////cout << "Old A: " << endl;
	////util::print(A_Filip);
	////cout << "New A: " << endl;
	////util::print(A_with_sigma);

	//SVD svd_with_sigma(A_with_sigma);
	//svd_with_sigma.solve(b_with_sigma, x_with_sigma, -1);

	//cout << "tsh: " << svd_with_sigma.tsh << endl;


	//cout << "w_with_sigma: " << endl;
	//util::print(svd_with_sigma.w);
	//cout << "x_with_sigma: " << endl;
	//util::print(x_with_sigma);

	//VecDoub delta_x(11, 0.0);

	////add threshold
	//for (int j = 0; j < delta_x.size(); j++)
	//{
	//	double sum = 0.0;
	//	for (int i = 0; i < svd_with_sigma.w.size(); i++)
	//	{
	//		//double div = svd_with_sigma.v[j][i] / svd_with_sigma.w[i];
	//		if (svd_with_sigma.w[i] > svd_with_sigma.tsh)
	//		{
	//			sum += pow(svd_with_sigma.v[j][i] / svd_with_sigma.w[i], 2);
	//		}

	//	}
	//	delta_x[j] = sqrt(sum);
	//}

	//cout << "delta_x: " << endl;
	//util::print(delta_x);

	return 0;
}
