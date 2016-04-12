#pragma once

#include <ImathVec.h>

#include "projection.h"

namespace scenarios {

class projection_naive : public scenario<projection_naive, projection>  {
	public:
		static std::string type();

		projection_naive(const boost::property_tree::ptree& config);

		virtual agents apply(const agents& source) const override;

	protected:
	private:
};

}
