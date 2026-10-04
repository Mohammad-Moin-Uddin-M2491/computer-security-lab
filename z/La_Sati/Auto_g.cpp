#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


// Calculates: base^exp mod mod
// Used to calculate g^i mod p efficiently
long long modPower(long long base, long long exp, long long mod)
{
    long long result = 1;

    while (exp > 0)
    {
        // If exponent is odd, include current base in result
        if (exp % 2 == 1)
        {
            result = (result * base) % mod;
        }

        // Square the base and reduce by mod
        base = (base * base) % mod;

        // Reduce exponent
        exp = exp / 2;
    }

    return result;
}


// Checks whether a given g is a primitive root of p
bool isPrimitiveRoot(int g, int p)
{
    vector<int> values;

    // Generate:
    // g^1 mod p, g^2 mod p, ..., g^(p-1) mod p
    for (int i = 1; i <= p - 1; i++)
    {
        int value = modPower(g, i, p);

        // Store each generated value
        values.push_back(value);
    }

    // Sort the generated values
    sort(values.begin(), values.end());

    // For a primitive root, after sorting we should get:
    // 1, 2, 3, ..., p-1
    for (int i = 0; i < p - 1; i++)
    {
        if (values[i] != i + 1)
        {
            return false;
        }
    }

    return true;
}


// Automatically finds the first valid primitive root
int findPrimitiveRoot(int p)
{
    // Try g = 2, 3, 4, ... until one works
    for (int g = 2; g < p; g++)
    {
        if (isPrimitiveRoot(g, p))
        {
            return g;
        }
    }

    return -1;
}


int main()
{
    int p;

    // User only gives the prime number
    cout << "Enter prime number p: ";
    cin >> p;

    // Program automatically finds g
    int g = findPrimitiveRoot(p);

    if (g == -1)
    {
        cout << "Primitive root not found." << endl;
    }
    else
    {
        cout << "Primitive root g = " << g << endl;
    }

    return 0;
}