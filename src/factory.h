#pragma once

#include <memory>
#include <stdexcept>
#include <iostream>

#include <boost/bimap.hpp>

template<typename T, typename PARAM>
class factory {
	public:
		static factory& singleton();

		std::unique_ptr<T> create(const std::string& type, const PARAM& param);

		struct registration_base {
			virtual ~registration_base() {};
			virtual std::unique_ptr<T> create(const PARAM& param) const = 0;
		};

		template<typename U>
		struct registration : public registration_base {
			registration();
			virtual ~registration() override;

			virtual std::unique_ptr<T> create(const PARAM& param) const override;
		};

	protected:
	private:
		boost::bimap<std::string, registration_base*> m_factories;

	template<typename U>
	friend class registration;
};

/////////////////////////////////////////////////////////////////////

template<typename T, typename PARAM>
template<typename U>
factory<T, PARAM>::registration<U>::registration() {
	singleton().m_factories.left.insert(std::make_pair(U::type(), this));
}

template<typename T, typename PARAM>
template<typename U>
factory<T, PARAM>::registration<U>::~registration() {
	auto it = singleton().m_factories.right.find(this);
	if(it != singleton().m_factories.right.end())
		singleton().m_factories.right.erase(it);
}

template<typename T, typename PARAM>
template<typename U>
std::unique_ptr<T> factory<T, PARAM>::registration<U>::create(const PARAM& param) const {
	return std::unique_ptr<T>(new U(param));
}

///

template<typename T, typename PARAM>
factory<T, PARAM>& factory<T, PARAM>::singleton() {
	static std::unique_ptr<factory<T, PARAM>> s_instance;
	if(s_instance.get() == NULL)
		s_instance = std::unique_ptr<factory<T, PARAM>>(new factory<T, PARAM>());

	return *s_instance;
}

template<typename T, typename PARAM>
std::unique_ptr<T> factory<T, PARAM>::create(const std::string& type, const PARAM& param) {
	auto it = m_factories.left.find(type);
	if(it == m_factories.left.end())
		throw std::runtime_error("can't create an object - factory " + type + " not found");
	return it->second->create(param);
}
