#include <iostream>
#include <vector>
#include <functional>
#include <random>

std::random_device rd;
std::mt19937 mt_generator(rd());
using domain_t = std::vector<double>;



auto beale(double x, double y){
    std::vector<double> f_args = {1.5,2.25,2.625};
    double result = 0;
    int i =1;
    for(double arg : f_args){
        result += pow(arg - x +x*pow(y,i),2);
        i++;
    }
    return result;
}

double matyas(double x, double y){
    return 0.26*(pow(x,2)+pow(y,2)) - 0.48 * x * y;
}

domain_t resolver(const std::function<double(domain_t)> &f, double minimal_d, double maximal_d, int max_iterations) {
    std::uniform_real_distribution<double> dist(minimal_d, maximal_d);
    double current_x = dist(mt_generator);
    double current_y = dist(mt_generator);
    std::vector<double> correct = {current_x,current_y};
    auto result = beale(current_x,current_y);
    for(int i = 1; i<max_iterations;i++){
        current_x = dist(mt_generator);
        current_y = dist(mt_generator);
        auto temp_res = f({current_x,current_y});
        if (result>0){
            if(temp_res<result){
                correct = {current_x,current_y};
                result = temp_res;
            }
        }
        if (result<0){
            if(temp_res>result){
                correct = {current_x,current_y};
                result = temp_res;
            }
        }
        if (result == 0){
            break;
        }
    }
    return {correct[0],correct[1],result};
};
int main() {
    using namespace std;
    auto booth_f = [](domain_t x){return pow(x[0]+2*x[1]-7,2) + pow((2*x[0]+x[1]-5),2);};
    auto best_point = resolver(booth_f,-10,10,100000);
    cout<<"booth("<<best_point[0]<<","<<best_point[1]<<") = "<<best_point[2]<<std::endl;
    auto baele_f =[] (domain_t x) {return pow(1.5-x[0]+x[0] * x[1],2) + pow(2.25-x[0]+x[0] * pow(x[1],2),2) + pow(2.625 - x[0] + x[0] * pow(x[1],3),2);};
    auto best_point2 = resolver(baele_f,-4.5,4.5,100000);
    cout<<"baele("<<best_point2[0]<<","<<best_point2[1]<<") = "<<best_point2[2]<<std::endl;
    auto matyas_f =[] (domain_t x) {return 0.26 * (pow(x[0],2) + pow(x[1],2) - 0.48 * x[0] * x[1]);};
    auto best_point3 = resolver(matyas_f,-10,10,100000);
    cout<<"matyas("<<best_point3[0]<<","<<best_point3[1]<<") = "<<best_point3[2]<<std::endl;
    return 0;
}