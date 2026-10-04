#include <iostream>
#include <vector>
using namespace std;


// ----------------------------------------------------
// Fast modular exponentiation
//
// Calculates:
// base^exp mod mod
// ----------------------------------------------------
long long modPower(long long base, long long exp, long long mod)
{
    long long result = 1;
    base = base % mod;

    while(exp > 0)
    {
        if(exp % 2 == 1) result = (result * base) % mod;

        base = (base * base) % mod;
        exp = exp / 2;
    }

    return result;
}


// ----------------------------------------------------
// Check whether g is a primitive root of p
//
// A valid g should generate all non-zero values
// 1, 2, ..., p-1 before repeating.
// ----------------------------------------------------
bool isPrimitiveRoot(long long g, long long p)
{
    vector<bool> used(p, false);

    for(long long i = 1; i <= p - 1; i++)
    {
        long long value = modPower(g, i, p);

        if(used[value]) return false;

        used[value] = true;
    }

    return true;
}


// ----------------------------------------------------
// Automatically find generator g
//
// Try:
// g = 2, 3, 4, ...
//
// Return the first valid primitive root.
// ----------------------------------------------------
long long findPrimitiveRoot(long long p)
{
    for(long long g = 2; g < p; g++)
    {
        if(isPrimitiveRoot(g, p)) return g;
    }

    return -1;
}


int main()
{
    long long p;

    cout << "Enter prime p: ";
    cin >> p;


    // Automatically find g
    long long g = findPrimitiveRoot(p);


    if(g == -1)
        cout << "Generator not found." << endl;
    else
        cout << "Generator g = " << g << endl;


    return 0;
}


/*
====================================================
INPUT FORMAT
====================================================

Enter prime p: 23


OUTPUT:

Generator g = 5

====================================================
*/