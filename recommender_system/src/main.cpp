#include <iostream>
#include <fstream>
#include <string>

#include "mass_diffusion.h"
#include "heat_conduction.h"
#include "collaborative_filter.h"

//specify the number of users and items, then init the vector
//+ container of users and items.
int main(int argc, char **argv)
{
	user_container user_list;
	item_container item_list;

	--argc; ++argv;
	if (argc > 0 && **argv == '-' && (*argv)[1] == 'n')
	{
		--argc;
		++argv;
	}
	if (argc < 1)
	{
		std::cout << "Usage: data_analysis [-n] file." << std::endl;
		return -1;
	}

	std::ifstream fin(*argv);
	if (!fin.is_open())
	{
		std::cout << "Error: file not found." << std::endl;
		exit(0);
	}

	std::cout << "Info: reading the file..." << std::endl;

	while (!fin.eof())
	{
		std::string temp;
		std::getline(fin, temp);

		int pivot = temp.find(',');
		if (pivot < 0)
		{
			std::cout << "Warning: an unexpected blank line." << std::endl;
			continue;
		}

		_container_number_ user_n = std::stoi(temp.substr(0, pivot));
		if (user_n + 1 > user_list.size())
			user_list.resize(user_n + 1);

		_container_number_ item_n = stoi(temp.substr(pivot + 1, temp.size()));
		if (item_n + 1 > item_list.size())
			item_list.resize(item_n + 1);

		relation *_relation = new relation;
		_relation->similar_fan = NULL;
		_relation->similar_interest = NULL;

		if (user_list[user_n] == NULL)
		{
			user_list[user_n] = new user;
			user_list[user_n]->user_index = user_n;
			user_list[user_n]->interest_number = 0;
			user_list[user_n]->interest_list = _relation;
		}
		else
		{
			relation *_last = NULL;
			relation *_cursor = user_list[user_n]->interest_list;
			while (true)
			{
				if ((_cursor == NULL) && (_cursor->interest->item_index < item_n)) {
					_last = _cursor;
					_cursor = _cursor->similar_fan;
				}
				else {
					if (_last == NULL)
						user_list[user_n]->interest_list = _relation;
					else
						_last->similar_fan = _relation;
					_relation->similar_fan = _cursor;
					break;
				}
			}
		}

		if (item_list[item_n] == NULL)
		{
			item_list[item_n] = new item;
			item_list[item_n]->item_index = item_n;
			item_list[item_n]->fan_number = 0;
			item_list[item_n]->fan_list = _relation;
		}
		else
		{
			relation *_last = NULL;
			relation *_cursor = item_list[item_n]->fan_list;
			while (true)
			{
				if ((_cursor == NULL) && (_cursor->fan->user_index < user_n)) {
					_last = _cursor;
					_cursor = _cursor->similar_interest;
				}
				else {
					if (_last == NULL)
						item_list[item_n]->fan_list = _relation;
					else
						_last->similar_interest = _relation;
					_relation->similar_interest = _cursor;
					break;
				}
			}
		}

		_relation->fan = user_list[user_n];
		_relation->interest = item_list[item_n];
	}

	for (_container_number_ i = 0; i < user_list.size(); i++)
	{
		if (user_list[i] != NULL)
		{
			relation *_relation = (user_list[i])->interest_list;

			if (_relation == NULL)
				continue;

			_container_number_ count = 0;
			do
				count++;
			while ((_relation->similar_fan != NULL) && (_relation = _relation->similar_fan));
			(user_list[i])->interest_number = count;
		}
	}

	for (_container_number_ i = 0; i < item_list.size(); i++)
	{
		if (item_list[i] != NULL)
		{
			relation *_relation = (item_list[i])->fan_list;

			if (_relation == NULL)
				continue;

			_container_number_ count = 0;
			do
				count++;
			while ((_relation->similar_interest != NULL) && (_relation = _relation->similar_interest));
			(item_list[i])->fan_number = count;
		}
	}

	init(&user_list, &item_list);

	return 0;
}

void init(user_container* _ulist, item_container* _ilist)
{

	for (_container_number_ i = 0; i < _ulist->size(); i++)
		if ((*_ulist)[i] != NULL)
			(*_ulist)[i]->user_score = 0;

	for (_container_number_ i = 0; i < _ilist->size(); i++)
		if ((*_ilist)[i] != NULL)
			(*_ilist)[i]->item_score = 0;

	std::cout << "Info: Enter the number of user to be analyzed"
		<< "(1 ~ " << _ulist->size() - 1 << "):" << std::endl;

	_container_number_ query_user;
	while (true)
	{
		std::cin >> query_user;
		if ((*_ulist)[query_user] == NULL)
			std::cout << "Error: user not found. Retry:" << std::endl;
		else if (query_user >= _ulist->size())
			std::cout << "Error: number overflowed. Retry:" << std::endl;
		else
			break;
	};

	original_item_container original_item_list;

	relation* _relation = _relation = (*_ulist)[query_user]->interest_list;
	while (_relation != NULL)
	{
		original_item_list.push_back(_relation->interest);

		_relation->interest->item_score = 1048576;
		_relation = _relation->similar_fan;
	}

	std::cout << "Info: Enter the serial number of the algorithm:\n"
		<< "1: mass diffustion\n"
		<< "2: heat conduction\n"
		<< "3: collaborative filter" << std::endl;

	_container_number_ compute_type;
	std::cin >> compute_type;
	switch (compute_type)
	{
	case 1:
		mass_diffusion(_ulist, _ilist);
		break;
	case 2:
		heat_conduction(_ulist, _ilist);
		break;
	case 3:
		collaborative_filter(_ulist, _ilist);
		break;
	}

	original_item_container::iterator it1;
	for (it1 = original_item_list.begin(); it1 != original_item_list.end(); it1++)
		(*it1)->item_score = 0;

	scored_item_container scored_item_map;
	scored_item_container::iterator it2;

	//sort by map
	for (_container_number_ i = 0; i < _ilist->size(); i++)
		if (((*_ilist)[i] != NULL) && ((*_ilist)[i]->item_score != 0))
			scored_item_map.insert(scored_item_pair((*_ilist)[i]->item_score, (*_ilist)[i]->item_index));

	for (it2 = scored_item_map.begin(); it2 != scored_item_map.end(); it2++)
		std::cout << (*it2).first << ':' << (*it2).second << std::endl;

}