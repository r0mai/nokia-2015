#include <iostream>
#include <valarray>
#include <unordered_set>
#include <iomanip>
#include <functional>
#include <vector>

using ValArr = std::valarray<int>;

ValArr input = {666, 1017, 1167, 1167, 1514, 1518, 1518, 1668, 1865, 2015, 2015, 2019, 2366, 2366, 2516, 2867};

//std::unordered_set<int> unique_input{std::begin(input), std::end(input)};
std::vector<int> unique_input{std::begin(input), std::end(input)};

const int N = std::sqrt(input.size());

const int need = input.sum() / N;

std::unordered_multiset<int> not_used_yet{std::begin(input), std::end(input)};

ValArr result(N*N);

int globalCounter = 0;

void print()
{
    std::clog << "\033[0;0H";
    for(int x = 0; x < N; ++x)
    {
        for(int y = 0; y < N; ++y)
        {
            std::clog << std::setw(8) << std::left << result[x*N+y];
        }
        std::clog << "\t" << static_cast<ValArr>(result[std::slice(N*x, N, 1)]).sum() << std::endl;
    }
    std::clog << std::endl;
    for(int y = 0; y < N; ++y)
    {
        std::clog << std::setw(8) << std::left << static_cast<ValArr>(result[std::slice(y, N, N)]).sum();
    }
    std::clog << std::endl << std::endl;
    std::clog << "osszeg: " << need << std::endl;

    std::clog << "eddig: " << globalCounter << std::endl;
}

std::function<void()> printer{[]{}};


void genX(int x, int y);

void genY(int x, int y)
{
    if(x != N)
    {
        for(int i : unique_input)
        {
            auto it = not_used_yet.find(i);
            if(it != std::end(not_used_yet))
            {
                not_used_yet.erase(it);
                result[x*N + y] = i;

                genY(x+1, y);

                not_used_yet.insert(i);
            }
        }
    }
    else if(static_cast<ValArr>(result[std::slice(y, N, N)]).sum() == need)
    {
        genX(y+1, y+1);
    }
}

void genX(int x, int y)
{
    if(y != N)
    {
        for(int i : unique_input)
        {
            auto it = not_used_yet.find(i);
            if(it != std::end(not_used_yet))
            {
                not_used_yet.erase(it);
                result[x*N + y] = i;

                genX(x, y+1);

                not_used_yet.insert(i);
            }
        }
    }
    else if(x == N)
    {
        ++globalCounter;
        printer();
    }
    else if(static_cast<ValArr>(result[std::slice(N*x, N, 1)]).sum() == need)
    {
        genY(x+1, x);
    }
}

int main(int argc, char* argv[])
{
    if(argc >= 2) printer = print;
    std::sort(std::begin(unique_input), std::end(unique_input));
    unique_input.erase( std::unique(std::begin(unique_input), std::end(unique_input)), std::end(unique_input) );

    std::clog << "kb 5 mp, ha megadsz parametert,\n akkor latszik a backtrack..." << std::endl;
    genX(0,0);
    std::cout << globalCounter << std::endl;
    return 0;
}
