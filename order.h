#include "cart.h"

typedef struct Order
{
    int id;
    char address[100];
    float total;
    int priority;
    CartItem *items;
    struct Order *next;
} Order;

typedef struct
{
    Order *front;
    Order *rear;
} Queue;

typedef struct
{
    int deliveredOrders;
    float revenue;
} Statistics;

int validateStock(CartItem *cart);
void updateStock(CartItem *cart);
void enqueue(Queue *q, Order *o);
Order *dequeue(Queue *q);
void processDelivery();
int validateCart(CartItem *cart);
Statistics getStats();
