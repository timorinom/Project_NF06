#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "stock.h"
#include "cli.h"

int main()
{
    if (loadProducts("stock.csv") != 0) //Checks if the stock.csv file exists if so loads the products
    {
        printf("stock.csv not found.\n");
        return 1;
    }

    int choice;
    do
    {
        printf("\n------ e-commerce delivery ------ \n"); //Prints the available actions
        printf("1. Customer\n");
        printf("2. Administrator\n");
        printf("0. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) //Gets the number of the chosen action
        {
            printf("Invalid number\n");
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF)
                ;
            choice = -1;
            continue;
        }

        if (choice == 0) //Exits the program
        {
        }
        else if (choice == 1) //Enters customer mode
        {
            customerMode();
        }
        else if (choice == 2) //Enters administrator mode
        {
            adminMode();
        }
        else
        {
            printf("Invalid choice.\n");
        }

    } while (choice != 0);

    saveProducts("stock.csv"); //Saves the stock into the stock.csv file
    printf("Stock saved.\n");

    return 0;
}
