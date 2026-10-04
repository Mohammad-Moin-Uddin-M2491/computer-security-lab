#include <iostream>
using namespace std;

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

long long findE(long long phi)
{
    for (long long e = 2; e < phi; e++)
    {
        if (gcd(e, phi) == 1)
        {
            return e;
        }
    }

    return -1;
}

long long findD(long long e, long long phi)
{
    for (long long d = 1; d < phi; d++)
    {
        if ((e * d) % phi == 1)
        {
            return d;
        }
    }

    return -1;
}

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

int main()
{
    long long p, q;

    cout << "Enter two prime numbers (p and q): ";
    cin >> p >> q;

    long long n = p * q;

    long long phi = (p - 1) * (q - 1);

    long long e = findE(phi);

    if (e == -1)
    {
        cout << "Could not find e." << endl;
        return 0;
    }

    long long d = findD(e, phi);

    if (d == -1)
    {
        cout << "Could not find d." << endl;
        return 0;
    }

    cout << "p = " << p << endl;
    cout << "q = " << q << endl;

    cout << "n = " << n << endl;
    cout << "phi(n) = " << phi << endl;

    cout << "e = " << e << endl;
    cout << "d = " << d << endl;

    cout << "\nPublic Key = (" << e << ", " << n << ")" << endl;
    cout << "Private Key = (" << d << ", " << n << ")" << endl;

    long long message;

    cout << "\nEnter a message: ";
    cin >> message;

    // Encryption
    long long ciphertext = modPower(message, e, n);

    cout << "\n----- Encryption -----" << endl;

    cout << "Original Message = " << message << endl;
    cout << "Encrypted Message = " << ciphertext << endl;

    // Decryption
    long long decrypted = modPower(ciphertext, d, n);

    cout << "\n----- Decryption -----" << endl;

    cout << "Ciphertext = " << ciphertext << endl;
    cout << "Decrypted Message = " << decrypted << endl;

    // Digital Signature
    long long signature = modPower(message, d, n);

    cout << "\n----- Digital Signature -----" << endl;

    cout << "Original Message = " << message << endl;
    cout << "Signature = " << signature << endl;

    // Tampered message
    long long tamperedMessage;

    cout << "\nEnter tampered message: ";
    cin >> tamperedMessage;

    // Verify the original signature using public key
    long long verifiedMessage = modPower(signature, e, n);

    cout << "\n----- Signature Verification -----" << endl;

    cout << "Original Message = " << message << endl;
    cout << "Tampered Message = " << tamperedMessage << endl;
    cout << "Verified Value = " << verifiedMessage << endl;

    // Compare verified value with the received/tampered message
    if (verifiedMessage == tamperedMessage)
    {
        cout << "Signature is VALID." << endl;
    }
    else
    {
        cout << "Signature is INVALID." << endl;
    }

    return 0;
}