#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "stock.h"
#include "cli.h"

int main()
{
    if (!loadProducts("stock.csv"))
    {
        printf("stock.csv not found.\n");
        return 1;
    }

    int choice;
    do
    {
        printf("\n------ e-commerce delivery ------ \n");
        printf("1. Customer\n");
        printf("2. Administrator\n");
        printf("0. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid number\n");
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF)
                ;
            choice = -1;
            continue;
        }

        switch (choice)
        {
        case 0:
            break;
        case 1:
            customerMode();
            break;
        case 2:
            adminMode();
            break;
        default:
            printf("Invalid choice.\n");
        }

    } while (choice != 0);

    printf("Stock saved.\n");
    saveProducts("stock.csv");

    return 0;
}