#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
void calculate(const char* password){
	int alpha=0;
	int digit=0;
	int special=0;
	for(int i=0;password[i]!='\0';i++){
		if(isalpha(password[i])!=0){
			alpha++;
		}else if(isdigit(password[i])!=0){
			digit++;
		}else{
			special++;
		}
	}
	if(alpha>0 && digit>0 && special>0){
		printf("\n\033[1;32mGreat! YOur password is strong\033[0m\n");
	}else if(alpha>0 && digit>0){
		printf("\n\033[1;33mYour password is moderate\n We recommend you to add some special character\033[0m\n");
	}else{
		printf("\n\033[1;31mYour password is weak!\033[0m\n");
	}
}
bool is_similar(char c){
	const char similarChars[]="iI1loO0";
	for(int i=0;similarChars[i]!='\0';i++){
		if(c==similarChars[i]){
			return true;
		}
	}
	return false;
}
void remove_similar(char* str){
	int len=strlen(str);
	int curr=0;
	if(len<=1){
		return;
	}
	for(int i=0;i<len;i++){
		if(!is_similar(str[i])){
			str[curr]=str[i];
			curr++;
		}
	}
    str[curr]='\0';

}
void remove_all_occurrences(char* str,const char ch,int idx){
	int k;
	while(str[idx]!='\0'){
		if(str[idx]==ch){
			for(k=idx;str[k]!='\0';k++){
				str[k]=str[k+1];
			}
			str[k]='\0';
		}
		idx++;
	}
}
void remove_duplicates(char* str){
	int len=strlen(str);
	if(len<=1){
		return;
	}
	for(int i=0;i<len;i++){
		remove_all_occurrences(str,str[i],i+1);
	}	
}
struct usersOptions{
int numbers;
int upperCase;
int lowerCase;
int specialChar;
int duplicateChar;
int similarChar;
};
void password_generator(char password[],int n,struct usersOptions user){
	int randomNum=0;
	//initialise random number generator
	srand((int)(time(NULL)));
	randomNum=rand()%4;
	//array for storing numbers
	char Numbers[]="0123456789";
	//array for storing lowercase alphabets
	char LowerCase[]="abcdefghijklmnopqrstuvwxyz";
	//array for storing uppercase alphabets
	char UpperCase[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	//array for all special characters
	char SpecialChar[]="!@#$%^&*?()";
	int i=0;
	while(i<n){
		if(randomNum==1){
			if(user.numbers==1){
				password[i]=Numbers[rand()%10];
				randomNum=rand()%4;
			}else{
				randomNum=rand()%4;
				continue;
			}
		}else if(randomNum==2){
			if(user.upperCase==2){
				password[i]=UpperCase[rand()%26];
				randomNum=rand()%4;
			}else{
				randomNum=rand()%4;
				continue;
			}
		}else if(randomNum==3){
			if(user.lowerCase==2){
				password[i]=LowerCase[rand()%26];
				randomNum=rand()%4;
			}else{
				randomNum=rand()%4;
				continue;
			}
		}else{
			if(user.specialChar==4){
				password[i]=SpecialChar[rand()%11];
				randomNum=rand()%4;
			}else{
				randomNum=rand()%4;
				continue;
			}
		}
		i++;
	}
	password[i]='\0';
	//exclude Duplicate Characters
	if(user.duplicateChar==5){
		remove_duplicates(password);
	}
	//exclude Similar Characters
	if(user.similarChar==6){
		remove_similar(password);
	}	
}
void print_heading(char* txt){
	printf("\n");
	printf("\033[1;33m");
	for(int i=0;i<25;i++){
		printf(" ");
	}
	for(int i=0;i<strlen(txt)+4;i++){
		printf("*");
	}
	printf("\n");
	for(int i=0;i<25;i++){
		printf(" ");
	}
	printf("* %s *",txt);
	printf("\n");
	for(int i=0;i<25;i++){
		printf(" ");
	}
	for(int i=0;i<strlen(txt)+4;i++){
		printf("*");
	}
	printf("\033[0m");
	printf("\n\n");
}

int main(){
	print_heading("GENERATE A RANDOM PASSWORD");
	printf("\033[1;31m RULES \n");
	printf("-------------------------------------------\n");
	printf("Specify the Maximum Length of the Password\n");
	printf("Press 1 to include Numbers[0-9]\n");
	printf("Press 2 to include Uppercase Alphabets[ABC]\n");
	printf("Press 3 to include Lowercase Alphabets[abc]\n");
	printf("Press 4 to include Special Characters\n");
	printf("Press 5 to exclude Duplicate Characters\n");
	printf("Press 6 to exclude Similar Characters\n");
	printf("Press 0 to Quit\n");
	printf("-------------------------------------------\n");
	printf("\033[0m");
	//length of password
	int len;
	printf("Enter the length of the Password: ");
	scanf("%d",&len);
	struct usersOptions user;
	printf("Press 1 to include Numbers else -1 \n");
	scanf("%d",&user.numbers);
	printf("Press 2 to include Uppercase Alphabets else -1 \n");
	scanf("%d",&user.upperCase);
	printf("Press 3 to include Lowercase Alphabets else -1 \n");
	scanf("%d",&user.lowerCase);
	printf("Press 4 to include Special Characters else -1 \n");
	scanf("%d",&user.specialChar);
	printf("Press 5 to exclude Duplicate Characters else -1 \n");
	scanf("%d",&user.duplicateChar);
	printf("Press 6 to exclude Similar Characters else -1 \n");
	scanf("%d",&user.similarChar);
	printf("Your password is: ");
	char password[len];
	//calling the function
	password_generator(password,len,user);	
// 	FILE*ptr=fopen("common.txt","r");
// if(ptr==NULL){
// 	printf("failed to open the file");
// 	return 1;
// }	
// 	char line[30];
// 		while(fgets(line,sizeof(line),ptr)!=NULL){
// 			line[strcspn(line,"\n")]='\0';
// 					if(strcmp(line,password)==0){
// 					printf("change the password");
// 					fclose(ptr);
// 					break;
// 					}
// 		}
	printf("%s",password);
	printf("\n\n");
	//to repeatedly ask if the user wants to generate a new password
	//or not.
	while(1){
		int choice;
		printf("Press 1 to generate a new password or press 0 to exit:");
		scanf("%d",&choice);
		if(choice==1){
			printf("Your new password is: ");	
			password_generator(password,len,user);
			printf("%s",password);
			printf("\n\n");
		}else{
			printf("Oops!! You have made an exit!!\n");
			break;
		}
	}
	printf("****************************\n");
	printf("\e[4;37mPASSWORD STRENGTH\e[0m\n");
	calculate(password);
	printf("****************************\n");
	return 0;
}
