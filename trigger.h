#ifndef TRIGGER_H
#define TRIGGER_H

void trigger_set();
void trigger_selprep(int *,fd_set *);
int trigger_pulled(fd_set *);

#endif
