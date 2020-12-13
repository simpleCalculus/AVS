/*
Нарзуллозода Сахижони Зафар
БПИ194
Микропроект 2
Вариант 14.
Задача о гостинице-2 (умные клиенты). В гостинице 10 номеров с
ценой 200 рублей, 10 номеров с ценой 400 рублей и 5 номеров с ценой 600
руб. Клиент, зашедший в гостиницу, обладает некоторой суммой и получает
номер по своим финансовым возможностям, если тот свободен. Если среди
доступных клиенту номеров нет свободных, клиент уходит искать ночлег в
другое место. Создать многопоточное приложение, моделирующее работу
гостиницы
*/
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <unistd.h>
#include <string>

using namespace std;

static const int MAX_NUMBER_OF_CASH = 5000; // Максимальное число которого может вернуть генератор случайных чисел
static const int DAY_LIMIT_MS = 7000000;

/// Структура описывающая комнату
struct Room {
    int room_price = 0;
    bool is_free = false;
    Room(){room_price = 0; is_free = false;};
} rooms[25]; // создаём 215 комнат
std::mutex mtx_rooms;

std::mutex mtx_report;

// Структура описывающая клиента
struct Client {
    int client_id;
    int cash;
    int client_room_type_id;
    Client() {client_id = 0; cash = 0; client_room_type_id = -1;};
};

// метод для вывода сообщений на консоль
// несколько потоков не могут одновременно использовать этот метод
void print_report(string message) {
    std::lock_guard<std::mutex> lock(mtx_report);
    cout << message << '\n' << '\n' << flush;
    usleep(1500000);
}

// метод реализующая попытку клиента получит комнату
// если клиент может получит комнату, тогда метод вернет номер этой комнаты
// в противном случае вернет -1
int client_trying_to_enter(Client client) {
    std::lock_guard<std::mutex> lock1(mtx_rooms);
    bool has_free_room = 0;
    string message = "";
    for (int i = 24; i >= 0; i--) {
        if (rooms[i].is_free && rooms[i].room_price <= client.cash) {
            rooms[i].is_free = false;
            message = " client " + to_string(client.client_id) + " find room ( " + to_string(i) + " ) with price = " + to_string(rooms[i].room_price);
            print_report(message);
            return i;
        }
        has_free_room = has_free_room | rooms[i].is_free;
    }
    if (has_free_room == 0) {
        message = " client " + to_string(client.client_id) + " couldn't find any free room in the hotel. ";
        print_report(message);
    }
    else {
        message =  " client " + to_string(client.client_id) + " couldn't afford any free room.  ";
        print_report(message);
    }
    return -1;
}

// метод описывающая действия клиента
void thread_proc(Client client) {
    while (true)
    {
        string message = " client " + to_string(client.client_id) + " is trying to find room ";
        print_report(message);
        int room_id = client_trying_to_enter(client);
        if (room_id == -1) {
            message = " client " + to_string(client.client_id) + " is leaving the hotel";
            print_report(message);
            return;
        }
        client.client_room_type_id = room_id;
        int k = client.cash / rooms[room_id].room_price;
        client.cash -= k * rooms[room_id].room_price;
        message = "client " + to_string(client.client_id) + " is payed " + to_string(k * rooms[room_id].room_price) + " for the " + to_string(k) + " days in the room number " + to_string(room_id) + " with price = " + to_string(rooms[room_id].room_price);
        print_report(message);
        usleep(k * DAY_LIMIT_MS);
    }
}

int main() {
    cout << " Hello " << '\n';
    for (int i = 0; i < 10; i++) { // создаём 10 комнат с ценой 200 р
        rooms[i].room_price = 200;
        rooms[i].is_free = true;
    }
    for (int i = 10; i < 20; i++) { // создаём 10 комнат с ценой 400 р
        rooms[i].room_price = 400;
        rooms[i].is_free = true;
    }
    for (int i = 20; i < 25; i++) { // создаём 5 комнат с ценой 600 р
        rooms[i].room_price = 600;
        rooms[i].is_free = true;
    }


    std::vector<std::thread> threads;
    for(int i = 0; i < 100; i++) { // создаём 100 клиентов
        Client client;
        client.cash = (rand() ^ rand()) % MAX_NUMBER_OF_CASH; // рандомное число
        client.client_id = i;
        client.client_room_type_id = -1; // пока что клиент не заселился
        string message = "\n main is creating new client \n";
        message += "client id = " + to_string(client.client_id) + "\n" +
            "client cash = " + to_string(client.cash) + "\n";
        print_report( message);

        std::thread thr(thread_proc, client);
        threads.emplace_back(std::move(thr));
        usleep(3000000);
    }

    for(auto& thr : threads) {
        thr.join();
    }

    std::cout << "Done!" << std::endl;
    return 0;
}
