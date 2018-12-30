//cracked test hashes
//anushree: 50xcIMJ0y.RXo: YES
//brian:    50mjprEcqC/ts: CA
//bjbrown:  50GApilQSG3E2: UPenn
//lloyd:    50n0AAUD.pL8g: lloyd
//malan:    50CcfIk1QrPr6: maybe
//maria:    509nVI8B9VfuA: TF
//natmelo:  50JIIyhDORqMU: nope
//rob:      50JGnXUgaafgc: ROFL
//stelios:  51u8F0dkeDSbY: NO
//zamyla:   50cI2vYkF0YU2: LOL


#define _XOPEN_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./crack hash\n");
        return 1;
    }
    else
    {
        string hash = argv[1];
        string hash_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        int hash_chars_len = strlen(hash_chars);

        char salt[3];
        strncpy(salt, hash, 2);
        salt[2] = '\0'; //add null pointer on end

        //the \0 will just be moved along in loop as more characters needed
        //means array can be defined here and not need to be resized as more chars needed.
        char pw_check[6];

        //loop through all the single letter passwords, then two letter and so on
        //will end early if a match is found and print out that password to the user.
        for (int chars_in_pw = 1; chars_in_pw <= 5; chars_in_pw++)
        {
            pw_check[chars_in_pw] = '\0';

            for (int i = 0; i < hash_chars_len; i++)
            {
                pw_check[0] = hash_chars[i];

                if (chars_in_pw > 1)
                {
                    for (int j = 0; j < hash_chars_len; j++)
                    {
                        pw_check[1] = hash_chars[j];

                        if (chars_in_pw > 2)
                        {
                            for (int k = 0; k < hash_chars_len; k++)
                            {
                                pw_check[2] = hash_chars[k];

                                if (chars_in_pw > 3)
                                {
                                    for (int m = 0; m < hash_chars_len; m++)
                                    {
                                        pw_check[3] = hash_chars[m];

                                        if (chars_in_pw > 4)
                                        {
                                            for (int n = 0; n < hash_chars_len; n++)
                                            {
                                                pw_check[4] = hash_chars[n];

                                                if (strcmp(crypt(pw_check, salt), hash) == 0)
                                                {
                                                    printf("%s\n", pw_check);
                                                    return 0;
                                                }
                                            }
                                        }

                                        if (strcmp(crypt(pw_check, salt), hash) == 0)
                                        {
                                            printf("%s\n", pw_check);
                                            return 0;
                                        }
                                    }
                                }


                                if (strcmp(crypt(pw_check, salt), hash) == 0)
                                {
                                    printf("%s\n", pw_check);
                                    return 0;
                                }
                            }
                        }

                        if (strcmp(crypt(pw_check, salt), hash) == 0)
                        {
                            printf("%s\n", pw_check);
                            return 0;
                        }
                    }
                }

                if (strcmp(crypt(pw_check, salt), hash) == 0)
                {
                    printf("%s\n", pw_check);
                    return 0;
                }
            }
        }

        printf("password not found\n");
    }
}

