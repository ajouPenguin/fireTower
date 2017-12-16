#ifdef __COMM_WITH_ARDUINO_H__
#define __COMM_WITH_ARDUINO_H__

void sendToArduino(int fd, char* data);
void receiveFromArduino(int fd, struct *fireData);

#endif
