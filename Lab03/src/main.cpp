#include <functional>
#include <iostream>
#include <list>
#include <optional>
#include <random>
#include <vector>
#include <cmath>
#include <time.h>
std::mt19937 mt_generator((std::random_device())());
using domain_t = std::vector<double>;
std::ostream &operator<<(std::ostream &o, domain_t &d) {
    o << d[0] << " " << d[1];
    return o;
}

auto booth_f = [](domain_t x){return pow(x[0]+2*x[1]-7,2) + pow((2*x[0]+x[1]-5),2);};
auto baele_f =[] (domain_t x) {return pow(1.5-x[0]+x[0] * x[1],2) + pow(2.25-x[0]+x[0] * pow(x[1],2),2) + pow(2.625 - x[0] + x[0] * pow(x[1],3),2);};
auto matyas_f =[] (domain_t x) {return 0.26 * (pow(x[0],2) + pow(x[1],2) - 0.48 * x[0] * x[1]);};

// Dziedzina = -5<= x,y <= 5; Minimum = 0:
// (3.0,2.0)
// (-2.805118,3.131312)
// (-3.779310,-3.28186)
// (3.584428,-1.848126)

auto himmelblaus_f = [] (domain_t x) {
    double left_inner = (pow(x[0],2) + x[1] - 11);
    double left = pow(left_inner,2);
    double right_inner = (x[0] + pow(x[1],2) - 7);
    double right= pow(right_inner,2); 
    std::cout<<x[0]<<std::endl;
    std::cout<<x[1]<<std::endl;
    double result = right + left;
    // double result = pow((pow(x[0],2) + x[1] + 11),2) + pow((x[0] + pow(x[1],2) + 7),2);
    return result;
};



auto holder_table_f = [] (domain_t x) { return - fabs(sin(x[0]) * cos(x[1]) * exp(fabs(1-(sqrt(pow(x[0],2) + pow(x[1],2)) / M_PI))));};


void print_task(domain_t entry_data, domain_t domain,double result){
    std::cout << "(x,y) = ("<< entry_data[0] << "," <<entry_data[1] << "); domain = "<<domain[0] << "<= x,y <= " << domain[1]<<std::endl;
    std::cout << "result = " << result<<std::endl;
}
double get_random_cud_variable(domain_t point){
    std::uniform_real_distribution<double> dist(0.0,1.0);
    double random_u = dist(mt_generator);
    double f_u;
    if (random_u<=point[0]){
        f_u = 0;
    }else if(point[0]< random_u && random_u <= point[1]){
        f_u = (point[0] - random_u)/(point[1] - random_u);
    }else{
        f_u = 1;
    }
    return f_u;
};

auto get_random_point = [](domain_t min_max) -> domain_t {
 std::uniform_real_distribution<double> distr(min_max[0],min_max[1]);
 return {distr(mt_generator), distr(mt_generator)};
 };
 
 auto get_close_points_random = [](domain_t p0) -> std::vector<domain_t> {
    std::normal_distribution<double> distr(0, 1);
    return {{p0[0] + distr(mt_generator), p0[1]+distr(mt_generator)}};
 };
double cud_v_exp(double best, double neighbour, int iter) {
    double upper = -1 * abs(neighbour - best);
    double lower = (double) 1 / iter;
    double result = exp(upper / lower);
    return result;
}

domain_t simulated_annealing(domain_t domain,const std::function<double(domain_t)> &f,int iterator){
    std::uniform_real_distribution<double> dist(domain[0], domain[1]);
    domain_t best_point = get_random_point(domain);
    double cud_v = get_random_cud_variable(best_point);
    for (int i = 0; i<iterator;i++){
        auto close_points = get_close_points_random(best_point);
         auto best_neighbour = *std::min_element(close_points.begin(), close_points.end(),[f](auto a, auto b) { return f(a) > f(b); });
        if(f(best_neighbour)<=f(best_point)){
            best_point = best_neighbour;
        }
        else{
            if(cud_v < cud_v_exp(f(best_point),f(best_neighbour),i)){
                best_point = best_neighbour;
            }
        }
    }
    return best_point;

}




int main() {
    // Dziedzina = -10<= x,y <= 10; Minimum = -19.2085:
// (8.05502,9.66459)
// (-8.05502,9.66459)
// (8.05502,-9.66459)
// (-8.05502,-9.66459)
    // domain_t res = simulated_annealing({-10,10},holder_table_f,10);
    // std::cout<<res[0]<<" "<< res[1]<<" = "<<holder_table_f(res)<<std::endl;
    domain_t res2 = simulated_annealing({-10,10},booth_f,100000);
    std::cout<<res2[0]<<" "<< res2[1]<<std::endl;
    //1,3
    domain_t res3 = simulated_annealing({-4.5,4.5},baele_f,100000);
    std::cout<<res3[0]<<" "<< res3[1]<<std::endl;
    //3,0.5
    return 0;
}