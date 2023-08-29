#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
void calculate(const char* pass){
	int alpha,digit,special;
	alpha=digit=special=0;
	for(int i=0;pass[i]!='\0';i++){
		if(isalpha(pass[i])!=0){
			alpha++;
		}else if(isdigit(pass[i])!=0){
			digit++;
		}else{
			special++;
		}
	}
	if(alpha>0 && digit>0 && special>0){
		printf("\nGreat! YOur password is strong\n");
	}else if(alpha>0 && digit>0){
		printf("\nYour password is moderate\n We recommend you to add some special character\n");
	}else{
		printf("\nYour password is weak!\n");
	}
}
bool isSimilar(char c){
	const char similarChars[]="iI1loO0";
	for(int i=0;similarChars[i]!='\0';i++){
		if(c==similarChars[i]){
			return true;
		}
	}
	return false;
}
void excludeSimilar(char* str){
	int len=strlen(str);
	int curr=0;
	if(len<=1){
		return;
	}
	for(int i=0;i<len;i++){
		if(!isSimilar(str[i])){
			str[curr]=str[i];
			curr++;
		}
	}
    str[curr]='\0';

}
void Remove_AllOccurrence(char* str,const char ch,int idx){
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
void removeDuplicates(char* str){
	int len=strlen(str);
	if(len<=1){
		return;
	}
	for(int i=0;i<len;i++){
		Remove_AllOccurrence(str,str[i],i+1);
	}	
}
void passwordgenerator(char password[],int n,int ans1,int ans2,int ans3,int ans4){
	int randomNo=0;
	//initialise random number generator
	srand((int)(time(NULL)));
	//array 1:for storing numbers
	char numbers[]="0123456789";
	//array 2:for storing lowercase alphabets
	char LowerCase[]="abcdefghijklmnopqrstuvwxyz";
	//array 3:for storing uppercase alphabets
	char UpperCase[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	//array 4:for all special characters
	char SpecialChar[]="!@#$%^&*?()";
	//final array for storing randomly generated password to select the random Number inside the loop
	randomNo=rand()%4;
	int i=0;
	//iterating over the length
	while(i<n){
		if( randomNo==1){
			if(ans1==1){
			password[i]=numbers[rand()%10];
			randomNo=rand()%4;
			}else{
			randomNo=rand()%4;
			continue;
			}
		}else if( randomNo==2){
			if(ans2==1){
			password[i]=SpecialChar[rand()%11];
			randomNo=rand()%4;
			}else{
			randomNo=rand()%4;
			continue;
			}
		}else if(randomNo==3){
			if(ans3==1){
			password[i]=LowerCase[rand()%26];
			randomNo=rand()%4;
			}else{	
			randomNo=rand()%4;
			continue;
			}
		}else{
			if(ans4==1){
			password[i]=UpperCase[rand()%26];
			randomNo=rand()%4;
			}else{
				randomNo=rand()%4;
			continue;
			}
		}
		i++;
	}
    password[i]='\0';
}
void printHeading(char* txt){
	int l=strlen(txt);
	printf("\n");
	for(int i=0;i<l+4;i++){
		printf("*");
	}
	printf("\n* \033[1;33m%s\033[0m *\n",txt);
	for(int i=0;i<l+4;i++){
		printf("*");
	}
	printf("\n\n");
}
int main(){
	printHeading("GENERATE A STRONG RANDOM PASSWORD");
	printf("\033[1;31mRules for generating a random password\n");
	printf("________________________________________\n");
	printf("1.Specify the Maximum length of the password\n");
	printf("2.Allow Numbers[0-9]\n");
	printf("3.Allow special characters\n");
	printf("4.Allow Uppercase[ABC]\n");
	printf("5.Allow Lowercase[abc]\n");
	printf("________________________________________\n");
	printf("\033[0m");
	//length of password
	int n;
	printf("Enter the length of the Password: ");
	scanf("%d",&n);
	int ans1,ans2,ans3,ans4,ans5,ans6;
	printf("Want to use numbers (yes/no): \n");
	scanf("%d",&ans1);
	printf("Want to use Special Characters (yes/no): \n");	
	scanf("%d",&ans2);
	printf("Want to use Lowercase Alphabets (yes/no): \n");
	scanf("%d",&ans3);
	printf("Want to use Uppercase Alphabets (yes/no): \n");
	scanf("%d",&ans4);
	printf("Want to exclude duplicate characters (yes/no): \n");
	scanf("%d",&ans5);
	printf("Want to exclude similar characters (yes/no): \n");
	scanf("%d",&ans6);
	printf("Your password is ");
	char password[n];
	//calling the function
	passwordgenerator(password,n,ans1,ans2,ans3,ans4);
	if(ans5==1){
		removeDuplicates(password);
	}
	if(ans6==1){
		excludeSimilar(password);
	}	
	printf("%s",password);
	printf("\n\n");
	//to repeatedly ask if the user wants to generate a new password
	//or not.
	while(1){
		int num;
		printf("Press 1 to generate a new password or press 0 to exit:");
		scanf("%d",&num);
		if(num==1){
			printf("Your new password is: ");	
			passwordgenerator(password,n,ans1,ans2,ans3,ans4);
		if(ans5==1){
		removeDuplicates(password);
		}
		if(ans6==1){
			excludeSimilar(password);
		}
			printf("%s",password);
			printf("\n\n");
		}else{
			printf("You have made an exit!!\n");
			break;
		}
	}
	printf("\033[1;31m##############################\n");
	printf("Password Strength:\n");
	calculate(password);
	printf("##############################\n");
        printf("\033[0m");	
	return 0;
}
