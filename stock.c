#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "stock.h"

Product products[MAX_PRODUCTS];
int productCount = 0;

//Function to load the stock from the stock.csv file)
int loadProducts(const char *filename)
{
    FILE *f = fopen(filename, "r");
    
    if (f == NULL)
        return 0;

    Product p;

    while (1)
    {
        //Gets categories
        int i = 0;
        char c;
        while ((c = fgetc(f)) != EOF && c != ',')
        {
            if (c != '\n' && c != '\r')
            {
                p.category[i] = c;
                i++;
            }
        }
        p.category[i] = '\0';

        //Gets names of the product
        i = 0;
        while ((c = fgetc(f)) != EOF && c != ',')
        {
            p.name[i] = c;
            i++;
        }
        p.name[i] = '\0';

        //Gets prices and stocks
        fscanf(f, "%f,%d", &p.price, &p.stock);
        if (feof(f))
            break;

        fgetc(f); //Takes the \n at the end of the line

        //Current product is registered, going to the next
        products[productCount] = p;
        productCount++;
    }

    fclose(f);
    return 1;
}
//Function to display the whole list product by product
void displayProducts()
{
    int i;
    printf("\nProducts :\n");
    for (i = 0; i < productCount; i++)
    {
        printf("%d. [%s] %s - %.2fEUR - Stock: %d\n",
               i + 1,
               products[i].category,
               products[i].name,
               products[i].price,
               products[i].stock);
    }
}

//Function to
void showAllCategories()
{
    int i, j;
    char categories[MAX_PRODUCTS][30]; //Declares a list of categories
    int categoryCount = 0;

    for (i = 0; i < productCount; i++) //Loop to check all products
    {
        int found = 0;
        for (j = 0; j < categoryCount; j++) //Loop to check all categories in categories[]
        {
            if (strcmp(categories[j], products[i].category) == 0) //Checks if there is a match
            {
                found = 1; //Orders not to copy category
                break;
            }
        }
        if (!found) //Copies if the category isn't already in categories[]
        {
            strcpy(categories[categoryCount++], products[i].category);
        }
    }

    printf("\nCategories :\n"); //Prints the list of categories
    for (i = 0; i < categoryCount; i++)
    {
        printf("%d. %s\n", i + 1, categories[i]);
    }
}

//Function to save the new product list into the file
void saveProducts(const char *filename)
{
    FILE *f = fopen(filename, "w"); //Opens the file in write mode (the file is rewritten from 0)
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

//Self explanatory
int getProductCount()
{
    return productCount;
}

//Self explanatory
Product getProduct(int index)
{
    return products[index];
}

//Same as getProduct but gives a pointer and not the value
Product *getProductRef(int index)
{
    return &products[index];
}
