#include <iostream>
#include <string>
using namespace std;


// ----------------------------------------------------
// DECRYPTION FUNCTION
//
// Formula:
// P = (C - key) mod 26
//
// This function decrypts the ciphertext
// using one specific key.
// ----------------------------------------------------
string decryptCaesar(string cipher, int key)
{
    string plain = "";

    key = key % 26;  // Keep key between 0 and 25

    // Check every character one by one
    for(char ch : cipher)
    {
        // If the character is uppercase
        if(ch >= 'A' && ch <= 'Z')
        {
            // ch-'A' converts A-Z into 0-25
            // -key shifts the letter backward
            // +26 prevents negative values
            // %26 keeps the result between 0 and 25
            // +'A' converts the number back to a letter
            char decrypted = (ch - 'A' - key + 26) % 26 + 'A';
            plain += decrypted;
        }

        // If the character is lowercase
        else if(ch >= 'a' && ch <= 'z')
        {
            // Same logic as uppercase, but using 'a'
            char decrypted = (ch - 'a' - key + 26) % 26 + 'a';
            plain += decrypted;
        }

        // Space, number or symbol stays unchanged
        else
        {
            plain += ch;
        }
    }

    return plain;
}


int main()
{
    string cipher;

    // Teacher gives only ciphertext
    cout << "Enter ciphertext: ";
    getline(cin, cipher);


    cout << "\nPossible plaintexts:\n" << endl;

    // Caesar cipher has only 25 useful non-zero shifts.
    // Try every possible key from 1 to 25.
    for(int key = 1; key < 26; key++)
    {
        string plain = decryptCaesar(cipher, key);

        cout << "Key " << key << " : " << plain << endl;
    }

    return 0;
}


/*
INPUT:

KHOOR


OUTPUT:

Key 1 : JGNNQ
Key 2 : IFMMP
Key 3 : HELLO
Key 4 : GDKKN
...
Key 25 : LIPPS


Here we can see:

Key 3 : HELLO

So the original Caesar key was 3.
*/