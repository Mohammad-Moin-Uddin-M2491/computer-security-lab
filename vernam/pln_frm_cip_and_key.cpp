#include <iostream>
#include <string>
#include <vector>
#include <bitset>
using namespace std;


// ----------------------------------------------------
// Convert one character into 8-bit binary.
//
// Example:
// 'K' = 01001011
// ----------------------------------------------------
string toBinary(char ch)
{
    return bitset<8>((unsigned char)ch).to_string();
}


// ----------------------------------------------------
// Convert 8-bit binary back into a character.
//
// Example:
// 01000001 -> 'A'
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
// 00001010
// 01001011
// --------
// 01000001
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
// DECRYPTION
//
// Formula:
//
// P = C XOR K
//
// cipher[i] is already binary.
//
// The key character is first converted to binary,
// then XORed with the ciphertext.
// ----------------------------------------------------
string decryptVernam(vector<string> cipher, string key)
{
    string plain = "";

    for(int i = 0; i < cipher.size(); i++)
    {
        // Convert corresponding key character to binary
        string keyBinary = toBinary(key[i]);

        // XOR ciphertext bits with key bits
        string plainBinary = xorBinary(cipher[i], keyBinary);

        // Convert recovered binary back into character
        char decrypted = binaryToChar(plainBinary);

        plain += decrypted;
    }

    return plain;
}


int main()
{
    int n;
    string key;

    // Number of ciphertext binary blocks
    cout << "Enter number of characters: ";
    cin >> n;

    vector<string> cipher(n);

    // Enter each ciphertext character as 8-bit binary
    cout << "Enter ciphertext in binary:" << endl;

    for(int i = 0; i < n; i++)
    {
        cin >> cipher[i];
    }


    cout << "Enter key: ";
    cin >> key;


    // Ciphertext and key must have same length
    if(key.length() != n)
    {
        cout << "Error: Key length must match ciphertext length." << endl;
        return 0;
    }


    // -----------------------------
    // DECRYPTION
    // -----------------------------
    string plain = decryptVernam(cipher, key);


    cout << "\n----- Decryption -----" << endl;

    // Show what happens for every character
    for(int i = 0; i < n; i++)
    {
        string keyBinary = toBinary(key[i]);
        string plainBinary = xorBinary(cipher[i], keyBinary);

        cout << "\nCipher bin : " << cipher[i] << endl;
        cout << "Key char   : " << key[i] << endl;
        cout << "Key bin    : " << keyBinary << endl;
        cout << "Plain bin  : " << plainBinary << endl;
        cout << "Plain char : " << binaryToChar(plainBinary) << endl;
    }


    cout << "\nRecovered Plaintext: " << plain << endl;

    return 0;
}


/*
INPUT FORMAT

Enter number of characters: 5

Enter ciphertext in binary:
00010000
00001000
00001111
00000111
00000011

Enter key:
XMCKL


OUTPUT:

Recovered Plaintext: HELLO
*/