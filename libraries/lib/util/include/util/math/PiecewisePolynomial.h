#ifndef PIECEWISEPOLYNOMIAL_H
#define PIECEWISEPOLYNOMIAL_H

#include <vector>

class PiecewisePolynomial {
public:
enum ComparisonType {
    LESS_THAN,
    GREATER_THAN,
    EQUAL_TO,
    LESS_THAN_OR_EQUAL,
    GREATER_THAN_OR_EQUAL
};
struct PiecewiseStruct{
    std::vector<double> breaks;
    std::vector<double> coef1;
    std::vector<double> coef2;
    std::vector<double> coef3;
    std::vector<double> coef4;
    int size;
    int order;
};
struct LocalCoordination{
	std::vector<double> lc_breaks;
	std::vector<int> lc_index;
};			
		

std::vector<double> BreaksGenerator(double start, 
                                      double end, 
                                      int NumberOfPoints);

 
std::vector<double> diff(const std::vector<double>& data);

void transpose(std::vector<double>& output,
               std::vector<double>& input, 
               int input_row, int input_column);

void matrixMultiplication(std::vector<double>& output,
                          std::vector<double>& input_1, 
                          int input_1_row, int input_1_column, 
                          std::vector<double>& input_2, 
                          int input_2_row, int input_2_column);

void sort(std::vector<double>& output, 
          std::vector<int>& outputIndex, 
          const std::vector<double>& input);

void sparseDiagonalMatrix(std::vector<double>&  sparse, 
                          std::vector<double>& input, 
                          int output_row, 
                          int output_column, 
                          int StartingReference);
std::vector<int> find(const std::vector<double>& input, 
                      double ref, ComparisonType comparison);

PiecewiseStruct CubicSplineSmoother(std::vector<double>& input_x,
                        std::vector<double>& input_y,
                        std::vector<double>& input_w,
                        double smoothingFactor);

PiecewiseStruct linearPiecewise(std::vector<double>& input_x,
                                    std::vector<double>& input_y);
                                    
void interpolateAndRemoveDuplicates(std::vector<double>& output_x,
                                    std::vector<double>& output_y,
                                    std::vector<double>& output_w,
                                    std::vector<double>& input_x,
                                    std::vector<double>& input_y,
                                    std::vector<double>& input_w);

bool hasValue(const std::vector<double>& data, double threshold, ComparisonType comparison);
int findIndex(double ref, PiecewiseStruct& pp);
LocalCoordination localCoordination(std::vector<double>& Userbreaks,  PiecewiseStruct &pp);
void LinearPiecewiseEvaluation(std::vector<double>& interpolatedLinear, 
                            std::vector<double>& Userbreaks, PiecewiseStruct &pp);
double linearEvaluation(int index, double point, PiecewiseStruct &pp );                            
double splineEvaluation(int index, double point, PiecewiseStruct &pp );
void CubicSplineEvaluation(std::vector<double>& interpolatedSpline, 
                            std::vector<double>& Userbreaks, PiecewiseStruct &pp);
void CubicSplineEvaluation(std::vector<double>& interpolatedSpline, 
                            std::vector<double>& d_interpolatedSpline, 
                            std::vector<double>& Userbreaks, PiecewiseStruct &pp);
void CubicSplineEvaluation(std::vector<double>& interpolatedSpline, 
                            std::vector<double>& d_interpolatedSpline, 
                            std::vector<double>& dd_interpolatedSpline, 
                            std::vector<double>& Userbreaks, PiecewiseStruct &pp);    
void CubicSplineEvaluation(std::vector<double>& interpolatedSpline, 
                            std::vector<double>& d_interpolatedSpline, 
                            std::vector<double>& dd_interpolatedSpline, 
                            std::vector<double>& ddd_interpolatedSpline, 
                            std::vector<double>& Userbreaks, PiecewiseStruct &pp);                                                      

private:                                                 
};

#endif PIECEWISEPOLYNOMIAL_H
