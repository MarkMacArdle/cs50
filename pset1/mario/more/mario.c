#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //get positive number less than 24 from user
    int n;
    do
    {
        n = get_int("Enter positive number less than 24: ");
    }
    while (n < 0 || n > 23);

    for (int i = 0; i < n; i++)
    {
        int brick_start = n - i - 1;

        //print up side of pyramid
        for (int j = 0; j < n; j++)
        {
            //print either a brick or space
            if (brick_start <= j)
            {
                printf("#");
            }
            else
            {
                printf(" ");
            }
        }

        //two spaces for the gap in the middle
        printf("  ");

        //print downside of pyramid
        for (int k = 0; k < (n - brick_start); k++)
        {
            printf("#");
        }

        printf("\n");
    }
}