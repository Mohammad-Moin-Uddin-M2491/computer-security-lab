#include <iostream>
using namespace std;


// ----------------------------------------------------
// Find GCD of two numbers
//
// Used to choose e such that:
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
// Find public exponent e
//
// Conditions:
// 1 < e < phi
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
// d is the modular inverse of e modulo phi.
//
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


int main()
{
    long long p1, q1, p2, q2, M;


    // =================================================
    // KEY PAIR 1
    //
    // Used for ENCRYPTION and DECRYPTION
    // =================================================

    cout << "Enter p1: ";
    cin >> p1;

    cout << "Enter q1: ";
    cin >> q1;


    // n1 = p1 * q1
    long long n1 = p1 * q1;

    // phi1 = (p1-1)(q1-1)
    long long phi1 = (p1 - 1) * (q1 - 1);

    // Find public and private exponents
    long long e1 = findE(phi1);
    long long d1 = findD(e1, phi1);


    cout << "\n----- Encryption/Decryption Key Pair -----" << endl;
    cout << "n1 = " << n1 << endl;
    cout << "phi1 = " << phi1 << endl;
    cout << "e1 = " << e1 << endl;
    cout << "d1 = " << d1 << endl;
    cout << "Public Key 1 = (" << e1 << ", " << n1 << ")" << endl;
    cout << "Private Key 1 = (" << d1 << ", " << n1 << ")" << endl;


    // =================================================
    // KEY PAIR 2
    //
    // Used for SIGNATURE and VERIFICATION
    // =================================================

    cout << "\nEnter p2: ";
    cin >> p2;

    cout << "Enter q2: ";
    cin >> q2;


    // n2 = p2 * q2
    long long n2 = p2 * q2;

    // phi2 = (p2-1)(q2-1)
    long long phi2 = (p2 - 1) * (q2 - 1);

    // Find public and private exponents
    long long e2 = findE(phi2);
    long long d2 = findD(e2, phi2);


    cout << "\n----- Signature/Verification Key Pair -----" << endl;
    cout << "n2 = " << n2 << endl;
    cout << "phi2 = " << phi2 << endl;
    cout << "e2 = " << e2 << endl;
    cout << "d2 = " << d2 << endl;
    cout << "Public Key 2 = (" << e2 << ", " << n2 << ")" << endl;
    cout << "Private Key 2 = (" << d2 << ", " << n2 << ")" << endl;


    // =================================================
    // MESSAGE
    // =================================================

    cout << "\nEnter message M: ";
    cin >> M;

    // Message must fit under both RSA moduli
    if(M < 0 || M >= n1 || M >= n2)
    {
        cout << "Message must be smaller than both n1 and n2." << endl;
        return 0;
    }


    // =================================================
    // RSA ENCRYPTION
    //
    // Use Public Key 1
    //
    // C = M^e1 mod n1
    // =================================================

    long long C = modPower(M, e1, n1);


    cout << "\n----- Encryption -----" << endl;
    cout << "Original Message = " << M << endl;
    cout << "Ciphertext C = " << C << endl;


    // =================================================
    // RSA DECRYPTION
    //
    // Use Private Key 1
    //
    // M = C^d1 mod n1
    // =================================================

    long long decryptedM = modPower(C, d1, n1);


    cout << "\n----- Decryption -----" << endl;
    cout << "Ciphertext C = " << C << endl;
    cout << "Decrypted Message = " << decryptedM << endl;


    // =================================================
    // RSA SIGNATURE
    //
    // Use Private Key 2
    //
    // S = M^d2 mod n2
    // =================================================

    long long S = modPower(M, d2, n2);


    cout << "\n----- Digital Signature -----" << endl;
    cout << "Message M = " << M << endl;
    cout << "Signature S = " << S << endl;


    // =================================================
    // RSA VERIFICATION
    //
    // Use Public Key 2
    //
    // V = S^e2 mod n2
    //
    // Signature is valid if:
    //
    // V == M
    // =================================================

    long long V = modPower(S, e2, n2);


    cout << "\n----- Signature Verification -----" << endl;
    cout << "Original Message M = " << M << endl;
    cout << "Verified Value V = " << V << endl;


    if(V == M)
        cout << "Signature is VALID." << endl;
    else
        cout << "Signature is INVALID." << endl;


    return 0;
}


/*
====================================================
INPUT FORMAT
====================================================

Encryption/Decryption Key Pair:

Enter p1: 11
Enter q1: 13


Signature/Verification Key Pair:

Enter p2: 17
Enter q2: 19


Message:

Enter message M: 5


====================================================
MAIN IDEA
====================================================

KEY PAIR 1:

n1 = p1 * q1

Public Key 1  = (e1, n1)
Private Key 1 = (d1, n1)

Encryption:
C = M^e1 mod n1

Decryption:
M = C^d1 mod n1


KEY PAIR 2:

n2 = p2 * q2

Public Key 2  = (e2, n2)
Private Key 2 = (d2, n2)

Signature:
S = M^d2 mod n2

Verification:
V = S^e2 mod n2

If:
V == M

Signature is VALID.

====================================================
*/