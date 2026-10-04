#include <iostream>
#include <string>
using namespace std;

// Encrypt plaintext using Caesar Cipher
string encryptCaesar(string text, int key)
{
    string cipher = "";
    key = key % 26;  // Keep key between 0 and 25

    // Check every character one by one
    for(char ch : text)
    {
        // If the character is uppercase
        if(ch >= 'A' && ch <= 'Z')
        {
            // ch-'A' converts A-Z into 0-25
            // +key shifts the letter forward
            // %26 wraps around after Z
            // +'A' converts the number back to a letter
            char encrypted = (ch - 'A' + key) % 26 + 'A';
            cipher += encrypted;
        }

        // If the character is lowercase
        else if(ch >= 'a' && ch <= 'z')
        {
            // Same logic as uppercase, but using 'a'
            char encrypted = (ch - 'a' + key) % 26 + 'a';
            cipher += encrypted;
        }

        // Space, number or symbol stays unchanged
        else
        {
            cipher += ch;
        }
    }

    return cipher;
}


// Decrypt ciphertext using Caesar Cipher
string decryptCaesar(string cipher, int key)
{
    string plain = "";
    key = key % 26;  // Keep key between 0 and 25

    for(char ch : cipher)
    {
        // Uppercase letter
        if(ch >= 'A' && ch <= 'Z')
        {
            // -key shifts backward
            // +26 prevents negative values
            char decrypted = (ch - 'A' - key + 26) % 26 + 'A';
            plain += decrypted;
        }

        // Lowercase letter
        else if(ch >= 'a' && ch <= 'z')
        {
            char decrypted = (ch - 'a' - key + 26) % 26 + 'a';
            plain += decrypted;
        }

        // Keep spaces and symbols unchanged
        else
        {
            plain += ch;
        }
    }

    return plain;
}


int main()
{
    string message;
    int key;

    // Take plaintext input
    cout << "Enter plaintext: ";
    getline(cin, message);

    // Take Caesar shift value
    cout << "Enter key: ";
    cin >> key;

    // Encrypt the plaintext
    string cipher = encryptCaesar(message, key);

    // Decrypt the generated ciphertext
    string decrypted = decryptCaesar(cipher, key);

    cout << "\nEncrypted Text: " << cipher << endl;
    cout << "Decrypted Text: " << decrypted << endl;

    return 0;
}


/*
Input:

HELLO
3

Output:

Encrypted Text: KHOOR
Decrypted Text: HELLO
*/