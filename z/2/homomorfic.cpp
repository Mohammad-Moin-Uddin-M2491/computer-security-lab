#include <iostream>
#include <vector>
using namespace std;


// Calculates: base^exp mod mod
long long modPower(long long base, long long exp, long long mod)
{
    long long result = 1;
    base = base % mod;

    while (exp > 0)
    {
        if (exp % 2 == 1)
        {
            result = (result * base) % mod;
        }

        base = (base * base) % mod;
        exp = exp / 2;
    }

    return result;
}


// Checks whether alpha is a primitive root of p
bool isPrimitiveRoot(long long alpha, long long p)
{
    vector<bool> used(p, false);

    for (long long i = 1; i <= p - 1; i++)
    {
        long long value = modPower(alpha, i, p);

        if (used[value])
        {
            return false;
        }

        used[value] = true;
    }

    return true;
}


// Automatically finds the first primitive root alpha
long long findPrimitiveRoot(long long p)
{
    for (long long alpha = 2; alpha < p; alpha++)
    {
        if (isPrimitiveRoot(alpha, p))
        {
            return alpha;
        }
    }

    return -1;
}


// Finds modular inverse
// (a * inverse) % mod = 1
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
    long long p;

    cout << "Enter prime number p: ";
    cin >> p;


    // Automatically find generator alpha
    long long alpha = findPrimitiveRoot(p);

    if (alpha == -1)
    {
        cout << "Primitive root not found." << endl;
        return 0;
    }


    // Private key
    long long x;

    cout << "Enter private key x: ";
    cin >> x;


    // beta = alpha^x mod p
    long long beta = modPower(alpha, x, p);


    cout << "\n----- Key Generation -----" << endl;
    cout << "alpha = " << alpha << endl;
    cout << "beta = " << beta << endl;
    cout << "Public Key = (" << alpha << ", " << beta << ", " << p << ")" << endl;
    cout << "Private Key = " << x << endl;


    // Two messages
    long long m1, m2;

    cout << "\nEnter first message m1: ";
    cin >> m1;

    cout << "Enter second message m2: ";
    cin >> m2;


    // Use different random values for the two encryptions
    long long k1, k2;

    cout << "Enter random k1: ";
    cin >> k1;

    cout << "Enter random k2: ";
    cin >> k2;


    // ====================================
    // ENCRYPT MESSAGE m1
    // ====================================

    // y11 = alpha^k1 mod p
    long long y11 = modPower(alpha, k1, p);

    // y12 = m1 * beta^k1 mod p
    long long y12 = (m1 * modPower(beta, k1, p)) % p;


    // ====================================
    // ENCRYPT MESSAGE m2
    // ====================================

    // y21 = alpha^k2 mod p
    long long y21 = modPower(alpha, k2, p);

    // y22 = m2 * beta^k2 mod p
    long long y22 = (m2 * modPower(beta, k2, p)) % p;


    cout << "\n----- Individual Encryption -----" << endl;
    cout << "Ciphertext of m1 = (" << y11 << ", " << y12 << ")" << endl;
    cout << "Ciphertext of m2 = (" << y21 << ", " << y22 << ")" << endl;


    // ====================================
    // HOMOMORPHIC MULTIPLICATION
    // ====================================

    // Multiply the two ciphertexts component-wise
    // This is the main homomorphic step
    long long Y1 = (y11 * y21) % p;
    long long Y2 = (y12 * y22) % p;


    cout << "\n----- Homomorphic Multiplication -----" << endl;
    cout << "Combined Ciphertext = (" << Y1 << ", " << Y2 << ")" << endl;


    // ====================================
    // DECRYPT COMBINED CIPHERTEXT
    // ====================================

    // Shared secret for combined ciphertext
    long long s = modPower(Y1, x, p);

    // Inverse of shared secret
    long long sInverse = modInverse(s, p);

    if (sInverse == -1)
    {
        cout << "Could not find modular inverse." << endl;
        return 0;
    }


    // Decrypt combined ciphertext
    // Result should be m1 * m2 mod p
    long long decryptedProduct = (Y2 * sInverse) % p;


    // Calculate expected result directly
    long long expectedProduct = (m1 * m2) % p;


    cout << "\n----- Homomorphic Result -----" << endl;
    cout << "Expected m1 * m2 mod p = " << expectedProduct << endl;
    cout << "Decrypted Combined Result = " << decryptedProduct << endl;


    if (decryptedProduct == expectedProduct)
    {
        cout << "Homomorphic property VERIFIED." << endl;
    }
    else
    {
        cout << "Homomorphic property NOT verified." << endl;
    }


    return 0;
}