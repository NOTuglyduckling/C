#include <cs50.h>
#include <stdio.h>

int length(long card);
int *make_array(int *nums, long card, int n);
void get_company(int num[], long card, int n);
bool valid(long card, int n);

int main(void)
{
    //get a card number from user
    long card = get_long("Type the creditcard number: ");
    int n = length(card);
    int *num;
    int nums[n];
    num = make_array(nums, card, n);
    get_company(num, card, n);
}

void get_company(int num[], long card, int n)
{
    if (valid(card, n))
    {
        //decides what card it is
        if (n == 15 && (num[0] == 3 && (num[1] == 4 || num[1] == 7)))
        {
            printf("AMEX\n");
        }
        else if (n == 16 && (num[0] == 5 && (num[1] == 1 || num[1] == 2 || num[1] == 3 || num[1] == 4 || num[1] == 5)))
        {
            printf("MASTERCARD\n");
        }
        else if ((n == 13 || n == 16) && num[0] == 4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}

bool valid(long card, int n)
{
    // check if it validates Luhn's Algorithm
    int sum = 0;
    do
    {
        //impair digit
        int d = card % 10;
        card /= 10;
        sum += d;
        //pair digit
        d = card % 10;
        card /= 10;
        d *= 2;
        int d2 = d % 10;
        d /= 10;
        sum += d + d2;
    }
    while (card > 0);
    return (sum % 10 == 0);
}

int length(long card)
{
    //get the length of the card number
    int n = 0;
    long v = card;
    while (v > 0)
    {
        v /= 10;
        n++;
    }
    return n;
}

int *make_array(int *nums, long card, int n)
{
    //make array to compare
    while (card)
    {
        for (int i = 1; i <= n; i++)
        {
            nums[n - i] = card % 10;
            card /= 10;
        }
    }
    return nums;
}