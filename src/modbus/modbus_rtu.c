#include <modbus.h>
#include <errno.h> 
#include "modbus_rtu.h"
#include <stdio.h>
#include <errno.h>

static modbus_t *ctx = NULL;

int modbus_rtu_init(const char *device, int baud) {
    ctx = modbus_new_rtu(device, baud, 'N', 8, 1);
    if (!ctx) {
        fprintf(stderr, "Erro ao criar contexto Modbus RTU\n");
        return 0;
    }

    // Endereço do escravo (padrão 1)
    modbus_set_slave(ctx, 1);

    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Erro ao conectar Modbus: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return 0;
    }

    return 1;
}

int modbus_rtu_open_door(int door_id) {
    if (!ctx) return 0;

    int address;
    if (door_id == 1) address = 0x34;
    else if (door_id == 2) address = 0x35;
    else return 0;

    uint16_t value = 0xFF; // Valor para liberar porta

    int rc = modbus_write_register(ctx, address, value);
    if (rc == -1) {
        printf("Erro ao enviar comando Modbus: %s\n", modbus_strerror(errno));
        return 0;
    }

    return 1;
}

void modbus_rtu_close() {
    if (ctx) {
        modbus_close(ctx);
        modbus_free(ctx);
        ctx = NULL;
    }
}
