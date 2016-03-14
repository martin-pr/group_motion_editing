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

namespace {
	struct scenario_setup {
		scenario_setup(const std::string& n) : name(n) {
		}

		std::string name;
		std::vector<std::pair<std::string, std::unique_ptr<scenarios::base>>> scenarios;
		std::vector<std::pair<std::string, std::unique_ptr<visualisation>>> visualisations;
	};
}

int main(int argc, char* argv[]) {
	// Declare the supported options.
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("input", po::value<std::string>(), "input json configuration file")
		("output", po::value<std::string>(), "output SVG filename")
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
	std::vector<std::pair<std::string, agent_setup>> agent_setups;
	for(auto& setup : tree.get_child("agent_setups"))
		agent_setups.push_back(std::make_pair(setup.first, agent_setup(setup.second)));

	// read the scenarios
	std::vector<scenario_setup> scenario_setups;
	for(auto& scen : tree.get_child("scenarios")) {
		scenario_setups.push_back(scenario_setup(scen.first));

		for(auto& setup : scen.second.get_child("setups")) {
			scenario_setups.back().scenarios.push_back(std::make_pair(setup.first,
				std::move(factory<scenarios::base, boost::property_tree::ptree>::singleton().create(
					scen.second.get_child("type").data(),
					setup.second))));
		}

		for(auto& vis : scen.second.get_child("visualisations")) {
			scenario_setups.back().visualisations.push_back(std::make_pair(
				vis.first,
				std::unique_ptr<visualisation>(new visualisation(vis.second))))	;
		}
	}

	const unsigned width = tree.get<unsigned>("scene.width");
	const unsigned height = tree.get<unsigned>("scene.height");
	const unsigned separation = tree.get<unsigned>("scene.separation");

	unsigned x = 0, y = 0;

	// open the output file
	std::stringstream svg;

	// for each scenario, each agent setup and each visualisation, generate an SVG
	for(auto& s : scenario_setups) {
		for(auto& sc : s.scenarios) {
			for(auto& v : s.visualisations) {
				y += separation / 2;

				x = 0;
				for(auto& a : agent_setups) {
					x += separation / 2;

					const agents output = sc.second->apply(*a.second);

					svg << "<svg x=\"" << x << "\" y=\"" << y << "\" width=\"" << width << "\" height=\"" << height << "\">" << endl;
					v.second->draw(svg, *a.second, output, *sc.second);
					svg << "</svg>" << endl;

					x += width + separation / 2;
				}

				y += height + separation / 2;
			}
		}
	}

	///

	std::ofstream file(vm["output"].as<std::string>().c_str());

	file << "<?xml version=\"1.0\" standalone=\"no\"?>" << endl;
	file << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << endl;

	file << "<svg version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" width=\"" << x << "\" height=\"" << y << "\">" << endl;

	file << svg.str();

	file << "</svg>" << endl;

	return 0;
}
