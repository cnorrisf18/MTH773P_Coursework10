#pragma once
#include <vector>
#include <map>

using namespace std;

class LinearInterpolator
{
public:

	LinearInterpolator() {};
	void insert(double x, double y); //add a single point to the private map
	double value(double x); //calculate and return the y-value at a given x

private:

	map<double, double> pointsmap; //this holds the points entered by the insert function
};


class DiscountCurve
{ //doesn't know about swaps
public:
	DiscountCurve();
	void insert(double T, double df);
	double discFac(double T);
	double continuously_compounded_rate(double T);
	double parRate(double T);
private:
	LinearInterpolator m_interpolator;
};


class Swap
{ //doesn't know about bootstrapping
public:
	Swap(int maturity, double swap_rate, double frequency) : m_maturity(maturity), m_swap_rate(swap_rate), m_frequency(frequency) {};
	double NPV( DiscountCurve& dc); //net present value
	int maturity() { return m_maturity; };
	double swap_rate() { return m_swap_rate; };
private:
	//Member variables: Maturity (in whole years), Swap rate (fixed rate), Frequency
	const int m_maturity;
	const double m_swap_rate;
	const int m_frequency;
};

DiscountCurve Bootstrapper(vector<Swap> swap_objs);
//Input: vector of N swap objects whose swap rates are par (swap rates quoted in the market)
//Use solver N times to determine N points on the discount factor curve


