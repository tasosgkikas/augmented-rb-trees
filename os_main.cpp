
#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include <cmath>
#include "order_statistic.cpp"

int main()
{
    OS::Tree T;

    /* setup for auto insertion/deletion */
    unsigned N = pow(2,7);
    std::vector<key_t> elements;
    unsigned seed = std::chrono::system_clock \
        ::now().time_since_epoch().count();
    std::default_random_engine generator(seed);


    std::string commands = "\n"
    " Commands\n"
    " Exit   : 0\n"
    " Insert : 1\n"
    " Delete : 2\n"
    " Search : 3\n"
    " Print  : 4\n"
    " Graph  : 5\n"
    " Height : 6\n"
    " Popul. : 7\n"
    " Select : 8\n"
    " Rank   : 9\n"
    " Destroy: 10";
    
    while (true)
    {
        std::cout << "\n (ENTER to show commands)\n Give your command: ";

        std::string input;
        std::cin.sync();
        std::getline(std::cin, input);
        
        if (input.empty()){
            std::cout << commands;
            continue;
        }

        // the rest if input != '\n'
        int command = std::stoi(input);

        if (command == 0){
            std::cout << "\n\tGoodbye!\n";
            break;
        }
        else if (command == 1){
            /* auto (random) insertion */
            std::cout << std::endl << " RANDOM INSERTION";
            elements.resize(N);
            std::iota(elements.begin(), elements.end(), 1);
            std::shuffle(elements.begin(), elements.end(), generator);
            for (auto element : elements) T.insert(element, false);
            
            /* user-input insertion */
            // key_t num;
            // std::cout << " Give an element to insert: ";
            // std::cin >> num;
            // T.insert(num);
        }
        else if (command == 2){
            /* auto random deletion */
            std::iota(elements.begin(), elements.end(), 1);
            std::shuffle(elements.begin(), elements.end(), generator);
            // for (auto iter = elements.begin(); iter != elements.end(); iter++)
            //     std::cout << (iter == elements.begin() ? " " : ", ") << *iter;

            /* one or n elements each time */
            unsigned n;
            std::cout << " Give number of elements to delete: ";
            std::cin >> n;
            for (int i = 0; i < n && elements.size() > 0; i++)
            {
                T.remove(elements.back(), false);
                elements.pop_back();
            }

            /* user-input deletion */
            // key_t num;
            // std::cout << " Give an element to delete: ";
            // std::cin >> num;
            // T.remove(num);
        }
        else if (command == 3){
            key_t num;
            std::cout << " Give an element to search: ";
            std::cin >> num;
            T.search(num);
        }
        else if (command == 4){
            T.showNodes();
            // T.inorderPrint();
            // for (auto iter = elements.begin(); iter != elements.end(); iter++)
            //     std::cout << (iter == elements.begin() ? " " : ", ") << *iter;
        }
        else if (command == 5){
            T.graph();
            // breadth_first_traversal(T.root);
        }
        else if (command == 6){
            std::cout << " " << T.height();
            // for (auto node : T.inorderAccess())
            //     std::cout << " " << node->key << ": " << T.height(node) << std::endl;
        }
        else if (command == 7){
            std::cout << " " << T.population();
        }
        else if (command == 8){
            unsigned i;
            std::cout << " Give i to find i-th smallest element: ";
            std::cin >> i;
            std::cout << " " << i << "th OS: " << T.select(i) << std::endl;
        }
        else if (command == 9){
            key_t key;
            std::cout << " Give key to find the rank: ";
            std::cin >> key;
            std::cout << " rank(" << key << ") = " << T.rank(key) << std::endl;
        }
        else if (command == 10){
            T.remove(false);
            std::cout << " Tree cleaned";
        }
        else std::cout << "\n Please enter one of the following\n" << commands;
    }
    
    return 0;
}