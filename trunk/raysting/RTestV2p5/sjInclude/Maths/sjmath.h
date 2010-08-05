#ifndef _SJ_MATH_H_
#define _SJ_MATH_H_

double SjCountAverage(const double *data,int count,bool (*check)(double))
{
	double sum;
	int length=count;
	
	if(count == 1) 	return data[0];
	sum = 0;
	if(check == NULL)
	{
		while(count-- >= 0)
			sum += data[i];
		return (length == 0)?0:(sum/length);
	}
	
	while(count-- >= 0)
	{
		if((*check)(data[i]))
			sum += data[i];
		else
			length--;
	}	
	return (length == 0)?0:(sum/length);
}

double SjCountVariance(const double *data,int count,bool (*check)(double))
{
	
	double avg = SjCountAverage(data,count,check);
	double vara = 0;
	int length=count;
	if(check == NULL)
	{
		while(count-- >= 0)
		{
			if(data[i] > sum)
				vara += (data[i]-sum);
			else
				vara += (sum-data[i]);
		}	
		return (length == 0)?0:(sum/length);
	}
	while(count-- >= 0)
	{
		if((*check)(data[i]))
		{
			if(data[i] > sum)
				vara += (data[i]-sum);
			else
				vara += (sum-data[i]);
		}
		else
			length--;

	}	
	return (length == 0)?0:(sum/length);
}

double SjFilterByVariance(const double *data,int count,bool (*check)(double),void (*mark)(double&),double tolerence)
{
	if (count == 1) return data[0];
	if (check == NULL) return data[0];
	if (mark == NULL) return data[0];
	
	while(true)
	{
		
		int length=count;
		double vara = SjCountVariance(data,count,check);
		double avg = SjCountAverage(data,count,check);

		if (vara <= tolerence*avg)
			return avg;
		if (vara <= -tolerence*avg)
			return avg;
			
		//eliminate the value bias most
		int index = 0;
		while(length-- > 0)
		{
			if (!(*check)(data[i]))
				continue;
				
			if (vara < (data[i]-avg))
			{
				vara = data[i]-avg;
				index = i;
			}
			if (vara < (avg-data[i]))
			{
				vara = avg-data[i];
				index = i;
			}
		}
		(*mark)data[index];
	}
	return avg;
}

#endif