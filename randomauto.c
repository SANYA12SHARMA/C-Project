#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

void store_passwords(const char* password)
{
	FILE* file=fopen("StorePasswords.txt","a");

	if(file==NULL){
		printf("Failed to open the file.\n");
	}else{
		fputs(password,file);
		fputs("\n",file);
	}
}
void calculate_strength(const char* password)
{
	int alpha = 0;
	int digit = 0;
	int special = 0;
	
	if(password[0] == '\0'){
		printf("No password generated");
		return;
	}
	for( int i = 0 ; password[i] != '\0' ; i++ )
	{
		if( isalpha(password[i]) != 0 ){
			alpha++;
		}else if( isdigit(password[i]) != 0 ){
			digit++;
		}else{
			special++;
		}
	}
	
	if( alpha > 0 && digit > 0 && special > 0 ){
		printf("\n\033[1;32mGreat! Your password is strong\033[0m\n");
	}else if( alpha > 0 && digit> 0 ){
		printf("\n\033[1;33mYour password is moderate.Add some special characters.\033[0m\n");
	}else{
		printf("\n\033[1;31mYour password is weak!\033[0m\n");
	}
	
}

bool check_from_dictionary(const char* password)
{
	FILE* file = fopen("common.txt","r");
	
	if( file == NULL ){
		printf("Failed to open the file.\n");
		return false;
	}	
	
	char line[30];
	
	while( fgets( line, sizeof(line), file ) != NULL )
	{
		line[strcspn(line ,"\n")] = '\0';
		
		if(strcmp(line,password) == 0){
			printf("change the password");
			fclose(file);
			return true;
		}
	}
	
	return false;
}

bool is_similar(char c)
{
	const char similarChars[] = "iI1loO0";
	
	for( int i = 0 ; similarChars[i] != '\0'; i++ )
	{
		if( c == similarChars[i] ){
			return true;
		}
	}
	
	return false;
}

void remove_similar(char* str)
{
	int len = strlen(str);
	int curr = 0;
	
	if(len <= 1){
		return;
	}
	
	for( int i = 0 ; i < len ; i++ )
	{
		if( !is_similar(str[i]) ){
			str[curr] = str[i];
			curr++;
		}
	}
	
	str[curr] = '\0';

}
void remove_all_occurrences(char* str, const char ch, int idx)
{
	int k;
	while( str[idx] != '\0' )
	{
		if( str[idx] == ch ){
			for( k = idx ; str[k] != '\0' ; k++ )
			{
				str[k] = str[k+1];
			}
			str[k] = '\0';
		}
		
		idx++;
	}
}
void remove_duplicates(char* str)
{
	int len = strlen(str);
	
	if(len <= 1){
		return;
	}
	
	for( int i = 0; i < len; i++ )
	{
		remove_all_occurrences(str, str[i], i+1);
	}	
}

typedef struct usersOptions
{
	int numbers;
	int upperCase;
	int lowerCase;
	int specialChar;
	int duplicateChar;
	int similarChar;
} user;

void password_generator(char password[], int n, user u1)
{
	int randomNum = 0;
	
	//initialise random number generator
	srand((unsigned int)(time(NULL)));
	randomNum = rand() % 4;
	
	//array for storing numbers
	char Numbers[] = "0123456789";
	//array for storing lowercase alphabets
	char LowerCase[] = "abcdefghijklmnopqrstuvwxyz";
	//array for storing uppercase alphabets
	char UpperCase[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	//array for all special characters
	char SpecialChar[] = "!@#$%^&*?()";
	
	int i=0;
	while( i < n )
	{
		if( randomNum == 1 && u1.numbers == 1 ){
				password[i] = Numbers[rand() % 10];
				i++;
		}else if( randomNum == 2 && u1.upperCase == 2 ){
				password[i] = UpperCase[rand() % 26];
				i++;
		}else if( randomNum == 3 && u1.lowerCase == 3 ){
				password[i] = LowerCase[rand() % 26];
				i++;
		}else{
			if( u1.specialChar == 4 ){
				password[i] = SpecialChar[rand() % 11];
				i++;
			}
		}
		
		randomNum = rand() % 4;
	}
	
	password[i] = '\0';
	
	//exclude Duplicate Characters
	if(u1.duplicateChar == 5){
		remove_duplicates(password);
	}
	
	//exclude Similar Characters
	if(u1.similarChar == 6){
		remove_similar(password);
	}	
}

void print_heading(char* txt)
{
	printf("\n\033[1;33m");
	
	for(int i = 0 ; i < 25 ; i++)
	{
		printf(" ");
	}
	for(int i = 0 ; i < strlen(txt) + 4 ; i++)
	{
		printf("*");
	}
	
	printf("\n");
	
	for(int i = 0 ; i < 25 ; i++)
	{
		printf(" ");
	}
	
	printf("* %s *",txt);
	printf("\n");
	
	for(int i = 0 ; i < 25 ; i++)
	{
		printf(" ");
	}
	for(int i = 0;i < strlen(txt) + 4 ; i++)
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
	printf("-------------------------------------------\n");
	printf("->Specify the Maximum Length of the Password\n");
	printf("->Press 1 to include Numbers[0-9]\n");
	printf("->Press 2 to include Uppercase Alphabets[ABC]\n");
	printf("->Press 3 to include Lowercase Alphabets[abc]\n");
	printf("->Press 4 to include Special Characters\n");
	printf("->Press 5 to exclude Duplicate Characters\n");
	printf("->Press 6 to exclude Similar Characters\n");
	printf("-------------------------------------------\n");
	printf("\033[0m");
	
	//length of password
	int len;
	printf("Enter the length of the Password: ");
	scanf("%d",&len);

	//Length should be greater than 6 and less than 64 
	while(len < 6 || len>64)
	{
		printf("Invalid Length, Please Enter Again: ");
		scanf("%d",&len);
	}

	//User-defined structure
	user u1;

	//Ask user the specifications to generate a password
	printf("\nPress 1 to include Numbers else Press -1. \n");
	scanf("%d",&u1.numbers);
	printf("Press 2 to include Uppercase Alphabets else Press -1. \n");
	scanf("%d",&u1.upperCase);
	printf("Press 3 to include Lowercase Alphabets else Press -1. \n");
	scanf("%d",&u1.lowerCase);
	printf("Press 4 to include Special Characters else Press -1. \n");
	scanf("%d",&u1.specialChar);
	printf("Press 5 to exclude Duplicate Characters else Press -1. \n");
	scanf("%d",&u1.duplicateChar);
	printf("Press 6 to exclude Similar Characters else Press -1. \n");
	scanf("%d",&u1.similarChar);
	
	char password[len];
	
	//Repeatedly ask the user to generate a password.
	while (1) {
        char choice;

        printf("Type 'y' to generate a password or 'n' to exit: ");
        scanf(" %c", &choice); // Adding a space before %c to skip whitespace characters

        if (choice == 'y') {

            // Call the password_generator function
            password_generator(password, len, u1);

            // Check if the password is from the dictionary
            while (check_from_dictionary(password)) {
                printf("Please try again: \n");
                password_generator(password, len, u1);
            }

			//store passwords in a file
			store_passwords(password);
			
            printf("Your password is: ");
            printf("%s", password);
            printf("\n\n");
        } else if (choice == 'n') {
            printf("Exiting...\n");
            break; // Exit the loop and the program
        } else {
            printf("Invalid input. Please enter 'y' or 'n'.\n");
        }
}
	//check strength of the password
	printf("\n******************************\n");
	printf("\e[4;37mPASSWORD STRENGTH\e[0m");
	
	calculate_strength(password);
	
	printf("******************************\n");
	
	return 0;
}

