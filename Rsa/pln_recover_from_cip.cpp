#include <iostream>
using namespace std;


// ----------------------------------------------------
// Find GCD of two numbers.
//
// We use this to check that:
// gcd(e, phi) = 1
//
// Otherwise e will not have a modular inverse.
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
// Find private exponent d.
//
// d is the modular inverse of e modulo phi.
//
// We need:
//
// e * d mod phi = 1
//
// Example:
// if e = 5 and phi = 31104,
// d = 6221 because:
//
// 5 * 6221 mod 31104 = 1
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
// We need this because C^d can become extremely large.
// ----------------------------------------------------
long long modPower(long long base, long long exp, long long mod)
{
    long long result = 1;
    base = base % mod;

    while(exp > 0)
    {
        // If exponent is odd, multiply result by base
        if(exp % 2 == 1) result = (result * base) % mod;

        // Square the base
        base = (base * base) % mod;

        // Reduce exponent by half
        exp = exp / 2;
    }

    return result;
}


int main()
{
    long long p, q, e, cipher;

    // ------------------------------------------------
    // STEP 1: Take the two prime numbers.
    // ------------------------------------------------
    cout << "Enter prime p: ";
    cin >> p;

    cout << "Enter prime q: ";
    cin >> q;


    // ------------------------------------------------
    // STEP 2: Calculate RSA modulus.
    //
    // n = p * q
    // ------------------------------------------------
    long long n = p * q;


    // ------------------------------------------------
    // STEP 3: Calculate Euler's phi.
    //
    // phi = (p-1)(q-1)
    // ------------------------------------------------
    long long phi = (p - 1) * (q - 1);


    // ------------------------------------------------
    // STEP 4: Take public exponent e.
    //
    // e must be coprime with phi.
    // ------------------------------------------------
    cout << "Enter public exponent e: ";
    cin >> e;

    if(gcd(e, phi) != 1)
    {
        cout << "Invalid e. gcd(e, phi) must be 1." << endl;
        return 0;
    }


    // ------------------------------------------------
    // STEP 5: Find private exponent d.
    //
    // e*d mod phi = 1
    // ------------------------------------------------
    long long d = findD(e, phi);

    if(d == -1)
    {
        cout << "Could not find private exponent d." << endl;
        return 0;
    }


    // ------------------------------------------------
    // STEP 6: Take the ciphertext.
    // ------------------------------------------------
    cout << "Enter ciphertext: ";
    cin >> cipher;


    cout << "\n----- RSA Values -----" << endl;
    cout << "n = " << n << endl;
    cout << "phi = " << phi << endl;
    cout << "Public exponent e = " << e << endl;
    cout << "Recovered private exponent d = " << d << endl;


    // =================================================
    // PLAINTEXT RECOVERY
    //
    // RSA decryption formula:
    //
    // M = C^d mod n
    //
    // C = ciphertext
    // d = private exponent
    // M = recovered plaintext
    // =================================================
    long long plaintext = modPower(cipher, d, n);


    cout << "\n----- Plaintext Recovery -----" << endl;
    cout << "Ciphertext = " << cipher << endl;
    cout << "Recovered Plaintext = " << plaintext << endl;


    return 0;
}


/*
====================================================
INPUT FORMAT
====================================================

Enter prime p: 163
Enter prime q: 193
Enter public exponent e: 5
Enter ciphertext: 3125


EXPECTED IMPORTANT VALUES:

n = 31459
phi = 31104
d = 6221

Recovered Plaintext = 5

====================================================
*/