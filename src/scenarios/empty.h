#pragma once

#include "base.h"

namespace scenarios {

class empty : public scenario<empty> {
	public:
		empty(const boost::property_tree::ptree& config);

		static std::string type();

	protected:
	private:
};

}
