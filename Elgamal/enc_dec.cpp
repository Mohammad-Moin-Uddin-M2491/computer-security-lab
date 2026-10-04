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
// Automatically find the first primitive root g
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
// a * inverse mod p = 1
// ----------------------------------------------------
long long modInverse(long long a, long long p)
{
    for(long long inverse = 1; inverse < p; inverse++)
    {
        if((a * inverse) % p == 1) return inverse;
    }

    return -1;
}


int main()
{
    long long p, x, M, k;

    // ------------------------------------------------
    // STEP 1: Enter prime p
    // ------------------------------------------------
    cout << "Enter prime p: ";
    cin >> p;


    // ------------------------------------------------
    // STEP 2: Automatically find generator g
    // ------------------------------------------------
    long long g = findPrimitiveRoot(p);

    if(g == -1)
    {
        cout << "Generator not found." << endl;
        return 0;
    }


    // ------------------------------------------------
    // STEP 3: Choose private key x
    // ------------------------------------------------
    cout << "Enter private key x: ";
    cin >> x;


    // ------------------------------------------------
    // Calculate public value:
    //
    // y = g^x mod p
    //
    // Public Key  = (p, g, y)
    // Private Key = x
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
    // STEP 4: Enter message M
    // ------------------------------------------------
    cout << "\nEnter message M: ";
    cin >> M;

    if(M <= 0 || M >= p)
    {
        cout << "Message must be between 1 and " << p - 1 << endl;
        return 0;
    }


    // ------------------------------------------------
    // STEP 5: Choose random k
    // ------------------------------------------------
    cout << "Enter random k: ";
    cin >> k;


    // =================================================
    // ENCRYPTION
    // =================================================

    // First ciphertext:
    //
    // C1 = g^k mod p
    long long C1 = modPower(g, k, p);


    // Shared secret:
    //
    // y^k mod p
    long long shared = modPower(y, k, p);


    // Second ciphertext:
    //
    // C2 = M * y^k mod p
    long long C2 = (M * shared) % p;


    cout << "\n----- Encryption -----" << endl;
    cout << "C1 = " << C1 << endl;
    cout << "C2 = " << C2 << endl;
    cout << "Ciphertext = (" << C1 << ", " << C2 << ")" << endl;


    // =================================================
    // DECRYPTION
    // =================================================

    // Calculate:
    //
    // s = C1^x mod p
    long long s = modPower(C1, x, p);


    // Find:
    //
    // s^(-1) mod p
    long long sInverse = modInverse(s, p);

    if(sInverse == -1)
    {
        cout << "Could not find inverse." << endl;
        return 0;
    }


    // Recover plaintext:
    //
    // M = C2 * s^(-1) mod p
    long long decryptedM = (C2 * sInverse) % p;


    cout << "\n----- Decryption -----" << endl;
    cout << "s = " << s << endl;
    cout << "s inverse = " << sInverse << endl;
    cout << "Decrypted M = " << decryptedM << endl;


    return 0;
}


/*
====================================================
INPUT FORMAT
====================================================

Enter prime p: 23
Enter private key x: 6
Enter message M: 10
Enter random k: 3


EXPECTED IMPORTANT VALUES:

g = 5
y = 8

C1 = 10
C2 = 14

Ciphertext = (10, 14)

s = 6
s inverse = 4

Decrypted M = 10

====================================================
*/