#include <iostream>
#include <math.h>
#include "collaborative_filter.h"

void collaborative_filter(user_container* _ulist, item_container* _ilist)
{

	for (_container_number_ i = 0; i < _ilist->size(); i++)
	{
		if ((*_ilist)[i] == NULL || (*_ilist)[i]->item_score == 0)
			continue;

		for (_container_number_ j = 0; j < _ilist->size(); j++)
		{
			if ((*_ilist)[j] == NULL || i == j)
				continue;

			_value_number_ similarity = 0;

			relation *_ir = (*_ilist)[i]->fan_list;
			relation *_jr = (*_ilist)[j]->fan_list;

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

			(*_ilist)[j]->item_score +=  similarity / (sqrt((*_ilist)[i]->fan_number) * sqrt((*_ilist)[i]->fan_number));
		}
	}
}