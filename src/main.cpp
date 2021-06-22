#include "tools.h"
#include <future>
#include <cstdio>

#define MAX_CLIENTS 6

void poll_peers(client clients[MAX_CLIENTS], ENetHost* server, ENetEvent &event){
    while (enet_host_service(server, &event, 10) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:{
                /* Store any relevant client information here. */
                // event.peer->data = "Client information";
                client new_player;
                new_player.address = event.peer->address;
                printf("<Adding new player>\n");
                //use event.peer new_player to add player
                printf("<A new client connected from %d:%d.>\n",event.peer->address.host,event.peer->address.port);
                //send data of new player to other clients                    
                break;
            }
            case ENET_EVENT_TYPE_RECEIVE:{
                /* Clean up the packet now that we're done using it. */

                Data parsed = parse((char*)(event.packet->data),' ');
                //qsort then binary search
                
                // new_player.stats.name = parsed.content[0];
                // new_player.stats.id = parsed.content[1];
                // new_player.stats.xp = stof(parsed.content[2]);
                // new_player.stats.health = stof(parsed.content[3]);
                // new_player.stats.armor = stof(parsed.content[4]);
                // new_player.stats.range = stof(parsed.content[5]);
                // new_player.stats.damage = stof(parsed.content[6]);
                
                enet_packet_destroy (event.packet);
                break;
            }   
            case ENET_EVENT_TYPE_DISCONNECT:{
                printf("<%d:%d disconnected...\n>",event.peer->address.host,event.peer->address.port);
                // string end_data = "2 " + clients[event.peer].ID;
                // printf("<bye bye message:%s>\n",end_data.c_str());
                // for(int i = 0;i<clients.size();i++){
                //     if(&server->peers[i] != event.peer){
                //         send_packet(&server->peers[i],end_data.c_str());
                //         enet_host_flush(server);
                //     }
                // }
                /* Reset the peer's client information. */
                printf("<Removing player>\n");
                // clients.erase(event.peer);                
                event.peer->data = NULL;
                break;
            }
            case ENET_EVENT_TYPE_NONE:{
                printf("<Waiting...>\n");
                break;
            }
        }
    }
}

int main() {
    bool run = true;
    client clients[MAX_CLIENTS];
    if(enet_initialize()!=0){
        printf("<An error occurred while initializing ENet...>\n");
        return 1;
    }
    atexit(enet_deinitialize);
    
    ENetAddress address;
    ENetHost* server;
    ENetEvent event;

    string command;

    address.host = ENET_HOST_ANY;
    address.port = 25570;
    server = enet_host_create(&address, MAX_CLIENTS, 2,0,0);

    if(server == NULL){
        printf("<Error: can't create ENet server host...>\n");
        return 1;
    }
    
    printf("<SUCCESS>\n<Server Started>\n");

    while(run){
        poll_peers(clients,server,event);
        // printf("<Running>\n");
    }

    enet_host_destroy(server);

    return EXIT_SUCCESS;
}

