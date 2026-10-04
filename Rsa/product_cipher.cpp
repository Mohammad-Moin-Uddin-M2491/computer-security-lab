#include <iostream>
using namespace std;


// ----------------------------------------------------
// Find GCD
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
// Automatically find public exponent e
//
// gcd(e, phi) = 1
// ----------------------------------------------------
long long findE(long long phi)
{
    for(long long e = 2; e < phi; e++)
    {
        if(gcd(e, phi) == 1) return e;
    }

    return -1;
}


// ----------------------------------------------------
// Find private exponent d
//
// e*d mod phi = 1
// ----------------------------------------------------
long long findD(long long e, long long phi)
{
    for(long long d = 1; d < phi; d++)
    {
        if((e * d) % phi == 1) return d;
    }

    return -1;
}


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


int main()
{
    long long p1, q1, p2, q2, M;


    // =================================================
    // RSA KEY PAIR 1
    // =================================================

    cout << "Enter p1: ";
    cin >> p1;

    cout << "Enter q1: ";
    cin >> q1;


    long long n1 = p1 * q1;
    long long phi1 = (p1 - 1) * (q1 - 1);

    long long e1 = findE(phi1);
    long long d1 = findD(e1, phi1);


    cout << "\n----- RSA Key Pair 1 -----" << endl;
    cout << "n1 = " << n1 << endl;
    cout << "phi1 = " << phi1 << endl;
    cout << "e1 = " << e1 << endl;
    cout << "d1 = " << d1 << endl;


    // =================================================
    // RSA KEY PAIR 2
    //
    // For this simple product-cipher demonstration,
    // use n2 > n1.
    //
    // Then C1 < n1 < n2, so C1 can safely become
    // the input of the second RSA encryption.
    // =================================================

    cout << "\nEnter p2: ";
    cin >> p2;

    cout << "Enter q2: ";
    cin >> q2;


    long long n2 = p2 * q2;
    long long phi2 = (p2 - 1) * (q2 - 1);

    long long e2 = findE(phi2);
    long long d2 = findD(e2, phi2);


    if(n2 <= n1)
    {
        cout << "\nFor this demo, choose p2 and q2 so that n2 > n1." << endl;
        return 0;
    }


    cout << "\n----- RSA Key Pair 2 -----" << endl;
    cout << "n2 = " << n2 << endl;
    cout << "phi2 = " << phi2 << endl;
    cout << "e2 = " << e2 << endl;
    cout << "d2 = " << d2 << endl;


    // =================================================
    // MESSAGE
    // =================================================

    cout << "\nEnter message M: ";
    cin >> M;


    if(M < 0 || M >= n1)
    {
        cout << "Message must be smaller than n1." << endl;
        return 0;
    }


    // =================================================
    // FIRST RSA ENCRYPTION
    //
    // C1 = M^e1 mod n1
    // =================================================

    long long C1 = modPower(M, e1, n1);


    cout << "\n----- First Encryption -----" << endl;
    cout << "Original Message = " << M << endl;
    cout << "C1 = " << C1 << endl;


    // =================================================
    // SECOND RSA ENCRYPTION
    //
    // Encrypt C1 again using RSA Key 2.
    //
    // C2 = C1^e2 mod n2
    //
    // C2 is the final product ciphertext.
    // =================================================

    long long C2 = modPower(C1, e2, n2);


    cout << "\n----- Second Encryption -----" << endl;
    cout << "Input C1 = " << C1 << endl;
    cout << "Final Ciphertext C2 = " << C2 << endl;


    // =================================================
    // DECRYPT SECOND LAYER FIRST
    //
    // recoveredC1 = C2^d2 mod n2
    // =================================================

    long long recoveredC1 = modPower(C2, d2, n2);


    cout << "\n----- First Decryption -----" << endl;
    cout << "Recovered C1 = " << recoveredC1 << endl;


    // =================================================
    // DECRYPT FIRST LAYER
    //
    // recoveredM = recoveredC1^d1 mod n1
    // =================================================

    long long recoveredM = modPower(recoveredC1, d1, n1);


    cout << "\n----- Final Decryption -----" << endl;
    cout << "Original Message = " << M << endl;
    cout << "Recovered Message = " << recoveredM << endl;


    // =================================================
    // VERIFICATION
    // =================================================

    if(recoveredM == M)
        cout << "RSA Product Cipher SUCCESSFUL." << endl;
    else
        cout << "RSA Product Cipher FAILED." << endl;


    return 0;
}


/*
================ INPUT FORMAT ================

p1 = 11
q1 = 13

p2 = 17
q2 = 19

M = 9

Input:

11
13
17
19
9

Expected:

n1 = 143
n2 = 323

First encryption:
M -> C1

Second encryption:
C1 -> C2

Then decryption:

C2 -> C1 -> M

Recovered Message = 9

==============================================
*/