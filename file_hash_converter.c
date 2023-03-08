#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

#define MAX_USERNAME_LENGTH 100
#define MAX_PASSWORD_LENGTH 100
#define HASH_LENGTH 65

typedef struct Entry
{
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
} Entry;

void hash_password(char *password, char *hash)
{
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256(password, strlen(password), digest);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(&hash[i * 2], "%02x", (unsigned int)digest[i]);
    }
    hash[HASH_LENGTH - 1] = '\0';
}

int main()
{
    FILE *fp = fopen("./username.txt", "r");
    if (fp == NULL)
    {
        perror("username.txt");
        exit(EXIT_FAILURE);
    }

    Entry entries[30];
    int num_entries = 0;

    char line[MAX_USERNAME_LENGTH + MAX_PASSWORD_LENGTH + 20];
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (strstr(line, "Username: ") == line)
        {
            strcpy(entries[num_entries].username, line + strlen("Username: "));
        }
        else if (strstr(line, "Password: ") == line)
        {
            strcpy(entries[num_entries].password, line + strlen("Password: "));
            entries[num_entries].password[strcspn(entries[num_entries].password, "\r\n")] = '\0';
            num_entries++;
        }
    }

    fclose(fp);

    for (int i = 0; i < num_entries; i++)
    {
        char hash[HASH_LENGTH];
        hash_password(entries[i].password, hash);
        printf("%s: %s\n", entries[i].username, hash);
    }

    return 0;
}
