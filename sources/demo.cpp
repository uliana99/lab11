#include <print.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

using namespace boost::program_options;
using namespace boost::filesystem;

int main(int argc, char** argv) {
    try {
        std::string text;

        //  Описываем флаги для сборки программы
        options_description desc{"Options"};
        desc.add_options()
        ("help,h", "Help screen")
        ("output", value<std::string>(), "Out");

        variables_map in_cons;
        variables_map in_file;

        //  Сканируем консоль на указанные флаги
        store(parse_command_line(argc, argv, desc), in_cons);

        std::string path = getenv("HOME");
        path += "/.config/demo.cfg";

        if(exists(path)) 
            //  Сканируем файл на наличие флагов 
            store(parse_config_file<char>( path.c_str(), desc ), in_file);

        notify(in_cons);
        notify(in_file);

        //  ОБРАБОТКА ПРИОРИТЕТОВ НА СПЕЦИФИКАЦИИ СБОРКИ ПРОГРАММЫ
        //  Определяем общую команду вывода всех возможных флагов для данной программы
        if(in_cons.count("help") || in_file.count("help")) {
            std::cout << desc << '\n';
        } 
        //  Обработка 1-го приоритета: указание в консоли флага --output example.log
        else if(in_cons.count("output")) {
            std::ofstream out(in_cons["output"].as<std::string>(), std::ios_base::app);
            while(std::cin >> text) {
                print(text, out);
                out << std::endl;
            }
            out.close();
        } 
        //  Обработка 2-го приоритета: через глобальную переменную DEMO_OUTPUT
        else if(getenv("DEMO_OUTPUT") != nullptr) {
            std::string _path = getenv("DEMO_OUTPUT");
            std::ofstream out(_path, std::ios_base::app);
            while(std::cin >> text) {
                print(text, out);
                out << std::endl;
            }
            out.close();
        } 
        //  Обработка 3-го приоритета: через конфигурационный файл demo.cfg
        else if(in_file.count("output")) {
            std::ofstream out(in_file["output"].as<std::string>(), std::ios_base::app);
            while(std::cin >> text) {
                print(text, out);
                out << std::endl;
            }
            out.close();
        } 
        //  Обработка 4-го приоритета: сборка программы по умолчанию
        else {
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
