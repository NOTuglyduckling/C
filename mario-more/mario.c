#include <cs50.h>
#include <stdio.h>

int get_height(void);
void build(int height);

int main(void)
{
    int height = get_height();
    build(height);
}

int get_height(void)
{
    int height;
    do
    {
        height = get_int("height: ");
    }
    while (height < 1 || 150 < height);
    return height;
}

void build(int height)
{
    for (int i = 1; i <= height; i++)
    {
        // spaces
        for (int j = 1; j <= height - i; j++)
        {
            printf(" ");
        }
        // blocks
        for (int l = 1; l <= i; l++)
        {
            printf("#");
        }
        // center partition
        printf("  ");
        // and now for the other side bricks, mirrored
        for (int l = 1; l <= i; l++)
        {
            printf("#");
        }
        printf("\n");
    }
}