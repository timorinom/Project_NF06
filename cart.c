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

int validateStock(CartItem *cart)
{
    CartItem *current = cart;

    while (current)
    {
        int totalQuantity = current->quantity;
        CartItem *next = current->next;

        while (next)
        {
            if (next->productIndex == current->productIndex)
            {
                totalQuantity += next->quantity;
            }
            next = next->next;
        }

        int stock = getProduct(current->productIndex).stock;
        if (stock != -1 && totalQuantity > stock)
        {
            return 0;
        }

        current = current->next;
    }

    return 1;
}

void updateStock(CartItem *cart)
{
    while (cart)
    {
        if (getProduct(cart->productIndex).stock != -1)
            getProductRef(cart->productIndex)->stock -= cart->quantity;
        cart = cart->next;
    }
}

int checkCart(CartItem *cart)
{
    if (cart == NULL)
    {
        printf("Cart empty.\n");
        return 0;
    }

    if (!validateStock(cart))
    {
        printf("Not enough stock.\n");
        return 0;
    }

    return 1;
}

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

    CartItem future = {.productIndex = id, .quantity = qty, .next = *cart};

    if (!checkCart(&future))
    {
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