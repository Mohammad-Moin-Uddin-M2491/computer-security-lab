#include <iostream>
using namespace std;


// ----------------------------------------------------
// Structure to store an ECC point
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
// Check whether two points are equal
// ----------------------------------------------------
bool samePoint(Point P, Point Q)
{
    if(P.infinity && Q.infinity) return true;
    if(P.infinity || Q.infinity) return false;

    return P.x == Q.x && P.y == Q.y;
}


// ----------------------------------------------------
// Check whether point lies on curve:
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
    // Point doubling
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
    // Addition of two different points
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


    // Calculate result point
    long long x3 = mod(lambda * lambda - P.x - Q.x, p);
    long long y3 = mod(lambda * (P.x - x3) - P.y, p);

    return {x3, y3, false};
}


// ----------------------------------------------------
// Scalar multiplication
//
// nP = P + P + ... + P
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
// P = (x,y)
//
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
// Recover numerical message
//
// Since:
//
// M = mG
//
// try 1G, 2G, 3G, ...
// until M is found.
// ----------------------------------------------------
long long recoverMessage(Point M, Point G, long long n, long long a, long long p)
{
    // m = 0 corresponds to Point at Infinity
    if(M.infinity) return 0;

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


    // Find order of G
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
    // Private Key = d
    // Public Key  = Q = dG
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
    // TAKE TWO NUMERICAL MESSAGES
    // =================================================

    long long m1, m2;

    cout << "\nEnter first numerical message m1: ";
    cin >> m1;

    cout << "Enter second numerical message m2: ";
    cin >> m2;


    if(m1 <= 0 || m1 >= n || m2 <= 0 || m2 >= n)
    {
        cout << "Messages must be between 1 and " << n - 1 << endl;
        return 0;
    }


    // ------------------------------------------------
    // Convert numerical messages into points
    //
    // M1 = m1G
    // M2 = m2G
    // ------------------------------------------------
    Point M1 = scalarMultiply(m1, G, a, p);
    Point M2 = scalarMultiply(m2, G, a, p);


    cout << "\n----- Message Encoding -----" << endl;

    cout << "M1 = m1G = ";
    printPoint(M1);
    cout << endl;

    cout << "M2 = m2G = ";
    printPoint(M2);
    cout << endl;


    // =================================================
    // RANDOM VALUES
    //
    // Each encryption gets its own random k
    // =================================================

    long long k1, k2;

    cout << "\nEnter random k1: ";
    cin >> k1;

    cout << "Enter random k2: ";
    cin >> k2;


    // =================================================
    // ENCRYPT MESSAGE M1
    //
    // C11 = k1G
    // C12 = M1 + k1Q
    // =================================================

    Point C11 = scalarMultiply(k1, G, a, p);
    Point k1Q = scalarMultiply(k1, Q, a, p);
    Point C12 = addPoints(M1, k1Q, a, p);


    // =================================================
    // ENCRYPT MESSAGE M2
    //
    // C21 = k2G
    // C22 = M2 + k2Q
    // =================================================

    Point C21 = scalarMultiply(k2, G, a, p);
    Point k2Q = scalarMultiply(k2, Q, a, p);
    Point C22 = addPoints(M2, k2Q, a, p);


    cout << "\n----- Individual Encryption -----" << endl;

    cout << "Ciphertext of M1 = (";
    printPoint(C11);
    cout << ", ";
    printPoint(C12);
    cout << ")" << endl;

    cout << "Ciphertext of M2 = (";
    printPoint(C21);
    cout << ", ";
    printPoint(C22);
    cout << ")" << endl;


    // =================================================
    // HOMOMORPHIC ADDITION
    //
    // Add ciphertexts component-wise:
    //
    // C1 = C11 + C21
    //
    // C2 = C12 + C22
    //
    // This gives an encryption of:
    //
    // M1 + M2
    // =================================================

    Point C1 = addPoints(C11, C21, a, p);
    Point C2 = addPoints(C12, C22, a, p);


    cout << "\n----- Homomorphic Addition -----" << endl;

    cout << "Combined C1 = ";
    printPoint(C1);
    cout << endl;

    cout << "Combined C2 = ";
    printPoint(C2);
    cout << endl;

    cout << "Combined Ciphertext = (";
    printPoint(C1);
    cout << ", ";
    printPoint(C2);
    cout << ")" << endl;


    // =================================================
    // DECRYPT COMBINED CIPHERTEXT
    //
    // dC1 = d * C1
    //
    // Msum = C2 - dC1
    // =================================================

    Point dC1 = scalarMultiply(d, C1, a, p);
    Point negativeDC1 = negativePoint(dC1, p);

    Point decryptedSum = addPoints(C2, negativeDC1, a, p);


    cout << "\n----- Decryption of Combined Ciphertext -----" << endl;

    cout << "Decrypted Message Point = ";
    printPoint(decryptedSum);
    cout << endl;


    // =================================================
    // RECOVER NUMERICAL RESULT
    //
    // Since:
    //
    // M1 + M2
    // = m1G + m2G
    // = (m1 + m2)G
    //
    // Numerical result is:
    //
    // (m1 + m2) mod n
    // =================================================

    long long recoveredSum = recoverMessage(decryptedSum, G, n, a, p);
    long long expectedSum = (m1 + m2) % n;


    cout << "\n----- Homomorphic Result -----" << endl;
    cout << "Expected (m1 + m2) mod n = " << expectedSum << endl;
    cout << "Recovered Numerical Result = " << recoveredSum << endl;


    if(recoveredSum == expectedSum)
        cout << "ECC Homomorphic Property VERIFIED." << endl;
    else
        cout << "ECC Homomorphic Property NOT verified." << endl;


    return 0;
}


/*
================ INPUT FORMAT ================

p = 17
a = 2
b = 2
G = 5 1
d = 7
m1 = 4
m2 = 5
k1 = 3
k2 = 4

Expected:
Recovered Numerical Result = 9

==============================================
*/