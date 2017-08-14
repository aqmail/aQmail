#ifndef ENV_H
#define ENV_H

extern int env_isinit;

int env_init(void);
int env_put(char *);
int env_put2(char *,char *); 
int env_unset(char *);
extern /*@null@*/char *env_get(char *);
char *env_pick(void);
void env_clear(void);
char *env_findeq(char *);

extern char **environ;

#endif
