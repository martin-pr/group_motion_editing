#pragma once

#include <ImathVec.h>

#include "base.h"
#include "curve.h"
#include "../bezier.h"

namespace scenarios {

class projection : public scenario<projection>, public curve {
	public:
		static std::string type();

		projection(const boost::property_tree::ptree& config);

		virtual agents apply(const agents& source) const override;

	protected:
	private:
};

}
