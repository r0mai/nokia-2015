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
#include <algorithm>
#include <deque>
#include <array>

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


struct Word : std::string
{
    bool reserved;
    Word(const std::string& str):std::string(std::move(str)), reserved(false){}
};
using Words = std::vector<Word>;

template<class T>
using RefWr = std::reference_wrapper<T>;

template <class U>
struct RefWrHashEq {
    template <class T>
    std::size_t operator()(const RefWr<T> &x) const
    {
        return std::hash<U>()(x.get());
    }

    template <class T>
    bool operator()(const RefWr<T> &x, const RefWr<T> &y) const
    {
        return &x.get() == &y.get();
    }
};

using Coord = std::pair<SizeType, SizeType>;
using WordIndex = std::tuple< SizeType, SizeType, SizeType >;

using RefWords = std::unordered_set<RefWr<Word>, RefWrHashEq<std::string>, RefWrHashEq<std::string>>;
using CharToWord = std::unordered_map<char, RefWords >;
using Possibilities = std::unordered_map< Coord , CharToWord, PairHash >;

// -2 NOT IN GAME
// -1 NOT SURE
// 0-9 SOLUTION

template<class T>
int intersect(T& to, const T& hh)
{
    int db = 0;
    for(auto it = std::begin(to); it != std::end(to);)
    {
        if(hh.count(*it))
        {
            ++it;
        }
        else
        {
            ++db;
            it = to.erase(it);
        }
    }
    return db;
}

struct SmartChar
{
    char rw;
    struct WordConnect
    {
        bool b = false;
        WordIndex wi;
        Coord c;
        CharToWord possibles;
    };
    WordConnect left;
    WordConnect down;

    SmartChar(char rw): rw(rw){}

    void add(const WordIndex& wi, const Coord& c)
    {
        if(std::get<2>(wi)) // left
        {
            left.b = true;
            left.wi = wi;
            left.c = c;
        }
        else
        {
            down.b = true;
            down.wi = wi;
            down.c = c;
        }
    }
    template<class T>
    bool checkImport(const T& guesses)
    {
        bool b = false;
        if(left.b)
        {
            auto& vw = guesses.at(left.wi).validWords;

            if(rw != -1 && left.possibles.size() != 1)
            {
                RefWords rws = std::move(left.possibles[rw]);
                left.possibles.clear();
                left.possibles[rw] = rws;
            }

            for(auto it = std::begin(left.possibles); it != std::end(left.possibles);)
            {
                intersect(it->second, vw);
                if(it->second.empty())
                {
                    if(down.b)
                        down.possibles.erase(it->first);

                    it = left.possibles.erase(it);
                    b = true;
                }
                else
                {
                    ++it;
                }
            }
            if(left.possibles.size() == 1 && rw == -1)
            {
                rw = left.possibles.begin()->first;
                if(down.b)
                {
                    RefWords rws = std::move(down.possibles[rw]);
                    down.possibles.clear();
                    down.possibles[rw] = rws;
                }
                b = true;
            }
            else if(left.possibles.empty())
            {
                std::stringstream ss;

                ss <<"Nem maradt betu "<< std::get<0>(left.wi)<< " " << std::get<1>(left.wi)
                    << "LEFT" << " " << left.c.first << " " << left.c.second << std::endl;
                throw std::logic_error(ss.str());
            }
        }

        if(down.b)
        {
            auto& vw = guesses.at(down.wi).validWords;

            if(rw != -1 && down.possibles.size() != 1)
            {
                RefWords rws = std::move(down.possibles[rw]);
                down.possibles.clear();
                down.possibles[rw] = rws;
            }

            for(auto it = std::begin(down.possibles); it != std::end(down.possibles);)
            {
                intersect(it->second, vw);
                if(it->second.empty())
                {
                    if(left.b)
                        left.possibles.erase(it->first);

                    it = down.possibles.erase(it);
                    b = true;
                }
                else
                {
                    ++it;
                }
            }
            if(down.possibles.size() == 1 && rw == -1)
            {
                rw = down.possibles.begin()->first;
                if(left.b)
                {
                    RefWords rws = std::move(left.possibles[rw]);
                    left.possibles.clear();
                    left.possibles[rw] = rws;
                }
                b = true;
            }
            else if(down.possibles.empty())
            {
                std::stringstream ss;

                ss <<"Nem maradt betu "<< std::get<0>(down.wi)<< " " << std::get<1>(down.wi)
                    << "DOWN" << " " << down.c.first << " " << down.c.second << std::endl;
                throw std::logic_error(ss.str());
            }
        }
        return b;
    }

    void make(Possibilities& pb)
    {
        if(left.b)
        {
            left.possibles = pb[left.c];
        }
        if(down.b)
        {
            down.possibles = pb[down.c];
        }
        twoSide();
    }

    bool twoSide()
    {
        bool b = false;
        if(left.b && down.b)
        {
            for(auto it = std::begin(left.possibles); it != std::end(left.possibles);)
            {
                if(!down.possibles.count(it->first))
                {
                    b = true;
                    it = left.possibles.erase(it);
                }
                else
                {
                    ++it;
                }
            }

            for(auto it = std::begin(down.possibles); it != std::end(down.possibles);)
            {
                if(!left.possibles.count(it->first))
                {
                    b = true;
                    it = down.possibles.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }
        return b;
    }

    operator char()
    {
        return rw;
    }
};
using Board = std::vector<std::vector<SmartChar>>;

struct GuessWord : std::vector< RefWr<SmartChar> >
{
    bool found = false;
    WordIndex lr;
    GuessWord(WordIndex lr):lr(lr){}

    RefWords validWords;
    void setValidWords()
    {
        for(auto& pr : (std::get<2>(lr) ? this->front().get().left.possibles : this->front().get().down.possibles ))
        {
            validWords.insert(std::begin(pr.second), std::end(pr.second));
        }
        check();
    }
    bool check()
    {
        if(found)
            return false;

        bool b = false;
        for(auto it = std::begin(validWords); it != std::end(validWords);)
        {
            if(it->get().reserved)
            {
                b = true;
                it = validWords.erase(it);
            }
            else
            {
                ++it;
            }
        }
        for(SmartChar& sc : *this)
        {
            RefWords vw;
            for(auto& pr : (std::get<2>(lr) ? sc.left.possibles : sc.down.possibles ))
            {
                vw.insert(std::begin(pr.second), std::end(pr.second));
            }
            b |= intersect(validWords, vw);

            if(validWords.size() == 0)
            {
                std::stringstream ss;
                ss << "No valid word " << std::get<0>(lr) << " " << std::get<1>(lr) << " " << (std::get<2>(lr) ? "LEFT" : "DOWN") << std::endl;
                throw std::logic_error(ss.str());
            }
        }
        if(validWords.size() == 1 && !found)
        {
            b = true;
            found = true;
            auto& word = validWords.begin()->get();
            word.reserved = true;
            auto begin1 = std::begin(word);
            auto end1 = std::end(word);
            auto begin2 = this->begin();
            while(begin1 != end1)
            {
                if(begin2->get().rw != -1 && begin2->get().rw != *begin1)
                {
                    throw std::logic_error("Kesz szo nem ugyanazt a karaktert akarja beirni :(");
                }
                begin2++->get().rw = *begin1++;
            }
        }
        return b;
    }
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
    res[18][0] = '5';

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

                auto& word = res.insert(std::make_pair(index, GuessWord{index})).first->second;
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
                auto& word = res.insert(std::make_pair(index, GuessWord{index})).first->second;
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
    for(auto& r : res)
    {
        r.second.setValidWords();
    }

    return std::move(res);
}

void doWork(GuessWords& res, Board& b)
{
    const SizeType x = b.size();
    const SizeType y = b[0].size();
    bool changed = true;
    while(changed)
    {
        changed = false;
        for(SizeType i = 0; i < x; ++i)
        {
            for(SizeType j = 0; j < y; ++j)
            {
                changed |= b[i][j].checkImport(res);
            }
        }

        changed = false;
        for(auto& r : res)
        {
            changed |= r.second.check();
        }
    }
    //std::cout << "END" << std::endl;
}

Possibilities makePossibilities(Words& w)
{
    Possibilities res;
    for(auto& wo : w)
    {
        SizeType s = wo.size();
        for(SizeType i = 0; i < s; ++i)
        {
            res[std::make_pair(s,i)][wo[i]].insert(std::ref(wo));
        }
    }

    return res;
}


void printSimpyBoard(Board& board)
{
    for(SizeType i = 0; i < board.size(); ++i)
    {
        for(SizeType j = 0; j < board[i].size(); ++j)
        {
            if(board[i][j] == -1)
            {
                std::clog << " ";
            }
            else if(board[i][j] == -2)
            {
                std::clog << "-";
            }
            else
            {
                std::clog << static_cast<char>(board[i][j]);

            }
        }
        std::clog<<std::endl;
    }
    std::clog<<std::endl;
}
void printBoard(Board& board)
{
    std::cout << "Board is: " << std::endl;
    for(SizeType i = 0; i < board.size(); ++i)
    {
        for(SizeType j = 0; j < board[i].size(); ++j)
        {
            std::cout << std::setw(3) << std::right << static_cast<int>(board[i][j]);

            std::stringstream ss;
            ss << "{";
            if(board[i][j].left.b)
            {
                for(auto& f : board[i][j].left.possibles)
                {
                    ss << f.first << ",";
                }
            }
            else if(board[i][j].down.b)
            {
                for(auto& f : board[i][j].down.possibles)
                {
                    ss << f.first << ",";
                }
            }
            ss << "}";
            std::cout << std::setw(23) << std::left << ss.str();
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
        std::cout <<"{";
        for(Word& w : guess.second.validWords)
        {
            std::cout << w << ",";
        }
        std::cout <<"}";

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
        for(auto& f : st.second)
        {
            std::cout << f.first << " ";
        }
        std::endl(std::cout);
    }
}

int main()
{
    Board board{readBoard()};
    Words words{readWords()};
    Possibilities possibilities{makePossibilities(words)};

    GuessWords guesses{makeGuessWords(board, possibilities)};

    try
    {
        doWork(guesses, board);
    }
    catch(std::logic_error& e)
    {
        std::cout << e.what() << std::endl;
    }

    printSimpyBoard(board);

    //printBoard(board);
    //printGuesses(guesses);
    //printPossibilities(possibilities);

    //std::cout << "Words are : " << words.size() << std::endl;

    std::array<int, 12> guessSizes{};
    std::array<int, 12> wordSizes{};
    for(auto& guess: guesses)
    {
        ++guessSizes[guess.second.size()];
    }

    std::clog << "Not used Words" << std::endl;
    for(auto& word : words)
    {
        if(!word.reserved)
        {
            std::clog << word << std::endl;
            ++wordSizes[word.size()];
        }
        if(!word.reserved && word.size() == 5)
        {
            std::cout << word << std::endl;
        }
    }
    for(int i = 0; i < 12; ++i)
    {
    //std::cout << i << " : w: " << wordSizes[i] << "\t g:"<< guessSizes[i] << std::endl;
    }

    return 0;
}
