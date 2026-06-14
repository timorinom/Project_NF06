#include "cli.h"
#include "stock.h"
#include "cart.h"
#include <stdio.h>
#include <stdlib.h>
#include "order.h"

//Function for the command line interface of customer mode
void customerMode()
{
    CartItem *cart = NULL;
    int choice;
    do
    {
        printf("\n------ CUSTOMER MODE ------\n");//Prints the available actions
        printf("1. View products\n");
        printf("2. View all categories\n");
        printf("3. Add to cart\n");
        printf("4. View cart\n");
        printf("5. Validate order\n");
        printf("0. Back\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1)//Gets the number of the chosen action
        {
            printf("Invalid number\n");
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF)
                ;

            continue;
            choice = -1;
        }

        if (choice == 0) //Breaks the do while and ends customer mode
            {
            }
        else if (choice == 1) //Displays the product
        {
            displayProducts();
        }
        else if (choice == 2) //Shows categories
        {
            showAllCategories();
        }
        else if (choice == 3) //Ads an item to the cart
        {
            addToCartCLI(&cart);
        }
        else if (choice == 4) //Shows the cart
        {
        showCart(cart);
        }
        else if (choice == 5) //Validates the cart and sends it into the order queues
        {
            if (validateCart(cart))
            {
                emptyCart(cart);
                cart = NULL;
            }
        }
        else //If another number is entered
        {
            printf("Invalid choice.\n");
        }

    } while (choice != 0);
}

//Function for the command line interface of admin mode
void adminMode()
{
    int choice;
    do
    {
        printf("\n------ ADMIN MODE ------\n");//Displays available actions
        printf("1. Process delivery\n");
        printf("2. Statistics\n");
        printf("0. Back\n");
        if (scanf("%d", &choice) != 1)//Checks if input is valid
        {
            printf("Invalid number\n");
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF)
                ;
            choice = -1;
            continue;
        }

        if (choice == 0)//Breaks and ends the function
        {
        }
        else if (choice == 1)//Processes the deliveries
        {
            processDelivery();
        }
        else if (choice == 2)//Gets the stats of todays deliveries
        {
            printf("Delivered: %d\n", getStats().deliveredOrders);
            printf("Revenue: %.2f EUR\n", getStats().revenue);
        }
        else //Invalid choice
        {
            printf("Invalid choice.\n");
        }

    } while (choice != 0);
}
