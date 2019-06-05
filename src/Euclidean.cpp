#include "Euclidean.h"
#if defined(TEST)
#include <stdio.h>
#include <utility>
using std::pair;
using std::make_pair;
#endif

//using std::list;

template<typename T>
list<T> replicate(int n, T x)
{
    return list<int>(n, x);
}

template<typename T, typename F>
list<T> concatMap(F f, const list<T>& xs)
{
    list<T> ys;

    for (auto x : xs)
        ys.splice(ys.end(), f(x));
    return ys;
}

list<int> euclidean(int k, int n)
{
    auto a = n % k;

    if (a == 0)
        return replicate(k, n / k);
    else
    {
        auto f = [n, k] (auto x)
        {
            auto xs = replicate(x - 1, n / k); 
            xs.push_back(n / k + 1);
            return xs;
        };
        return concatMap(f, euclidean(a, k));
    }
}

list<int> expand(const list<int>& xs)
{
    auto f = [] (auto x)
    {
        list<int> ys = replicate(x - 1, 0);
        ys.push_front(1);
        return ys;
    };
    return concatMap(f, xs);
}

#if defined(TEST)

void display(const list<int>& xs)
{
    for (auto x : xs)
        printf("%d", x);
    printf("\n");
}

int main()
{
    pair<int, int> xs[] =
        { make_pair(1,2)
        , make_pair(1,3)
        , make_pair(4,12)
        , make_pair(2,3)
        , make_pair(2,5)
        , make_pair(3,4)
        , make_pair(3,5)
        , make_pair(3,8)
        , make_pair(7,12)
        , make_pair(7,16)
        , make_pair(1,32)
        , make_pair(2,32)
        , make_pair(3,32)
        , make_pair(5,32)
        , make_pair(7,32)
        };

    for (auto x : xs)
        display(expand(euclidean(x.first, x.second)));

    for (int n = 1; n <= 100; ++n)
        for (int k = 1; k <= n; ++k)
            display(expand(euclidean(k, n)));
}

#endif
