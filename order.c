#include "order.h"
#include "stock.h"
#include "cart.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Queue q1 = {NULL, NULL};
Queue q2 = {NULL, NULL};

Statistics stats = {0, 0};

int nextOrderId = 1;
int scheduleCycle = 0;

int validateStock(CartItem *cart)
{
    while (cart)
    {
        int stock = getProduct(cart->productIndex).stock;
        if (stock != -1 && cart->quantity > stock)
            return 0;
        cart = cart->next;
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

void enqueue(Queue *q, Order *o)
{
    o->next = NULL;
    if (q->rear == NULL)
    {
        q->front = q->rear = o;
    }
    else
    {
        q->rear->next = o;
        q->rear = o;
    }
}

Order *dequeue(Queue *q)
{
    if (q->front == NULL)
        return NULL;

    Order *tmp = q->front;
    q->front = q->front->next;

    if (q->front == NULL)
        q->rear = NULL;

    return tmp;
}

int empty(Queue *q)
{
    return q->front == NULL;
}

void writeHistory(Order *o)
{
    char filename[32];

    time_t t = time(NULL);
    struct tm *tmv = localtime(&t);

    sprintf(filename, "%02d-%02d-%04d.txt",
            tmv->tm_mday,
            tmv->tm_mon + 1,
            tmv->tm_year + 1900);

    FILE *f = fopen(filename, "a");
    if (!f)
        return;

    fprintf(f, "%d;%s;%.2f\n",
            o->id,
            o->address,
            o->total);

    fclose(f);
}

void processDelivery()
{
    Order *o = NULL;

    if (scheduleCycle < 2)
    {
        if (!empty(&q1))
        {
            o = dequeue(&q1);
            scheduleCycle++;
        }
        else if (!empty(&q2))
        {
            o = dequeue(&q2);
        }
    }
    else
    {
        if (!empty(&q2))
        {
            o = dequeue(&q2);
            scheduleCycle = 0;
        }
        else if (!empty(&q1))
        {
            o = dequeue(&q1);
        }
    }

    if (o == NULL)
    {
        printf("No orders waiting.\n");
        return;
    }

    printf("\nDelivered order #%d -> %s (%.2f EUR)\n",
           o->id, o->address, o->total);

    stats.deliveredOrders++;
    stats.revenue += o->total;

    writeHistory(o);

    free(o);
}

int validateCart(CartItem *cart)
{
    if (cart == NULL)
    {
        printf("Cart empty.\n");
        return 0;
    }

    if (!validateStock(cart))
    {
        printf("Stock changed.\n");
        return 0;
    }

    Order *o = malloc(sizeof(Order));

    o->id = nextOrderId++;
    o->total = cartTotal(cart);
    o->items = cart;

    printf("Address: ");
    scanf(" %[^\n]", o->address);

    printf("Priority (1=high 2=normal): ");
    scanf("%d", &o->priority);

    updateStock(cart);

    if (o->priority == 1)
        enqueue(&q1, o);
    else
        enqueue(&q2, o);

    printf("Order validated. ID=%d\n", o->id);

    cart = NULL;
    return 1;
}

Statistics getStats()
{
    return stats;
}