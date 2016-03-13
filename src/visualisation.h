#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/noncopyable.hpp>

namespace scenarios {
	class base;
}

namespace visualisations {
	class base;
}

class agents;

class visualisation : public boost::noncopyable {
	public:
		visualisation(const boost::property_tree::ptree& config);
		~visualisation();

		void draw(std::ostream& svg, const agents& input, const agents& output, const scenarios::base& scenario) const;

	protected:
	private:
		std::vector<std::unique_ptr<visualisations::base>> m_visualisations;
};
