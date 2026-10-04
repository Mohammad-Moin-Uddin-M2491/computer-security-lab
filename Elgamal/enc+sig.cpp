#include <iostream>
#include <vector>
using namespace std;


// ----------------------------------------------------
// Find GCD
//
// For ElGamal signature:
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
//
// Try:
// g = 2, 3, 4, ...
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
    long long p, x1, x2, M, k1, k2;


    // =================================================
    // COMMON VALUES
    // =================================================

    cout << "Enter prime p: ";
    cin >> p;


    // Automatically find generator g
    long long g = findPrimitiveRoot(p);

    if(g == -1)
    {
        cout << "Generator not found." << endl;
        return 0;
    }


    cout << "\nGenerator g = " << g << endl;


    // =================================================
    // KEY PAIR 1
    //
    // Used for ENCRYPTION and DECRYPTION
    // =================================================

    cout << "\nEnter private key x1 for encryption/decryption: ";
    cin >> x1;


    // Public value:
    //
    // y1 = g^x1 mod p
    long long y1 = modPower(g, x1, p);


    cout << "\n----- Encryption/Decryption Key Pair -----" << endl;
    cout << "x1 = " << x1 << endl;
    cout << "y1 = " << y1 << endl;
    cout << "Public Key 1 = (" << p << ", " << g << ", " << y1 << ")" << endl;
    cout << "Private Key 1 = " << x1 << endl;


    // =================================================
    // KEY PAIR 2
    //
    // Used for SIGNATURE and VERIFICATION
    // =================================================

    cout << "\nEnter private key x2 for signature/verification: ";
    cin >> x2;


    // Public value:
    //
    // y2 = g^x2 mod p
    long long y2 = modPower(g, x2, p);


    cout << "\n----- Signature/Verification Key Pair -----" << endl;
    cout << "x2 = " << x2 << endl;
    cout << "y2 = " << y2 << endl;
    cout << "Public Key 2 = (" << p << ", " << g << ", " << y2 << ")" << endl;
    cout << "Private Key 2 = " << x2 << endl;


    // =================================================
    // MESSAGE
    // =================================================

    cout << "\nEnter message M: ";
    cin >> M;

    if(M <= 0 || M >= p)
    {
        cout << "Message must be between 1 and " << p - 1 << endl;
        return 0;
    }


    // =================================================
    // ELGAMAL ENCRYPTION
    //
    // Use Public Key 1
    //
    // C1 = g^k1 mod p
    //
    // C2 = M * y1^k1 mod p
    // =================================================

    cout << "Enter random k1 for encryption: ";
    cin >> k1;


    long long C1 = modPower(g, k1, p);
    long long C2 = (M * modPower(y1, k1, p)) % p;


    cout << "\n----- Encryption -----" << endl;
    cout << "Original Message M = " << M << endl;
    cout << "C1 = " << C1 << endl;
    cout << "C2 = " << C2 << endl;
    cout << "Ciphertext = (" << C1 << ", " << C2 << ")" << endl;


    // =================================================
    // ELGAMAL DECRYPTION
    //
    // Use Private Key x1
    //
    // s = C1^x1 mod p
    //
    // M = C2 * s^(-1) mod p
    // =================================================

    long long s = modPower(C1, x1, p);
    long long sInverse = modInverse(s, p);

    if(sInverse == -1)
    {
        cout << "Could not find inverse." << endl;
        return 0;
    }


    long long decryptedM = (C2 * sInverse) % p;


    cout << "\n----- Decryption -----" << endl;
    cout << "s = " << s << endl;
    cout << "s inverse = " << sInverse << endl;
    cout << "Decrypted Message = " << decryptedM << endl;


    // =================================================
    // ELGAMAL DIGITAL SIGNATURE
    //
    // Use Private Key x2
    //
    // k2 must satisfy:
    // gcd(k2, p-1) = 1
    //
    // A = g^k2 mod p
    //
    // sig = k2^(-1)(M - x2*A) mod (p-1)
    // =================================================

    cout << "\nEnter random k2 for signature: ";
    cin >> k2;


    if(gcd(k2, p - 1) != 1)
    {
        cout << "Invalid k2. gcd(k2, p-1) must be 1." << endl;
        return 0;
    }


    // First signature value
    long long A = modPower(g, k2, p);


    // Find inverse of k2 modulo p-1
    long long k2Inverse = modInverse(k2, p - 1);


    // Second signature value
    long long sig = (k2Inverse * (M - x2 * A)) % (p - 1);

    // C++ may give negative remainder
    if(sig < 0) sig += (p - 1);


    cout << "\n----- Digital Signature -----" << endl;
    cout << "A = " << A << endl;
    cout << "sig = " << sig << endl;
    cout << "Signature = (" << A << ", " << sig << ")" << endl;


    // =================================================
    // ELGAMAL SIGNATURE VERIFICATION
    //
    // Use Public Key y2
    //
    // V1 = g^M mod p
    //
    // V2 = y2^A * A^sig mod p
    //
    // Signature valid if:
    //
    // V1 == V2
    // =================================================

    long long V1 = modPower(g, M, p);
    long long V2 = (modPower(y2, A, p) * modPower(A, sig, p)) % p;


    cout << "\n----- Signature Verification -----" << endl;
    cout << "V1 = " << V1 << endl;
    cout << "V2 = " << V2 << endl;


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

Enter private key x1 for encryption/decryption: 6

Enter private key x2 for signature/verification: 5

Enter message M: 10

Enter random k1 for encryption: 3

Enter random k2 for signature: 7


IMPORTANT:

For encryption k1:
any suitable temporary random value can be used.

For signature k2:
gcd(k2, p-1) must be 1.

Here:

p - 1 = 22

gcd(7, 22) = 1

so k2 = 7 is valid.


====================================================
FORMULAS
====================================================

KEY 1:

y1 = g^x1 mod p


ENCRYPTION:

C1 = g^k1 mod p

C2 = M * y1^k1 mod p


DECRYPTION:

s = C1^x1 mod p

M = C2 * s^(-1) mod p


KEY 2:

y2 = g^x2 mod p


SIGNATURE:

A = g^k2 mod p

sig = k2^(-1)(M - x2*A) mod (p-1)


VERIFICATION:

V1 = g^M mod p

V2 = y2^A * A^sig mod p


If:

V1 == V2

Signature is VALID.

====================================================
*/