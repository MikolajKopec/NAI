#include <iostream>
#include <vector>
#include <functional>
#include <random>
std::random_device rd;
std::mt19937 mt_generator(rd());
using domain_t = std::vector<double>;

auto himmelblaus_f = [] (domain_t x) {
    double left_inner = (pow(x[0],2) + x[1] - 11);
    double left = pow(left_inner,2);
    double right_inner = (x[0] + pow(x[1],2) - 7);
    double right= pow(right_inner,2);
    double result = right + left;
    // double result = pow((pow(x[0],2) + x[1] + 11),2) + pow((x[0] + pow(x[1],2) + 7),2);
    return result;
};


auto genetic_algorithm = [](
        auto initial_population, auto fitness, auto term_condition,
        auto selection, double p_crossover,
        auto crossover, double p_mutation,  auto mutation) {
    using namespace std;
    uniform_real_distribution<double> uniform(0.0,1.0);
    auto population = initial_population;
    vector<double> population_fit = fitness(population);
    while (!term_condition(population,population_fit)) {
        auto parents_indexes = selection(population_fit);
        decltype(population) new_population;
        for (int i = 0 ; i < parents_indexes.size(); i+=2) {
            decltype(initial_population) offspring = {population[i],population[i+1]};
            if (uniform(mt_generator) < p_crossover) {
                offspring = crossover(offspring);
            }
            for (auto chromosome : offspring) new_population.push_back(chromosome);
        }
        for (auto & chromosome : new_population) {
            chromosome = mutation(chromosome,p_mutation);
        }
        population = new_population;
        population_fit = fitness(population);
    }
    return population;
};

using chromosome_t = std::vector<int>;
chromosome_t create_chromosome(int size){
    std::uniform_int_distribution<int> dist(0,1);
    double random = 0;
    chromosome_t result;
    for(int i=0; i<size;i++){
        random = dist(mt_generator);
        result.push_back(random);
    }
    return result;
}
struct chromosome {
    int chromosome_size;
    chromosome_t left_part;
    chromosome_t right_part;
public:
    chromosome(int size);
};
chromosome::chromosome(int size){
    chromosome_size = size;
    left_part = create_chromosome(size/2);
    right_part = create_chromosome(size/2);

}

std::ostream &operator<<(std::ostream &o,const chromosome &chromosome){

    for(auto x : chromosome.left_part){
        o<<x;
    }
    o<<std::endl;
    for(auto x : chromosome.right_part){
        o<<x;
    }
    o<<std::endl;
}
using phenotype_t = std::vector<double>;
using population_t = std::vector<chromosome>;


double bin_to_double(std::vector<int> chromosome,int integer_part_lenght){
    // for(auto z : chromosome){
        // std::cout<<z;
    // }
    // std::cout<<" = in \n";
    int sign = -1;
    double integer_part = 0;
    long double fractional_part = 0;
    if (chromosome[0] == 0 ){
        sign = 1;
    }
    for(int i = 1;i<chromosome.size();i++){
        if(i<=integer_part_lenght){
            // std::cout<<"int_"<<chromosome[i]<<std::endl;
            if(chromosome[i] == 1){
            integer_part = integer_part + (pow(2,i-1));
            }
        }if(i>=integer_part_lenght+1){
            if(std::to_string(fractional_part).length()>10){
                break;
            }
            if(chromosome[i] == 1){
                fractional_part = fractional_part + (pow(2,i-integer_part_lenght+1));
            }
        }
    }
    int temp_fract = int(fractional_part);
    int length = 1;
    while(temp_fract/= 10){
//         std::cout<<temp_fract<<std::endl;
        length++;
    }
//        std::cout<<"fractional_part: "<<fractional_part<<"l:"<<length<<std::endl;
    fractional_part = fractional_part * pow(10,(-length));
//     std::cout<<"integer_part: "<<integer_part<<std::endl;
//     std::cout<<"sign: "<<sign<<std::endl;
//     std::cout<<"fractional_part: "<<fractional_part<<std::endl;
    return sign * (integer_part + fractional_part);

}

phenotype_t decode_f(chromosome chromosome){
    double x = bin_to_double(chromosome.left_part,3);
    double y = bin_to_double(chromosome.right_part,3);
    return {x,y};
}

std::vector<double> fintess_function(population_t pop,domain_t domain){
    std::vector<double> fitness_result;
    for(chromosome chromosome : pop){
        bool not_in_domain = false;
        long double res = 0;
        phenotype_t phenotype =decode_f(chromosome);
//        std::cout<<"x"<<phenotype[0]<<" y"<<phenotype[1]<<std::endl;
        for (auto xy : phenotype) {
            if(xy < domain[0] || xy > domain[1]){
                not_in_domain = true;
            }
        }
        if(not_in_domain){
            res = 1000;
        }else{
            res = himmelblaus_f(phenotype);
            if (res<0){
                res = res*(-1);
            }
        }
        std::cout<<"Ocena rozwizania to "<<1000 - res<<"\n";
        fitness_result.push_back(1000 - res);
        }
    return fitness_result;
}
//chromosome_t  one_point_crossing(std::vector<chromosome_t> parents) {
//    chromosome_t child;
//    std::uniform_int_distribution<int> dist(0,100);
//    int random = dist(mt_generator);
//    for ()parents[0]
//    return child;
//}

std::vector<int> selection_empty(std::vector<double> fitnesses) {
    return {};
}
std::vector<chromosome_t > crossover_empty(std::vector<chromosome_t > parents) {
    return parents;
}
chromosome_t mutation_empty(chromosome_t parents, double p_mutation) {
    return parents;
}
auto goldstine_f = [](domain_t x){
    return (1 + pow(x[0] +x[1] +1,2) * (19-14*x[0]+3*pow(x[0],2) - 14*x[1] + 6*x[0] * x[1] + 3*pow(x[1],2))) * (30 +pow(2*x[0] - 3*x[1],2) * (18-32 * x[0] +12 * pow(x[0],2) + 48 * x[1] - 36 * x[0] *x[1] + 27 * pow(x[1],2)));};





chromosome one_point_crossover(chromosome chromosome){
    std::uniform_int_distribution<int> dist (0,chromosome.chromosome_size/2-1);
    auto start = chromosome.left_part.begin();
    int slice_point = dist(mt_generator);
    auto end = chromosome.left_part.begin() + slice_point;
    std::vector<int> helper(slice_point);
    copy(start,end,helper.begin());
    std::cout<<slice_point<<std::endl;
    std::cout<<chromosome<<std::endl;
    // for(auto x : helper){
    //     std::cout<<x;
    // }
    for(int i = 0;i=slice_point;i++){
        std::swap(chromosome.left_part[i],chromosome.right_part[i]);
    }
    std::cout<<chromosome<<std::endl;
    return chromosome;
}

int main() {
    using namespace std;
//    population_t population = {{1,0,1,0,1,0,1}, {1,0,1,0,1,0,1}};
//    auto result = genetic_algorithm(population,
//                                    fintess_function,
//                                    [](auto a, auto b){return true;},
//                                    selection_empty, 1.0,
//                                    crossover_empty,
//                                    0.01, mutation_empty);
//    for (chromosome_t chromosome: result) {
//        cout << "[";
//        for (int p: chromosome) {
//            cout << p;
//        }
//        cout << "] ";
//    }
//    cout << endl;
    chromosome_t my_geno(100+((2440%10) * 2));
    population_t pop;
    chromosome chromosome1(100);
    chromosome chromosome2(100);
    chromosome chromosome3(100);
    chromosome chromosome4(100);
    // pop.push_back(chromosome1);
    // pop.push_back(chromosome2);
    // pop.push_back(chromosome3);
    // pop.push_back(chromosome4);
    one_point_crossover(chromosome1);
    // phenotype_t phenotype = decode_f(my_geno);
    // std::vector<double> fittnes_results_for_pop = fintess_function(pop,{-5,5});
    // std::cout<<int(himmelblaus_f({3,2});
    // bin_to_double({0,1,1,0,0,1,1,1,1,0,1,1,0,1,1});
    // cout<<"("<<phenotype[0]<<","<<phenotype[1]<<")";
    return 0;
}