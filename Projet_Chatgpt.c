
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PRODUCTS 100

typedef struct {
    char name[50];
    char category[30];
    float price;
    int stock;
} Product;

typedef struct CartItem {
    int productIndex;
    int quantity;
    struct CartItem* next;
} CartItem;

typedef struct Order {
    int id;
    char address[100];
    float total;
    int priority;
    CartItem* items;
    struct Order* next;
} Order;

typedef struct {
    Order* front;
    Order* rear;
} Queue;

typedef struct {
    int deliveredOrders;
    float revenue;
} Statistics;

Product products[MAX_PRODUCTS];
int productCount = 0;

Queue q1 = {NULL,NULL};
Queue q2 = {NULL,NULL};

Statistics stats = {0,0};

int nextOrderId = 1;
int scheduleCycle = 0;

void trim(char *s){
    s[strcspn(s,"\r\n")] = 0;
}

int loadProducts(const char* filename){
    FILE* f = fopen(filename,"r");
    char line[256];
    if(!f) return 0;

    while(fgets(line,sizeof(line),f)){
        trim(line);
        if(strlen(line)==0) continue;

        Product p;
        char cat[30], name[50];

        if(sscanf(line," %29[^,],%49[^,],%f,%d",cat,name,&p.price,&p.stock)==4){
            strcpy(p.category,cat);
            strcpy(p.name,name);
            products[productCount++] = p;
        }
    }
    fclose(f);
    return 1;
}

void saveProducts(const char* filename){
    FILE* f = fopen(filename,"w");
    int i;
    for(i=0;i<productCount;i++){
        fprintf(f,"%s,%s,%.2f,%d\n",
                products[i].category,
                products[i].name,
                products[i].price,
                products[i].stock);
    }
    fclose(f);
}

void displayProducts(){
    int i;
    printf("\n===== PRODUCTS =====\n");
    for(i=0;i<productCount;i++){
        printf("%d. [%s] %s - %.2f EUR - Stock: %d\n",
               i+1,
               products[i].category,
               products[i].name,
               products[i].price,
               products[i].stock);
    }
}

void addToCart(CartItem** cart,int idx,int qty){
    CartItem* n = malloc(sizeof(CartItem));
    n->productIndex = idx;
    n->quantity = qty;
    n->next = *cart;
    *cart = n;
}

float cartTotal(CartItem* cart){
    float total = 0;
    while(cart){
        total += products[cart->productIndex].price * cart->quantity;
        cart = cart->next;
    }
    return total;
}

void showCart(CartItem* cart){
    printf("\n===== CART =====\n");
    while(cart){
        printf("%s x%d\n",
               products[cart->productIndex].name,
               cart->quantity);
        cart = cart->next;
    }
}

int validateStock(CartItem* cart){
    while(cart){
        int stock = products[cart->productIndex].stock;
        if(stock != -1 && cart->quantity > stock)
            return 0;
        cart = cart->next;
    }
    return 1;
}

void updateStock(CartItem* cart){
    while(cart){
        if(products[cart->productIndex].stock != -1)
            products[cart->productIndex].stock -= cart->quantity;
        cart = cart->next;
    }
}

void enqueue(Queue* q, Order* o){
    o->next = NULL;
    if(q->rear == NULL){
        q->front = q->rear = o;
    }else{
        q->rear->next = o;
        q->rear = o;
    }
}

Order* dequeue(Queue* q){
    if(q->front == NULL) return NULL;

    Order* tmp = q->front;
    q->front = q->front->next;

    if(q->front == NULL)
        q->rear = NULL;

    return tmp;
}

int empty(Queue* q){
    return q->front == NULL;
}

void writeHistory(Order* o){
    char filename[32];

    time_t t = time(NULL);
    struct tm* tmv = localtime(&t);

    sprintf(filename,"%02d-%02d-%04d.txt",
            tmv->tm_mday,
            tmv->tm_mon+1,
            tmv->tm_year+1900);

    FILE* f = fopen(filename,"a");
    if(!f) return;

    fprintf(f,"%d;%s;%.2f\n",
            o->id,
            o->address,
            o->total);

    fclose(f);
}

void processDelivery(){
    Order* o = NULL;

    if(scheduleCycle < 2){
        if(!empty(&q1)){
            o = dequeue(&q1);
            scheduleCycle++;
        }else if(!empty(&q2)){
            o = dequeue(&q2);
        }
    }else{
        if(!empty(&q2)){
            o = dequeue(&q2);
            scheduleCycle = 0;
        }else if(!empty(&q1)){
            o = dequeue(&q1);
        }
    }

    if(o == NULL){
        printf("No orders waiting.\n");
        return;
    }

    printf("\nDelivered order #%d -> %s (%.2f EUR)\n",
           o->id,o->address,o->total);

    stats.deliveredOrders++;
    stats.revenue += o->total;

    writeHistory(o);

    free(o);
}

void customerMode(){
    CartItem* cart = NULL;
    int choice;

    do{
        printf("\n--- CUSTOMER ---\n");
        printf("1. View products\n");
        printf("2. Add to cart\n");
        printf("3. View cart\n");
        printf("4. Validate order\n");
        printf("0. Back\n");
        scanf("%d",&choice);

        if(choice==1){
            displayProducts();
        }

        else if(choice==2){
            int id,qty;

            displayProducts();

            printf("Product number: ");
            scanf("%d",&id);

            if(id<1 || id>productCount){
                printf("Invalid product.\n");
                continue;
            }

            printf("Quantity: ");
            scanf("%d",&qty);

            id--;

            if(products[id].stock == 0){
                printf("Unavailable.\n");
                continue;
            }

            if(products[id].stock != -1 &&
               qty > products[id].stock){
                printf("Not enough stock.\n");
                continue;
            }

            addToCart(&cart,id,qty);
        }

        else if(choice==3){
            showCart(cart);
            printf("Total: %.2f EUR\n",cartTotal(cart));
        }

        else if(choice==4){
            if(cart == NULL){
                printf("Cart empty.\n");
                continue;
            }

            if(!validateStock(cart)){
                printf("Stock changed.\n");
                continue;
            }

            Order* o = malloc(sizeof(Order));

            o->id = nextOrderId++;
            o->total = cartTotal(cart);
            o->items = cart;

            printf("Address: ");
            scanf(" %[^\n]",o->address);

            printf("Priority (1=high 2=normal): ");
            scanf("%d",&o->priority);

            updateStock(cart);

            if(o->priority == 1)
                enqueue(&q1,o);
            else
                enqueue(&q2,o);

            printf("Order validated. ID=%d\n",o->id);

            cart = NULL;
        }

    }while(choice != 0);
}

void adminMode(){
    int c;

    do{
        printf("\n--- ADMIN ---\n");
        printf("1. Process delivery\n");
        printf("2. Statistics\n");
        printf("0. Back\n");
        scanf("%d",&c);

        if(c==1){
            processDelivery();
        }
        else if(c==2){
            printf("Delivered: %d\n",stats.deliveredOrders);
            printf("Revenue: %.2f EUR\n",stats.revenue);
        }

    }while(c != 0);
}

int main(){
    int choice;

    if(!loadProducts("stock.csv")){
        printf("stock.csv not found.\n");
        return 1;
    }

    do{
        printf("\n===== E-COMMERCE DELIVERY =====\n");
        printf("1. Customer\n");
        printf("2. Administrator\n");
        printf("3. Exit\n");
        printf("Choice: ");
        scanf("%d",&choice);

        switch(choice){
            case 1:
                customerMode();
                break;
            case 2:
                adminMode();
                break;
            case 3:
                saveProducts("stock.csv");
                printf("Stock saved.\n");
                break;
            default:
                printf("Invalid choice.\n");
        }

    }while(choice != 3);

    return 0;
}
