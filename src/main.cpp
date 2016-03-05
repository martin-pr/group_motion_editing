#include <iostream>

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

	// for(auto& setup : agent_setups) {
	// 	cout << "setup " << setup.first << ":" << endl;
	// 	for(auto& agent : setup.second) {
	// 		cout << "  agent" << endl;
	// 		for(float t=0; t<1; t += agent.sampling)
	// 			cout << "    " << agent.curve[t] << endl;
	// 	}
	// }

	// read the scenarios
	std::map<std::string, std::unique_ptr<scenarios::base>> scenarios;
	for(auto& setup : tree.get_child("scenarios"))
		scenarios.insert(std::make_pair(setup.first,
			std::move(factory<scenarios::base, boost::property_tree::ptree>::singleton().create(
				setup.second.get_child("type").data(),
				setup.second))));

	return 0;
}
