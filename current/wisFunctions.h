/*
  Carmen Maidantchik <lodi@lps.ufrj.br>
  Rafael Santelli <rafaelsantelli@hotmail.com>

  Last Update: 05.09.2003 - 00:00 - Rafael
 */

#ifndef __FUNCTIONS__
#define __FUNCTIONS__

int wisDeleteFile(char *);
int ctbDeleteFiles(char *);
int ctbFileExists(char *);
//int ctbStageFile(char *);
//int ctbCopyFile(char *, char *);
void ctbSendEmailTo(char *to, char *from, char *mailSubject, char *htmlMailMessage, char *textMailMessage);
void ctbTouchFile(char *);
int ctbIsAllDigit(char *);

#endif

