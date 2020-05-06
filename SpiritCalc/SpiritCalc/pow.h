#ifndef __POW_H__
#define __POW_H__



struct calc_pow_impl {
	template <typename T1>
	struct result { typedef double type; };
	double operator()(double& base, double& exp) const;
};


#endif