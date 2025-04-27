#ifndef MODBUS_RTU_H
#define MODBUS_RTU_H

// Inicializa conexão Modbus
int modbus_rtu_init(const char *device, int baud);

// Abre uma porta (1 ou 2)
int modbus_rtu_open_door(int door_id);

// Fecha conexão Modbus
void modbus_rtu_close();

#endif // MODBUS_RTU_H
