#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

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
