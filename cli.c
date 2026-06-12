#include "cli.h"
#include "stock.h"
#include "cart.h"
#include <stdio.h>
#include <stdlib.h>
#include "order.h"

void customerMode()
{
    CartItem *cart = NULL;
    int choice;
    do
    {
        printf("\n------ CUSTOMER MODE ------\n");
        printf("1. View products\n");
        printf("2. View all categories\n");
        printf("3. Add to cart\n");
        printf("4. View cart\n");
        printf("5. Validate order\n");
        printf("0. Back\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid number\n");
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF)
                ;

            continue;
            choice = -1;
        }

        switch (choice)
        {
        case 0:
            break;
        case 1:
            displayProducts();
            break;
        case 2:
            showAllCategories();
            break;
        case 3:
            addToCartCLI(&cart);
            break;
        case 4:
            showCart(cart);
            break;
        case 5:
            if (validateCart(cart))
            {
                emptyCart(cart);
                cart = NULL;
            }
            break;
        default:
            printf("Invalid choice.\n");
        }

    } while (choice != 0);
}

void adminMode()
{
    int choice;
    do
    {
        printf("\n------ ADMIN MODE ------\n");
        printf("1. Process delivery\n");
        printf("2. Statistics\n");
        printf("0. Back\n");
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
            processDelivery();
            break;
        case 2:
            printf("Delivered: %d\n", getStats().deliveredOrders);
            printf("Revenue: %.2f EUR\n", getStats().revenue);
            break;
        default:
            printf("Invalid choice.\n");
        }

    } while (choice != 0);
}
