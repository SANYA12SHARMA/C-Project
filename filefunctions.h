#include <stdbool.h>
void read_file(const char* filename);
void ask_user();
void decrypt_passwords();
void encrypt_passwords(const char* ptr);
bool check_from_dictionary(const char* password);
void ask_and_read(const char* message,const char* fileName);
int special_character_idx(char c);
char decrypt_shift(char c,int shiftValue);
char encrypt_shift(char c,int shiftValue);
char* encrypt(const char Message[],int shiftValue);
void decrypt(char Message[],int shiftValue);

