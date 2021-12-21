#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "synchronize.h"

/* Идентификатор семафора взаимного исключения */
static int lock_data_sem_id;
/* Идентификаторы наборов семафоров, используемых для
синхронизации. */
static int processes_in_sem_id;
static int processes_out_sem_id;
/* Структура данных для операций с набором семафоров */
struct sembuf* lock_all;
/* "Открыть" новый набор семафоров длины n и установить
начальное значение каждого семафора val. Возвращает
идентификатор набора, -1 в случае ошибки. */
static int sem_open(int n, int val)
{
	int id; /* результат */
	/* Структура данных для управления семафором */
#ifdef _SEM_SEMUN_UNDEFINED
	union semun
	{
		/* Значение для команды SETVAL */
		int val;
		/* Буфер для команд IPC_STAT и IPC_SET */
		struct semid_ds* buf;
		/* Массив для команд GETALL и SETALL */
		unsigned short int* array;
		/* Буфер для команды IPC_INFO */
		struct seminfo* __buf;
	};
#endif
	union semun s_un;
	int i;
	/* Создаем новый набор семафоров */
	if ((id = semget(IPC_PRIVATE, n,
		IPC_CREAT | 0664)) == -1)
	{
		fprintf(stderr, "Cannot create semaphore\n");
		return -1;
	}
	/* Выделяем память под массив начальных значений */
	if (!(s_un.array = (unsigned short int*)
		malloc(n * sizeof(unsigned short int))))
	{
		/* Мало памяти */
		fprintf(stderr, "Not enough memory\n");
		return -1;
	}
	/* Начальное значение набора семафоров */
	for (i = 0; i < n; i++)
		s_un.array[i] = val;
	/* Установить начальное значение каждого семафора */
	if (semctl(id, 0, SETALL, s_un) == -1)
	{
		fprintf(stderr, "Cannot init semaphore\n");
		/* С семафором работать нельзя, заканчиваем */
		free(s_un.array);
		return -1;
	}
	free(s_un.array);
	return id;
}
/* "Закрыть" семафор с идентификатором id.
Возвращает не 0 в случае ошибки. */
static int sem_close(int id)
{
	/* Удаляем семафор. */
	if (semctl(id, 0, IPC_RMID, 0) == -1)
	{
		fprintf(stderr, "Cannot delete semaphore\n");
		return -1;
	}
	return 0;
}
/* Инициализировать все объекты синхронизации и
взаимного исключения. Возвращает -1 в случае ошибки.*/
int init_synchronize_lock(int total_processes)
{
	int i;
	/* Создаем 1 семафор с начальным значением 1 */
	lock_data_sem_id = sem_open(1, 1);
	/* Создаем total_processes семафоров с начальным
	значением 0 */
	processes_in_sem_id = sem_open(total_processes, 0);
	/* Создаем total_processes семафоров с начальным
	значением 0 */
	processes_out_sem_id = sem_open(total_processes, 0);
	/* Проверяем успешность создания */
	if (lock_data_sem_id == -1 || processes_in_sem_id == -1
		|| processes_out_sem_id == -1)
		return -1; /* семафоры не созданы */
		/* Выделяем память под массив */
	if (!(lock_all = (struct sembuf*)
		malloc(total_processes * sizeof(struct sembuf))))
		return -2; /* нет памяти */
		/* Инициализируем массив */
	for (i = 0; i < total_processes; i++)
	{
		lock_all[i].sem_num = i;
		lock_all[i].sem_op = -1;
		lock_all[i].sem_flg = 0;
	}
	return 0;
}
/* Закончить работу со всеми объектами синхронизации и
взаимного исключения. Возвращает -1 в случае ошибки.*/
int destroy_synchronize_lock()
{
	if (sem_close(lock_data_sem_id) == -1
		|| sem_close(processes_in_sem_id) == -1
		|| sem_close(processes_out_sem_id) == -1)
		return -1; /* ошибка */
	free(lock_all);
	return 0;
}
/* Обеспечить исключительную работу с разделяемой памятью
текущему процессу. Возвращает не 0 в случае ошибки. */
int lock_data()
{
	/* Структура данных для операций с семафором */
	struct sembuf s_buf = { 0, -1, 0 };
	if (semop(lock_data_sem_id, &s_buf, 1) == -1)
	{
		fprintf(stderr, "Cannot lock semaphore\n");
		return -1; /* ошибка */
	}
	return 0;
}
/* Разрешить другим процессам работать с разделяемой
памятью. Возвращает не 0 в случае ошибки. */
int unlock_data()
{
	/* Структура данных для операций с семафором */
	struct sembuf s_buf = { 0, 1, 0 };
	if (semop(lock_data_sem_id, &s_buf, 1) == -1)
	{
		fprintf(stderr, "Cannot unlock semaphore\n");
		return -1; /* ошибка */
	}
	return 0;
}
/* Подпрограмма для synchronize */
static int synchronize_internal(int id, short unsigned int process_num, short int total_processes)
{
	/* Добавить total_processes единиц к семафору с номером
	process_num из набора с идентификатором id. */
	/* Структура данных для операций с семафором */
	struct sembuf s_buf = { process_num, total_processes, 0 };
	if (semop(id, &s_buf, 1) == -1)
		return -1; /* ошибка */
/* Вычесть 1 из каждого из total_processes семафоров
из набора с идентификатором id. */
	if (semop(id, lock_all, total_processes) == -1)
		return -2; /* ошибка */
	return 0;
}
/* Дождаться в текущем процессе с номером process_num
остальных процессов (из общего числа total_processes)*/
void synchronize(int process_num, int total_processes)
{
	/* Дождаться, пока все процессы войдут в эту функцию */
	synchronize_internal(processes_in_sem_id, process_num,
		total_processes);
	/* Дождаться, пока все процессы выйдут из этой функции*/
	synchronize_internal(processes_out_sem_id, process_num,
		total_processes);
}