#include "user_menu.h"
#include "db_manager.h"
#include <stdio.h>

int main() {
    // Inicializa o banco de dados
    if (db_init("access_control.db") != 0) {
        fprintf(stderr, "Falha ao inicializar o banco de dados.\n");
        return 1;
    }

    // Inicia o menu principal
    user_menu_start();

    return 0;
}