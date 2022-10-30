#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

struct light_up {
    int size;
    std::vector<int> board;
    std::vector<int> get_row(int x){
        int index = 0;
        std::vector<int> result(size);
        for(int i = (x*size);i<(x+1)*size;i++){
            result[index] = board.at(i);
            index++;
        }
        return result;
    }
    void change_field(int field_index,int value){
        board[field_index] = value;
    }
    std::vector<int> get_column(int x){
        std::vector<int> result(size);
        int index = 0;
        for(int i=x;i<=(size*size-(size-x));i=i+size){
            result[index] = board.at(i);
            index++;
        }
        return result;
    }
};

std::ostream &operator<<(std::ostream &o, const light_up &puzzle){
    using namespace std;
    for (int y = 0; y<puzzle.size;y++){
        for (int x = 0; x<puzzle.size;x++) {
            auto &val = puzzle.board[y*puzzle.size+x];
            string display_ch;
            switch (val) {
                case -4:
                    display_ch = 'o';
                    break;
                case -3:
                    display_ch = '#';
                    break;
                case -2:
                    display_ch = '*';
                    break;
                default:
                    display_ch = to_string(val);
                    break;

            }
            o<< "\t" <<display_ch;
        }
        o <<endl;
    }

    return o;
}
bool check_if_bulb_is_alone(std::vector<int> row){
    using namespace std;
    bool is_bulb = false;
    for (int field:row) {
        if(is_bulb && field==-4){
            return false;
        }else if(is_bulb && field>=0){
            is_bulb = false;
        }
        if (field==-4){
            is_bulb = true;
        }

    }
    return true;
}
bool check_if_bulbs_are_alone(light_up puzzle){
    for(int i=0;i<puzzle.size;i++){
        bool bulbs_in_row = check_if_bulb_is_alone(puzzle.get_row(i));
        bool bulbs_in_columns = check_if_bulb_is_alone(puzzle.get_column(i));
        if(bulbs_in_row == false or bulbs_in_columns == false){
            return false;
        }
    }
    return true;
}

bool check_field_neighbors(int field,light_up puzzle,int field_index){
    int board_size = puzzle.size;
    int number_of_neighbors = field;
    if(field_index<board_size){
        // std::cout<<"up niedostepne"<<std::endl;
    }else{
        if(puzzle.board[field_index-board_size] == -4){
            number_of_neighbors--;
        }
    }
    if(field_index + board_size>=board_size*board_size){
        // std::cout<<"down niedostepne"<<std::endl;
    }else{
        if(puzzle.board[field_index+board_size] == -4){
            number_of_neighbors--;
        }
    }
    if(field_index % board_size == 0 ){
        // std::cout<<"left niedostepne"<<std::endl;
    }else{
        if(puzzle.board[field_index-1] == -4){
            number_of_neighbors--;
        }
    }
    if(field_index% (board_size) == board_size-1 ){
        // std::cout<<"right niedostepne"<<std::endl;
    }else{
        if(puzzle.board[field_index+1] == -4){
            number_of_neighbors--;
        }
    }
    // std::cout<<number_of_neighbors;
    if(number_of_neighbors==0 || field==5){
        return true;
    }else{
        return false;
    }

}

bool check_if_bulbs_are_next_to_squares(light_up puzzle){
    bool result;
    for(int i = 0;i<puzzle.board.size();i++){
        int field = puzzle.board[i];
        if(field>0){
            result = check_field_neighbors(field,puzzle,i);
            // std::cout<<"Dla kwadratu o indexie: "<< i<<" o wymaganiu: "<<field <<" wynik sasiadow (true/false) = "<<result<<std::endl;
            if (!result){
                return false;
            }
        }

    }
    return true;
}

bool check_if_all_fields_are_light_up(light_up puzzle){
    for(int field : puzzle.board){
        if(field == -3){
            // std::cout<<"Pozostały niezapalone pola"<<std::endl;
            return false;
        }
    }
    return true;
}
bool evaluate_puzzle(light_up &basic_board,light_up &puzzle){
    for(int i = 0; i<basic_board.size * basic_board.size;i++){
        if (basic_board.board[i]>=0){
        puzzle.board[i] = basic_board.board[i];
        }
    }
    if(check_if_bulbs_are_alone(puzzle) and
        check_if_bulbs_are_next_to_squares(puzzle) and
        check_if_all_fields_are_light_up(puzzle)){
        std::cout<<"Puzzle poprawne"<<std::endl;
        return true;
    }else{
        std::cout<<"Puzzle niepoprawne"<<std::endl;
        return false;
    }
}

int check_row(int i,int puzzle_size){
    return i/puzzle_size;
}
int check_column(int i,int puzzle_size){
    return i%puzzle_size;
}

void light_up_field(light_up &puzzle,int iterator){
        if (puzzle.board[iterator] == -3){
            puzzle.change_field(iterator,-2);
        }

}
void light_row(light_up &puzzle,int bulb_index){
    // std::cout<<"########\n"<<puzzle;
    int row = check_row(bulb_index,puzzle.size);
    for(int i = bulb_index; i<puzzle.size*(row+1);i++){
        light_up_field(puzzle,i);
        if (puzzle.board[i]>=0){
            break;
        }
    }
    for(int i = bulb_index;i>=puzzle.size*row;i--){
        light_up_field(puzzle,i);
        if (puzzle.board[i]>=0){
            break;
        }
    }
    // std::cout<<"########\n"<<puzzle;
}
void light_column(light_up &puzzle,int bulb_index){
    int column = check_column(bulb_index,puzzle.size);
    int i = bulb_index;
    for(int i = bulb_index;i<(puzzle.size*puzzle.size) - puzzle.size + column;i=i+puzzle.size){
        light_up_field(puzzle,i);
        if (puzzle.board[i]>=0){
            break;
        }
    }
    for(int i = bulb_index;i>column;i=i-puzzle.size){
        light_up_field(puzzle,i);
        if (puzzle.board[i]>=0){
            break;
        }
    }
}
void change_to_light_up(light_up &puzzle){
    int i = 0;
    for (auto field : puzzle.board){
        if (field == -4){
            light_row(puzzle,i);
            light_column(puzzle,i);
        }
        i++;
    }
}
int main() {
    using namespace std;
    // -4 = bulb
    // -3 = nieoświetlone
    // -2 = oświetlone
    //  0,1,2,3,4 = czarne z liczba
    // 5 = czarny bez liczby
    light_up puzzle {
            7,
            {
                    -3,-3,5,-3,-3,-3,-3,
                    -3,1,-3,-3,-3,2,-3,
                    -3,-3,-3,-3,-3,-3,5,
                    -3,-3,-3,1,-3,-3,-3,
                    2,-3,-3,-3,-3,-3,-3,
                    -3,4,-3,-3,-3,2,-3,
                    -3,-3,-3,-3,5,-3,-3
            }};
    light_up puzzle_resolve {
        7,
        {
            -2,-4,5,-2,-2,-4,-2,
            -2,1,-2,-4,-2,2,-4,
            -4,-2,-2,-2,-2,-2,5,
            -2,-2,-2,1,-4,-2,-2,
            2,-4,-2,-2,-2,-2,-2,
            -4,4,-4,-2,-2,2,-4,
            -2,-4,-2,-2,5,-4,-2
        }
    };
    light_up puzzle_resolve_without_light {
        7,
        {
            -3,-4,5,-3,-3,-4,-3,
            -3,1,-3,-4,-3,2,-4,
            -4,-3,-3,-3,-3,-3,5,
            -3,-3,-3,1,-4,-3,-3,
            2,-4,-3,-3,-3,-3,-3,
            -4,4,-4,-3,-3,2,-4,
            -3,-4,-3,-3,5,-4,-3
        }
    };
    light_up test = {4,{
        2,-3,-3,-3,
        -3,-3,-3,-3,
        -3,-3,-3,2,
        5,-3,-3,-3,
        -3,-3,-3,-3
    }};

    light_up resolve_test = {4,{
        -3,-4,-3,-3,
        -4,-3,-3,-3,
        -3,-3,-4,-3,
        -3,-3,-3,-4
    }};
    // change_to_light_up(resolve_test);
    // evaluate_puzzle(test,resolve_test);
    // cout<<test;
    int arr[16] = { };
    std::fill_n(arr, 16, -3);
    // arr[1] = -4;
    // arr[4] = -4;
    // arr[10] = -4;
    // arr[15] = -4;
    // std::vector<int> v(arr, arr + sizeof arr / sizeof arr[0]);
    // light_up test_111 = {4,v};
    // change_to_light_up(test_111);
    // evaluate_puzzle(test,test_111);
    // cout<<test_111;
    bool result_find = false;
    for (int i=15; i>=0;i--){
        arr[i] = -4;

        sort(arr, arr + 3);

    do { 
        std::vector<int> v(arr, arr + sizeof arr / sizeof arr[0]);
        light_up xzy = {4,v};
        change_to_light_up(xzy);
        if(evaluate_puzzle(test,xzy) == true){
            cout<<xzy;
            result_find = true;
            break;
        }
        for(int each : v){
            cout << each;
        }
        cout<<"\n";
    } while (next_permutation(arr, arr + 16));
    if (result_find == true){
        
        break;
    }
    }

    return 0;
}