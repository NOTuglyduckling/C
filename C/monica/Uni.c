#include <stdio.h>
#include <stdlib.h>

// Function to display the player's options and get their choice
int getPlayerChoice() {
    int choice;
    printf("\nWhat do you want to do?\n");
    printf("1. Go left\n");
    printf("2. Go right\n");
    printf("3. Open the door\n");
    printf("4. Look around\n");
    printf("5. Ride the unicorn\n");
    printf("6. Kiss Luna\n");
    printf("7. Quit the game\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    return choice;
}

int main() {
    int playerChoice;
    int gameOver = 0;
    int hasKey = 0;
    int metUnicorn = 0;
    int unicornRide = 0;
    int foundKeys = 0;

    printf("Welcome to the Text Adventure Game!\n");
    printf("You find yourself in a mysterious room.\n");

    while (!gameOver) {
        playerChoice = getPlayerChoice();

        switch (playerChoice) {
            case 1:
                if (unicornRide) {
                    printf("\nYou cannot go back after riding the unicorn.\n");
                } else {
                    printf("\nYou go left and find a locked door.\n");
                    if (hasKey) {
                        printf("You use the key to unlock the door and discover a secret passage!\n");
                        printf("You continue your adventure.\n");
                        hasKey = 0;
                    } else {
                        printf("The door is locked. You need a key.\n");
                    }
                }
                break;
            case 2:
                if (unicornRide) {
                    printf("\nYou ride the unicorn and reach the Enchanted Forest.\n");
                    printf("The unicorn shares its wisdom with you as you explore.\n");
                    printf("You find a hidden cave and discover a chest of gold!\n");
                    printf("Congratulations! You've won the game!\n");
                    gameOver = 1;
                } else {
                    if (!metUnicorn) {
                        printf("\nAs you go right, you encounter a magical unicorn!\n");
                        printf("The unicorn's name is Luna, and she says, 'I love you. Do you want to ride me?'\n");
                        printf("Luna is known for her kindness and wisdom among unicorns.\n");
                        metUnicorn = 1;
                    } else {
                        printf("\nYou go right and encounter a hostile monster. Prepare for a fight!\n");
                        // Implement a combat system or simply end the game here.
                        gameOver = 1;
                    }
                }
                break;
            case 3:
                if (unicornRide) {
                    printf("\nYou cannot open the door while riding the unicorn.\n");
                } else {
                    printf("\nYou open the door and find a small key on a table.\n");
                    printf("You take the key with you.\n");
                    hasKey = 1;
                }
                break;
            case 4:
                if (unicornRide) {
                    printf("\nYou look around while riding Luna.\n");
                    printf("She tells you stories of her magical adventures and the secrets of the forest.\n");
                } else {
                    printf("\nYou look around the room and find a hidden note.\n");
                    printf("The note reads, 'To escape, find the three hidden keys.'\n");
                    if (foundKeys == 3) {
                        printf("You've found all the keys required to escape!\n");
                    }
                }
                break;
            case 5:
                if (metUnicorn && !unicornRide) {
                    printf("\nLuna says, 'I love you too! Let's go on an adventure!'\n");
                    printf("You mount Luna, and she takes you on a magical journey through the Enchanted Forest.\n");
                    unicornRide = 1;
                } else if (unicornRide) {
                    printf("\nYou are already riding Luna.\n");
                } else {
                    printf("\nThere's no unicorn here to ride.\n");
                }
                break;
            case 6:
                if (metUnicorn) {
                    printf("\nYou lean in and kiss Luna on her magical horn. She smiles and nuzzles you affectionately.\n");
                } else {
                    printf("\nYou try to kiss the air, but there's no one to kiss.\n");
                }
                break;
            case 7:
                printf("\nYou quit the game. Thanks for playing!\n");
                gameOver = 1;
                break;
            default:
                printf("\nInvalid choice. Please select a valid option.\n");
        }
    }

    return 0;
}
