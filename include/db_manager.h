#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include <sqlite3.h>

// Estrutura para representar um usuário
typedef struct {
    int id;
    char name[50];
    char password[50];
    int is_admin; // 1 para administrador, 0 para usuário comum
} User;

// Inicializa o banco de dados
int db_init(const char *db_path);

// Insere um novo usuário no banco de dados
int db_insert_user(const char *name, const char *password, int is_admin);

// Lista todos os usuários cadastrados
int db_list_users();

// Registra um evento de acesso
int db_log_event(const char *user_name, int door_id);

// Lista todos os eventos registrados
int db_list_events();

extern sqlite3 *db; // Variável global para o banco de dados

#endif // DB_MANAGER_H