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

void addToCart(CartItem **cart, int idx, int qty)
{
    CartItem *n = malloc(sizeof(CartItem));
    n->productIndex = idx;
    n->quantity = qty;
    n->next = *cart;
    *cart = n;
}

void addToCartCLI(CartItem **cart)
{
    int id, qty;

    displayProducts();

    printf("Product number: ");
    scanf("%d", &id);

    if (id < 1 || id > getProductCount())
    {
        printf("Invalid product.\n");
        return;
    }

    printf("Quantity: ");
    scanf("%d", &qty);

    id--;

    if (getProduct(id).stock == 0)
    {
        printf("Unavailable.\n");
        return;
    }

    if (getProduct(id).stock != -1 && qty > getProduct(id).stock)
    {
        printf("Not enough stock.\n");
        return;
    }
    if (qty < 1)
    {
        printf("Choose a valid number as quantity\n");
        return;
    }

    addToCart(cart, id, qty);
}

void showCart(CartItem *cart)
{

    if (cart == NULL)
    {
        printf("\nCart empty.\n");
        return;
    }

    float price = 0;

    printf("\nCart : \n");
    while (cart)
    {
        printf("%s x%d\n",
               getProduct(cart->productIndex).name,
               cart->quantity);
        price += getProduct(cart->productIndex).price * cart->quantity;
        cart = cart->next;
    }

    printf("Total : %.2fEUR\n", price);
}

float cartTotal(CartItem *cart)
{
    float total = 0;
    while (cart)
    {
        total += getProduct(cart->productIndex).price * cart->quantity;
        cart = cart->next;
    }
    return total;
}

void emptyCart(CartItem *cart)
{
    while (cart)
    {
        CartItem *tmp = cart;
        cart = cart->next;
        free(tmp);
    }
}