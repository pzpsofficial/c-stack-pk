#ifndef APP_INTERFACE_H
#define APP_INTERFACE_H

enum APP_INTERFACE_AVAILABLE_OPERATIONS {
    APP_INTERFACE_DISPLAY,
    APP_INTERFACE_PUSH,
    APP_INTERFACE_POP,
    APP_INTERFACE_SEARCH,
    APP_INTERFACE_SAVE_TO_FILE,
    APP_INTERFACE_READ_FROM_FILE,
    APP_INTERFACE_EXIT
};

void display();
void push();
void pop();
void search();
void save_to_file();
void read_from_file();
void app_exit();
void menu();

#endif //APP_INTERFACE_H
