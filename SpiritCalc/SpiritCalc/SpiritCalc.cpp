// SpiritCalc.cpp : Defines the entry point for the application.
//

#include "SpiritCalc.h"
#include "pow.h"

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;

using qi::double_;
using qi::_1;
using ascii::space;
using phoenix::ref;

boost::phoenix::function<calc_pow_impl> calc_pow;

template <typename Iterator>
bool parse_expr(Iterator first, Iterator last, double& n) {
	qi::rule<Iterator, double, ascii::space_type> subexpr;
	qi::rule<Iterator, double, ascii::space_type> uminus = ('-' >> (double_ | subexpr)[qi::_val = -_1]) | ((double_ | subexpr)[qi::_val = _1]);
	qi::rule<Iterator, double, ascii::space_type> _pow = (uminus)[qi::_val = _1] >> *('^' >> (uminus)[qi::_val = calc_pow(qi::_val, _1)]);
	qi::rule<Iterator, double, ascii::space_type> mul = _pow[qi::_val = _1] >> *(('*' >> _pow[qi::_val *= _1])|('/' >> _pow[qi::_val /= _1]));
	qi::rule<Iterator, double, ascii::space_type> add_sub = mul[qi::_val = _1] >> *(('+' >> mul[qi::_val += _1]) | ('-' >> mul[qi::_val -= _1]));
	subexpr = '(' >> add_sub[qi::_val = _1] >> ')';

	bool r = qi::phrase_parse(first, last,
		(
			add_sub[ref(n)= _1]
		), space);
	if (first != last) return false;
	return r;
}

int main()
{
	using namespace std;
	string str;
	std::cout << "Simple calculator implemented on boost::spirit::qi" << std::endl;
	std::cout << "For exit type 'Q' or 'q'" << std::endl;
	while (std::cout << "calc> ", getline(cin, str))
	{
		try {
			if (str.empty() || str[0] == 'q' || str[0] == 'Q') break;
			double n = 1;
			if (parse_expr(str.begin(), str.end(), n)) {
				cout << n << std::endl;
			}
			else {
				cout << "Parsing failed!" << std::endl;
			}
		}
		catch (exception e) {
			std::cerr << "Error: " << e.what() << std::endl;
		}
	}
	return 0;
}
