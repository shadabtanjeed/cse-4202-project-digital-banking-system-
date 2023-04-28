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

typedef struct Username
{
    char Username[30];
    char Password[30];
} Username;

int loginverify(char *userid, char *pass);
void createaccount();
void mainmenu(char *usernm);
int ReadAccountInfo();
void ViewAccounts(char *usrnm, int sumaccounts);
void SearchAndPrint(char *username, int count, AccountInfo *account_info);
int MatchAndShow(char *usname, struct AccountInfo *Accounts, int counter, int *matchingAccounts);
void Balance(char *username, int count, AccountInfo *account_info);
void CashDeposit(char *username, int count, AccountInfo *account_info);
void CashWithdrawal(char *username, int count, AccountInfo *account_info);
int CheckAccountExists(long long accountNo);
void CreateAccount2();
void CloseAccount(char *username, int count, AccountInfo *account_info, Username *user_name);

int main()
{
    int flag, signinoption = 0;
    char user_name[20], password[30];
    printf("\n\nWelcome to My Bank\n\n");
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
            // createaccount();
            CreateAccount2();
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
    Username *user_name = (Username *)malloc(MAX_ACCOUNTS * sizeof(Username));
    while (menuchoice != 12)
    {
        int count = 0;

        FILE *fp;
        FILE *fp5;

        fp = fopen(ACCOUNT_DATA, "r");
        fp5 = fopen(USER_PASS, "r");
        if (fp == NULL || fp5 == NULL)
        {
            printf("Error: Could not open file\n");
        }

        while (fscanf(fp, "Name: %[^\n] \nAccount Type: %[^\n] \nAccount No: %lld\nBalance: %lld\nPhone: %lld\nNID No: %lld\nUsername: %s\n", account_info[count].Name, account_info[count].AccountType, &account_info[count].AccountNo, &account_info[count].Balance, &account_info[count].Phone, &account_info[count].NID, account_info[count].Username) == 7)
        {
            ++count;
        }

        for(int k = 0; k < count; k++)
        {
            fscanf(fp5, "Username: %s\nPassword: %s\n", user_name[k].Username, user_name[k].Password ) == 2;
        }

        fclose(fp);
        fclose(fp5);

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

        case 3:
            CashDeposit(usernm, count, account_info);
            break;

        case 4:
            CashWithdrawal(usernm, count, account_info);
            break;

        case 5:

        case 6:

        case 7:

        case 8:

        case 9:
            CloseAccount(usernm, count, account_info, user_name);
            break;


        case 10:

        case 11:

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

void CloseAccount(char *username, int count, AccountInfo *account_info, Username *user_name)
{
    long long accountNo;
    int i, j, flag = 0;

    printf("Enter Account Number: ");
    scanf("%lld", &accountNo);

    // Check if the account number belongs to the user
    for (i = 0; i < count; i++)
    {
        if (account_info[i].AccountNo == accountNo && strcmp(account_info[i].Username, username) == 0)
        {
            flag = 1;
            break;
        }
    }

    // If account belongs to the user, remove the account information and update files
    if (flag == 1)
    {
        for (j = i; j < count - 1; j++)
        {
            account_info[j] = account_info[j + 1];
            user_name[j] = user_name[j+1];
        }

        printf("\nAccount closed successfully.\n");

        // Update account and username files
        FILE *fp;
        
        fp = fopen(ACCOUNT_DATA, "w");

        if (fp == NULL)
        {
            printf("Error: Could not open file\n");
            return;
        }

        for (i = 0; i < count - 1; i++)
        {
            fprintf(fp, "Name: %s\nAccount Type: %s\nAccount No: %lld\nBalance: %lld\nPhone: %lld\nNID No: %lld\nUsername: %s\n", account_info[i].Name, account_info[i].AccountType, account_info[i].AccountNo, account_info[i].Balance, account_info[i].Phone, account_info[i].NID, account_info[i].Username);
        }
        fclose(fp);
        
        FILE *fp5;
        fp5 = fopen(USER_PASS, "w");

        if (fp5 == NULL)
        {
            printf("Error: Could not open file\n");
            return;
        }

        for (i = 0; i < count - 1; i++)
        {
            fprintf(fp5, "Username: %s\nPassword: %s\n", user_name[i].Username, user_name[i].Password);
        }
        fclose(fp5);

    

    }
    else
    {
        printf("\nAccount not found or does not belong to you.\n");
    }
}

int CheckAccountExists(long long accountNo)
{
    FILE *fp;
    AccountInfo account;
    fp = fopen(ACCOUNT_DATA, "r");
    if (fp == NULL)
    {
        printf("Error: Could not open file\n");
        return 0;
    }

    while (fscanf(fp, "Name: %[^\n] \nAccount Type: %[^\n] \nAccount No: %lld\nBalance: %lld\nPhone: %lld\nNID No: %lld\nUsername: %[^\n]\n", account.Name, account.AccountType, &account.AccountNo, &account.Balance, &account.Phone, &account.NID, account.Username) == 7)
    {
        if (account.AccountNo == accountNo)
        {
            printf("Error: Account number already exists. Please try again.\n\n");
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void CreateAccount2()
{
    AccountInfo *account_info = malloc(MAX_ACCOUNTS * sizeof(AccountInfo));
    if (account_info == NULL)
    {
        printf("Error: Memory allocation failed\n");
        return;
    }

    FILE *fp = fopen(ACCOUNT_DATA, "r");
    if (fp == NULL)
    {
        printf("Error: Could not open file\n");
        free(account_info);
        return;
    }

    int count = 0;
    while (fscanf(fp, "Name: %[^\n]\nAccount Type: %[^\n]\nAccount No: %lld\nBalance: %lld\nPhone: %lld\nNID No: %lld\nUsername: %s\n",
                  account_info[count].Name, account_info[count].AccountType, &account_info[count].AccountNo,
                  &account_info[count].Balance, &account_info[count].Phone, &account_info[count].NID, account_info[count].Username) == 7)
    {
        count++;
    }

    fclose(fp);

    printf("\nEnter your Name (first and last name): ");
    scanf(" %[^\n]", account_info[count].Name);
    printf("Enter Account Type (Savings or Current or Fixed Deposit): ");
    scanf(" %[^\n]", account_info[count].AccountType);
    printf("Enter Initial Deposit: ");
    scanf("%lld", &account_info[count].Balance);
    printf("Enter Phone number (13 digits): ");
    scanf("%lld", &account_info[count].Phone);
    printf("Enter NID number (9 digits): ");
    scanf("%lld", &account_info[count].NID);
    printf("Enter Username: ");
    scanf("%s", account_info[count].Username);

    char password[30];
    do
    {
        printf("Enter Password (at least 10 characters): ");
        scanf("%s", password);
    } while (strlen(password) < 10);

    srand(time(0));
    long long int accountNo = rand() % 9000000000 + 1000000000;
    while (CheckAccountExists(accountNo))
    {
        accountNo = rand() % 9000000000 + 1000000000;
    }
    account_info[count].AccountNo = accountNo;

    FILE *fp1 = fopen(USER_PASS, "a");
    if (fp1 == NULL)
    {
        printf("Error: Could not open file\n");
        free(account_info);
        return;
    }

    fprintf(fp1, "\nUsername: %s\n", account_info[count].Username);
    fprintf(fp1, "Password: %s\n", password);

    fclose(fp1);

    printf("\nAccount created successfully!\n");
    printf("Your Account No: %lld\n\n", account_info[count].AccountNo);

    count++;

    FILE *fp4 = fopen(ACCOUNT_DATA, "w");
    if (fp4 == NULL)
    {
        printf("Error: Could not open file.\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        fprintf(fp4, "Name: %s \nAccount Type: %s \nAccount No: %lld\nBalance: %lld\nPhone: %lld\nNID No: %lld\nUsername: %s\n\n",
                account_info[i].Name, account_info[i].AccountType, account_info[i].AccountNo,
                account_info[i].Balance, account_info[i].Phone, account_info[i].NID, account_info[i].Username);
    }

    fclose(fp4);
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

void CashDeposit(char *username, int count, AccountInfo *account_info)
{
    long long deposit, old_balance;
    int *matchingAccounts = malloc(count * sizeof(int));
    int i, choice, matchingCount = 0;

    int found = MatchAndShow(username, account_info, count, matchingAccounts);

    if (found == 0)
    {
        printf("No matching accounts found.\n");
        return;
    }

    else
    {
        printf("Choose the corresponding Account (1/2/3...): ");
        scanf("%d", &choice);
        int index = matchingAccounts[choice - 1];

        printf("\nEnter the amount to deposit: ");
        scanf("%lld", &deposit);
        printf("\n");

        old_balance = account_info[index].Balance;

        account_info[index].Balance = account_info[index].Balance + deposit;

        printf("Account Deposit Successful\n");
        printf("Current Balnce: Tk. %lld only", account_info[index].Balance);

        printf("\n \n");
    }

    FILE *fp = fopen(ACCOUNT_DATA, "w");
    if (fp == NULL)
    {
        printf("Error: Could not open file.\n");
        return;
    }

    for (i = 0; i < count; i++)
    {
        fprintf(fp, "Name: %s\nAccount Type: %s\nAccount No: %lld\nBalance: %lld\nPhone: %lld\nNID No: %lld\nUsername: %s\n\n",
                account_info[i].Name, account_info[i].AccountType, account_info[i].AccountNo,
                account_info[i].Balance, account_info[i].Phone, account_info[i].NID, account_info[i].Username);
    }

    fclose(fp);
    free(matchingAccounts);
}

void CashWithdrawal(char *username, int count, AccountInfo *account_info)
{
    long long withdrawal, old_balance;
    int *matchingAccounts = malloc(count * sizeof(int));
    int i, choice, matchingCount = 0;

    int found = MatchAndShow(username, account_info, count, matchingAccounts);

    if (found == 0)
    {
        printf("No matching accounts found.\n");
        return;
    }

    else
    {
        printf("Choose the corresponding Account (1/2/3...): ");
        scanf("%d", &choice);
        int index = matchingAccounts[choice - 1];

        printf("\nEnter the amount to withdraw: ");
        scanf("%lld", &withdrawal);
        printf("\n");

        old_balance = account_info[index].Balance;

        if (withdrawal > old_balance)
            printf("Not sufficient balance\n\n");

        else
        {
            account_info[index].Balance = account_info[index].Balance - withdrawal;

            printf("Account Deposit Successful\n");
            printf("Current Balnce: Tk. %lld only", account_info[index].Balance);

            printf("\n \n");
        }
    }

    FILE *fp = fopen(ACCOUNT_DATA, "w");
    if (fp == NULL)
    {
        printf("Error: Could not open file.\n");
        return;
    }

    for (i = 0; i < count; i++)
    {
        fprintf(fp, "Name: %s\nAccount Type: %s\nAccount No: %lld\nBalance: %lld\nPhone: %lld\nNID No: %lld\nUsername: %s\n\n",
                account_info[i].Name, account_info[i].AccountType, account_info[i].AccountNo,
                account_info[i].Balance, account_info[i].Phone, account_info[i].NID, account_info[i].Username);
    }

    fclose(fp);
    free(matchingAccounts);
}
