/*
    ==================================================
    Assignment #2 Milestone #3:
    ==================================================
    Name   : Tuan Nguyen
    ID     : 109062166
    Email  : tnguyen140@myseneca.ca
    Section: NFF
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "commonHelpers.h"
#include "account.h"
#include "accountTicketingUI.h"
#include "ticket.h"
#include <string.h>
#include <stdlib.h>

void displayAccountSummaryHeader(void)
{
    printf("Acct# Acct.Type Birth\n");
    printf("----- --------- -----\n");
}

void displayAccountSummaryRecord(const struct Account* account)
{
    char accountType[] = "CUSTOMER";
    if (account->account_type == 'A')
    {
        strcpy(accountType, "AGENT");
    }
    printf("%05d %-9s %5d\n", account->account_number, accountType, account->demographic.birth_year);
}

void displayAllAccountSummaryRecords(const struct Account accounts[], int size)
{
    int i = 0;
    displayAccountSummaryHeader();
    for (i = 0; i < size; i++)
    {
        if (accounts[i].account_number > 0)
        {
            displayAccountSummaryRecord(&accounts[i]);
        }
    }
}

void displayAccountDetailHeader(void)
{
    printf("Acct# Acct.Type Birth Income      Country    Disp.Name       Login      Password\n");
    printf("----- --------- ----- ----------- ---------- --------------- ---------- --------\n");
}

void displayAccountDetailRecord(const struct Account* account)
{
    char accountType[] = "CUSTOMER";
    if (account->account_type == 'A')
    {
        strcpy(accountType, "AGENT");
    }
    char hiddenPassword[9];
    strcpy(hiddenPassword, account->user_login.password);
    int passwordLength = strlen(hiddenPassword);
    int i;
    for (i = 0; i < passwordLength; i++)
    {
        if (i % 2 != 0)
        {
            hiddenPassword[i] = '*';
        }
    }
    printf("%05d %-9s %5d $%10.2lf %-10s %-15s %-10s %8s\n", account->account_number, accountType, account->demographic.birth_year, account->demographic.income, account->demographic.country, account->user_login.display_name, account->user_login.login_name, hiddenPassword);
}

void displayAllAccountDetailRecords(const struct Account accounts[], int size)
{
    int i = 0;
    displayAccountDetailHeader();
    for (i = 0; i < size; i++)
    {
        if (accounts[i].account_number > 0)
        {
            displayAccountDetailRecord(&accounts[i]);
        }
    }
}

void displayAgentTicketHeader(void)
{
    printf("------ ----- --------------- ------ ------------------------------ --------\n");
    printf("Ticket Acct# Display Name    Status Subject                        Messages\n");
    printf("------ ----- --------------- ------ ------------------------------ --------\n");
}

void displayAgentTicketRecord(const struct Ticket* tickets)
{
    char ticketStatus[] = "ACTIVE";
    if (tickets->ticket_status == 0)
    {
        strcpy(ticketStatus, "CLOSED");
    }
    printf("%06d %5d %-15s %-6s %-30s %5d\n", tickets->unique_number, tickets->ticket_number, tickets->messages->display_name, ticketStatus, tickets->subject_line, tickets->counter);
}

void displayAgentTicketEnder(void)
{
    printf("------ ----- --------------- ------ ------------------------------ --------\n");
}

void displayCustomerTicketHeader(void)
{
    printf("------ ------ ------------------------------ --------\n");
    printf("Ticket Status Subject                        Messages\n");
    printf("------ ------ ------------------------------ --------\n");
}

void displayCustomerTicketRecord(const struct Ticket* tickets)
{
    char ticketStatus[] = "ACTIVE";
    if (tickets->ticket_status == 0)
    {
        strcpy(ticketStatus, "CLOSED");
    }
    printf("%06d %-6s %-30s %5d\n", tickets->unique_number, ticketStatus, tickets->subject_line, tickets->counter);
}

void displayCustomerTicketEnder(void)
{
    printf("------ ------ ------------------------------ --------\n");
}

void applicationStartup(struct AccountTicketingData* accountsAndTickets)
{
    int userIndex = -1;
    int flag = 1;
    do
    {
        userIndex = menuLogin(accountsAndTickets);
        if (userIndex < 0)
        {
            flag = 0;
        }
    } while (flag);
}

int menuLogin(struct AccountTicketingData* accountsAndTickets)
{
    int userIndex;
    int selection, flag = 1;
    char exit;
    do
    {
        userIndex = -1;
        printf("==============================================\n");
        printf("Account Ticketing System - Login\n");
        printf("==============================================\n");
        printf("1) Login to the system\n");
        printf("0) Exit application\n");
        printf("----------------------------------------------\n\n");
        printf("Selection: ");
        selection = getIntFromRange(0, 1);
        switch (selection)
        {
        case 0:
            printf("\nAre you sure you want to exit? ([Y]es|[N]o): ");
            exit = getCharOption("yYnN");
            putchar('\n');
            if (exit == 'y' || exit == 'Y')
            {
                printf("==============================================\n");
                printf("Account Ticketing System - Terminated\n");
                printf("==============================================\n");
                flag = 0;
            }
            break;
        case 1:
            userIndex = loginValidation(accountsAndTickets);
            if (userIndex < 0)
            {
                printf("\nERROR:  Login failed!\n\n");
                pauseExecution();
            }
            else
            {
                if (accountsAndTickets->accounts[userIndex].account_type == 'A')
                {
                    putchar('\n');
                    menuAgent(accountsAndTickets, &accountsAndTickets->accounts[userIndex]);
                }
                else
                {
                    putchar('\n');
                    menuCustomer(accountsAndTickets, &accountsAndTickets->accounts[userIndex]);
                }
                flag = 0;
            }
            break;
        default:
            break;
        }
    } while (flag);
    return userIndex;
}

void menuAgent(struct AccountTicketingData* accountsAndTickets, const struct Account* userAccount)
{
    const int userAccountNumber = userAccount->account_number;
    int selection, flag = 1, i = 0;
    do
    {
        int emptyAccountIndex = -1, accountIndex = -1, ticketIndex = -1;
        int accountNumber = 0, ticketNumber = 0, highestNumber = 0;
        char removeAccount;
        printf("AGENT: %s (%d)\n", userAccount->user_login.display_name, userAccount->account_number);
        printf("==============================================\n");
        printf("Account Ticketing System - Agent Menu\n");
        printf("==============================================\n");
        printf(" 1) Add a new account\n");
        printf(" 2) Modify an existing account\n");
        printf(" 3) Remove an account\n");
        printf(" 4) List accounts: summary view\n");
        printf(" 5) List accounts: detailed view\n");
        printf("----------------------------------------------\n");
        printf(" 6) List new tickets\n");
        printf(" 7) List active tickets\n");
        printf(" 8) List closed tickets\n");
        printf(" 9) Manage a ticket\n");
        printf("10) Archive closed tickets\n");
        printf("----------------------------------------------\n");
        printf("0) Logout\n\n");
        printf("Selection: ");
        selection = getIntFromRange(0, 9);
        switch (selection)
        {
        case 0:
            printf("\n### LOGGED OUT ###\n\n");
            flag = 0;
            break;
        case 1:
            for (i = 0; i < accountsAndTickets->ACCOUNT_MAX_SIZE; i++)
            {
                if (accountsAndTickets->accounts[i].account_number <= 0)
                {
                    emptyAccountIndex = i;
                    break;
                }
            }
            if (emptyAccountIndex < 0)
            {
                printf("\nERROR: Account listing is FULL, call ITS Support!\n\n");
                pauseExecution();
            }
            else
            {
                for (i = 0; i < accountsAndTickets->ACCOUNT_MAX_SIZE; i++)
                {
                    if (highestNumber <= accountsAndTickets->accounts[i].account_number)
                    {
                        highestNumber = accountsAndTickets->accounts[i].account_number + 1;
                    }
                }
                putchar('\n');
                getAccount(&accountsAndTickets->accounts[emptyAccountIndex], highestNumber);
                getUserLogin(&accountsAndTickets->accounts[emptyAccountIndex].user_login);
                getDemographic(&accountsAndTickets->accounts[emptyAccountIndex].demographic);
                printf("*** New account added! ***\n\n");
                pauseExecution();
            }
            break;
        case 2:
            printf("\nEnter your account#: ");
            scanf("%d", &accountNumber);
            clearStandardInputBuffer();
            for (i = 0; i < accountsAndTickets->ACCOUNT_MAX_SIZE; i++)
            {
                if (accountNumber == accountsAndTickets->accounts[i].account_number)
                {
                    accountIndex = i;
                }
            }
            if (accountIndex < 0)
            {
                printf("\nERROR:  Access Denied.\n\n");
                pauseExecution();
            }
            else
            {
                updateAccount(&accountsAndTickets->accounts[accountIndex]);
            }
            putchar('\n');
            break;
        case 3:
            printf("\nEnter your account#: ");
            scanf("%d", &accountNumber);
            clearStandardInputBuffer();
            putchar('\n');
            for (i = 0; i < accountsAndTickets->ACCOUNT_MAX_SIZE; i++)
            {
                if (accountNumber == accountsAndTickets->accounts[i].account_number)
                {
                    accountIndex = i;
                }
            }
            if (accountIndex < 0)
            {
                printf("\nERROR:  Access Denied.\n\n");
                pauseExecution();
            }
            else
            {
                if (accountsAndTickets->accounts[accountIndex].account_number == userAccount->account_number)
                {
                    printf("\nERROR: You can't remove your own account!\n\n");
                    pauseExecution();
                }
                else
                {
                    displayAccountDetailHeader();
                    displayAccountDetailRecord(&accountsAndTickets->accounts[accountIndex]);
                    printf("\nAre you sure you want to remove this record? ([Y]es|[N]o): ");
                    removeAccount = getCharOption("YN");
                    if (removeAccount == 'Y')
                    {
                        //Set the targeted Account's account number to 0, effectively removing the account
                        accountsAndTickets->accounts[accountIndex].account_number = 0;
                        //Start removing the targeted account's related Tickets
                        for (i = 0; i < accountsAndTickets->TICKET_MAX_SIZE; i++)
                        {
                            if (accountNumber == accountsAndTickets->tickets[i].ticket_number)
                            {
                                //If a related Ticket's status is ACTIVE,
                                if (accountsAndTickets->tickets[i].ticket_status == 1)
                                {
                                    //Set the related Ticket's unique_number and ticket_number to 0, effectively removing the ticket
                                    accountsAndTickets->tickets[i].ticket_number = 0;
                                    accountsAndTickets->tickets[i].unique_number = 0;
                                }
                            }
                        }
                        printf("\n*** Account Removed! ***\n\n");
                        pauseExecution();
                    }
                    else
                    {
                        printf("\n*** No changes made! ***\n\n");
                        pauseExecution();
                    }
                }
            }
            break;
        case 4:
            putchar('\n');
            displayAllAccountSummaryRecords(accountsAndTickets->accounts, accountsAndTickets->ACCOUNT_MAX_SIZE);
            putchar('\n');
            pauseExecution();
            break;
        case 5:
            putchar('\n');
            displayAllAccountDetailRecords(accountsAndTickets->accounts, accountsAndTickets->ACCOUNT_MAX_SIZE);
            putchar('\n');
            pauseExecution();
            break;
        case 6:
            putchar('\n');
            viewNewTickets(accountsAndTickets,  userAccount);
            putchar('\n');
            break;
        case 7:
            putchar('\n');
            viewActiveTickets(accountsAndTickets, userAccount);
            putchar('\n');
            break;
        case 8:
            putchar('\n');
            viewClosedTickets(accountsAndTickets, userAccount);
            putchar('\n');
            break;
        case 9:
            putchar('\n');
            printf("Enter ticket number: ");
            scanf("%d", &ticketNumber);
            clearStandardInputBuffer();
            putchar('\n');
            for (i = 0; i < accountsAndTickets->TICKET_MAX_SIZE; i++)
            {
                if (ticketNumber == accountsAndTickets->tickets[i].unique_number)
                {
                    ticketIndex = i;
                    break;
                }
            }
            if (ticketIndex > -1)
            {
                updateTicketAsAgent(&accountsAndTickets->tickets[ticketIndex], userAccount);
            }
            else
            {
                printf("ERROR: Invalid ticket number - you may only modify your own ticket.\n\n");
                pauseExecution();
            }
            break;
        case 10:
            printf("\nFeature #%d currently unavailable!\n\n", selection);
            pauseExecution();
            break;
        default:
            break;
        }
    } while (flag);
}

void menuCustomer(struct AccountTicketingData* accountsAndTickets, const struct Account* userAccount)
{
    int selection, flag = 1;
    int i = 0, j = 0;
    do
    {
        int emptyTicketIndex = -1, ticketIndex = -1;
        int ticketNumber = 0, highestNumber = 0, ownTicket = 1;
        printf("CUSTOMER: %s (%d)\n", userAccount->user_login.display_name, userAccount->account_number);
        printf("==============================================\n");
        printf("Customer Main Menu\n");
        printf("==============================================\n");
        printf("1) View your account detail\n");
        printf("2) Create a new ticket\n");
        printf("3) Modify an active ticket\n");
        printf("4) List my tickets\n");
        printf("----------------------------------------------\n");
        printf("0) Logout\n\n");
        printf("Selection: ");
        selection = getIntFromRange(0, 4);
        switch (selection)
        {
        case 0:
            printf("\n### LOGGED OUT ###\n\n");
            flag = 0;
            break;
        case 1:
            putchar('\n');
            displayAccountDetailHeader();
            displayAccountDetailRecord(userAccount);
            putchar('\n');
            pauseExecution();
            break;
        case 2:
            for (i = 0; i < accountsAndTickets->TICKET_MAX_SIZE; i++)
            {
                if (accountsAndTickets->tickets[i].unique_number <= 0)
                {
                    emptyTicketIndex = i;
                    break;
                }
                else
                {
                    emptyTicketIndex = -1;
                }
            }
            if (emptyTicketIndex < 0)
            {
                printf("\nERROR: Ticket listing is FULL, call ITS Support!\n\n");
                pauseExecution();
            }
            else
            {
                for (i = 0; i < accountsAndTickets->TICKET_MAX_SIZE; i++)
                {
                    if (highestNumber < accountsAndTickets->tickets[i].unique_number)
                    {
                        highestNumber = accountsAndTickets->tickets[i].unique_number + 1;
                    }
                }
                getTicket(&accountsAndTickets->tickets[emptyTicketIndex], highestNumber, userAccount);
                printf("\n*** New ticket created! ***\n\n");
                pauseExecution();
            }
            break;
        case 3:
            putchar('\n');
            printf("Enter ticket number: ");
            scanf("%d", &ticketNumber);
            clearStandardInputBuffer();
            for (i = 0; i < accountsAndTickets->TICKET_MAX_SIZE; i++)
            {
                if (ticketNumber == accountsAndTickets->tickets[i].unique_number)
                {
                    ticketIndex = i;
                    for (j = 0; j < accountsAndTickets->tickets[i].counter; j++)
                    {
                        if (strcmp(accountsAndTickets->tickets[i].messages[j].display_name, userAccount->user_login.display_name) == 0)
                        {
                            ownTicket = 0;
                        }
                    }
                    break;
                }
            }
            if (accountsAndTickets->tickets[ticketIndex].ticket_status == 1)
            {
                if ((ticketIndex > -1) && (ownTicket == 0))
                {
                    updateTicketAsCustomer(&accountsAndTickets->tickets[ticketIndex], userAccount);
                }
                else if (ticketIndex > -1)
                {
                    printf("\nERROR: Invalid ticket number - you may only modify your own ticket.\n\n");
                    pauseExecution();
                }
                else
                {
                    printf("\nERROR: Invalid ticket number.\n\n");
                    pauseExecution();
                }
            }
            else
            {
                printf("\nERROR: Ticket is closed - changes are not permitted.\n\n");
                pauseExecution();
            }
            break;
        case 4:
            putchar('\n');
            viewTickets(accountsAndTickets, userAccount);
            putchar('\n');
            break;
        default:
            break;
        }
    } while (flag);
}

int findAccountIndexByAcctNum(int accountNumber, const struct Account accounts[], int size, int prompt)
{
    return 0;
}

// Pause execution until user enters the enter key
void pauseExecution(void)
{
    printf("<< ENTER key to Continue... >>");
    clearStandardInputBuffer();
    putchar('\n');
}

int loginValidation(struct AccountTicketingData* accountsAndTickets)
{
    int attempts = 2, userIndex = -1;
    int accountNumber, flag = 1, i = 0;
    char userLoginName[30] = "\0";
    char password[30] = "\0";
    do
    {
        printf("\nEnter your account#: ");
        accountNumber = getPositiveInteger2();
        printf("User Login    : ");
        scanf("%30[^\n]", userLoginName);
        clearStandardInputBuffer();
        printf("Password      : ");
        scanf("%30[^\n]", password);
        clearStandardInputBuffer();
        for (i = 0; i < accountsAndTickets->ACCOUNT_MAX_SIZE; i++)
        {
            if (accountNumber == accountsAndTickets->accounts[i].account_number)
            {
                if (strcmp(userLoginName, accountsAndTickets->accounts[i].user_login.login_name) == 0)
                {
                    if (strcmp(password, accountsAndTickets->accounts[i].user_login.password) == 0)
                    {
                        userIndex = i;
                        flag = 0;
                    }
                }
            }
        }
        if (flag)
        {
            if (attempts >= 0)
            {
                printf("INVALID user login/password combination! [attempts remaining:%d]\n", attempts);
            }
            if (attempts == 0)
            {
                flag = 0;
            }
            else
            {
                attempts--;
            }
        }
    } while (flag);
    return userIndex;
}

void viewNewTickets(struct AccountTicketingData* accountsAndTickets, const struct Account* account)
{
    int flag = 1, i = 0;
    int ticketNumber, ticketIndex;
    do
    {
        ticketNumber = 0;
        ticketIndex = -1;

        displayAgentTicketHeader();
        for (i = 0; i < accountsAndTickets->TICKET_MAX_SIZE; i++)
        {
            if (accountsAndTickets->tickets[i].ticket_status == 1 && accountsAndTickets->tickets[i].counter == 1)
            {
                displayAgentTicketRecord(&accountsAndTickets->tickets[i]);
            }
        }
        displayAgentTicketEnder();
        putchar('\n');
        printf("Enter the ticket number to view the messages or\n0 to return to previous menu: ");
        ticketNumber = getPositiveIntegerOrZero();
        if (ticketNumber > 0)
        {
            for (i = 0; i < accountsAndTickets->TICKET_MAX_SIZE; i++)
            {
                if (ticketNumber == accountsAndTickets->tickets[i].unique_number)
                {
                    ticketIndex = i;
                    break;
                }
            }
            if (ticketIndex > -1)
            {
                char ticketStatus[] = "ACTIVE";
                if (accountsAndTickets->tickets[ticketIndex].ticket_status == 0)
                {
                    strcpy(ticketStatus, "CLOSED");
                }
                printf("\n================================================================================\n");
                printf("%06d (%-6s) Re: %s\n", accountsAndTickets->tickets[ticketIndex].unique_number, ticketStatus, accountsAndTickets->tickets[ticketIndex].subject_line);
                printf("================================================================================\n");
                for (i = 0; i < accountsAndTickets->tickets[ticketIndex].counter; i++)
                {
                    char messageType[] = "CUSTOMER";
                    if (accountsAndTickets->tickets[ticketIndex].messages[i].account_type == 'A')
                    {
                        strcpy(messageType, "AGENT");
                    }
                    printf("%s (%s):\n", messageType, accountsAndTickets->tickets[ticketIndex].messages[i].display_name);
                    printf("   %s\n", accountsAndTickets->tickets[ticketIndex].messages[i].message);
                }
                putchar('\n');
                pauseExecution();
            }
            else
            {
                printf("\nERROR: Invalid ticket number.\n\n");
                pauseExecution();
            }
        }
        else
        {
            flag = 0;
        }
    } while (flag);
}

void viewActiveTickets(struct AccountTicketingData* accountsAndTickets, const struct Account* account)
{
    int flag = 1, i = 0;
    int ticketNumber, ticketIndex;
    int counter;
    do
    {
        ticketNumber = 0;
        ticketIndex = -1;
        counter = 0;

        displayAgentTicketHeader();
        for (i = 0; i < accountsAndTickets->TICKET_MAX_SIZE; i++)
        {
            if ((accountsAndTickets->tickets[i].ticket_status == 1) && (accountsAndTickets->tickets[i].ticket_number > 0))
            {
                displayAgentTicketRecord(&accountsAndTickets->tickets[i]);
            }
        }
        displayAgentTicketEnder();
        putchar('\n');

        printf("Enter the ticket number to view the messages or\n0 to return to previous menu: ");
        ticketNumber = getPositiveIntegerOrZero();
        if (ticketNumber > 0)
        {
            for (i = 0; i < accountsAndTickets->TICKET_MAX_SIZE; i++)
            {
                if (ticketNumber == accountsAndTickets->tickets[i].unique_number)
                {
                    ticketIndex = i;
                    break;
                }
            }
            if (ticketIndex > -1)
            {
                counter = 0;
                char ticketStatus[] = "ACTIVE";
                if (accountsAndTickets->tickets[ticketIndex].ticket_status == 0)
                {
                    strcpy(ticketStatus, "CLOSED");
                }
                printf("\n================================================================================\n");
                printf("%06d (%-6s) Re: %s\n", accountsAndTickets->tickets[ticketIndex].unique_number, ticketStatus, accountsAndTickets->tickets[ticketIndex].subject_line);
                printf("================================================================================\n");
                for (i = 0; i < accountsAndTickets->tickets[ticketIndex].counter; i++)
                {
                    char messageType[] = "CUSTOMER";
                    if (accountsAndTickets->tickets[ticketIndex].messages[i].account_type == 'A')
                    {
                        strcpy(messageType, "AGENT");
                    }
                    printf("%s (%s):\n", messageType, accountsAndTickets->tickets[ticketIndex].messages[i].display_name);
                    printf("   %s\n\n", accountsAndTickets->tickets[ticketIndex].messages[i].message);
                    counter++;
                    if (counter % 5 == 0)
                    {
                        pauseExecution();
                    }
                }
                if (counter % 5 != 0)
                {
                    pauseExecution();
                }
            }
            else
            {
                printf("\nERROR: Invalid ticket number.\n\n");
                pauseExecution();
            }
        }
        else
        {
            flag = 0;
        }
    } while (flag);
}

void viewClosedTickets(struct AccountTicketingData* accountsAndTickets, const struct Account* account)
{
    int flag = 1, i = 0;
    int ticketNumber, ticketIndex;
    int counter;
    do
    {
        ticketNumber = 0;
        ticketIndex = -1;
        counter = 0;

        displayAgentTicketHeader();
        for (i = 0; i < accountsAndTickets->TICKET_MAX_SIZE; i++)
        {
            if ((accountsAndTickets->tickets[i].ticket_status == 0) && (accountsAndTickets->tickets[i].ticket_number > 0))
            {
                displayAgentTicketRecord(&accountsAndTickets->tickets[i]);
            }
        }
        displayAgentTicketEnder();
        putchar('\n');

        printf("Enter the ticket number to view the messages or\n0 to return to previous menu: ");
        ticketNumber = getPositiveIntegerOrZero();
        if (ticketNumber > 0)
        {
            for (i = 0; i < accountsAndTickets->TICKET_MAX_SIZE; i++)
            {
                if (ticketNumber == accountsAndTickets->tickets[i].unique_number)
                {
                    ticketIndex = i;
                    break;
                }
            }
            if (ticketIndex > -1)
            {
                counter = 0;
                char ticketStatus[] = "ACTIVE";
                if (accountsAndTickets->tickets[ticketIndex].ticket_status == 0)
                {
                    strcpy(ticketStatus, "CLOSED");
                }
                printf("\n================================================================================\n");
                printf("%06d (%-6s) Re: %s\n", accountsAndTickets->tickets[ticketIndex].unique_number, ticketStatus, accountsAndTickets->tickets[ticketIndex].subject_line);
                printf("================================================================================\n");
                for (i = 0; i < accountsAndTickets->tickets[ticketIndex].counter; i++)
                {
                    char messageType[] = "CUSTOMER";
                    if (accountsAndTickets->tickets[ticketIndex].messages[i].account_type == 'A')
                    {
                        strcpy(messageType, "AGENT");
                    }
                    printf("%s (%s):\n", messageType, accountsAndTickets->tickets[ticketIndex].messages[i].display_name);
                    printf("   %s\n\n", accountsAndTickets->tickets[ticketIndex].messages[i].message);
                    counter++;
                    if (counter % 5 == 0)
                    {
                        pauseExecution();
                    }
                }
                if (counter % 5 != 0)
                {
                    pauseExecution();
                }
            }
            else
            {
                printf("\nERROR: Invalid ticket number.\n\n");
                pauseExecution();
            }
        }
        else
        {
            flag = 0;
        }
    } while (flag);
}

void viewTickets(struct AccountTicketingData* accountsAndTickets, const struct Account* account)
{
    int flag = 1, i = 0;
    int ticketNumber, ticketIndex;
    int counter;
    do
    {
        ticketNumber = 0;
        ticketIndex = -1;
        counter = 0;

        displayCustomerTicketHeader();
        for (i = 0; i < accountsAndTickets->TICKET_MAX_SIZE; i++)
        {
            if (accountsAndTickets->tickets[i].ticket_number == account->account_number)
            {
                displayCustomerTicketRecord(&accountsAndTickets->tickets[i]);
            }
        }
        displayCustomerTicketEnder();
        putchar('\n');

        printf("Enter the ticket number to view the messages or\n0 to return to previous menu: ");
        ticketNumber = getPositiveIntegerOrZero();
        if (ticketNumber > 0)
        {
            for (i = 0; i < accountsAndTickets->TICKET_MAX_SIZE; i++)
            {
                if (accountsAndTickets->tickets[i].ticket_number == account->account_number)
                {
                    if (ticketNumber == accountsAndTickets->tickets[i].unique_number)
                    {
                        ticketIndex = i;
                        break;
                    }
                }
            }
            if (ticketIndex > -1)
            {
                counter = 0;
                char ticketStatus[] = "ACTIVE";
                if (accountsAndTickets->tickets[ticketIndex].ticket_status == 0)
                {
                    strcpy(ticketStatus, "CLOSED");
                }
                printf("\n================================================================================\n");
                printf("%06d (%-6s) Re: %s\n", accountsAndTickets->tickets[ticketIndex].unique_number, ticketStatus, accountsAndTickets->tickets[ticketIndex].subject_line);
                printf("================================================================================\n");
                for (i = 0; i < accountsAndTickets->tickets[ticketIndex].counter; i++)
                {
                    char messageType[] = "CUSTOMER";
                    if (accountsAndTickets->tickets[ticketIndex].messages[i].account_type == 'A')
                    {
                        strcpy(messageType, "AGENT");
                    }
                    printf("%s (%s):\n", messageType, accountsAndTickets->tickets[ticketIndex].messages[i].display_name);
                    printf("   %s\n\n", accountsAndTickets->tickets[ticketIndex].messages[i].message);
                    counter++;
                    if (counter % 5 == 0)
                    {
                        pauseExecution();
                    }
                }
                if (counter % 5 != 0)
                {
                    pauseExecution();
                }
            }
            else
            {
                printf("\nERROR: Invalid ticket number - you may only access your own tickets.\n\n");
                pauseExecution();
            }
        }
        else
        {
            flag = 0;
        }
    } while (flag);
}