//
// Created by leon on 03.05.24.
//

#include "PGConnection.h"

PGConnection::PGConnection() {
    _connection.reset(PQsetdbLogin(_dbHost.c_str(), std::to_string(_dbPort).c_str(), nullptr, nullptr, _dbName.c_str(), _dbUser.c_str(), _dbPass.c_str()), &PQfinish);

    if(PQstatus(_connection.get()) != CONNECTION_OK && PQsetnonblocking(_connection.get(), 1) != 0) {
        throw std::runtime_error{PQerrorMessage(_connection.get())};
    }
}

std::shared_ptr<PGconn> PGConnection::connection() const {
    return _connection;
}

void PGConnection::establish_connection() {

}