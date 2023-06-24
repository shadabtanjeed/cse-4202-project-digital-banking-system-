/*
__________IMPORTANT NOTES__________
For inputting Name manually in the text file put a space after the last name
For inputting Fixed Deposit as Account type in the text file, put a space at last
*/

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define USER_PASS "./username.txt"
#define ACCOUNT_DATA "./AccountInfo.txt"
#define TRANSACTION_HISTORY "./Transactions.txt"
#define BENEFICIARY_INFO "./BeneficiaryInfo.txt"
#define LOAN_TEXT "./Loan.txt"
#define FEEDBACK "./Feedback.txt"
#define CREDIT_CARD_INFO "./CreditCardInfo.txt"
#define LOCATION_INFO "./LocationInfo.txt"

#define MAX_ACCOUNTS 100
#define ID_LENGTH 10
#define CARD_NUMBER_LENGTH 16
#define CVV_LENGTH 3
#define EXPIRY_DATE_LENGTH 6
#define MAX_REGION_LENGTH 15
#define MAX_NAME_LENGTH 100
#define MAX_ADDRESS_LENGTH 200
#define MAX_LOCATIONS 30

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

typedef struct Username
{
    char Username[30];
    char Password[30];
} Username;

typedef struct BeneficiaryInfo
{
    char Name[30];
    long long AccountNo;
    long long Phone;
    long long NID;
    char Username[30];
    char Beneficiary_Of[30];
} BeneficiaryInfo;

typedef struct LoanInfo
{
    char Username[30];
    long long AccountNo;
    int loan_term;
    char date[20];
    int interest_rate;
    long long amount;
} LoanInfo;

typedef struct CreditCardInfo
{
    char CardNo[20];
    char CardType[20];
    char ExpiryDate[8];
    char CVV[5];
    long long CardLimit;
    long long AvailableBalance;
    char Username[30];
} CreditCardInfo;

typedef struct LocationInfo
{
    char BranchName[MAX_NAME_LENGTH];
    char Address[MAX_ADDRESS_LENGTH];
    char Region[MAX_REGION_LENGTH];
} LocationInfo;

int loginverify(char *userid, char *pass);
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
void CreateAnotherAccount(char *username, int count, AccountInfo *account_info);
void Transaction(long long ac_no, char *date, char *trnx_type, long long old_balance, long long trnx_amnt, long long new_balance);
void generate_transaction_id(char *id);
void Statement(char *username, AccountInfo *account_info, int counter);
void swap_transactions(TransactionInfo *a, TransactionInfo *b);
void sort_transactions_by_date(TransactionInfo *transaction_info, int count_transaction);
void CloseAccount(char *username, int count, int count_user, AccountInfo *account_info, Username *user_name);
int compare_dates(TransactionInfo *a, TransactionInfo *b);
void AccountSettings(char *username, int count, AccountInfo *account_info, Username *user_name, int count_user);
void Beneficiary(char *username);
int ShowBenificiary(char *usname, struct BeneficiaryInfo *benefeciary_info, int counter, int *matchingAccounts);
void FundTransfer(char *username, int count, AccountInfo *account_info);
void BillPayment(char *username, int count, AccountInfo *account_info, CreditCardInfo *credit_card_info, int card_count);
void LoanManagement(char *username, int count, AccountInfo *account_info);
void Feedback(char *username);
int MatchAndShowCards(char *username, CreditCardInfo *credit_card_info, int count, int *matchingcards);
void ApplyForCreditCard(char *username, int count, CreditCardInfo *credit_card_info);
void RemoveCreditCard(char *username, int count, CreditCardInfo *credit_card_info);
void ViewCreditCardDetails(char *username, int count, CreditCardInfo *credit_card_info);
void CreditCardMenu(char *username, int count, CreditCardInfo *credit_card_info);
void ShowLocation();

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
    CreditCardInfo *credit_card_info = (CreditCardInfo *)malloc(MAX_ACCOUNTS * sizeof(CreditCardInfo));

    while (menuchoice != 12)
    {
        int count = 0, count_user = 0, count_credit_cards = 0;
        char garbage_value;

        FILE *fp;
        FILE *fp5;
        FILE *fp6;

        fp = fopen(ACCOUNT_DATA, "r");
        fp5 = fopen(USER_PASS, "r");
        fp6 = fopen(CREDIT_CARD_INFO, "r");
        if (fp == NULL || fp5 == NULL || fp6 == NULL)
        {
            printf("Error: Could not open file\n");
        }

        while (fscanf(fp, "Name: %[^\n] \nAccount Type: %[^\n] \nAccount No: %lld\nBalance: %lld\nPhone: %lld\nNID No: %lld\nUsername: %s\n", account_info[count].Name, account_info[count].AccountType, &account_info[count].AccountNo, &account_info[count].Balance, &account_info[count].Phone, &account_info[count].NID, account_info[count].Username) == 7)
        {
            ++count;
        }

        while (fscanf(fp5, "Username: %s\nPassword: %s\n", user_name[count_user].Username, user_name[count_user].Password) == 2)
        {
            count_user++;
        }

        while (fscanf(fp6, "Card No: %[^\n]\nCard Type: %[^\n]\nExpiry Date: %[^\n]\nCVV: %[^\n]\nCard Limit: %lld\nAvailable Balance: %lld\nUsername: %[^\n]\n", credit_card_info[count_credit_cards].CardNo, credit_card_info[count_credit_cards].CardType, credit_card_info[count_credit_cards].ExpiryDate, credit_card_info[count_credit_cards].CVV, &credit_card_info[count_credit_cards].CardLimit, &credit_card_info[count_credit_cards].AvailableBalance, credit_card_info[count_credit_cards].Username) == 7)
        {
            ++count_credit_cards;
        }


        fclose(fp);
        fclose(fp5);
        fclose(fp6);

        printf("1. View All Accounts\n");
        printf("2. Check Balance\n");
        printf("3. Cash Deposit\n");
        printf("4. Cash Withdraw\n");
        printf("5. Fund Transfer\n");
        printf("6. Bill Payment\n");
        printf("7. Beneficiary Management\n");
        printf("8. Credit Card Management\n");
        printf("9. Loan Management\n");
        printf("10. Mini Statement\n");
        printf("11. Create Another Account\n");
        printf("12. Account Settings\n");
        printf("13. Close Account\n");
        printf("14. ATM/Branch Locations\n");
        printf("15. Customer Support\n");
        printf("16. Exit\n");
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
            FundTransfer(usernm, count, account_info);
            break;

        case 6:
            BillPayment(usernm, count, account_info, credit_card_info, count_credit_cards);
            break;

        case 7:
            Beneficiary(usernm);
            break;

        case 8:
            CreditCardMenu(usernm, count_credit_cards, credit_card_info);
            break;

        case 9:
            LoanManagement(usernm, count, account_info);
            break;

        case 10:
            Statement(usernm, account_info, count);
            break;

        case 11:
            CreateAnotherAccount(usernm, count, account_info);
            break;

        case 12:
            AccountSettings(usernm, count, account_info, user_name, count_user);
            break;

        case 13:
            CloseAccount(usernm, count, count_user, account_info, user_name);
            break;

        case 14:
            ShowLocation();
            break;

        case 15:
            Feedback(usernm);
            break;

        case 16:
            free(account_info);
            return;
            break;

        default:
            printf("Please enter a valid choice \n \n");
        }
    }
}

void AccountSettings(char *username, int count, AccountInfo *account_info, Username *user_name, int count_user)
{
    int i, j, k, found = 0, user_choice;
    char new_name[30], new_account_type[20], new_phone[12], new_nid[20], new_password[30], new_username[30];

    // Search for the account with the matching username
    for (i = 0; i < count; i++)
    {
        if (strcmp(account_info[i].Username, username) == 0)
        {
            found = 1;
            break;
        }
    }

    for (j = 0; j < count_user; j++)
    {
        if (strcmp(user_name[j].Username, username) == 0)
        {
            break;
        }
    }

    if (found == 1)
    {
        printf("\n\nAccount Settings\n\n");
        printf("1. Update Name\n");
        printf("2. Update Account Type\n");
        printf("3. Update Phone Number\n");
        printf("4. Update National ID (NID) Number\n");
        printf("5. Update Password\n");
        printf("6. Update Username\n");
        printf("7. Exit\n\n");
        printf("Enter your choice: ");
        scanf("%d", &user_choice);
        printf("\n");

        switch (user_choice)
        {
        case 1:
            printf("Enter new name: ");
            scanf(" %[^\n]", new_name);
            strcpy(account_info[i].Name, new_name);
            printf("Name updated successfully.\n\n");
            break;

        case 2:
            printf("Enter new account type: ");
            scanf(" %[^\n]", new_account_type);
            strcpy(account_info[i].AccountType, new_account_type);
            printf("Account type updated successfully.\n\n");
            break;

        case 3:
            printf("Enter new phone number: ");
            scanf("%s", new_phone);
            account_info[i].Phone = atoll(new_phone);
            printf("Phone number updated successfully.\n\n");
            break;

        case 4:
            printf("Enter new NID number: ");
            scanf("%s", new_nid);
            account_info[i].NID = atoll(new_nid);
            printf("NID number updated successfully.\n\n");
            break;

        case 5:
            printf("Enter new password: ");
            scanf("%s", new_password);
            strcpy(user_name[j].Password, new_password);
            printf("Password updated successfully.\n");
            break;

        case 6:
            printf("Enter new username: ");
            scanf("%s", new_username);
            // Check if the new username already exists
            for (k = 0; k < count_user; k++)
            {
                if (strcmp(new_username, user_name[k].Username) == 0)
                {
                    printf("Error: Username already exists\n");
                    return;
                }
            }
            strcpy(account_info[i].Username, new_username);
            strcpy(user_name[j].Username, new_username);
            printf("Username updated successfully.\n\n");
            break;

        case 7:
            return;

        default:
            printf("Invalid choice.\n");
            break;
        }

        // Write the updated account information to file
        if (user_choice != 5)
        {
            FILE *fp;
            fp = fopen(ACCOUNT_DATA, "w");
            if (fp == NULL)
            {
                printf("Error: Could not open file\n");
                return;
            }

            for (i = 0; i < count; i++)
            {
                fprintf(fp, "Name: %s\nAccount Type: %s\nAccount No: %lld\nBalance: %lld\nPhone: %lld\nNID No: %lld\nUsername: %s\n\n", account_info[i].Name, account_info[i].AccountType, account_info[i].AccountNo, account_info[i].Balance, account_info[i].Phone, account_info[i].NID, account_info[i].Username);
            }

            fclose(fp);
        }

        // Update the username or password in the username file if changed
        if (user_choice == 6 || user_choice == 5)
        {
            FILE *fp2;
            fp2 = fopen(USER_PASS, "w");
            if (fp2 == NULL)
            {
                printf("Error: Could not open file\n");
                return;
            }

            for (k = 0; k < count_user; k++)
            {
                fprintf(fp2, "Username: %s\nPassword: %s\n\n", user_name[k].Username, user_name[k].Password);
            }

            fclose(fp2);
        }
    }
    else
    {
        printf("Error: Account not found.\n");
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

void CreateAnotherAccount(char *username, int count, AccountInfo *account_info)
{
    int found_index;
    for (int i = 0; i < count; i++)
    {
        scanf("Name: %s \nAccount Type: %s \nAccount No: %lld\nBalance: %lld\nPhone: %lld\nNID No: %lld\nUsername: %s\n\n",
              account_info[i].Name, account_info[i].AccountType, &account_info[i].AccountNo,
              &account_info[i].Balance, &account_info[i].Phone, &account_info[i].NID, account_info[i].Username);

        if (strcmp(username, account_info[i].Username) == 0)
        {
            found_index = i;
            break;
        }
    }

    printf("Enter Account Type (Savings or Current or Fixed Deposit): ");
    scanf(" %[^\n]", account_info[count].AccountType);
    printf("Enter Initial Deposit: ");
    scanf("%lld", &account_info[count].Balance);

    strcpy(account_info[count].Name, account_info[found_index].Name);
    account_info[count].NID = account_info[found_index].NID;
    account_info[count].Phone = account_info[found_index].Phone;
    strcpy(account_info[count].Username, account_info[found_index].Username);

    srand(time(0));
    long long int accountNo = rand() % 9000000000 + 1000000000;
    while (CheckAccountExists(accountNo))
    {
        accountNo = rand() % 9000000000 + 1000000000;
    }
    account_info[count].AccountNo = accountNo;

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
        printf("No matching accounts found\n\n");
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
    const char charset[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int len = sizeof(charset) - 1;

    for (int i = 0; i < 10; i++)
    {
        id[i] = charset[rand() % len];
    }
    id[10] = '\0';
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

void CloseAccount(char *username, int count, int count_user, AccountInfo *account_info, Username *user_name)
{
    int i, j, flag = 0;

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

        for (int j = index; j < count - 1; j++)
        {
            account_info[j] = account_info[j + 1];
        }

        printf("\nAccount closed successfully.\n\n");

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
            fprintf(fp, "Name: %s\nAccount Type: %s\nAccount No: %lld\nBalance: %lld\nPhone: %lld\nNID No: %lld\nUsername: %s\n\n", account_info[i].Name, account_info[i].AccountType, account_info[i].AccountNo, account_info[i].Balance, account_info[i].Phone, account_info[i].NID, account_info[i].Username);
        }
        fclose(fp);

        if (found == 1)
        {

            int user_index;
            for (int i = 0; i < count_user; ++i)
            {
                if (strcmp(user_name[i].Username, username) == 0)
                {
                    user_index = i;
                    break;
                }
            }

            for (int j = user_index; j < count_user - 1; j++)
            {
                user_name[j] = user_name[j + 1];
            }

            FILE *fp5;
            fp5 = fopen(USER_PASS, "w");

            if (fp5 == NULL)
            {
                printf("Error: Could not open file\n");
                return;
            }

            for (i = 0; i < count_user - 1; i++)
            {
                fprintf(fp5, "Username: %s\nPassword: %s\n\n", user_name[i].Username, user_name[i].Password);
            }
            fclose(fp5);
        }
    }

    free(matchingAccounts);
}

int ShowBenificiary(char *usname, struct BeneficiaryInfo *benefeciary_info, int counter, int *matchingAccounts)
{
    int numMatchingAccounts = 0;
    if (matchingAccounts == NULL)
    {
        printf("Error: Could not allocate memory\n");
        return 0;
    }

    for (int i = 0; i < counter; i++)
    {
        if (strcmp(benefeciary_info[i].Beneficiary_Of, usname) == 0)
        {
            matchingAccounts[numMatchingAccounts] = i;
            numMatchingAccounts++;
        }
    }

    printf("\nBeneficiaries for %s:\n\n", usname);
    for (int i = 0; i < numMatchingAccounts; i++)
    {
        int accountIndex = matchingAccounts[i];
        printf("%d. Name: %s\n   Account No: %lld \n\n", i + 1, benefeciary_info[accountIndex].Name, benefeciary_info[accountIndex].AccountNo);
    }
    printf("\n");

    return numMatchingAccounts;
}

void Beneficiary(char *username)
{
    int menu_choice, beneficiary_found = 0, count_beneficiary = 0;

    FILE *fp1;
    fp1 = fopen(BENEFICIARY_INFO, "r");

    if (fp1 == NULL)
        printf("Could not open file\n\n");
    else
    {
        BeneficiaryInfo *beneficiary_info = (BeneficiaryInfo *)malloc(MAX_ACCOUNTS * sizeof(BeneficiaryInfo));

        while (fscanf(fp1, "Name: %[^\n] \nAccount No: %lld\nPhone: %lld\nNID No: %lld\nUsername: %[^\n] \nBeneficiary for: %[^\n] \n", beneficiary_info[count_beneficiary].Name, &beneficiary_info[count_beneficiary].AccountNo, &beneficiary_info[count_beneficiary].Phone, &beneficiary_info[count_beneficiary].NID, beneficiary_info[count_beneficiary].Username, beneficiary_info[count_beneficiary].Beneficiary_Of) == 6)

        {
            count_beneficiary++;
        }

        fclose(fp1);

        printf("1. View all Beneficiaries\n");
        printf("2. Add new Beneficiary\n");
        printf("3. Remove a Beneficiary\n\n");
        printf("Choose your option: ");
        scanf("%d", &menu_choice);

        if (menu_choice == 1)
        {
            printf("\n");
            for (int i = 0; i < count_beneficiary; ++i)
            {
                if (strcmp(username, beneficiary_info[i].Beneficiary_Of) == 0)
                {
                    beneficiary_found++;
                    printf("Beneficiary %d\n", beneficiary_found);
                    printf("Name: %s\n", beneficiary_info[i].Name);
                    printf("Account No: %lld\n", beneficiary_info[i].AccountNo);
                    printf("\n");
                }
            }

            if (beneficiary_found == 0)
                printf("No beneficiary found\n\n");
        }
        else if (menu_choice == 2)
        {
            printf("Enter Name: ");
            scanf(" %[^\n]", beneficiary_info[count_beneficiary].Name);

            printf("Enter Account No: ");
            scanf("%lld", &beneficiary_info[count_beneficiary].AccountNo);

            printf("Enter Phone No: ");
            scanf("%lld", &beneficiary_info[count_beneficiary].Phone);

            printf("Enter NID: ");
            scanf("%lld", &beneficiary_info[count_beneficiary].NID);

            printf("Enter Username: ");
            scanf(" %[^\n]", beneficiary_info[count_beneficiary].Username);

            strcpy(beneficiary_info[count_beneficiary].Beneficiary_Of, username);

            count_beneficiary++;

            FILE *fp2;
            fp2 = fopen(BENEFICIARY_INFO, "w");

            if (fp2 == NULL)
                printf("Could not open file\n\n");

            else
            {
                for (int j = 0; j < count_beneficiary; ++j)
                {
                    fprintf(fp2, "Name: %s\nAccount No: %lld\nPhone: %lld\nNID No: %lld\nUsername: %s\nBeneficiary for: %s\n\n", beneficiary_info[j].Name, beneficiary_info[j].AccountNo, beneficiary_info[j].Phone, beneficiary_info[j].NID, beneficiary_info[j].Username, beneficiary_info[j].Beneficiary_Of);
                }
                fclose(fp2);

                printf("\nBeneficiary Added Successfully\n\n");
            }
        }

        else if (menu_choice == 3)
        {

            int *matchingAccounts = malloc(count_beneficiary * sizeof(int));
            ShowBenificiary(username, beneficiary_info, count_beneficiary, matchingAccounts);
            printf("Choose the account you want to remove: ");
            int choice_remove;
            scanf("%d", &choice_remove);

            int index_remove = matchingAccounts[choice_remove - 1];

            for (int j = index_remove; j < count_beneficiary; j++)
            {
                beneficiary_info[j] = beneficiary_info[j + 1];
            }

            count_beneficiary--;

            FILE *fp3;
            fp3 = fopen(BENEFICIARY_INFO, "w");

            if (fp3 == NULL)
                printf("Could not open file\n\n");

            else
            {
                for (int j = 0; j < count_beneficiary; ++j)
                {
                    fprintf(fp3, "Name: %s\nAccount No: %lld\nPhone: %lld\nNID No: %lld\nUsername: %s\nBeneficiary for: %s\n\n", beneficiary_info[j].Name, beneficiary_info[j].AccountNo, beneficiary_info[j].Phone, beneficiary_info[j].NID, beneficiary_info[j].Username, beneficiary_info[j].Beneficiary_Of);
                }
                fclose(fp3);

                printf("\nAccount closed successfully.\n\n");
            }
        }
        else
            printf("Invalid Choice");

        free(beneficiary_info);
    }
}

void FundTransfer(char *username, int count, AccountInfo *account_info)
{
    int choice_main;
    long long old_source, new_source, old_destination, new_destination;

    printf("1. Transfer within own account\n");
    printf("2. Transfer to a different account\n");
    printf("\nChoose your option: ");
    scanf("%d", &choice_main);

    if (choice_main == 1)
    {
        long long ac_source, ac_destination;

        int *matchingAccounts1 = malloc(count * sizeof(int));

        printf("\n");
        int found = MatchAndShow(username, account_info, count, matchingAccounts1);

        if (found == 0)
        {
            printf("No matching accounts found.\n");
            free(matchingAccounts1);
            return;
        }
        else
        {
            int ac_choice;
            printf("Choose the source Account (1/2/3...): ");
            scanf("%d", &ac_choice);
            int index_source = matchingAccounts1[ac_choice - 1];

            printf("\n");

            int *matchingAccounts2 = malloc(count * sizeof(int));
            MatchAndShow(username, account_info, count, matchingAccounts2);

            printf("Choose the destination Account (1/2/3...): ");
            scanf("%d", &ac_choice);
            int index_destination = matchingAccounts2[ac_choice - 1];

            if (index_destination == index_source)
            {
                printf("Transfer to the same account not possible\n\n");
                free(matchingAccounts1);
                free(matchingAccounts2);
                return;
            }

            long long amount;
            printf("Enter amount: ");
            scanf("%lld", &amount);

            if (account_info[index_source].Balance < amount)
            {
                printf("Insufficient Balance\n\n");
                free(matchingAccounts1);
                free(matchingAccounts2);
                return;
            }

            char date[12];
            printf("Enter the date of transaction in the format (dd/mm/yyyy): ");
            scanf("%s", date);

            printf("\nSelect the method of transfer: \n\n");
            printf("1. EFT\n");
            printf("2. NPSB\n");
            printf("3. RTGS\n\n");
            printf("Enter your choice: ");

            int transfer_method;
            scanf("%d", &transfer_method);

            // OTP PART
            int otp;
            srand(time(NULL));
            otp = rand() % 9000 + 1000;

            FILE *fptr;
            fptr = fopen("OTP.txt", "w");

            if (fptr == NULL)
            {
                printf("Error opening file!");
                free(matchingAccounts1);
                free(matchingAccounts2);
                return;
            }

            fprintf(fptr, "%d", otp);

            fclose(fptr);

            int OTP_user;

            printf("A 4 digit OTP has been sent to the OTP.txt file\n\n");
            printf("OTP: ");
            scanf("%d", &OTP_user);
            printf("\n");

            if (otp != OTP_user)
            {
                printf("OTP didn't match. \n\n");
                free(matchingAccounts1);
                free(matchingAccounts2);
                return;
            }

            old_source = account_info[index_source].Balance;
            new_source = old_source - amount;
            account_info[index_source].Balance = new_source;

            Transaction(account_info[index_source].AccountNo, date, "FundTransfer(OUT)", old_source, amount, new_source);

            old_destination = account_info[index_destination].Balance;
            new_destination = old_destination + amount;
            account_info[index_destination].Balance = new_destination;

            Transaction(account_info[index_destination].AccountNo, date, "FundTransfer(IN)", old_destination, amount, new_destination);

            printf("Fund Transfer Successful\n\n");

            FILE *fp = fopen(ACCOUNT_DATA, "w");
            if (fp == NULL)
            {
                printf("Error: Could not open file.\n");
                free(matchingAccounts1);
                free(matchingAccounts2);
                return;
            }

            for (int i = 0; i < count; i++)
            {
                fprintf(fp, "Name: %s\nAccount Type: %s\nAccount No: %lld\nBalance: %lld\nPhone: %lld\nNID No: %lld\nUsername: %s\n\n",
                        account_info[i].Name, account_info[i].AccountType, account_info[i].AccountNo,
                        account_info[i].Balance, account_info[i].Phone, account_info[i].NID, account_info[i].Username);
            }

            fclose(fp);

            free(matchingAccounts1);
            free(matchingAccounts2);
        }
    }
    else if (choice_main == 2)
    {
        int *matchingAccounts3 = malloc(count * sizeof(int));

        printf("\n");
        int found = MatchAndShow(username, account_info, count, matchingAccounts3);

        if (found == 0)
        {
            printf("No matching accounts found.\n");
            free(matchingAccounts3);
            return;
        }
        else
        {
            int source_choice, source_index, destination_choice, destination_index, benefeciary_index, temp_choice;
            long long amount, source_old, source_new, destination_old, destination_new;
            printf("Choose the source account: ");
            scanf("%d", &source_choice);

            source_index = matchingAccounts3[source_choice - 1];
            free(matchingAccounts3);

            printf("Enter destination account info: \n\n");
            printf("1. Input manually\n");
            printf("2. Select from beneficiary\n\n");
            printf("Your Choice: ");
            scanf("%d", &temp_choice);

            if (temp_choice == 1)
            {
                long Phone, NID, AccountNo;
                char Name[20];

                printf("Enter details\n\n");
                printf("Name: ");
                scanf(" %[^\n]", Name);
                printf("Account No: ");
                scanf("%lld", &AccountNo);
                printf("Phone No: ");
                scanf("%lld", &Phone);
                printf("NID: ");
                scanf("%lld", &NID);

                int ac_found = 0;

                for (int i = 0; i < count; ++i)
                {
                    if (AccountNo == account_info[i].AccountNo)
                    {
                        ac_found = 1;
                        destination_index = i;
                        break;
                    }
                }

                if (ac_found == 0)
                {
                    printf("Destination Account Not Found\n\n");
                    return;
                }
            }

            if (temp_choice == 2)
            {
                int beneficiary_index, beneficiary_found = 0, count_beneficiary = 0;

                FILE *fp5;
                fp5 = fopen(BENEFICIARY_INFO, "r");

                if (fp5 == NULL)
                {
                    printf("Could not open file\n\n");
                    return;
                }
                else
                {
                    BeneficiaryInfo *beneficiary_info = malloc(MAX_ACCOUNTS * sizeof(BeneficiaryInfo));

                    while (fscanf(fp5, "Name: %[^\n] \nAccount No: %lld\nPhone: %lld\nNID No: %lld\nUsername: %[^\n] \nBeneficiary for: %[^\n] \n", beneficiary_info[count_beneficiary].Name, &beneficiary_info[count_beneficiary].AccountNo, &beneficiary_info[count_beneficiary].Phone, &beneficiary_info[count_beneficiary].NID, beneficiary_info[count_beneficiary].Username, beneficiary_info[count_beneficiary].Beneficiary_Of) == 6)

                    {
                        count_beneficiary++;
                    }

                    fclose(fp5);

                    int *matchingAccounts4 = malloc(count * sizeof(int));

                    ShowBenificiary(username, beneficiary_info, count_beneficiary, matchingAccounts4);

                    printf("Choose the beneficiary: ");
                    int beneficiary_choice;
                    scanf("%d", &beneficiary_choice);

                    beneficiary_index = matchingAccounts4[beneficiary_choice - 1];

                    for (int i = 0; i < count; ++i)
                    {
                        if (account_info[i].AccountNo == beneficiary_info[beneficiary_index].AccountNo)
                        {
                            destination_index = i;
                            break;
                        }
                    }

                    free(beneficiary_info);
                    free(matchingAccounts4);
                }
            }

            printf("\nEnter amount: ");
            scanf("%lld", &amount);

            if (account_info[source_index].Balance < amount)
            {
                printf("Insufficient Balance\n\n");
                return;
            }

            char date[12];
            printf("Enter the date of transaction in the format (dd/mm/yyyy): ");
            scanf("%s", date);

            printf("\nSelect the method of transfer: \n\n");
            printf("1. EFT\n");
            printf("2. NPSB\n");
            printf("3. RTGS\n\n");
            printf("Enter your choice: ");
            int transfer_method;
            scanf("%d", &transfer_method);

            // OTP PART
            int otp;
            srand(time(NULL));
            otp = rand() % 9000 + 1000;

            FILE *fptr;
            fptr = fopen("OTP.txt", "w");

            if (fptr == NULL)
            {
                printf("Error opening file!");
                return;
            }

            fprintf(fptr, "%d", otp);

            fclose(fptr);

            int OTP_user;

            printf("A 4 digit OTP has been sent to the OTP.txt file\n\n");
            printf("OTP: ");
            scanf("%d", &OTP_user);
            printf("\n");

            if (otp != OTP_user)
            {
                printf("OTP didn't match. \n\n");
                return;
            }

            source_old = account_info[source_index].Balance;
            source_new = source_old - amount;
            account_info[source_index].Balance = source_new;

            Transaction(account_info[source_index].AccountNo, date, "FundTransfer(OUT)", source_old, amount, source_new);

            destination_old = account_info[destination_index].Balance;
            destination_new = destination_old + amount;
            account_info[destination_index].Balance = destination_new;

            Transaction(account_info[destination_index].AccountNo, date, "FundTransfer(IN)", destination_old, amount, destination_new);

            printf("Fund Transfer Successful\n\n");

            FILE *fp = fopen(ACCOUNT_DATA, "w");
            if (fp == NULL)
            {
                printf("Error: Could not open file.\n");
                return;
            }

            for (int i = 0; i < count; i++)
            {
                fprintf(fp, "Name: %s\nAccount Type: %s\nAccount No: %lld\nBalance: %lld\nPhone: %lld\nNID No: %lld\nUsername: %s\n\n",
                        account_info[i].Name, account_info[i].AccountType, account_info[i].AccountNo,
                        account_info[i].Balance, account_info[i].Phone, account_info[i].NID, account_info[i].Username);
            }

            fclose(fp);
        }
    }
}

void BillPayment(char *username, int count, AccountInfo *account_info, CreditCardInfo *credit_card_info, int card_count)
{


    printf("\n");
    printf("1. Mobile Recharge\n");
    printf("2. Electricity Bill\n");
    printf("3. Gas Bill\n");
    printf("4. Internet Bill\n");
    printf("5. Tution Fee\n");
    printf("6. Merchant Payment\n\n");

    int menu_choice;
    printf("Choose your option: ");
    scanf("%d", &menu_choice);

    long long amount;
    char transaction_type[20];

    if (menu_choice == 1)
    {
        printf("\nSelect your operator:\n\n");
        printf("1. Grameenphone\n");
        printf("2. Banglalink\n");
        printf("3. Robi\n");
        printf("4. Airtel\n");
        printf("5. Teletalk\n\n");

        int operator_choice;
        printf("Your choice: ");
        scanf("%d", &operator_choice);

        long long phone_no;
        printf("Enter your phone number: ");
        scanf("%lld", &phone_no);

        printf("Enter recharge amount: ");
        scanf("%lld", &amount);

        strcpy(transaction_type, "MobileRecharge");

        printf("\n\n");
    }

    else if (menu_choice == 2)
    {
        char customer_id[20], month[10];
        printf("Enter your customer id: ");
        scanf("%s", customer_id);

        printf("Enter the month of payment: ");
        scanf("%s", month);

        printf("Enter bill amount: ");
        scanf("%lld", &amount);

        strcpy(transaction_type, "ElectricityBill");

        printf("\n\n");
    }

    else if (menu_choice == 3)
    {
        char customer_id[20], month[10];
        printf("Enter your customer id: ");
        scanf("%s", customer_id);

        printf("Enter the month of payment: ");
        scanf("%s", month);

        printf("Enter bill amount: ");
        scanf("%lld", &amount);

        strcpy(transaction_type, "GasBill");

        printf("\n\n");
    }

    else if (menu_choice == 4)
    {
        char customer_id[20], month[10];
        printf("Enter your customer id: ");
        scanf("%s", customer_id);

        printf("Enter the month of payment: ");
        scanf("%s", month);

        printf("Enter bill amount: ");
        scanf("%lld", &amount);

        strcpy(transaction_type, "InternetBill");

        printf("\n\n");
    }

    else if (menu_choice == 5)
    {
        char institution_name[50];
        char month[10];

        printf("Enter Institution Name: ");
        scanf("%s", institution_name);

        printf("Enter the month of payment: ");
        scanf("%s", month);

        printf("Enter the amount of tution fee: ");
        scanf("%lld", &amount);

        strcpy(transaction_type, "TutionFee");

        printf("\n\n");
    }

    else if (menu_choice == 6)
    {
        char merchant_id[20];
        char month[10];

        printf("Enter Merchant ID: ");
        scanf("%s", merchant_id);

        printf("Enter bill amount: ");
        scanf("%lld", &amount);

        strcpy(transaction_type, "MerchantBill");

        printf("\n\n");
    }

    else
    {
        printf("Invalid Choice\n\n");
        return;
    }


    int paychoice;
    printf("1.Pay from Account\n");
    printf("2.Pay from Credit Card\n");
    printf("Enter your choice: ");
    scanf("%d", &paychoice);
    printf("\n");
    
    switch (paychoice)
    {
    case 1:
        printf("Select source account: \n\n");

        int *matchingAccounts = malloc(count * sizeof(int));
        int ACChoice;
        int found = MatchAndShow(username, account_info, count, matchingAccounts);
        if (found == 0)
            printf("No accounts found \n\n");
        else
        {
            printf("Choose the corresponding Account (1/2/3...): ");
            scanf("%d", &ACChoice);
            int index = matchingAccounts[ACChoice - 1];
            if (amount > account_info[index].Balance)
            {
                printf("Insuffecient Balance\n\n");
                return;
            }
            else
            {
                char date[20];

                printf("Enter the date of transaction (dd/mm/yyyy): ");
                scanf("%s", date);

                // OTP Part
                int otp;
                srand(time(NULL));
                otp = rand() % 9000 + 1000;

                FILE *fptr;
                fptr = fopen("OTP.txt", "w");

                if (fptr == NULL)
                {
                    printf("Error opening file!");
                    return;
                }

                fprintf(fptr, "%d", otp);

                fclose(fptr);

                int OTP_user;

                printf("A 4 digit OTP has been sent to the OTP.txt file\n\n");
                printf("OTP: ");
                scanf("%d", &OTP_user);
                printf("\n");

                if (otp != OTP_user)
                {
                    printf("OTP didn't match. \n\n");
                    return;
                }

                long long old_balance = account_info[index].Balance;
                long long new_balance = old_balance - amount;
                account_info[index].Balance = new_balance;

                Transaction(account_info[index].AccountNo, date, transaction_type, old_balance, amount, new_balance);

                FILE *fp = fopen(ACCOUNT_DATA, "w");
                if (fp == NULL)
                {
                    printf("Error: Could not open file.\n");
                    return;
                }

                for (int i = 0; i < count; i++)
                {
                    fprintf(fp, "Name: %s\nAccount Type: %s\nAccount No: %lld\nBalance: %lld\nPhone: %lld\nNID No: %lld\nUsername: %s\n\n",
                            account_info[i].Name, account_info[i].AccountType, account_info[i].AccountNo,
                            account_info[i].Balance, account_info[i].Phone, account_info[i].NID, account_info[i].Username);
                }

                fclose(fp);
                printf("Fund Transfer Successful\n\n");
            }
            printf("\n \n");
        }

        free(matchingAccounts);
        break;

    case 2:
        printf("Select source account: \n\n");

        int *matchingcards = malloc(count * sizeof(int));
        int cardchoice;
        found = MatchAndShowCards(username, credit_card_info, card_count, matchingcards);
        if (found == 0)
            printf("No accounts found \n\n");
        else
        {
            printf("Choose the corresponding Card (1/2/3...): ");
            scanf("%d", &cardchoice);
            int index = matchingcards[cardchoice - 1];
            if (amount > credit_card_info[index].AvailableBalance)
            {
                printf("Insuffecient Balance\n\n");
                return;
            }
            else
            {
                // OTP Part
                int otp;
                srand(time(NULL));
                otp = rand() % 9000 + 1000;

                FILE *fptr;
                fptr = fopen("OTP.txt", "w");

                if (fptr == NULL)
                {
                    printf("Error opening file!");
                    return;
                }

                fprintf(fptr, "%d", otp);

                fclose(fptr);

                int OTP_user;

                printf("A 4 digit OTP has been sent to the OTP.txt file\n\n");
                printf("OTP: ");
                scanf("%d", &OTP_user);
                printf("\n");

                if (otp != OTP_user)
                {
                    printf("OTP didn't match. \n\n");
                    return;
                }

                credit_card_info[index].AvailableBalance -= amount;

                FILE *fp = fopen(CREDIT_CARD_INFO, "w");
                if (fp == NULL)
                {
                    printf("Error: Could not open file.\n");
                    return;
                }

                for (int i = 0; i < card_count; i++)
                {
                    fprintf(fp, "Card No: %s\nCard Type: %s\nExpiry Date: %s\nCVV: %s\nCard Limit: %lld\nAvailable Balance: %lld\nUsername: %s\n\n", credit_card_info[i].CardNo, credit_card_info[i].CardType, credit_card_info[i].ExpiryDate, credit_card_info[i].CVV, credit_card_info[i].CardLimit, credit_card_info[i].AvailableBalance, credit_card_info[i].Username);
                }

                fclose(fp);
                printf("Payment Successful\n\n");
            }
            printf("\n \n");
            free(matchingcards);
        }
        break;
    }
    
}

void LoanManagement(char *username, int count, AccountInfo *account_info)
{
    LoanInfo *loan_info = malloc(MAX_ACCOUNTS * sizeof(LoanInfo));

    FILE *fp1;
    fp1 = fopen(LOAN_TEXT, "r");

    int count_loan_entry = 0;

    while (fscanf(fp1, "Username: %s\nAccount No: %lld\nLoan Term: %d\nDate: %s\nInterest Rate: %d\nAmount: %lld\n", loan_info[count_loan_entry].Username, &loan_info[count_loan_entry].AccountNo, &loan_info[count_loan_entry].loan_term, loan_info[count_loan_entry].date, &loan_info[count_loan_entry].interest_rate, &loan_info[count_loan_entry].amount) == 6)
    {
        count_loan_entry++;
    }

    fclose(fp1);

    int user_found = 0, user_index;

    for (int i = 0; i < count_loan_entry; ++i)
    {
        if (strcmp(username, loan_info[i].Username) == 0)
        {
            user_found = 1;
            user_index = i;
            break;
        }
    }

    int menuchoice;

    printf("\n1. Apply for a loan\n");
    printf("2. View Loan Details\n");
    printf("3. Make Loan Payment\n");
    printf("4. Modify Loan Terms\n\n");
    printf("Select your option: ");
    scanf("%d", &menuchoice);
    printf("\n");

    if (menuchoice == 1)
    {
        if (user_found == 1)
        {
            printf("You have already taken a loan\n\n");
            return;
        }
        else
        {
            printf("Select against which account you want to take loan\n");

            int *matchingAccounts = malloc(count * sizeof(int));
            int ACChoice;
            int found = MatchAndShow(username, account_info, count, matchingAccounts);
            if (found == 0)
            {
                printf("No accounts found \n\n");
                free(matchingAccounts);
            }
            else
            {
                printf("Choose the corresponding Account (1/2/3...): ");
                scanf("%d", &ACChoice);
                int index = matchingAccounts[ACChoice - 1];

                long long loan_amount;
                int loan_term, interesrrate;
                char date[20];
                printf("Enter the amount of loan: ");
                scanf("%lld", &loan_amount);
                printf("Enter the loan term (in months): ");
                scanf("%d", &loan_term);
                printf("Enter date(dd/mm/yyyy): ");
                scanf("%s", date);
                printf("\n");

                interesrrate = (rand() % (15 - 5 + 1)) + 5;

                printf("Your interest rate will be %d\n\n", interesrrate);

                loan_info[count_loan_entry].AccountNo = account_info[index].AccountNo;
                loan_info[count_loan_entry].loan_term = loan_term;
                loan_info[count_loan_entry].interest_rate = interesrrate;
                loan_info[count_loan_entry].amount = loan_amount;
                strcpy(loan_info[count_loan_entry].Username, username);
                strcpy(loan_info[count_loan_entry].date, date);

                count_loan_entry++;

                FILE *fp2;
                fp2 = fopen(LOAN_TEXT, "w");

                for (int i = 0; i < count_loan_entry; ++i)
                {
                    fprintf(fp2, "Username: %s\nAccount No: %lld\nLoan Term: %d\nDate: %s\nInterest Rate: %d\nAmount: %lld\n\n", loan_info[i].Username, loan_info[i].AccountNo, loan_info[i].loan_term, loan_info[i].date, loan_info[i].interest_rate, loan_info[i].amount);
                }

                fclose(fp2);

                int new_balance = account_info[index].Balance + loan_amount;
                int old_balance = account_info[index].Balance;
                account_info[index].Balance = new_balance;

                Transaction(account_info[index].AccountNo, date, "Loan", old_balance, loan_amount, new_balance);

                FILE *fp3 = fopen(ACCOUNT_DATA, "w");
                if (fp3 == NULL)
                {
                    printf("Error: Could not open file.\n");
                    return;
                }

                for (int i = 0; i < count; i++)
                {
                    fprintf(fp3, "Name: %s\nAccount Type: %s\nAccount No: %lld\nBalance: %lld\nPhone: %lld\nNID No: %lld\nUsername: %s\n\n",
                            account_info[i].Name, account_info[i].AccountType, account_info[i].AccountNo,
                            account_info[i].Balance, account_info[i].Phone, account_info[i].NID, account_info[i].Username);
                }

                fclose(fp3);

                printf("Your loan application has been received.\n\n");
            }
            free(matchingAccounts);
        }
    }

    else if (menuchoice == 2)
    {
        if (user_found == 0)
        {
            printf("No loan details found\n\n");
            return;
        }

        else
        {
            printf("Loan Amount: %lld\n", loan_info[user_index].amount);
            printf("Account No: %lld\n", loan_info[user_index].AccountNo);
            printf("Loan taken on: %s\n", loan_info[user_index].date);
            printf("Interest rate: %d%%\n", loan_info[user_index].interest_rate);
            printf("Loan Term: %d months\n\n", loan_info[user_index].loan_term);
        }
    }

    else if (menuchoice == 3)
    {
        if (user_found == 0)
        {
            printf("No loan details found\n\n");
            return;
        }

        else
        {
            printf("Your Loan Amount: %lld\n", loan_info[user_index].amount);
            printf("Your Interest rate: %d%%\n", loan_info[user_index].interest_rate);

            long long repay_amount = (loan_info[user_index].amount * (100 + loan_info[user_index].interest_rate)) / 100;
            printf("Your payable amount: %lld\n\n", repay_amount);

            printf("Do you want to proceed?\n\n");
            printf("1. Yes\n");
            printf("2. No\n\n");
            printf("Your choice: ");

            int repay_choice;
            scanf("%d", &repay_choice);

            if (repay_choice == 2)
                return;

            else if (repay_choice == 1)
            {
                int account_index;

                for (int i = 0; i < count; ++i)
                {
                    if (account_info[i].AccountNo == loan_info[user_index].AccountNo)
                    {
                        account_index = i;
                        break;
                    }
                }

                if (account_info[account_index].AccountNo < repay_amount)
                {
                    printf("Insufficient Amount\n\n");
                    return;
                }

                else
                {
                    long long new_balance = account_info[account_index].Balance - repay_amount;
                    long long old_balance = account_info[account_index].Balance;
                    account_info[account_index].Balance = new_balance;

                    char date[20];
                    printf("Enter Date(dd/mm/yyyy): ");
                    scanf("%s", date);

                    Transaction(account_info[account_index].AccountNo, date, "LoanPayment", old_balance, repay_amount, new_balance);

                    for (int j = user_index; j < count_loan_entry - 1; j++)
                    {
                        loan_info[j] = loan_info[j + 1];
                    }

                    FILE *fp;

                    fp = fopen(LOAN_TEXT, "w");

                    if (fp == NULL)
                    {
                        printf("Error: Could not open file\n");
                        return;
                    }

                    for (int i = 0; i < count_loan_entry - 1; i++)
                    {
                        fprintf(fp, "Username: %s\nAccount No: %lld\nLoan Term: %d\nDate: %s\nInterest Rate: %d\nAmount: %lld\n\n", loan_info[i].Username, loan_info[i].AccountNo, loan_info[i].loan_term, loan_info[i].date, loan_info[i].interest_rate, loan_info[i].amount);
                    }
                    fclose(fp);

                    FILE *fp3 = fopen(ACCOUNT_DATA, "w");
                    if (fp3 == NULL)
                    {
                        printf("Error: Could not open file.\n");
                        return;
                    }

                    for (int i = 0; i < count; i++)
                    {
                        fprintf(fp3, "Name: %s\nAccount Type: %s\nAccount No: %lld\nBalance: %lld\nPhone: %lld\nNID No: %lld\nUsername: %s\n\n",
                                account_info[i].Name, account_info[i].AccountType, account_info[i].AccountNo,
                                account_info[i].Balance, account_info[i].Phone, account_info[i].NID, account_info[i].Username);
                    }

                    fclose(fp3);

                    printf("\nLoan Payment Successful.\n\n");
                }
            }

            else
                printf("Invalid Choice\n\n");
        }
    }

    else if (menuchoice == 4)
    {
        if (user_found == 0)
        {
            printf("No loan history found\n\n");
            return;
        }
        printf("Enter your new term (in months): ");
        scanf("%d", &loan_info[user_index].loan_term);

        FILE *fp;

        fp = fopen(LOAN_TEXT, "w");

        if (fp == NULL)
        {
            printf("Error: Could not open file\n");
            return;
        }

        for (int i = 0; i < count_loan_entry; i++)
        {
            fprintf(fp, "Username: %s\nAccount No: %lld\nLoan Term: %d\nDate: %s\nInterest Rate: %d\nAmount: %lld\n\n", loan_info[i].Username, loan_info[i].AccountNo, loan_info[i].loan_term, loan_info[i].date, loan_info[i].interest_rate, loan_info[i].amount);
        }
        fclose(fp);

        printf("Loan Term Updated.\n\n");
    }

    else
    {
        printf("Invalid Choice\n\n");
        return;
    }

    free(loan_info);
}

void Feedback(char *username)
{
    printf("1. Contact\n");
    printf("2. E-mail\n");
    printf("3. Send feeback/Report an issue\n\n");

    printf("Your choice: ");
    int feedback_choice;
    scanf("%d", &feedback_choice);
    printf("\n");

    if (feedback_choice == 1)
    {
        printf("Customer Care (24/7): 16249\n");
        printf("Headquarters (Sun-Thu 10 am to 4 pm): 16249\n\n");
    }

    else if (feedback_choice == 2)
    {
        printf("General Inquiry: info@thebestbank.org\n");
        printf("Account Related Issues: accounts@thebestbank.org\n");
        printf("Card Related Issues: cards@thebestbank.org\n");
        printf("Remittance Related Inquiry: remittance@thebestbank.org\n");
        printf("Drop your CV: jobs@thebestbank.org\n");
        printf("Others: public-relations@thebestbank.org\n\n");
    }

    else if (feedback_choice == 3)
    {

        char *str = (char *)malloc(1000 * sizeof(char));

        printf("Write your feedback within 1000 words: ");
        scanf(" %[^\n]", str);
        printf("\n");

        FILE *fp = fopen(FEEDBACK, "a");
        if (fp == NULL)
        {
            printf("Error: Could not open file\n");
            return;
        }

        fprintf(fp, "Username: %s\nMessage: %s\n", username, str);

        free(str);
        fclose(fp);

        printf("Your feedback has been recorded.\n\n");
    }

    else
        printf("Invalid choice.\n\n");
}

void ShowLocation()
{
    char region[MAX_REGION_LENGTH];
    int count = 0, i, found = 0;
    printf("Enter Region: ");
    scanf("%s", region);
    printf("\n");

    LocationInfo *location_info = malloc(MAX_LOCATIONS * sizeof(LocationInfo));

    FILE *fp = fopen(LOCATION_INFO, "r");
    if (fp == NULL)
    {
        printf("Failed to open file.\n");
        return;
    }

    while (fscanf(fp, "Branch Name: %[^\n]\nAddress: %[^\n]\nRegion: %s\n", location_info[count].BranchName, location_info[count].Address, location_info[count].Region) == 3)
    {
        count++;
    }

    fclose(fp);

    for (i = 0; i < count; i++)
    {
        if (strcmp(location_info[i].Region, region) == 0)
        {
            printf("Branch Name: %s\nAddress: %s\nRegion: %s\n\n", location_info[i].BranchName, location_info[i].Address, location_info[i].Region);
            found++;
        }
    }

    if (found == 0)
    {
        printf("No Branches in the region.\n\n");
    }

    free(location_info);
}

void CreditCardMenu(char *username, int count, CreditCardInfo *credit_card_info)
{
    int menuchoice;

    printf("Credit Card Management:\n");
    printf("1. View Credit Card Details\n");
    printf("2. Apply for Credit Card\n");
    printf("3. Suspend Credit Card\n");
    printf("4. Go Back\n");
    printf("\n");
    printf("Choose your option: ");
    scanf("%d", &menuchoice);
    printf("\n");

    switch (menuchoice)
    {
    case 1:
        ViewCreditCardDetails(username, count, credit_card_info);
        break;

    case 2:
        ApplyForCreditCard(username, count, credit_card_info);
        break;

    case 3:
        RemoveCreditCard(username, count, credit_card_info);
        break;

    case 4:
        return;
        break;

    default:
        printf("Please enter a valid choice \n \n");
    }
}

void ApplyForCreditCard(char *username, int count, CreditCardInfo *credit_card_info)
{
    srand(time(NULL));
    char card_number[CARD_NUMBER_LENGTH + 1];
    for (int i = 0; i < CARD_NUMBER_LENGTH; i++)
    {
        card_number[i] = (rand() % 10) + '0';
    }
    card_number[CARD_NUMBER_LENGTH] = '\0';

    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    int current_year = current_time->tm_year % 100;
    int current_month = current_time->tm_mon + 1;
    int expiry_year = current_year + 2; // Expiry after 2 years
    int expiry_month = current_month;

    char cvv[CVV_LENGTH + 1];
    for (int i = 0; i < CVV_LENGTH; i++)
    {
        cvv[i] = (rand() % 10) + '0';
    }
    cvv[CVV_LENGTH] = '\0';

    printf("Enter Card Type: ");
    scanf(" %[^\n]", credit_card_info[count].CardType);
    printf("Enter the Card Limit you want: ");
    scanf("%lld", &credit_card_info[count].CardLimit);

    strcpy(credit_card_info[count].CardNo, card_number);
    sprintf(credit_card_info[count].ExpiryDate, "%02d/%02d", expiry_month, expiry_year);
    strcpy(credit_card_info[count].CVV, cvv);

    credit_card_info[count].AvailableBalance = credit_card_info[count].CardLimit;
    strcpy(credit_card_info[count].Username, username);

    printf("Credit card added successfully\n\n");

    printf("Card No: %s\n", credit_card_info[count].CardNo);
    printf("Card Type: %s\n", credit_card_info[count].CardType);
    printf("Expiry Date: %s\n", credit_card_info[count].ExpiryDate);
    printf("CVV: %s\n", credit_card_info[count].CVV);
    printf("Card Limit: %lld\n", credit_card_info[count].CardLimit);
    printf("Available Balance: %lld\n\n", credit_card_info[count].AvailableBalance);

    FILE *fp = fopen(CREDIT_CARD_INFO, "w");
    if (fp == NULL)
    {
        printf("Error: Could not open file\n");
        return;
    }

    (count)++;

    for (int i = 0; i < count; i++)
    {
        fprintf(fp, "Card No: %s\nCard Type: %s\nExpiry Date: %s\nCVV: %s\nCard Limit: %lld\nAvailable Balance: %lld\nUsername: %s\n\n", credit_card_info[i].CardNo, credit_card_info[i].CardType, credit_card_info[i].ExpiryDate, credit_card_info[i].CVV, credit_card_info[i].CardLimit, credit_card_info[i].AvailableBalance, credit_card_info[i].Username);
    }
    fclose(fp);
}

void ViewCreditCardDetails(char *username, int count, CreditCardInfo *credit_card_info)
{
    int *matchingcards;
    matchingcards = malloc(count * sizeof(int));
    int num_matching_cards = MatchAndShowCards(username, credit_card_info, count, matchingcards);

    if (num_matching_cards == 0)
    {
        printf("No credit cards found for the given username\n");
    }
    else
    {
        int card_choice = 0;

        printf("Choose a credit card to view more the details: ");
        scanf("%d", &card_choice);

        int index = matchingcards[card_choice - 1];

        if (card_choice > 0 && card_choice <= num_matching_cards)
        {
            printf("\nCard No: %s\nCard Type: %s\nExpiry Date: %s\nCVV: %s\nCard Limit: %lld\nAvailable Balance: %lld\n", credit_card_info[index].CardNo, credit_card_info[index].CardType, credit_card_info[index].ExpiryDate, credit_card_info[index].CVV, credit_card_info[index].CardLimit, credit_card_info[index].AvailableBalance);
        }
        else
        {
            printf("Invalid credit card choice\n");
        }
    }

    printf("\n");
    free(matchingcards);
}

void RemoveCreditCard(char *username, int count, CreditCardInfo *credit_card_info)
{
    int *matchingcards = malloc(count * sizeof(int));
    int num_matching_cards = MatchAndShowCards(username, credit_card_info, count, matchingcards);

    if (num_matching_cards == 0)
    {
        printf("No credit cards found for the given username\n");
    }
    else
    {
        int card_choice = 0;

        printf("Choose a credit card to remove: ");
        scanf("%d", &card_choice);

        if (card_choice > 0 && card_choice <= num_matching_cards)
        {
            int index = matchingcards[card_choice - 1];

            // Remove the selected credit card by shifting the remaining cards in the dynamic array
            for (int j = index; j < count - 1; j++)
            {
                credit_card_info[j] = credit_card_info[j + 1];
            }

            printf("Credit card removed successfully\n");
        }
        else
        {
            printf("Invalid credit card choice\n");
        }
        // Update creditcards file
        FILE *fp;

        fp = fopen(CREDIT_CARD_INFO, "w");

        if (fp == NULL)
        {
            printf("Error: Could not open file\n");
            return;
        }

        for (int i = 0; i < count - 1; i++)
        {
            fprintf(fp, "Card No: %s\nCard Type: %s\nExpiry Date: %s\nCVV: %s\nCard Limit: %lld\nAvailable Balance: %lld\nUsername: %s\n\n", credit_card_info[card_choice - 1].CardNo, credit_card_info[card_choice - 1].CardType, credit_card_info[card_choice - 1].ExpiryDate, credit_card_info[card_choice - 1].CVV, credit_card_info[card_choice - 1].CardLimit, credit_card_info[card_choice - 1].AvailableBalance, credit_card_info[card_choice - 1].Username);
        }
        fclose(fp);
    }

    printf("\n");
}

int MatchAndShowCards(char *username, CreditCardInfo *credit_card_info, int count, int *matchingcards)
{
    int num_matching_cards = 0;
    if (matchingcards == NULL)
    {
        printf("Could not allocate memory\n");
        return 0;
    }

    // Find and store the indices of credit cards matching the given username
    for (int i = 0; i < count; i++)
    {
        if (strcmp(credit_card_info[i].Username, username) == 0)
        {
            matchingcards[num_matching_cards] = i;
            num_matching_cards++;
        }
    }

    // Show the matching credit cards
    if (num_matching_cards > 0)
    {
        printf("Matching credit cards:\n");
        for (int i = 0; i < num_matching_cards; i++)
        {
            int cardindex = matchingcards[i];
            printf("%d. Card No: %s\n   Card Type: %s\n", i + 1, credit_card_info[cardindex].CardNo, credit_card_info[cardindex].CardType);
        }
        printf("\n");
    }

    return num_matching_cards;
}