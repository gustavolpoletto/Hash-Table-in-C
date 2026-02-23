typedef struct person{
    char name[50];
    int age;
}person;

typedef struct node{
    person* p;
    struct node* next;
}node;

typedef struct hash{
    node** buckets;
    int size;
    int count;
}hash_table;

#define MAX_SIZE 101

int hash(char* name, int size);
hash_table* create_table(int size);
void insert(hash_table* table);
 person* search_person (hash_table* table, char* name);
 void print_person(hash_table* table);
 void remove_person (hash_table* table);
 void print_table (hash_table *table);
void free_table (hash_table * table);
void print_menu();
void save_to_csv (hash_table* table);
hash_table* load_from_csv (char* filename);
void csv_insert(hash_table* table, char*name, int age);