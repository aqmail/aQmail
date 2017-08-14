#ifndef COMMANDS_H
#define COMMANDS_H

struct commands {
  char *text;
  void (*fun)();
  void (*flush)();
} ;

int commands();

#endif
