#ifndef   _PROCESS_H_
#define   _PROCESS_H_
#include  "Arduino.h"
//void sedPackage(byte package);

boolean protocol_prase(byte buffer[],int len,ST_protocol *recv);
int protocol_prase_speed(ST_protocol *recv);
int protocol_prase_degree(ST_protocol *recv);

#endif // _PROCESS_H_
