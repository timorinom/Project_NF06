#include <stdio.h>
#include <stdlib.h>
#include "stock.h"
#include "cart.h"
#include "order.h"

typedef struct CartItem
{
    int productIndex;
    int quantity;
    struct CartItem *next;
} CartItem;

//Function to check if stock of ordered products is sufficient
int validateStock(CartItem *cart)
{
    CartItem *current = cart;

    while (current != NULL)//Enters validation process
    {
        int totalQuantity = current->quantity; //Initialises quantity for a product
        CartItem *next = current->next;

        while (next != NULL)//Checks for doubles
        {
            if (next->productIndex == current->productIndex)
            {
                totalQuantity += next->quantity;
            }
            next = next->next;
        }

        int stock = getProduct(current->productIndex).stock; //Gets stock of product
        if (stock != -1 && totalQuantity > stock) //Checks if quantity is sufficient
        {
            return 0;
        }

        current = current->next; //Goes to next
    }

    return 1;
}

//Function to update the stock list
void updateStock(CartItem *cart)
{
    while (cart) //Updates for each cart item
    {
        if (getProduct(cart->productIndex).stock != -1)
            getProductRef(cart->productIndex)->stock -= cart->quantity;
        cart = cart->next;
    }
}

//Function to check if cart empty or if stock isn't sufficient
int checkCart(CartItem *cart)
{
    if (cart == NULL) //Empty cart checking
    {
        printf("Cart empty.\n");
        return 0;
    }

    if (!validateStock(cart)) //Not sufficient stock checking
    {
        printf("Not enough stock.\n");
        return 0;
    }

    return 1;
}

//Ads an item to the cart chained list
void addToCart(CartItem **cart, int idx, int qty)
{
    CartItem *n = malloc(sizeof(CartItem));
    n->productIndex = idx;
    n->quantity = qty;
    n->next = *cart;
    *cart = n;
}

//Function for the client side of adding an item to the cart
void addToCartCLI(CartItem **cart)
{
    int id, qty;

    displayProducts();

    printf("Product number: "); //Asks for which product is wanted
    scanf("%d", &id);

    if (id < 1 || id > getProductCount()) //Checks if given product number is valid
    {
        printf("Invalid product.\n");
        return;
    }

    printf("Quantity: "); //Asks for quantity
    scanf("%d", &qty);

    id--;

    CartItem future = {.productIndex = id, .quantity = qty, .next = *cart}; //Creates the new item to be added in the cart

    if (!checkCart(&future)) //Checks if stock is sufficient
    {
        return;
    }
    if (qty < 1) //Checks if quantity is valid
    {
        printf("Choose a valid number as quantity\n");
        return;
    }

    addToCart(cart, id, qty); //Ads to the cart list
}

//Shows the content of the cart list
void showCart(CartItem *cart)
{

    if (cart == NULL)
    {
        printf("\nCart empty.\n");
        return;
    }

    float price = 0;

    printf("\nCart : \n");
    while (cart) //Prints name, quantity and total price of each item in the cart
    {
        printf("%s x%d\n",
               getProduct(cart->productIndex).name,
               cart->quantity);
        price += getProduct(cart->productIndex).price * cart->quantity;
        cart = cart->next;
    }

    printf("Total : %.2fEUR\n", price);
}

//Returs the total price of the cart
float cartTotal(CartItem *cart)
{
    float total = 0;
    while (cart)
    {
        total += getProduct(cart->productIndex).price * cart->quantity; //Sum of the price of every item
        cart = cart->next;
    }
    return total;
}

//Empties the cart
void emptyCart(CartItem *cart)
{
    while (cart)
    {
        CartItem *tmp = cart;
        cart = cart->next;
        free(tmp); //Frees every item
    }
}
