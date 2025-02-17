#include <iostream>
#include <fstream>
#include <omp.h>
#include <unistd.h>
#include <thread>

//Вариант № 16: Читатели-писатели. Приоритет писателей. Два замка и критическая секция (п.3.2)
// Как только появился хоть один писатель, никого больше не пускать. Все остальные могут простаивать.

using namespace std;

int Nrdr = 0;
int info = 0;
ofstream file;
//omp_lock_t readers; // определяем специальный тип данных под замки
//omp_lock_t writers;

void Reader(int iter_num)
{
//#pragma omp critical //выполняется только одним потоком за раз, чтобы не писали все присутствующие здесь потоки
	//{
		cout << "Reader " << omp_get_thread_num() << " (iter: " << iter_num << ") is waiting" << endl;
		file << "Reader " << omp_get_thread_num() << " (iter: " << iter_num << ") is waiting" << endl;
	//}
	//Sleep(300); // задержка
	//omp_set_lock(&readers); // Блокируем читателей для нормального увеличения счётчика и работы с блоком писателя
	Nrdr += 1;
	//omp_unset_lock(&readers); // остальные могут заходить и повышать счётчик
//#pragma omp critical //выполняется только одним потоком за раз, чтобы не писали все присутствующие здесь потоки
	//{
		cout << "Reader " << omp_get_thread_num() << " (iter: " << iter_num << ") walked in" << endl;
		file << "Reader " << omp_get_thread_num() << " (iter: " << iter_num << ") walked in" << endl;
	//}
	usleep(50 + rand() % (1000 - 50 + 1));
//#pragma omp critical //выполняется только одним потоком за раз, чтобы не писали все присутствующие здесь потоки
	//{
		int my_info = info;
		usleep(50 + rand() % (1000 - 50 + 1));
		file << "Reader " << omp_get_thread_num() << " (iter: " << iter_num << ") is reading data: " << my_info << "\n";
		cout << "Reader " << omp_get_thread_num() << " (iter: " << iter_num << ") is reading data: " << my_info << endl;

		//Sleep(300); // задержка
	//}

	//omp_set_lock(&readers); //Также блокируем для уменьшения счётчика
	Nrdr -= 1;

	//omp_unset_lock(&readers);
//#pragma omp critical //выполняется только одним потоком за раз, чтобы не писали все присутствующие здесь потоки
	//{
		cout << "Reader " << omp_get_thread_num() << " (iter: " << iter_num << ") left." << endl;
		file << "Reader " << omp_get_thread_num() << " (iter: " << iter_num << ") left." << endl;
	//}
}

void Writer(int iter_num)
{
	// блокируем всех остальных
	//omp_set_lock(&writers);
	//omp_set_lock(&readers);

//#pragma omp critical //выполняется только одним потоком за раз, чтобы не писали все присутствующие здесь потоки
	//{
		cout << "Writer " << omp_get_thread_num() << " (iter: " << iter_num << ") Walked in" << endl;
		file << "Writer " << omp_get_thread_num() << " (iter: " << iter_num << ") Walked in" << endl;
	//}

//#pragma omp critical //выполняется только одним потоком за раз, чтобы не писали все присутствующие здесь потоки
	//{
		int my_info = info;
		usleep(50 + rand() % (1000 - 50 + 1));
		// записываем
		file << "Writer " << omp_get_thread_num() << " (iter: " << iter_num << ") is writing data: " << ++my_info << "\n";
		cout << "Writer " << omp_get_thread_num() << " (iter: " << iter_num << ") is writing data: " << my_info << endl;
		info = my_info;
	//}
	// разблокируем читателей и писателей
	//omp_unset_lock(&readers);
		//Sleep(300);// задержка
	//omp_unset_lock(&writers);
}

int main(int argc, char* argv[])
{
	int n; //Число итераций для читателей и писателей

	//omp_init_lock(&readers); //инициализируем второй замок
	//omp_init_lock(&writers);
	file.open("output.txt");

	if (argc == 2)
		n = atoi(argv[1]);
	else {
		cout << "Enter num of iters: ";
		cin >> n;
	}

#pragma omp parallel num_threads(8)
	{
#pragma omp sections nowait
		{
#pragma omp section
			{
				for (int i = 0; i < n; i++)
					Writer(i);
			}
#pragma omp section
			{
				for (int i = 0; i < n; i++)
					Reader(i);
			}
#pragma omp section
			{
				for (int i = 0; i < n; i++)
					Writer(i);
			}
#pragma omp section
			{
				for (int i = 0; i < n; i++)
					Reader(i);
			}
		}
	}

	file.close();


	cout << "End!\n";
}
