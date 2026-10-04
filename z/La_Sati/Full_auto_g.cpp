#include <iostream>
#include <vector>
using namespace std;


// Finds GCD of two numbers
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


// Checks whether g is a primitive root of p
bool isPrimitiveRoot(long long g, long long p)
{
    // Keeps track of generated values
    vector<bool> used(p, false);

    // Calculate:
    // g^1 mod p, g^2 mod p, ..., g^(p-1) mod p
    for (long long i = 1; i <= p - 1; i++)
    {
        long long value = modPower(g, i, p);

        // If a value appears again before generating
        // all values, g is not a primitive root
        if (used[value])
        {
            return false;
        }

        used[value] = true;
    }

    return true;
}


// Automatically finds the first primitive root g
long long findPrimitiveRoot(long long p)
{
    // Try g = 2, 3, 4, ... until one is valid
    for (long long g = 2; g < p; g++)
    {
        if (isPrimitiveRoot(g, p))
        {
            return g;
        }
    }

    return -1;
}


// Finds modular inverse of a
// Condition: (a * inverse) % mod = 1
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
    // User only enters prime number p
    long long p;

    cout << "Enter prime number p: ";
    cin >> p;


    // Automatically find primitive root g
    long long g = findPrimitiveRoot(p);

    if (g == -1)
    {
        cout << "Primitive root not found." << endl;
        return 0;
    }

    cout << "Automatically selected primitive root g = "
         << g << endl;


    // Private key x
    // Condition: 1 < x < p - 1
    long long x;

    cout << "Enter private key x: ";
    cin >> x;


    // Calculate public value
    // y = g^x mod p
    long long y = modPower(g, x, p);


    cout << "\n----- Key Generation -----" << endl;

    cout << "p = " << p << endl;
    cout << "g = " << g << endl;

    cout << "Private Key x = " << x << endl;
    cout << "Public value y = " << y << endl;

    cout << "Public Key = ("
         << p << ", "
         << g << ", "
         << y << ")" << endl;


    // Input message
    // Message should be less than p
    long long message;

    cout << "\nEnter message: ";
    cin >> message;


    // Temporary random value k
    long long k;

    cout << "Enter random value k: ";
    cin >> k;



    // =========================
    // ELGAMAL ENCRYPTION
    // =========================


    // C1 = g^k mod p
    long long C1 = modPower(g, k, p);


    // Sender calculates shared secret:
    // y^k mod p
    long long sharedSecret = modPower(y, k, p);


    // C2 = message * sharedSecret mod p
    long long C2 = (message * sharedSecret) % p;


    cout << "\n----- Encryption -----" << endl;

    cout << "Original Message = "
         << message << endl;

    cout << "C1 = " << C1 << endl;
    cout << "C2 = " << C2 << endl;

    cout << "Ciphertext = ("
         << C1 << ", "
         << C2 << ")" << endl;



    // =========================
    // ELGAMAL DECRYPTION
    // =========================


    // Receiver recreates the same shared secret
    // s = C1^x mod p
    long long s = modPower(C1, x, p);


    // Find inverse of shared secret
    long long sInverse = modInverse(s, p);


    if (sInverse == -1)
    {
        cout << "Could not find modular inverse." << endl;
        return 0;
    }


    // Recover original message
    // M = C2 * sInverse mod p
    long long decryptedMessage =
        (C2 * sInverse) % p;


    cout << "\n----- Decryption -----" << endl;

    cout << "Shared Secret = "
         << s << endl;

    cout << "Inverse of Shared Secret = "
         << sInverse << endl;

    cout << "Decrypted Message = "
         << decryptedMessage << endl;


    return 0;
}