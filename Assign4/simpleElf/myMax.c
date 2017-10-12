

int myMax(int argc, int numArr[]){
	
	if(argc <= 0)
	return 0;

	
	int i;
	int tempMax = numArr[0];
	int finalMax;

	for (i = 0; i < argc; ++i)
	{
		if(numArr[i] > tempMax)
		{
			tempMax = numArr[i];
			finalMax = numArr[i];
		}

	}

	return finalMax;

}