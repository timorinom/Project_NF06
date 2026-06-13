#include "order.h"
#include "stock.h"
#include "cart.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Queue structure described in order.h, composed of a front (head) and a rear (tail)
Queue q1 = {NULL, NULL};
Queue q2 = {NULL, NULL};

Statistics stats = {0, 0};

//Initialises general data
int nextOrderId = 1;
int scheduleCycle = 0;

//Ads an element to the queue
void enqueue(Queue *q, Order *o) //q is like the head of the list, and o is the order to be added
{
    o->next = NULL; //The pointer to the next link is set to NULL as the new link is now the end of the list
    if (q->rear == NULL) //If the list was empty
    {
        q->front = q->rear = o; //Both the front and rear are set to the adress of the new link
    }
    else
    {
        q->rear->next = o; //The new link is attached to the former last one, and the rear is set to the new last link
        q->rear = o;
    }
}

//Deletes an element from the queue
Order *dequeue(Queue *q)
{
    if (q->front == NULL) //If the list is NULL, the function isn't executed
        return NULL;

    Order *tmp = q->front; //A buffer for the front of the list, which will be set to the link next to the front link
    q->front = q->front->next;

    if (q->front == NULL) //If there was only one link, the list is now empty
        q->rear = NULL;

    return tmp; //Returns the former front of the list
}

//Fully empties the queue by setting the head to NULL
int empty(Queue *q)
{
    return q->front == NULL;
}

//writes a file about an order
void writeHistory(Order *o)
{
    char filename[32];

    time_t t = time(NULL);
    struct tm *tmv = localtime(&t); //Gets the current time

    sprintf(filename, "%02d-%02d-%04d.txt",
            tmv->tm_mday,
            tmv->tm_mon + 1,
            tmv->tm_year + 1900); //Uses the time of the order to generate a name for the file

    FILE *f = fopen(filename, "a");
    if (f == NULL)
        return;

    fprintf(f, "%d;%s;%.2f\n",
            o->id,
            o->address,
            o->total); //Prints the order in the file

    fclose(f);
}

//Processes an order
void processDelivery()
{
    
    Order *o = NULL;

    if (scheduleCycle < 2) //Checks if a priority order is to be processed
    {
        if (!empty(&q1))
        {
            o = dequeue(&q1); //An order to be treated is deleted from the queue and the cycle is incremented
            scheduleCycle++;
        }
        else if (!empty(&q2)) //If priority queue is empty
        {
            o = dequeue(&q2);
        }
    }
    else //A normal order is to be treated
    {
        if (!empty(&q2))
        {
            o = dequeue(&q2);
            scheduleCycle = 0;
        }
        else if (!empty(&q1)) //If normal queue is empty
        {
            o = dequeue(&q1);
        }
    }

    if (o == NULL)
    {
        printf("No orders waiting.\n");
    }
    else {
        printf("\nDelivered order #%d -> %s (%.2f EUR)\n",
               o->id, o->address, o->total);

        stats.deliveredOrders++;
        stats.revenue += o->total; //Updates general stats

        writeHistory(o); //Writes a file about the order
        
        free(o);
    }
}

//Validates the cart and creates an order
int validateCart(CartItem *cart)
{
    if (!checkCart(cart)) //Checks if the cart is valid (see in cart.c and cart.h for the corresponding functions)
    {
        emptyCart(cart);
        return 0;
    }

    Order *o = malloc(sizeof(Order)); //Allocates space for an order

    o->id = nextOrderId++;
    o->total = cartTotal(cart);
    o->items = cart;

    printf("Address: ");
    scanf(" %[^\n]", o->address); //Asks for the adress of the order

    printf("Priority (1=high 2=normal): "); //Asks for priority
    scanf("%d", &o->priority);

    updateStock(cart); //Updates the stock (see stock.c and stock.h)

    if (o->priority == 1) //Ads to corresponding queue
        enqueue(&q1, o);
    else
        enqueue(&q2, o);

    printf("Order validated. ID=%d\n", o->id);

    cart = NULL;
    return 1;
}

//Self Explanatory
Statistics getStats()
{
    return stats;
}
