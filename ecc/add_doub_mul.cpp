#include <iostream>
using namespace std;


// ----------------------------------------------------
// Structure to store one ECC point (x, y)
//
// infinity = true means the special "Point at Infinity"
// which works like 0 in normal addition.
// ----------------------------------------------------
struct Point
{
    long long x;
    long long y;
    bool infinity;
};


// ----------------------------------------------------
// Converts negative modulo results into positive values.
//
// Example:
// -2 mod 17 should be 15
//
// In C++, -2 % 17 may give -2,
// so this function fixes that.
// ----------------------------------------------------
long long mod(long long value, long long p)
{
    return (value % p + p) % p;
}


// ----------------------------------------------------
// Finds modular inverse.
//
// We need inverse such that:
//
//      a * inverse mod p = 1
//
// Example:
// inverse of 4 mod 17 = 13
// because 4 * 13 mod 17 = 1
//
// Simple loop is used because this is a beginner lab.
// ----------------------------------------------------
long long modInverse(long long a, long long p)
{
    a = mod(a, p);

    for (long long inverse = 1; inverse < p; inverse++)
    {
        if ((a * inverse) % p == 1)
        {
            return inverse;
        }
    }

    return -1;
}


// ----------------------------------------------------
// Checks whether a point lies on:
//
//      y^2 = x^3 + ax + b mod p
// ----------------------------------------------------
bool isOnCurve(Point P, long long a, long long b, long long p)
{
    if (P.infinity)
        return true;

    long long left = mod(P.y * P.y, p);

    long long right =
        mod(P.x * P.x * P.x + a * P.x + b, p);

    return left == right;
}


// ----------------------------------------------------
// POINT ADDITION
//
// This function can handle:
//
// 1. Two different points: P + Q
// 2. Same point: P + P = 2P
// 3. Point + inverse point = Point at Infinity
//
// So the same function is useful everywhere.
// ----------------------------------------------------
Point addPoints(Point P, Point Q, long long a, long long p)
{
    // O + Q = Q
    if (P.infinity)
        return Q;

    // P + O = P
    if (Q.infinity)
        return P;


    // ------------------------------------------------
    // If P and Q have same x but opposite y:
    //
    // P + (-P) = O
    //
    // Example:
    // (3,2) + (3,15) mod 17 = O
    // because 2 + 15 = 17 = 0 mod 17
    // ------------------------------------------------
    if (P.x == Q.x && mod(P.y + Q.y, p) == 0)
    {
        return {0, 0, true};
    }


    long long lambda;


    // =================================================
    // CASE 1: POINT DOUBLING
    //
    // P == Q
    //
    // lambda =
    // (3*x1^2 + a) / (2*y1) mod p
    // =================================================
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

        lambda = mod(numerator * inverse, p);
    }


    // =================================================
    // CASE 2: ADD TWO DIFFERENT POINTS
    //
    // lambda =
    // (y2-y1) / (x2-x1) mod p
    // =================================================
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

        lambda = mod(numerator * inverse, p);
    }


    // ------------------------------------------------
    // Now calculate the new point R = (x3, y3)
    //
    // x3 = lambda^2 - x1 - x2 mod p
    // y3 = lambda(x1-x3) - y1 mod p
    // ------------------------------------------------
    long long x3 =
        mod(lambda * lambda - P.x - Q.x, p);

    long long y3 =
        mod(lambda * (P.x - x3) - P.y, p);


    return {x3, y3, false};
}


// ----------------------------------------------------
// POINT DOUBLING
//
// 2P simply means:
//
//      P + P
//
// So we can reuse addPoints().
// ----------------------------------------------------
Point doublePoint(Point P, long long a, long long p)
{
    return addPoints(P, P, a, p);
}


// ----------------------------------------------------
// SCALAR MULTIPLICATION
//
// nP means:
//
// P + P + P + ... + P
//        n times
//
// Example:
// 5P = P + P + P + P + P
//
// This repeated-addition version is intentionally
// simple for understanding.
// ----------------------------------------------------
Point scalarMultiply(long long n, Point P, long long a, long long p)
{
    Point result = {0, 0, true};   // Start with O

    for (long long i = 0; i < n; i++)
    {
        result = addPoints(result, P, a, p);
    }

    return result;
}


// ----------------------------------------------------
// Prints an ECC point
// ----------------------------------------------------
void printPoint(Point P)
{
    if (P.infinity)
    {
        cout << "Point at Infinity (O)" << endl;
    }
    else
    {
        cout << "(" << P.x << ", " << P.y << ")" << endl;
    }
}


int main()
{
    long long p, a, b;

    // Curve:
    //
    // y^2 = x^3 + ax + b mod p
    //
    cout << "Enter prime p: ";
    cin >> p;

    cout << "Enter curve parameter a: ";
    cin >> a;

    cout << "Enter curve parameter b: ";
    cin >> b;


    cout << "\nCurve: y^2 = x^3 + "
         << a << "x + " << b
         << " mod " << p << endl;


    int choice;

    cout << "\nChoose operation:" << endl;
    cout << "1. Add Two Points" << endl;
    cout << "2. Point Doubling" << endl;
    cout << "3. Number x Point (Scalar Multiplication)" << endl;

    cout << "\nEnter choice: ";
    cin >> choice;


    // =================================================
    // TASK 1: ADD TWO DIFFERENT POINTS
    // =================================================
    if (choice == 1)
    {
        Point P, Q;

        P.infinity = false;
        Q.infinity = false;

        cout << "\nEnter P (x1 y1): ";
        cin >> P.x >> P.y;

        cout << "Enter Q (x2 y2): ";
        cin >> Q.x >> Q.y;


        // Optional safety check
        if (!isOnCurve(P, a, b, p) ||
            !isOnCurve(Q, a, b, p))
        {
            cout << "One or both points are NOT on the curve." << endl;
            return 0;
        }


        Point R = addPoints(P, Q, a, p);

        cout << "\nP + Q = ";
        printPoint(R);
    }


    // =================================================
    // TASK 2: POINT DOUBLING
    // =================================================
    else if (choice == 2)
    {
        Point P;

        P.infinity = false;

        cout << "\nEnter point P (x y): ";
        cin >> P.x >> P.y;


        if (!isOnCurve(P, a, b, p))
        {
            cout << "Point is NOT on the curve." << endl;
            return 0;
        }


        Point R = doublePoint(P, a, p);

        cout << "\n2P = ";
        printPoint(R);
    }


    // =================================================
    // TASK 3: NUMBER x POINT
    //
    // Example:
    // n = 5
    //
    // 5P = P + P + P + P + P
    // =================================================
    else if (choice == 3)
    {
        Point P;
        long long n;

        P.infinity = false;

        cout << "\nEnter point P (x y): ";
        cin >> P.x >> P.y;

        cout << "Enter number n: ";
        cin >> n;


        if (!isOnCurve(P, a, b, p))
        {
            cout << "Point is NOT on the curve." << endl;
            return 0;
        }


        Point R = scalarMultiply(n, P, a, p);

        cout << "\n" << n << "P = ";
        printPoint(R);
    }


    else
    {
        cout << "Invalid choice." << endl;
    }


    return 0;
}

/*
====================================================
INPUT FORMAT
====================================================

First enter the curve parameters:

p
a
b

For the test curve:

p = 17
a = 2
b = 2

So the curve is:

y^2 = x^3 + 2x + 2 mod 17


----------------------------------------------------
CHOICE 1: ADD TWO POINTS
----------------------------------------------------

Input:

17
2
2
1
5 1
6 3

Meaning:

p = 17
a = 2
b = 2
choice = 1

P = (5,1)
Q = (6,3)

Expected:

P + Q = (10,6)


----------------------------------------------------
CHOICE 2: POINT DOUBLING
----------------------------------------------------

Input:

17
2
2
2
5 1

Meaning:

p = 17
a = 2
b = 2
choice = 2

P = (5,1)

Expected:

2P = (6,3)


----------------------------------------------------
CHOICE 3: SCALAR MULTIPLICATION
----------------------------------------------------

Input:

17
2
2
3
5 1
5

Meaning:

p = 17
a = 2
b = 2
choice = 3

P = (5,1)
n = 5

Expected:

5P = (9,16)


----------------------------------------------------
IMPORTANT
----------------------------------------------------

Enter a point like this:

5 1

NOT:

(5,1)

NOT:

5,1

Because the code uses:

cin >> P.x >> P.y;

====================================================
*/