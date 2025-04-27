#!/bin/bash

# Mata socat antigos se existir
pkill -f "socat.*pty,raw"

echo "Iniciando socat..."
# Inicia socat em background, redirecionando logs
socat -d -d pty,raw,echo=0 pty,raw,echo=0 2> socat.log &

# Espera um pouco pra socat inicializar
sleep 1

# Pega os nomes das portas criadas
DEVICE1=$(grep -m 1 "PTY is" socat.log | awk '{print $7}')
DEVICE2=$(grep -m 2 "PTY is" socat.log | tail -n 1 | awk '{print $7}')

echo "Dispositivos criados:"
echo "  Escravo (Python): $DEVICE1"
echo "  Mestre (C): $DEVICE2"

# Roda o Python simulador
echo "Iniciando Modbus Slave (Python)..."
python3 simulator/modbus_slave.py "$DEVICE1" &

# Espera o Python inicializar
sleep 1

# Compila e executa seu projeto C apontando para DEVICE2
echo "Compilando e rodando o programa C..."
make clean
make
./bin/room_access_control "$DEVICE2"
