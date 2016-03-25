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
	const unsigned font_size = tree.get<unsigned>("scene.font_size");

	// the output stream
	std::stringstream svg;

	unsigned y = 0, x = 0;

	// for each scenario, each agent setup and each visualisation, generate an SVG
	for(auto& s : scenario_setups) {
		// put the scenario heading
		svg << "<rect x=\"0\" y=\"" << y+font_size*0.5 << "\" "
			"width=\"" << agent_setups.size()*width + (agent_setups.size()-1)*separation + font_size*3.5 << "\" "
			"height=\"" << font_size * 3 << "\" "
			"fill=\"grey\"/>" << endl;

		svg << "<text font-family=\"Verdana, sans-serif\" font-weight=\"bold\" "
			"font-size=\"" << font_size*2 << "\" "
			"x=\"" << font_size/2 << "\" y=\"" << y+font_size*2.5 << "\" "
			"fill=\"white\">" << s.name <<"</text>" << endl;

		y += font_size*3;

		{
			unsigned ctr = 0;
			for(auto& a : agent_setups) {
				svg << "<text font-family=\"Verdana, sans-serif\" "
					"font-size=\"" << font_size << "\" "
					"x=\"" << (width+separation)*ctr + font_size*3.5 << "\" y=\"" << y+font_size*1.5 << "\" "
					">" << a.first <<"</text>" << endl;

				++ctr;
			}
			y += font_size * 2;
		}

		for(auto& sc : s.scenarios) {
			svg << "<rect x=\"0\" y=\"" << y << "\" "
				"width=\"" << font_size*1.75 << "\" "
				"height=\"" << s.visualisations.size()*height + (s.visualisations.size()-1)*separation << "\" "
				"fill=\"lightgrey\"/>" << endl;

			svg << "<text font-family=\"Verdana, sans-serif\" font-weight=\"bold\" "
				"font-size=\"" << font_size << "\" x=\"" << font_size*1.5 << "\" "
				"y=\"" << y + separation*(s.visualisations.size()-1) + height*(s.visualisations.size()) << "\" "
				"fill=\"white\" transform=\"rotate(-90 " << font_size*1.125 << ", " << y + separation*(s.visualisations.size()-1) + height*(s.visualisations.size()) << ")\">" << sc.first <<"</text>" << endl;

			for(auto& v : s.visualisations) {
				svg << "<text font-family=\"Verdana, sans-serif\" "
					"font-size=\"" << font_size << "\" x=\"" << font_size*3 << "\" y=\"" << y+height << "\" "
					"transform=\"rotate(-90 " << font_size*3 << ", " << y+height << ")\">" << v.first <<"</text>" << endl;

				x = font_size*3.5;
				for(auto& a : agent_setups) {
					const agents output = sc.second->apply(*a.second);

					svg << "<svg x=\"" << x << "\" y=\"" << y << "\" width=\"" << width << "\" height=\"" << height << "\">" << endl;
					v.second->draw(svg, *a.second, output, *sc.second);
					svg << "</svg>" << endl;

					x += width + separation;
				}

				y += height + separation;
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
