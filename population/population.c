#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt for start size
    int population;
    do
    {
        population = get_int("Population start size: ");
    }
    while (population < 9);
    // Prompt for end size
    int end;
    do
    {
        end = get_int("Population end size: ");
    }
    while (end < population);
    // Calculate number of years until we reach threshold
    int years = 0;
    while (population < end)
    {
        population += (population / 3) - (population / 4);
        years++;
    }
    // Print number of years
    printf("It will take %i years.\n", years);
}
