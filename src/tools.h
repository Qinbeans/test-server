#include <enet/enet.h>
#include <string>

using std::string;

typedef struct Stats{
   string name;
   //6 digit ID, server check if
   //atoi(id) < 0 then it will
   //assign random number for ID
   string id;
   float xp;//xp
   float speed;//spd
   float health;//hp
   float armor;//arm
   float range;//rng
   float damage;//dps
}Stats;

typedef struct client{
    ENetAddress address;
    Stats stats;
}client;

typedef struct Data{
   string* content;
   unsigned int size;
}Data;

Data parse(string in, char delim);