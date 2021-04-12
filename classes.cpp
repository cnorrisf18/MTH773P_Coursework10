#include <iostream>
#include <map>
#include <iterator>
#include "classes.h"

using namespace std;

void LinearInterpolator::insert(double x, double y)
{
	if (pointsmap.count(x) > 0) //the x-value is already present in the map, so delete it so it gets overridden
	{
		pointsmap.erase(x);
	}
	 //now we can add the point no matter what
	pointsmap.insert(pair<double, double>(x, y));
}
double LinearInterpolator::value(double x)
{
	map<double, double>::iterator lowerpoint = pointsmap.begin();
	map<double, double>::iterator higherpoint = prev(pointsmap.end());

	if ((x < lowerpoint->first) || (x > higherpoint->first)) // the given value of x lies outside of the known range of points 
	{
		double y_sum = 0.0;
		//cout << "The given value of x lies outside of the boundary of known values, so a y-value cannot be computed. Returning the average of given y-values." << endl;
		for (map<double, double>::iterator it = pointsmap.begin(); it != pointsmap.end(); ++it)
		{
			y_sum += it->second;
		}
		return y_sum / pointsmap.size();
	}


	for (map<double, double>::iterator it = pointsmap.begin(); it != pointsmap.end(); ++it)
	{
		//we are going through the map value by value using iterators
		if (it->first == x) //if the x value at the iterator equals the given x-value, our returned y value is just the previously entered y
		{
			return it->second;
		}
		if (it->first < x)  //if the x value at the iterator is greater than the lower point, set the lower point to the iterator
		{
			lowerpoint = it;
		}
		if (it->first > x) //if the x value at the iterator is less than the higher point, set the higher point to the iterator and break the loop
		{
			higherpoint = it;
			break;
		}

	}

	//after the for loop, we should have correct iterators for the lower point and the higher point values surrounding the given x-value

	//get the needed values from the iterators
	double x1 = lowerpoint->first;
	double y1 = lowerpoint->second;
	double x2 = higherpoint->first;
	double y2 = higherpoint->second;

	//calculate y-value

	double y = y1 + ((x - x1) * ((y2 - y1) / (x2 - x1)));
	return y;

}

DiscountCurve::DiscountCurve()
{
	m_interpolator.insert(0, 1);
}

void DiscountCurve::insert(double T, double df)
{
	m_interpolator.insert(T, df);
}
double DiscountCurve::discFac(double T)
{
	double deposit_rate = m_interpolator.value(T);
	return 1 / pow(1.0 + deposit_rate, T);
}
double DiscountCurve::continuously_compounded_rate(double T)
{
	double discount_factor = discFac(T);
	double ccr = -log(discount_factor) / T;
	return ccr;
}
double DiscountCurve::parRate(double T)
{
	return m_interpolator.value(T);
}

double Swap::NPV(DiscountCurve& dc)
{
	double S_f = m_swap_rate / m_frequency;
	int n = m_frequency * m_maturity;
	double sum = 0;

	for (int i = 1; i <= n; ++i)
	{
		double t_i = double(i) / m_frequency;
		sum += dc.discFac(t_i);
	}

	return S_f * sum + dc.discFac(m_maturity) - 1;
}

DiscountCurve Bootstrapper(vector<Swap> swap_objs)
//Input: vector of N swap objects whose swap rates are par (swap rates quoted in the market)
//Use solver N times to determine N points on the discount factor curve
{
	DiscountCurve dc;
	for (int i = 0; i < swap_objs.size(); ++i)
	{
		dc.insert(swap_objs[i].maturity(), swap_objs[i].swap_rate());
	}
	return dc;
}