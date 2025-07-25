#include "Sqlite.h"
#include "esp_timer.h"
#include <string.h>

namespace APP {

    bool Sqlite::sqliteInitialize = false;
    int8_t Sqlite::itemFound = -1;
    int Sqlite::callback(void *data, int argc, char **argv, char **azColName) {
        int i = 0;

        if (data != NULL) {
            productDesc *product = (productDesc*)data;
            //printf("%s: ", (const char*)data);
            strcpy(product->product_brand, argv[0] ? argv[0] : "NULL");
            strcpy(product->product_type, argv[1] ? argv[1] : "NULL");
            strcpy(product->product_price, argv[2] ? argv[2] : "NULL");
            strcpy(product->product_weight, argv[3] ? argv[3] : "NULL");
            strcpy(product->product_barcode_id, argv[4] ? argv[4] : "NULL");
            itemFound = 0;
        }


        for (i = 0; i<argc; i++){
            printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        }
        //printf("\n");
        return 0;
    }

    int Sqlite::db_open(const char *filename, sqlite3 **db) {
        int rc = sqlite3_open(filename, db);
        if (rc) {
            printf("Can't open database: %s\n", sqlite3_errmsg(*db));
            return rc;
        } else {
            printf("Opened database successfully\n");
        }
        return rc;
    }

    int Sqlite::db_exec(sqlite3 *db, const char *sql, productDesc *product) {
        char *zErrMsg = 0;
        printf("%s\n", sql);
        int64_t start = esp_timer_get_time();
        int rc = sqlite3_exec(db, sql, callback, (void*)product, &zErrMsg);
        if (rc != SQLITE_OK) {
            printf("SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        } else {
            printf("Operation done successfully: %d\n", rc);
        }
        printf("Time taken: %lld\n", esp_timer_get_time()-start);
        return rc;
    }

    int Sqlite::db_exec(sqlite3 *db, const char *sql) {
        char *zErrMsg = 0;
        printf("%s\n", sql);
        int64_t start = esp_timer_get_time();
        int rc = sqlite3_exec(db, sql, callback, NULL, &zErrMsg);
        if (rc != SQLITE_OK) {
            printf("SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        } else {
            printf("Operation done successfully\n");
        }
        printf("Time taken: %lld\n", esp_timer_get_time()-start);
        return rc;
    }

    Sqlite::Sqlite(){
        if (!sqliteInitialize){
            sqlite3_initialize();
            sqliteInitialize = true;
        } 
    }

    Sqlite::~Sqlite(){
        if (sqliteInitialize){
            sqlite3_shutdown();
            sqliteInitialize = false;
        }
    }

    void Sqlite::createInventoryItemTable( void ){
        int rc = db_exec(db, "CREATE TABLE inventory (product_brand TEXT, product_type TEXT, product_price TEXT, product_weight TEXT, product_barcode_id TEXT, product_quantity TEXT, total_price TEXT);");
        if (rc != SQLITE_OK) {
            printf("Error create inventory database");
        }
    }

    void Sqlite::openDataBase(std::string fileName){
        DBfileName = fileName;
        db_open(DBfileName.c_str(), &db);
    }


    void Sqlite::addItemToInventoryDB( productDesc *product ){
        const char *sql = "INSERT INTO inventory "
        "(product_brand, product_type, product_price, product_weight, "
        "product_barcode_id, product_quantity, total_price) "
        "VALUES (?, ?, ?, ?, ?, ?, ?);";

        sqlite3_stmt *stmt;

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, product->product_brand, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, product->product_type, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, product->product_price, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, product->product_weight, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, product->product_barcode_id, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 6, product->product_quantity, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 7, product->total_price, -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Failed to insert data: %s\n", sqlite3_errmsg(db));
        }

        sqlite3_finalize(stmt);
        } else {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        }
    }

    void Sqlite::closeDataBase( void ){
        sqlite3_close(db);
    }


    void Sqlite::deleteDBFile( void ){
        unlink(DBfileName.c_str());
    }

    int8_t Sqlite::searchItems(const char* itemBarcode, productDesc *product){
        
        itemFound = -1;
        char query[256];
        snprintf(query, sizeof(query), "SELECT * FROM products WHERE barcode = '%s'", itemBarcode);
    
        int rc = db_exec(db, query, product);
        if (rc != SQLITE_OK) {
            return -1;
            }

        return itemFound;
    }





}