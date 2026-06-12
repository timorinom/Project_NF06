typedef struct CartItem
{
    int productIndex;
    int quantity;
    struct CartItem *next;
} CartItem;

void addToCartCLI(CartItem **cart);
float cartTotal(CartItem *cart);
void showCart(CartItem *cart);
void emptyCart(CartItem *cart);
int validateStock(CartItem *cart);
int checkCart(CartItem *cart);