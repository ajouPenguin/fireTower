//#ifdef __THREE_THREAD_H__
//#define __THREE_THREAD_H__

void *thread_touchLCD(void *args);
void *thread_touch(void *args);
void *thread_etcPeri(void *args);
void *thread_communication(void *args);
//#endif
