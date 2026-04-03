#include "airport.hpp"

Flight::Flight(int id, const std::string& flight_num, const std::string& plane,
    const std::string& depart, const std::string& arrive)
    : id(id), flight_number(flight_num), plane_name(plane),
    departure_time(depart), arrival_time(arrive) {}

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

Airport::Airport() : next_id(1) {}

void Airport::add_flight(const std::string& flight_number, const std::string& plane_name,
    const std::string& departure_time, const std::string& arrival_time) {
    flights.push_back(Flight(next_id++, flight_number, plane_name, departure_time, arrival_time));
    std::cout << "flight '" << flight_number << "' added with id: " << next_id - 1 << "\n";
}

void Airport::print_flights() const {
    if (flights.empty()) {
        std::cout << "empty\n";
        return;
    }

    for (const auto& flight : flights) {
        flight.print_short();
    }
}

Flight* Airport::find_flight_by_number(const std::string& flight_number) {
    for (auto& flight : flights) {
        if (flight.flight_number == flight_number) {
            return &flight;
        }
    }
    return nullptr;
}

void Airport::find_flights_by_plane(const std::string& plane_name) const {
    bool found = false;
    std::cout << "\nflights with plane '" << plane_name << "':\n";

    for (const auto& flight : flights) {
        if (flight.plane_name.find(plane_name) != std::string::npos) {
            std::cout << flight.id << " " << flight.flight_number
                << " " << flight.departure_time << " " << flight.arrival_time << "\n";
            found = true;
        }
    }

    if (!found) {
        std::cout << "no flights found with plane: " << plane_name << "\n";
    }
}

void Airport::find_flights_by_departure_time(const std::string& departure_time) const {
    bool found = false;
    std::cout << "\nflights departing at: " << departure_time << ":\n";

    for (const auto& flight : flights) {
        if (flight.departure_time == departure_time) {
            std::cout << flight.id << "  " << flight.flight_number
                << " " << flight.plane_name << " " << flight.arrival_time << "\n";
            found = true;
        }
    }

    if (!found) {
        std::cout << "no flights found departing at: " << departure_time << "\n";
    }
}

void Airport::find_flights_by_arrival_time(const std::string& arrival_time) const {
    bool found = false;
    std::cout << "\nflights arriving at: " << arrival_time << "\n";

    for (const auto& flight : flights) {
        if (flight.arrival_time == arrival_time) {
            std::cout << flight.id << " " << flight.flight_number
                << " " << flight.plane_name << " " << flight.departure_time << "\n";
            found = true;
        }
    }

    if (!found) {
        std::cout << "no flights found arriving at: " << arrival_time << "\n";
    }
}

void Airport::delete_flight(int id) {
    if (flights.empty()) {
        std::cout << "empty\n";
        return;
    }

    auto it = std::find_if(flights.begin(), flights.end(),
        [id](const Flight& f) { return f.id == id; });

    if (it != flights.end()) {
        flights.erase(it);
        std::cout << "flight with id " << id << " deleted\n";
    }
    else {
        std::cout << "flight with id: " << id << " not found\n";
    }
}

void Airport::edit_flight(int id, const std::string& flight_number,
    const std::string& plane_name, const std::string& departure_time,
    const std::string& arrival_time) {
    auto it = std::find_if(flights.begin(), flights.end(),
        [id](const Flight& f) { return f.id == id; });

    if (it == flights.end()) {
        std::cout << "flight with id: " << id << " not found\n";
        return;
    }

    if (!flight_number.empty()) it->flight_number = flight_number;
    if (!plane_name.empty()) it->plane_name = plane_name;
    if (!departure_time.empty()) it->departure_time = departure_time;
    if (!arrival_time.empty()) it->arrival_time = arrival_time;

    std::cout << "flight with id " << id << " updated\n";
}

void Airport::save_to_file(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file) {
        std::cout << "err: cannot open file\n";
        return;
    }

    for (const auto& flight : flights) {
        file << flight.id << "|" << flight.flight_number << "|"
            << flight.plane_name << "|" << flight.departure_time << "|"
            << flight.arrival_time << "\n";
    }

    file.close();
    std::cout << "saved to " << filename << "\n";
}

void Airport::load_from_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cout << "file '" << filename << "' not found, starting empty\n";
        return;
    }

    flights.clear();
    next_id = 1;
    std::string line;
    int max_id = 0;

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
            flights.push_back(Flight(id, parts[1], parts[2], parts[3], parts[4]));
            if (id > max_id) max_id = id;
        }
    }

    next_id = max_id + 1;
    file.close();
    std::cout << "loaded " << flights.size() << " flight(s) from " << filename << "\n";
}

Flight* Airport::get_flight_by_index(int index) {
    if (index >= 0 && index < static_cast<int>(flights.size())) {
        return &flights[index];
    }
    return nullptr;
}

bool validate_time_format(const std::string& time) {
    if (time.length() != 5) return false;
    if (time[2] != ':') return false;

    for (int i = 0; i < 5; i++) {
        if (i != 2) {
            if (!std::isdigit(static_cast<unsigned char>(time[i]))) {
                return false;
            }
        }
    }

    int hour = (time[0] - '0') * 10 + (time[1] - '0');
    int minute = (time[3] - '0') * 10 + (time[4] - '0');

    if (hour < 0 || hour > 23) return false;
    if (minute < 0 || minute > 59) return false;

    return true;
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