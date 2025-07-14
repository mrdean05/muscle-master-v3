#ifndef __SQLITE_H__
#define __SQLITE_H__

#include "sqlite3.h"
#include "Fatfs.h"
#include "StateEnums.h"
#include <string>

namespace APP {


    using productDesc = Bsp::product_obj_t;
    class Sqlite {
        private:
            sqlite3 *db;
            std::string DBfileName;
            static bool sqliteInitialize;
            static int callback(void *data, int argc, char **argv, char **azColName);
            int db_open(const char *filename, sqlite3 **db);
            int db_exec(sqlite3 *db, const char *sql, productDesc *product);
            int db_exec(sqlite3 *db, const char *sql);
        public:
            Sqlite();
            void openDataBase(std::string fileName);
            void createInventoryItemTable( void );
            void addItemToInventoryDB( productDesc *product );
            int8_t searchItems(const char* itemBarcode, productDesc *product);
            void closeDataBase( void );
            void deleteDBFile( void );
            ~Sqlite();

    };
}

#endif