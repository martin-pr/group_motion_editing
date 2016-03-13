#pragma once

#include "base.h"

namespace scenarios {

class empty : public scenario<empty> {
	public:
		empty(const boost::property_tree::ptree& config);

		static std::string type();

		virtual agents apply(const agents& source) const override;

	protected:
	private:
};

}
