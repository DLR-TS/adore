#include <cstddef>
#include "util/math/PiecewisePolynomial.h"
#include <stdexcept>
#include <iostream>
#include <numeric>  
#include <Eigen/Dense>


std::vector<double> PiecewisePolynomial::BreaksGenerator(double start, 
                                                double end, 
                                                int NumberOfPoints)
{
	std::vector<double> output;
	double resolution = (end - start) / (NumberOfPoints-1);
	output.push_back(start);
	for (int i = 1; i < NumberOfPoints; i++)
	{
		output.push_back(output[i-1] + resolution);
	}
	output[NumberOfPoints] = end;
	return output;
}
std::vector<double> PiecewisePolynomial::diff(const std::vector<double>& data) {
    // Check if there are at least two elements in the vector
    if (data.size() < 2) {
        std::cerr << "Error: Vector must have at least two elements for differences calculation." << std::endl;
        return {};
    }

    // Preallocate space for differences
    std::vector<double> differences(data.size() - 1);

    // Calculate differences
    for (size_t i = 1; i < data.size(); ++i) {
        differences[i - 1] = data[i] - data[i - 1];
    }

    return differences;
}



bool PiecewisePolynomial::hasValue(const std::vector<double>& data, double threshold, ComparisonType comparison) {
    switch (comparison) {
        case LESS_THAN:
            for (const auto& value : data) {
                if (value < threshold) {
                    return true;
                }
            }
            return false;

        case GREATER_THAN:
            for (const auto& value : data) {
                if (value > threshold) {
                    return true;
                }
            }
            return false;

        case EQUAL_TO:
            for (const auto& value : data) {
                if (value == threshold) {
                    return true;
                }
            }
            return false;

        case LESS_THAN_OR_EQUAL:
            for (const auto& value : data) {
                if (value <= threshold) {
                    return true;
                }
            }
            return false;

        case GREATER_THAN_OR_EQUAL:
            for (const auto& value : data) {
                if (value >= threshold) {
                    return true;
                }
            }
            return false;
    }

    // Handle an invalid comparison type
    std::cerr << "Error: Invalid comparison type." << std::endl;
    return false;
}
std::vector<int> PiecewisePolynomial::find(const std::vector<double>& input, double ref, ComparisonType comparison) {
        std::vector<int> output;
        switch (comparison) {
            case EQUAL_TO:
                for (int i = 0; i < input.size(); ++i) {
                    if (input[i] == ref) {
                        output.push_back(i);
                    }
                }
                break;
            case GREATER_THAN:
                for (int i = 0; i < input.size(); ++i) {
                    if (input[i] > ref) {
                        output.push_back(i);
                    }
                }
                break;
            case LESS_THAN:
                for (int i = 0; i < input.size(); ++i) {
                    if (input[i] < ref) {
                        output.push_back(i);
                    }
                }
                break;
            case GREATER_THAN_OR_EQUAL:
                for (int i = 0; i < input.size(); ++i) {
                    if (input[i] >= ref) {
                        output.push_back(i);
                    }
                }
                break;
            case LESS_THAN_OR_EQUAL:
                for (int i = 0; i < input.size(); ++i) {
                    if (input[i] <= ref) {
                        output.push_back(i);
                    }
                }
                break;
            default:
                printf("\nNOT VALID CONDITION FOR adore::mad::ArrayMatrixTools::find");
                break;
        }
        return output;
}
void PiecewisePolynomial::transpose(std::vector<double>& output,std::vector<double>& input, int input_row, int input_column)
{
	for (int i = 0; i < input_row; i++)
	{
		for (int j = 0; j < input_column; j++)
		{
			output[j*input_row + i] = input[i*input_column + j];
		}
	}
}
void PiecewisePolynomial::matrixMultiplication(std::vector<double>& output,std::vector<double>& input_1, int input_1_row, int input_1_column, std::vector<double>& input_2, int input_2_row, int input_2_column)
{
	if (input_1_column != input_2_row){
        printf("\n Error, invalid matrix size");
        return;
    }
    for (int i = 0; i < input_1_row*input_2_column; i++){
        output[i] = 0;   //initialization
    }
    for (int r_1 = 0; r_1 < input_1_row; r_1++)
	{
		for (int c_2 = 0; c_2 < input_2_column; c_2++)
		{
			for (int c_1 = 0; c_1 < input_1_column; c_1++)
			{
				output[(r_1*input_2_column) + c_2] += input_1[(r_1*input_1_column) + c_1] * input_2[(c_1*input_2_column) + c_2];
			}
		}
	}
}

void PiecewisePolynomial::sort(std::vector<double>& output, std::vector<int>& outputIndex, const std::vector<double>& input) {
    size_t inputSize = input.size();

    // Create a non-const copy of the input vector
    std::vector<double> nonConstInput(input);

    // Initialize outputIndex
    outputIndex.resize(inputSize);
    std::iota(outputIndex.begin(), outputIndex.end(), 0);

    // Sorting
    for (size_t i = 0; i < inputSize; ++i) {
        for (size_t j = inputSize - 1; j > i; --j) {
            if (nonConstInput[j] < nonConstInput[j - 1]) {
                std::swap(nonConstInput[j - 1], nonConstInput[j]);
                std::swap(outputIndex[j - 1], outputIndex[j]);
            }
        }
    }

    // Copy sorted data to the output vector
    output.resize(inputSize);
    std::copy(nonConstInput.begin(), nonConstInput.end(), output.begin());
}
void PiecewisePolynomial::sparseDiagonalMatrix(std::vector<double>&  sparse, std::vector<double>& input, int output_row, int output_column, int StartingReference)
{
    int inputSize = input.size();
	int offsetRow = 0;
	int offsetColumn = 0;
	int counter = 0;
	if (StartingReference > 0)
		offsetColumn = StartingReference;
	if (StartingReference < 0)
		offsetRow = std::abs(StartingReference);
	if (inputSize < std::min(output_row - offsetRow, output_column - offsetColumn) || output_row - offsetRow == 0 || output_column - offsetColumn == 0)
	{
		printf("\nError, DiagonalSparseMatrix, requested matrix has bigger diagonal than delivered input");
		return;
	}
	if (output_column > output_row)
	{
		for (int i = 0; i < output_row; i++)
		{
			for (int j = 0; j < output_column; j++)
			{
				if (i - offsetRow == j - offsetColumn)
				{
					sparse[i*output_column + j] = input[counter + offsetRow];
					counter++;
				}
				else
				{
					sparse[i*output_column + j] = 0;
				}
			}
		}
	}
	if (output_column <= output_row)
	{
		for (int i = 0; i < output_row; i++)
		{
			for (int j = 0; j < output_column; j++)
			{
				if (i - offsetRow == j - offsetColumn)
				{
					sparse[(i*output_column) + j] = input[counter + offsetColumn];
					counter++;
				}
				else
				{
					sparse[(i*output_column) + j] = 0;
				}
			}
		}
	}
}
int PiecewisePolynomial::findIndex(double ref, PiecewisePolynomial::PiecewiseStruct& pp)
{
	int index = -1;
	for(int i = 0; i < pp.size; i++)
	{
		if(ref >= pp.breaks[i]  && ref <= pp.breaks[i+1] )
		{
			index = i;
			break;
		}
	}
	if(index == -1)
	{
		if(ref < pp.breaks[0])
		{
			index = 0;
		}
		if(ref > pp.breaks[pp.size])
		{
			index = pp.size;
		}
	}
	if(index==-1) printf("There is a problem to find index of piecewise polynomial at, %f\n",ref );
	return index;

    
}

PiecewisePolynomial::LocalCoordination PiecewisePolynomial::localCoordination(std::vector<double>& Userbreaks,  
                                                                              PiecewisePolynomial::PiecewiseStruct &pp)
{
    LocalCoordination lc;
    int N = Userbreaks.size();
	lc.lc_breaks.resize(N);
	lc.lc_index.resize(N);
    const double eps = 1e-20;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j <= pp.size - 1; j++)
		{
			if (Userbreaks[i] + eps >= pp.breaks[j] && Userbreaks[i] + eps <= pp.breaks[j + 1]) 
			{
				lc.lc_index[i] = j;
			}
		}
	}
	for (int i = 0; i < N; i++) 
	{		//Local coordination of high resoluted data		
		lc.lc_breaks[i] = (Userbreaks[i] - pp.breaks[lc.lc_index[i]]);
	}	
	return lc;

}

void PiecewisePolynomial::LinearPiecewiseEvaluation(std::vector<double>& interpolatedLinear, 
                            std::vector<double>& Userbreaks, PiecewisePolynomial::PiecewiseStruct &pp)
{
    auto lc = localCoordination(Userbreaks,pp);
    interpolatedLinear.resize(Userbreaks.size());
	for (int i = 0; i < Userbreaks.size(); i++)
	{		
		interpolatedLinear[i] = (pp.coef1[lc.lc_index[i]] * lc.lc_breaks[i] + pp.coef2[lc.lc_index[i]]);			
	}    
}



double PiecewisePolynomial::splineEvaluation(int index, double point, PiecewisePolynomial::PiecewiseStruct &pp )
{
	double s1 = point-pp.breaks[index];
	double s2 = s1 * s1;
	double s3 = s2 * s1;
	return pp.coef1[index]*s3 + pp.coef2[index]*s2 + pp.coef3[index]*s1 + pp.coef4[index];

}
double PiecewisePolynomial::linearEvaluation(int index, double point, PiecewisePolynomial::PiecewiseStruct &pp )
{
    double s1 = point-pp.breaks[index];
    return (pp.coef1[index] * s1 + pp.coef2[index]);

}

void PiecewisePolynomial::CubicSplineEvaluation(std::vector<double>& interpolatedSpline, 
                                                std::vector<double>& Userbreaks, 
                                                PiecewisePolynomial::PiecewiseStruct &pp)
{
	auto lc = localCoordination(Userbreaks,pp);
    interpolatedSpline.resize(Userbreaks.size());
	//ax^3+bx^2+cx+d
	for (int i = 0; i < Userbreaks.size(); i++)
	{		
		interpolatedSpline[i] = ((((pp.coef1[lc.lc_index[i]] * lc.lc_breaks[i] + pp.coef2[lc.lc_index[i]])*lc.lc_breaks[i]) + pp.coef3[lc.lc_index[i]])*lc.lc_breaks[i]) + pp.coef4[lc.lc_index[i]];			
	}

}

void PiecewisePolynomial::CubicSplineEvaluation(std::vector<double>& interpolatedSpline, 
                                                std::vector<double>& d_interpolatedSpline, 
                                                std::vector<double>& Userbreaks, 
                                                PiecewisePolynomial::PiecewiseStruct &pp)
{
	auto lc = localCoordination(Userbreaks,pp);
    interpolatedSpline.resize(Userbreaks.size());
    d_interpolatedSpline.resize(Userbreaks.size());
	//ax^3+bx^2+cx+d
	for (int i = 0; i < Userbreaks.size(); i++)
	{		
		interpolatedSpline[i] = ((((pp.coef1[lc.lc_index[i]] * lc.lc_breaks[i] + pp.coef2[lc.lc_index[i]])*lc.lc_breaks[i]) + pp.coef3[lc.lc_index[i]])*lc.lc_breaks[i]) + pp.coef4[lc.lc_index[i]];	
        d_interpolatedSpline[i] = (((3 * pp.coef1[lc.lc_index[i]] * lc.lc_breaks[i] + 2 * pp.coef2[lc.lc_index[i]])*lc.lc_breaks[i]) + pp.coef3[lc.lc_index[i]]);		
	}

}

void PiecewisePolynomial::CubicSplineEvaluation(std::vector<double>& interpolatedSpline, 
                                                std::vector<double>& d_interpolatedSpline, 
                                                std::vector<double>& dd_interpolatedSpline, 
                                                std::vector<double>& Userbreaks, 
                                                PiecewisePolynomial::PiecewiseStruct &pp)
{
	auto lc = localCoordination(Userbreaks,pp);
    interpolatedSpline.resize(Userbreaks.size());
    d_interpolatedSpline.resize(Userbreaks.size()); 
    dd_interpolatedSpline.resize(Userbreaks.size()); 
	//ax^3+bx^2+cx+d
	for (int i = 0; i < Userbreaks.size(); i++)
	{		
		interpolatedSpline[i] = ((((pp.coef1[lc.lc_index[i]] * lc.lc_breaks[i] + pp.coef2[lc.lc_index[i]])*lc.lc_breaks[i]) + pp.coef3[lc.lc_index[i]])*lc.lc_breaks[i]) + pp.coef4[lc.lc_index[i]];	
        d_interpolatedSpline[i] = (((3 * pp.coef1[lc.lc_index[i]] * lc.lc_breaks[i] + 2 * pp.coef2[lc.lc_index[i]])*lc.lc_breaks[i]) + pp.coef3[lc.lc_index[i]]);		
        dd_interpolatedSpline[i] = 6 * pp.coef1[lc.lc_index[i]] * lc.lc_breaks[i] + 2 * pp.coef2[lc.lc_index[i]];
	}

}
void PiecewisePolynomial::CubicSplineEvaluation(std::vector<double>& interpolatedSpline, 
                                                std::vector<double>& d_interpolatedSpline, 
                                                std::vector<double>& dd_interpolatedSpline, 
                                                std::vector<double>& ddd_interpolatedSpline, 
                                                std::vector<double>& Userbreaks, 
                                                PiecewisePolynomial::PiecewiseStruct &pp)
{
	auto lc = localCoordination(Userbreaks,pp);
    interpolatedSpline.resize(Userbreaks.size());
    d_interpolatedSpline.resize(Userbreaks.size()); 
    dd_interpolatedSpline.resize(Userbreaks.size());   
    ddd_interpolatedSpline.resize(Userbreaks.size());    
	//ax^3+bx^2+cx+d
	for (int i = 0; i < Userbreaks.size(); i++)
	{		
		interpolatedSpline[i] = ((((pp.coef1[lc.lc_index[i]] * lc.lc_breaks[i] + pp.coef2[lc.lc_index[i]])*lc.lc_breaks[i]) + pp.coef3[lc.lc_index[i]])*lc.lc_breaks[i]) + pp.coef4[lc.lc_index[i]];	
        d_interpolatedSpline[i] = (((3 * pp.coef1[lc.lc_index[i]] * lc.lc_breaks[i] + 2 * pp.coef2[lc.lc_index[i]])*lc.lc_breaks[i]) + pp.coef3[lc.lc_index[i]]);		
        dd_interpolatedSpline[i] = 6 * pp.coef1[lc.lc_index[i]] * lc.lc_breaks[i] + 2 * pp.coef2[lc.lc_index[i]];
        ddd_interpolatedSpline[i] = 6 * pp.coef1[lc.lc_index[i]];
	}

}
PiecewisePolynomial::PiecewiseStruct PiecewisePolynomial::linearPiecewise(std::vector<double>& input_x,
                                                std::vector<double>& input_y)
{
    std::vector<double> input_w(input_x.size(), 1.0);
    std::vector<double> x,y,w;
    interpolateAndRemoveDuplicates(x, y, w, input_x, input_y, input_w);
     int N = x.size();
    auto diff_x = diff(x);
    auto diff_y = diff(y);
    std::vector<double> DyDx(N-1);
    std::vector<double> oneDevDx(N-1);
	for (int i = 0; i < N - 1; i++)
	{
        oneDevDx[i] = 1 / diff_x[i];
		DyDx[i] = oneDevDx[i] *  diff_y[i];
	}  
    PiecewiseStruct cp;
    cp.size = N-1;
    cp.order = 1;
    cp.breaks.resize(N);
    cp.coef1.resize(N-1);
    cp.coef2.resize(N-1);
    cp.coef3.resize(N-1);
    cp.coef4.resize(N-1);
    for(int i=0; i<N-1; i++)
    {
        cp.breaks[i] = x[i];
        cp.coef1[i] = DyDx[i];
        cp.coef2[i] = y[i];
        cp.coef3[i] = 0.0;
        cp.coef4[i] = 0.0;
    }
    cp.breaks[N-1] = x[N-1];
    return cp;     

}
PiecewisePolynomial::PiecewiseStruct PiecewisePolynomial::CubicSplineSmoother(std::vector<double>& input_x,
                                                                  std::vector<double>& input_y,
                                                                  std::vector<double>& input_w,
                                                                  double smoothingFactor)
{
	/*
	based on Schöneberg and Reinsch smoothing spline
	short description is given here, for more info see below links
	https://link.springer.com/content/pdf/10.1007/BF02162161.pdf
	https://en.wikipedia.org/wiki/Smoothing_spline
	
	cubic polynomial form f(x)=a + b(x-xi)+c(x-xi)^2+d(x-xi)^3
	continuity and smoothing conditions must be fullfilled
	 1) f''(x+) = f''(x-) 
	 2) f(x+) = f (x-)
	 3) f'(x+) = f' (x-)
	 general formula consists of two parts data fitting and smoothing and smoothingFactor is the weight between parts
	/*Step one: natural end condition f''(x1) = f''(xN) = 0 
	any other points f''(x+) = f''(x-) 
	which results in c_i-1 .Dx_i-1 + c_i.2.(Dx_i-1 + Dx_i) + c_i+1.D_x = 3(Da_i/Dx_i - Da_i-1/Dx_i-1)
	in matrix form Rc = 3 Q_transpose a
	c := (c_i) [2 --> N-1]
	a := (a_i) [1 --> N]
	below we try to build R
	R is a symetric tridiagonal matrix of order N-2  (Dx_i-1, 2(Dx_i-1 + Dx_i), Dx_i)
	*/

    std::vector<double> x,y,w;
    interpolateAndRemoveDuplicates(x, y, w, input_x, input_y, input_w);
    int N = x.size();
    auto diff_x = diff(x);
    auto diff_y = diff(y);
    std::vector<double> DyDx(N-1);
    std::vector<double> oneDevDx(N-1);
	for (int i = 0; i < N - 1; i++)
	{
        oneDevDx[i] = 1 / diff_x[i];
		DyDx[i] = oneDevDx[i] *  diff_y[i];
	}
    std::vector<double> dx1n_2(diff_x.begin() + 1, diff_x.begin() + 1 + (N-2));
    std::vector<double> dx0n_3(diff_x.begin() , diff_x.begin() + 1 + (N-3));
    std::vector<double> dx2x1n_20n_3(N-2);
	for (int i = 0; i < N - 2; i++) {
		dx2x1n_20n_3[i] = 2 * (dx1n_2[i] + dx0n_3[i]);
	}
    std::vector<double> sp_dx1n_2((N-2)*(N-2));
    std::vector<double> sp_dx0n_3((N-2)*(N-2));
    std::vector<double> sp_dx2x1n_20n_3((N-2)*(N-2));
    std::vector<double> R((N-2)*(N-2));
 	sparseDiagonalMatrix(sp_dx1n_2, dx1n_2, N - 2, N - 2, -1);  //sparsing to new size
	sparseDiagonalMatrix(sp_dx0n_3, dx0n_3, N - 2, N - 2, 1);
	sparseDiagonalMatrix(sp_dx2x1n_20n_3, dx2x1n_20n_3, N - 2, N - 2, 0);
 	for (int i = 0; i < N- 2; i++) 
	{
		for (int j = 0; j < N - 2; j++)
		{
			R[i*(N - 2) + j] = sp_dx1n_2[i*(N - 2) + j] + sp_dx0n_3[i*(N - 2) + j] + sp_dx2x1n_20n_3[i*(N - 2) + j];
		}
	}
    /*
    Build Q_transpose
    Q_transpose is a tridiagonal matrix of order (N-2)x(N)
    general row 1/Dx_i-1  , -1/Dx_i-1 - 1*Dx_i , 1/Dx_i
    Process is similar to the R
    */
    std::vector<double> oneDivDx1n_2(oneDevDx.begin() + 1, oneDevDx.begin() + 1 + (N-2));
    std::vector<double> oneDivDx0n_3(oneDevDx.begin() , oneDevDx.begin() + 1 + (N-3));
    std::vector<double> oneDivDx_1n_20n_3(N-2);
    for (int i = 0; i < N - 2; i++) {
	 oneDivDx_1n_20n_3[i] = -1 * (oneDivDx1n_2[i] + oneDivDx0n_3[i]);
    } 
	std::vector<double> sp_oneDivDx1n_2 ((N - 2)*(N));
	std::vector<double> sp_oneDivDx0n_3 ((N - 2)*(N));
	std::vector<double> sp_oneDivDx_1n_20n_3 ((N - 2)*(N));
	sparseDiagonalMatrix(sp_oneDivDx1n_2, oneDivDx1n_2, N - 2, N, 2);
	sparseDiagonalMatrix(sp_oneDivDx0n_3, oneDivDx0n_3, N - 2, N, 0);
	sparseDiagonalMatrix(sp_oneDivDx_1n_20n_3, oneDivDx_1n_20n_3, N - 2, N, 1);
    std::vector<double> Qt ((N - 2)*(N));
    std::vector<double> QtT (N * (N - 2));
	for (int i = 0; i < N - 2; i++)  //data are from 1-->n-2 or 0-->n-3 this is why there is an extrea -2 in i<n-2-2
	{
		for (int j = 0; j < N; j++)
		{
			Qt[i*(N)+j] = sp_oneDivDx1n_2[i*(N)+j] + sp_oneDivDx0n_3[i*(N)+j] + sp_oneDivDx_1n_20n_3[i*(N)+j];
		}
	}
	std::vector<double> D(N);
    std::vector<double> sp_D(N*N);
    std::vector<double> QtD((N-2)*N);
    std::vector<double> QtDQ((N-2)*(N-2));
    std::vector<double> temp_M((N-2)*(N-2));
	for (int i = 0; i < N; i++)
	{
		D[i] = 1 / w[i];
	}
    /*
    equation (4) from https://link.springer.com/content/pdf/10.1007/BF02162161.pdf
    can be written as Mc = 3pQ_transpose y
    M = 6(1-p)Q_t D^2 Q + pR
    D := variance in y
    p := smoothing factor [0, 1]
    a = y - 6(1-p)D^2Qu
    c = 3pu
    */
    sparseDiagonalMatrix(sp_D, D, N, N, 0);
    transpose(QtT, Qt, N - 2, N);
    matrixMultiplication(QtD, Qt, N - 2, N, sp_D, N, N);
    matrixMultiplication(QtDQ, QtD, N - 2, N, QtT, N, N - 2);
    Eigen::MatrixXd M = Eigen::MatrixXd::Zero((N-2),(N-2)); 
	for (int i = 0; i < (N - 2); i++) {
		for (int j = 0; j < (N - 2); j++) {
			M(i, j) = 6 * (1 - smoothingFactor)*QtDQ[i*(N - 2) + j] + smoothingFactor * R[i*(N - 2) + j];
		}
	}
    Eigen::MatrixXd inv_M_e = M.inverse();
    std::vector<double> inv_M((N-2)*(N-2));
	for (int i = 0; i < (N - 2); i++) {
		for (int j = 0; j < (N - 2); j++) {
			inv_M[i*(N - 2) + j] = inv_M_e(i, j);
		}
	}
    std::vector<double> diff_DyDx(N-2);
    std::vector<double> u(N-2);
    diff_DyDx = diff(DyDx);
    matrixMultiplication(u, inv_M, N - 2, N - 2, diff_DyDx, N - 2, 1);
    std::vector<double> u_n(N);
    std::vector<double> diff_u_n(N-1);
	u_n[0] = 0;
	u_n[N - 1] = 0;
	for (int i = 0; i < N - 2; i++) u_n[i + 1] = u[i];
    diff_u_n = diff(u_n);
    std::vector<double> diff_u_n_oneDivDx(N-1);
    for(int i=0; i<N-1;i++){
      diff_u_n_oneDivDx[i] =   diff_u_n[i] * oneDevDx[i];
    }
    std::vector<double> diff_unoneDivDx_n1(N+1);
	diff_unoneDivDx_n1[0] = 0;
	diff_unoneDivDx_n1[N] = 0;
	for (int i = 0; i < N - 1; i++)  diff_unoneDivDx_n1[i + 1] = diff_u_n_oneDivDx[i];
    std::vector<double> diff2_unoneDivDx_n1(N);
    diff2_unoneDivDx_n1 = diff(diff_unoneDivDx_n1);
	for (int i = 0; i < N; i++)
	{
		y[i] = y[i] - (6 * (1 - smoothingFactor))*sp_D[i*N + i] * diff2_unoneDivDx_n1[i];
	}
    std::vector<double> c3(N);
    std::vector<double> diff_c3(N-1);
    c3[0] = 0;
    c3[N - 1] = 0;
    for (int i = 0; i < N - 2; i++) {
        c3[i + 1] = u[i] * smoothingFactor;
    }
    diff_c3 = diff(c3);
    std::vector<double> diff_yi(N-1);
    diff_yi = diff(y);
    std::vector<double> c2(N-1);
	for (int i = 0; i < N - 1; i++) c2[i] = diff_yi[i] * oneDevDx[i] - diff_x[i] * (2 * c3[i] + c3[i + 1]);
    PiecewiseStruct cp;
    cp.size = N-1;
    cp.order = 3;
    cp.breaks.resize(N);
    cp.coef1.resize(N-1);
    cp.coef2.resize(N-1);
    cp.coef3.resize(N-1);
    cp.coef4.resize(N-1);
    for(int i=0; i<N-1; i++)
    {
        cp.breaks[i] = x[i];
        cp.coef1[i] = diff_c3[i] * oneDevDx[i];
        cp.coef2[i] = 3 * c3[i];
        cp.coef3[i] = c2[i];
        cp.coef4[i] = y[i];
    }
    cp.breaks[N-1] = x[N-1];
    return cp;
}                                                 
                                                



void PiecewisePolynomial::interpolateAndRemoveDuplicates(std::vector<double>& output_x,
                                                 std::vector<double>& output_y,
                                                 std::vector<double>& output_w,
                                                 std::vector<double>& input_x,
                                                 std::vector<double>& input_y,
                                                 std::vector<double>& input_w)
{
   
	//check for repeated value of x axis, if there is any, an interpolation by considering their importance (weight) is done
    //1 : check if data on the x (axis) are ascending
    int N = input_x.size();
    auto diff_x = diff(input_x);
    std::vector<double> tmp_x, tmp_y, tmp_w;
    std::vector<int> ind_x;
    tmp_x.resize(N);
    tmp_y.resize(N);
    tmp_w.resize(N);
    ind_x.resize(N);
    if(hasValue(diff_x,0.00,LESS_THAN))
    {
        //sorting is needed
        printf("There is a ,repetetive value, sorting is necessary\n");
        sort(tmp_x, ind_x, input_x);
        for(int i=0; i<N; i++){
			tmp_y[i] = input_y[ind_x[i]];
			tmp_w[i] = input_w[ind_x[i]];
        }
    std::copy(tmp_x.begin(), tmp_x.end(), input_x.begin());
    std::copy(tmp_y.begin(), tmp_y.end(), input_y.begin());
    std::copy(tmp_w.begin(), tmp_w.end(), input_w.begin());
    }
    //2: check for repetitive value, which means diff of data is equal to zero
    if(!hasValue(diff_x,0. ,EQUAL_TO))  //good data
    {
        output_x.resize(N);
        output_y.resize(N);
        output_w.resize(N);
        std::copy(input_x.begin(), input_x.end(), output_x.begin());
        std::copy(input_y.begin(), input_y.end(), output_y.begin());
        std::copy(input_w.begin(), input_w.end(), output_w.begin());
        return;
    }
	printf("\nThere is repetetive value");
	//********************************************************************************
	// from here till end is about the case of repetetive value on x, and necssity of deleting them and replacing them by a good interpolation by considering their importance (weight)
	//********************************************************************************
    auto repeatedValueIndex = find(diff_x, 0.0, GREATER_THAN);
    std::vector<int> index_tmp1, index_tmp2, repetitionVector;
    int repeatedValueIndexSize = repeatedValueIndex.size();
    index_tmp1.resize(repeatedValueIndexSize+1);
    index_tmp2.resize(repeatedValueIndexSize+1);
    repetitionVector.resize(repeatedValueIndexSize+1);
    index_tmp1[0] = 0;
	for (int i = 0; i < repeatedValueIndexSize; i++)
	{
		index_tmp1[i + 1] = repeatedValueIndex[i] + 1;
	}
    std::copy(index_tmp1.begin() + 1, index_tmp1.begin() + repeatedValueIndexSize + 1, index_tmp2.begin()); 
    index_tmp2[repeatedValueIndexSize] = N; 
	for (int i = 0; i < repeatedValueIndexSize + 1; i++)
	{
		repetitionVector[i] = index_tmp2[i] - index_tmp1[i];
	} 
    std::vector<double> new_y_interpolated(repeatedValueIndexSize+1); 
    Eigen::MatrixXd sumw = Eigen::MatrixXd::Zero(repeatedValueIndexSize+1,1); 
    Eigen::MatrixXd wvec = Eigen::MatrixXd::Zero(repeatedValueIndexSize+1,1);
    Eigen::MatrixXd y_array = Eigen::MatrixXd::Zero(1,repeatedValueIndexSize+1);
	for (int i = 0; i < repeatedValueIndexSize + 1; i++)
	{
		new_y_interpolated[i] = input_y[index_tmp1[i]];
		sumw(i, 0) = input_w[index_tmp1[i]];
	}    
    std::vector<int> ind(repeatedValueIndexSize+1);
	for (int i = 0; i < repeatedValueIndexSize + 1; i++)
	{
		if (repetitionVector[i] > 1)     //repeated value
		{
			sumw(i, 0) = 0;
			for (int j = 1; j <= repetitionVector[i]; j++)
			{
				ind[j] = j + (index_tmp1[i] - 1);
				wvec(j, 0) = input_w[ind[j]];
				sumw(i, 0) = sumw(i, 0) + wvec(j, 0);
				y_array(0, j) = input_y[ind[j]];
			}
            new_y_interpolated[i] = ((y_array * wvec)(0, 0)) / sumw(i, 0);  // Assuming new_y_interpolated is a vector or array of type double
		}
		output_x[i] = input_x[index_tmp1[i]];
		output_y[i] = new_y_interpolated[i];
		output_w[i] = sumw(i, 0);
	}    

}

