#include <stdio.h>

typedef struct {
    char name[50];
    char category[30];
    float price;
    int stock;
}Product;

typedef struct CartItem {
    int productIndex;
    int quantity;
    struct CartItem* next;
}CartItem;

typedef struct Order {
    int id;
    char address[100];
    float total;
    int priority;

    CartItem* items;

    struct Order* next;
}Order;

typedef struct {
    Order* front;
    Order* rear;
}Queue;

typedef struct {
    int deliveredOrders;
    float revenue;
} Statistics;


int loadProducts(char* filename);
void saveProducts(char* filename);
void displayCategories();
void displayProductsByCategory(char* category);
int findProduct(char* name);

void addToCart(CartItem** cart, int productIndex, int quantity);
void displayCart(CartItem* cart, Product products[]);
float calculateTotal(CartItem* cart, Product products[]);

void initQueue(Queue* q);
int isEmpty(Queue* q);
void enqueue(Queue* q, Order* order);
Order* dequeue(Queue* q);



int main() {
    Product products[500];
    int nbProducts = 0;
    Queue priority1;
    Queue priority2;
    Statistics stats = {0,0};
    stats.deliveredOrders++;
    stats.revenue += order->total;



    return 0;
}


void enqueue(Queue* q, Order* order)
{
    order->next = NULL;

    if(q->rear == NULL)
    {
        q->front = order;
        q->rear = order;
    }
    else
    {
        q->rear->next = order;
        q->rear = order;
    }
}


Order* dequeue(Queue* q)
{
    if(isEmpty(q))
        return NULL;

    Order* tmp = q->front;

    q->front = q->front->next;

    if(q->front == NULL)
        q->rear = NULL;

    return tmp;
}

int loadProducts(char* filename)
{
    char currentCategory[30];
    strcpy(currentCategory, line);
    Product p;

    sscanf(line, "%[^,],%f,%d", p.name, &p.price, &p.stock);

    strcpy(p.category, currentCategory);

    products[nbProducts++] = p;
}

void addToCart(CartItem** cart, int productIndex, int quantity)
{
    if(product.stock == 0)
    {
        printf("Unavailable\n");
    }
    else if(product.stock != -1 && quantity > product.stock)
    {
        printf("Not enough stock\n");
    }
}
