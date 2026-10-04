#include <iostream>
#include <vector>
#include <algorithm>
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


int main()
{
    long long p, g;

    // ------------------------------------------------
    // User gives both p and candidate g
    // ------------------------------------------------
    cout << "Enter prime p: ";
    cin >> p;

    cout << "Enter generator candidate g: ";
    cin >> g;


    vector<long long> values;


    // ------------------------------------------------
    // Calculate:
    //
    // g^1 mod p
    // g^2 mod p
    // ...
    // g^(p-1) mod p
    // ------------------------------------------------
    for(long long i = 1; i <= p - 1; i++)
    {
        long long value = modPower(g, i, p);

        values.push_back(value);

        cout << g << "^" << i << " mod " << p << " = " << value << endl;
    }


    // Sort all generated values
    sort(values.begin(), values.end());


    // ------------------------------------------------
    // If g is primitive, sorted values should be:
    //
    // 1, 2, 3, ..., p-1
    // ------------------------------------------------
    bool primitive = true;

    for(long long i = 0; i < p - 1; i++)
    {
        if(values[i] != i + 1)
        {
            primitive = false;
            break;
        }
    }


    cout << endl;

    if(primitive)
        cout << g << " is a primitive root of " << p << endl;
    else
        cout << g << " is NOT a primitive root of " << p << endl;


    return 0;
}


/*
====================================================
INPUT FORMAT
====================================================

Enter prime p: 23
Enter generator candidate g: 5


OUTPUT:

5 is a primitive root of 23

====================================================
*/