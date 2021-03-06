#ifndef HEAT_CONDUCTION_H
#define HEAT_CONDUCTION_H

#include "common.h"

//1.compute the socres of the users and then the score of
//+ the items
extern void heat_conduction(user_container*, item_container*);

//2.As the score of each item defined, set the score of user
//+ accordingly.
static void score_user(user_container*, item_container*);

//3.As the score of each user defined, set the score of item
//+ accordingly.
static void score_item(user_container*, item_container*);

#endif
