#include <stdio.h>
#include <ctype.h>
#include <cs50.h>
#include <string.h>

int main(int argc, string argv[])
{

    //accept only one command line argument
    if (argc != 2)
    {
        printf("Try again with a single command line argument.\n");
        return 1;
    }

    string key = (argv[1]);



    //accept only alphabetical key
    for (int i = 0; i < strlen(key); i++)

        if (!isalpha(key[i]))
        {
            printf("Try again with an alphabetical key.\n");
            return 1;
        }

    //get a message to be encrypted
    string plain = get_string("plaintext: ");
    printf("ciphertext: ");

    //encipher the message
    for (int i = 0, alpha_counter = 0; i < strlen(plain); i++)

        if isalpha(plain[i])
        {
            int j = (alpha_counter % strlen(key));

            //encipher uppercase letters of the message
            if isupper(plain[i])
            {
                printf("%c", ((((int)plain[i] - 65) + ((int)toupper(key[j]) - 65)) % 26) + 65);
                alpha_counter++;
            }

            //encipher lowercase letters of the message
            if islower(plain[i])
            {
                printf("%c", ((((int)plain[i] - 97) + ((int)tolower(key[j]) - 97)) % 26) + 97);
                alpha_counter++;
            }

        }

    //encipher non-alphabetical characters
        else
        {
            printf("%c", plain[i]);
        }
    printf("\n");



}