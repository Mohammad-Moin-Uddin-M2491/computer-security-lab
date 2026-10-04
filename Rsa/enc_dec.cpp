#include <iostream>
using namespace std;


// ----------------------------------------------------
// Find GCD of two numbers.
//
// We need this when selecting e.
// e must satisfy:
//
// gcd(e, phi) = 1
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
// Find public exponent e.
//
// Conditions:
//
// 1 < e < phi
// gcd(e, phi) = 1
//
// We simply select the first valid e.
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
// Find private exponent d.
//
// d is the modular inverse of e modulo phi.
//
// We need:
//
// e * d mod phi = 1
//
// For beginner understanding, we use a simple loop.
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
// Fast modular exponentiation.
//
// Calculates:
//
// base^exp mod mod
//
// We use this because powers such as M^e or C^d
// can become very large.
// ----------------------------------------------------
long long modPower(long long base, long long exp, long long mod)
{
    long long result = 1;
    base = base % mod;

    while(exp > 0)
    {
        // If exponent is odd, multiply result by base
        if(exp % 2 == 1) result = (result * base) % mod;

        // Square the base and reduce modulo mod
        base = (base * base) % mod;

        // Divide exponent by 2
        exp = exp / 2;
    }

    return result;
}


int main()
{
    long long p, q, message;

    // ------------------------------------------------
    // STEP 1: Take two prime numbers p and q
    // ------------------------------------------------
    cout << "Enter prime p: ";
    cin >> p;

    cout << "Enter prime q: ";
    cin >> q;


    // ------------------------------------------------
    // STEP 2: Calculate n
    //
    // n = p * q
    //
    // n is used in both public and private keys.
    // ------------------------------------------------
    long long n = p * q;


    // ------------------------------------------------
    // STEP 3: Calculate Euler's phi
    //
    // phi = (p-1)(q-1)
    // ------------------------------------------------
    long long phi = (p - 1) * (q - 1);


    // ------------------------------------------------
    // STEP 4: Find public exponent e
    //
    // gcd(e, phi) must be 1.
    // ------------------------------------------------
    long long e = findE(phi);


    // ------------------------------------------------
    // STEP 5: Find private exponent d
    //
    // e*d mod phi = 1
    // ------------------------------------------------
    long long d = findD(e, phi);


    cout << "\n----- Key Generation -----" << endl;
    cout << "n = " << n << endl;
    cout << "phi = " << phi << endl;
    cout << "e = " << e << endl;
    cout << "d = " << d << endl;

    cout << "Public Key = (" << e << ", " << n << ")" << endl;
    cout << "Private Key = (" << d << ", " << n << ")" << endl;


    // ------------------------------------------------
    // STEP 6: Take plaintext message
    //
    // For this simple RSA program:
    //
    // 0 <= message < n
    // ------------------------------------------------
    cout << "\nEnter plaintext message: ";
    cin >> message;

    if(message < 0 || message >= n)
    {
        cout << "Message must be between 0 and " << n - 1 << endl;
        return 0;
    }


    // =================================================
    // RSA ENCRYPTION
    //
    // C = M^e mod n
    //
    // M = plaintext
    // e = public exponent
    // n = modulus
    // =================================================
    long long cipher = modPower(message, e, n);

    cout << "\n----- Encryption -----" << endl;
    cout << "Plaintext = " << message << endl;
    cout << "Ciphertext = " << cipher << endl;


    // =================================================
    // RSA DECRYPTION
    //
    // M = C^d mod n
    //
    // C = ciphertext
    // d = private exponent
    // n = modulus
    // =================================================
    long long decrypted = modPower(cipher, d, n);

    cout << "\n----- Decryption -----" << endl;
    cout << "Ciphertext = " << cipher << endl;
    cout << "Decrypted Plaintext = " << decrypted << endl;


    return 0;
}


/*
====================================================
INPUT FORMAT
====================================================

Enter prime p: 163
Enter prime q: 193
Enter plaintext message: 5


EXPECTED IMPORTANT VALUES:

n = 31459
phi = 31104
e = 5
d = 6221

Ciphertext = 3125
Decrypted Plaintext = 5

====================================================
*/