#include "access_manager.h"
#include "db_manager.h"
#include <stdio.h>
#include <string.h>
#include "crypto.h"

#define INPUT_SIZE 64

// Função auxiliar para ler uma linha do terminal
static void read_line(char *buffer, size_t size) {
    if (fgets(buffer, size, stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

// Autentica usuário e retorna nome e se é admin
int authenticate_user(char *out_username, int *out_is_admin) {
    char input_name[INPUT_SIZE];
    char input_password[INPUT_SIZE];

    printf("Usuário: ");
    read_line(input_name, sizeof(input_name));

    printf("Senha: ");
    read_line(input_password, sizeof(input_password));

    // Gera o hash da senha digitada
    char hashed_input[65];
    sha256_hash(input_password, hashed_input);

    // Consulta no banco de dados
    const char *sql = "SELECT password, is_admin FROM users WHERE name = ?";
    sqlite3_stmt *stmt;
    extern sqlite3 *db;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        printf("Erro na autenticação.\n");
        return 0;
    }

    sqlite3_bind_text(stmt, 1, input_name, -1, SQLITE_STATIC);

    int result = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *db_password = (const char *)sqlite3_column_text(stmt, 0);
        int db_is_admin = sqlite3_column_int(stmt, 1);

        if (strcmp(hashed_input, db_password) == 0) {
            *out_is_admin = db_is_admin;
            strcpy(out_username, input_name);
            result = 1;
        }
    }

    sqlite3_finalize(stmt);
    return result;
}

int request_door_access(int door_id) {
    char username[INPUT_SIZE];
    int is_admin;

    printf("== Autenticação necessária para liberar porta %d ==\n", door_id);

    if (authenticate_user(username, &is_admin)) {
        printf("Acesso autorizado. Liberando porta %d...\n", door_id);
        // Aqui no futuro: chamar modbus para abrir a porta
        db_log_event(username, door_id);
        printf("Evento registrado!\n");
        return 1;
    } else {
        printf("Acesso negado. Usuário ou senha inválidos.\n");
        return 0;
    }
}

int request_events_access();
