#include "db_manager.h"
#include "access_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crypto.h"

// Ponteiro para o banco de dados SQLite
sqlite3 *db = NULL;
#define INPUT_SIZE 64 // Tamanho máximo para entradas de texto

// Inicializa o banco de dados e cria as tabelas necessárias
int db_init(const char *db_path) {
    char *err_msg = NULL;

    // Abre o banco de dados
    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        fprintf(stderr, "Erro ao abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    // Cria a tabela de usuários
    const char *create_users_table = 
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT NOT NULL, "
        "password TEXT NOT NULL, "
        "is_admin INTEGER NOT NULL);";

    if (sqlite3_exec(db, create_users_table, 0, 0, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "Erro ao criar tabela de usuários: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }

    // Cria a tabela de eventos
    const char *create_events_table = 
        "CREATE TABLE IF NOT EXISTS events ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "user_name TEXT NOT NULL, "
        "door_id INTEGER NOT NULL, "
        "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);";

    if (sqlite3_exec(db, create_events_table, 0, 0, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "Erro ao criar tabela de eventos: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }

    return 0;
}

// Insere um novo usuário no banco de dados
int db_insert_user(const char *name, const char *password, int is_admin) {
    char hashed_password[65];
    sha256_hash(password, hashed_password); // gera o hash da senha

    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO users (name, password, is_admin) VALUES (?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "Erro ao preparar inserção de usuário: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, hashed_password, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, is_admin);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Erro ao inserir usuário: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }

    sqlite3_finalize(stmt);
    return 0;
}

// Lista todos os usuários cadastrados
int db_list_users() {
    const char *sql = "SELECT id, name, is_admin FROM users;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "Erro ao preparar consulta: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    printf("ID | Nome | Administrador\n");
    printf("-------------------------\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *name = (const char *)sqlite3_column_text(stmt, 1);
        int is_admin = sqlite3_column_int(stmt, 2);

        printf("%d | %s | %s\n", id, name, is_admin ? "Sim" : "Não");
    }

    sqlite3_finalize(stmt);
    return 0;
}

// Registra um evento de acesso
int db_log_event(const char *user_name, int door_id) {
    char *err_msg = NULL;
    char sql[256];

    snprintf(sql, sizeof(sql), 
        "INSERT INTO events (user_name, door_id) VALUES ('%s', %d);", 
        user_name, door_id);

    if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "Erro ao registrar evento: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }

    return 0;
}

// Lista todos os eventos registrados
int db_list_events() {
    const char *sql = "SELECT id, user_name, door_id, timestamp FROM events;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "Erro ao preparar consulta: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    printf("ID | Usuário | Porta | Timestamp\n");
    printf("--------------------------------\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *user_name = (const char *)sqlite3_column_text(stmt, 1);
        int door_id = sqlite3_column_int(stmt, 2);
        const char *timestamp = (const char *)sqlite3_column_text(stmt, 3);

        printf("%d | %s | %d | %s\n", id, user_name, door_id, timestamp);
    }


    sqlite3_finalize(stmt);
    return 0;
}

int request_events_access() {
    char username[INPUT_SIZE];
    int is_admin;

    printf("== Autenticação necessária para listar eventos ==\n");

    if (authenticate_user(username, &is_admin)) {
        if (is_admin) {
            printf("Acesso autorizado. Listando eventos...\n");
            db_list_events();
            return 1;
        } else {
            printf("Acesso negado. Apenas administradores podem listar eventos.\n");
        }
    } else {
        printf("Acesso negado. Usuário ou senha inválidos.\n");
    }

    return 0;
}