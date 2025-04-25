#include "user_menu.h"
#include "../database/db_manager.h"
#include "../access_control/access_manager.h"
#include <stdio.h>
#include <string.h>

#define INPUT_SIZE 64

// Função auxiliar para ler entrada
static void read_line(char *buffer, size_t size) {
    if (fgets(buffer, size, stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

void user_menu_start() {
    int option;

    do {
        printf("\n=== Room Access Control Menu ===\n");
        printf("1. Register New User\n");
        printf("2. List All Users\n");
        printf("3. List Access Events (Admin Only)\n");
        printf("4. Open Door 1\n");
        printf("5. Open Door 2\n");
        printf("0. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &option);
        getchar(); // consumir '\n' restante

        switch (option) {
            case 1: {
                char name[INPUT_SIZE];
                char password[INPUT_SIZE];
                int is_admin;

                printf("Enter username: ");
                read_line(name, sizeof(name));

                printf("Enter password: ");
                read_line(password, sizeof(password));

                printf("Is admin? (1 = yes, 0 = no): ");
                scanf("%d", &is_admin);
                getchar();

                if (db_insert_user(name, password, is_admin) == 0) {
                    printf("User registered successfully.\n");
                } else {
                    printf("Error registering user.\n");
                }
                break;
            }

            case 2:
                db_list_users();
                break;

            case 3:
                request_events_access();
                break;

            case 4:
                request_door_access(1);
                break;

            case 5:
                request_door_access(2);
                break;

            case 0:
                printf("Exiting system...\n");
                break;

            default:
                printf("Invalid option.\n");
        }

    } while (option != 0);
}
