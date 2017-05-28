/**
将一天24小时按每半小划分成48段，我们用一个位图表示选中的时间区间，例如`110000000000000000000000000000000000000000000000`，
表示第一个半小时和第二个半小时被选中了，其余时间段都没有被选中，也就是对应00:00~01:00这个时间区间。一个位图中可能有多个不连续的
时间区间被选中，例如`110010000000000000000000000000000000000000000000`，表示00:00-01:00和02:00-02:30这两个时间区间被选中了。

要求：写一个函数timeBitmapToRanges，将上述规则描述的时间位图转换成一个选中时间区间的数组。
示例输入：`"110010000000000100000000000000000000000000000000"`
示例输出：`["00:00~01:00", "02:00~02:30"]`
*/

/**
	input:number
	state:number
	return:number
	func:00:0->0,01:0->1,10:1->0,11:1->1
*/
const switchState = (input,state) => (state % 2) * 2 + input;

/**
	num:number
	return:string
*/
const num2str = (num) => {
	let str = num.toString();
	if(str.length === 1)
		str = '0' + str;
	return str;
}

/**
	start:number
	end:number
*/
const formatTime = (start, end) => {
	let start_temp = num2str(Math.floor(start / 2)) + ':' + num2str((start % 2) * 30),
		end_temp = num2str(Math.floor(end / 2)) + ':' + num2str((end % 2) * 30);
    return start_temp + '~' + end_temp;
}

function timeBitmapToRanges(bitmap) {
  
	let time_list = [];
  
	let start = 0,
		end = 0,
		state = 0;
  
	for(let i = 0; i < bitmap.length; i ++)
	{
		state = switchState(parseInt(bitmap[i]), state);
		switch(state)
		{
			case 0:
				break;
			case 1:
				start = i;
				break;
			case 2:
				end = i;
				time_list.push(formatTime(start, end));
				break;
			case 3:
				break;
		}
	}
	return time_list;
}

console.log(timeBitmapToRanges('110010000000000100000000000000000000000000000000'));