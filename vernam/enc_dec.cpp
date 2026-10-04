#include <iostream>
#include <string>
#include <vector>
#include <bitset>
using namespace std;


// ----------------------------------------------------
// Convert one character into 8-bit binary.
//
// Example:
// 'A' = ASCII 65
// Binary = 01000001
// ----------------------------------------------------
string toBinary(char ch)
{
    return bitset<8>((unsigned char)ch).to_string();
}


// ----------------------------------------------------
// Convert an 8-bit binary string back to character.
//
// Example:
// 01000001 -> 65 -> 'A'
// ----------------------------------------------------
char binaryToChar(string binary)
{
    return (char)bitset<8>(binary).to_ulong();
}


// ----------------------------------------------------
// XOR two 8-bit binary strings.
//
// XOR rule:
// 0 XOR 0 = 0
// 0 XOR 1 = 1
// 1 XOR 0 = 1
// 1 XOR 1 = 0
//
// Same bits     -> 0
// Different bits -> 1
// ----------------------------------------------------
string xorBinary(string a, string b)
{
    string result = "";

    for(int i = 0; i < 8; i++)
    {
        if(a[i] == b[i])
            result += '0';
        else
            result += '1';
    }

    return result;
}


// ----------------------------------------------------
// VERNAM ENCRYPTION
//
// Steps for every character:
//
// 1. Convert plaintext character to binary
// 2. Convert key character to binary
// 3. XOR the two binary values
// 4. Store the XOR result as ciphertext
//
// Formula:
// C = P XOR K
// ----------------------------------------------------
vector<string> encryptVernam(string plain, string key)
{
    vector<string> cipher;

    for(int i = 0; i < plain.length(); i++)
    {
        string plainBinary = toBinary(plain[i]);
        string keyBinary = toBinary(key[i]);

        // XOR plaintext bits with key bits
        string cipherBinary = xorBinary(plainBinary, keyBinary);

        cipher.push_back(cipherBinary);
    }

    return cipher;
}


// ----------------------------------------------------
// VERNAM DECRYPTION
//
// Decryption also uses XOR.
//
// Formula:
// P = C XOR K
//
// Because:
// (P XOR K) XOR K = P
// ----------------------------------------------------
string decryptVernam(vector<string> cipher, string key)
{
    string plain = "";

    for(int i = 0; i < cipher.size(); i++)
    {
        string keyBinary = toBinary(key[i]);

        // XOR ciphertext with the SAME key
        string plainBinary = xorBinary(cipher[i], keyBinary);

        // Convert recovered binary back to character
        char decrypted = binaryToChar(plainBinary);

        plain += decrypted;
    }

    return plain;
}


int main()
{
    string plain, key;

    cout << "Enter plaintext: ";
    getline(cin, plain);

    cout << "Enter key: ";
    getline(cin, key);


    // For this simple Vernam implementation,
    // plaintext and key must have the same length.
    if(plain.length() != key.length())
    {
        cout << "Error: Key length must be equal to plaintext length." << endl;
        return 0;
    }


    // -----------------------------
    // ENCRYPTION
    // -----------------------------
    vector<string> cipher = encryptVernam(plain, key);


    cout << "\n----- Encryption -----" << endl;

    for(int i = 0; i < plain.length(); i++)
    {
        cout << "\nPlain char : " << plain[i] << endl;
        cout << "Plain bin  : " << toBinary(plain[i]) << endl;

        cout << "Key char   : " << key[i] << endl;
        cout << "Key bin    : " << toBinary(key[i]) << endl;

        cout << "Cipher bin : " << cipher[i] << endl;
    }


    // Print complete ciphertext in binary
    cout << "\nCiphertext: ";

    for(string bits : cipher)
        cout << bits << " ";

    cout << endl;


    // -----------------------------
    // DECRYPTION
    // -----------------------------
    string decrypted = decryptVernam(cipher, key);

    cout << "\n----- Decryption -----" << endl;
    cout << "Decrypted Text: " << decrypted << endl;


    return 0;
}


/*
INPUT FORMAT

Enter plaintext: HELLO
Enter key: XMCKL

The key must have the same number of characters
as the plaintext.

Example:

Plaintext = HELLO   -> 5 characters
Key       = XMCKL   -> 5 characters
*/