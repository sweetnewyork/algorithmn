#include <iostream>

#include "heat_conduction.h"

void heat_conduction(user_container* _ulist, item_container* _ilist)
{
	score_user(_ulist, _ilist);
	score_item(_ulist, _ilist);
}

void score_item(user_container* _ulist, item_container* _ilist)
{
	for (_container_number_ i = 0; i < _ilist->size(); i++)
		if ((*_ilist)[i] != NULL)
			(*_ilist)[i]->item_score = 0;

	for (_container_number_ i = 0; i < _ulist->size(); i++)
	{
		if (((*_ulist)[i] != NULL) && ((*_ulist)[i]->user_score > 0))
		{
			relation* _relation = (*_ulist)[i]->interest_list;

			if (_relation == NULL)
				continue;

			_value_number_ added_value = (*_ulist)[i]->user_score / (*_ulist)[i]->interest_number;

			do
			{
				_value_number_ added_value = (*_ulist)[i]->user_score / _relation->interest->fan_number;
				_relation->interest->item_score += added_value;
			}
			while ((_relation->similar_fan != NULL) && (_relation = _relation->similar_fan));
		}
	}
}

void score_user(user_container* _ulist, item_container* _ilist)
{
	for (_container_number_ i = 0; i < _ulist->size(); i++)
		if ((*_ulist)[i] != NULL)
			(*_ulist)[i]->user_score = 0;

	for (_container_number_ i = 0; i < _ilist->size(); i++)
	{
		if (((*_ilist)[i] != NULL) && ((*_ilist)[i]->item_score > 0))
		{
			relation* _relation = (*_ilist)[i]->fan_list;

			if (_relation == NULL)
				continue;

			do
			{
				_value_number_ added_value = (*_ilist)[i]->item_score / _relation->fan->interest_number;
				_relation->fan->user_score += added_value;
			}
			while ((_relation->similar_interest != NULL) && (_relation = _relation->similar_interest));
		}
	}
}