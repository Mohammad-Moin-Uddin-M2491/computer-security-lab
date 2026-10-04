#include <iostream>
using namespace std;


// ----------------------------------------------------
// Find GCD of two numbers
//
// Needed to choose e such that:
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
    long long p, q, M1, M2;


    // ------------------------------------------------
    // STEP 1: Enter two prime numbers
    // ------------------------------------------------
    cout << "Enter prime p: ";
    cin >> p;

    cout << "Enter prime q: ";
    cin >> q;


    // ------------------------------------------------
    // STEP 2: RSA key generation
    //
    // n = p * q
    // phi = (p-1)(q-1)
    // ------------------------------------------------
    long long n = p * q;
    long long phi = (p - 1) * (q - 1);

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
    // STEP 3: Enter two plaintext messages
    // ------------------------------------------------
    cout << "\nEnter M1: ";
    cin >> M1;

    cout << "Enter M2: ";
    cin >> M2;

    if(M1 < 0 || M1 >= n || M2 < 0 || M2 >= n)
    {
        cout << "Messages must be between 0 and " << n - 1 << endl;
        return 0;
    }


    // =================================================
    // ENCRYPT M1
    //
    // C1 = M1^e mod n
    // =================================================
    long long C1 = modPower(M1, e, n);


    // =================================================
    // ENCRYPT M2
    //
    // C2 = M2^e mod n
    // =================================================
    long long C2 = modPower(M2, e, n);


    cout << "\n----- Individual Encryption -----" << endl;
    cout << "Ciphertext of M1 = " << C1 << endl;
    cout << "Ciphertext of M2 = " << C2 << endl;


    // =================================================
    // HOMOMORPHIC MULTIPLICATION
    //
    // Multiply the two ciphertexts:
    //
    // C = C1 * C2 mod n
    //
    // This should represent encryption of:
    //
    // M1 * M2 mod n
    // =================================================
    long long C = (C1 * C2) % n;


    cout << "\n----- Homomorphic Multiplication -----" << endl;
    cout << "Combined Ciphertext = " << C << endl;


    // =================================================
    // DECRYPT COMBINED CIPHERTEXT
    //
    // M = C^d mod n
    //
    // Result should be:
    //
    // M1 * M2 mod n
    // =================================================
    long long decryptedProduct = modPower(C, d, n);


    // Calculate expected result directly
    long long expectedProduct = (M1 * M2) % n;


    cout << "\n----- Homomorphic Result -----" << endl;
    cout << "Expected M1 * M2 mod n = " << expectedProduct << endl;
    cout << "Decrypted Combined Result = " << decryptedProduct << endl;


    // ------------------------------------------------
    // Verify homomorphic property
    // ------------------------------------------------
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

Enter prime p: 11
Enter prime q: 13
Enter M1: 5
Enter M2: 7


EXPECTED IMPORTANT RESULT:

n = 143

M1 * M2 mod n
= 5 * 7 mod 143
= 35

Decrypted Combined Result = 35

Homomorphic property VERIFIED.

====================================================
*/