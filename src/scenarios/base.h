#pragma once

#include <vector>
#include <memory>

#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/iterator/transform_iterator.hpp>

#include "../factory.h"
#include "../visualisation.h"

namespace scenarios {

class base : public boost::noncopyable {
	public:
		base(const boost::property_tree::ptree& config);

		virtual ~base() {};

		const std::map<std::string, std::unique_ptr<visualisation>>& visualisations() const;

	protected:
	private:
		std::map<std::string, std::unique_ptr<visualisation>> m_visualisations;
};

template<typename DERIVED>
class scenario : public base {
	public:
		scenario(const boost::property_tree::ptree& config);
		virtual ~scenario() override;

	protected:
	private:
		static factory<base, boost::property_tree::ptree>::registration<DERIVED> s_factory;
};

//////////////////////////////////

template<typename DERIVED>
factory<base, boost::property_tree::ptree>::registration<DERIVED> scenario<DERIVED>::s_factory;

template<typename DERIVED>
scenario<DERIVED>::scenario(const boost::property_tree::ptree& config) : base(config) {
}

template<typename DERIVED>
scenario<DERIVED>::~scenario() {
	// a dummy statement to make sure the factory doesn't get optimized away by GCC
	boost::lexical_cast<std::string>(&s_factory);
}

}
