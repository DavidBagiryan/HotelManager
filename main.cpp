#include <iostream>
#include <map>
#include <deque>
#include <unordered_set>

using namespace std;

class HotelManager {
public:
    void Book(int time, string& hotel, int client_id, int room_count) {
        deque<tuple<int, int>> client_room;
        client_room.emplace_back(client_id, room_count);
        
        map<int, deque<tuple<int, int>>> time_client_room;
        time_client_room[time] = client_room;
        
        hotel_time_client_room_[hotel].emplace_back(time_client_room);
        current_time_ = time;
        hotel_time_[hotel].push_back(time);
    }
    
    int ComputeClientCount(string& hotel_name) {
        return get<0>(ComputeClientRoomCount(hotel_name));
    }
    
    int ComputeRoomCount(string& hotel_name) {
        return get<1>(ComputeClientRoomCount(hotel_name));
    }

private:
    int current_time_;
    map<string, deque<int>> hotel_time_;
    map<string, deque<map<int, deque<tuple<int, int>>>>> hotel_time_client_room_;
    
    tuple<int, int> ComputeClientRoomCount(string& hotel_name) {
        if (hotel_time_.find(hotel_name) == hotel_time_.end()) {
            return {0, 0};
        }
        
        int day_time = 86400;
        unordered_set<int> actual_time;
        
        deque<int> time_reserve = hotel_time_[hotel_name];
        for (const int time: time_reserve) {
            if (current_time_ - day_time < time && time <= current_time_) {
                actual_time.insert(time);
            }
        }
        
        if (actual_time.empty()) {
            return {0, 0};
        }
        
        int count_client = 0;
        int count_room = 0;
        
        unordered_set<int> client;
        for (const int time: actual_time) {
            for (map<int, deque<tuple<int, int>>>& time_client_room: hotel_time_client_room_[hotel_name]) {
                deque<tuple<int, int>> client_room_deq = time_client_room[time];
                for (const auto client_room: client_room_deq) {
                    if (client.find(get<0>(client_room)) == client.end()) {
                        client.insert(get<0>(client_room));
                        ++count_client;
                    }
                    count_room += get<1>(client_room);
                }
            }
        }
        
        return {count_client, count_room};
    }
};

int main() {
    HotelManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;

        if (query_type == "BOOK") {
            int64_t time;
            cin >> time;
            string hotel_name;
            cin >> hotel_name;
            int client_id, room_count;
            cin >> client_id >> room_count;
            manager.Book(time, hotel_name, client_id, room_count);
        } else {
            string hotel_name;
            cin >> hotel_name;
            if (query_type == "CLIENTS") {
                cout << manager.ComputeClientCount(hotel_name) << "\n";
            } else if (query_type == "ROOMS") {
                cout << manager.ComputeRoomCount(hotel_name) << "\n";
            }
        }
    }

    return 0;
}