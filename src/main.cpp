#include <iostream>
#include <fstream>
#include <sstream>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "agent_setup.h"
#include "scenarios/base.h"
#include "factory.h"
#include "tools.h"

namespace po = boost::program_options;

using std::cout;
using std::endl;

int main(int argc, char* argv[]) {
	// Declare the supported options.
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("input", po::value<std::string>(), "input json configuration file")
		("output", po::value<std::string>(), "output directory")
	;

	// process the options
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if(vm.count("help") || (!vm.count("input")) || (!vm.count("output"))) {
		cout << desc << "\n";
		return 1;
	}

	// read the whole input into a ptree
	boost::property_tree::ptree tree;
	boost::property_tree::read_json(vm["input"].as<std::string>(), tree);

	// cout << tree << endl;

	// read the agent setups
	std::map<std::string, agent_setup> agent_setups;
	for(auto& setup : tree.get_child("agent_setups"))
		agent_setups.insert(std::make_pair(setup.first, agent_setup(setup.second)));

	// read the scenarios
	std::map<std::string, std::unique_ptr<scenarios::base>> scenarios;
	for(auto& setup : tree.get_child("scenarios"))
		scenarios.insert(std::make_pair(setup.first,
			std::move(factory<scenarios::base, boost::property_tree::ptree>::singleton().create(
				setup.second.get_child("type").data(),
				setup.second))));

	// for each scenario, each agent setup and each visualisation, generate an SVG
	for(auto& s : scenarios) {
		for(auto& a : agent_setups) {
			for(auto& v : s.second->visualisations()) {
				const std::string filename = vm["output"].as<std::string>() + "/" + s.first + "_" + a.first + "_" + v.first + ".svg";

				std::stringstream svgContent;
				v.second->draw(svgContent, *a.second, *s.second);

				std::ofstream svg(filename.c_str());

				svg << "<?xml version=\"1.0\" standalone=\"no\"?>" << endl;
				svg << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << endl;

				svg << "<svg version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">" << endl;
				svg << svgContent.str();
				svg << "</svg>" << endl;
			}
		}
	}

	return 0;
}
