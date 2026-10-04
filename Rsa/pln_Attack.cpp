#include <iostream>
using namespace std;


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
    long long n, M, C, maxD;


    // =================================================
    // KNOWN INFORMATION
    //
    // Attacker knows:
    //
    // n = RSA public modulus
    // M = known plaintext
    // C = corresponding ciphertext
    //
    // Attacker does NOT know private exponent d.
    // =================================================

    cout << "Enter public modulus n: ";
    cin >> n;

    cout << "Enter known plaintext M: ";
    cin >> M;

    cout << "Enter known ciphertext C: ";
    cin >> C;

    cout << "Enter maximum d to search: ";
    cin >> maxD;


    // =================================================
    // KNOWN-PLAINTEXT BRUTE FORCE
    //
    // Try:
    //
    // d = 1, 2, 3, ...
    //
    // and check:
    //
    // C^d mod n == M
    //
    // If equal, d is a possible private exponent.
    // =================================================

    long long foundD = -1;

    for(long long d = 1; d <= maxD; d++)
    {
        long long decrypted = modPower(C, d, n);

        if(decrypted == M)
        {
            foundD = d;
            break;
        }
    }


    // =================================================
    // RESULT
    // =================================================

    if(foundD == -1)
    {
        cout << "\nNo possible d found in the given range." << endl;
    }
    else
    {
        cout << "\n----- Attack Result -----" << endl;
        cout << "Possible private exponent d = " << foundD << endl;

        long long recoveredM = modPower(C, foundD, n);

        cout << "Recovered plaintext = " << recoveredM << endl;

        if(recoveredM == M)
            cout << "Known-plaintext brute-force attack SUCCESSFUL." << endl;
        else
            cout << "Attack FAILED." << endl;
    }


    return 0;
}


/*
================ INPUT FORMAT ================

n = 143
M = 9
C = 48
maxD = 150

Input:

143
9
48
150

Expected:

Possible private exponent d = 103
Recovered plaintext = 9

==============================================
*/