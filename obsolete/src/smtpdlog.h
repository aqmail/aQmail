#ifndef SMTPDLOG_H
#define SMTPDLOG_H
#define FDLOG 2

void flush();
void out();

void smtpdlog_init(void);
void smtp_loga(char *,char *,char *,char *,char *,char *,char *,char *,char *);
void smtp_logb(char *,char *,char *,char *,char *,char *,char *);
void smtp_logg(char *,char *,char *,char *,char *,char *,char *);
void smtp_logh(char *,char *,char *,char *,char *);
void smtp_logi(char *,char *,char *,char *,char *,char *,char *,char *);
void smtp_logr(char *,char *,char *,char *,char *,char *,char *,char *);

void die_read(void);
void die_alarm(void);
void die_nomem(void);
void die_control(void);
void die_ipme(void);
void die_starttls(void);
void die_recipients(void);
void straynewline(void);

void err_unimpl(void);
void err_syntax(void);
void err_noop(void);
void err_vrfy(void);
void err_wantrcpt(void);
void err_qqt(void);

int err_child(void);
int err_fork(void);
int err_pipe(void);
int err_write(void);
int err_starttls(void);
void err_tlsreq(char *,char *,char *,char *,char *);

void err_helo(char *,char *,char *,char *,char *,char *,char *,char *);
void err_spf(char *,char *,char *,char *,char *,char *,char *,char *);

void err_authd(void);
void err_authmail(void); 
void err_authfail(char *,char *,char *,char *,char *,char *,char *);
void err_authabrt(void);
void err_authreq(char *,char *,char *,char *,char *);
void err_submission(char *,char *,char *,char *,char *);
int err_authabort(void);
int err_authinput(void); 
int err_noauth(void);

void err_wantmail(void);
void err_mav(char *,char *,char *,char *,char *,char *,char *);
void err_bmf(char *,char *,char *,char *,char *,char *,char *,char *);
void err_mfdns(char *,char *,char *,char *,char *,char *,char *);

void err_nogateway(char *,char *,char *,char *,char *,char *,char *);
void err_brt(char *,char *,char *,char *,char *,char *,char *);
void err_rcpts(char *,char *,char *,char *,char *,char *,char *);
void err_recipient(char *,char *,char *,char *,char *,char *,char *);

void straynewline(void); 
void err_notorious(void);
void err_size(char *,char *,char *,char *,char *,char *,char *);
void err_data(char *,char *,char *,char *,char *,char *,char *,char *);

#endif
