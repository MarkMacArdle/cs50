#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int print_cipher_letter(char letter, int ascii_base, int cipher_step);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        return 1;
    }
    else
    {
        int cipher_step = atoi(argv[1]) % 26; //get modulo to deal with values >26
        eprintf("%i\n", cipher_step);
        string plaintext = get_string("plaintext: ");
        char letter;
        int ascii_num;

        printf("ciphertext: ");
        for (int i = 0, sl = strlen(plaintext); i < sl; i++)
        {
            letter = plaintext[i];

            if (letter >= 'A' && letter <= 'Z')
            {
                print_cipher_letter(letter, (int) 'A', cipher_step);
            }
            else if (letter >= 'a' && letter <= 'z')
            {
                print_cipher_letter(letter, (int) 'a', cipher_step);
            }
            else
            {
                printf("%c", letter);
            }
        }
q
        printf("\n");
    }
}

int print_cipher_letter(char letter, int ascii_base, int cipher_step)
{
    int pos_in_alphabet = ((int) letter) - ascii_base;
    char cipher_letter = ((pos_in_alphabet + cipher_step) % 26) + ascii_base;
    printf("%c", cipher_letter);

    return 0;
}