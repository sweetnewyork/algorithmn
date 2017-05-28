#include<iostream>
#include<algorithm>
#include<vector>
#include<bitset>
#include<climits>

bool increase(std::bitset<64> &_bitset)
{
	for (int i = 0; i < 64; i++)
	{
		if (_bitset[i] == 1)
		{
			_bitset[i] = 0;
		}
		else
		{
			_bitset[i] = 1;
			break;
		}
	}
	return true;
}

int main(int argc, char **args)
{
	int n = 0;
	std::cin >> n;
	std::vector<int> task(n);
	for (int i = 0; i < task.size(); i++)
		std::cin >> task[i];

	std::bitset<64> flag;
	std::bitset<64> final;

	for (int i = 0; i < task.size(); i++)
		final[i] = 1;
	
	int left = 0,
		right = 0;

	int result = INT_MAX;
	do
	{
		left = 0;
		right = 0;
		for (int i = 0; i < task.size(); i++)
		{
			if (flag[i] == 1)
				left += task[i];
			else
				right += task[i];
		}
		result = std::min(result, std::max(left, right));
		
	} while ((flag != final) && (increase(flag)));
	
	std::cout << result << std::endl;

	return 0;
}