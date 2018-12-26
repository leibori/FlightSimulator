#include "OpenServerCommand.h"

using namespace std;

struct openDataServerParams {
    SymbolTable *symbolTable;
    int port;
    int frequency;
};

void OpenServerCommand::execute() {
    struct openDataServerParams param;
    struct openDataServerParams* params;
    param.symbolTable = this->symbolTable;
    param.frequency = this->frequency;
    param.port = this->port;
    params = &param;

    std::thread thread1(executeInThread, symbolTable, port, frequency);
    thread1.detach();
    /*pthread_t serverThread;
    pthread_create(&serverThread, nullptr, executeInThread, params);*/
}


static void* executeInThread(SymbolTable* symbolTable, int port, int freqency) {
    //struct openDataServerParams* params = (struct openDataServerParams*) args;
    int sock_fd, newsockfd, clilen;
    struct sockaddr_in serv_addr, cli_addr;
    /* First call to m_socket() function */
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (sock_fd < 0) {
        perror("ERROR opening m_socket");
        exit(1);
    }

    /* Initialize m_socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons((uint16_t) port);

    /* Now bind the host address using bind() call.*/
    if (bind(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
    }

    /* Now start listening for the clients, here process will
       * go in sleep mode and will wait for the incoming connection
    */

    listen(sock_fd,1);

    //initializeBindValues(params);

    clilen = sizeof(cli_addr);

    /* Accept actual connection from the client */
    newsockfd = accept(sock_fd, (struct sockaddr *)&cli_addr, (socklen_t*)&clilen);

    if (newsockfd < 0) {
        perror("ERROR on accept");
        exit(1);
    }

    constantRead(newsockfd, symbolTable);
}

/*void initializeBindValues(openDataServerParams* params) {
    //std::lock_guard<std::mutex> guard(g_pages_mutex);
    params->symbolTable->getBindValuesTable()["/instrumentation/airspeed-indicator/indicated-speed-kt"] = 0;
    params->symbolTable->getBindValuesTable().insert(pair<string,double>("/instrumentation/altimeter/indicated-altitude-ft",0));
    params->symbolTable->getBindValuesTable().insert(pair<string,double>("/instrumentation/altimeter/pressure-alt-ft",0));
    params->symbolTable->getBindValuesTable().insert(pair<string,double>("/instrumentation/attitude-indicator/indicated-pitch-deg",0));
    params->symbolTable->getBindValuesTable().insert(pair<string,double>("/instrumentation/attitude-indicator/indicated-roll-deg",0));
    params->symbolTable->getBindValuesTable().insert(pair<string,double>("/instrumentation/attitude-indicator/internal-pitch-deg",0));
    params->symbolTable->getBindValuesTable().insert(pair<string,double>("/instrumentation/attitude-indicator/internal-roll-deg",0));
    params->symbolTable->getBindValuesTable().insert(pair<string,double>("/instrumentation/encoder/indicated-altitude-ft",0));
    params->symbolTable->getBindValuesTable().insert(pair<string,double>("/instrumentation/encoder/pressure-alt-ft",0));
    params->symbolTable->getBindValuesTable().insert(pair<string,double>("/instrumentation/gps/indicated-altitude-ft",0));
    params->symbolTable->getBindValuesTable().insert(pair<string,double>("/instrumentation/gps/indicated-ground-speed-kt",0));
    params->symbolTable->getBindValuesTable().insert(pair<string,double>("/instrumentation/gps/indicated-vertical-speed",0));
    params->symbolTable->getBindValuesTable().insert(pair<string,double>("/instrumentation/heading-indicator/indicated-heading-deg",0));
    params->symbolTable->getBindValuesTable().insert(pair<string,double>("/instrumentation/magnetic-compass/indicated-heading-deg",0));
    params->symbolTable->getBindValuesTable().insert(pair<string,double>("/instrumentation/slip-skid-ball/indicated-slip-skid",0));
    params->symbolTable->getBindValuesTable().insert(pair<string,double>("/instrumentation/turn-indicator/indicated-turn-rate",0));
    params->symbolTable->getBindValuesTable().insert(pair<string,double>("/instrumentation/vertical-speed-indicator/indicated-speed-fpm",0));
    params->symbolTable->getBindValuesTable().insert(pair<string,double>("/controls/flight/aileron",0));
    params->symbolTable->getBindValuesTable().insert(pair<string,double>("/controls/flight/elevator",0));
    params->symbolTable->getBindValuesTable().insert(pair<string,double>("/controls/flight/rudder",0));
    params->symbolTable->getBindValuesTable().insert(pair<string,double>("/controls/flight/flaps",0));
    params->symbolTable->getBindValuesTable().insert(pair<string,double>("/controls/engines/engine/throttle",0));
    params->symbolTable->getBindValuesTable().insert(pair<string,double>("/engines/engine/rpm",0));
}*/

void constantRead(int newsockfd, SymbolTable* symbolTable) {
    while (true){
        char buffer[255];
        ssize_t bytes_read;
        bytes_read = read(newsockfd, buffer, 255);
        if (bytes_read < 0) {
            __throw_bad_exception();
        } else if (bytes_read == 0)   {
            // TODO connection closed
        } else  {
            Lexer lexer(buffer);
            vector<string> values = lexer.splitIt(buffer, ",");
            updateBindValues(values, symbolTable);
            cout << buffer;
            this_thread::__sleep_for(chrono::seconds(1), chrono::nanoseconds(0));
        }
    }
}

void updateBindValues(vector<string> values, SymbolTable* symbolTable) {
    symbolTable->updateBindValuesTable("/instrumentation/airspeed-indicator/indicated-speed-kt", stod(values[0].c_str()));
    symbolTable->updateBindValuesTable("/instrumentation/altimeter/indicated-altitude-ft", stod(values[1].c_str()));
    symbolTable->updateBindValuesTable("/instrumentation/altimeter/pressure-alt-ft", stod(values[2].c_str()));
    symbolTable->updateBindValuesTable("/instrumentation/attitude-indicator/indicated-pitch-deg", stod(values[3].c_str()));
    symbolTable->updateBindValuesTable("/instrumentation/attitude-indicator/indicated-roll-deg", stod(values[4].c_str()));
    symbolTable->updateBindValuesTable("/instrumentation/attitude-indicator/internal-pitch-deg", stod(values[5].c_str()));
    symbolTable->updateBindValuesTable("/instrumentation/attitude-indicator/internal-roll-deg", stod(values[6].c_str()));
    symbolTable->updateBindValuesTable("/instrumentation/encoder/indicated-altitude-ft", stod(values[7].c_str()));
    symbolTable->updateBindValuesTable("/instrumentation/encoder/pressure-alt-ft", stod(values[8].c_str()));
    symbolTable->updateBindValuesTable("/instrumentation/gps/indicated-altitude-ft", stod(values[9].c_str()));
    symbolTable->updateBindValuesTable("/instrumentation/gps/indicated-ground-speed-kt", stod(values[10].c_str()));
    symbolTable->updateBindValuesTable("/instrumentation/gps/indicated-vertical-speed", stod(values[11].c_str()));
    symbolTable->updateBindValuesTable("/instrumentation/heading-indicator/indicated-heading-deg", stod(values[12].c_str()));
    symbolTable->updateBindValuesTable("/instrumentation/magnetic-compass/indicated-heading-deg", stod(values[13].c_str()));
    symbolTable->updateBindValuesTable("/instrumentation/slip-skid-ball/indicated-slip-skid", stod(values[14].c_str()));
    symbolTable->updateBindValuesTable("/instrumentation/turn-indicator/indicated-turn-rate", stod(values[15].c_str()));
    symbolTable->updateBindValuesTable("/instrumentation/vertical-speed-indicator/indicated-speed-fpm", stod(values[16].c_str()));
    symbolTable->updateBindValuesTable("/controls/flight/aileron", stod(values[17].c_str()));
    symbolTable->updateBindValuesTable("/controls/flight/elevator", stod(values[18].c_str()));
    symbolTable->updateBindValuesTable("/controls/flight/rudder", stod(values[19].c_str()));
    symbolTable->updateBindValuesTable("/controls/flight/flaps", stod(values[20].c_str()));
    symbolTable->updateBindValuesTable("/controls/engines/engine/throttle", stod(values[21].c_str()));
    symbolTable->updateBindValuesTable("/engines/engine/rpm", stod(values[22].c_str()));
}
