/*
    ==================================================
    Assignment #2 Milestone #3:
    ==================================================
    Name   : Tuan Nguyen
    ID     : 109062166
    Email  : tnguyen140@myseneca.ca
    Section: NFF
*/

#ifndef TICKET_H_
#define TICKET_H_

struct Message
{
    char account_type;
    char display_name[31];
    char message[151];
};

struct Ticket
{
    int unique_number;
    int ticket_number;
    int ticket_status;
    char subject_line[31];
    int counter;
    struct Message messages[20];
};

void getTicket(struct Ticket* ticket, int highestNumber, const struct Account* account);
void updateTicketAsCustomer(struct Ticket* ticket, const struct Account* account);
void updateTicketAsAgent(struct Ticket* ticket, const struct Account* account);

#endif