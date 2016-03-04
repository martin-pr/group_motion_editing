#include <iostream>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "agent_setup.h"

namespace po = boost::program_options;

using std::cout;
using std::endl;

namespace {
	void printPtree(const boost::property_tree::ptree& input, const unsigned indent = 0) {
		cout << input.data() << endl;

		for(auto& it : input) {
			for(unsigned a=0;a<indent;++a)
				cout << "  ";
			cout << it.first << ": " << std::flush;
			printPtree(it.second, indent+1);
		}
	}
}

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

	printPtree(tree);
	cout << endl;

	// read the agent setups
	std::map<std::string, agent_setup> agent_setups;
	for(auto& setup : tree.get_child("agent_setups"))
		agent_setups.insert(std::make_pair(setup.first, agent_setup(setup.second)));

	for(auto& setup : agent_setups) {
		cout << "setup " << setup.first << ":" << endl;
		for(auto& agent : setup.second) {
			cout << "  agent" << endl;
			for(float t=0; t<1; t += agent.sampling)
				cout << "    " << agent.curve[t] << endl;
		}
	}

	return 0;
}
