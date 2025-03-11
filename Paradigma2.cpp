// Paradigma2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.




#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <chrono>
#include <conio.h>

using namespace std;

bool flag = false;
queue<pair<long, long>> requests;
mutex mtxQueue;
mutex mtxCout;

void stop()
{
    while (true)
    {
        if (_getch() == 3)
        {
            cout << "Exiting program..." << endl;
            flag = true;
            return;
        }
    }
}

void generator(long numberGroup, long capacityStorage)
{
    while (!flag)
    {
        this_thread::sleep_for(chrono::seconds(rand() % 2 + 1));

        if (!flag)
        {
            mtxQueue.lock();

            if (requests.size() < capacityStorage)
            {
                long group = rand() % numberGroup + 1;
                long type = rand() % 3 + 1;
                requests.push(make_pair(group, type));
                mtxCout.lock();
                cout << "A request of type " << type << " is generated for group " << group << ". Queue size: " << requests.size() << endl;
                mtxCout.unlock();
            }
            else
            {
                mtxCout.lock();
                cout << "Max capacity." << endl;
                mtxCout.unlock();

              //  this_thread::sleep_for(chrono::seconds(1));
            }

            mtxQueue.unlock();
        }
    }
}

void Service(long deviceNumber, long capacityStorage)
{
    long processingTime;

    while (!flag)
    {
        mtxQueue.lock();

        if (!requests.empty())
        {
            pair<long, long>request = requests.front();
            requests.pop();
            mtxQueue.unlock();
            processingTime = rand() % 5 + 1;

            this_thread::sleep_for(chrono::seconds(processingTime));

            if (!flag)
            {
                mtxCout.lock();
                cout << "Device number " << deviceNumber << " is processing a request from group " << request.first << " with type " << request.second << ". Processing time: " << processingTime << endl;
                mtxCout.unlock();
            }
        }

        else
        {
            mtxCout.lock();
            cout << "Device number " << deviceNumber << " is idle." << endl;
            mtxCout.unlock();
            mtxQueue.unlock();

            this_thread::sleep_for(chrono::seconds(1));
        }
    }
}

int main()
{
    long numberGroup;
    long numberDevices;
    long capacityStorage;

    cout << "Enter the number of groups: ";
    cin >> numberGroup;

    cout << "Enter the number of devices: ";
    cin >> numberDevices;

    cout << "Enter capacity storage: ";
    cin >> capacityStorage;

    thread StopThread(stop);
    thread generatorThread(generator, numberGroup, capacityStorage);

    thread* threads = new thread[numberDevices * numberGroup];

    for (long i = 0; i < numberDevices * numberGroup; i++)
    {
        threads[i] = thread(Service, i + 1, capacityStorage);
    }

    StopThread.join();

    for (long i = 0; i < numberDevices * numberGroup; i++)
    {


        threads[i].join();

    }

    generatorThread.join();


    cout << "Program finished" << endl;


    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
