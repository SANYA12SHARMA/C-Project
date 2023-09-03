#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define MAX_PASSWORD_LENGTH 64

// User-defined structure for password options
typedef struct usersOptions
{
	int numbers;
	int upperCase;
	int lowerCase;
	int specialChar;
	int duplicateChar;
	int similarChar;
} user;

// Function to calculate password strength
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

//Function to generate a password
void password_generator(char* password, int n, user u1)
{
	int randomNum = 0;

	srand((unsigned int)(time(NULL))); //initialise random number generator

	randomNum = rand() % 4; // Generate a random number between 0 and 3 (inclusive)
	
	 // Arrays for storing character sets
	char Numbers[] = "0123456789";
	char LowerCase[] = "abcdefghijklmnopqrstuvwxyz";
	char UpperCase[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
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

// Function to print a heading with a border
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
    // Print a heading for the program
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
	
	//Length of password
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
			
			//Check if the password is from the CommonPasswords file
			while (check_from_dictionary(password)) {
				printf("Please try again: \n");
				password_generator(password, len, u1);
			}

			// Check if the password is empty
			if (strlen(password) == 0) {
				printf("Password is empty. Calculating strength is not possible.\n");
			} else {
				char ptr[strlen(password)+1]; // Duplicate array to store password
				strcpy(ptr,password);
				
				encrypt_passwords(ptr); // Call the encryption function
				decrypt_passwords(); // Call the decryption function
				
				printf("Your password is: %s\n\n", password);
			
				// Check the strength of the password
				printf("\n******************************\n");
				printf("\e[4;37mPASSWORD STRENGTH\e[0m");
				calculate_strength(password);
				printf("******************************\n");
			}
		} else if (choice == 'n') {
			printf("Exit...\n");
			break; 
		} else {
			printf("Invalid input. Please enter 'y' or 'n'.\n");
		}
	}
	free(password); // Deallocate the memory
	ask_user(); // ask user about displaying encrypted and decrypted passwords
	return 0;
}
