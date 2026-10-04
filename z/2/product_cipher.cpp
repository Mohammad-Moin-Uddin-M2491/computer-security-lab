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

        // Repeated value means alpha cannot generate all values
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


// Finds modular inverse of a
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


    // x is the private key
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


    // Message must be smaller than p
    long long m;

    cout << "\nEnter message m: ";
    cin >> m;


    if (m <= 0 || m >= p)
    {
        cout << "Message must be between 1 and " << p - 1 << endl;
        return 0;
    }


    // k is a temporary random value for encryption
    long long k;

    cout << "Enter random k: ";
    cin >> k;


    // ====================================
    // ELGAMAL ENCRYPTION
    // ====================================

    // y1 = alpha^k mod p
    long long y1 = modPower(alpha, k, p);


    // Shared value = beta^k mod p
    long long sharedSecret = modPower(beta, k, p);


    // y2 = m * beta^k mod p
    long long y2 = (m * sharedSecret) % p;


    cout << "\n----- Encryption -----" << endl;
    cout << "Original Message = " << m << endl;
    cout << "y1 = " << y1 << endl;
    cout << "y2 = " << y2 << endl;
    cout << "Ciphertext = (" << y1 << ", " << y2 << ")" << endl;


    // ====================================
    // ELGAMAL DECRYPTION
    // ====================================

    // Receiver calculates the same shared secret:
    // y1^x mod p
    long long s = modPower(y1, x, p);


    // Find inverse of shared secret
    long long sInverse = modInverse(s, p);

    if (sInverse == -1)
    {
        cout << "Could not find modular inverse." << endl;
        return 0;
    }


    // Recover message:
    // m = y2 * s^(-1) mod p
    long long decryptedMessage = (y2 * sInverse) % p;


    cout << "\n----- Decryption -----" << endl;
    cout << "Shared Secret = " << s << endl;
    cout << "Inverse = " << sInverse << endl;
    cout << "Decrypted Message = " << decryptedMessage << endl;


    return 0;
}