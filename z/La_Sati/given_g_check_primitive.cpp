#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


// Calculates: base^exp mod mod
long long modPower(long long base, long long exp, long long mod)
{
    long long result = 1;

    while (exp > 0)
    {
        if (exp % 2 == 1)
        {
            result = (result * base) % mod;
        }

        base = (base * base) % mod;
        exp = exp / 2;
    }

    return result;
}


int main()
{
    int p, g;

    // Take prime number and primitive root candidate
    cout << "Enter prime number p: ";
    cin >> p;

    cout << "Enter primitive root candidate g: ";
    cin >> g;


    // Store all values of g^i mod p
    vector<int> values;


    // Calculate g^i mod p for i = 1 to p-1
    for (int i = 1; i <= p - 1; i++)
    {
        int value = modPower(g, i, p);

        values.push_back(value);

        cout << g << "^" << i
             << " mod " << p
             << " = " << value << endl;
    }


    // Sort the generated values
    sort(values.begin(), values.end());


    cout << "\nSorted values: ";

    for (int value : values)
    {
        cout << value << " ";
    }

    cout << endl;


    // If g is a primitive root, sorted values should be:
    // 1, 2, 3, ..., p-1
    bool isPrimitiveRoot = true;

    for (int i = 0; i < p - 1; i++)
    {
        if (values[i] != i + 1)
        {
            isPrimitiveRoot = false;
            break;
        }
    }


    if (isPrimitiveRoot)
    {
        cout << g << " is a primitive root of " << p << endl;
    }
    else
    {
        cout << g << " is NOT a primitive root of " << p << endl;
    }


    return 0;
}