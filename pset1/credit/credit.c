#include <cs50.h>
#include <stdio.h>

int return_digits(int n);

int main(void)
{

    long long int n = get_long_long("Number: ");
    //eprintf("n: %lld\n", n);
    int digit;
    int prev_digit;
    int doubled_digit;
    int loop_count = 0;
    int checksum = 0;

    while (n)
    {
        loop_count += 1;
        prev_digit = digit;
        digit = n % 10;
        n /= 10;
        //eprintf("loop: %i\n", loop_count);
        //eprintf("digit: %i\n", digit);

        if (loop_count % 2 == 0)
        {
            doubled_digit = digit * 2;
            if (doubled_digit >= 10)
            {
                //eprintf("doubled_digit: %i\n", doubled_digit);
                //if 10 or more add first then second digit
                //can't get a three digit number from add two one digits
                //so no need for a loop
                checksum += doubled_digit % 10;
                doubled_digit /= 10;
                checksum += doubled_digit;
            }
            else
            {
                checksum += doubled_digit;
            }
        }
        else
        {
            checksum += digit;
        }

        //eprintf("checksum = %i\n", checksum);

    }

    //default is number isn't valid
    string s = "INVALID";

    //test if checksum result valid of not
    if (checksum % 10 == 0)
    {
        //loop_count conditions for how many digits should be in card number
        if (digit == 4 && (loop_count == 13 || loop_count == 16))
        {
            s = "VISA";
        }
        else if (digit == 3 && (prev_digit == 4 || prev_digit == 7) && loop_count == 15)
        {
            s = "AMEX";
        }
        else if (digit == 5 && prev_digit >= 1 && prev_digit <= 5 && loop_count == 16)
        {
            s = "MASTERCARD";
        }

        //used to when checking if it works, but question asks for
        //only VISA, AMEX, MASTERCARD or INVALID to be the output
        //so commenting it out
        //else {s = "valid but diff card";}
    }

    printf("%s\n", s);
}
