#include <stdio.h>
#include <stdlib.h>

#include "hash.h"

int main()
{
    hash_table* HASH_TABLE =NULL;

    int option;
    do{
        printf("[1]Create new Hash Table\n[2]Open from CSV file\n - ");
        if(scanf("%d", &option) != 1)
        {
            printf("Invalid Input");
            exit(1);
        }

        
        switch (option)
        {
            case 1:
            HASH_TABLE = create_table(MAX_SIZE);
            if (!HASH_TABLE)
            {
                return 2;
            }
            break;
            
            case 2:
            char filename[50];
            printf("Enter filename: ");
            scanf("%s", filename);

            HASH_TABLE = load_from_csv(filename);
            if (!HASH_TABLE)
            {
                return 2;
            }
            break;
            
            default:
            printf("Invalid option\n");
            break;
        }
    }while(option != 1 && option !=2);
    
    
    do{
        print_menu();
        if(scanf("%d", &option) != 1)
        {   
            printf("Invalid input\n");
            exit(1);
        }

        switch (option)
        {
            case 1:
            insert(HASH_TABLE);
            break;
            
            case 2:
            remove_person(HASH_TABLE);
            break;

            case 3:
            print_person(HASH_TABLE);
            break;

            case 4:
            print_table(HASH_TABLE);
            break;

            case 5:
            save_to_csv(HASH_TABLE);
            break;

            case 6:
            printf("Exiting...\n");
            break;

            default:
            printf("Invalid option\n");
        }
    }while(option != 6);

    free_table(HASH_TABLE);

}