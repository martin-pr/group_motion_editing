#include "tools.h"

using std::endl;

namespace {
	void printPtree(std::ostream& out, const boost::property_tree::ptree& input, const unsigned indent = 0) {
		out << input.data() << endl;

		for(auto& it : input) {
			for(unsigned a=0;a<indent;++a)
				out << "  ";
			out << it.first << ": " << std::flush;
			printPtree(out, it.second, indent+1);
		}
	}
}

namespace boost { namespace property_tree {

std::ostream& operator << (std::ostream& out, const ptree& value) {
	printPtree(out, value);

	return out;
}

} }
