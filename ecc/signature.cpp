#include <iostream>
using namespace std;


// ============================================================
// Structure for one ECC point
//
// Example:
// P = (5, 1)
//
// infinity = true means the special Point at Infinity O
// ============================================================
struct Point
{
    long long x;
    long long y;
    bool infinity;
};


// ============================================================
// Positive modulo function
//
// C++ may give negative remainder.
//
// Example:
// -3 % 17 may give -3
//
// But mathematically:
// -3 mod 17 = 14
//
// So this function always gives a positive result.
// ============================================================
long long mod(long long value, long long m)
{
    return (value % m + m) % m;
}


// ============================================================
// Modular inverse
//
// We want:
//
// a * inverse mod m = 1
//
// Example:
//
// 3^(-1) mod 19 = 13
//
// because:
//
// 3 * 13 = 39
// 39 mod 19 = 1
//
// This simple loop is good for small classroom examples.
// ============================================================
long long modInverse(long long a, long long m)
{
    a = mod(a, m);

    for (long long inverse = 1; inverse < m; inverse++)
    {
        if ((a * inverse) % m == 1)
        {
            return inverse;
        }
    }

    return -1;
}


// ============================================================
// Check whether a point is on the elliptic curve:
//
// y^2 = x^3 + ax + b mod p
// ============================================================
bool isOnCurve(Point P, long long a, long long b, long long p)
{
    if (P.infinity)
        return true;

    long long left = mod(P.y * P.y, p);

    long long right =
        mod(P.x * P.x * P.x + a * P.x + b, p);

    return left == right;
}


// ============================================================
// Add two elliptic curve points.
//
// This function handles:
//
// 1. P + Q
// 2. P + P = 2P
// 3. P + (-P) = O
// 4. P + O = P
//
// All point calculations are done modulo p.
// ============================================================
Point addPoints(Point P, Point Q, long long a, long long p)
{
    // O + Q = Q
    if (P.infinity)
        return Q;

    // P + O = P
    if (Q.infinity)
        return P;


    // --------------------------------------------------------
    // If P and Q are opposite points:
    //
    // P + (-P) = O
    //
    // Example modulo 17:
    //
    // (5,1) and (5,16)
    //
    // because:
    // 1 + 16 = 17 = 0 mod 17
    // --------------------------------------------------------
    if (P.x == Q.x && mod(P.y + Q.y, p) == 0)
    {
        return {0, 0, true};
    }


    long long lambda;


    // ========================================================
    // CASE 1: POINT DOUBLING
    //
    // P == Q
    //
    // lambda =
    //
    // (3*x1^2 + a)
    // ---------------- mod p
    //      2*y1
    //
    // Division means multiplication by modular inverse.
    // ========================================================
    if (P.x == Q.x && P.y == Q.y)
    {
        long long numerator =
            mod(3 * P.x * P.x + a, p);

        long long denominator =
            mod(2 * P.y, p);

        long long inverse =
            modInverse(denominator, p);

        if (inverse == -1)
        {
            return {0, 0, true};
        }

        lambda =
            mod(numerator * inverse, p);
    }


    // ========================================================
    // CASE 2: TWO DIFFERENT POINTS
    //
    // lambda =
    //
    // y2 - y1
    // -------- mod p
    // x2 - x1
    // ========================================================
    else
    {
        long long numerator =
            mod(Q.y - P.y, p);

        long long denominator =
            mod(Q.x - P.x, p);

        long long inverse =
            modInverse(denominator, p);

        if (inverse == -1)
        {
            return {0, 0, true};
        }

        lambda =
            mod(numerator * inverse, p);
    }


    // ========================================================
    // Calculate resulting point R = (x3, y3)
    //
    // x3 = lambda^2 - x1 - x2 mod p
    //
    // y3 = lambda(x1-x3) - y1 mod p
    // ========================================================
    long long x3 =
        mod(lambda * lambda - P.x - Q.x, p);

    long long y3 =
        mod(lambda * (P.x - x3) - P.y, p);


    return {x3, y3, false};
}


// ============================================================
// Scalar multiplication
//
// kP means:
//
// P + P + P + ... + P
//         k times
//
// Example:
//
// 3P = P + P + P
//
// We use repeated addition because it is easier for a beginner
// to understand.
//
// Real ECC implementations use faster algorithms.
// ============================================================
Point scalarMultiply(long long k, Point P,
                     long long a, long long p)
{
    Point result = {0, 0, true};

    for (long long i = 0; i < k; i++)
    {
        result = addPoints(result, P, a, p);
    }

    return result;
}


// ============================================================
// Print ECC point
// ============================================================
void printPoint(Point P)
{
    if (P.infinity)
    {
        cout << "Point at Infinity (O)";
    }
    else
    {
        cout << "(" << P.x << ", " << P.y << ")";
    }
}


// ============================================================
// MAIN PROGRAM
// ============================================================
int main()
{
    long long p, a, b, n;

    // --------------------------------------------------------
    // Curve:
    //
    // y^2 = x^3 + ax + b mod p
    //
    // p = prime number used for curve arithmetic
    //
    // n = order of base point G
    //
    // IMPORTANT:
    // p and n are NOT the same thing.
    // --------------------------------------------------------

    cout << "Enter prime p: ";
    cin >> p;

    cout << "Enter curve parameter a: ";
    cin >> a;

    cout << "Enter curve parameter b: ";
    cin >> b;

    cout << "Enter order n of base point G: ";
    cin >> n;


    // ========================================================
    // BASE POINT G
    // ========================================================
    Point G;

    G.infinity = false;

    cout << "\nEnter base point G (x y): ";
    cin >> G.x >> G.y;


    if (!isOnCurve(G, a, b, p))
    {
        cout << "Error: G is not on the curve." << endl;
        return 0;
    }


    // ========================================================
    // PRIVATE KEY
    //
    // d is secret.
    //
    // Normally:
    //
    // 1 <= d <= n-1
    // ========================================================
    long long d;

    cout << "Enter private key d: ";
    cin >> d;


    if (d <= 0 || d >= n)
    {
        cout << "Private key must be between 1 and n-1." << endl;
        return 0;
    }


    // ========================================================
    // PUBLIC KEY
    //
    // Q = dG
    //
    // Q is public.
    // d remains secret.
    // ========================================================
    Point Q =
        scalarMultiply(d, G, a, p);


    cout << "\n========== KEY GENERATION ==========" << endl;

    cout << "Private Key d = " << d << endl;

    cout << "Public Key Q = ";
    printPoint(Q);
    cout << endl;


    // ========================================================
    // MESSAGE
    //
    // Real ECDSA first hashes the message.
    //
    // Example:
    //
    // z = Hash(message)
    //
    // To keep this classroom program simple,
    // we enter an integer message directly and use:
    //
    // z = message mod n
    //
    // Think of z as the message hash value.
    // ========================================================
    long long message;

    cout << "\nEnter integer message/hash value: ";
    cin >> message;

    long long z = mod(message, n);


    // ========================================================
    // RANDOM NONCE k
    //
    // The signer chooses a temporary random secret k.
    //
    // k must normally be:
    //
    // 1 <= k <= n-1
    //
    // IMPORTANT:
    // In real ECDSA, k must be secret and must never be reused.
    //
    // Here we manually enter k only for learning.
    // ========================================================
    long long k;

    cout << "Enter random k: ";
    cin >> k;


    if (k <= 0 || k >= n)
    {
        cout << "k must be between 1 and n-1." << endl;
        return 0;
    }


    // ========================================================
    //
    //                 SIGNATURE CREATION
    //
    // ========================================================


    // --------------------------------------------------------
    // STEP 1:
    //
    // Calculate:
    //
    // R = kG
    //
    // R is an ECC point.
    // --------------------------------------------------------
    Point R =
        scalarMultiply(k, G, a, p);


    if (R.infinity)
    {
        cout << "Invalid k. Try another value." << endl;
        return 0;
    }


    // --------------------------------------------------------
    // STEP 2:
    //
    // r = x-coordinate of R mod n
    //
    // If:
    //
    // R = (xR, yR)
    //
    // then:
    //
    // r = xR mod n
    // --------------------------------------------------------
    long long r =
        mod(R.x, n);


    if (r == 0)
    {
        cout << "r became 0. Choose another k." << endl;
        return 0;
    }


    // --------------------------------------------------------
    // STEP 3:
    //
    // Find inverse of k modulo n.
    //
    // kInverse satisfies:
    //
    // k * kInverse mod n = 1
    // --------------------------------------------------------
    long long kInverse =
        modInverse(k, n);


    if (kInverse == -1)
    {
        cout << "k has no modular inverse." << endl;
        return 0;
    }


    // --------------------------------------------------------
    // STEP 4:
    //
    // Calculate:
    //
    // s = k^(-1) * (z + r*d) mod n
    //
    // Where:
    //
    // z = message/hash
    // r = first part of signature
    // d = private key
    // kInverse = inverse of k
    // --------------------------------------------------------
    long long s =
        mod(kInverse * (z + r * d), n);


    if (s == 0)
    {
        cout << "s became 0. Choose another k." << endl;
        return 0;
    }


    cout << "\n========== SIGNATURE CREATION ==========" << endl;

    cout << "Message/hash z = " << z << endl;

    cout << "kG = ";
    printPoint(R);
    cout << endl;

    cout << "r = " << r << endl;

    cout << "k inverse mod n = "
         << kInverse << endl;

    cout << "s = " << s << endl;

    cout << "\nSignature = ("
         << r << ", " << s << ")" << endl;



    // ========================================================
    //
    //                  SIGNATURE VERIFICATION
    //
    // ========================================================


    // --------------------------------------------------------
    // Receiver knows:
    //
    // message/hash z
    // signature (r,s)
    // public key Q
    //
    // Receiver does NOT need private key d.
    // --------------------------------------------------------


    // Check signature values
    if (r <= 0 || r >= n ||
        s <= 0 || s >= n)
    {
        cout << "\nSignature is INVALID." << endl;
        return 0;
    }


    // --------------------------------------------------------
    // STEP 1:
    //
    // w = s^(-1) mod n
    // --------------------------------------------------------
    long long w =
        modInverse(s, n);


    if (w == -1)
    {
        cout << "\nSignature is INVALID." << endl;
        return 0;
    }


    // --------------------------------------------------------
    // STEP 2:
    //
    // u1 = z*w mod n
    //
    // u2 = r*w mod n
    // --------------------------------------------------------
    long long u1 =
        mod(z * w, n);

    long long u2 =
        mod(r * w, n);


    // --------------------------------------------------------
    // STEP 3:
    //
    // Calculate:
    //
    // X = u1*G + u2*Q
    //
    // Here:
    //
    // u1*G = scalar multiplication
    // u2*Q = scalar multiplication
    //
    // Then add those two ECC points.
    // --------------------------------------------------------
    Point point1 =
        scalarMultiply(u1, G, a, p);

    Point point2 =
        scalarMultiply(u2, Q, a, p);

    Point X =
        addPoints(point1, point2, a, p);


    cout << "\n========== SIGNATURE VERIFICATION ==========" << endl;

    cout << "w = s^(-1) mod n = "
         << w << endl;

    cout << "u1 = " << u1 << endl;

    cout << "u2 = " << u2 << endl;

    cout << "u1 * G = ";
    printPoint(point1);
    cout << endl;

    cout << "u2 * Q = ";
    printPoint(point2);
    cout << endl;

    cout << "X = u1G + u2Q = ";
    printPoint(X);
    cout << endl;


    // --------------------------------------------------------
    // STEP 4:
    //
    // Signature is valid if:
    //
    // X.x mod n == r
    //
    // --------------------------------------------------------
    if (!X.infinity &&
        mod(X.x, n) == r)
    {
        cout << "\nSignature is VALID." << endl;
    }
    else
    {
        cout << "\nSignature is INVALID." << endl;
    }


    return 0;
}

/*
================ INPUT FORMAT ================

p = 17
a = 2
b = 2
n = 19
G = 5 1
d = 7
message = 9
k = 3

Expected:
Signature = (10, 1)
Signature is VALID.

==============================================
*/