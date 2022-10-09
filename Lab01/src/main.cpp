#include <any>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cmath>

int main(int argc, char **argv) {
    try{

    using namespace std;
    map <string,function<double(vector<double>)> > my_functions;
    my_functions["add"] = [](auto arg) {return arg.at(0) + arg.at(1);};
    my_functions["sin"] = [](auto arg) {return sin(arg.at(0));};
    my_functions["mod"] = [](auto arg) {return (int)arg.at(0) % (int)arg.at(1);};
    try {
        vector<string> text_args(argv, argv + 2);
        vector<string> num_args(argv+2, argv + argc);
        auto selected_f = text_args.at(1);
        std::vector<double> doubleVector(num_args.size());
        std::transform(num_args.begin(), num_args.end(), doubleVector.begin(), [](const std::string& val)
        {
            return std::stod(val);
        });
        auto s_function = my_functions.at(selected_f);
        auto result = s_function(doubleVector);
        cout<<"Wynik = "<< result;
    }catch (std::out_of_range aor) {
        cout << "Program przyjmuje jeden argument dla funkcji sin, oraz dwa dla funkcji add oraz mod.\n Sprobuj jeszcze raz podajac wymagane argumenty.";
    }

    }
    catch(std::logic_error le){
        std::cout << "Program przyjmuje jeden argument dla funkcji sin, oraz dwa dla funkcji add oraz mod.\n Sprobuj jeszcze raz podajac wymagane argumenty.";
    }
    return 0;
}
