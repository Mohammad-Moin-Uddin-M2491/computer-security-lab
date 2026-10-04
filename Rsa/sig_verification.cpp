#include <iostream>
using namespace std;


// ----------------------------------------------------
// Find GCD of two numbers.
//
// We need gcd(e, phi) = 1
// when choosing the public exponent e.
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
// 1 < e < phi
// gcd(e, phi) = 1
//
// We choose the first valid e.
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
// Condition:
// e * d mod phi = 1
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
// base^exp mod mod
//
// We need this because numbers such as M^d
// and S^e can become very large.
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
    // STEP 2: Calculate n and phi
    //
    // n = p * q
    // phi = (p-1)(q-1)
    // ------------------------------------------------
    long long n = p * q;
    long long phi = (p - 1) * (q - 1);


    // ------------------------------------------------
    // STEP 3: Generate public exponent e
    // and private exponent d
    // ------------------------------------------------
    long long e = findE(phi);
    long long d = findD(e, phi);


    cout << "\n----- Key Generation -----" << endl;
    cout << "n = " << n << endl;
    cout << "phi = " << phi << endl;
    cout << "e = " << e << endl;
    cout << "d = " << d << endl;

    cout << "Public Key = (" << e << ", " << n << ")" << endl;
    cout << "Private Key = (" << d << ", " << n << ")" << endl;


    // ------------------------------------------------
    // STEP 4: Take the message
    //
    // For this simple textbook RSA signature,
    // the message itself is signed directly.
    //
    // Message should satisfy:
    // 0 <= message < n
    // ------------------------------------------------
    cout << "\nEnter message: ";
    cin >> message;

    if(message < 0 || message >= n)
    {
        cout << "Message must be between 0 and " << n - 1 << endl;
        return 0;
    }


    // =================================================
    // RSA SIGNATURE
    //
    // Signature is created using PRIVATE key d.
    //
    // S = M^d mod n
    //
    // M = message
    // d = private exponent
    // =================================================
    long long signature = modPower(message, d, n);


    cout << "\n----- Digital Signature -----" << endl;
    cout << "Message = " << message << endl;
    cout << "Signature = " << signature << endl;


    // =================================================
    // RSA SIGNATURE VERIFICATION
    //
    // Receiver uses PUBLIC key e.
    //
    // VerifiedMessage = S^e mod n
    //
    // If VerifiedMessage == original message,
    // then signature is valid.
    // =================================================
    long long verifiedMessage = modPower(signature, e, n);


    cout << "\n----- Signature Verification -----" << endl;
    cout << "Verified Message = " << verifiedMessage << endl;


    // Compare recovered value with original message
    if(verifiedMessage == message)
        cout << "Signature is VALID." << endl;
    else
        cout << "Signature is INVALID." << endl;


    return 0;
}


/*
====================================================
INPUT FORMAT
====================================================

Enter prime p: 163
Enter prime q: 193
Enter message: 5


EXPECTED IMPORTANT VALUES:

n = 31459
phi = 31104
e = 5
d = 6221

Signature = 15493
Verified Message = 5

Signature is VALID.

====================================================
*/