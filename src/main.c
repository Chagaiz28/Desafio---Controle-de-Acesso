#include "user_menu.h"
#include "db_manager.h"
#include "modbus_rtu.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s /dev/pts/X\n", argv[0]);
        return 1;
    }

    // Inicializa o banco de dados
    if (db_init("access_control.db") != 0) {
        fprintf(stderr, "Falha ao inicializar o banco de dados.\n");
        return 1;
    }

    // Inicializa conexão Modbus
    if (!modbus_rtu_init(argv[1], 9600)) {
        fprintf(stderr, "Falha ao inicializar conexão Modbus.\n");
    }

    // Inicia o menu principal
    user_menu_start();

    modbus_rtu_close();

    return 0;
}
