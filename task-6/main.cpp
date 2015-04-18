#include <iostream>
#include <valarray>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <fstream>
#include <iterator>
#include <iomanip>
#include <atomic>
#include <sstream>

using ColorType = int;
using SizeType = int;
using ResultType = std::uintmax_t;

using ColorVector = std::vector<ColorType>;
using ColorMatrix = std::vector<ColorVector>;


bool g_print0 = false,
    g_print1 = false;

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
    if(a<0) a = -a;
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
    Coord res((first-second).cshift(1));
    res /= res[0] < 0 ? gcd(res[0] = -res[0], res[1]) : gcd(res[0], res[1] = -res[1]);
    // lenormáljuk a legkisebb egész koordinátákra, hogy x>=0 legyen
    return std::move(res);
}

ResultType do_it(const ColorMatrix& m)
{
    const SizeType x = m.size();
    const SizeType y = m[0].size();
    const SizeType sum = y * x;

    const Coord min_c{0,0};
    const Coord max_c{x,y};

    ResultType res = 0;

    // végigmegyünk az első koordináták lehetőségein
    for(SizeType first_c = 0; first_c < sum; ++first_c)
    {
        if(g_print0)
            std::clog << "\033[0;0H" << first_c << "\t" << sum << std::endl;
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
                    const Coord third(first + moved);

                    // ha benne vagyunk a range-ben, OK (kben break)
                    if( (min_c <=third).min() && (third < max_c).min() )
                    {
                        // legyen színegyezés ill a harmadik a második után következzen
                        if(now_col == get(m, third) && second_c < third[0]*y+third[1])
                        {
                            // negyedik koord számolás
                            const Coord fourth(second + moved);

                            // ha benne vagyunk a range-ben, OK (kben break)
                            if( (min_c <=fourth).min() && (fourth < max_c).min() )
                            {
                                // ha itt is színegyezés, akkor OK, találtunk 1-et
                                if(now_col == get(m, fourth))
                                {
                                    ++res;
                                    if(g_print1)
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

ResultType do_it_2(const ColorMatrix& m)
{
    std::atomic<ResultType> res{0};

    const SizeType x = m.size();
    const SizeType y = m[0].size();
    const SizeType sum = y * x;

    const Coord min_c{0,0};
    const Coord max_c{x,y};

    #pragma omp parallel for
    for(SizeType i = 0; i < sum; ++i)
    {
        std::clog << "\033[0;0H" << std::setw(10) << std::left << i << " / " << sum << " eddig: " << res << std::endl;

        Coord x1{i / y, i % y};
        const ColorType& now_col = get(m, x1);

        for(SizeType j = i+1; j < sum; ++j)
        {
            Coord x2{j / y, j % y};

            if(now_col != get(m, x2))
                continue;

            for(SizeType k = j+1; k < sum; ++k)
            {
                Coord x3{k / y, k % y};

                if(now_col != get(m, x3))
                    continue;

                if(((x1-x2)*(x1-x3)).sum() == 0)
                {
                    Coord x4 = x2 + (x3-x1);

                    if( (min_c <=x4).min() && (x4 < max_c).min() && now_col == get(m, x4))
                    {
                        ++res;
                    }
                }
            }
        }
    }
    return res;
}


ResultType do_it_3(const ColorMatrix& m)
{
    const SizeType x = m.size();
    const SizeType y = m[0].size();

    std::atomic<ResultType> res{0};

    // eloszor a nem dolteket nezzuk:

    #pragma omp parallel for
    for(SizeType x1 = 0; x1 < x; ++x1)
    {
        ResultType p_res = 0;
        for(SizeType y1 = 0; y1 < y; ++y1)
        {
            ColorType col = m[x1][y1];
            for(SizeType y2 = y1+1; y2 < y; ++y2)
            {
                if(col != m[x1][y2])
                    continue;

                for(SizeType x2 = x1+1; x2 < x; ++x2)
                {
                    if(col == m[x2][y1] && col == m[x2][y2])
                    {
                        ++p_res;
                    }
                }
            }
        }
        res += p_res;
    }


    #pragma omp parallel for
    for(SizeType x1 = 0; x1 < x; ++x1)
    {
        ResultType p_res = 0;
        for(SizeType y1 = 0; y1 < y; ++y1)
        {
            ColorType col = m[x1][y1];
            for(SizeType x2 = x1+1; x2 < x; ++x2)
            {
                for(SizeType y2 = y1+1; y2 < y; ++y2)
                {
                    if(col != m[x2][y2])
                        continue;

                    SizeType nvx = y2-y1;
                    SizeType nvy = x1-x2;

                    SizeType g = gcd(nvx, nvy);
                    nvx /= g;
                    nvy /= g;

                    SizeType x3 = x1 + nvx;
                    SizeType y3 = y1 + nvy;
                    SizeType x4 = x2 + nvx;
                    SizeType y4 = y2 + nvy;
                    while(y3 >= 0 && x4 < x)
                    {
                        if(col == m[x3][y3] && col == m[x4][y4])
                        {
                            ++p_res;
                        }
                        x3 += nvx;
                        y3 += nvy;
                        x4 += nvx;
                        y4 += nvy;
                    }
                }
            }
        }
        res += p_res;
    }
    return res;
}



ColorMatrix readBMP(std::string filename)
{
    using CharType = char; // hmm :( nem működik unsigned char-ral

    std::basic_ifstream<CharType> fx(filename);
    if(!fx.is_open())
        throw std::invalid_argument("No file");

    CharType info[54];
    fx.read(info, sizeof(CharType) * 54); // read the 54-byte header

    // extract image height and width from header
    int height = *reinterpret_cast<int*>(info + 18);
    int width = *reinterpret_cast<int*>(info + 22);

    int size = 3 * width * height;

    std::basic_string<CharType> data(size, ' ');
    fx.read(&data.front(), sizeof(CharType) * size); // read the rest of the data at once

    ColorMatrix result(width, ColorVector(height));

    for(int i = 0; i < size; i+=3)
    {
        result[i/3 / height][i/3 % height] = (static_cast<unsigned int>(static_cast<unsigned char>(data[i]  )) << 16) +
                                           (static_cast<unsigned int>(static_cast<unsigned char>(data[i+1])) << 8) +
                                           (static_cast<unsigned int>(static_cast<unsigned char>(data[i+2])));
    }
    if(g_print1)
    {
        std::clog << "Read: " << height << " " << width << std::endl;
        for(SizeType i = 0; i < width; ++i)
        {
            for(SizeType j = 0; j < height; ++j)
            {
                std::clog << std::setw(9) << std::right << result[i][j];
            }
            std::clog << std::endl;
        }
    }
    return std::move(result);
}

ColorMatrix readFile(std::string filename)
{
    std::ifstream fx(filename);
    if(!fx.is_open())
        throw std::invalid_argument("No file");

    std::string str;
    ColorMatrix res;
    while(std::getline(fx, str))
    {
        std::stringstream ss(str);
        res.push_back(ColorVector{std::istream_iterator<char>(ss), std::istream_iterator<char>()});
    }
    return res;
}

int main(int argc, char* argv[])
{
    g_print0 = argc >= 2;
    g_print1 = argc >= 3;

    // beolvasni!!!
    ColorMatrix v{{1,2,1,3,3,1},
                  {1,4,1,3,3,1},
                  {2,5,5,5,4,2},
                  {4,5,5,5,4,4},
                  {4,5,5,5,2,4}};

    std::cout << do_it_3(readBMP("task6.bmp")) << std::endl;
    return 0;
}
