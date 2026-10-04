#include <iostream>
using namespace std;


// ----------------------------------------------------
// Structure to store one ECC point
//
// Example:
// (5,1)
//
// infinity = true means Point at Infinity O
// ----------------------------------------------------
struct Point
{
    long long x, y;
    bool infinity;
};


// ----------------------------------------------------
// Make modulo result positive
//
// Example:
// -3 mod 17 = 14
// ----------------------------------------------------
long long mod(long long value, long long p)
{
    return (value % p + p) % p;
}


// ----------------------------------------------------
// Find modular inverse
//
// a * inverse mod p = 1
// ----------------------------------------------------
long long modInverse(long long a, long long p)
{
    a = mod(a, p);

    for(long long inverse = 1; inverse < p; inverse++)
    {
        if((a * inverse) % p == 1) return inverse;
    }

    return -1;
}


// ----------------------------------------------------
// Check whether point P is on:
//
// y^2 = x^3 + ax + b mod p
// ----------------------------------------------------
bool isOnCurve(Point P, long long a, long long b, long long p)
{
    if(P.infinity) return true;

    long long left = mod(P.y * P.y, p);
    long long right = mod(P.x * P.x * P.x + a * P.x + b, p);

    return left == right;
}


// ----------------------------------------------------
// Check whether two points are equal
// ----------------------------------------------------
bool samePoint(Point P, Point Q)
{
    if(P.infinity && Q.infinity) return true;
    if(P.infinity || Q.infinity) return false;

    return P.x == Q.x && P.y == Q.y;
}


// ----------------------------------------------------
// Add two ECC points
//
// Handles:
//
// P + Q
// P + P = 2P
// P + (-P) = O
// ----------------------------------------------------
Point addPoints(Point P, Point Q, long long a, long long p)
{
    if(P.infinity) return Q;
    if(Q.infinity) return P;

    // P + (-P) = O
    if(P.x == Q.x && mod(P.y + Q.y, p) == 0)
        return {0, 0, true};


    long long lambda;


    // ------------------------------------------------
    // POINT DOUBLING
    //
    // lambda = (3*x1^2 + a)/(2*y1) mod p
    // ------------------------------------------------
    if(P.x == Q.x && P.y == Q.y)
    {
        long long numerator = mod(3 * P.x * P.x + a, p);
        long long denominator = mod(2 * P.y, p);
        long long inverse = modInverse(denominator, p);

        if(inverse == -1) return {0, 0, true};

        lambda = mod(numerator * inverse, p);
    }

    // ------------------------------------------------
    // ADD TWO DIFFERENT POINTS
    //
    // lambda = (y2-y1)/(x2-x1) mod p
    // ------------------------------------------------
    else
    {
        long long numerator = mod(Q.y - P.y, p);
        long long denominator = mod(Q.x - P.x, p);
        long long inverse = modInverse(denominator, p);

        if(inverse == -1) return {0, 0, true};

        lambda = mod(numerator * inverse, p);
    }


    // Result point R = (x3, y3)
    long long x3 = mod(lambda * lambda - P.x - Q.x, p);
    long long y3 = mod(lambda * (P.x - x3) - P.y, p);

    return {x3, y3, false};
}


// ----------------------------------------------------
// Scalar multiplication
//
// nP = P + P + ... + P
//
// Example:
// 4G = G + G + G + G
// ----------------------------------------------------
Point scalarMultiply(long long n, Point P, long long a, long long p)
{
    Point result = {0, 0, true};

    for(long long i = 0; i < n; i++)
        result = addPoints(result, P, a, p);

    return result;
}


// ----------------------------------------------------
// Negative of a point
//
// If:
// P = (x,y)
//
// then:
// -P = (x,-y mod p)
// ----------------------------------------------------
Point negativePoint(Point P, long long p)
{
    if(P.infinity) return P;

    return {P.x, mod(-P.y, p), false};
}


// ----------------------------------------------------
// Find order n of base point G
//
// n is the smallest positive number where:
//
// nG = O
// ----------------------------------------------------
long long findPointOrder(Point G, long long a, long long p)
{
    Point result = {0, 0, true};

    for(long long n = 1; n <= 2 * p + 2; n++)
    {
        result = addPoints(result, G, a, p);

        if(result.infinity) return n;
    }

    return -1;
}


// ----------------------------------------------------
// Recover numerical message from message point
//
// Since we encoded:
//
// M = mG
//
// try:
//
// G, 2G, 3G, ...
//
// until the decrypted point is found.
// ----------------------------------------------------
long long recoverMessage(Point M, Point G, long long n, long long a, long long p)
{
    for(long long m = 1; m < n; m++)
    {
        Point test = scalarMultiply(m, G, a, p);

        if(samePoint(test, M)) return m;
    }

    return -1;
}


// ----------------------------------------------------
// Print ECC point
// ----------------------------------------------------
void printPoint(Point P)
{
    if(P.infinity)
        cout << "Point at Infinity (O)";
    else
        cout << "(" << P.x << ", " << P.y << ")";
}


int main()
{
    long long p, a, b;


    // =================================================
    // CURVE PARAMETERS
    //
    // y^2 = x^3 + ax + b mod p
    // =================================================

    cout << "Enter prime p: ";
    cin >> p;

    cout << "Enter curve parameter a: ";
    cin >> a;

    cout << "Enter curve parameter b: ";
    cin >> b;


    // =================================================
    // BASE POINT G
    // =================================================

    Point G;
    G.infinity = false;

    cout << "\nEnter base point G (x y): ";
    cin >> G.x >> G.y;


    if(!isOnCurve(G, a, b, p))
    {
        cout << "G is not on the curve." << endl;
        return 0;
    }


    // ------------------------------------------------
    // Automatically find order n of G
    // ------------------------------------------------
    long long n = findPointOrder(G, a, p);

    if(n == -1)
    {
        cout << "Could not find order of G." << endl;
        return 0;
    }


    cout << "Order of G, n = " << n << endl;


    // =================================================
    // KEY GENERATION
    //
    // Private key = d
    //
    // Public key:
    // Q = dG
    // =================================================

    long long d;

    cout << "Enter private key d: ";
    cin >> d;


    Point Q = scalarMultiply(d, G, a, p);


    cout << "\n----- Key Generation -----" << endl;
    cout << "Private Key d = " << d << endl;

    cout << "Public Key Q = ";
    printPoint(Q);
    cout << endl;


    // =================================================
    // NUMERICAL MESSAGE
    //
    // Sir gives message as one number m.
    //
    // Example:
    // m = 4
    //
    // Convert number into ECC point:
    //
    // M = mG
    // =================================================

    long long m;

    cout << "\nEnter numerical message m: ";
    cin >> m;


    if(m <= 0 || m >= n)
    {
        cout << "Message must be between 1 and " << n - 1 << endl;
        return 0;
    }


    // Convert number to message point
    Point M = scalarMultiply(m, G, a, p);


    cout << "\n----- Message Encoding -----" << endl;
    cout << "Numerical Message m = " << m << endl;

    cout << "Message Point M = mG = ";
    printPoint(M);
    cout << endl;


    // =================================================
    // RANDOM k
    // =================================================

    long long k;

    cout << "\nEnter random k: ";
    cin >> k;


    // =================================================
    // ECC ELGAMAL ENCRYPTION
    // =================================================

    // First ciphertext point:
    //
    // C1 = kG
    Point C1 = scalarMultiply(k, G, a, p);


    // Temporary point:
    //
    // kQ
    Point kQ = scalarMultiply(k, Q, a, p);


    // Second ciphertext point:
    //
    // C2 = M + kQ
    Point C2 = addPoints(M, kQ, a, p);


    cout << "\n----- Encryption -----" << endl;

    cout << "Message Point M = ";
    printPoint(M);
    cout << endl;

    cout << "C1 = ";
    printPoint(C1);
    cout << endl;

    cout << "kQ = ";
    printPoint(kQ);
    cout << endl;

    cout << "C2 = ";
    printPoint(C2);
    cout << endl;

    cout << "Ciphertext = (";
    printPoint(C1);
    cout << ", ";
    printPoint(C2);
    cout << ")" << endl;


    // =================================================
    // ECC ELGAMAL DECRYPTION
    // =================================================

    // Calculate:
    //
    // dC1
    Point dC1 = scalarMultiply(d, C1, a, p);


    // Decryption:
    //
    // M = C2 - dC1
    //
    //   = C2 + (-dC1)
    Point negativeDC1 = negativePoint(dC1, p);

    Point decryptedM = addPoints(C2, negativeDC1, a, p);


    cout << "\n----- Decryption -----" << endl;

    cout << "dC1 = ";
    printPoint(dC1);
    cout << endl;

    cout << "-dC1 = ";
    printPoint(negativeDC1);
    cout << endl;

    cout << "Decrypted Message Point = ";
    printPoint(decryptedM);
    cout << endl;


    // =================================================
    // RECOVER ORIGINAL NUMERICAL MESSAGE
    //
    // Since:
    //
    // M = mG
    //
    // find m again from the decrypted point.
    // =================================================

    long long recoveredM = recoverMessage(decryptedM, G, n, a, p);


    cout << "\n----- Numerical Message Recovery -----" << endl;
    cout << "Original Numerical Message = " << m << endl;
    cout << "Recovered Numerical Message = " << recoveredM << endl;


    if(recoveredM == m)
        cout << "ECC Encryption/Decryption SUCCESSFUL." << endl;
    else
        cout << "ECC Encryption/Decryption FAILED." << endl;


    return 0;
}


/*
====================================================
INPUT FORMAT
====================================================

Use this test curve:

y^2 = x^3 + 2x + 2 mod 17

Input:

17
2
2
5 1
7
4
3


Meaning:

p = 17
a = 2
b = 2

Base Point:
G = (5,1)

Private Key:
d = 7

Numerical Message:
m = 4

Random value:
k = 3


====================================================
EXPECTED IMPORTANT VALUES
====================================================

Order of G:

n = 19


Public Key:

Q = dG
Q = 7G
Q = (0,6)


NUMERICAL MESSAGE ENCODING:

m = 4

M = mG
M = 4G
M = (3,1)


ENCRYPTION:

C1 = kG
C1 = 3G
C1 = (10,6)

kQ = (6,3)

C2 = M + kQ
C2 = (16,13)

Ciphertext:

((10,6), (16,13))


DECRYPTION:

dC1 = (6,3)

M = C2 - dC1

Decrypted Message Point:

(3,1)


NUMERICAL MESSAGE RECOVERY:

(3,1) = 4G

Therefore:

Recovered Numerical Message = 4


====================================================
IMPORTANT INPUT RULE
====================================================

Enter points with SPACE:

5 1

NOT:

(5,1)

NOT:

5,1

====================================================
*/