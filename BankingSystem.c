/*
__________IMPORTANT NOTES__________
For inputting Name manually put a space after the last name
For inputting Fixed Deposit as Account type, put a space at last
*/

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define USER_PASS "./username.txt"
#define ACCOUNT_DATA "./AccountInfo.txt"
#define MAX_ACCOUNTS 100

typedef struct AccountInfo
{
    char Name[30];
    char AccountType[20];
    long long AccountNo;
    long long Balance;
    long long Phone;
    long long NID;
    char Username[30];
};

int loginverify(char *userid, char *pass);
void createaccount();
void mainmenu(char *usernm);
int ReadAccountInfo();
void ViewAccounts(char *usrnm, int sumaccounts);
void SearchAndPrint(char *username);
void MatchAndShow(char *usname, struct AccountInfo Accounts[MAX_ACCOUNTS], int *mathingAccounts);
void Balance(char *username);

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

        switch (menuchoice)
        {
        case 1:
            SearchAndPrint(usrnm);
            break;

        case 2:
            Balance(usernm);
            break;

        default:
            break;
        }
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
    FILE *fp;
    struct AccountInfo acc;
    int found = 0, ac_count = 1;

    fp = fopen(ACCOUNT_DATA, "r");
    if (fp == NULL)
    {
        printf("Error: Could not open file\n");
        return;
    }

    while (fscanf(fp, "Name: %[^\n] \nAccount Type: %[^\n] \nAccount No: %lld\nBalance: %lld\nPhone: %lld\nNID No: %lld\nUsername: %s\n", acc.Name, acc.AccountType, &acc.AccountNo, &acc.Balance, &acc.Phone, &acc.NID, acc.Username) == 7)
    {
        if (strcmp(username, acc.Username) == 0)
        {
            printf("Account %d: \n", ac_count);
            printf("Name: %s\nAccount Type: %s\nAccount No: %lld\nBalance: %lld\n \n", acc.Name, acc.AccountType, acc.AccountNo, acc.Balance);
            found = 1;
            ++ac_count;
        }
    }

    fclose(fp);

    if (!found)
    {
        printf("No matching accounts found\n");
    }
}

void MatchAndShow(char *usname, struct AccountInfo account[MAX_ACCOUNTS], int *matchingAccounts)
{
    FILE *fp;
    fp = fopen(ACCOUNT_DATA, "r");
    if (fp == NULL)
    {
        printf("Error: Could not open file\n");
        return NULL;
    }

    int numAccounts = 0;
    while (fscanf(fp, "Name: %[^\n] \nAccount Type: %[^\n] \nAccount No: %lld\nBalance: %lld\nPhone: %lld\nNID No: %lld\nUsername: %s\n", account[numAccounts].Name, account[numAccounts].AccountType, &account[numAccounts].AccountNo, &account[numAccounts].Balance, &account[numAccounts].Phone, &account[numAccounts].NID, account[numAccounts].Username) == 7)
    {
        numAccounts++;
    }

    int numMatchingAccounts = 0;
    for (int i = 0; i < numAccounts; i++)
    {
        if (strcmp(account[i].Username, usname) == 0)
        {
            matchingAccounts[numMatchingAccounts] = i;
            numMatchingAccounts++;
        }
    }

    fclose(fp);

    printf("Accounts for username %s:\n \n", usname);
    for (int i = 0; i < numMatchingAccounts; i++)
    {
        int accountIndex = matchingAccounts[i];
        printf("%d. Account No: %lld\n   Account Type: %s \n", i + 1, account[accountIndex].AccountNo, account[accountIndex].AccountType);
        printf("\n");
    }
    printf("\n");
    return NULL;
}

void Balance(char *username)
{
    struct AccountInfo account[MAX_ACCOUNTS];
    int matchingAccounts[20];
    int BalanceChoice;
    MatchAndShow(username, account, matchingAccounts);
    printf("Choose the corresponding Account (1/2/3...): ");
    scanf("%d", &BalanceChoice);
    int index = matchingAccounts[BalanceChoice - 1];
    printf("\nCurrent Balance: Tk. %lld", account[index].Balance);
    printf("\n \n");
}