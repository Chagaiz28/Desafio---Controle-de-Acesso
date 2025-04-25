#include "user_menu.h"
#include "../database/db_manager.h"
#include <stdio.h>
#include <string.h>

// Buffer de entrada
#define INPUT_SIZE 64

// Função auxiliar para ler uma linha do terminal
static void read_line(char *buffer, size_t size) {
    if (fgets(buffer, size, stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0'; // remove \n
    }
}

// Função auxiliar para autenticar um usuário
static int authenticate_user(char *username, int *is_admin) {
    char input_name[INPUT_SIZE];
    char input_password[INPUT_SIZE];

    printf("Digite seu nome de usuário: ");
    read_line(input_name, sizeof(input_name));

    printf("Digite sua senha: ");
    read_line(input_password, sizeof(input_password));

    // Aqui futuramente deveríamos validar senha (ex: com hash)
    // Por enquanto, buscamos no banco de dados
    const char *sql = "SELECT password, is_admin FROM users WHERE name = ?";
    sqlite3_stmt *stmt;
    extern sqlite3 *db;  // acesso direto para consulta simples
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        printf("Erro na autenticação.\n");
        return 0;
    }
    sqlite3_bind_text(stmt, 1, input_name, -1, SQLITE_STATIC);

    int authenticated = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *db_password = (const char *)sqlite3_column_text(stmt, 0);
        int db_is_admin = sqlite3_column_int(stmt, 1);

        if (strcmp(db_password, input_password) == 0) {
            authenticated = 1;
            *is_admin = db_is_admin;
            strcpy(username, input_name); // retorna o nome validado
        }
    }
    sqlite3_finalize(stmt);
    return authenticated;
}

void user_menu_start() {
    int option;
    char username[INPUT_SIZE];
    int is_admin;

    do {
        printf("\n=== Gerenciamento de Acesso ===\n");
        printf("1. Cadastrar Usuário\n");
        printf("2. Listar Usuários\n");
        printf("3. Listar Eventos (Admin)\n");
        printf("4. Liberar Porta 1\n");
        printf("5. Liberar Porta 2\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &option);
        getchar(); // limpar o \n deixado pelo scanf

        switch (option) {
            case 1: {
                char name[INPUT_SIZE];
                char password[INPUT_SIZE];
                int admin_flag;

                printf("Nome do novo usuário: ");
                read_line(name, sizeof(name));

                printf("Senha: ");
                read_line(password, sizeof(password));

                printf("É administrador? (1=Sim, 0=Não): ");
                scanf("%d", &admin_flag);
                getchar();

                if (db_insert_user(name, password, admin_flag) == 0) {
                    printf("Usuário cadastrado com sucesso!\n");
                } else {
                    printf("Erro ao cadastrar usuário.\n");
                }
                break;
            }

            case 2:
                db_list_users();
                break;

            case 3: {
                printf("Autenticação para listar eventos:\n");
                if (authenticate_user(username, &is_admin)) {
                    if (is_admin) {
                        db_list_events();
                    } else {
                        printf("Apenas administradores podem visualizar eventos!\n");
                    }
                } else {
                    printf("Falha na autenticação.\n");
                }
                break;
            }

            case 4: case 5: {
                printf("Autenticação para liberar porta:\n");
                if (authenticate_user(username, &is_admin)) {
                    int door_id = (option == 4) ? 1 : 2;
                    // Aqui futuramente chamaremos também liberação via Modbus
                    printf("Liberando porta %d...\n", door_id);
                    db_log_event(username, door_id);
                    printf("Acesso registrado!\n");
                } else {
                    printf("Falha na autenticação.\n");
                }
                break;
            }

            case 0:
                printf("Encerrando o sistema.\n");
                break;

            default:
                printf("Opção inválida.\n");
        }
    } while (option != 0);
}
