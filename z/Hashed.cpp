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


// Simple hash function for lab demonstration
long long simpleHash(long long message)
{
    return message % 10;
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


    cout << "\nn = " << n << endl;
    cout << "phi(n) = " << phi << endl;

    cout << "e = " << e << endl;
    cout << "d = " << d << endl;

    cout << "\nPublic Key = (" << e << ", " << n << ")" << endl;
    cout << "Private Key = (" << d << ", " << n << ")" << endl;


    long long message;

    cout << "\nEnter message: ";
    cin >> message;


    // Calculate hash of original message
    long long hashValue = simpleHash(message);


    cout << "\n----- Hash -----" << endl;

    cout << "Message = " << message << endl;
    cout << "Hash = " << hashValue << endl;


    // Sign the HASH using private key
    long long signature = modPower(hashValue, d, n);


    cout << "\n----- Digital Signature -----" << endl;

    cout << "Hash = " << hashValue << endl;
    cout << "Signature = " << signature << endl;


    // Receiver receives the message
    long long receivedMessage;

    cout << "\nEnter received message: ";
    cin >> receivedMessage;


    // Receiver calculates hash again
    long long receivedHash = simpleHash(receivedMessage);


    // Recover hash from signature using public key
    long long verifiedHash = modPower(signature, e, n);


    cout << "\n----- Signature Verification -----" << endl;

    cout << "Received Message = " << receivedMessage << endl;

    cout << "Calculated Hash = " << receivedHash << endl;

    cout << "Verified Hash = " << verifiedHash << endl;


    if (receivedHash == verifiedHash)
    {
        cout << "Signature is VALID." << endl;
    }
    else
    {
        cout << "Signature is INVALID." << endl;
    }


    return 0;
}