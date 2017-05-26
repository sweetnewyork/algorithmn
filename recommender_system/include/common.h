#pragma once
#ifndef _MAIN_H
#define _MAIN_H

#include<vector>
#include<list>
#include<map>
#include<set>

//1.declare the structs
struct user;
struct item;
struct relation;

//2.declare the containers
typedef unsigned int _container_number_;
typedef double _value_number_;

typedef std::set<item*> key_container;

typedef std::vector<user*> user_container;
typedef std::vector<item*> item_container;

typedef std::map<_value_number_, _container_number_> result_container;
typedef std::pair<_value_number_, _container_number_> result_pair;

typedef std::vector<std::set<_container_number_>> test_container;
typedef std::vector<_container_number_> roc_container;

//3.define a struct of user with a score and a list of his
//+ favorite things.
struct user
{
	_value_number_ user_score;
	_container_number_ user_index;
	_container_number_ interest_number;
	relation* interest_list;
};

//4.define a struct of item with a score and a list of its fans.
struct item
{
	_value_number_ item_score;
	_container_number_ item_index;
	_container_number_ fan_number;
	relation* fan_list;
};

//5.define a relationship between a user and a item which interests
//+ him, with the reference to the user, and the item. An instance
//+ of relation should be kept in duo linked list, which belongs to
//+ the user and the item respectively, at the same time.
struct relation
{
	user* fan;
	item* interest;
	relation* similar_fan;
	relation* similar_interest;
};

//6.declare the function, which reset the state of the network, zero
//+ being the socre of each person and each item.
void init(user_container*, item_container*);

//7.declare the function, which compute the roc of the result
void roc(_container_number_, result_container*);

//8.declare the function, which compute the inner-table similarity
void inner_similarity(user_container*, item_container*, result_container*);

#endif // !_MAIN_H
