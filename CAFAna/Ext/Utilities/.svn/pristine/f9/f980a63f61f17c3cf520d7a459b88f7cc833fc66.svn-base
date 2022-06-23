#include <iostream>

#include "sqlite3.h"

// Too difficult to link to the ART libraries, and this file is completely
// standalone
#include "art/Framework/IO/Root/RootDB/tkeyvfs.cc"

#include "TError.h"
#include "TFile.h"
#include "TTree.h"


void ListTables(sqlite3* db)
{
  std::cout << "\nListing tables..." << std::endl;

  sqlite3_stmt* stmt = 0;
  sqlite3_prepare_v2(db, "SELECT name FROM sqlite_master WHERE type='table';", -1, &stmt, 0);

  while(sqlite3_step(stmt) == SQLITE_ROW){
    std::cout << (const char*)sqlite3_column_text(stmt, 0) << std::endl;
  }
  sqlite3_reset(stmt);
}

void SampleRows(sqlite3* db)
{
  std::cout << "\nSample of rows..." << std::endl;

  sqlite3_stmt* stmt = 0;
  sqlite3_prepare_v2(db, "SELECT * FROM ParameterSets;", -1, &stmt, NULL);
  if(sqlite3_step(stmt) == SQLITE_ROW){
    std::cout << (const char*)sqlite3_column_text(stmt, 0) << std::endl;
    std::cout << (const char*)sqlite3_column_text(stmt, 1) << std::endl;
  }
  sqlite3_reset(stmt);
}


int main(int argc, char** argv)
{
  if(argc != 2){
    std::cout << "Usage: " << argv[0] << " <file to strip>" << std::endl;
    std::cout << "Stripping occurs in-place, ie it alters the input file" << std::endl;
    return 2;
  }

  std::cout << "Stripping " << argv[1] << std::endl;

  // Virtual file system that lets sqlite look inside the ROOT file
  tkeyvfs_init();

  // Don't want to see errors about missing dicts for file's data products
  gErrorIgnoreLevel = 2001;
  TFile file(argv[1], "UPDATE");
  gErrorIgnoreLevel = 0;
  TKey* key = (TKey*)file.Get("RootFileDB");
  if(!key){
    std::cerr << "Didn't find RootFileDB key in file" << std::endl;
    return 1;
  }

  sqlite3* db = 0;
  if(tkeyvfs_open_v2("RootFileDB", &db, SQLITE_OPEN_READWRITE, &file)){
    std::cerr << "Failed to open DB \"" << sqlite3_errmsg(db) << "\"" << std::endl;
    return 1;
  }


  ListTables(db);

  //  SampleRows(db);

  std::cout << "\nDropping ParameterSets table..." << std::endl;
  char* msg = 0;
  sqlite3_exec(db, "BEGIN TRANSACTION; DROP TABLE IF EXISTS ParameterSets; COMMIT;", 0, 0, &msg);
  if(msg) std::cerr << "Warning: \"" << msg << "\"" << std::endl;

  ListTables(db);


  sqlite3_close(db);

  // Persist the new DB back to the file
  if(!key->Write("RootFileDB")){
    std::cerr << "Error writing RootFileDB back to file" << std::endl;
    return 1;
  }

  return 0;
}
