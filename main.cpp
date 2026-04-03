#include "airport.hpp"

int main() {
    Airport db;
    db.load_from_file("flights.txt");

    int choice;
    while (1) {
        print_menu();
        if (!(std::cin >> choice)) {
            std::cout << "err: invalid input\n";
            std::cin.clear();
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (choice) {
        case 1: // вывод рейсов
            db.print_flights();
            break;

        case 2: { // добавление нового
            std::string flight_number, plane_name;
            std::string departure_time, arrival_time;

            std::cout << "enter flight num (example: QF025): ";
            std::getline(std::cin, flight_number);

            std::cout << "enter plane name: ";
            std::getline(std::cin, plane_name);

            while (1) {
                std::cout << "enter departure time (hh:mm): ";
                std::getline(std::cin, departure_time);
                if (validate_time_format(departure_time)) {
                    break;
                }
                else {
                    std::cout << "err: incorrect format\n";
                }
            }

            while (1) {
                std::cout << "enter arrival time (hh:mm): ";
                std::getline(std::cin, arrival_time);
                if (validate_time_format(arrival_time)) {
                    break;
                }
                else {
                    std::cout << "err: incorrect format\n";
                }
            }

            db.add_flight(flight_number, plane_name, departure_time, arrival_time);
            break;
        }

        case 3: { // поиск рейса
            int search_choice;
            while (1) {
                print_search_menu();
                if (!(std::cin >> search_choice)) {
                    std::cout << "err: invalid input\n";
                    std::cin.clear();
                    clear_input_buffer();
                    continue;
                }
                clear_input_buffer();

                if (search_choice == 5) {
                    break;
                }

                switch (search_choice) {
                case 1: { // поиск по номеру рейса
                    std::string flight_number;
                    std::cout << "enter flight number: ";
                    std::getline(std::cin, flight_number);

                    Flight* flight = db.find_flight_by_number(flight_number);
                    if (flight) {
                        flight->print_details();
                    }
                    else {
                        std::cout << "flight not found\n";
                    }
                    break;
                }
                case 2: { // поиск по названию самолета
                    std::string plane_name;
                    std::cout << "enter plane name (or part of name): ";
                    std::getline(std::cin, plane_name);

                    db.find_flights_by_plane(plane_name);
                    break;
                }
                case 3: { // поиск по времени вылета
                    std::string departure_time;
                    std::cout << "enter departure time (hh:mm): ";
                    std::getline(std::cin, departure_time);

                    if (validate_time_format(departure_time)) {
                        db.find_flights_by_departure_time(departure_time);
                    }
                    else {
                        std::cout << "err: incorrect time format\n";
                    }
                    break;
                }
                case 4: { // поиск по времени посадки
                    std::string arrival_time;
                    std::cout << "enter arrival time (hh:mm): ";
                    std::getline(std::cin, arrival_time);

                    if (validate_time_format(arrival_time)) {
                        db.find_flights_by_arrival_time(arrival_time);
                    }
                    else {
                        std::cout << "err: incorrect time format\n";
                    }
                    break;
                }
                default:
                    std::cout << "incorrect choice\n";
                }

                std::cout << "\npress Enter to continue";
                clear_input_buffer();
                std::cin.get();
            }
            break;
        }

        case 4: { // удаление рейса
            int id;
            std::cout << "enter id for removing: ";
            std::cin >> id;
            clear_input_buffer();
            db.delete_flight(id);
            break;
        }

        case 5: { // редактирование рейса
            int id;
            std::string flight_number, plane_name;
            std::string departure_time, arrival_time;

            std::cout << "enter id for editing: ";
            std::cin >> id;
            clear_input_buffer();

            // поиск по id
            Flight* flight_to_edit = nullptr;
            for (int i = 0; i < db.get_count(); i++) {
                Flight* f = db.get_flight_by_index(i);
                if (f && f->id == id) {
                    flight_to_edit = f;
                    break;
                }
            }

            if (!flight_to_edit) {
                std::cout << "flight with id: " << id << " not found\n";
                break;
            }

            flight_to_edit->print_details();

            std::cout << "new flight number (enter '.' to skip): ";
            std::getline(std::cin, flight_number);
            if (flight_number == ".") flight_number = "";

            std::cout << "new plane name (enter '.' to skip): ";
            std::getline(std::cin, plane_name);
            if (plane_name == ".") plane_name = "";

            while (1) {
                std::cout << "enter new departure time (hh:mm) or '.' to skip: ";
                std::getline(std::cin, departure_time);
                if (departure_time == ".") {
                    departure_time = "";
                    break;
                }
                if (validate_time_format(departure_time)) {
                    break;
                }
                else {
                    std::cout << "err: incorrect format\n";
                }
            }

            while (1) {
                std::cout << "enter new arrival time (hh:mm) or '.' to skip: ";
                std::getline(std::cin, arrival_time);
                if (arrival_time == ".") {
                    arrival_time = "";
                    break;
                }
                if (validate_time_format(arrival_time)) {
                    break;
                }
                else {
                    std::cout << "err: incorrect format\n";
                }
            }

            db.edit_flight(id, flight_number, plane_name, departure_time, arrival_time);
            break;
        }

        case 6: // сохранение в файл
            db.save_to_file("flights.txt");
            break;

        case 7: // выход
            db.save_to_file("flights.txt");
            std::cout << "Goodbye!\n";
            return 0;

        default:
            std::cout << "incorrect number\n";
        }
    }

    return 0;
}