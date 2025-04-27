#ifndef CRYPTO_H
#define CRYPTO_H

// Calcula o hash SHA-256 de uma string de entrada.
// 'output' precisa ter espaço para pelo menos 65 caracteres (64 do hash + '\0')
void sha256_hash(const char *input, char output[65]);

#endif // CRYPTO_H
