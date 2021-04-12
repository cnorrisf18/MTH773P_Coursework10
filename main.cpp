#include <iostream>
#include <vector>
#include "classes.h"

using namespace std;

int main()
{
	vector<Swap> swap_vec;
	int f = 2;
	swap_vec.push_back(Swap(1, .0264, f));
	swap_vec.push_back(Swap(2, .0302, f));
	swap_vec.push_back(Swap(3, .0342, f));
	swap_vec.push_back(Swap(5, .0411, f));
	swap_vec.push_back(Swap(7, .0456, f));
	swap_vec.push_back(Swap(10, .0497, f));

	DiscountCurve dc = Bootstrapper(swap_vec);


	//i 
	cout << "i: " << endl;
	Swap ex_i(1, dc.discFac(1), 1);
	cout << "Price today of a zero coupon bond with maturity 1 year: " << ex_i.NPV(dc) << endl << endl;

	//ii
	cout << "ii: " << endl;
	cout << "Continuously-compounded 2-year deposit rate: " << dc.continuously_compounded_rate(2) << endl << endl;

	//iii
	cout << "iii: " << endl;
	Swap ex_iii(7, .0442, 2);
	cout << "Price today of a 7-year swap with a swap rate of 4.42: "<<  ex_iii.NPV(dc) * 100 << endl << endl;

	//iv
	cout << "iv: " << endl;
	cout << "Par rate for a swap with a maturity of 9 years: " << dc.parRate(9) << endl << endl;
}