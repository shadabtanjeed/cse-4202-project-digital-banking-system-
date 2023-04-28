/*
__________IMPORTANT NOTES__________
For inputting Name manually in the text file put a space after the last name
For inputting Fixed Deposit as Account type in the text file, put a space at last
*/

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define USER_PASS "./username.txt"
#define ACCOUNT_DATA "./AccountInfo.txt"
#define TRANSACTION_HISTORY "./Transactions.txt"

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

typedef struct TransactionInfo
{
    long long AC_No;
    char Date[12];
    char Transaction_Type[20];
    long long OLD_Balance;
    long long TRNX_Amount;
    long long NEW_Balance;
    char TRNX_ID[10];
} TransactionInfo;

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
void Transaction(long long ac_no, char *date, char *trnx_type, long long old_balance, long long trnx_amnt, long long new_balance);
void generate_transaction_id(char *id);
void Statement(char *username, AccountInfo *account_info, int counter);
void swap_transactions(TransactionInfo *a, TransactionInfo *b);
void sort_transactions_by_date(TransactionInfo *transaction_info, int count_transaction);

int compare_dates(TransactionInfo *a, TransactionInfo *b);

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

        case 3:
            CashDeposit(usernm, count, account_info);
            break;

        case 4:
            CashWithdrawal(usernm, count, account_info);
            break;

        case 5:

        case 6:

        case 7:
            Statement(usernm, account_info, count);
            break;
        case 8:

        case 9:

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

/*
void createaccount()
{
    AccountInfo newAccount;

    char balance[10], phone[13], nid[9];

    printf("\nEnter your Name (first and last name): ");
    scanf("%s", newAccount.Name);
    printf("Enter Account Type (Savings or Current or Fixed Deposit): ");
    scanf("%s", newAccount.AccountType);
    printf("Enter Initial Deposit: ");
    scanf("%s", balance);
    printf("Enter Phone number (13 digits): ");
    scanf("%s", phone);
    printf("Enter NID number (9 digits): ");
    scanf("%s", nid);
    printf("Enter Username: ");
    scanf("%s", newAccount.Username);

    newAccount.Balance = atoi(balance);
    newAccount.Phone = atoi(phone);
    newAccount.NID = atoi(nid);

    // prompt for password
    char password[30];
    do
    {
        printf("Enter Password (at least 10 characters): ");
        scanf("%s", password);
    } while (strlen(password) < 10);

    // generate 10-digit account number
    srand(time(0));
    long long int accountNo = rand() % 9000000000 + 1000000000; // range from 1000000000 to 9999999999
    while (CheckAccountExists(accountNo))
    {
        accountNo = rand() % 9000000000 + 1000000000; // generate a new account number if it already exists
    }
    newAccount.AccountNo = accountNo;

    // add new account to first file
    FILE *fp3 = fopen(ACCOUNT_DATA, "a");
    if (fp3 == NULL)
    {
        printf("Error: Could not open file\n");
        return;
    }

    fprintf(fp3, "Name: %s\n", newAccount.Name);
    fprintf(fp3, "Account Type: %s\n", newAccount.AccountType);
    fprintf(fp3, "Account No: %lld\n", newAccount.AccountNo);
    fprintf(fp3, "Balance: %lld\n", newAccount.Balance);
    fprintf(fp3, "Phone: %lld\n", newAccount.Phone);
    fprintf(fp3, "NID No: %lld\n", newAccount.NID);
    fprintf(fp3, "Username: %s\n", newAccount.Username);
    fprintf(fp3, "Password: %s\n", password);

    fclose(fp3);

    // add new account to the other file
    FILE *fp1 = fopen(USER_PASS, "a");
    if (fp1 == NULL)
    {
        printf("Error: Could not open file\n");
        return;
    }

    fprintf(fp1, "Username: %s\n", newAccount.Username);
    fprintf(fp1, "Password: %s\n", password);

    fclose(fp1);

    printf("\nAccount created successfully!\n");
    printf("Your Account No: %lld\n", newAccount.AccountNo);
}
*/

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

        char date[12];
        printf("Enter the date of trancaction in the format (dd/mm/yyyy): ");
        scanf("%s", date);

        printf("\nEnter the amount to deposit: ");
        scanf("%lld", &deposit);
        printf("\n");

        old_balance = account_info[index].Balance;

        account_info[index].Balance = account_info[index].Balance + deposit;

        Transaction(account_info[index].AccountNo, date, "Deposit", old_balance, deposit, account_info[index].Balance);

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

        char date[12];
        printf("Enter the date of trancaction in the format (dd/mm/yyyy): ");
        scanf("%s", date);

        printf("\nEnter the amount to withdraw: ");
        scanf("%lld", &withdrawal);
        printf("\n");

        old_balance = account_info[index].Balance;

        if (withdrawal > old_balance)
            printf("Not sufficient balance\n\n");

        else
        {
            account_info[index].Balance = account_info[index].Balance - withdrawal;

            Transaction(account_info[index].AccountNo, date, "Withdraw", old_balance, withdrawal, account_info[index].Balance);

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

void Transaction(long long ac_no, char *date, char *trnx_type, long long old_balance, long long trnx_amnt, long long new_balance)
{
    char trnx_id[11];
    generate_transaction_id(trnx_id);

    FILE *fp1;
    fp1 = fopen(TRANSACTION_HISTORY, "a");
    if (fp1 == NULL)
    {
        printf("Error: Could not open file.\n");
        return;
    }
    fprintf(fp1, "Transaction ID: %s\nAccount No: %lld\nTransaction Date: %s\nTransaction Type: %s\nPrevious Balance: %lld\nTransaction Amount: %lld\nUpdated Balance: %lld\n\n", trnx_id, ac_no, date, trnx_type, old_balance, trnx_amnt, new_balance);

    fclose(fp1);
}

void generate_transaction_id(char *id)
{
    FILE *fp1;
    fp1 = fopen(TRANSACTION_HISTORY, "r");
    if (fp1 == NULL)
    {
        printf("Error: Could not open file.\n");
        return;
    }

    const char charset[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int len = sizeof(charset) - 1;

    while (1)
    {
        for (int i = 0; i < 10; i++)
        {
            id[i] = charset[rand() % len];
        }
        id[10] = '\0';

        char line[100];
        while (fgets(line, sizeof(line), fp1))
        {
            if (strstr(line, id))
            {
                fseek(fp1, 0, SEEK_SET);
                continue;
            }
        }
        break;
    }

    fclose(fp1);
}

void swap_transactions(TransactionInfo *a, TransactionInfo *b)
{
    TransactionInfo temp = *a;
    *a = *b;
    *b = temp;
}

int compare_dates(TransactionInfo *a, TransactionInfo *b)
{
    char *date_a = a->Date;
    char *date_b = b->Date;

    int day_a, month_a, year_a, day_b, month_b, year_b;

    // Splitting date_a based on its length
    if (strlen(date_a) == 10)
    {
        sscanf(date_a, "%d/%d/%d", &day_a, &month_a, &year_a);
    }
    else if (strlen(date_a) == 8)
    {
        sscanf(date_a, "%2d%2d%4d", &day_a, &month_a, &year_a);
    }
    else
    {
        return 0; // Invalid date format
    }

    // Splitting date_b based on its length
    if (strlen(date_b) == 10)
    {
        sscanf(date_b, "%d/%d/%d", &day_b, &month_b, &year_b);
    }
    else if (strlen(date_b) == 8)
    {
        sscanf(date_b, "%2d%2d%4d", &day_b, &month_b, &year_b);
    }
    else
    {
        return 0; // Invalid date format
    }

    if (year_a != year_b)
    {
        return year_a - year_b;
    }
    else if (month_a != month_b)
    {
        return month_a - month_b;
    }
    else
    {
        return day_a - day_b;
    }
}

void sort_transactions_by_date(TransactionInfo *transaction_info, int count_transaction)
{
    for (int i = 0; i < count_transaction - 1; i++)
    {
        for (int j = 0; j < count_transaction - i - 1; j++)
        {
            if (compare_dates(&transaction_info[j], &transaction_info[j + 1]) > 0)
            {
                swap_transactions(&transaction_info[j], &transaction_info[j + 1]);
            }
        }
    }
}

void Statement(char *username, AccountInfo *account_info, int counter)
{
    int *matchingAccounts = malloc(counter * sizeof(int));
    int choice;
    int found = MatchAndShow(username, account_info, counter, matchingAccounts);
    if (found == 0)
        printf("No accounts found \n\n");
    else
    {
        printf("Choose the corresponding Account (1/2/3...): ");
        scanf("%d", &choice);
        int index = matchingAccounts[choice - 1];
        long long account_no = account_info[index].AccountNo;

        free(matchingAccounts);

        TransactionInfo *transaction_info = malloc(MAX_ACCOUNTS * sizeof(TransactionInfo));

        FILE *fp;
        fp = fopen(TRANSACTION_HISTORY, "r");

        int count_transaction = 0;

        while (fscanf(fp, "Transaction ID: %s\nAccount No: %lld\nTransaction Date: %s\nTransaction Type: %s\nPrevious Balance: %lld\nTransaction Amount: %lld\nUpdated Balance: %lld\n", transaction_info[count_transaction].TRNX_ID, &transaction_info[count_transaction].AC_No, transaction_info[count_transaction].Date, transaction_info[count_transaction].Transaction_Type, &transaction_info[count_transaction].OLD_Balance, &transaction_info[count_transaction].TRNX_Amount, &transaction_info[count_transaction].NEW_Balance) == 7)
        {
            count_transaction++;
        }

        sort_transactions_by_date(transaction_info, count_transaction);

        int transactions = 0;

        for (int i = 0; i < count_transaction; ++i)
        {
            if (transaction_info[i].AC_No == account_no)
            {
                transactions++;
                if (transactions == 1)
                {
                    printf("\n\n");
                    printf("%-21s %-21s %-21s %-21s %-21s %-21s\n", "Transaction Date", "Transaction ID", "Transaction Type", "Previous Balance", "Transaction Amount", "Updated Balance");
                    printf("%-21s %-21s %-21s %-21s %-21s %-21s\n", "--------------", "------------------", "------------------", "-------------------", "-------------------", "-------------------");
                }
                printf("%-21s %-21s %-21s %-21lld %-21lld %-21lld\n", transaction_info[i].Date, transaction_info[i].TRNX_ID, transaction_info[i].Transaction_Type, transaction_info[i].OLD_Balance, transaction_info[i].TRNX_Amount, transaction_info[i].NEW_Balance);
            }
        }
        printf("\n\n");

        if (transactions > 0)
            printf("Final Balance: %lld\n\n", account_info[index].Balance);

        if (transactions == 0)
            printf("No transactions found!!\n\n");
        free(transaction_info);
    }
}
