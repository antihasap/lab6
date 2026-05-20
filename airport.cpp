#include "airport.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <limits>

// реализация Flight
Flight::Flight(int id, const std::string& flight_num, const std::string& plane,
               const std::string& depart, const std::string& arrive)
    : id(id), flight_number(flight_num), plane_name(plane),
      departure_time(depart), arrival_time(arrive), next(nullptr) {}

void Flight::print_details() const {
    std::cout << "id: " << id << "\n";
    std::cout << "flight number: " << flight_number << "\n";
    std::cout << "plane: " << plane_name << "\n";
    std::cout << "departure time: " << departure_time << "\n";
    std::cout << "arrival time: " << arrival_time << "\n";
}

void Flight::print_short() const {
    std::cout << id << " " << flight_number << " " << plane_name
              << " " << departure_time << " " << arrival_time << "\n";
}

// реализация Airport через связный список
Airport::Airport() : head(nullptr), count(0), next_id(1) {}

Airport::~Airport() {
    Flight* current = head;
    while (current != nullptr) {
        Flight* temp = current;
        current = current->next;
        delete temp;
    }
}

void Airport::add_flight(const std::string& flight_number, const std::string& plane_name,
                         const std::string& departure_time, const std::string& arrival_time) {
    Flight* new_flight = new Flight(next_id++, flight_number, plane_name, departure_time, arrival_time);
    if (head == nullptr) {
        head = new_flight;
    } else {
        Flight* curr = head;
        while (curr->next != nullptr) {
            curr = curr->next;
        }
        curr->next = new_flight;
    }
    count++;
    std::cout << "flight '" << flight_number << "' added with id: " << next_id - 1 << "\n";
}

void Airport::print_flights() const {
    if (count == 0) {
        std::cout << "empty\n";
        return;
    }
    Flight* curr = head;
    while (curr != nullptr) {
        curr->print_short();
        curr = curr->next;
    }
}

Flight* Airport::find_flight_by_number(const std::string& flight_number) const {
    Flight* curr = head;
    while (curr != nullptr) {
        if (curr->flight_number == flight_number) {
            return curr;
        }
        curr = curr->next;
    }
    return nullptr;
}

void Airport::find_flights_by_plane(const std::string& plane_name) const {
    bool found = false;
    std::cout << "\nflights with plane '" << plane_name << "':\n";
    Flight* curr = head;
    while (curr != nullptr) {
        if (curr->plane_name.find(plane_name) != std::string::npos) {
            std::cout << curr->id << " " << curr->flight_number
                      << " " << curr->departure_time << " " << curr->arrival_time << "\n";
            found = true;
        }
        curr = curr->next;
    }
    if (!found) {
        std::cout << "no flights found with plane: " << plane_name << "\n";
    }
}

void Airport::find_flights_by_departure_time(const std::string& departure_time) const {
    bool found = false;
    std::cout << "\nflights departing at: " << departure_time << ":\n";
    Flight* curr = head;
    while (curr != nullptr) {
        if (curr->departure_time == departure_time) {
            std::cout << curr->id << " " << curr->flight_number
                      << " " << curr->plane_name << " " << curr->arrival_time << "\n";
            found = true;
        }
        curr = curr->next;
    }
    if (!found) {
        std::cout << "no flights found departing at: " << departure_time << "\n";
    }
}

void Airport::find_flights_by_arrival_time(const std::string& arrival_time) const {
    bool found = false;
    std::cout << "\nflights arriving at: " << arrival_time << "\n";
    Flight* curr = head;
    while (curr != nullptr) {
        if (curr->arrival_time == arrival_time) {
            std::cout << curr->id << " " << curr->flight_number
                      << " " << curr->plane_name << " " << curr->departure_time << "\n";
            found = true;
        }
        curr = curr->next;
    }
    if (!found) {
        std::cout << "no flights found arriving at: " << arrival_time << "\n";
    }
}

void Airport::delete_flight(int id) {
    if (head == nullptr) {
        std::cout << "empty\n";
        return;
    }

    Flight* curr = head;
    Flight* prev = nullptr;

    while (curr != nullptr && curr->id != id) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == nullptr) {
        std::cout << "flight with id: " << id << " not found\n";
        return;
    }

    if (prev == nullptr) {
        // Удаляем голову
        head = curr->next;
    } else {
        prev->next = curr->next;
    }
    delete curr;
    count--;
    std::cout << "flight with id " << id << " deleted\n";
}

void Airport::edit_flight(int id, const std::string& flight_number,
                          const std::string& plane_name, const std::string& departure_time,
                          const std::string& arrival_time) {
    Flight* curr = head;
    while (curr != nullptr && curr->id != id) {
        curr = curr->next;
    }

    if (curr == nullptr) {
        std::cout << "flight with id: " << id << " not found\n";
        return;
    }

    if (!flight_number.empty()) curr->flight_number = flight_number;
    if (!plane_name.empty()) curr->plane_name = plane_name;
    if (!departure_time.empty()) curr->departure_time = departure_time;
    if (!arrival_time.empty()) curr->arrival_time = arrival_time;

    std::cout << "flight with id " << id << " updated\n";
}

void Airport::save_to_file(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file) {
        std::cout << "err: cannot open file for writing\n";
        return;
    }

    Flight* curr = head;
    while (curr != nullptr) {
        file << curr->id << "|" << curr->flight_number << "|"
             << curr->plane_name << "|" << curr->departure_time << "|"
             << curr->arrival_time << "\n";
        curr = curr->next;
    }
    std::cout << "saved to " << filename << "\n";
}

void Airport::load_from_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cout << "file '" << filename << "' not found, starting empty\n";
        return;
    }

    while (head != nullptr) {
        Flight* temp = head;
        head = head->next;
        delete temp;
    }
    count = 0;
    next_id = 1;
    int max_id = 0;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> parts;

        while (std::getline(ss, token, '|')) {
            parts.push_back(token);
        }

        if (parts.size() == 5) {
            int id = std::stoi(parts[0]);
            Flight* new_flight = new Flight(id, parts[1], parts[2], parts[3], parts[4]);
            if (head == nullptr) {
                head = new_flight;
            } else {
                Flight* curr = head;
                while (curr->next != nullptr) {
                    curr = curr->next;
                }
                curr->next = new_flight;
            }
            count++;
            if (id > max_id) max_id = id;
        }
    }
    next_id = max_id + 1;
    std::cout << "loaded " << count << " flight(s) from " << filename << "\n";
}

Flight* Airport::get_flight_by_index(int index) const {
    if (index < 0 || index >= count) {
        return nullptr;
    }
    Flight* curr = head;
    for (int i = 0; i < index; ++i) {
        curr = curr->next;
    }
    return curr;
}

bool validate_time_format(const std::string& time) {
    if (time.length() != 5) return false;
    if (time[2] != ':') return false;
    for (int i = 0; i < 5; ++i) {
        if (i != 2 && !std::isdigit(static_cast<unsigned char>(time[i]))) {
            return false;
        }
    }
    int hour = (time[0] - '0') * 10 + (time[1] - '0');
    int minute = (time[3] - '0') * 10 + (time[4] - '0');
    return (hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59);
}

void clear_input_buffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void print_menu() {
    std::cout << "\nMENU\n";
    std::cout << "1. show all flights\n";
    std::cout << "2. add a new flight\n";
    std::cout << "3. find flights\n";
    std::cout << "4. remove flight\n";
    std::cout << "5. edit flight\n";
    std::cout << "6. save all\n";
    std::cout << "7. exit\n";
    std::cout << "Choice: ";
}

void print_search_menu() {
    std::cout << "\nSEARCH FLIGHTS\n";
    std::cout << "1. search by flight number\n";
    std::cout << "2. search by plane name\n";
    std::cout << "3. search by departure time\n";
    std::cout << "4. search by arrival time\n";
    std::cout << "5. back to main menu\n";
    std::cout << "choose search option: ";
}
