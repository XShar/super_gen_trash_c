#include "pch.h"
#include "../../super_tresh_c/super_tresh_c/Release/trash.h"
#include <iostream>
#include <ctime>

int main()
{
	printf("START TEST TRASH \n");

	//Инициализация генератора случайных чисел
	srand(time(0));

	//Получение рандомного i, для случайного попадания в ветки
	int i = 1 + rand() % 255;

	//Запускаем мусорный код :)
	int result_i = tresh_gen(i);


	printf("END TEST TRASH I = %d \n", result_i);

	system("PAUSE");
}
