#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "stock.h"

Product products[MAX_PRODUCTS];
int productCount = 0;

int loadProducts(const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (!f)
        return 0;

    Product p;

    while (1)
    {

        // GET CATEGORY
        int i = 0;
        char c;
        while ((c = fgetc(f)) != EOF && c != ',')
        {
            if (c != '\n' && c != '\r')
            {
                p.category[i++] = c;
            }
        }
        p.category[i] = '\0';

        // GET NAME
        i = 0;
        while ((c = fgetc(f)) != EOF && c != ',')
        {
            p.name[i++] = c;
        }
        p.name[i] = '\0';

        // GET PRICE AND STOCK
        if (fscanf(f, "%f,%d", &p.price, &p.stock) != 2)
        {
            break;
        }

        fgetc(f);

        products[productCount] = p;
        productCount++;
    }

    fclose(f);
    return 1;
}

void displayProducts()
{
    int i;
    printf("\nProducts :\n");
    for (i = 0; i < productCount; i++)
    {
        if (products[i].stock == 0)
            continue;
        printf("%d. [%s] %s - %.2fEUR - Stock: %d\n",
               i + 1,
               products[i].category,
               products[i].name,
               products[i].price,
               products[i].stock);
    }
}

void showAllCategories()
{
    int i, j;
    char categories[MAX_PRODUCTS][30];
    int categoryCount = 0;

    for (i = 0; i < productCount; i++)
    {
        int found = 0;
        for (j = 0; j < categoryCount; j++)
        {
            if (strcmp(categories[j], products[i].category) == 0)
            {
                found = 1;
                break;
            }
        }
        if (!found)
        {
            strcpy(categories[categoryCount++], products[i].category);
        }
    }

    printf("\nCategories :\n");
    for (i = 0; i < categoryCount; i++)
    {
        printf("%d. %s\n", i + 1, categories[i]);
    }
}

void saveProducts(const char *filename)
{
    FILE *f = fopen(filename, "w");
    int i;
    for (i = 0; i < productCount; i++)
    {
        fprintf(f, "%s,%s,%.2f,%d\n",
                products[i].category,
                products[i].name,
                products[i].price,
                products[i].stock);
    }
    fclose(f);
}

int getProductCount()
{
    return productCount;
}

Product getProduct(int index)
{
    return products[index];
}

Product *getProductRef(int index)
{
    return &products[index];
}
