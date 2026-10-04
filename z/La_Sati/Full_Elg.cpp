#include <iostream>
using namespace std;


// --------------------------------------------------
// Function to find GCD
// Used when finding modular inverse
// --------------------------------------------------
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


// --------------------------------------------------
// Fast modular exponentiation
//
// Calculates:
//
// base^exp mod mod
//
// We use this because powers become very large
// --------------------------------------------------
long long modPower(long long base, long long exp, long long mod)
{
    long long result = 1;

    base = base % mod;

    while (exp > 0)
    {
        // If exponent is odd
        if (exp % 2 == 1)
        {
            result = (result * base) % mod;
        }

        // Square the base
        base = (base * base) % mod;

        // Divide exponent by 2
        exp = exp / 2;
    }

    return result;
}


// --------------------------------------------------
// Find modular inverse
//
// We need inverse of a modulo mod:
//
// (a * inverse) % mod = 1
//
// For beginner understanding,
// we find it using a simple loop
// --------------------------------------------------
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
    // --------------------------------------------------
    // Step 1: Input prime p and generator g
    //
    // Example:
    // p = 23
    // g = 5
    // --------------------------------------------------

    long long p, g;

    cout << "Enter prime number p: ";
    cin >> p;

    cout << "Enter generator g: ";
    cin >> g;


    // --------------------------------------------------
    // Step 2: Choose private key x
    //
    // x must satisfy:
    //
    // 1 < x < p - 1
    //
    // x is SECRET
    // --------------------------------------------------

    long long x;

    cout << "Enter private key x: ";
    cin >> x;


    // --------------------------------------------------
    // Step 3: Calculate public value y
    //
    // y = g^x mod p
    //
    // Public key = (p, g, y)
    // Private key = x
    // --------------------------------------------------

    long long y = modPower(g, x, p);


    cout << "\n----- Key Generation -----" << endl;

    cout << "p = " << p << endl;
    cout << "g = " << g << endl;

    cout << "Private Key x = " << x << endl;

    cout << "Public value y = " << y << endl;

    cout << "Public Key = (" << p << ", " << g << ", " << y << ")" << endl;


    // --------------------------------------------------
    // Step 4: Input message
    //
    // Message should be less than p
    // --------------------------------------------------

    long long message;

    cout << "\nEnter message: ";
    cin >> message;


    // --------------------------------------------------
    // Step 5: Choose random temporary key k
    //
    // k is used only for this encryption
    //
    // Normally k should be random
    //
    // For lab understanding,
    // we take it from input
    // --------------------------------------------------

    long long k;

    cout << "Enter random value k: ";
    cin >> k;


    // ==================================================
    // ELGAMAL ENCRYPTION
    // ==================================================


    // --------------------------------------------------
    // Calculate C1
    //
    // C1 = g^k mod p
    // --------------------------------------------------

    long long C1 = modPower(g, k, p);


    // --------------------------------------------------
    // Calculate shared secret
    //
    // sharedSecret = y^k mod p
    //
    // This value is used to hide the message
    // --------------------------------------------------

    long long sharedSecret = modPower(y, k, p);


    // --------------------------------------------------
    // Calculate C2
    //
    // C2 = message * sharedSecret mod p
    // --------------------------------------------------

    long long C2 = (message * sharedSecret) % p;


    cout << "\n----- Encryption -----" << endl;

    cout << "Original Message = " << message << endl;

    cout << "C1 = " << C1 << endl;
    cout << "C2 = " << C2 << endl;

    cout << "Ciphertext = (" << C1 << ", " << C2 << ")" << endl;


    // ==================================================
    // ELGAMAL DECRYPTION
    // ==================================================


    // --------------------------------------------------
    // Receiver recreates the same shared secret
    //
    // s = C1^x mod p
    //
    // Because:
    //
    // C1 = g^k
    //
    // so:
    //
    // C1^x = g^(kx)
    //
    // This is the same shared secret used by sender
    // --------------------------------------------------

    long long s = modPower(C1, x, p);


    // --------------------------------------------------
    // Find modular inverse of s
    //
    // We need:
    //
    // sInverse such that
    //
    // (s * sInverse) % p = 1
    // --------------------------------------------------

    long long sInverse = modInverse(s, p);


    if (sInverse == -1)
    {
        cout << "Could not find modular inverse." << endl;
        return 0;
    }


    // --------------------------------------------------
    // Recover original message
    //
    // message = C2 * sInverse mod p
    // --------------------------------------------------

    long long decryptedMessage = (C2 * sInverse) % p;


    cout << "\n----- Decryption -----" << endl;

    cout << "Shared Secret = " << s << endl;

    cout << "Inverse of Shared Secret = " << sInverse << endl;

    cout << "Decrypted Message = " << decryptedMessage << endl;


    return 0;
}