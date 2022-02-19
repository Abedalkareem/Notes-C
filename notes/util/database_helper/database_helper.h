//
//  database_helper.h
//  cquiz
//
//  Created by abedalkareem omreyh on 30/01/2022.
//

#ifndef database_helper_h
#define database_helper_h
#include "note.h"

#include <stdio.h>

void getNotes(struct Note **note, int *size);
void addNote(char *title, char *note);
void deleteNote(int noteID);

#endif /* database_helper_h */
