# Room Access Control

Este projeto é um sistema de controle de acesso para gerenciar o acesso de usuários a salas de uma empresa. Ele utiliza **C**, **SQLite** para o banco de dados e **Modbus RTU** para comunicação com dispositivos externos.

## Funcionalidades

- **Cadastro de Usuários**:
  - Nome, senha e tipo de usuário (administrador ou comum).
- **Autenticação**:
  - Validação de credenciais para acesso ao sistema.
- **Controle de Portas**:
  - Liberação de portas (1 e 2) após autenticação.
  - Registro de eventos de acesso no banco de dados.
- **Listagem**:
  - Usuários cadastrados.
  - Eventos de acesso (restrito a administradores).
- **Comunicação via Modbus RTU**:
  - Gerenciamento de portas utilizando protocolo Modbus.
- **Menu Interativo**:
  - Interface via terminal para interação com o sistema.

## Estrutura do Projeto

```plaintext
room-access-control/
├── src/
│   ├── access_control/
│   │   ├── [access_manager.c](http://_vscodecontentref_/1)
│   │   └── access_manager.h
│   ├── database/
│   │   ├── db_manager.c
│   │   └── db_manager.h
│   ├── menu/
│   │   ├── user_menu.c
│   │   └── user_menu.h
│   ├── modbus/
│   │   ├── modbus_rtu.c
│   │   └── modbus_rtu.h
│   └── main.c
├── include/
│   └── config.h
├── Makefile
└── [README.md](http://_vscodecontentref_/2)
