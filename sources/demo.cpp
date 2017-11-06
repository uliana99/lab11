#include <print.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

using namespace boost::program_options;
using namespace boost::filesystem;

int main(int argc, char** argv) {
try {
	std::string text;

    options_description desc{"Options"};
    desc.add_options()
      ("help,h", "Help screen")
      ("output", value<std::string>(), "Out");

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);

    std::string path = getenv("HOME");
    path += "/.config/demo.cfg";

    if(exists(path))
    	store(parse_config_file<char>( path.c_str(), desc ), vm);

    notify(vm);

    if(vm.count("help")) {
      std::cout << desc << '\n';
    } else if(getenv("DEMO_OUTPUT") != nullptr) {
        std::string _path = getenv("DEMO_OUTPUT");
        std::ofstream out(_path, std::ios_base::app);
        while(std::cin >> text) {
			print(text, out);
			out << std::endl;
		}
        out.close();
  	} else if(vm.count("output")) {
  		std::ofstream out(vm["output"].as<std::string>(), std::ios_base::app);
		while(std::cin >> text) {
			print(text, out);
			out << std::endl;
		}
		out.close();
  	} else {
		std::ofstream out("default.log", std::ios_base::app);
		while(std::cin >> text) {
			print(text, out);
			out << std::endl;
		}
		out.close();
  	}

  } catch (const error &ex) {
    std::cerr << ex.what() << '\n';
  }
}
