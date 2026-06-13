typedef struct
{
    char name[50];
    char category[30];
    float price;
    int stock; // -1 means unlimited
} Product;//Product structure as defined in the project instructions

int loadProducts(const char *filename);
void saveProducts(const char *filename);
void displayProducts();
int getProductCount();
Product *getProductRef(int index);
Product getProduct(int index);
void showAllCategories();

#define MAX_PRODUCTS 100
