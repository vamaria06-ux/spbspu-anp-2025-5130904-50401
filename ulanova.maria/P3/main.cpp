#include <iostream>

int main(int argc, char ** argv) {
    namespace ul = ulanova;
    ul:: size_t rows=0, cols=0;
    //Проверка параметров входных данных
    if (argc < 4) {
        std::cerr << "Not enough arguments\n";
        return 1;
    } else if (argc > 4){
        std::cerr << "Too many arguments\n";
        return 1;
    }
    try {
        if( std::stoi(argv[1])!='1' && std::stoi(argv[1])!='2'){
            std::cerr << "Wrong arguments \n";
            return 1;
        }
    } catch (const std::invalid_argument& e){
        std::cerr << "First parametr is not a number \n";
        return 1;
    }
    //Открываем файл
    std::ifstream input(argv[2]);
    if (!input) {
        std::cerr << "Cannot open input file\n";
        return 1;
    }
}
