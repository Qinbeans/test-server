#define ENET_IMPLIMENTATION
#include <enet/enet.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>

#define MAX_CLIENTS 6

using std::string;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;
using std::cin;
using std::map;
using std::pair;

struct client{
    string ID;
    ENetAddress address;
};

string data_to_string(unsigned char* str){
    return string((char*)str);
}

void parse(string input, vector<string> &data)
{
    string temp;
    int out_index = 0;
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] != 32)
        {
            temp += input[i];
        }
        else
        {
            data.push_back(temp);
            out_index++;
            temp = "";
        }
    }
    data.push_back(temp);
}

void send_packet(ENetPeer* server, string data){
    ENetPacket* packet = enet_packet_create(data.c_str(),data.size()+1,ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(server,1,packet);
}

void proc_comm(string& command,bool& run){
    std::getline(cin,command);
    vector<string> dig_com;
    parse(command,dig_com);
    if(dig_com[0] == "/exit" || dig_com[0] == "/q"){
        cout<<"Bye Bye..."<<endl;
        run = false;
    }
}

void poll_peers(map<ENetPeer*,client> &clients, ENetHost* server, ENetEvent &event){
    while (enet_host_service(server, &event, 10) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:{
                /* Store any relevant client information here. */
                // event.peer->data = "Client information";
                client new_player;
                new_player.address = event.peer->address;
                cerr<<"<Adding new player>"<<endl;                    
                clients.insert(pair<ENetPeer*,client>(event.peer,new_player));
                cerr<<"A new client connected from "<<event.peer->address.host<<":"<<(event.peer->address.port)<<"."<<endl;
                //send data of new player to other clients                    
                break;
            }
            case ENET_EVENT_TYPE_RECEIVE:{
                // cerr<<"<Received>"<<endl;
                // cerr<<"A packet of length "<<event.packet->dataLength<<" containing "<<event.packet->data<<" was received from "<<event.peer->data<<" on channel "<<event.channelID<<endl;
                /* Clean up the packet now that we're done using it. */
                string p_data = data_to_string(event.packet->data);
                vector<string> temp_data;
                parse(p_data, temp_data);
                if(std::stoi(temp_data[0]) == 0){
                    clients[event.peer].ID = temp_data[2];
                    enet_host_flush(server);
                }
                for(int i = 0;i<clients.size();i++){
                    if(&server->peers[i] != event.peer){
                        send_packet(&server->peers[i],p_data);
                        enet_host_flush(server);
                    }
                }
                enet_packet_destroy (event.packet);
                break;
            }   
            case ENET_EVENT_TYPE_DISCONNECT:{
                cerr<<event.peer->address.host<<":"<<event.peer->address.port<<" disconnected..."<<endl;
                string end_data = "2 " + clients[event.peer].ID;
                cerr<<"bye bye message:"<<end_data<<endl;
                for(int i = 0;i<clients.size();i++){
                    if(&server->peers[i] != event.peer){
                        send_packet(&server->peers[i],end_data.c_str());
                        enet_host_flush(server);
                    }
                }
                /* Reset the peer's client information. */
                cerr<<"<Removing player>"<<endl;
                clients.erase(event.peer);                
                event.peer->data = NULL;
                break;
            }
            case ENET_EVENT_TYPE_NONE:{
                cout<<"Waiting..."<<endl;
                break;
            }
        }
    }
}

int main() {
    bool run = true;
    map<ENetPeer*,client> clients;
    if(enet_initialize()!=0){
        cerr<<"An error occurred while initializing ENet..."<<endl;
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
        cerr<<"<Error: can't create ENet server host...>"<<endl;
        return 1;
    }
    
    cout<<"<SUCCESS>\n<Server Started>"<<endl;

    while(run){
        
        #pragma omp parallel num_threads(1)
        #pragma omp single
        {
            try{
                #pragma omp task
                poll_peers(clients,server,event);
            }catch(const std::bad_alloc&){
                cerr<<"<Bad Alloc>"<<endl;
            }
            // #pragma omp task
            // proc_comm(command,run);
            #pragma omp taskwait
        }
    }

    enet_host_destroy(server);

    return EXIT_SUCCESS;
}

