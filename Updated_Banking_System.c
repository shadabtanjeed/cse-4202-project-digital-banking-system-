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
} AccountInfo;

int loginverify(char *userid, char *pass);
void createaccount();
void mainmenu(char *usernm);
int ReadAccountInfo();
void ViewAccounts(char *usrnm, int sumaccounts);
void SearchAndPrint(char *username, int count, AccountInfo *account_info);
int MatchAndShow(char *usname, struct AccountInfo *Accounts, int counter, int *matchingAccounts);
void Balance(char *username, int count, AccountInfo *account_info);

int main()
{
    int flag, signinoption = 0;
    char user_name[20], password[30];
    printf("\nWelcome to My Bank\n\n");
    while (1)
    {
        printf("1. Login\n");
        printf("2. Create a new account\n");
        printf("3. Exit\n\n");
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
    AccountInfo *account_info = (AccountInfo *)malloc(MAX_ACCOUNTS * sizeof(AccountInfo));
    while (menuchoice != 12)
    {
        int count = 0;

        FILE *fp;

        fp = fopen(ACCOUNT_DATA, "r");
        if (fp == NULL)
        {
            printf("Error: Could not open file\n");
        }

        while (fscanf(fp, "Name: %[^\n] \nAccount Type: %[^\n] \nAccount No: %lld\nBalance: %lld\nPhone: %lld\nNID No: %lld\nUsername: %s\n", account_info[count].Name, account_info[count].AccountType, &account_info[count].AccountNo, &account_info[count].Balance, &account_info[count].Phone, &account_info[count].NID, account_info[count].Username) == 7)
        {
            ++count;
        }

        fclose(fp);

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
            SearchAndPrint(usernm, count, account_info);
            break;

        case 2:
            Balance(usernm, count, account_info);
            break;

        case 12:
            free(account_info);
            return;
            break;

        default:
            printf("Please enter a valid choice \n \n");
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
    // here unique account number is required. or it can be generated automatically
    printf("Enter your Account No: ");
    printf("Enter your Balance: ");
    printf("Enter your Phone No: ");
    // this here also random unique number can be generated
    printf("Enter your NID No: ");
    printf("\n");
    printf("Account Created Successfully\n");
    printf("\n");
}

void SearchAndPrint(char *username, int count, AccountInfo *account_info)
{
    int found = 0, ac_count = 1;

    for (int i = 0; i < count; ++i)
    {
        if (strcmp(username, account_info[i].Username) == 0)
        {
            printf("Account %d: \n", ac_count);
            printf("Name: %s\nAccount Type: %s\nAccount No: %lld\n \n", account_info[i].Name, account_info[i].AccountType, account_info[i].AccountNo);
            found = 1;
            ++ac_count;
        }
    }

    if (!found)
    {
        printf("No matching accounts found\n");
    }
}

int MatchAndShow(char *usname, struct AccountInfo *Accounts, int counter, int *matchingAccounts)
{
    int numMatchingAccounts = 0;
    if (matchingAccounts == NULL)
    {
        printf("Error: Could not allocate memory\n");
        return 0;
    }

    for (int i = 0; i < counter; i++)
    {
        if (strcmp(Accounts[i].Username, usname) == 0)
        {
            matchingAccounts[numMatchingAccounts] = i;
            numMatchingAccounts++;
        }
    }

    printf("Accounts for %s:\n\n", usname);
    for (int i = 0; i < numMatchingAccounts; i++)
    {
        int accountIndex = matchingAccounts[i];
        printf("%d. Account No: %lld\n   Account Type: %s \n\n", i + 1, Accounts[accountIndex].AccountNo, Accounts[accountIndex].AccountType);
    }
    printf("\n");

    return numMatchingAccounts;
}

void Balance(char *username, int count, AccountInfo *account_info)
{
    int *matchingAccounts = malloc(count * sizeof(int));
    int BalanceChoice;
    int found = MatchAndShow(username, account_info, count, matchingAccounts);
    if (found == 0)
        printf("No accounts found \n\n");
    else
    {
        printf("Choose the corresponding Account (1/2/3...): ");
        scanf("%d", &BalanceChoice);
        int index = matchingAccounts[BalanceChoice - 1];
        printf("\nCurrent Balance: Tk. %lld only", account_info[index].Balance);
        printf("\n \n");
    }

    free(matchingAccounts);
}
