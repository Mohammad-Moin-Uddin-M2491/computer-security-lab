#include <iostream>
#include <vector>
using namespace std;


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
// Check whether g is primitive root
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
    long long p, x, M1, M2, k1, k2;


    // ------------------------------------------------
    // Key generation
    // ------------------------------------------------
    cout << "Enter prime p: ";
    cin >> p;

    long long g = findPrimitiveRoot(p);

    if(g == -1)
    {
        cout << "Generator not found." << endl;
        return 0;
    }


    cout << "Enter private key x: ";
    cin >> x;


    // Public value:
    //
    // y = g^x mod p
    long long y = modPower(g, x, p);


    cout << "\n----- Key Generation -----" << endl;
    cout << "g = " << g << endl;
    cout << "y = " << y << endl;
    cout << "Public Key = (" << p << ", " << g << ", " << y << ")" << endl;
    cout << "Private Key = " << x << endl;


    // ------------------------------------------------
    // Take two messages
    // ------------------------------------------------
    cout << "\nEnter M1: ";
    cin >> M1;

    cout << "Enter M2: ";
    cin >> M2;


    // Each encryption uses its own random k
    cout << "Enter random k1: ";
    cin >> k1;

    cout << "Enter random k2: ";
    cin >> k2;


    // =================================================
    // ENCRYPT M1
    //
    // C11 = g^k1 mod p
    // C12 = M1 * y^k1 mod p
    // =================================================
    long long C11 = modPower(g, k1, p);
    long long C12 = (M1 * modPower(y, k1, p)) % p;


    // =================================================
    // ENCRYPT M2
    //
    // C21 = g^k2 mod p
    // C22 = M2 * y^k2 mod p
    // =================================================
    long long C21 = modPower(g, k2, p);
    long long C22 = (M2 * modPower(y, k2, p)) % p;


    cout << "\n----- Individual Encryption -----" << endl;
    cout << "Ciphertext of M1 = (" << C11 << ", " << C12 << ")" << endl;
    cout << "Ciphertext of M2 = (" << C21 << ", " << C22 << ")" << endl;


    // =================================================
    // HOMOMORPHIC MULTIPLICATION
    //
    // Multiply ciphertexts component-wise:
    //
    // C1 = C11 * C21 mod p
    // C2 = C12 * C22 mod p
    // =================================================
    long long C1 = (C11 * C21) % p;
    long long C2 = (C12 * C22) % p;


    cout << "\n----- Combined Ciphertext -----" << endl;
    cout << "C1 = " << C1 << endl;
    cout << "C2 = " << C2 << endl;
    cout << "Combined Ciphertext = (" << C1 << ", " << C2 << ")" << endl;


    // =================================================
    // DECRYPT COMBINED CIPHERTEXT
    //
    // s = C1^x mod p
    // M = C2 * s^(-1) mod p
    // =================================================
    long long s = modPower(C1, x, p);
    long long sInverse = modInverse(s, p);

    if(sInverse == -1)
    {
        cout << "Could not find inverse." << endl;
        return 0;
    }


    long long decryptedProduct = (C2 * sInverse) % p;


    // Expected answer
    long long expectedProduct = (M1 * M2) % p;


    cout << "\n----- Homomorphic Result -----" << endl;
    cout << "Expected M1 * M2 mod p = " << expectedProduct << endl;
    cout << "Decrypted Result = " << decryptedProduct << endl;


    if(decryptedProduct == expectedProduct)
        cout << "Homomorphic property VERIFIED." << endl;
    else
        cout << "Homomorphic property NOT verified." << endl;


    return 0;
}


/*
====================================================
INPUT FORMAT
====================================================

Enter prime p: 23
Enter private key x: 6
Enter M1: 4
Enter M2: 5
Enter random k1: 3
Enter random k2: 7


EXPECTED IMPORTANT RESULT:

g = 5
y = 8

M1 * M2 mod p
= 4 * 5 mod 23
= 20

Decrypted Result = 20

Homomorphic property VERIFIED.

====================================================
*/