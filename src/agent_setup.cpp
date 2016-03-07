#include "agent_setup.h"

#include "bezier.h"

using std::cout;
using std::endl;

namespace {
	std::vector<agents::agent_frame> readCurve(const boost::property_tree::ptree& value) {
		assert(value.get<std::string>("type") == "curve");

		// reading the curve points
		bezier<Imath::Vec2<float>> curve;
		for(auto& point : value.get_child("points")) {
			std::vector<float> p;
			for(auto& pi : point.second)
				p.push_back(pi.second.get<float>(""));
			if(p.size() != 2)
				throw std::runtime_error("agent_setup - only 2D points are supported at the moment.");

			curve.add_point(Imath::Vec2<float>(p[0], p[1]));
		}

		// sampling the curve into frames (using integer algebra to avoid numerical errors)
		const float sampling = value.get<float>("sampling");
		assert(sampling > 0.0f);
		// get the integer divisor
		const unsigned div = round(1.0f / sampling);

		// add the values
		std::vector<agents::agent_frame> result;
		for(unsigned a=0; a<=div; ++a) {
			const float t = (float)a / (float)div;
			result.push_back(agents::agent_frame{curve[t], curve.normdiff(t)});
		}

		return result;
	}
}

agent_setup::agent_setup(const boost::property_tree::ptree& value) {
	std::vector<std::vector<agents::agent_frame>> frames;

	// read the frame values
	for(auto& item : value) {
		const std::string type = item.second.get<std::string>("type");

		if(type == "curve")
			frames.push_back(readCurve(item.second));

		else
			throw std::runtime_error("agent_setup - only curve type is supported at the moment.");
	}

	if(frames.empty())
		throw std::runtime_error("agent_setup - cannot instantiate an empty setup.");

	// make sure that all frames have the same number of samples, by discarding additional ones
	size_t count = std::numeric_limits<size_t>::max();
	for(auto& a : frames)
		count = std::min(count, a.size());
	for(auto& a : frames)
		if(a.size() > count) {
			cout << "Incorrect number of samples in agent setup." << endl;
			a.resize(count);
		}

	// create the m_agents array, and transfer the data
	m_agents = std::unique_ptr<agents>(new agents(frames.size(), count));
	for(unsigned a=0;a<frames.size();++a)
		for(size_t f=0;f<count;++f)
			(*m_agents)[a][f] = frames[a][f];
}

const agents& agent_setup::operator* () const {
	assert(m_agents.get() != NULL);
	return *m_agents;
}
