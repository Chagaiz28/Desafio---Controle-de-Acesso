#ifndef ACCESS_MANAGER_H
#define ACCESS_MANAGER_H

// Autentica usuário: retorna 1 se válido, 0 se falhar.
// Preenche username e is_admin.
int authenticate_user(char *out_username, int *out_is_admin);

// Libera uma porta (1 ou 2) se autenticação for bem-sucedida.
// Registra evento no banco.
int request_door_access(int door_id);

// Lista eventos somente se o usuário for admin.
int request_events_access();

#endif // ACCESS_MANAGER_H
