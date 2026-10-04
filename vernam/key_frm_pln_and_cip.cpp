#include <iostream>
#include <string>
#include <vector>
#include <bitset>
using namespace std;


// ----------------------------------------------------
// Convert one character into 8-bit binary.
//
// Example:
// 'H' = 01001000
// ----------------------------------------------------
string toBinary(char ch)
{
    return bitset<8>((unsigned char)ch).to_string();
}


// ----------------------------------------------------
// Convert 8-bit binary back into a character.
//
// Example:
// 01011000 -> 'X'
// ----------------------------------------------------
char binaryToChar(string binary)
{
    return (char)bitset<8>(binary).to_ulong();
}


// ----------------------------------------------------
// XOR two 8-bit binary strings.
//
// XOR rule:
//
// Same bits      -> 0
// Different bits -> 1
//
// Example:
//
// 01001000
// 00010000
// --------
// 01011000
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
// RECOVER KEY
//
// Formula:
//
// K = P XOR C
//
// Plaintext character is converted to binary,
// then XORed with ciphertext binary.
// ----------------------------------------------------
string recoverKey(string plain, vector<string> cipher)
{
    string key = "";

    for(int i = 0; i < plain.length(); i++)
    {
        // Convert plaintext character into binary
        string plainBinary = toBinary(plain[i]);

        // XOR plaintext binary with ciphertext binary
        string keyBinary = xorBinary(plainBinary, cipher[i]);

        // Convert recovered key binary into character
        char keyChar = binaryToChar(keyBinary);

        key += keyChar;
    }

    return key;
}


int main()
{
    string plain;
    int n;

    cout << "Enter plaintext: ";
    getline(cin, plain);

    // Number of ciphertext binary blocks
    n = plain.length();

    vector<string> cipher(n);

    cout << "Enter ciphertext in binary:" << endl;

    // Take one 8-bit ciphertext block
    // for each plaintext character
    for(int i = 0; i < n; i++)
    {
        cin >> cipher[i];
    }


    // -----------------------------
    // RECOVER THE KEY
    // -----------------------------
    string key = recoverKey(plain, cipher);


    cout << "\n----- Key Recovery -----" << endl;

    // Show calculation for each character
    for(int i = 0; i < n; i++)
    {
        string plainBinary = toBinary(plain[i]);

        // K = P XOR C
        string keyBinary = xorBinary(plainBinary, cipher[i]);

        cout << "\nPlain char : " << plain[i] << endl;
        cout << "Plain bin  : " << plainBinary << endl;
        cout << "Cipher bin : " << cipher[i] << endl;
        cout << "Key bin    : " << keyBinary << endl;
        cout << "Key char   : " << binaryToChar(keyBinary) << endl;
    }


    cout << "\nRecovered Key: " << key << endl;

    return 0;
}


/*
INPUT FORMAT

Enter plaintext:
HELLO

Enter ciphertext in binary:
00010000
00001000
00001111
00000111
00000011


OUTPUT:

Recovered Key: XMCKL
*/