#include "cart.h"

typedef struct Order
{
    int id;//ID of the order
    char address[100];//Address of the order
    float total;//Total price of the order
    int priority;//Level of priority of the order
    CartItem *items;//Items in the order (see cart.c and cart.h)
    struct Order *next;//Pointer to the next link in the queue
} Order;

typedef struct
{
    Order *front;//Head of the queue
    Order *rear;//Last element (tail) of the queue
} Queue;

typedef struct
{
    int deliveredOrders;//Number of delivered orders
    float revenue;//Total revenue
} Statistics;

int validateStock(CartItem *cart);
void updateStock(CartItem *cart);
void enqueue(Queue *q, Order *o);
Order *dequeue(Queue *q);
void processDelivery();
int validateCart(CartItem *cart);
Statistics getStats();
