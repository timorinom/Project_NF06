typedef struct CartItem
{
    int productIndex;
    int quantity;
    struct CartItem *next;
} CartItem;//Defines the item structure for a cart item, which is the index for a product, an quantity wanted and a pointer to the next link of the chained list

void addToCartCLI(CartItem **cart);
float cartTotal(CartItem *cart);
void showCart(CartItem *cart);
void emptyCart(CartItem *cart);
int validateStock(CartItem *cart);
int checkCart(CartItem *cart);
