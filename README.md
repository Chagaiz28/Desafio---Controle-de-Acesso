# Room Access Control System - Embedded Linux

A complete and modular room access control system with password authentication, permission control, event logging, simulated Modbus RTU communication, and a Flask web interface. Designed to simulate a real embedded environment, with all components professionally integrated.

## 🧠 Overview

This project was developed as part of an Embedded Linux challenge and aims to simulate a physical access control system, with administrative functionalities and hardware simulation via Modbus RTU.

## ✨ Features

- User registration (common or administrator)
- Authentication with encrypted password (SHA-256)
- Permission control (admin views events)
- Event logging with timestamp
- Communication with simulated hardware via Modbus RTU
- Web interface with Flask for data consultation
- Serial port simulation with socat

## 📃 Project Structure


## Estrutura do Projeto

```plaintext
Desafio---Controle-de-Acesso/
├── src/
│   ├── main.c                   # Ponto de entrada
│   ├── access_control/          # Liberação das portas via Modbus
│   ├── database/                # CRUD com SQLite
│   ├── logging/                 # Registro de eventos
│   ├── menu/                    # Menu interativo
│   ├── modbus/                  # Comunicação Modbus RTU
│   └── security/                # Criptografia de senhas
│
├── simulator/                   # Simulação de dispositivo escravo
│   ├── modbus_slave.py
│   └── modbus_client.py
│
├── webserver/                   # Servidor Flask
│   ├── app.py
│   ├── venv/                    # Ambiente virtual (não versionar)
│   └── templates/
│       ├── events.html
│       └── users.html
│
├── scripts/
│   └── start_simulation.sh      # Inicia socat, simulador e app C
├── Makefile
├── access_control.db
└── README.md
```
## 🔐 Password Encryption

User passwords are hashed with SHA-256 before being stored in the SQLite database. The function is located in the `src/security/crypto.c` file, using the `openssl/sha.h` library.

## 🦖 How to Run (WSL mode)

1.  Configure WSL (if you haven't already)

    ```bash
    wsl --install
    ```

    Restart and follow the Ubuntu setup.
2.  Install dependencies

    ```bash
    sudo apt update && sudo apt upgrade -y
    sudo apt install -y build-essential gcc libsqlite3-dev libmodbus-dev socat git python3 python3-pip
    pip3 install flask pymodbus
    ```
3.  Clone the repository

    ```bash
    git clone https://github.com/seu-usuario/Desafio---Controle-de-Acesso.git
    cd Desafio---Controle-de-Acesso
    ```
4.  Run the complete system

    ```bash
    chmod +x scripts/start_simulation.sh
    ./scripts/start_simulation.sh
    ```
5.  Run the web server (optional)

    ```bash
    cd webserver
    python3 -m venv venv
    source venv/bin/activate
    pip install flask
    python3 app.py
    ```

    Access in the browser:

    ```
    http://localhost:5000/events
    ```

## 🔪 Simulation with socat

We use socat to simulate two serial ports:

-   One connected to the C program (Modbus master)
-   Another connected to the Python simulator (slave)

Example:
Slave (Python): /dev/pts/10 Master (C): /dev/pts/11

## ✅ Made with

-   C (GCC + libmodbus)
-   SQLite3
-   OpenSSL (SHA-256)
-   Python (Flask + pymodbus)
-   socat
-   Bash
-   WSL + Ubuntu
