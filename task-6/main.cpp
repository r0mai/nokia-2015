#include <iostream>
#include <valarray>
#include <vector>
#include <unordered_set>
#include <iterator>

using ColorType = int;
using SizeType = int;

using ColorVector = std::vector<ColorType>;
using ColorMatrix = std::vector<ColorVector>;


bool g_print = false;

using Coord = std::valarray<SizeType>;

template<class T, class U = typename T::value_type>
inline void print(const T& c)
{
    std::copy(std::begin(c), std::end(c), std::ostream_iterator<U>(std::clog, " "));
    std::endl(std::clog);
}

inline auto get(const ColorMatrix& m, const Coord& c)
{
    //return m[std::get<0>(c)][std::get<1>(c)];
    return m[c[0]][c[1]];
}

template<class T>
inline T gcd(T a, T b)
{
    if(b<0) b = -b;
    while( b!=0 )
    {
        a %= b;
        if( a==0 ) return b;
        b %= a;
    }
    return a;
}

inline Coord normal(const Coord& first, const Coord& second)
{
    Coord res{(first-second).cshift(1)};
    res /= res[0] < 0 ? gcd(res[0] = -res[0], res[1]) : gcd(res[0], res[1] = -res[1]);
    // lenormáljuk a legkisebb egész koordinátákra, hogy x>=0 legyen
    return std::move(res);
}

SizeType do_it(const ColorMatrix& m)
{
    const SizeType x = m.size();
    const SizeType y = m[0].size();
    const SizeType sum = y * x;

    const Coord min_c{0,0};
    const Coord max_c{x,y};

    SizeType res = 0;

    // végigmegyünk az első koordináták lehetőségein
    for(SizeType first_c = 0; first_c < sum; ++first_c)
    {
        // kiszámoljuk a koordinátát, lementjük a színt
        const Coord first{first_c / y, first_c % y};
        const ColorType& now_col = get(m, first);

        // végigmegyünk a nála nagyobb 2. kiválasztott koordinátán
        for(SizeType second_c = first_c+1; second_c < sum; ++second_c)
        {
            const Coord second{second_c / y, second_c % y};

            if(now_col == get(m, second))
            {
                // vesszük az általuk alkotott egyenes normálvektorát(x >= 0)
                Coord norm{normal(first, second)};

                // a vektorral lépkedünk
                for(Coord moved = norm; ;moved+=norm)
                {
                    // 3. koordináta
                    const Coord third{first + moved};

                    // ha benne vagyunk a range-ben, OK (kben break)
                    if( (min_c <=third).min() && (third < max_c).min() )
                    {
                        // legyen színegyezés ill a harmadik a második után következzen
                        if(now_col == get(m, third) && second_c < third[0]*y+third[1])
                        {
                            // negyedik koord számolás
                            const Coord fourth{second + moved};

                            // ha benne vagyunk a range-ben, OK (kben break)
                            if( (min_c <=fourth).min() && (fourth < max_c).min() )
                            {
                                // ha itt is színegyezés, akkor OK, találtunk 1-et
                                if(now_col == get(m, fourth))
                                {
                                    ++res;
                                    if(g_print)
                                    {
                                        std::clog << std::endl  << "SOLUTION - col: " << now_col << std::endl;
                                        print(first);
                                        print(second);
                                        print(third);
                                        print(fourth);
                                    }
                                }
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }
    return res;
}




int main(int argc, char* argv[])
{
    g_print = argc >= 2;

    // beolvasni!!!
    ColorMatrix v{{1,2,1,3,3,1},
                  {1,4,1,3,3,1},
                  {2,5,5,5,4,2},
                  {4,5,5,5,4,4},
                  {4,5,5,5,2,4}};

    std::cout << do_it(v) << std::endl;
    return 0;
}
