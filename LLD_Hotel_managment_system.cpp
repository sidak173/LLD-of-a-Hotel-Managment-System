#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define M 1000000007
#define fio                           \
    ios_base::sync_with_stdio(false); \
    cin.tie(NULL);
#define d(x) cout << #x << " " << x << "\n";
#define min(x1, x2) (x1 > x2 ? x2 : x1)
#define max(x1, x2) (x1 < x2 ? x2 : x1)
#define min3(x1, x2, x3) (x3 > min(x1, x2) ? min(x2, x1) : x3)
#define max3(x1, x2, x3) (x3 < max(x1, x2) ? max(x1, x2) : x3)
#define ll long long int
#define ul unsigned long long int
#define p pair<ll, ll>
#define ld long double
#define dv(v)                            \
    cerr << #v << "  ";                  \
    for (int i = 0; i < (v).size(); i++) \
        cerr << v[i] << " ";             \
    cerr << "\n";
#define inf LONG_LONG_MAX
#define mp(x, y) make_pair(x, y)
#define oset tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update>
using namespace __gnu_pbds;
using namespace std;

enum RoomStyle
{
    Standard,
    Delux,
    Family_Suite
};
enum RoomStatus
{
    Available,
    Not_Available
};

class Hotel;
class Room;
class Person;
class Guest;
class Receptionist;

class Room
{
public:
    Room(int rid, RoomStyle rs, Hotel *h);
    int id;
    RoomStyle room_style;
    RoomStatus room_status;
    Hotel *hotel;
    Guest *guest;
};

class Hotel
{
public:
    Hotel(string hname, int hid)
    {
        name = hname;
        id = hid;
    }
    string name;
    int id;
    vector<Room *> rooms;
};

class Person
{
public:
    string name;
    Person(string person_name)
    {
        name = person_name;
    }
};

class Guest : public Person
{
public:
    Guest(string name) : Person(name)
    {
    }
    vector<Room *> rooms;
    friend vector<Room> search_room(Hotel, RoomStyle);
    friend bool book_room(Room *, Guest *);
    friend bool cancel_room(Room *, Guest *);
};

class Receptionist : public Person
{
public:
    Receptionist(string name) : Person(name)
    {
        hotel = NULL;
    }
    Hotel *hotel;
    friend vector<Room> search_room(Hotel, RoomStyle);
    friend bool book_room(Room *, Guest *);
    friend bool cancel_room(Room *, Guest *);
};

vector<Room> search_room(Hotel hotel, RoomStyle room_style)
{
    vector<Room> all_rooms;
    for (auto room : hotel.rooms)
    {
        if (room->room_style == room_style && room->room_status == Available)
        {
            all_rooms.push_back(*room);
        }
    }
    return all_rooms;
}

bool book_room(Room *room, Guest *guest)
{
    if (room->room_status == Available)
    {
        room->room_status = Not_Available;
        room->guest = guest;
        guest->rooms.push_back(room);
        return 1;
    }
    return 0;
}

bool cancel_room(Room *room, Guest *guest)
{
    if (room->room_status == Not_Available && room->guest == guest)
    {
        room->room_status = Available;
        room->guest = NULL;
        int i = 0;
        for (auto room_of_guest : guest->rooms)
        {
            if (room_of_guest == room)
            {
                guest->rooms.erase(guest->rooms.begin() + i);
                return 1;
            }
            ++i;
        }
    }
    return 0;
}

Room ::Room(int rid, RoomStyle rs, Hotel *h)
{
    id = rid;
    room_status = Available;
    room_style = rs;
    hotel = h;
    guest = NULL;
    h->rooms.push_back(this);
}

int main()
{
    Hotel my_hotel = Hotel("my_hotel", 123);
    Room room1 = Room(1, Delux, &my_hotel);
    Room room2 = Room(2, Delux, &my_hotel);
    Room room3 = Room(3, Standard, &my_hotel);
    Room room4 = Room(4, Family_Suite, &my_hotel);
    Guest guest1 = Guest("sidak");
    Receptionist recep = Receptionist("cred");
    vector<Room> rooms = search_room(my_hotel, Delux);
    d(rooms.size());
    book_room(&room1, &guest1);
    rooms = search_room(my_hotel, Delux);
    d(rooms.size());
    cancel_room(&room1, &guest1);
    rooms = search_room(my_hotel, Delux);
    d(rooms.size());
}