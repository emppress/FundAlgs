#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "my_string.h"

typedef struct Address
{
    String city, street, building, index;
    unsigned int house_number, apartment_number;
} Address;

typedef struct Mail
{
    Address address;
    double weight;
    String postal_index;
    String create_time, getting_time;
    char delivered;
} Mail;

typedef struct Post
{
    Mail *mail_arr;
    Address *address_ptr;
    size_t mail_capacity;
} Post;

void print_menu();
status get_address(Address *address);
status post_init(Post *post);
status add_mail(Post *post, int *count_mails);
status delete_mail(Post *post, int *count_mails);
status search_mail(Post *post, int *count_mails);
int compare_mails(const void *a, const void *b);
void print_mail_info(Mail *mail);
status get_postal_index(String *str);
status set_delivered(Post *post, int *count_mails);
status search_delivered(Post *post, int count_mails, int *count_delivered, Mail **arr_delivered);
void clear_address(Address *address);
void clear_mail(Mail *mail);
status search_overdue(Post *post, int count_mails, int *count_overdue, Mail **arr_overdue);
void clear_post(Post *post, int count_mails);
int compare_dates(const char *time_1, const char *time_2);

#endif