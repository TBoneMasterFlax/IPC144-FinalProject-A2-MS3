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

void getTicket(struct Ticket* ticket, int highestNumber, const struct Account* account)
{
    printf("\nNew Ticket (Ticket#:%06d)\n", highestNumber);
    printf("----------------------------------------\n");
    ticket->unique_number = highestNumber;
    ticket->ticket_number = account->account_number;
    ticket->ticket_status = 1;
    ticket->counter = 0;
    printf("Enter the ticket SUBJECT (30 chars. maximum): ");
    getCString(ticket->subject_line, 1, 30);

    ticket->messages[ticket->counter].account_type = account->account_type;
    strcpy(ticket->messages[ticket->counter].display_name, account->user_login.display_name);
    printf("\nEnter the ticket message details (150 chars. maximum). Press the ENTER key to\nsubmit: \n");
    getCString(ticket->messages[ticket->counter].message, 1, 150);
    ticket->counter++;
}

void updateTicketAsCustomer(struct Ticket* ticket, const struct Account* account)
{
    int selection, flag = 1, i = 0;
    char removeTicket, partingMessage;
    do
    {
        char ticketStatus[] = "ACTIVE";
        if (ticket->ticket_status == 0)
        {
            strcpy(ticketStatus, "CLOSED");
        }
        printf("\n----------------------------------------\n");
        printf("Ticket %06d - Update Options\n", ticket->unique_number);
        printf("----------------------------------------\n");
        printf("Status  : %s\n", ticketStatus);
        printf("Subject : %s\n", ticket->subject_line);
        printf("----------------------------------------\n");
        printf("1) Modify the subject\n");
        printf("2) Add a message\n");
        printf("3) Close ticket\n");
        printf("0) Done\n");
        printf("Selection: ");
        selection = getIntFromRange(0, 3);
        switch (selection)
        {
        case 0:
            putchar('\n');
            pauseExecution();
            flag = 0;
            break;
        case 1:
            printf("\nEnter the revised ticket SUBJECT (30 chars. maximum): ");
            getCString(ticket->subject_line, 1, 30);
            break;
        case 2:
            if (ticket->ticket_status == 1)
            {
                if (ticket->counter < 20)
                {
                    ticket->messages[ticket->counter].account_type = account->account_type;
                    strcpy(ticket->messages[ticket->counter].display_name, account->user_login.display_name);
                    printf("\nEnter the ticket message details (150 chars. maximum). Press the ENTER key to\nsubmit: \n");
                    getCString(ticket->messages[ticket->counter].message, 1, 150);
                    ticket->counter++;
                }
                else
                {
                    printf("\nERROR: Message limit has been reached, call ITS Support!\n");
                }
            }
            else
            {
                printf("\nERROR: Ticket is closed - new messages are not permitted.\n\n");
            }
            break;
        case 3:
            if (ticket->ticket_status == 1)
            {
                printf("\nAre you sure you CLOSE this ticket? ([Y]es|[N]o): ");
                removeTicket = getCharOption("YN");
                if (removeTicket == 'Y')
                {
                    ticket->ticket_status = 0;
                    if (ticket->counter < 20)
                    {
                        printf("\nDo you want to leave a closing message? ([Y]es|[N]o): ");
                        partingMessage = getCharOption("YN");
                        if (partingMessage == 'Y')
                        {
                            ticket->messages[ticket->counter].account_type = account->account_type;
                            strcpy(ticket->messages[ticket->counter].display_name, account->user_login.display_name);
                            printf("\nEnter the ticket message details (150 chars. maximum). Press the ENTER key to\nsubmit: \n");
                            getCString(ticket->messages[ticket->counter].message, 1, 150);
                            ticket->counter++;
                        }
                    }
                    flag = 0;
                    printf("\n*** Ticket closed! ***\n\n");
                    pauseExecution();
                }
                else
                {
                    printf("\n*** No changes made! ***\n\n");
                    pauseExecution();
                }
            }
            else
            {
                printf("\nERROR: Ticket is already closed!\n\n");
            }
            break;
        default:
            break;
        }
    } while (flag);
}

void updateTicketAsAgent(struct Ticket* ticket, const struct Account* account)
{
    int selection, flag = 1, i = 0;
    char removeTicket, partingMessage, openTicket;
    do
    {
        char ticketStatus[] = "ACTIVE";
        if (ticket->ticket_status == 0)
        {
            strcpy(ticketStatus, "CLOSED");
        }
        printf("----------------------------------------\n");
        printf("Ticket %06d - Update Options\n", ticket->unique_number);
        printf("----------------------------------------\n");
        printf("Status  : %s\n", ticketStatus);
        printf("Subject : %s\n", ticket->subject_line);
        printf("Acct#   : %d\n", ticket->ticket_number);
        printf("Customer: %s\n", ticket->messages->display_name);
        printf("----------------------------------------\n");
        printf("1) Add a message\n");
        printf("2) Close ticket\n");
        printf("3) Re-open ticket\n");
        printf("0) Done\n");
        printf("Selection: ");
        selection = getIntFromRange(0, 3);
        switch (selection)
        {
        case 0:
            putchar('\n');
            flag = 0;
            break;
        case 1:
            if (ticket->ticket_status == 1)
            {
                if (ticket->counter < 20)
                {
                    ticket->messages[ticket->counter].account_type = account->account_type;
                    strcpy(ticket->messages[ticket->counter].display_name, account->user_login.display_name);
                    printf("\nEnter the ticket message details (150 chars. maximum). Press the ENTER key to\nsubmit: \n");
                    getCString(ticket->messages[ticket->counter].message, 1, 150);
                    ticket->counter++;
                    putchar('\n');
                }
                else
                {
                    printf("\nERROR: Message limit has been reached, call ITS Support!\n\n");
                }
            }
            else
            {
                printf("\nERROR: Ticket is closed - new messages are not permitted.\n\n");
            }
            break;
        case 2:
            if (ticket->ticket_status == 1)
            {
                printf("\nAre you sure you CLOSE this ticket? ([Y]es|[N]o): ");
                removeTicket = getCharOption("YN");
                if (removeTicket == 'Y')
                {
                    ticket->ticket_status = 0;
                    if (ticket->counter < 20)
                    {
                        printf("\nDo you want to leave a closing message? ([Y]es|[N]o): ");
                        partingMessage = getCharOption("YN");
                        if (partingMessage == 'Y')
                        {
                            ticket->messages[ticket->counter].account_type = account->account_type;
                            strcpy(ticket->messages[ticket->counter].display_name, account->user_login.display_name);
                            printf("\nEnter the ticket message details (150 chars. maximum). Press the ENTER key to\nsubmit: \n");
                            getCString(ticket->messages[ticket->counter].message, 1, 150);
                            ticket->counter++;
                        }
                    }
                    printf("\n*** Ticket closed! ***\n\n");
                }
                else
                {
                    printf("\n*** No changes made! ***\n\n");
                    pauseExecution();
                }
            }
            else
            {
                printf("\nERROR: Ticket is already closed!\n\n");
            }
            break;
        case 3:
            putchar('\n');
            if (ticket->ticket_status == 0)
            {
                printf("Are you sure you RE-OPEN this closed ticket? ([Y]es|[N]o): ");
                openTicket = getCharOption("YN");
                if (openTicket == 'Y')
                {
                    ticket->ticket_status = 1;
                    printf("\n*** Ticket re-opened! ***\n");
                }
                putchar('\n');
            }
            else
            {
                printf("ERROR: Ticket is already active!\n\n");
            }
            break;
        default:
            break;
        }
    } while (flag);
}