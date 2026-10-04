#include <iostream>
#include <vector>
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


// ----------------------------------------------------
// Check whether g is a primitive root of p
// ----------------------------------------------------
bool isPrimitiveRoot(long long g, long long p)
{
    vector<bool> used(p, false);

    for(long long i = 1; i <= p - 1; i++)
    {
        long long value = modPower(g, i, p);

        if(used[value]) return false;

        used[value] = true;
    }

    return true;
}


// ----------------------------------------------------
// Automatically find the first primitive root g
// ----------------------------------------------------
long long findPrimitiveRoot(long long p)
{
    for(long long g = 2; g < p; g++)
    {
        if(isPrimitiveRoot(g, p)) return g;
    }

    return -1;
}


// ----------------------------------------------------
// Find modular inverse
//
// a * inverse mod p = 1
// ----------------------------------------------------
long long modInverse(long long a, long long p)
{
    for(long long inverse = 1; inverse < p; inverse++)
    {
        if((a * inverse) % p == 1) return inverse;
    }

    return -1;
}


int main()
{
    long long p, x, M, k, r;


    // ------------------------------------------------
    // STEP 1: Enter prime p
    // ------------------------------------------------
    cout << "Enter prime p: ";
    cin >> p;


    // ------------------------------------------------
    // STEP 2: Automatically find generator g
    // ------------------------------------------------
    long long g = findPrimitiveRoot(p);

    if(g == -1)
    {
        cout << "Generator not found." << endl;
        return 0;
    }


    // ------------------------------------------------
    // STEP 3: Enter private key x
    // ------------------------------------------------
    cout << "Enter private key x: ";
    cin >> x;


    // ------------------------------------------------
    // Calculate public value:
    //
    // y = g^x mod p
    //
    // Public Key  = (p, g, y)
    // Private Key = x
    // ------------------------------------------------
    long long y = modPower(g, x, p);


    cout << "\n----- Key Generation -----" << endl;
    cout << "p = " << p << endl;
    cout << "g = " << g << endl;
    cout << "x = " << x << endl;
    cout << "y = " << y << endl;
    cout << "Public Key = (" << p << ", " << g << ", " << y << ")" << endl;
    cout << "Private Key = " << x << endl;


    // ------------------------------------------------
    // STEP 4: Enter original message M
    // ------------------------------------------------
    cout << "\nEnter message M: ";
    cin >> M;

    if(M <= 0 || M >= p)
    {
        cout << "Message must be between 1 and " << p - 1 << endl;
        return 0;
    }


    // ------------------------------------------------
    // STEP 5: Enter random k for encryption
    // ------------------------------------------------
    cout << "Enter random k: ";
    cin >> k;


    // =================================================
    // ORIGINAL ELGAMAL ENCRYPTION
    // =================================================

    // C1 = g^k mod p
    long long C1 = modPower(g, k, p);

    // C2 = M * y^k mod p
    long long C2 = (M * modPower(y, k, p)) % p;


    cout << "\n----- Original Encryption -----" << endl;
    cout << "C1 = " << C1 << endl;
    cout << "C2 = " << C2 << endl;
    cout << "Original Ciphertext = (" << C1 << ", " << C2 << ")" << endl;


    // =================================================
    // FIRST DECRYPTION
    //
    // Decrypt the ORIGINAL ciphertext
    // =================================================

    // s1 = C1^x mod p
    long long s1 = modPower(C1, x, p);

    // Find inverse of s1
    long long s1Inverse = modInverse(s1, p);

    if(s1Inverse == -1)
    {
        cout << "Could not find inverse." << endl;
        return 0;
    }

    // M = C2 * s1^(-1) mod p
    long long firstDecryptedM = (C2 * s1Inverse) % p;


    cout << "\n----- First Decryption -----" << endl;
    cout << "s1 = " << s1 << endl;
    cout << "s1 inverse = " << s1Inverse << endl;
    cout << "Decrypted M = " << firstDecryptedM << endl;


    // =================================================
    // RE-RANDOMIZATION
    // =================================================

    // Choose a NEW random value r
    cout << "\nEnter random r for re-randomization: ";
    cin >> r;


    // ------------------------------------------------
    // Create a new ciphertext of the SAME message.
    //
    // C1_new = C1 * g^r mod p
    //
    // C2_new = C2 * y^r mod p
    // ------------------------------------------------
    long long C1_new = (C1 * modPower(g, r, p)) % p;
    long long C2_new = (C2 * modPower(y, r, p)) % p;


    cout << "\n----- Re-randomization -----" << endl;
    cout << "New C1 = " << C1_new << endl;
    cout << "New C2 = " << C2_new << endl;
    cout << "Re-randomized Ciphertext = (" << C1_new << ", " << C2_new << ")" << endl;


    // =================================================
    // SECOND DECRYPTION
    //
    // Decrypt the RE-RANDOMIZED ciphertext
    // =================================================

    // s2 = C1_new^x mod p
    long long s2 = modPower(C1_new, x, p);

    // Find inverse of s2
    long long s2Inverse = modInverse(s2, p);

    if(s2Inverse == -1)
    {
        cout << "Could not find inverse." << endl;
        return 0;
    }

    // M = C2_new * s2^(-1) mod p
    long long secondDecryptedM = (C2_new * s2Inverse) % p;


    cout << "\n----- Second Decryption -----" << endl;
    cout << "s2 = " << s2 << endl;
    cout << "s2 inverse = " << s2Inverse << endl;
    cout << "Decrypted M = " << secondDecryptedM << endl;


    // =================================================
    // FINAL VERIFICATION
    //
    // Both ciphertexts should decrypt to the
    // same original message M.
    // =================================================

    cout << "\n----- Verification -----" << endl;
    cout << "Original Message = " << M << endl;
    cout << "First Decryption = " << firstDecryptedM << endl;
    cout << "Second Decryption = " << secondDecryptedM << endl;


    if(firstDecryptedM == M && secondDecryptedM == M)
        cout << "Re-randomization SUCCESSFUL." << endl;
    else
        cout << "Re-randomization FAILED." << endl;


    return 0;
}


/*
====================================================
INPUT FORMAT
====================================================

Enter prime p: 23
Enter private key x: 6
Enter message M: 10
Enter random k: 3
Enter random r for re-randomization: 4


EXPECTED IMPORTANT VALUES:

g = 5
y = 8


ORIGINAL ENCRYPTION:

C1 = 10
C2 = 14

Original Ciphertext = (10, 14)


FIRST DECRYPTION:

Decrypted M = 10


RE-RANDOMIZATION:

New C1 = 17
New C2 = 5

Re-randomized Ciphertext = (17, 5)


SECOND DECRYPTION:

Decrypted M = 10


FINAL IDEA:

Original Ciphertext       = (10, 14)
Re-randomized Ciphertext  = (17, 5)

The ciphertext changed,
but both decrypt to:

M = 10

Re-randomization SUCCESSFUL.

====================================================
*/