#pragma once

#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/lexical_cast.hpp>

#include "../factory.h"

namespace visualisations {

class base : public boost::noncopyable {
	public:
		virtual ~base() {};

	protected:
	private:
};

template<typename DERIVED>
class visualisation : public base {
	public:
		visualisation();
		virtual ~visualisation() override;

	protected:
	private:
		static factory<base, boost::property_tree::ptree>::registration<DERIVED> s_factory;
};

//////////////////////////////////

template<typename DERIVED>
factory<base, boost::property_tree::ptree>::registration<DERIVED> visualisation<DERIVED>::s_factory;

template<typename DERIVED>
visualisation<DERIVED>::visualisation() {
}

template<typename DERIVED>
visualisation<DERIVED>::~visualisation() {
	// a dummy statement to make sure the factory doesn't get optimized away by GCC
	boost::lexical_cast<std::string>(&s_factory);
}

}
