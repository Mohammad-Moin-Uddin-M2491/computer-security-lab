#include <iostream>
#include <vector>
using namespace std;


// ----------------------------------------------------
// Find GCD
//
// For signature:
// gcd(k, p-1) must be 1
// ----------------------------------------------------
long long gcd(long long a, long long b)
{
    while(b != 0)
    {
        long long temp = b;
        b = a % b;
        a = temp;
    }

    return a;
}


// ----------------------------------------------------
// Fast modular exponentiation
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
// ----------------------------------------------------
long long findPrimitiveRoot(long long p)
{
    for(long long g = 2; g < p; g++)
    {
        if(isPrimitiveRoot(g, p)) return g;
    }

    return -1;
}


// ----------------------------------------------------
// Find modular inverse
//
// a * inverse mod mod = 1
// ----------------------------------------------------
long long modInverse(long long a, long long mod)
{
    for(long long inverse = 1; inverse < mod; inverse++)
    {
        if((a * inverse) % mod == 1) return inverse;
    }

    return -1;
}


int main()
{
    long long p, x, m, k;


    // ------------------------------------------------
    // STEP 1: Enter prime p
    // ------------------------------------------------
    cout << "Enter prime p: ";
    cin >> p;


    // Automatically find generator g
    long long g = findPrimitiveRoot(p);

    if(g == -1)
    {
        cout << "Generator not found." << endl;
        return 0;
    }


    // ------------------------------------------------
    // STEP 2: Choose private key x
    // ------------------------------------------------
    cout << "Enter private key x: ";
    cin >> x;


    // ------------------------------------------------
    // Public value:
    //
    // y = g^x mod p
    // ------------------------------------------------
    long long y = modPower(g, x, p);


    cout << "\n----- Key Generation -----" << endl;
    cout << "p = " << p << endl;
    cout << "g = " << g << endl;
    cout << "x = " << x << endl;
    cout << "y = " << y << endl;
    cout << "Public Key = (" << p << ", " << g << ", " << y << ")" << endl;
    cout << "Private Key = " << x << endl;


    // ------------------------------------------------
    // STEP 3: Enter message/hash m
    // ------------------------------------------------
    cout << "\nEnter message m: ";
    cin >> m;


    // ------------------------------------------------
    // STEP 4: Choose random k
    //
    // For ElGamal signature:
    //
    // gcd(k, p-1) = 1
    // ------------------------------------------------
    cout << "Enter random k: ";
    cin >> k;

    if(gcd(k, p - 1) != 1)
    {
        cout << "Invalid k. gcd(k, p-1) must be 1." << endl;
        return 0;
    }


    // =================================================
    // SIGNATURE GENERATION
    // =================================================

    // First signature value:
    //
    // A = g^k mod p
    long long A = modPower(g, k, p);


    // Find:
    //
    // k^(-1) mod (p-1)
    long long kInverse = modInverse(k, p - 1);


    // Second signature value:
    //
    // sig = k^(-1)(m - x*A) mod (p-1)
    long long sig = (kInverse * (m - x * A)) % (p - 1);

    // Make sig positive if C++ gives a negative remainder
    if(sig < 0) sig += (p - 1);


    cout << "\n----- Digital Signature -----" << endl;
    cout << "A = " << A << endl;
    cout << "sig = " << sig << endl;
    cout << "Signature = (" << A << ", " << sig << ")" << endl;


    // =================================================
    // SIGNATURE VERIFICATION
    // =================================================

    // Left side:
    //
    // V1 = g^m mod p
    long long V1 = modPower(g, m, p);


    // Right side:
    //
    // V2 = y^A * A^sig mod p
    long long V2 = (modPower(y, A, p) * modPower(A, sig, p)) % p;


    cout << "\n----- Signature Verification -----" << endl;
    cout << "V1 = " << V1 << endl;
    cout << "V2 = " << V2 << endl;


    // Signature is valid when:
    //
    // V1 = V2
    if(V1 == V2)
        cout << "Signature is VALID." << endl;
    else
        cout << "Signature is INVALID." << endl;


    return 0;
}


/*
====================================================
INPUT FORMAT
====================================================

Enter prime p: 23
Enter private key x: 6
Enter message m: 10
Enter random k: 7


IMPORTANT:

k must satisfy:

gcd(k, p-1) = 1

Here:

gcd(7, 22) = 1

So k = 7 is valid.

====================================================
*/