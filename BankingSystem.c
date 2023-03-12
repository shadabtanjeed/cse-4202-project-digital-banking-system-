#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define USER_PASS "./username.txt"
#define ACCOUNT_DATA "./AccountInfo.txt"

typedef struct
{
    char Name[30];
    char AccountType[20];
    long long AccountNo;
    long long Balance;
    long long Phone;
    long long NID;
    char Username[30];
} AccountInfo;

int loginverify(char *userid, char *pass);
void createaccount();
void mainmenu(char *usernm);
int ReadAccountInfo();
void ViewAccounts(char *usrnm, int sumaccounts);
void SearchAndPrint(char *username);

int main()
{
    int flag, signinoption;
    char user_name[20], password[30];
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
                scanf("%19s", user_name);
                printf("Enter Password: ");
                scanf("%29s", password);
                flag = loginverify(user_name, password);

                if (flag == 1)
                {
                    printf("\nLogin successful. \n");
                    printf("\n");
                    mainmenu(user_name);
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

void mainmenu(char *usernm)
{
    int menuchoice = 100;
    char usrnm[30];
    strcpy(usrnm, usernm);
    while (menuchoice != 12)
    {

        printf("1. View All Accounts\n");
        printf("2. Check Balance\n");
        printf("3. Cash Deposit\n");
        printf("4. Cash Withdraw\n");
        printf("5. Fund Transfer\n");
        printf("6. Benificiary Management\n");
        printf("7. Mini Statement\n");
        printf("8. Account Settings\n");
        printf("9. Close Account\n");
        printf("10. ATM/Branch Locations\n");
        printf("11. Customer Support\n");
        printf("12. Exit\n");

        printf("\n");
        printf("Choose your option: ");
        scanf("%d", &menuchoice);
        printf("\n");

        if (menuchoice == 1)
            SearchAndPrint(usrnm);
    }
}

int loginverify(char *userid, char *pass)
{
    FILE *fp;
    char user[20], password[30];
    int found = 0;

    fp = fopen(USER_PASS, "r");
    if (fp == NULL)
    {
        printf("Error: Could not open file\n");
        return 0;
    }

    while (fscanf(fp, "Username: %s\nPassword: %s\n", user, password) == 2)
    {
        if (strcmp(userid, user) == 0 && strcmp(pass, password) == 0)
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
    // a function can be added to check uniquenness
    printf("Enter Username: ");
    scanf("%s", username);
    printf("Enter Password: ");
    scanf("%s", password);
    printf("\n");
    printf("Enter your Account details: \n");
    printf("Enter your Name: ");
    printf("Enter your Account Type (Savings/Current/Fixed Deposit): ");
    // here unique account number is requires. or it can be generated automatically
    printf("Enter your Account No: ");
    printf("Enter your Balance: ");
    printf("Enter your Phone No: ");
    printf("Enter your NID No: ");
    printf("\n");
    printf("Account Created Successfully\n");
    printf("\n");
}

void SearchAndPrint(char *username)
{
    printf("User Name: %s\n", username);
    FILE *fp;
    AccountInfo acc;
    int found = 0;

    fp = fopen(ACCOUNT_DATA, "r");
    if (fp == NULL)
    {
        printf("Error: Could not open file\n");
        return;
    }

    while (fscanf(fp, "Name: %[^\n] \nAccount Type: %s \nAccount No: %lld \nBalance: %lld \nPhone: %lld \nNID No: %lld \nUsername: %s \n", acc.Name, acc.AccountType, &acc.AccountNo, &acc.Balance, &acc.Phone, &acc.NID, acc.Username) == 7)
    {
        if (strcmp(username, acc.Username) == 0)
        {
            printf("Name: %s\nAccount Type: %s\nAccount No: %lld\nBalance: %lld\nPhone: %lld\nNID No: %lld\n\n", acc.Name, acc.AccountType, acc.AccountNo, acc.Balance, acc.Phone, acc.NID);
            found = 1;
        }
    }

    if (!found)
    {
        printf("No matching account found.\n");
    }

    fclose(fp);
}
