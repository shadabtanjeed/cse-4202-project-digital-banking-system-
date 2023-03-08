#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define USER_PASS "./username.txt"

int loginverify(char *userid, char *pass);
void createaccount();
void mainmenu();

int main()
{
    int flag, signinoption;
    char username[20], password[30];
    printf("Welcome to My Bank\n");
    while (1)
    {
        printf("1. Login\n");
        printf("2. Create a new account\n");
        printf("3. Exit\n");
        printf("Choose your option: ");
        scanf("%d", &signinoption);
        printf("\n");
        switch (signinoption)
        {
        case 1:
            while (1)
            {
                printf("Enter Username: ");
                scanf("%s", username);
                printf("Enter Password: ");
                scanf("%s", password);
                flag = loginverify(username, password);

                if (flag == 1)
                {
                    printf("\nLogin successful. \n");
                    printf("\n");
                    {
                        mainmenu();
                    }
                    break;
                }
                else
                {
                    printf("Login Failed. Try again. \n");
                    printf("\n");
                }
            }
            break;
        case 2:
            createaccount();
            break;
        case 3:
            return 0;
            break;
        }
        if (signinoption == 1)
            break;
    }
    return 0;
}

void mainmenu()
{
    printf("1. Check Balance\n");
    printf("2. Cash Deposit\n");
    printf("3. Cash Withdraw\n");
    printf("4. Fund Transfer\n");
    printf("5. Benificiary Management\n");
    printf("6. Mini Statement\n");
    printf("7. Account Settings\n");
    printf("8. Close Account\n");
    printf("9. ATM/Branch Locations\n");
    printf("10. Customer Support\n");
    printf("11. open account\n");
}

int loginverify(char *userid, char *pass)
{
    FILE *fp;
    char username[20], password[30];
    int found = 0;

    fp = fopen(USER_PASS, "r");
    if (fp == NULL)
    {
        printf("Error: Could not open file\n");
        return 0;
    }

    while (fscanf(fp, "Username: %s\nPassword: %s\n", username, password) == 2)
    {
        if (strcmp(userid, username) == 0 && strcmp(pass, password) == 0)
        {
            found = 1;
            break;
        }
    }

    fclose(fp);
    return found;
}

void createaccount()
{
    char username[20], password[20];
    printf("Enter your login credentials: \n");
    printf("Enter Username: ");
    scanf("%s", username);
    printf("Enter Password: ");
    scanf("%s", password);
    printf("\n");
    printf("Enter your Account details: \n");
    printf("Account Created Successfully\n");
    printf("\n");
}