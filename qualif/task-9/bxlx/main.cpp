#include <iostream>
#include <algorithm>
#include <vector>
#include <valarray>
#include <sstream>
#include <iterator>
#include <unordered_set>
#include <fstream>
#include <functional>
#include <csignal>
#include <exception>
#include <set>
#include <sstream>


using SizeType = std::size_t;

using Node = SizeType;

using Connectivity = std::unordered_set<Node>;
using Matrix = std::vector<Connectivity>;

class SymmetricMatrix
{
    Matrix m;

public:
    SymmetricMatrix(SizeType n):m(n){}

    bool get(Node x, Node y) const
    {
        return m[x].count(y);
    }

    void set(Node x, Node y)
    {
        m[x].insert(y);
        m[y].insert(x);
    }

    Connectivity& getNeigbours(Node n)
    {
        return m[n];
    }

    void erase(Node x, Node y)
    {
        m[x].erase(y);
        m[y].erase(x);
    }

    void erase(Node x)
    {
        for(const Node& n :m[x])
        {
            m[n].erase(x);
        }
        m[x].clear();
    }

};

int main()
{
    bool file = true;

    std::ifstream input_f("task9.txt");

    std::istream& input(file && input_f.is_open() ? input_f : std::cin);

    std::string tmp;
	std::getline(input, tmp);
	std::istringstream ss(tmp);

	const SizeType width = std::distance(std::istream_iterator<std::string>(ss), std::istream_iterator<std::string>());

    SymmetricMatrix graph(width);

    // beolvasas
    for(SizeType i = 0; i < width; ++i)
    {
        Node i_tmp;
        input >> i_tmp;
        for(SizeType j = 0; j < width; ++j)
        {
            input >> i_tmp;
            if(i_tmp)
                graph.set(i, j);
        }
    }

    Connectivity setA; // nem toroljuk ki
    Connectivity setB; // kitoroljuk

    // redukálás - amelyik pontnak nincs szomszédja, automatikusan nem kitörlendő lesz
    auto reduce = [&]
    {
        for(Node i = 0; i < width; ++i)
        {
            if(!setA.count(i) && !setB.count(i) && graph.getNeigbours(i).size() == 0)
                setA.insert(i);
        }
    };

    reduce();

    // min keresés, illetve a minimum megoldások
    SizeType min = width;
    std::vector<Connectivity> mins;
    // 56 van, de fő a biztonság
    mins.reserve(64);

    // rekurzív lambdát valósítsunk meg
    std::function<void()> recursive;


    (recursive = [&]
    {
        // mentsük el az aktuális állapotot
        SymmetricMatrix save = graph;
        Connectivity setACopy = setA;
        Connectivity setBCopy = setB;

        // az összes még fel nem használt node-on végigmegyünk
        for(Node i = 0; i < width; ++i)
        {
            if(!setA.count(i) && !setB.count(i))
            {
                // vagy megtartandó, vagy nem. Kezdjük az igaz ággal.


                // tegyük bele a megtartandok koze
                setA.insert(i);

                // ekkor a szomszedjait ki kell torolni - beletenni a B-be
                for(auto it = std::begin(graph.getNeigbours(i)); it != std::end(graph.getNeigbours(i));)
                {
                    Node n = *it;
                    it = graph.getNeigbours(i).erase(it);

                    graph.erase(n);
                    setB.insert(n);
                }

                // csak akkor folytatjuk, ha B merete kisebb a min-nál (kben nem javít az egészen)
                if(setB.size() <= min)
                {
                    // a 0 -s pontokat tegyuk bele A-ba
                    reduce();

                    // ha mindegyik Node bennevan legalabb 1 halmazba
                    if(setA.size() + setB.size() == width)
                    {
                        // nezzuk meg h eddigiekhez kepest javult-e
                        if(min > setB.size())
                        {
                            min = setB.size();
                            // igen, toroljuk az eddigieket ...
                            mins.clear();
                            mins.push_back(setB);
                        }
                        else if(min == setB.size()) // ha ugyan az, akkor csak mentsuk el
                        {
                            mins.push_back(setB);
                        }
                    }
                    else // ha meg nincs meg mindegyik, rekurzivan haladjunk befele
                    {
                        recursive();
                    }
                }

                // vegigneztuk az osszes lehetoseget, amikor "i" benne van.
                // most mar csak azok kellenek, amikor i nincs benne.

                // toroljuk ki i-t, es tegyuk bele B-be (az eredetibe - hogy a tobbinel a valtoztatas meglegyen)
                save.erase(i);
                setBCopy.insert(i);

                // ha ezzel leptuk tul a minimumot, egy rekurzios lepessel kijjebb kell menni mar
                if(setBCopy.size() > min)
                    return;

                // toltsuk vissza a mentett allapotot
                graph = save;
                setA = setACopy;
                setB = setBCopy;

                // tegyuk bele a 0-sakat
                reduce();

                // ha vege van, akkor nezzuk meg, hogy javitottunk-e, majd lepjunk egy rekurzios lepessel vissza
                if(setA.size() + setB.size() == width)
                {
                    // nezzuk meg h eddigiekhez kepest javult-e
                    if(min > setB.size())
                    {
                        min = setB.size();

                        mins.clear();
                        mins.push_back(setB);
                    }
                    else if(min == setB.size()) // ha ugyan az, akkor csak mentsuk el
                    {
                        mins.push_back(setB);
                    }
                    return;
                }
            }
        }
    })();
    std::clog << "Minimalis " << min << ", db: " << mins.size() << std::endl;


    // gyujtsuk ossze az osszeset (es logoljunk)
    std::set<Node> ns;
    for(auto& m : mins)
    {
        std::copy(std::begin(m), std::end(m), std::ostream_iterator<Node>(std::clog, " "));
        ns.insert(std::begin(m), std::end(m));
        std::clog << std::endl;
    }

    // formazzuk a megfelelo kimenethez
    std::stringstream sso;
    for(Node n : ns)
    {
        ++n;
        if(n < 10)
            sso << "0" << n;
        else
            sso << n;
    }
    std::clog << sso.str() << std::endl;

    for(SizeType i = 5; i < sso.str().size(); i+=6)
    {
        std::cout << sso.str()[i];
    }
    std::cout << std::endl;

    return 0;
}
