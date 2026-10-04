#include <iostream>
#include <vector>
using namespace std;


// Finds GCD of two numbers
long long gcd(long long a, long long b)
{
    while (b != 0)
    {
        long long temp = b;
        b = a % b;
        a = temp;
    }

    return a;
}


// Calculates: base^exp mod mod
long long modPower(long long base, long long exp, long long mod)
{
    long long result = 1;
    base = base % mod;

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


// Checks whether alpha is a primitive root of p
bool isPrimitiveRoot(long long alpha, long long p)
{
    vector<bool> used(p, false);

    for (long long i = 1; i <= p - 1; i++)
    {
        long long value = modPower(alpha, i, p);

        if (used[value])
        {
            return false;
        }

        used[value] = true;
    }

    return true;
}


// Automatically finds the first primitive root alpha
long long findPrimitiveRoot(long long p)
{
    for (long long alpha = 2; alpha < p; alpha++)
    {
        if (isPrimitiveRoot(alpha, p))
        {
            return alpha;
        }
    }

    return -1;
}


// Finds modular inverse
long long modInverse(long long a, long long mod)
{
    for (long long inverse = 1; inverse < mod; inverse++)
    {
        if ((a * inverse) % mod == 1)
        {
            return inverse;
        }
    }

    return -1;
}


int main()
{
    long long p;

    cout << "Enter prime number p: ";
    cin >> p;


    // Automatically find generator alpha
    long long alpha = findPrimitiveRoot(p);

    if (alpha == -1)
    {
        cout << "Primitive root not found." << endl;
        return 0;
    }


    // Private key x
    long long x;

    cout << "Enter private key x: ";
    cin >> x;


    // beta = alpha^x mod p
    long long beta = modPower(alpha, x, p);


    cout << "\n----- Key Generation -----" << endl;
    cout << "alpha = " << alpha << endl;
    cout << "beta = " << beta << endl;
    cout << "Public Key = (" << alpha << ", " << beta << ", " << p << ")" << endl;
    cout << "Private Key = " << x << endl;


    // Message
    long long m;

    cout << "\nEnter message m: ";
    cin >> m;


    // Random k for signature
    long long k;

    cout << "Enter random k for signature: ";
    cin >> k;


    // k must be coprime with p-1
    if (gcd(k, p - 1) != 1)
    {
        cout << "Invalid k. gcd(k, p-1) must be 1." << endl;
        return 0;
    }


    // y1 = alpha^k mod p
    long long y1 = modPower(alpha, k, p);


    // Find k^(-1) mod (p-1)
    long long kInverse = modInverse(k, p - 1);

    if (kInverse == -1)
    {
        cout << "Could not find inverse of k." << endl;
        return 0;
    }


    // y2 = k^(-1) * (m - x*y1) mod (p-1)
    long long y2 = (kInverse * (m - x * y1)) % (p - 1);

    // Make y2 positive if needed
    if (y2 < 0)
    {
        y2 = y2 + (p - 1);
    }


    cout << "\n----- Digital Signature -----" << endl;
    cout << "y1 = " << y1 << endl;
    cout << "y2 = " << y2 << endl;
    cout << "Signature = (" << y1 << ", " << y2 << ")" << endl;


    // Verification
    long long leftSide = (modPower(y1, y2, p) * modPower(beta, y1, p)) % p;
    long long rightSide = modPower(alpha, m, p);


    cout << "\n----- Signature Verification -----" << endl;
    cout << "Left Side = " << leftSide << endl;
    cout << "Right Side = " << rightSide << endl;


    if (leftSide == rightSide)
    {
        cout << "Signature is VALID." << endl;
    }
    else
    {
        cout << "Signature is INVALID." << endl;
    }


    return 0;
}