#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define USER_PASS "./username.txt"
#define ACCOUNT_DATA "./AccountInfo.txt"

typedef struct _accountInfo
{
    char Name[30];
    char AccountType[20];
    int AccountNo;
    int Balance;
    int Phone;
    int NID;
    char Username[30];
} AccountInfo;

int loginverify(char *userid, char *pass);
void createaccount();
void mainmenu(char *usernm);
int ReadAccountInfo();
void ViewAccounts(char *usrnm, int sumaccounts);
char SearchAndPrint(char *username);

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
                scanf("%s", user_name);
                printf("Enter Password: ");
                scanf("%s", password);
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

    while (menuchoice != 0)
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
            SearchAndPrint(usernm);
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

char SearchAndPrint(char *username)
{
    int numAccounts = 0;
    FILE *fp;
    fp = fopen(ACCOUNT_DATA, "r");
    AccountInfo accounts[100];

    if (fp == NULL)
    {
        printf("Error: Could not open file\n");
    }

    else
    {
        while (fscanf(fp, "Name: %s\nAccount Type: %s\nAccountNo: %d\nBalance: %d\nPhone: %d\nNID no: %d\nUsername: %s\n", accounts[numAccounts].Name, accounts[numAccounts].AccountType, &accounts[numAccounts].AccountNo, &accounts[numAccounts].Balance, &accounts[numAccounts].Phone, &accounts[numAccounts].NID, accounts[numAccounts].Username == 7))
        {
            ++numAccounts;
        }
    }

    int matchingAccounts[100];
    int numMatchingAccounts = 0;
    for (int i = 0; i < numAccounts; ++i)
    {
        if (strcmp(accounts[i].Username, username) == 0)
        {
            matchingAccounts[numMatchingAccounts] = i;
            numMatchingAccounts++;
        }
    }

    printf("All accounts of the user: \n");
    for (int i = 0; i < numMatchingAccounts; i++)
    {
        int accountIndex = matchingAccounts[i];
        printf("%d. %d (%s)\n", i + 1, accounts[accountIndex].AccountNo, accounts[accountIndex].AccountType);
    }
    fclose(fp);
}
