//
//  database_helper.c
//  cquiz
//
//  Created by abedalkareem omreyh on 30/01/2022.
//

#include "database_helper.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "note.h"

void openDB(struct sqlite3 **db) {
  int status = sqlite3_open_v2("notes.db", db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
  if (status != SQLITE_OK) {
    printf("Something went wrong while creating/opening the database, error: %s\n", sqlite3_errmsg(*db));
    return;
  }
}

bool isTableExist(char *tableName) {
  struct sqlite3 *db;
  openDB(&db);

  struct sqlite3_stmt *stmt;
  char *sql = "SELECT count(type) FROM sqlite_master WHERE type='table' and name=?";
  sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  sqlite3_bind_text(stmt, 1, tableName, (int)strlen(tableName), NULL);
  int status = sqlite3_step(stmt);

  if (status != SQLITE_ROW) {
    printf("Something went wrong while checking is the table exist.\n");
    return false;
  }

  int tableCount = sqlite3_column_int(stmt, 0);

  sqlite3_finalize(stmt);

  return tableCount == 1;
}

void createTable(void) {
  struct sqlite3 *db;
  openDB(&db);

  if (isTableExist("Notes")) {
    return;
  }

  char *sql = "CREATE TABLE Notes(Id INTEGER PRIMARY KEY AUTOINCREMENT, Title TEXT, Note TEXT);";
  char *errmsg;
  int status = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
  if (status != SQLITE_OK) {
    printf("Something went wrong while creating the table, error: %s.\n", errmsg);
    return;
  }
  sqlite3_close(db);
}

void addNote(char *title, char *note) {
  createTable();

  struct sqlite3 *db;
  openDB(&db);

  struct sqlite3_stmt *stmt;
  const char* sql  = "INSERT INTO Notes values (?,?,?);";
  sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  sqlite3_bind_text(stmt, 2, title, (int)strlen(title), NULL);
  sqlite3_bind_text(stmt, 3, note, (int)strlen(note), NULL);

  int status = sqlite3_step(stmt);

  if (status != SQLITE_DONE) {
    printf("Something went wrong while inserting to the database\n");
    return;
  }

  sqlite3_finalize(stmt);
}

void getNotes(struct Note **note, int *size) {
  createTable();

  struct sqlite3 *db;
  openDB(&db);

  struct sqlite3_stmt *stmt;
  sqlite3_prepare(db, "select * from Notes", -1, &stmt, NULL);

  int index = 0;
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    if (note == NULL) {
      *note = malloc(sizeof(struct Note));
    } else {
      *note = realloc(*note, (index + 1) * sizeof(struct Note));
    }
    struct Note newNote = (*note)[index];
    newNote.id = sqlite3_column_int(stmt, 0);
    char *titleText = (char*)sqlite3_column_text(stmt, 1);
    char *noteText = (char*)sqlite3_column_text(stmt, 2);
    if (titleText) {
      newNote.title = (char *) malloc(sizeof(char) * (strlen(titleText) + 1));
      strcpy(newNote.title, titleText);
    } else {
      newNote.title = NULL;
    }
    if (noteText) {
      newNote.note = (char *) malloc(sizeof(char) * (strlen(noteText) + 1));
      strcpy(newNote.note, noteText);
    } else {
      newNote.note = NULL;
    }
    (*note)[index] = newNote;
    index++;
  }
  *size = index;

  sqlite3_finalize(stmt);
}

void deleteNote(int noteID) {
  struct sqlite3 *db;
  openDB(&db);

  struct sqlite3_stmt *stmt;
  char *sql = "DELETE FROM Notes WHERE Id = ?;";
  sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  sqlite3_bind_int(stmt, 1, noteID);
  int status = sqlite3_step(stmt);
  if (status != SQLITE_DONE) {
    printf("Something went wrong while deleting the note.\n");
    return;
  }

  sqlite3_finalize(stmt);

  sqlite3_close(db);
}
