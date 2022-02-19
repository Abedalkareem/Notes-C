//
//  outputer.c
//  notes
//
//  Created by abedalkareem omreyh on 19/02/2022.
//

#include "outputer.h"
#include "database_helper.h"
#include "note.h"
#include <stdlib.h>

void startChoicesSelection(void) {
  printf("Hi! What do you want to do?\n");
  printf("1-Get all the notes\n");
  printf("2-Insert a new note\n");
  printf("3-Delete a note\n");

  int choice;
  scanf("%i", &choice);
  switch (choice) {
    case 1:
      printAllNotes();
      break;
    case 2:
      startNoteInsertion();
      break;
    case 3:
      startNoteDeletion();
      break;
  }
}

void printAllNotes(void) {
  struct Note *notes = NULL;
  int size;
  getNotes(&notes, &size);

  printf("\n");

  if (size == 0) {
    printf("No notes found.\n\n");
    return;
  }

  printf("Notes List:\n");
  for (int i = 0; i < size; i++) {
    struct Note note = notes[i];
    printf("%i- Title: %s, Note: %s\n", note.id, note.title, note.note);
    free(note.title);
    free(note.note);
  }
  printf("\n");

  free(notes);
}

void startNoteInsertion(void) {
  char title[50];
  char note[100];
  printf("Please enter the title of the note\n");
  scanf("%s", title);
  printf("Please enter the note\n");
  scanf("%s", note);
  addNote(title, note);
  printf("\n");
}

void startNoteDeletion(void)  {
  printAllNotes();
  printf("\n");
  printf("Which note do you want to delete, please enter the id.\n");
  int choice;
  scanf("%i", &choice);
  deleteNote(choice);
  printf("Note deleted successfully!.\n");
}
