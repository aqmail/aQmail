#ifndef SIG_H
#define SIG_H

void sig_catch();
void sig_block();
void sig_unblock();
void sig_blocknone();
void sig_pause();

void sig_dfl();

void sig_miscignore();
void sig_bugcatch();

void sig_pipeignore();
void sig_pipedefault();

void sig_contblock();
void sig_contunblock();
void sig_contcatch();
void sig_contdefault();

void sig_termblock();
void sig_termunblock();
void sig_termcatch();
void sig_termdefault();

void sig_alarmblock();
void sig_alarmunblock();
void sig_alarmcatch();
void sig_alarmdefault();

void sig_childblock();
void sig_childunblock();
void sig_childcatch();
void sig_childdefault();

void sig_hangupblock();
void sig_hangupunblock();
void sig_hangupcatch();
void sig_hangupdefault();

#endif
