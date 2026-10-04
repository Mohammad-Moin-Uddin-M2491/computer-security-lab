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
    base %= mod;

    while(exp > 0)
    {
        if(exp % 2 == 1) result = (result * base) % mod;

        base = (base * base) % mod;
        exp /= 2;
    }

    return result;
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
// Automatically find primitive root g
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
    long long p1, p2, x1, x2, M;
    long long k1, k2, k3;


    // =================================================
    // FIRST ELGAMAL KEY PAIR
    // =================================================

    cout << "Enter first prime p1: ";
    cin >> p1;

    long long g1 = findPrimitiveRoot(p1);

    cout << "Enter first private key x1: ";
    cin >> x1;

    long long y1 = modPower(g1, x1, p1);


    cout << "\n----- First Key Pair -----" << endl;
    cout << "g1 = " << g1 << endl;
    cout << "y1 = " << y1 << endl;


    // =================================================
    // SECOND ELGAMAL KEY PAIR
    //
    // Use p2 > p1 so first ciphertext components
    // can be preserved as messages in stage 2.
    // =================================================

    cout << "\nEnter second prime p2: ";
    cin >> p2;

    if(p2 <= p1)
    {
        cout << "For this simple demo, use p2 > p1." << endl;
        return 0;
    }

    long long g2 = findPrimitiveRoot(p2);

    cout << "Enter second private key x2: ";
    cin >> x2;

    long long y2 = modPower(g2, x2, p2);


    cout << "\n----- Second Key Pair -----" << endl;
    cout << "g2 = " << g2 << endl;
    cout << "y2 = " << y2 << endl;


    // =================================================
    // MESSAGE
    // =================================================

    cout << "\nEnter message M: ";
    cin >> M;

    cout << "Enter random k1 for first encryption: ";
    cin >> k1;


    // =================================================
    // FIRST ELGAMAL ENCRYPTION
    //
    // C1 = g1^k1 mod p1
    //
    // C2 = M * y1^k1 mod p1
    // =================================================

    long long C1 = modPower(g1, k1, p1);
    long long C2 = (M * modPower(y1, k1, p1)) % p1;


    cout << "\n----- First Encryption -----" << endl;
    cout << "C1 = " << C1 << endl;
    cout << "C2 = " << C2 << endl;
    cout << "First Ciphertext = (" << C1 << ", " << C2 << ")" << endl;


    // =================================================
    // SECOND ELGAMAL ENCRYPTION
    //
    // The first ciphertext contains TWO values:
    //
    // C1 and C2
    //
    // So encrypt each value using the second key.
    // =================================================

    cout << "\nEnter random k2 for encrypting C1: ";
    cin >> k2;

    cout << "Enter random k3 for encrypting C2: ";
    cin >> k3;


    // Encrypt C1 using second ElGamal key
    long long D11 = modPower(g2, k2, p2);
    long long D12 = (C1 * modPower(y2, k2, p2)) % p2;


    // Encrypt C2 using second ElGamal key
    long long D21 = modPower(g2, k3, p2);
    long long D22 = (C2 * modPower(y2, k3, p2)) % p2;


    cout << "\n----- Second Encryption -----" << endl;

    cout << "Encrypted C1 = (" << D11 << ", " << D12 << ")" << endl;
    cout << "Encrypted C2 = (" << D21 << ", " << D22 << ")" << endl;

    cout << "\nFinal Product Ciphertext = ";
    cout << "((" << D11 << ", " << D12 << "), ";
    cout << "(" << D21 << ", " << D22 << "))" << endl;


    // =================================================
    // DECRYPT SECOND LAYER
    //
    // Recover original C1 and C2.
    // =================================================

    long long s1 = modPower(D11, x2, p2);
    long long s1Inverse = modInverse(s1, p2);

    long long recoveredC1 = (D12 * s1Inverse) % p2;


    long long s2 = modPower(D21, x2, p2);
    long long s2Inverse = modInverse(s2, p2);

    long long recoveredC2 = (D22 * s2Inverse) % p2;


    cout << "\n----- Decrypt Second Layer -----" << endl;
    cout << "Recovered C1 = " << recoveredC1 << endl;
    cout << "Recovered C2 = " << recoveredC2 << endl;

    cout << "Recovered First Ciphertext = ("
         << recoveredC1 << ", " << recoveredC2 << ")" << endl;


    // =================================================
    // DECRYPT FIRST LAYER
    //
    // s = C1^x1 mod p1
    //
    // M = C2 * s^(-1) mod p1
    // =================================================

    long long s = modPower(recoveredC1, x1, p1);
    long long sInverse = modInverse(s, p1);

    long long recoveredM = (recoveredC2 * sInverse) % p1;


    cout << "\n----- Final Decryption -----" << endl;
    cout << "Original Message = " << M << endl;
    cout << "Recovered Message = " << recoveredM << endl;


    if(recoveredM == M)
        cout << "Product Cipher SUCCESSFUL." << endl;
    else
        cout << "Product Cipher FAILED." << endl;


    return 0;
}


/*
================ INPUT FORMAT ================

p1 = 23
x1 = 6

p2 = 47
x2 = 5

M = 10

k1 = 3
k2 = 3
k3 = 7

Input:

23
6
47
5
10
3
3
7

Expected:
First Ciphertext = (10, 14)

Recovered C1 = 10
Recovered C2 = 14

Recovered Message = 10

==============================================
*/