#include "pch.h"
#include <iostream>
#include <string>
#include <ctime>
#include <stdio.h>
#include <windows.h>


static int is_random_init = 0;
static std::string tresh = "";

//Генератор случайных чисел
static int do_random (int start, int end)
{
	if (is_random_init == 0)
	{
		srand(time(0));
		is_random_init = 1;
	}

	return (start + rand() % end);
}

//Хелпер для создания заголовочного файла с мусором

//Запись в файл
static bool WriteFileHelper(LPCWSTR lpwName, PBYTE lpStubData, DWORD dwFileSize_in)
{

	HANDLE hFile;
	if ((hFile = CreateFile(lpwName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
		return false;
	DWORD VeryUsefulDword;
	if (!WriteFile(hFile, lpStubData, dwFileSize_in, &VeryUsefulDword, NULL))
		return false;
	CloseHandle(hFile);
	return true;
}

// генерация случайной строки определенной длинны
static std::string rnd(int len)
{
	//srand(time(NULL));
	std::string str = "abcdefghijklmnopqrstuvwxyz";
	std::string newstr = "";
	int pos;
	while (newstr.size() != len) {
		pos = ((rand() % (str.size() - 1)));
		newstr += str.substr(pos, 1);
	}
	return newstr;
}


static void gen_tresh(void)
{
	
	// генерируем уникальные названия
	std::string x[255];
	int c = 254;
	int i = 0;
	bool ref = false;
	while (true) {
		ref = false;
		if (i >= c)  break;
		x[i] = rnd(6);
		if (i > 0)
			for (int s = 0; s < i - 1; s++) {
				if (x[i] == x[s]) { ref = true; break; }
			}
		if (ref == true) continue;
		//printf("%s\n", x[i].c_str());
		i++;
	}

	int r = do_random(100, 200);
	tresh = "#include <windows.h>\n\n";

	//Генерация мусорного кода

	tresh = tresh + "static __inline int sg(int i1,int i2) {\n";
	tresh = tresh + "return " + std::to_string(do_random(1, 255)) + "+i1+i2;\n";
	tresh = tresh + "}\n";

	tresh = tresh + "static __inline int dg(int i1,int i2) {\n";
	tresh = tresh + "return " + std::to_string(do_random(1, 255)) + "+i1+i2;\n";
	tresh = tresh + "}\n";

	int l = 0;
	int v = 0;
	int cnt = 0;
	int g = 0;
	l = do_random(1024, 2048);
	
	while (true) {
		tresh = tresh + "static __inline int r" + std::to_string(cnt) + "(int i) {\n";
		if (cnt != 0) tresh = tresh + "i=i+r" + std::to_string(cnt - 1) + "(i);\n";
	
		//Генерация WinAPI, можно много нагенерить
		g = do_random(1, 3);
		if (g == 1)
			tresh = tresh + "if (i<=-" + std::to_string(do_random(1, 255)) + ") VirtualAlloc(NULL, " + std::to_string(do_random(100, 200)) + ", MEM_COMMIT, PAGE_READWRITE);\n";
		if (g == 2)
			tresh = tresh + "if (i<=-" + std::to_string(do_random(1, 255)) + ") VirtualAlloc(NULL, " + std::to_string(do_random(1, 255)) + ", MEM_COMMIT, PAGE_READWRITE);\n";
		if (g == 3)
			tresh = tresh + "if (i<-" + std::to_string(do_random(1, 255)) + ") return i;\n";

		for (int h = 0; h < c; h++) {
			tresh = tresh + "int " + x[h] + ";\n";
		}
		for (int h = 0; h < c; h++) {
			tresh = tresh + x[h] + " = i+" + std::to_string(do_random(1, 200)) + ";\n";
		}

		int n = do_random(50, 100);
		for (int h = 0; h < n; h++) {
			int cas = do_random(1, 4);
			if (cas == 1)
				tresh = tresh + x[do_random(0, c - 1)] + "=" + x[do_random(0, c - 1)] + "* " + x[do_random(0, c - 1)] + "; \n";
			if (cas == 2)
				tresh = tresh + x[do_random(0, c - 1)] + "=" + x[do_random(0, c - 1)] + "* " + x[do_random(0, c - 1)] + ";\n";
			if (cas == 3) {
				tresh = tresh + "if (" + x[do_random(0, c - 1)] + " >= " + x[do_random(0, c - 1)] + ") ";
				tresh = tresh + x[do_random(0, c - 1)] + "=" + x[do_random(0, c - 1)] + "+" + x[do_random(0, c - 1)] + ";\n";
			}
			if (cas == 4) {
				tresh = tresh + "if (" + x[do_random(0, c - 1)] + " <= " + x[do_random(0, c - 1)] + ") ";
				tresh = tresh + x[do_random(0, c - 1)] + "=" + x[do_random(0, c - 1)] + "+" + x[do_random(0, c - 1)] + ";\n";
			}

			v++;
		}

		// отдаем все результаты, иначе оптимизация убьет не используемые функции и переменные!!!
		tresh = tresh + "i = i + ";
		int h = 0;
		for (h = 0; h < c - 1; h++) {
			tresh = tresh + x[h] + "+";
		}
		tresh = tresh + x[h] + ";\n";

		tresh = tresh + "return i;}\n\n";
		if (v >= l) break;
		cnt++;
	}
	
	// создаем основную функцию откуда идет инициализация и старт кода, API можно много нагенерить 
	tresh = tresh + "static __inline int tresh_gen(int i) {\n";
	tresh = tresh + "i = i + r" + std::to_string(cnt) + "(i);\n";
	tresh = tresh + "if (i<=-" + std::to_string(do_random(1, 255)) + ") VirtualAlloc(NULL, " + std::to_string(do_random(50, 100)) + ", MEM_COMMIT, PAGE_READWRITE);\n";
	tresh = tresh + "if (i<=-" + std::to_string(do_random(1, 255)) + ") VirtualAlloc(NULL, " + std::to_string(do_random(1, 255)) + ", MEM_COMMIT, PAGE_READWRITE);\n";
	tresh = tresh + "if (i<=-" + std::to_string(do_random(1, 255)) + ") MessageBoxW(0,L\"" + rnd(do_random(1, 255)) + "\",L\"" + rnd(do_random(1, 255)) + "\", MB_OK);\n";
	tresh = tresh + "return i;\n}; \n";

	if (WriteFileHelper(L"trash.h", (PBYTE)tresh.data(), tresh.size())) {
		printf(" - generation tresh	: completed!\n");
	}
	else { printf(" - generation tresh	: error!\n"); return; }
}

int main()
{
	printf("START TRESH GEN \n");
	gen_tresh();
	system("PAUSE");
}
