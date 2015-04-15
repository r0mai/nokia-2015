#include <iostream>
#include <functional>
#include <vector>
#include <iomanip>
#include <iterator>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <fstream>


using namespace std::placeholders;
struct PairHash {
    template <class T, class U>
    std::size_t operator()(const std::pair<T, U> &x) const
    {
        return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
    }
};

struct TupleHash
{
    template<class T, class...U>
    std::size_t operator()(const std::tuple<T,U...>& t) const
    {
        return this->operator()(t, std::make_index_sequence<sizeof...(U)>());
    }

    template<class T, class...U, std::size_t ...ints>
    std::size_t operator()(const std::tuple<T,U...>& t, std::index_sequence<ints...>&&) const
    {
        return this->operator()(std::hash<U>()(std::get<ints>(t))... );
    }

    template<class ... Ts>
    std::size_t operator()(std::size_t res, Ts&& ... x) const
    {
        return res ^ this->operator()(std::forward<Ts>(x)...);
    }

    std::size_t operator()() const
    {
        return 0;
    }
};


using Field = char;
using SizeType = std::size_t;


using Word = std::string;
using Words = std::vector<Word>;

template<class T>
using RefWr = std::reference_wrapper<T>;

using Coord = std::pair<SizeType, SizeType>;
using WordIndex = std::tuple< SizeType, SizeType, SizeType >;

using Possibilities = std::unordered_map< Coord , std::unordered_set<char>, PairHash >;

// -2 NOT IN GAME
// -1 NOT SURE
// 0-9 SOLUTION

struct SmartChar
{
    char rw;
    struct WordConnect
    {
        WordIndex wi;
        Coord c;
        std::unordered_set< char > possibles;
    };
    std::vector<WordIndex> words;
    std::vector<WordIndex> word_in;
    std::unordered_set< char > possibles;

    SmartChar(char rw): rw(rw){}

    void add(const WordIndex& wi, const Coord& c)
    {
        words.push_back(wi);
        word_in.push_back(WordIndex{c.first, c.second, std::get<2>(wi)});
    }

    void make(Possibilities& pb)
    {
        if(word_in.size())
        {
            possibles = pb[Coord{std::get<0>(word_in[0]), std::get<1>(word_in[0])}];

            if(word_in.size() > 1)
            {
                auto& r = pb[Coord{std::get<0>(word_in[1]), std::get<1>(word_in[1])}];
                for(auto it = std::begin(possibles); it != std::end(possibles);)
                {
                    if(!r.count(*it))
                    {
                        it = possibles.erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                }
            }
        }
    }

    operator char()
    {
        return rw;
    }
};
using Board = std::vector<std::vector<SmartChar>>;

struct GuessWord : std::vector< RefWr<SmartChar> >
{

};

using GuessWords = std::unordered_map< WordIndex , GuessWord, TupleHash >;

Board readBoard()
{
    std::ifstream i("task4-1.txt");
    if(!i.is_open())
        throw std::logic_error("figyeljman a mappara kotsog, nincs itt az input");

    Board res;

    std::string str;
    std::vector<SmartChar> smc;
    while(std::getline(i, str))
    {
        if(str.size())
        {
            smc.clear();
            for(char c : str)
            {
                smc.push_back(-1-(c-'0'));
            }
            res.push_back(std::move(smc));
        }
    }

    return std::move(res);
}

Words readWords()
{
    std::ifstream i("task4-2.txt");
    if(!i.is_open())
        throw std::logic_error("figyeljman a mappara kotsog, nincs itt az input");
    return {std::istream_iterator<std::string>(i), std::istream_iterator<std::string>()};
}

GuessWords makeGuessWords(Board& b, Possibilities& p)
{
    GuessWords res;
    const SizeType x = b.size();
    const SizeType y = b[0].size();
    for(SizeType i = 0; i < x; ++i)
    {
        for(SizeType j = 0; j < y; ++j)
        {
            if(b[i][j] == -2)
                continue;

            if( ( i == 0 || b[i-1][j] == -2 ) && (i < x-1 && b[i+1][j] != -2) )
            {
                WordIndex index{i,j,0};
                auto& word = res[index];
                for(SizeType ii = i; ii < x && b[ii][j] != -2; ++ii)
                {
                    word.push_back(std::ref(b[ii][j]));
                }
                for(SizeType ii = i; ii < x && b[ii][j] != -2; ++ii)
                {
                    b[ii][j].add(index, Coord{word.size(), ii-i});
                }
            }
            if( ( j == 0 || b[i][j-1] == -2 ) && (j < y-1 && b[i][j+1] != -2) )
            {
                WordIndex index{i,j,1};
                auto& word = res[index];
                for(SizeType jj = j; jj < x && b[i][jj] != -2; ++jj)
                {
                    word.push_back(std::ref(b[i][jj]));
                }
                for(SizeType jj = j; jj < x && b[i][jj] != -2; ++jj)
                {
                    b[i][jj].add(index, Coord{word.size(), jj-j});
                }
            }
        }
    }
    for(SizeType i = 0; i < x; ++i)
    {
        for(SizeType j = 0; j < y; ++j)
        {
            b[i][j].make(p);
        }
    }
    return std::move(res);
}

Possibilities makePossibilities(const Words& w)
{
    Possibilities res;
    for(auto& wo : w)
    {
        SizeType s = wo.size();
        for(SizeType i = 0; i < s; ++i)
        {
            res[std::make_pair(s,i)].insert(wo[i]);
        }
    }

    return res;
}

void printBoard(Board& board)
{
    std::cout << "Board is: " << std::endl;
    for(SizeType i = 0; i < board.size(); ++i)
    {
        for(SizeType j = 0; j < board[i].size(); ++j)
        {
            std::cout << std::setw(3) << std::right << static_cast<int>(board[i][j]);
            //*
            std::stringstream ss;
            ss << "{";
            std::copy(std::begin(board[i][j].possibles),
                        std::end(board[i][j].possibles),
                        std::ostream_iterator<char>(ss, ","));
            ss << "}";

            std::cout << std::setw(12) << ss.str();//*/
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
void printGuesses(const GuessWords& guesses)
{
    std::cout << "Guesses is: " << guesses.size() << std::endl;
    for(auto& guess: guesses)
    {
        std::cout << std::setw(3) << std::right << std::get<0>(guess.first)
                  << std::setw(3) << std::right << std::get<1>(guess.first) << " " << (std::get<2>(guess.first) ? "left" : "down") << " : ";
        for(SizeType j = 0; j < guess.second.size(); ++j)
        {
            std::cout << std::setw(3) << std::right << static_cast<int>(static_cast<SmartChar>(guess.second[j]).rw);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
void printPossibilities(Possibilities& possibilities)
{
    std::cout << "Possibilities:" << std::endl;
    for(auto st : possibilities)
    {
        std::cout << st.first.first << " " << st.first.second << " : ";
        //std::copy(st.second.begin(), st.second.end(), std::ostream_iterator<char>(std::cout, " "));
        std::endl(std::cout);
    }
}



namespace NEW
{
    using Field = char;
    using SizeType = std::size_t;
    template<class T>
    using RefWr = std::reference_wrapper<T>;

    struct Word;
    struct SmartChar;


    using GuessWordIndex = std::tuple< SizeType, SizeType, SizeType >; // x, y, dir
    using CharPosition = std::tuple< SizeType, SizeType >; // size, pos
    using CharToWords = std::unordered_map< Field , std::unordered_set< Word* > >;

    struct Word
    {
        std::string str;
        bool used;

        std::unordered_map< void* , std::function<void(Word*)> > events;

        Word(std::string str):str(str),used(false){}

        template<class T>
        void registering(T* that, void(T::*x)(Word*))
        {
            if(!used)
                events[reinterpret_cast<void*>(that)] = std::bind(x, that, _1);
        }

        template<class T>
        void deRegister(T* that)
        {
            if(!used)
                events.erase(reinterpret_cast<void*>(that));
        }

        void getUsed()
        {
            if(used)
                throw std::logic_error("Ketszer hasznalta a ["+ str +"] szot!");

            used = true;
            for(auto& e: events)
            {
                e.second(this);
            }
            events.clear();
        }
    };

    struct SmartChar
    {
        struct GuessWordConnect
        {
            bool valid;
            GuessWordIndex gwi;
            CharPosition cp;
            CharToWords chars;

            std::unordered_map< void* , std::function<void(Field)> > events;

            template<class T>
            void registering(T* that, void(T::*x)(Field))
            {
                events[reinterpret_cast<void*>(that)] = std::bind(x, that, _1);
            }

            template<class T>
            void deRegister(T* that)
            {
                events.erase(reinterpret_cast<void*>(that));
            }

            GuessWordConnect():valid(false){}

            void notValidWord(Word* w)
            {
                for(auto& stuff : chars)
                {
                    auto it = stuff.second.find(w);
                    if(it != std::end(stuff.second))
                    {
                        stuff.second.erase(it);
                        if(stuff.second.size() == 0)
                        {
                            for(auto& e: events)
                            {
                                e.second(stuff.first);
                            }
                            chars.erase(stuff.first);
                        }
                        break;
                    }
                }
            }

            void notValidCharAnymore(Field f)
            {

            }

            void set(GuessWordIndex&& i, CharPosition&& c, const CharToWords& ctw)
            {
                gwi = std::move(i);
                cp = std::move(c);
                chars = ctw;
                for(auto& stuff : chars)
                {
                    for(auto& asd : stuff.second)
                    {
                        asd->registering(this, &GuessWordConnect::notValidWord);
                    }
                }
            }
        };
    };
}
int main()
{
    Board board{readBoard()};
    Words words{readWords()};
    Possibilities possibilities{makePossibilities(words)};

    GuessWords guesses{makeGuessWords(board, possibilities)};
    printBoard(board);
    printGuesses(guesses);
    printPossibilities(possibilities);

    std::cout << "Words are : " << words.size() << std::endl;

    std::array<int, 12> guessSizes{};
    std::array<int, 12> wordSizes{};
    for(auto& guess: guesses)
    {
        ++guessSizes[guess.second.size()];
    }
    for(auto& word : words)
    {
        ++wordSizes[word.size()];
    }
    for(int i = 0; i < 12; ++i)
    {
        std::cout << i << " : w: " << wordSizes[i] << "\t g:"<< guessSizes[i] << std::endl;
    }

    return 0;
}
