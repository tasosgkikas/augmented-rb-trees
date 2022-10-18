
#include <iostream>
#include <ios>
#include <limits>
#include <numeric>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include "interval.cpp"

int main()
{
    using key_t = Interval::key_t;
    Interval::Tree T;

    /* setup for auto insertion/deletion */
    key_t max_width = 8;
    key_t max_high = 200;
    key_t max_low = max_high - max_width;

    using unif = std::uniform_int_distribution<>;

    std::vector<key_t> lows;
    std::vector<Interval::interval> intervals;
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
    " Time   : 6\n"
    " Height : 7\n"
    " Overlap: 8\n"
    " Count  : 9\n"
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

        int command = std::stoi(input);

        if (command == 0){
            // for (auto interval : intervals) delete interval;
            std::cout << "\n\tGoodbye!\n";
            break;
        }
        else if (command == 1){
            /* auto insertion */
            unsigned N;
            std::cout << " Give number of elements" \
                " to insert in range [1, " << max_high << "]: ";
            std::cin >> N;

            if (lows.size() == 0)
            {
                lows.resize(max_high);
                std::iota(lows.begin(), lows.end(), 0);
                std::shuffle(lows.begin(), lows.end(), generator);
                lows.resize(N);

                for (auto low : lows)
                {
                    unif high_dist(low, low + max_width);
                    key_t high = high_dist(generator);

                    intervals.emplace_back(low, high);
                    T.insert(intervals.back());
                }
            }
            else
            {
                unsigned size = intervals.size();
                while (intervals.size() < size + N)  // intervals initialization
                {
                    unif low_dist(0, max_low);
                    key_t low = low_dist(generator);

                    unif high_dist(low, low + max_width);
                    key_t high = high_dist(generator);

                    intervals.emplace_back(low, high);
                    if (!T.insert(intervals.back())) intervals.pop_back();
                }
            }
            
            /* user-input insertion */
            // key_t num;
            // std::cout << " Give an element to insert: ";
            // std::cin >> num;
            // T.insert(num);
        }
        else if (command == 2){
            /* auto deletion */
            std::shuffle(
                intervals.begin(), intervals.end(),
                std::default_random_engine(seed)
            );
            
            unsigned N;
            std::cout << " Give number of elements to delete: ";
            std::cin >> N;
            
            unsigned size = intervals.size();
            while (intervals.size() > size - N) {
                T.remove(intervals.back());
                intervals.pop_back();
            }

            /* user-input deletion */
            // key_t num;
            // std::cout << " Give an \"low high\" interval to delete: ";
            // std::cin >> num;
            // T.remove(num);
        }
        else if (command == 3){
            key_t low, high;
            std::cout << " Give \"low high\" interval to search: ";
            std::cin >> low >> high;
            Interval::interval interval(low, high);
            T.search(interval);
        }
        else if (command == 4){
            T.showNodes();
            // T.inorderPrint();
            // for (auto node : T.inorderAccess())
            //     std::cout << " " << node;
        }
        else if (command == 5){
            T.graph();
            // breadth_first_traversal(T.root);
        }
        else if (command == 6){
            T.timeline();
        }
        else if (command == 7){
            std::cout << " " << T.height();
            // for (auto node : T.inorderAccess())
            //     std::cout << " " << node << ": " << T.height(node) << std::endl;
        }
        else if (command == 8){
            /* auto search */
            unif low_dist(1, max_low);
            key_t low = low_dist(generator);

            unif high_dist(low, low + max_width);
            key_t high = high_dist(generator);
            
            Interval::interval interval(low, high);
            T.overlap(interval);

            /* user-input search */
            // key_t low, high;
            // std::cout << " Give \"low high\" interval to find overlap: ";
            // std::cin >> low >> high;
            // Interval::interval interval(low, high);
            // T.overlap(interval);
        }
        else if (command == 9){
            std::cout << " " << T.population();
        }
        else if (command == 10){
            T.remove(false);
        }
        else std::cout << "\n Please enter one of the following\n" << commands;
    }
    
    return 0;
}