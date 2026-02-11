#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILENAME "accounts.dat"
#define MAX_ACCOUNTS 100
#define MAX_NAME_LENGTH 50

typedef struct {
    int acc_no;
    char name[MAX_NAME_LENGTH];
    float balance;
} Account;

Account accounts[MAX_ACCOUNTS];
int account_count = 0;

// Function prototypes
void loadAccounts();
void saveAccounts();
void createAccount();
void deposit();
void withdraw();
void checkBalance();
void viewAllAccounts();
void clearInputBuffer();
int findAccount(int acc_no);

int main() {
    loadAccounts();
    
    int choice;
    
    while (1) {
        printf("\n===== Banking System Menu =====\n");
        printf("1. Create New Account\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Check Balance\n");
        printf("5. View All Accounts\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        
        switch (choice) {
            case 1: createAccount(); break;
            case 2: deposit(); break;
            case 3: withdraw(); break;
            case 4: checkBalance(); break;
            case 5: viewAllAccounts(); break;
            case 6: 
                saveAccounts();
                printf("Thank you for using our banking system!\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    
    return 0;
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void loadAccounts() {
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        return; // No existing file, start with empty accounts
    }
    
    account_count = fread(accounts, sizeof(Account), MAX_ACCOUNTS, file);
    fclose(file);
}

void saveAccounts() {
    FILE *file = fopen(FILENAME, "wb");
    if (file == NULL) {
        printf("Error saving accounts!\n");
        return;
    }
    
    fwrite(accounts, sizeof(Account), account_count, file);
    fclose(file);
}

int findAccount(int acc_no) {
    for (int i = 0; i < account_count; i++) {
        if (accounts[i].acc_no == acc_no) {
            return i;
        }
    }
    return -1; // Not found
}

void createAccount() {
    if (account_count >= MAX_ACCOUNTS) {
        printf("Maximum account limit reached!\n");
        return;
    }
    
    Account new_acc;
    
    printf("\n=== Create New Account ===\n");
    printf("Enter account number: ");
    if (scanf("%d", &new_acc.acc_no) != 1) {
        printf("Invalid account number.\n");
        clearInputBuffer();
        return;
    }
    
    if (findAccount(new_acc.acc_no) != -1) {
        printf("Account number already exists!\n");
        return;
    }
    
    clearInputBuffer();
    printf("Enter account holder name: ");
    fgets(new_acc.name, MAX_NAME_LENGTH, stdin);
    new_acc.name[strcspn(new_acc.name, "\n")] = '\0'; // Remove newline
    
    printf("Enter initial balance: ");
    if (scanf("%f", &new_acc.balance) != 1 || new_acc.balance < 0) {
        printf("Invalid balance amount.\n");
        clearInputBuffer();
        return;
    }
    
    accounts[account_count++] = new_acc;
    saveAccounts();
    printf("\nAccount created successfully!\n");
    printf("Account Number: %d\n", new_acc.acc_no);
    printf("Account Holder: %s\n", new_acc.name);
    printf("Initial Balance: %.2f\n", new_acc.balance);
}

void deposit() {
    if (account_count == 0) {
        printf("No accounts exist yet!\n");
        return;
    }
    
    int acc_no;
    float amount;
    
    printf("\n=== Deposit Money ===\n");
    printf("Enter account number: ");
    if (scanf("%d", &acc_no) != 1) {
        printf("Invalid account number.\n");
        clearInputBuffer();
        return;
    }
    
    int index = findAccount(acc_no);
    if (index == -1) {
        printf("Account not found!\n");
        return;
    }
    
    printf("Enter amount to deposit: ");
    if (scanf("%f", &amount) != 1 || amount <= 0) {
        printf("Invalid amount.\n");
        clearInputBuffer();
        return;
    }
    
    accounts[index].balance += amount;
    saveAccounts();
    printf("\nDeposit successful!\n");
    printf("New balance: %.2f\n", accounts[index].balance);
}

void withdraw() {
    if (account_count == 0) {
        printf("No accounts exist yet!\n");
        return;
    }
    
    int acc_no;
    float amount;
    
    printf("\n=== Withdraw Money ===\n");
    printf("Enter account number: ");
    if (scanf("%d", &acc_no) != 1) {
        printf("Invalid account number.\n");
        clearInputBuffer();
        return;
    }
    
    int index = findAccount(acc_no);
    if (index == -1) {
        printf("Account not found!\n");
        return;
    }
    
    printf("Enter amount to withdraw: ");
    if (scanf("%f", &amount) != 1 || amount <= 0) {
        printf("Invalid amount.\n");
        clearInputBuffer();
        return;
    }
    
    if (amount > accounts[index].balance) {
        printf("Insufficient balance!\n");
        return;
    }
    
    accounts[index].balance -= amount;
    saveAccounts();
    printf("\nWithdrawal successful!\n");
    printf("Remaining balance: %.2f\n", accounts[index].balance);
}

void checkBalance() {
    if (account_count == 0) {
        printf("No accounts exist yet!\n");
        return;
    }
    
    int acc_no;
    
    printf("\n=== Check Balance ===\n");
    printf("Enter account number: ");
    if (scanf("%d", &acc_no) != 1) {
        printf("Invalid account number.\n");
        clearInputBuffer();
        return;
    }
    
    int index = findAccount(acc_no);
    if (index == -1) {
        printf("Account not found!\n");
        return;
    }
    
    printf("\nAccount Holder: %s\n", accounts[index].name);
    printf("Account Balance: %.2f\n", accounts[index].balance);
}

void viewAllAccounts() {
    if (account_count == 0) {
        printf("No accounts exist yet!\n");
        return;
    }
    
    printf("\n=== All Accounts ===\n");
    printf("%-10s %-20s %-10s\n", "Acc No", "Name", "Balance");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < account_count; i++) {
        printf("%-10d %-20s %-10.2f\n", 
               accounts[i].acc_no, 
               accounts[i].name, 
               accounts[i].balance);
    }
}
