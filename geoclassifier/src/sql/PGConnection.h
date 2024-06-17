#pragma once

#include "memory"
#include "mutex"
#include "postgresql/libpq-fe.h"

class PGConnection {
public:
    PGConnection();
    std::shared_ptr<PGconn> connection() const;
private:
    void establish_connection();

    std::string _dbHost{"localhost"};
    int _dbPort{5432};
    std::string _dbName{"demo"};
    std::string _dbUser{"postgres"};
    std::string _dbPass{"postgres"};

    std::shared_ptr<PGconn> _connection;
};