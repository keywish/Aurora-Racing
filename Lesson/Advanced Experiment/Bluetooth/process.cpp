#include "protocol.h"

static void sendPackage(ST_protocol *send_dat,int len)
{   
    int i;
    byte buf[32];
    byte *p_data = &buf[4];
    buf[0] = send_dat->start_code;
    buf[1] = send_dat->type;
    buf[2] = send_dat->addr;
    buf[3] = send_dat->function;
    for(i = 0; i < len-7; i++)
    {
       *(p_data+i) = *(send_dat->data);
    }
    Serial.flush();
    Serial.write(buf,len);
}

int sendBatteryPackage(byte *battery_value)
{
    ST_protocol send_dat;
    int send_len;
    send_dat.start_code = START_CODE;
    send_dat.type = 0x01;
    send_dat.addr = 0x01;
    send_dat.function = E_BATTERY;
    send_dat.data = battery_value;
    send_dat.sum = send_dat.type+send_dat.addr+send_dat.function+*send_dat.data;
    send_dat.end_code = END_CODE;
    send_len = 8;
    sendPackage(&send_dat,send_len);
}

boolean protocol_prase(byte buffer[],int len,ST_protocol *recv)
{
      int i;
      unsigned short int check_sum = 0;
      recv->start_code = buffer[0];
      for (i=1; i<len-3; i++) {
          check_sum+=buffer[i];
      }
      if ((check_sum & 0xFF) != buffer[len-2] && ((check_sum >> 8) & 0xFF) != buffer[len-3]) {
      //    Serial.print("check sum error \n");
          return false ;
      }
     delay(50);
     recv->function = buffer[3];
     //Serial.print(recv->function) ;
     recv->data = &buffer[4];
//     Serial.print("The buffer[4] is :");
//     Serial.print(buffer[4]);
//     Serial.print("\n");
	recv->end_code = buffer[len-1];
    return 0;
}

int protocol_prase_speed(ST_protocol *recv)
{
    return (int)(*(recv->data));
}

int protocol_prase_degree(ST_protocol *recv)
{
    return ((int)(*(recv->data)<< 8)|(int)(*(recv->data+1)));
    //return ((int)*(recv->data+1)<< 8);
}
