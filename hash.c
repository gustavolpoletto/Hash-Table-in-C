#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"

int hash(char* name, int size)
{
    int hashValue = 0, len = strlen(name);
    for (int i = 0; i < len; i++)
    {
        hashValue += name[i];
        hashValue = hashValue * name[i] % size;
    }
    return hashValue;
}

hash_table* create_table(int size)
{
    hash_table* table = malloc(sizeof(hash_table));
    if(!table)
    {
        printf("Error to allocate table\n");
        return NULL;
    }
    table->size = size;
    table->count = 0;
    table->buckets = calloc(size, sizeof(node*));
    if(!(table->buckets))
    {
        printf("Error to allocate buckets\n");
        free(table);
        return NULL;
    }

    return table;
}

void insert(hash_table* table)
{
    if(!table)
    {
        printf("table do not exists\n");
        return;
    }

    person* p = malloc(sizeof(person));
    if(!p)
    {
        printf("error to allocate person\n");
        return;
    }
    getchar();
    printf("Enter Name: ");
    fgets(p->name, 50, stdin);
    p->name[strcspn(p->name, "\n")] = 0;

    printf("Enter Age: ");
    scanf("%i", &(p->age));
    getchar();

    int index = hash(p->name, table->size);

    if(search_person(table, p->name))
    {
        printf("Already Exists Someone With this name\n");
        free(p);
        return;
    }

    node* pnode = malloc(sizeof(node));
    if(!pnode)
    {
        printf("error to allocate bucket\n");
        free(p);
        return;
    }
    pnode->p = p;
    pnode->next = table->buckets[index];
    table->buckets[index] = pnode;
    
    table->count++;
}

person* search_person (hash_table* table, char* name)
 {
    if(!table)
    {
        printf("table do not exists\n");
        return NULL;
    }

    node* ptr = table->buckets[hash(name, table->size)];

    
    while(ptr != NULL)
    {
        if(strcmp(name, ptr->p->name) == 0)
            {
                return ptr->p;
            }
        ptr = ptr->next;
    }

    return NULL;
}

void print_person(hash_table* table)
{

    getchar();
    char name[50];
    printf("Enter Name: ");
    fgets(name, 50, stdin);
    name[strcspn(name, "\n")] = 0;
    person* p = search_person(table, name);

    if(!p)
    {
        printf("Person not Found\n");
        return;
    }
    printf("Name: %s\n", p->name);
    printf("Age: %i\n", p->age);

}

void remove_person (hash_table* table)
{
    getchar();
    char name[50];
    printf("Enter the Name of the person to remove: ");
    fgets(name, 50, stdin);
    name[strcspn(name, "\n")] = 0;

    int index = hash(name, table->size);
    node* ptr = table->buckets[index];
    node* temp;

    while(ptr)
    {
        if (strcmp(ptr->p->name, name) == 0)
        {
            if(temp)
                temp->next = ptr->next;
            else
                table->buckets[index] = ptr->next;
            
            free(ptr->p);
            free(ptr);
            table->count--;
            return;
        }
        temp = ptr;
        ptr = ptr->next;
    }

    printf("person not found\n");
}

void print_table (hash_table *table)
{
    for (int i = 0; i < table->size; i++)
    {
        node* temp =  table->buckets[i];
        
        while(temp)
        {
            printf("[%d]- %s - %i\n",i, temp->p->name, temp->p->age);
            temp = temp->next;
        }
    }
}

void free_table (hash_table * table)
{
    for (int i = 0; i < table->size; i++)
    {
        node* temp =  table->buckets[i];
        
        while(temp)
        {
            node* ptr =  temp->next;
            free(temp->p);
            free(temp);
            temp = temp->next;
        }
        
    }
    free(table->buckets);
    free(table);
}

void print_menu()
{
    printf("\tOPTION\n[1]Insert Person\n[2]Remove Person\n[3]Print Person\n[4]Print Full Hash Table\n[5]Save Hash Table to CSV file\n[6]Exit\n - ");
}

void save_to_csv (hash_table* table)
{
    if(!table)
    {
    printf("Table does not exist\n");
    return;
    }

    FILE* fp = fopen("hash_table.csv", "w");
    if(!fp)
    {
        printf("fail to open file\n");
        return;
    }

    fprintf(fp, "Hash Index,Name,Age\n");
    for (int i = 0; i < table->size; i++)
    {
        node* temp =  table->buckets[i];

        while(temp)
        {
            fprintf(fp, "%d,\"%s\",\"%i\"\n",i, temp->p->name, temp->p->age);
            temp = temp->next;
        }
    }
    fclose(fp);
}

hash_table* load_from_csv (char* filename)
{
    FILE* fp = fopen(filename, "r");
    if(!fp)
    {
        printf("fail to open file\n");
        return NULL;
    }

    hash_table* csvHash = create_table(MAX_SIZE);

    char line[200];
    int line_count = 0;
    fgets(line, 200, fp); //skip header

    while(fgets(line, 200, fp))
    {
        char* position = strtok(line, ",");
        char* name = strtok(NULL, ",");
        char* age = strtok(NULL, ",");

        age[strcspn(age, "\n")] = 0;

        if(!name || !age) continue;

        if(name[0] == '"')
            memmove(name, name+1, strlen(name));
        
        int len = strlen(name);
        if(len > 0 && name[len-1] == '"')
            name[len-1] = '\0';
    
        if(age[0] == '"')
            memmove(age, age+1, strlen(age));
        
        len = strlen(age);
        if(len > 0 && age[len-1] == '"')
            age[len-1] = '\0';

        csv_insert(csvHash, name, atoi(age));
    }

    fclose(fp);
    printf("%d people loaded\n", csvHash->count);
    return csvHash;
}

void csv_insert(hash_table* table, char*name, int age)
{
    if(!table)
    {
        printf("table do not exists\n");
        return;
    }

    person* p = malloc(sizeof(person));
    if(!p)
    {
        printf("error to allocate person\n");
        return;
    }
    strcpy(p->name, name);
    p->age = age;

    int index = hash(p->name, table->size);

    if(search_person(table, p->name))
    {
        printf("Already Exists Someone With this name\n");
        free(p);
        return;
    }

    node* pnode = malloc(sizeof(node));
    if(!pnode)
    {
        printf("error to allocate bucket\n");
        free(p);
        return;
    }
    pnode->p = p;
    pnode->next = table->buckets[index];
    table->buckets[index] = pnode;
    
    table->count++;
}