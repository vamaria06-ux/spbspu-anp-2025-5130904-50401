#include <iostream>

int main(int argc, char ** argv) {
    if (argc < 4) {
        std::cerr << "Not enough arguments\n";
        return 1;
    } else if (argc > 4){
        std::cerr << "Too many arguments\n";
        return 1;
    }
    try {
        if( argv[1]!='1' && argv[1]!='2'){
            std::cerr << "Wrong arguments \n";
            return 1;
        }
    } catch (const std::invalid_arguments& e){
        std::cerr << "First parametr is not a number \n";
        return 1;
    }
}
