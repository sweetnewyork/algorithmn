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
					similarity += 64;
					_ir = (*_ir).similar_interest;
					_jr = (*_jr).similar_interest;
					continue;
				}
			}

			_value_number_ xx = 0;
			_ir = (*_ilist)[i]->fan_list;
			do
				xx += 1;
			while (((*_ir).similar_interest != NULL) && (_ir = (*_ir).similar_interest));


			_value_number_ yy = 0;
			_jr = (*_ilist)[j]->fan_list;
			do
				yy += 1;
			while (((*_jr).similar_interest != NULL) && (_jr = (*_jr).similar_interest));

			(*_ilist)[j]->item_score +=  int(similarity / (sqrt(double(xx)) * sqrt(double(yy))));
		}
	}
}