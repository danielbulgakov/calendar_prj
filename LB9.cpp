#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <winuser.h>
#include <stdio.h>
#include "time.h"
#include <stdlib.h>
#include <omp.h>
#include <locale.h>
#include <math.h>

#define MAX_ARRAY_SIZE 85000 // Иначе кол-во перстановок выйдет за границы unsigned int
int array_main[MAX_ARRAY_SIZE] ;
int array_sort[MAX_ARRAY_SIZE] ;
unsigned int itt[7] = { 0 }, swap_count[7] = { 0 };
double time_s[7] = { 0 };
int sort_count[8] = { 0 };
int out_c = 0;
int sl = 1;
int errors[7];

//----------------------------------------------------------------------------------
//--------- Interface Functioning
//----------------------------------------------------------------------------------

void vse_horosho() {
	printf_s("ОК - Тесирование прошло успешно!\n");
}

void select_menu(int sl) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	system("cls");
	
	printf("%s  %sСортировка выбором%s  \n" ,  sl == 1 ? "\x1B[31m"  "-> "   : "",                                  sort_count[1] == 1 ? "\x1B[31m" : "\x1B[37m", sl == 1 ? "\x1B[90m""\n\n---------------> Принцип работы <--------------- \n 1. Находим номер минимального значения в текущем списке.\n 2. Производим обмен этого значения со значением первой неотсортированной позиции\n 3. Теперь сортируем хвост списка, исключив из рассмотрения уже отсортированные элементы\n  Сложность : O(n^2 / 2)\n""\x1B[37m" : "");
	printf("%s  %sСортировка пузырьком%s\n",   sl == 2 ? "\x1B[31m"  "-> "    : "",                                 sort_count[2] == 1 ? "\x1B[31m" : "\x1B[37m", sl == 2 ? "\x1B[90m""\n\n---------------> Принцип работы <--------------- \n Cравниваем значения соседних элементов и меняем числа местами, если предыдущее оказывается больше последующего.\n  Сложность : O(n^2 / 2)\n""\x1B[37m" : " ");
	printf("%s  %sСортировка вставками%s \n",  sl == 3 ? "\x1B[31m"  "-> "   : "",                                 sort_count[3] == 1 ? "\x1B[31m" : "\x1B[37m", sl == 3 ? "\x1B[90m""\n\n---------------> Принцип работы <--------------- \n Массив постепенно перебирается слева направо. При этом каждый последующий элемент размещается так, \n чтобы он оказался между ближайшими элементами с минимальным и максимальным значением. \n  Сложность : O(n^2 / 4)\n" "\x1B[37m" : " ");
	printf("%s  %sCортировка перемешиванием  / шейкерная сортировка%s \n", sl == 4 ? "\x1B[31m"  "-> "   : "",         sort_count[4] == 1 ? "\x1B[31m" : "\x1B[37m", sl == 4 ? "\x1B[90m""\n\n---------------> Принцип работы <--------------- \n Двунаправленная пузырьковая сортировка: алгоритм перемещается не строго слева направо, \n а сначала слева направо, затем справа налево.\n  Сложность : O(n^2 / 2)\n""\x1B[37m" : " ");
	printf("%s  %sСортировка расчёской%s \n", sl == 5 ? "\x1B[31m"  "-> "  : "",                                  sort_count[5] == 1 ? "\x1B[31m" : "\x1B[37m", sl == 5 ? "\x1B[90m""\n\n---------------> Принцип работы <--------------- \n Принцип схож с пузырьковой сортировкой, только при «расчёсывании» сначала берётся\n достаточно большое расстояние между сравниваемыми значениями, а потом оно сужается вплоть до минимального.\n  Сложность : O(n^2/ 2^p), p - количество инкрементов\n " "\x1B[37m" : " ");
	printf("%s  %sБыстрая сортировка Хоара%s \n", sl == 6 ? "\x1B[31m"  "-> "   : "",                                  sort_count[6] == 1 ? "\x1B[31m" : "\x1B[37m", sl == 6 ? "\x1B[90m""\n\n---------------> Принцип работы <--------------- \n 1. Выбрать из массива элемент, называемый опорным. Это может быть любой из элементов массива.\n 2. Сравнить все остальные элементы с опорным и переставить их в массиве так,\n    чтобы разбить массив на три непрерывных отрезка, следующих друг за другом:\n    «элементы меньшие опорного», «равные» и «большие»\n 3. Для отрезков «меньших» и «больших» значений выполнить рекурсивно ту же последовательность операций,\n    если длина отрезка больше единицы.\n  Сложность : O (n * log(n))\n ""\x1B[37m" : " ");
	printf("\n%s  Далее\n", sl == 7 ? "\x1B[31m"  "-> "  "\x1B[37m" : "\x1B[37m");
	//for (int i = 0; i <= 7; i++) printf_s("\na[%d]=%d\n", i, sort_count[i]);
}

int counter() {
	
	while (TRUE) {
		if (GetAsyncKeyState(VK_UP)) {
			keybd_event(VK_UP, 0, KEYEVENTF_KEYUP, 0);
			if (sl > 1) sl--;
			select_menu(sl);
		}
		if (GetAsyncKeyState(VK_DOWN)) {
			keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
			if (sl < 7) sl++;
			select_menu(sl);
		}
		if (GetAsyncKeyState(VK_RETURN)) {
			keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
			if (sl == 7) {
				if (sort_count[1] != 0 || sort_count[2] != 0 || sort_count[3] != 0 || sort_count[4] != 0 || sort_count[5] != 0 || sort_count[6] != 0) {
					break;
				}
				else printf_s("\n \x1B[31m !Выберите хотя бы один пункт!\x1B[37m\n");
				system("pause");
			}
			if (sort_count[sl] == 0) sort_count[sl] = 1;
			else sort_count[sl] = 0;
			select_menu(sl);
			
		}
		
	}
	return (sl);
	

}

void sort_check_out(int i) {
	switch (i) {
	case 1: 
	{
		printf_s("Сортировка выбором\n...");
		if (errors[1] == 0) vse_horosho();
		else printf("Ошибка сортировки на индексе %d\n", errors[1]);
		 }break;
	case 2: 
	{
		printf_s("Сортировка пузырьком\n...");
		if (errors[2] == 0) vse_horosho();
		else printf("Ошибка сортировки на индексе %d\n", errors[2]);
		 }break;
	case 3: 
	{
		printf_s("Сортировка вставками\n...");
		if (errors[3] == 0) vse_horosho();
		else printf("Ошибка сортировки на индексе %d\n", errors[3]);
	
	}break;
	case 4: 
	{
		printf_s("Cортировка перемешиванием\n...");
		if (errors[4] == 0) vse_horosho();
		else printf("Ошибка сортировки на индексе %d\n", errors[4]);
	}break;
	case 5: 
	{
		printf_s("Сортировка расчёской\n...");
		if (errors[5] == 0) vse_horosho();
		else printf("Ошибка сортировки на индексе %d\n", errors[5]);
	}break;
	case 6: {
		printf_s("Быстрая сортировка Хоара\n...");
		if (errors[6] == 0) vse_horosho();
		else printf("Ошибка сортировки на индексе %d\n", errors[6]);
	}break;
	}
}

void results() {
	printf_s("+----------------------------+------------------------+------------------------+------------------------+---------------------------+\n");
	printf_s("| Вид сортировки             | Кол-во перестановок    | Кол-во иттераций       |  Время                 | Сложность                 |\n");
	printf_s("+----------------------------+------------------------+------------------------+------------------------+---------------------------+\n");
	if (sort_count[1] == 1) {
		printf_s("| %26s | %22u | %22u | %22lf | %25s |\n", "Сортировка выбором", swap_count[1], itt[1], time_s[1], "O(n^2 / 2)");
		printf_s("+----------------------------+------------------------+------------------------+------------------------+---------------------------+\n");
	}
	if (sort_count[2] == 1) {
		printf_s("| %26s | %22u | %22u | %22lf | %25s |\n", "Сортировка пузырьком", swap_count[2], itt[2], time_s[2], "O(n^2 / 2)");
		printf_s("+----------------------------+------------------------+------------------------+------------------------+---------------------------+\n");
	}
	if (sort_count[3] == 1) {
		printf_s("| %26s | %22u | %22u | %22lf | %25s |\n", "Сортировка вставками", swap_count[3], itt[3], time_s[3], "O(n^2 / 4)");
		printf_s("+----------------------------+------------------------+------------------------+------------------------+---------------------------+\n");
	}
	if (sort_count[4] == 1) {
		printf_s("| %26s | %22u | %22u | %22lf | %25s |\n", "Сортировка перемешиванием", swap_count[4], itt[4], time_s[4], "O(n^2 / 2)");
		printf_s("+----------------------------+------------------------+------------------------+------------------------+---------------------------+\n");
	}
	if (sort_count[5] == 1) {
		printf_s("| %26s | %22u | %22u | %22lf | %25s |\n", "Сортировка расчёской", swap_count[5], itt[5], time_s[5], "O(n^2/ 2^p)");
		printf_s("+----------------------------+------------------------+------------------------+------------------------+---------------------------+\n");
	}
	if (sort_count[6] == 1) {
		printf_s("| %26s | %22u | %22u | %22lf | %25s |\n", "Быстрая сортировка Хоара", swap_count[6], itt[6], time_s[6], "O (n * log(n))");
		printf_s("+----------------------------+------------------------+------------------------+------------------------+---------------------------+\n\n\n\n");
	}
}

//----------------------------------------------------------------------------------
//--------- Array Controls
//----------------------------------------------------------------------------------

int array_size_control() {
	int size = -15;
	do {
		system("cls");
		system("cls");
		system("cls");
		printf_s("----------< Введите размер массива >----------\n");
		printf_s("Максимальный размер массива -- %d\n", MAX_ARRAY_SIZE);
		printf_s("Значение размера --> "); scanf_s("%d", &size);
		if (size < MAX_ARRAY_SIZE && size > 2) return (size);
	} while (TRUE);
}

void border_array_control(int* l, int* r) {
	do {
		system("cls");
		printf_s("----------< Введите границы массива >----------\n");
		printf_s("1ая граница --> "); scanf_s("%d", &*l);
		printf_s("2ая граница --> "); scanf_s("%d", &*r); 
	} while (*l > 1e9 || *r < -(1e9) );
}

void generate_array(int a[], int len, int l_b_i, int r_b_i) {
	srand((unsigned)time(0));
	for (int i = 0; i < len; i++) a[i] = int((double)(rand()) / RAND_MAX * (r_b_i - l_b_i) + l_b_i);
}

int testSort(int arr[], int len) {
	for (int i = 1; i < len; i++) {
		if (arr[i] < arr[i - 1]) return(i);
	}
	return(0);
}

void array_to_start(int len) {
	for (int i = 0; i < len; i++)
		array_sort[i] = array_main[i];
}

void output_array(int a[], int len) {
	for (int i = 0; i < len; i++) {
		printf("%d ", a[i]);
	}
}

//----------------------------------------------------------------------------------
//--------- Array Sort  
//----------------------------------------------------------------------------------
// 1 . Сортировка выбором
void sort_type1(int a[], int len) {
	double start = omp_get_wtime();
	for (int i = 0; i < len - 1 ; i++) {
		for (int j = i; j < len ; j++) {
			itt[1]++;
			if (a[i] > a[j]) {
				int temp;
				temp = a[i];
				a[i] = a[j];
				a[j] = temp;

				swap_count[1]++;
			}
		}
	}
	double end = omp_get_wtime();
	time_s[1] = end - start;
	errors[1] = testSort(a, len);	
}

// 2 . Сортировка пузырьком
void sort_type2(int a[], int len) {
	double start = omp_get_wtime();
	for (int i = 0; i < len - 1; i++) {
		for (int j = 0; j < len - 1 - i; j++) {
			itt[2]++;
			if (a[j] > a[j + 1]) {
				int temp;
				temp = a[j];
				a[j] = a[j+1];
				a[j+1] = temp;
				swap_count[2]++;
			}
		}
	}
	double end = omp_get_wtime();
	time_s[2] = end - start;
	errors[2] = testSort(a, len) ;	
}

// 3. Сортировка вставками
void sort_type3(int a[], int len) {
	int j = 0;
	double start = omp_get_wtime();
	for (int i = 0; i < len; i++) {
		j = i;
		while (j >= 0 && a[j] < a[j - 1]) {
			itt[3]++;
			int temp;
			temp = a[j];
			a[j] = a[j -1];
			a[j - 1] = temp;
			j--;
			swap_count[3]++;
		}
	}
	double end = omp_get_wtime();
	time_s[3] = end - start;
	errors[3] = testSort(a, len) ;	
}

// 4. Cортировка перемешиванием  / шейкерная сортировка (Двунаправленная пузырьковая)
void sort_type4(int a[], int len) {
	int left = 0;
	int right = len - 1;
	double start = omp_get_wtime();
	while (left <= right) {
		for (int i = right; i > left; i--) {
			itt[4]++;
			if (a[i - 1] > a[i]) {
				swap_count[4]++;
				int temp;
				temp = a[i];
				a[i] = a[i-1];
				a[i-1] = temp;

			}
		}
		++left;
		for (int i = left; i < right; i++) {
			itt[4]++;
			if (a[i] > a[i + 1]) {
				swap_count[4]++;
				int temp;
				temp = a[i];
				a[i] = a[i + 1];
				a[i + 1] = temp;

			}
		}
		--right;
	}
	double end = omp_get_wtime();
	time_s[4] = end - start;
	errors[4] = testSort(a, len) ;	
}

// 5. Сортировка расчёской (С работой ужимается расстояния между сравниваемыми значениями)
void sort_type5(int a[], int len) {
	const double factor = 1.2473309; // Фактор уменьшения
	double step = double(len) - 1;
	double start = omp_get_wtime();
	while (step >= 1) {
		for (int i = 0; i + step < len; i++) {
			itt[5]++;
			if (a[i] > a[i + int(step)]) {
				swap_count[5]++;
				int temp;
				temp = a[i];
				a[i] = a[i + int(step)];
				a[i + int(step)] = temp;
			}
		}
		step /= factor;
	}
	double end = omp_get_wtime();
	time_s[5] = end - start;
	errors[5] = testSort(a, len);	
}

// 6. Быстрая сортировка Хоара
void sort_type6(int a[], int len, int left, int right) {
	
	int i = left, j = right, x = a[(left + right) / 2];

	do {
		itt[6]++;
		while (a[i] < x) {
			i++; itt[6]++;
		}
		while (a[j] > x) {
			j--; itt[6]++;
		}
		if (i <= j) {
			if (a[i] > a[j]) {
				swap_count[6]++;
				int temp;
				temp = a[j];
				a[j] = a[i];
				a[i] = temp;
			}
			
			i++;
			j--;
		}
	} while (i <= j);

	if (i < right)
		sort_type6(a,len, i, right);
	if (left< j)
		sort_type6(a,len, left, j);
}

//Контроль сортировки
void sort_control(int size) {
	if (sort_count[1] == 1) {
		sort_type1(array_sort, size);
		sort_check_out(1);
		array_to_start(size);
	}
	if (sort_count[2] == 1) {
		sort_type2(array_sort, size);
		sort_check_out(2);
		array_to_start(size);
	}
	if (sort_count[3] == 1) {
		sort_type3(array_sort, size);
		sort_check_out(3);
		array_to_start(size);
	}
	if (sort_count[4] == 1) {
		sort_type4(array_sort, size);
		sort_check_out(4);
		array_to_start(size);
	}
	if (sort_count[5] == 1) {
		sort_type5(array_sort, size);
		sort_check_out(5);
		array_to_start(size);
	}
	if (sort_count[6] == 1) {
		double start = omp_get_wtime();
		sort_type6(array_sort, size, 0, size - 1);
		double end = omp_get_wtime();
		time_s[6] = end - start;
		errors[6] = testSort(array_sort, size);
		sort_check_out(6);
		array_to_start(size);
	}
}
//---------Sort end - Main---------------------------------------------------------
int main() {
	setlocale(LC_ALL, "Russian");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	select_menu(1);
	counter();
	system("cls");
	printf("\r");
	
	int size = array_size_control();
	int l = 0, r = 0;
	border_array_control(&l, &r);
	generate_array(array_main, size, l, r);
	
	
	
	array_to_start( size);
	output_array(array_main, size);//Вывод введенного массива
	system("pause");
	system("cls");
	sort_control(size);// Проверка сортировки
	system("pause");
	system("cls");
	results(); // Выводим результат
	system("pause");
	return 0;

}
