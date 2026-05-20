#ifndef AIRPORT_HPP
#define AIRPORT_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <limits>

struct Flight {
    int id;
    std::string flight_number;
    std::string plane_name;
    std::string departure_time;
    std::string arrival_time;
    Flight* next; // указатель на следующий элемент в списке

    Flight(int id, const std::string& flight_num, const std::string& plane,
           const std::string& depart, const std::string& arrive);
};

// класс базы данных (односвязный список)
class Airport {
private:
    Flight* head;
    int count;
    int next_id;

public:
    Airport();
    ~Airport();

    Airport(const Airport&) = delete;
    Airport& operator=(const Airport&) = delete;

    void add_flight(const std::string& flight_number, const std::string& plane_name,
                    const std::string& departure_time, const std::string& arrival_time);
    void print_flights() const;
    Flight* find_flight_by_number(const std::string& flight_number) const;
    void find_flights_by_plane(const std::string& plane_name) const;
    void find_flights_by_departure_time(const std::string& departure_time) const;
    void find_flights_by_arrival_time(const std::string& arrival_time) const;
    void delete_flight(int id);
    void edit_flight(int id, const std::string& flight_number,
                     const std::string& plane_name, const std::string& departure_time,
                     const std::string& arrival_time);

    void save_to_file(const std::string& filename) const;
    void load_from_file(const std::string& filename);

    int get_count() const { return count; }
    Flight* get_flight_by_index(int index) const;
};

bool validate_time_format(const std::string& time);
void clear_input_buffer();
void print_menu();
void print_search_menu();

#endif
