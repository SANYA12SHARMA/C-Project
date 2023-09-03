#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Define a list of special characters
const char* specialCharacterList = "!@$#%^&*?()";

// Function to read and print the content of a file
void read_file(const char *filename) {
    FILE *filePointer;
    char buffer[1024]; // Buffer to store the read data

    // Open the file for reading
    filePointer = fopen(filename, "r");
    
    if (filePointer == NULL) {
        perror("Error opening the file");
        return;
    }

    // Read and print the file line by line
    while (fgets(buffer, sizeof(buffer), filePointer) != NULL) {
        printf("%s", buffer);
    }

    // Close the file
    fclose(filePointer);
}

// Function to get the index of a special character
int special_character_idx(char c) {
    int specialCharacterCount = strlen(specialCharacterList);
    for (int i = 0; i < specialCharacterCount; i++) {
        if (c == specialCharacterList[i]) {
            return i;
        }
    }
    return -1;
}

// Function to decrypt a character
char decrypt_shift(char c, int shiftValue) 
{
    if (isalpha(c)) {
        char base = isupper(c) ? 'A' : 'a';
        return (c - base - shiftValue + 26) % 26 + base;
    } else if (isdigit(c)) {
        return '0' + ((c - '0' - shiftValue + 10) % 10);
    } else {
        int specialCharacterIndex = special_character_idx(c);
        if (specialCharacterIndex == -1) {
            return '\0';
        } else {
            return specialCharacterList[(specialCharacterIndex - shiftValue + strlen(specialCharacterList)) % strlen(specialCharacterList)];
        }
    }
}


// Function to decrypt a message 
void decrypt(char givenMessage[], int shiftValue) {
    int messageLength = strlen(givenMessage);

    for (int i = 0; i < messageLength; i++) {
        givenMessage[i] = decrypt_shift(givenMessage[i], shiftValue);
    }
}

// Function to decrypt passwords from a file
void decrypt_passwords() {
    FILE* inputFile = fopen("EncryptedPasswords.txt", "r");
    if (inputFile == NULL) {
        perror("Failed to open the input file");
        return;
    }
    
    long fileLength = 0; // Variable to store the length of the file
    fseek(inputFile, 0, SEEK_END); // Move to the end of the file
    fileLength = ftell(inputFile); // Get the file length
    
    if (fileLength <= 0) {
        // Handle empty file
        fclose(inputFile);
        return;
    }

    long offset = -1;
    int foundNonEmptyLine = 0;
    char character;

    while (ftell(inputFile) > 0) {
        fseek(inputFile, offset, SEEK_END);
        character = fgetc(inputFile);

        if (character == '\n') {
            if (foundNonEmptyLine) {
                // This is the end of the last non-empty line
                break;
            }
        } else if (!isspace(character)) {
            // Found a non-empty character, mark it
            foundNonEmptyLine = 1;
        }

        offset--;
    }

    // Calculate the length of the last non-empty line
    long lineLength = ftell(inputFile) - 1;
    
    fseek(inputFile, offset + 1, SEEK_END); // Position at the beginning of the last non-empty line

    // Allocate memory for the character array to store the line
    char* lastLine = (char*)malloc((lineLength + 1) * sizeof(char));
    if (lastLine == NULL) {
        perror("Failed to allocate memory");
        fclose(inputFile);
        return;
    }

    // Read the last non-empty line and store it in the character array
    if (fgets(lastLine, lineLength + 1, inputFile) == NULL) {
        perror("Failed to read the last non-empty line");
        free(lastLine);
        fclose(inputFile);
        return;
    }

    char line[100];
    strcpy(line, lastLine);
    free(lastLine);

    FILE* outputFile = fopen("DecryptedPasswords.txt", "a");
    if (outputFile == NULL) {
        perror("Failed to open the output file");
        fclose(inputFile);
        return;
    }
    fseek(outputFile, 0, SEEK_END);
    if (ftell(outputFile) == 0) {
        fputs("Decrypted passwords:\n", outputFile);
    }

    // Decrypt the line read from the input file
    decrypt(line, 3);

    // Write the decrypted line to the output file
    fputs(line, outputFile);
    fputs("\n", outputFile);

    // Close the input and output files
    fclose(inputFile);
    fclose(outputFile);
}


// Function to encrypt a character by shifting it
char encrypt_shift(char c, int shiftValue) 
{
    if (isalpha(c)) {
        char base = isupper(c) ? 'A' : 'a';
        return (c - base + shiftValue) % 26 + base;
    } else if (isdigit(c)) {
        return '0' + ((c - '0' + shiftValue) % 10);
    } else {
        int specialCharacterIndex = special_character_idx(c);
        if (specialCharacterIndex == -1) {
            return '\0';
        } else {
            return specialCharacterList[(specialCharacterIndex + shiftValue) % strlen(specialCharacterList)];
        }
    }
}

// Function to encrypt a given message and return the encrypted version
char* encrypt(const char givenMessage[], int shiftValue) {
    int messageLength = strlen(givenMessage);

    // Allocate memory for the encrypted message (including space for null terminator)
    char* encryptedMessage = (char*)malloc((messageLength + 1) * sizeof(char));

    if (encryptedMessage == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    for (int i = 0; i < messageLength; i++) {
        encryptedMessage[i] = encrypt_shift(givenMessage[i], shiftValue);
    }

    // Add null terminator to the end of the encrypted message
    encryptedMessage[messageLength] = '\0';

    return encryptedMessage;
}

void encrypt_passwords(const char *ptr)
{
    FILE* file = fopen("EncryptedPasswords.txt", "a");
    if (file == NULL) {
        printf("Failed to open store the file.\n");
        return;
    }
    // Encrypt the password and store it
    fseek(file, 0, SEEK_END);
    if(ftell(file)==0){
        fputs("Encrypted passwords:\n",file);
    }
    char* encryptedPassword = encrypt(ptr, 3);

    if (encryptedPassword != NULL) {
        fputs(encryptedPassword, file);
        fputs("\n", file);
        free(encryptedPassword); // Free the memory allocated for the encrypted password
    } else {
        printf("Failed to encrypt the password.\n");
    }

    fclose(file);

    printf("Password stored successfully.\n");
}

// Function to check if a password exists in a common passwords file
int check_from_dictionary(const char* password) {
    FILE* file = fopen("CommonPasswords.txt", "r");
    
    // Check if the file could be opened successfully.
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return 0;
    }   
    
    char line[30];
    
    // Read lines from the file one by one.
    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove the newline character, if present, from the end of the line.
        line[strcspn(line, "\n")] = '\0';
        
        // Compare the current line with the provided password.
        if (strcmp(line, password) == 0) {
            printf("Change the password.\n");
            fclose(file);
            return 1;
        }
    }
    
    // Close the dictionary file and return 0 if the password was not found.
    fclose(file);
    return 0;
}

// Function to ask the user and read the corresponding file
void ask_and_read(const char* message, const char* fileName) {
    char askuser;

    do {
        printf("%s (yes or no): ", message);
        scanf(" %c", &askuser);

        if (askuser == 'y' || askuser == 'Y') {
            read_file(fileName);
            return; // Exit the function after successful input
        } else if (askuser == 'n' || askuser == 'N') {
            return; // Exit the function if the user chooses not to read the file
        } else {
            printf("Invalid input. Please enter 'yes' (y/Y) or 'no' (n/N).\n");
        }
    } while (1); // Continue looping until valid input is received
}

void ask_user(){
    // Ask and read the Encrypted passwords file
    ask_and_read("Do you want to see Encrypted passwords?", "EncryptedPasswords.txt");

    // Ask and read the Decrypted passwords file
    ask_and_read("Do you want to see Decrypted passwords?", "DecryptedPasswords.txt");
    return;
}

