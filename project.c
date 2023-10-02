#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include<stdbool.h>
#define HASH_MAP_SIZE 100

struct HashMap {
    struct HashMapEntry* table[HASH_MAP_SIZE];
};
struct UserAccount{
    char username[50];
    char password[20];
    char address[100];
};
struct HashMapEntry {
    char username[50];
    struct UserAccount* account;
    struct HashMapEntry* next;
};
struct UserHashMap{
    struct HashMapEntry* table[HASH_MAP_SIZE];
};

void initUserHashMap(struct UserHashMap* hashmap){
    for(int i = 0; i<HASH_MAP_SIZE;i++){
        hashmap->table[i] = NULL;
    }
}

int chartoAscii(char* str){
    int hash = 0;//sum of ascii values
    int i = 0;
    while(str[i]!= '\0'){
        hash+= str[i];
        i++;
    }
    return hash;
}
// Function to search for a user by username in the hashmap
struct UserAccount* searchUser(struct UserHashMap* hashMap, char* username) {
    // Calculate the hash table index number for the username
    int index =  chartoAscii(username)%HASH_MAP_SIZE;
    // Get the pointer to the first entry in the corresponding bucket
    struct HashMapEntry* current = hashMap->table[index];


    while (current != NULL) {
        if (strcmp(current->account->username, username) == 0) {
            // Username found, return the associated user account
            return current->account;
        }
        current = current->next;
    }

    // Username not found in the hashmap
    return NULL;
}


void insertUser(struct UserHashMap* hashMap, char* username, struct UserAccount* newUser) {
    // Calculate the hash code for the username

    int index =  chartoAscii(username)%HASH_MAP_SIZE;//hashmap index number using

    // Create a new entry for the user
    struct HashMapEntry* newEntry = (struct HashMapEntry*)malloc(sizeof(struct HashMapEntry));
    strcpy(newEntry->username, username);
    newEntry->account = newUser;
    newEntry->next = NULL;

    // Check if the bucket is empty (no collisions)
    if (hashMap->table[index] == NULL) {
        // Set the new entry as the first entry in the bucket
        hashMap->table[index] = newEntry;
    } else {
        // Chaining  Handle collision by adding to the linked list
        struct HashMapEntry* current = hashMap->table[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newEntry;
    }
}

void registerUser(struct UserHashMap* hashmap,char* username, char* password, char* address ){
    if(searchUser(hashmap, username)!= NULL){
        printf("Username %s is already taken. Please try another name\n", username);
        return;
    }
    else{

    struct UserAccount* newUser = (struct UserAccount*)malloc(sizeof(struct UserAccount));
    strcpy(newUser->username, username);
    strcpy(newUser->password, password);
    strcpy(newUser->address,address);

    insertUser(hashmap,username,newUser);
}
//printf("User '%s' registered successfully.\n", username);

}

// Function to authenticate a user
struct UserAccount* authenticateUser(struct UserHashMap* hashMap, char* username, char* password) {
    struct UserAccount* user = searchUser(hashMap, username);

    if (user != NULL && strcmp(user->password, password) == 0) {//search in user account
        printf("User '%s' authenticated successfully.\n", username);
        return user;
    }
    printf("Authentication failed for user '%s'.\n", username);
    return NULL;
}

// Define a structure for an order
struct Order {
    int productId;
    char name[50];
    char address[100];
    char phoneNumber[20];
};

/*-----------------------------------------BST Searching functions-------------------------------------------------------------------*/
struct Product {
    char name[100];
    double price;
    int id;
};

struct TreeNode {
    char key[100];
    struct Product product;
    struct TreeNode* left;
    struct TreeNode* right;
};

// Function to create a new TreeNode
struct TreeNode* createNode(const char* key, struct Product product) {
    struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    if (newNode != NULL) {
        strcpy(newNode->key, key);
        newNode->product = product;
        newNode->left = newNode->right = NULL;
    }
    return newNode;
}

// Function to insert a product into the BST
struct TreeNode* insert(struct TreeNode* root, const char* key, struct Product product) {
    if (root == NULL) {
        return createNode(key, product);
    }

    int compareResult = strcmp(key, root->key);

    if (compareResult < 0) {
        root->left = insert(root->left, key, product);
    } else if (compareResult > 0) {
        root->right = insert(root->right, key, product);
    }

    return root;
}

// Function to search for a product in the BST
struct TreeNode* searchProduct(struct TreeNode* root, const char* key) {
    if (root == NULL) {
        return NULL;
    }

    int compareResult = strcmp(key, root->key);

    if (compareResult < 0) {
        return searchProduct(root->left, key);
    } else if (compareResult > 0) {
        return searchProduct(root->right, key);
    } else {
        return root;
    }
}

// Function to display product details
void displayProductDetails(struct Product product) {

    printf("\t+-------------------------------+\n");

	 printf("\t|\tProduct ID: %d\t\t|\n", product.id);
    printf("\t|\tProduct Name: %s\t|\n", product.name);
    printf("\t|\tProduct Price: %.2lf\t|\n", product.price);
    printf("\t+-------------------------------+\n");


}

void saveProductsToFile(struct TreeNode* root, const char* filename) {
    FILE* file = fopen(filename, "w"); // Open the file for writing
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    // Write each product node to the file
    saveProductToFile(root, file);

    fclose(file);
}

// Helper function to recursively save product nodes to a text file
void saveProductToFile(struct TreeNode* node, FILE* file) {
    if (node == NULL) {
        return;
    }

    fprintf(file, "%s %.2lf %d\n", node->key, node->product.price, node->product.id);

    saveProductToFile(node->left, file);
    saveProductToFile(node->right, file);
}

// Function to load products from a text file with product IDs
struct TreeNode* loadProductsFromFile(const char* filename) {
    FILE* file = fopen(filename, "r"); // Open the file for reading (text mode)
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return NULL;
    }

    struct TreeNode* root = NULL;
    struct Product tempProduct;
    char tempKey[100];

    while (fscanf(file, "%s %lf %d", tempKey, &tempProduct.price, &tempProduct.id) != EOF) {
        strcpy(tempProduct.name, tempKey);
        root = insert(root, tempKey, tempProduct);
    }

    fclose(file);
    return root;
}
void displayProductsFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Unable to open file: %s\n", filename);
        return;
    }

    struct Product product;
    while (fscanf(file, "%s %lf %d", product.name, &product.price, &product.id) == 3) {
        displayProductDetails(product);
    }

    fclose(file);
}


struct TreeNode* inOrderPredecessor(struct TreeNode* node) {
    struct TreeNode* current = node->left;
    while (current && current->right) {
        current = current->right;
    }
    return current;
}

// Function to delete a node with a given key from the BST
struct TreeNode* deleteNode(struct TreeNode* root, const char* key) {
    if (root == NULL) {
        return root;
    }

    // smaller means its in left subtree
    if (strcmp(key, root->key) < 0) {
        root->left = deleteNode(root->left, key);
    }
    // bigger means its in right subtree
    else if (strcmp(key, root->key) > 0) {
        root->right = deleteNode(root->right, key);
    }
    // key in same as the root's key, then this is the node to be deleted
    else {
        // Node with only one child or no child
        if (root->left == NULL) {
            struct TreeNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct TreeNode* temp = root->left;
            free(root);
            return temp;
        }

        // Node with two children: Get the in-order predecessor
        struct TreeNode* temp = inOrderPredecessor(root);

        // Copy the in-order predecessor's content to this node
        strcpy(root->key, temp->key);
        root->product = temp->product;

        // Delete the in-order predecessor
        root->left = deleteNode(root->left, temp->key);
    }
    return root;
}


/*-----------------------------------------Maxheap functions-------------------------------------------------------------------*/

struct MaxHeap {
    struct Product* array;
    int size;
    int capacity;
};

// Function to create a new max-heap
struct MaxHeap* createMaxHeap(int capacity) {
    struct MaxHeap* heap = (struct MaxHeap*)malloc(sizeof(struct MaxHeap));
    heap->capacity = capacity;
    heap->size = 0;
    heap->array = (struct Product*)malloc(sizeof(struct Product) * capacity);
    return heap;
}

// Function to swap two products in the heap
void swap(struct Product* a, struct Product* b) {
    struct Product temp = *a;
    *a = *b;
    *b = temp;
}


void maxHeapify(struct MaxHeap* heap, int index) {
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap->size && heap->array[left].price > heap->array[largest].price) {
        largest = left;
    }

    if (right < heap->size && heap->array[right].price > heap->array[largest].price) {
        largest = right;
    }

    if (largest != index) {
        swap(&heap->array[index], &heap->array[largest]);
        maxHeapify(heap, largest);
    }
}

// Function to insert a new product into the max-heap
void insertMax(struct MaxHeap* heap, struct Product product) {
    if (heap->size == heap->capacity) {
        printf("Heap is full. Cannot insert.\n");
        return;
    }

    int index = heap->size;
    heap->array[index] = product;
    heap->size++;

    while (index > 0 && heap->array[index].price > heap->array[(index - 1) / 2].price) {
        swap(&heap->array[index], &heap->array[(index - 1) / 2]);
        index = (index - 1) / 2;
    }
}

// Function to read product data from a file and build a max-heap based on prices
struct MaxHeap* buildMaxHeapFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Unable to open file: %s\n", filename);
        return NULL;
    }

    int capacity = 10; // Initial capacity
    struct MaxHeap* heap = createMaxHeap(capacity);

    while (!feof(file)) {
        struct Product product;
        if (fscanf(file, "%s %lf %d", product.name, &product.price, &product.id) == 3) {
            insertMax(heap, product);
        }
    }

    fclose(file);
    return heap;
}


void printMaxHeap(struct MaxHeap* heap) {
    // Create a copy of the heap to preserve the original data
    struct MaxHeap tempHeap = *heap;

    while (tempHeap.size > 0) {
        // extract min
        struct Product maxProduct = tempHeap.array[0];

        printf("\t+-------------------------------+\n");
        printf("\t|\tProduct ID: %d\t\t|\n", maxProduct.id);
        printf("\t|\tProduct Name: %s\t|\n", maxProduct.name);
        printf("\t|\tProduct Price: %.2lf\t|\n", maxProduct.price);
        printf("\t+-------------------------------+\n");


        tempHeap.array[0] = tempHeap.array[tempHeap.size - 1];
        tempHeap.size--;


        maxHeapify(&tempHeap, 0);
    }

    printf("\n");
}

/*-----------------------------------------Minheap functions-------------------------------------------------------------------*/

struct MinHeap {
    struct Product* array;
    int size;
    int capacity;
};


struct MinHeap* createMinHeap(int capacity) {
    struct MinHeap* heap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    heap->capacity = capacity;
    heap->size = 0;
    heap->array = (struct Product*)malloc(sizeof(struct Product) * capacity);
    return heap;
}


void minHeapify(struct MinHeap* heap, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap->size && heap->array[left].price < heap->array[smallest].price) {
        smallest = left;
    }

    if (right < heap->size && heap->array[right].price < heap->array[smallest].price) {
        smallest = right;
    }

    if (smallest != index) {
        swap(&heap->array[index], &heap->array[smallest]);
        minHeapify(heap, smallest);
    }
}


void insertMin(struct MinHeap* heap, struct Product product) {
    if (heap->size == heap->capacity) {
        printf("Heap is full. Cannot insert.\n");
        return;
    }

    int index = heap->size;
    heap->array[index] = product;
    heap->size++;

    while (index > 0 && heap->array[index].price < heap->array[(index - 1) / 2].price) {
        swap(&heap->array[index], &heap->array[(index - 1) / 2]);
        index = (index - 1) / 2;
    }
}


struct MinHeap* buildMinHeapFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Unable to open file: %s\n", filename);
        return NULL;
    }

    int capacity = 10; // Initial capacity
    struct MinHeap* heap = createMinHeap(capacity);

    while (!feof(file)) {
        struct Product product;
        if (fscanf(file, "%s %lf %d", product.name, &product.price, &product.id) == 3) {
            insertMin(heap, product);
        }
    }

    fclose(file);
    return heap;
}



void printMinHeap(struct MinHeap* heap) {
    struct MinHeap tempHeap = *heap; // Create a copy of the heap

    while (tempHeap.size > 0) {

        struct Product minProduct = tempHeap.array[0];
        printf("\t+-------------------------------+\n");
        printf("\t|\tProduct ID: %d\t\t|\n", minProduct.id);
        printf("\t|\tProduct Name: %s\t|\n", minProduct.name);
        printf("\t|\tProduct Price: %.2lf\t|\n", minProduct.price);
        printf("\t+-------------------------------+\n");


        tempHeap.array[0] = tempHeap.array[tempHeap.size - 1];
        tempHeap.size--;


        minHeapify(&tempHeap, 0);
    }
    printf("\n");
}



int main(){

    struct TreeNode* root = NULL;

    // Insert products into the BST
    struct Product product1 = { "Apple", 100 ,1 };
    root = insert(root, "Apple", product1);
     struct Product product2 = { "Red_Apple", 500, 3 };
    root = insert(root, "Red_Apple", product2);
    struct Product product3= { "Banana", 80 , 2};
    root = insert(root, "Banana", product3);
 struct Product product4= { "Grapes", 350 , 4};
    root = insert(root, "Grapes", product4);
 struct Product product5= { "Orange", 200 , 123};
    root = insert(root, "Orange", product5);

    // Save products to a text file
    saveProductsToFile(root, "products.txt");

    // Load products from the file
    struct TreeNode* loadedRoot = loadProductsFromFile("products.txt");



      printf("+-------------------------------------------------------+\n");
      printf("|            **** FOOD ORDERING SYSTEM ****       	|\n");
      printf("+-------------------------------------------------------+\n\n\n");


 while (1) {
 	printf("+-------------------------------------------------------+\n");
		printf("|\t\t ALL PRODUCTS LIST\t\t\t|\n");
		printf("+-------------------------------------------------------+\n");
 	displayProductsFromFile("products.txt");
    // Display the main menu
     printf("+-------------------------------------------------------+\n");
    printf("1 : Search Product \n");
    printf("2 : Buy Product[use product id]\n");//customer
    printf("3 : Add Product[Admin] \n");//admin
    printf("4 : Remove Product[Admin]\n");//admin
    printf("5 : Filter Product \n");
    printf("6 : Exit\n");
        printf("+-------------------------------------------------------+\n");
    printf("\nChoose Option :  ");

     int choice;
    scanf("%d", &choice);
    printf("\n");
    // Handle the user's choice
    switch (choice) {
     case 1:
{
    // Prompt the user to enter the product name to search for
    char searchKey[100];
    printf("Enter the product name to search for: ");
    scanf("%s", searchKey);

    // Search for the product
    struct TreeNode* result = searchProduct(loadedRoot, searchKey);

    if (result != NULL) {
        printf("Product Found:\n");
        displayProductDetails(result->product);

    } else {
        printf("Product Not Found: %s\n", searchKey);
    }
    break;
}
        // BUY PRODUCT
     case 2:
{
    struct Order order;

    printf("Enter the product ID to buy: ");
    scanf("%d", &order.productId);
    printf("Enter your name: ");
    scanf("%s", order.name);
    printf("Enter your address: ");
    scanf(" %[^\n]", order.address);  // Read the address with spaces
    printf("Enter your phone number: ");
    scanf("%s", order.phoneNumber);

    // Open the order list file in append mode
    FILE *orderFile = fopen("order_list.txt", "a");

    if (orderFile != NULL)
    {
        // Write the order information to the file
        fprintf(orderFile, "%d %s %s %s\n", order.productId, order.name, order.address, order.phoneNumber);
        fclose(orderFile);

        printf("Order placed successfully.\n");
    }
    else
    {
        printf("Error opening order list file.\n");
    }
    break;
}



      case 3:
        // ADD PRODUCT
  {
 bool innerExit = false;
         while (!innerExit){
      printf ("1. Login Account\n");
      printf ("2. Exit\n");

      int userChoice;
      scanf ("%d", &userChoice);
      switch (userChoice){
		case 1: {
    struct UserHashMap* userHashMap;
    initUserHashMap(&userHashMap);

    // Register system owner
    registerUser(&userHashMap, "system", "4321", "foodShop");

    char username[50];
    char password[50];

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    // Authenticate the user
    struct UserAccount* loggedInUser = authenticateUser(&userHashMap, username, password);

    if (loggedInUser != NULL) {
        char productName[100];
        double price = 0.0;
        int id = 0;

        printf("Product Name: ");
        scanf("%s", productName);
        printf("Product Price: ");
        scanf("%lf", &price); // Use %lf for double
        printf("Product ID: ");
        scanf("%d", &id);

        // Initialize a new product
        struct Product newProduct = {productName, price, id};

        root = insert(root, productName, newProduct);

        saveProductsToFile(root, "products.txt");

        printf("Product has been added and saved to 'products.txt'.\n");
    } else {
        printf("You are not authorized to add products.\n");
    }
    break;
}


  case 2:{
		 innerExit = true;
               break;
  }
      default:
                    printf("Invalid choice\n");
            }
        }
        break;
    }

        case 4:
    // Remove Product [Admin]
    {
        char productToRemove[100];
        printf("Enter the name of the product to remove: ");
        scanf("%s", productToRemove);

        // Search for the product to remove
        struct TreeNode* productNode = searchProduct(loadedRoot, productToRemove);

        if (productNode != NULL) {
            // Product found, remove it from the BST
            loadedRoot = deleteNode(loadedRoot, productToRemove);
            printf("Product '%s' has been successfully removed.\n", productToRemove);

            // Save the updated product list to the file
            saveProductsToFile(loadedRoot, "products.txt");
        } else {
            printf("Product '%s' not found.\n", productToRemove);
        }
        break;
    }
      case 5:
        // filtering functionality
{

    bool innerExit = false;
         while (!innerExit)
            {
            	 printf("+-------------------------------------------------------+\n");
                   printf("1 : Show highest to lowest price\n");
                   printf("2 : Show lowest to highest price\n");
                   printf("3 : Back to main menu\n");
                    printf("+-------------------------------------------------------+\n");
                   printf("\nEnter your choice: ");


                  int filterChoice;
                  scanf("%d", &filterChoice);
                switch (filterChoice) {

             case 1:
            {

    struct MaxHeap* heap = buildMaxHeapFromFile("products.txt");

    if (heap != NULL) {
        printf("Products from highest to lowest price:\n");
        printMaxHeap(heap);

        // Free allocated memory
        free(heap->array);
        free(heap);
    }
                break;
            }

            case 2:
            {

        struct MinHeap* heap = buildMinHeapFromFile("products.txt");

    if (heap != NULL) {
        printf("Products from lowest to highest price:\n");
        printMinHeap(heap);

        // Free allocated memory
        free(heap->array);
        free(heap);
    }
                break;
            }

            case 3:
            {
               innerExit = true;
               break;
            }
            default:
            printf("Invalid choice\n");
                }
            }
               break;
}
      case 6:
        return 0;
      default:
        printf("Invalid choice\n");
    }
  }
}
