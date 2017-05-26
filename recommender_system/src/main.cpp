#include <iostream>
#include <fstream>
#include <string>

#include <math.h>

#include "mass_diffusion.h"
#include "heat_conduction.h"
#include "collaborative_filter.h"

//specify the number of users and items, then init the vector
//+ container of users and items.
int main(int argc, char **argv)
{

	std::string t_file;
	std::cout << "Info: Enter the file name of the training set" << std::endl;
	std::cin >> t_file;

	user_container user_list;
	item_container item_list;

	std::ifstream fin1(t_file);
	if (!fin1.is_open())
	{
		std::cout << "Error: file not found." << std::endl;
		exit(0);
	}

	std::cout << "Info: reading the file..." << std::endl;

	while (!fin1.eof())
	{
		std::string temp;
		std::getline(fin1, temp);

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
			user_list[user_n]->user_score = 0;
			user_list[user_n]->interest_number = 1;
			user_list[user_n]->interest_list = _relation;
		}
		else
		{
			user_list[user_n]->interest_number++;

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
			item_list[item_n]->item_score = 0;
			item_list[item_n]->fan_number = 1;
			item_list[item_n]->fan_list = _relation;
		}
		else
		{
			item_list[item_n]->fan_number++;

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

	init(&user_list, &item_list);

	return 0;
}

void init(user_container* _ulist, item_container* _ilist)
{
	std::cout << "Info: Enter the number of user to be analyzed."
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

	key_container init_list;
	relation* _relation = _relation = (*_ulist)[query_user]->interest_list;

	while (_relation != NULL)
	{
		init_list.insert(_relation->interest);

		_relation->interest->item_score = 1;
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

	key_container::iterator it1;
	for (it1 = init_list.begin(); it1 != init_list.end(); it1++)
		(*it1)->item_score = 0;

	result_container result_map;

	//sort by map
	for (_container_number_ i = 0; i < _ilist->size(); i++)
		if (((*_ilist)[i] != NULL) && ((*_ilist)[i]->item_score != 0))
			result_map.insert(result_pair((*_ilist)[i]->item_score, (*_ilist)[i]->item_index));
	roc(query_user, &result_map);

	//auto it3 = result_map.begin();
	//for (int i = 0; i < 5; i++) {
	//	if (it3 == result_map.end())
	//		break;
	//	it3++;
	//}
	////result_container(result_map.begin(), it3)
	inner_similarity(_ulist, _ilist, &result_map);
}

void roc(_container_number_ query_user, result_container *result_map)
{
	test_container test_list;

	std::string testing_f;
	std::cout << "Info: Enter the file name of the testing set" << std::endl;
	std::cin >> testing_f;

	std::ifstream fin2(testing_f);
	if (!fin2.is_open())
	{
		std::cout << "Error: file not found." << std::endl;
		exit(0);
	}

	std::cout << "Info: reading the file..." << std::endl;

	while (!fin2.eof())
	{
		std::string temp;
		std::getline(fin2, temp);

		int pivot = temp.find(',');
		if (pivot < 0)
		{
			std::cout << "Warning: an unexpected blank line." << std::endl;
			continue;
		}

		_container_number_ user_n = std::stoi(temp.substr(0, pivot));
		_container_number_ item_n = stoi(temp.substr(pivot + 1, temp.size()));

		if (user_n + 1 > test_list.size())
			test_list.resize(user_n + 1);

		test_list[user_n].insert(item_n);
	}
	std::cout << "true:" << test_list[query_user].size() << std::endl;

	int true_positive = 0;
	roc_container roc_list;

	result_container::iterator it2;
	for (it2 = result_map->begin(); it2 != result_map->end(); it2++)
	{
		if (test_list[query_user].find((*it2).second) != test_list[query_user].end())
			roc_list.push_back(1);
		else
			roc_list.push_back(0);
	}

	for (_container_number_ i = 0; i < roc_list.size(); i++)
		std::cout << roc_list[i] << ',';

	std::cout << std::endl;
}

void inner_similarity(user_container* _ulist, item_container* _ilist, result_container *result_map)
{

	_value_number_ sum = 0.0;
	
	for (auto i = result_map->begin(); i != result_map->end(); i++) {
		auto j = i;
		while ((++j) != result_map->end())
		{
			_value_number_ similarity = 0;

			relation *_ir = (*_ilist)[(*i).second]->fan_list;
			relation *_jr = (*_ilist)[(*j).second]->fan_list;

			while (true)
			{
				if (_ir == NULL || _jr == NULL)
					break;

				if ((*_ir).fan->user_index > (*_jr).fan->user_index)
				{
					_jr = (*_jr).similar_interest;
				}
				else if ((*_ir).fan->user_index < (*_jr).fan->user_index)
				{
					_ir = (*_ir).similar_interest;
				}
				else
				{
					similarity += 1;
					_ir = (*_ir).similar_interest;
					_jr = (*_jr).similar_interest;
					continue;
				}
			}
			sum += similarity / (sqrt((*_ilist)[(*i).second]->fan_number) * sqrt((*_ilist)[(*j).second]->fan_number));
		}
	}
	int length = result_map->size();

	std::cout << (2 * sum / (length * (length - 1))) << std::endl;

}
