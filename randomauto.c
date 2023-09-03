#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#define MAX_PASSWORD_LENGTH 64

const char* specialCharacterList = "!@$#%^&*?()";
// const char specialCharacterList[] = "!@$#%^&*?()";

typedef struct usersOptions
{
	int numbers;
	int upperCase;
	int lowerCase;
	int specialChar;
	int duplicateChar;
	int similarChar;
} user;

int getSpecialCharacterIndex(char c) 
{
    int specialCharacterCount = strlen(specialCharacterList);
    for (int i = 0; i < specialCharacterCount; i++) {
        if (c == specialCharacterList[i]) {
            return i;
        }
    }
    return -1;
}
char shiftCharacter(char c, int shiftValue) 
{
    if (isalpha(c)) {
        char base = isupper(c) ? 'A' : 'a';
        return (c - base - shiftValue + 26) % 26 + base;
    } else if (isdigit(c)) {
        return '0' + ((c - '0' - shiftValue + 10) % 10);
    } else {
        int specialCharacterIndex = getSpecialCharacterIndex(c);
        if (specialCharacterIndex == -1) {
            printf("Unidentified character found: %c\n", c);
            return '\0';
        } else {
            return specialCharacterList[(specialCharacterIndex - shiftValue + strlen(specialCharacterList)) % strlen(specialCharacterList)];

        }
    }
}

void decrypt(char givenMessage[], int shiftValue) 
{
    int messageLength = strlen(givenMessage);

    for (int i = 0; i < messageLength; i++) 
	{
        givenMessage[i] = shiftCharacter(givenMessage[i], shiftValue); // Decrypting is shifting in the opposite direction
    }
}

void decryptfile()
{
    FILE* inputFile = fopen("StorePasswords.txt", "r");
    if (inputFile == NULL) {
        perror("Failed to open the input file");
        return;
    }

    fseek(inputFile, 0, SEEK_END); // Move to the end of the file
    long fileLength = ftell(inputFile);

    if (fileLength <= 0) {
        // Handle empty file
        fclose(inputFile);
        return;
    }

    char* lastLine = NULL;
    long lineLength = 0;

    // Start reading from the end of the file
    fseek(inputFile, -1, SEEK_END);

    while (ftell(inputFile) > 0) {
        char character = fgetc(inputFile);

        if (character == '\n') {
            // End of line reached, break
            break;
        }

        lineLength++;
        fseek(inputFile, -2, SEEK_CUR); // Move back by 2 characters (1 character + 1 newline)
    }

    // Read the last non-empty line into lastLine
    lastLine = (char*)malloc((lineLength + 1) * sizeof(char));

    if (lastLine == NULL) {
        perror("Failed to allocate memory");
        fclose(inputFile);
        return;
    }

    fgets(lastLine, lineLength + 1, inputFile);

    // Close the input file
    fclose(inputFile);

    if (lineLength > 0) {
        // Remove the newline character, if it exists, from the last line
        char* newlinePosition = strchr(lastLine, '\n');
        if (newlinePosition != NULL) {
            *newlinePosition = '\0'; // Replace '\n' with '\0' to terminate the string
        }

        char line[100];
        strcpy(line, lastLine);
        free(lastLine);

        FILE* outputFile = fopen("decrypted_passwords.txt", "a");
        if (outputFile == NULL) {
            perror("Failed to open the output file");
            return;
        }

        // Decrypt the line read from the input file
        decrypt(line, 3);

        // Write the decrypted line to the output file
        fputs(line, outputFile);
        fputs("\n", outputFile);

        // Close the output file
        fclose(outputFile);
    }
}


void calculate_strength(const char* password)
{
	int alpha = 0;
	int digit = 0;
	int special = 0;
	
	for( int i = 0; password[i] != '\0'; i++)
	{
		if( isalpha(password[i]) != 0 ){
			alpha++;
		}else if( isdigit(password[i]) != 0 ){
			digit++;
		}else{
			special++;
		}
	}
	
	if(alpha > 0 && digit > 0 && special > 0){
		printf("\n\033[1;32mGreat! Your password is strong\033[0m\n");
	}else if(alpha > 0 && digit> 0){
		printf("\n\033[1;33mYour password is moderate.Add some special characters.\033[0m\n");
	}else{
		printf("\n\033[1;31mYour password is weak!\033[0m\n");
	}
	
}


char shiftCharactere(char c, int shiftValue) 
{
    if (isalpha(c)) {
        char base = isupper(c) ? 'A' : 'a';
        return (c - base + shiftValue) % 26 + base;
    } else if (isdigit(c)) {
        return '0' + ((c - '0' + shiftValue) % 10);
    } else {
        int specialCharacterIndex = getSpecialCharacterIndex(c);
        if (specialCharacterIndex == -1) {
            printf("Unidentified character found: %c\n", c);
            return '\0';
        } else {
            return specialCharacterList[(specialCharacterIndex + shiftValue) % strlen(specialCharacterList)];
        }
    }

}

// Function to encrypt a given message and return the encrypted version
char* encrypt(const char givenMessage[], int shiftValue) 
{
    int messageLength = strlen(givenMessage);

    // Allocate memory for the encrypted message (including space for null terminator)
    char* encryptedMessage = (char*)malloc((messageLength + 1) * sizeof(char));

    if (encryptedMessage == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    for (int i = 0; i < messageLength; i++) {
        encryptedMessage[i] = shiftCharactere(givenMessage[i], shiftValue);
    }

    // Add null terminator to the end of the encrypted message
    encryptedMessage[messageLength] = '\0';

    return encryptedMessage;
}

void store_passwords(const char *ptr)
{
    FILE* file = fopen("StorePasswords.txt", "a");
    if (file == NULL) {
        printf("Failed to open store the file.\n");
        return;
    }

    // Encrypt the password and store it
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

// Function to check if a password exists in a common passwords file.
bool check_from_dictionary(const char* password)
{
    FILE* file = fopen("CommonPasswords.txt", "r");
    
    // Check if the file could be opened successfully.
    if (file == NULL)
    {
        printf("Failed to open the file.\n");
        return false;
    }   
    
    char line[30];
    
    // Read lines from the file one by one.
    while (fgets(line, sizeof(line), file) != NULL)
    {
        // Remove the newline character, if present, from the end of the line.
        line[strcspn(line, "\n")] = '\0';
        
        // Compare the current line with the provided password.
        if (strcmp(line, password) == 0)
        {
            printf("Change the password.\n");
            fclose(file);
            return true;
        }
    }
    
    // Close the dictionary file and return false if the password was not found.
    fclose(file);
    return false;
}

// Function to check if a character 'c' is similar to any character in the 'similarChars' array.
bool is_similar(char c)
{
    const char similarChars[] = "iI1loO0";
    
    for (int i = 0; similarChars[i] != '\0'; i++)
    {
        if (c == similarChars[i])
        {
            return true;
        }
    }
    return false;
}

// Function to remove similar characters from the input string 'password'.
void remove_similar(char* password)
{
    int len = strlen(password);
    int curr = 0;
    
    // If the string has a length of 0 or 1, there are no similar characters to remove, so return.
    if (len <= 1)
    {
        return;
    }
    
    // Loop through each character in the password, and if it is not similar, copy it to the 'curr' position.
    for (int i = 0; i < len; i++)
    {
        if (!is_similar(password[i]))
        {
            password[curr] = password[i];
            curr++;
        }
    }
    
    // Terminate the modified password with a null character.
    password[curr] = '\0';
}

// Function to remove occurrences of a specific character .
void remove_all_occurrences(char* password, const char ch, int index)
{
    while (password[index] != '\0')
    {
        if (password[index] == ch)
        {
			int k;
            for (k = index; password[k] != '\0'; k++)
            {
                password[k] = password[k + 1];
            }
            password[k] = '\0';
        }
        
        index++;
    }
}

// Function to remove duplicate characters from the input string 'password'
void remove_duplicates(char* password)
{
    int len = strlen(password);
    
    // If the string has a length of 0 or 1, it already has no duplicates, so return.
    if (len <= 1)
    {
        return;
    }
    
    // Loop through each character in the string and remove its occurrences beyond the current position.
    for (int i = 0; i < len; i++)
    {
        remove_all_occurrences(password, password[i], i + 1);
    }
}

//Function to generate a password
void password_generator(char* password, int n, user u1)
{
	int randomNum = 0;
	
	//initialise random number generator
	srand((unsigned int)(time(NULL)));

	randomNum = rand() % 4; // Generate a random number between 0 and 3 (inclusive)
	
	//array for storing numbers
	char Numbers[] = "0123456789";
	//array for storing lowercase alphabets
	char LowerCase[] = "abcdefghijklmnopqrstuvwxyz";
	//array for storing uppercase alphabets
	char UpperCase[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	//array for all special characters
	char SpecialChar[] = "!@$#%^&*?()";
	
	int i=0;
	while(i < n)
	{
		if(randomNum == 1 && u1.numbers == 1){
				password[i] = Numbers[rand() % 10];
				i++;
		}else if(randomNum == 2 && u1.upperCase == 1){
				password[i] = UpperCase[rand() % 26];
				i++;
		}else if(randomNum == 3 && u1.lowerCase == 1){
				password[i] = LowerCase[rand() % 26];
				i++;
		}else{
			if(u1.specialChar == 1){
				password[i] = SpecialChar[rand() % 11];
				i++;
			}
		}
		
		randomNum = rand() % 4;
	}
	
	password[i] = '\0';
	
	//Remove Duplicate Characters
	if(u1.duplicateChar == 1){
		remove_duplicates(password);
	}
	
	//Remove Similar Characters
	if(u1.similarChar == 1){
		remove_similar(password);
	}	

	// Handle empty password after removal
    if (strlen(password) == 0){
        printf("Password became empty after removing duplicates or similar characters. Regenerating it...\n");
        password_generator(password, n, u1);
    }
}

// Function to check if all options are excluded
int all_options_excluded(user u) 
{
    return (u.numbers == -1 && u.upperCase == -1 && u.lowerCase == -1 && u.specialChar == -1);
}

// Function to get user input for an option with confirmation
int user_option(const char *message) 
{
    int choice;

    printf("%s\n", message);
    printf("Press 1 to include the following option or -1 to not: ");
    scanf("%d", &choice);

    if (choice != 1 && choice != -1) {
        printf("Invalid choice. Please enter 1 to include or -1 to not.\n");
        return user_option(message); // Recursive call for input validation
    }

    return choice;
}

void print_heading(char* txt)
{
	printf("\n\033[1;33m");
	
	for(int i = 0; i < strlen(txt) + 4; i++)
	{
		printf("*");
	}
	
	printf("\n");
	
	printf("* %s *",txt);
	printf("\n");
	
	for(int i = 0; i < strlen(txt) + 4; i++)
	{
		printf("*");
	}
	
	printf("\033[0m");
	printf("\n\n");
}

int main()
{
	print_heading("GENERATE A RANDOM PASSWORD");
	printf("\033[1;31m RULES \n");
	printf("----------------------------------------------------------------\n");
	printf("->Specify the Maximum Length of the Password\n");
	printf("->Press 1 to include following options else Press -1 to exclude.\n");
	printf("=>Numbers[0-9]\n");
	printf("=>Uppercase Alphabets[ABC]\n");
	printf("=>Lowercase Alphabets[abc]\n");
	printf("=>Special Characters\n");
	printf("=>Remove Duplicate Characters\n");
	printf("=>Remove Similar Characters\n");
	printf("----------------------------------------------------------------\n");
	printf("\033[0m");
	
	//length of password
	int len;
	printf("Enter the length of the Password: ");
	scanf("%d",&len);

	//Length should be greater than or equal to 6 and less than  or equal to 64 
	while(len < 6 || len > MAX_PASSWORD_LENGTH)
	{
		printf("Invalid Length, Please Enter Again: ");
		scanf("%d",&len);
	}

	//User-defined structure
	user u1;

	//Ask user the specifications to generate a password
	u1.numbers = user_option("Include numbers in the password.");
	u1.upperCase = user_option("Include uppercase letters in the password.");
	u1.lowerCase = user_option("Include lowercase letters in the password.");
	u1.specialChar = user_option("Include special characters in the password.");
	u1.duplicateChar = user_option("Remove duplicate characters in the password.");
	u1.similarChar = user_option("Remove similar characters in the password.");
	
	//Dynamically allocate memory for a password string and store its address in 'password'
	char *password=(char*)malloc(len);
	
	//Repeatedly ask the user to generate a password.
	while (1) {
		char choice;

		printf("Type 'y' to generate a password or 'n' to exit: ");
		scanf(" %c", &choice); // Adding a space before %c to skip whitespace characters

		if (choice == 'y') {
			if (all_options_excluded(u1)) {
				printf("Error: All password options are excluded.\n");
				printf("Password generation failed...\n");
				return 1;
			}

			password_generator(password, len, u1); // Call the password_generator function
			
			// Check if the password is from the CommonPasswords file
			while (check_from_dictionary(password)) {
				printf("Please try again: \n");
				password_generator(password, len, u1);
			}

			// Check if the password is empty
            if (strlen(password) == 0) {
                printf("Password is empty. Calculating strength is not possible.\n");
            } else {
                char ptr[strlen(password)+1];
				strcpy(ptr,password);
                // Store passwords in a file
                store_passwords(ptr);
                printf("File encrypted successfully!!!");
                decryptfile();
                printf("Your password is: %s\n\n", password);
                
                // Check strength of the password
                printf("\n******************************\n");
                printf("\e[4;37mPASSWORD STRENGTH\e[0m");
                
                calculate_strength(password);
                
                printf("******************************\n");
            }
		} else if (choice == 'n') {
			printf("Exit...\n");
			break; // Exit the loop 
		} else {
			printf("Invalid input. Please enter 'y' or 'n'.\n");
		}
	}
	free(password);//deallocate the memory
	
	return 0;
}

