#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>  
#include <string.h>
#include<ctype.h>

struct Data 
{
	char name[30];
	char mobile[30];
	char email[30];
	char address[30];
};

void Add_contact();
void Print_contact();
void Search_contact();
void Edit_contact();
void Delete_contact();
void blink(char str[], const char* );
void valid_names(struct Data *,char names[]);
void valid_num(struct Data *,char num[],int);
void valid_mail(struct Data *, char mail[], int ); 
int repeat_name(struct Data *,char repeat_name[], int );

int main()
{
	printf(":::::::::::::::::::::::::::::::::::::::::::::\n");
	printf("\033[1;31m");  
	printf("         Welcome to Address Book\n");
	printf("\033[0m");
	printf(":::::::::::::::::::::::::::::::::::::::::::::\n");

	char ch = 'y';
	int choice;

	while (ch == 'y' || ch == 'Y')
	{
		printf("\033[1;33m"); 
		printf("            1. Add data    \n");
		printf("            2. Print Data \n");
		printf("            3. Search Data \n");
		printf("            4. Edit Data \n");
		printf("            5. Delete Data \n");
		printf("            6. Exit \n");
		printf("\n");
		printf("\033[0m");
		printf("Enter your choice: ");
		scanf("%d", &choice);

		switch (choice)
		{
			case 1: Add_contact(); break;
			case 2: Print_contact(); break;
			case 3: Search_contact(); break;
			case 4: Edit_contact(); break;
			case 5: Delete_contact(); break;
            case 6:
			       printf("\n");
			       char bye[] = "Exiting the address book. Goodbye!\n";
			       blink(bye,"\033[1;31m"); 
				   printf("\n");
			       exit(0);  
			default: printf("Invalid choice. Try again.\n");
		}
		printf("\n");
		printf("Do you want to continue Y/N: ");
		scanf(" %c", &ch);  
	}

	return 0;
}

void Add_contact()
{
	struct Data contacts;
	FILE *fptr = fopen("Data.txt", "r");
	if (fptr == NULL)
	{
		fptr = fopen("Data.txt", "w+");
		if (fptr == NULL)
		{
			printf("Error opening file for writing.\n");
			return;
		}
		fprintf(fptr, "0\n"); 
		fclose(fptr);
		fptr = fopen("Data.txt", "r+"); 
	}

	FILE *temp = fopen("temp.txt", "w");
	if (temp == NULL)
	{
		printf("Error opening temp file.\n");
		fclose(fptr);
		return;
	}

	int count = 0;
	fscanf(fptr, "%d\n", &count);
	fprintf(temp, "%d\n", count + 1);

	for (int i = 0; i < count; i++)
	{
		fscanf(fptr, "%[^,],%[^,],%[^,],%[^\n]\n", contacts.name, contacts.mobile, contacts.email, contacts.address);
		fprintf(temp, "%s,%s,%s,%s\n", contacts.name, contacts.mobile, contacts.email, contacts.address);
	}

	char str1[] = "Add the data below.";
	blink(str1,"\033[1;32m");
        printf("\n");
	printf("........................................\n");
    
	char names[30];
    valid_names(&contacts,names);

	char num[30];
	valid_num(&contacts,num,count);

    char mail[30];
    valid_mail(&contacts, mail, count);

	printf("Enter the address: ");
	scanf(" %[^\n]", contacts.address);
	printf("..........................................\n");

	char str2[] = "Data added successfullyy!!!";
	blink(str2,"\033[1;32m");
	printf("\n");


	fprintf(temp, "%s,%s,%s,%s\n", contacts.name, contacts.mobile, contacts.email, contacts.address);

	fclose(fptr);
	fclose(temp);

	remove("Data.txt");
	rename("temp.txt", "Data.txt");
}


void Print_contact()
{
	FILE *fptr = fopen("Data.txt", "r");
	if (fptr == NULL)
	{
		printf("Error opening file.\n");
		return;
	}

	struct Data contacts;
	int count;


	if (fscanf(fptr, "%d\n", &count) != 1 || count == 0)	
	{
		printf("\n");
		printf("No contact added yet!!!\n");
		fclose(fptr);
		return;
	}


	printf("Contacts List:\n");
	printf("--------------------------------------------------\n");
	for (int i = 0; i < count; i++)
	{
		if (fscanf(fptr, "%[^,],%[^,],%[^,],%[^\n]\n",
					contacts.name, contacts.mobile, contacts.email, contacts.address) == 4)
		{
			printf("\033[1;34m");
			printf("Name     : %s\n", contacts.name);
			printf("Mobile   : %s\n", contacts.mobile);
			printf("Email    : %s\n", contacts.email);
			printf("Address  : %s\n", contacts.address);
			printf("\033[0m");
			printf("--------------------------------------------------\n");
		}
		else
		{
			printf("Error reading contact data.\n");
			break;
		}
	}

	fclose(fptr);
}

void Search_contact()
{
    struct Data contacts[100];  
    FILE *search = fopen("Data.txt", "r");
    if (search == NULL)
    {
        printf("\n");
        char str8[] = "File not found!";
        blink(str8,"\033[1;31m");
        printf("\n");
        return;
    }

    int count = 0;
    if (fscanf(search, "%d\n", &count) != 1 || count <= 0)
    {
        printf("\n");
        char str9[] = "Error reading the number of contacts.";
        blink(str9,"\033[1;31m");
        printf("\n");
        fclose(search);
        return;
    }

    
    for (int i = 0; i < count; i++)
    {
        if (fscanf(search, "%[^,],%[^,],%[^,],%[^\n]\n", contacts[i].name, contacts[i].mobile, contacts[i].email, contacts[i].address) != 4)
        {
            printf("\nError reading contact data.\n");
            fclose(search);
            return;
        }
    }

    char search_name[30];
    printf("Enter the name to search: ");
    scanf(" %[^\n]", search_name);

    int found = 0;
    int occurrence = 0;

    
    for (int i = 0; i < count; i++)
    {
        if (strcmp(contacts[i].name, search_name) == 0)
        {
            occurrence++;
            found = 1; 
        }
    }

    if (!found)
    {
        printf("\nNo contact found with the name: %s\n", search_name);
    }
    else if (occurrence == 1)
    {
        for (int i = 0; i < count; i++)
        {
            if (strcmp(contacts[i].name, search_name) == 0)
            {
                printf("\n");
                char var4[] = "Contact found!!!";
                blink(var4,"\033[1;32m");
                printf("\n");
                printf("Name    : %s\n", contacts[i].name);
                printf("Mobile  : %s\n", contacts[i].mobile);
                printf("Email   : %s\n", contacts[i].email);
                printf("Address : %s\n", contacts[i].address);
                break;
            }
        }
    }
    else 
    {
        printf("\n");
        char var3[] = "The name exists more than once!!";
        blink(var3,"\033[1;31m");
        printf("\n");
        
        char search_num[15];
        int matched = 0;

        while (1)
        {
            printf("Enter the mobile number of the contact: ");
            scanf(" %[^\n]", search_num);

            
            for (int i = 0; i < count; i++)
            {
                if (strcmp(contacts[i].mobile, search_num) == 0)
                {
                    printf("\n");
                    char var5[] = "Contact found!!!";
                    blink(var5,"\033[1;32m");
                    printf("\n");
                    printf("Name    : %s\n", contacts[i].name);
                    printf("Mobile  : %s\n", contacts[i].mobile);
                    printf("Email   : %s\n", contacts[i].email);
                    printf("Address : %s\n", contacts[i].address);
                    matched = 1;
                    break; 
                }
            }

            if (matched)
                break; 
            printf("\n");
            char var2[] = "Invalid number, try again!!!";
            blink(var2,"\033[1;31m");
            printf("\n");
        }
    }

    fclose(search);
}



void Edit_contact() 
{
    FILE *fptr = fopen("Data.txt", "r");
    if (fptr == NULL) 
    {
        printf("File not opened\n");
        return;
    }

    struct Data contacts[100];
    int contact_count = 0;

    fscanf(fptr, "%d\n", &contact_count);
    for (int i = 0; i < contact_count; i++) 
    {
        fscanf(fptr, "%[^,],%[^,],%[^,],%[^\n]\n", contacts[i].name, contacts[i].mobile, contacts[i].email, contacts[i].address);
    }

    fclose(fptr);

    char new_data[30];
    printf("Enter the name of the contact you want to modify: ");
    scanf(" %[^\n]", new_data);

    int res = repeat_name(contacts, new_data, contact_count);
    if (res) 
    {
        for (int i = 0; i < contact_count; i++) 
        {
            if (strcmp(new_data, contacts[i].name) == 0) 
            {
                printf("What do you want to edit?\n");
                printf("1. Name\n");
                printf("2. Mobile\n");
                printf("3. Email\n");
                printf("4. Address\n");

                int opt;
                printf("Enter your choice: ");
                scanf("%d", &opt);

                switch (opt) 
                {
                    case 1:
                        valid_names(&contacts[i], contacts[i].name);
                        break;
                    case 2:
                        valid_num(&contacts[i], contacts[i].mobile, contact_count);
                        break;
                    case 3:
                        valid_mail(&contacts[i], contacts[i].email, contact_count);
                        break;
                    case 4:
                        printf("Enter the new address: ");
                        scanf(" %[^\n]", contacts[i].address);
                        break;
                    default:
                        printf("Invalid option.\n");
                        return;
                }
                break;
            }
        }

        fptr = fopen("Data.txt", "w");
        if (fptr == NULL)
        {
            printf("File not opened\n");
            return;
        }

        fprintf(fptr, "%d\n", contact_count);
        for (int i = 0; i < contact_count; i++) 
        {
            fprintf(fptr, "%s,%s,%s,%s\n", contacts[i].name, contacts[i].mobile, contacts[i].email, contacts[i].address);
        }
        fclose(fptr);
        printf("\n");
        char var[] ="Contact updated successfully!!";
		blink(var,"\033[1;32m");
		printf("\n");
    } 
    else 
    {
		printf("\n");
        char var1[] = "No contact found with the provided name.";
		blink(var1,"\033[1;31m");
		printf("\n");
    }
}



void Delete_contact() 
{
	FILE *fptr = fopen("Data.txt", "r"); 
	if (fptr == NULL) 
	{
		printf("File not opened\n");
		return;
	}

	struct Data contacts[100];
	int contact_count = 0;


	fscanf(fptr, "%d\n", &contact_count);


	for (int i = 0; i < contact_count; i++) 
	{
		fscanf(fptr, "%[^,],%[^,],%[^,],%[^\n]\n", contacts[i].name, contacts[i].mobile, contacts[i].email, contacts[i].address);
	}

	fclose(fptr);

	char delete_name[30];
    printf("\n");
	printf("Enter the name of the contact to delete: ");
	scanf(" %[^\n]", delete_name);

	int found = 0;
    int res = repeat_name(contacts,delete_name, contact_count);
    
	if (res) 
    {
	for (int i = 0; i < contact_count; i++) 
	{
		if (strcmp(contacts[i].name, delete_name) == 0) 
		{
			found = 1;


			for (int j = i; j < contact_count - 1; j++) 
			{
				contacts[j] = contacts[j + 1];
			}

			contact_count--; 
			break;
		}
	}

	if (found) 
	{

		fptr = fopen("Data.txt", "w");
		if (fptr == NULL) 
		{
			printf("File not opened\n");
			return;
		}


		fprintf(fptr, "%d\n", contact_count);


		for (int i = 0; i < contact_count; i++) 
		{
			fprintf(fptr, "%s,%s,%s,%s\n", contacts[i].name, contacts[i].mobile, contacts[i].email, contacts[i].address);
		}

		fclose(fptr);
		char str1[] ="Contact deleted successfully!\n";
                blink(str1,"\033[1;31m");
	} 
	else 
	{
		char str2[] ="Contact not found!\n";
                blink(str2,"\033[1;31m");
	}
}
}

void blink(char str[], const char* color) 
{
    printf("%s", color);  

    for (int i = 0; str[i] != '\0'; i++) 
    {
        printf("%c", str[i]);
        fflush(stdout);  
        usleep(100000);  
    }

    printf("\033[0m\n");  
}



void valid_names(struct Data *contacts,char names[])
{
int valid_name = 0;
	while(!valid_name)
	{
		printf("Enter the name :");
		scanf(" %[^\n]", contacts->name);
		valid_name = 1;
		for(int i = 0 ;i<strlen(contacts->name);i++)
		{
			if (!isalpha(contacts->name[i]) && contacts->name[i] != ' ' && contacts->name[i] != '.')
			{
				valid_name = 0;
				printf("\n");
				char str7[] = "ERORR :The name should not have any digits and special characters";
				blink(str7, "\033[1;31m");
				printf("\n");
				break;
			}
		}
	}
}


void valid_num(struct Data *contacts, char num[], int count)
 {
    int valid = 0;
    while (!valid) {
        printf("Enter the mobile number: ");
        scanf(" %[^\n]", contacts->mobile);

        int length = strlen(contacts->mobile);
        valid = 1;

        for (int i = 0; i < length; i++) 
		{
            if (!isdigit(contacts->mobile[i])) 
			{
                valid = 0;
                printf("Error: Mobile number should only contain digits.\n");
                break;
            }
        }

        if (valid && length != 10)
		{
            valid = 0;
            printf("The number should be exactly 10 digits.\n");
        }

        if (valid)
		 {
            FILE *fptr = fopen("Data.txt", "r"); 
            if (fptr == NULL) 
			{
                printf("File not opened\n");
                return;
            }
            
            struct Data temp_data;
            fscanf(fptr, "%d\n", &count); 
            for (int i = 0; i < count; i++) {
                fscanf(fptr, "%[^,],%[^,],%[^,],%[^\n]\n", temp_data.name, temp_data.mobile, temp_data.email, temp_data.address);
                if (strcmp(temp_data.mobile, contacts->mobile) == 0) 
				{
                    valid = 0;
                    printf("The number already exists. Try again.\n");
                    break;
                }
            }
            fclose(fptr);
        }
    }
}


void valid_mail(struct Data *contacts, char mail[], int count) 
{
    int valid_email = 0;

    while (!valid_email)
    {
        printf("Enter the email id: ");
        scanf(" %[^\n]", mail);
       
        int length = strlen(mail);
        valid_email = 1; 

        for (int i = 0; i < length; i++)
        {
            if (mail[i] == '@') 
            {
                if (isalnum(mail[i - 1]) && isalnum(mail[i + 1])) 
                {
                    continue;
                } 
                else 
                {
                    valid_email = 0;
					printf("\n");
                    char str5[] = "Invalid '@' placement in the email!!";
                    blink(str5, "\033[1;31m");
					printf("\n");
                    break;
                }
            }

            if (mail[i] == '.') 
            {
                if (isalnum(mail[i - 1])) 
                {
                    valid_email = 1;
                    break;  
                } 
                else 
                {
                    valid_email = 0;
					printf("\n");
                    char str6[] = "Invalid '.' placement in the email!!";
                    blink(str6, "\033[1;31m");
					printf("\n");
                    break;
                }
            }
        }

        if (strstr(mail, "@") == NULL || strstr(mail, ".com") == NULL) 
        {
            valid_email = 0;
            printf("\n");
            printf("Error: Invalid email format!!!\n");
            printf("\n");
        }

        if (valid_email)
        {
            FILE *fptr = fopen("Data.txt", "r");
            struct Data temp_data;
            while (fscanf(fptr, "%[^,],%[^,],%[^,],%[^\n]\n", temp_data.name, temp_data.mobile, temp_data.email, temp_data.address) == 4) {
                if (strcmp(temp_data.email, mail) == 0) 
                {
					printf("\n");
                    char string[] ="The email id already exists!!!";
					blink(string, "\033[1;31m");
					printf("\n");
                    valid_email = 0;
                    break;
                }
            }
            fclose(fptr);
        }
    }
    strcpy(contacts->email, mail);

    for (int i = 0; i < strlen(contacts->email); i++) 
    {
        contacts->email[i] = tolower(contacts->email[i]);
    }
}



int repeat_name(struct Data *contacts, char repeat_name[], int contact_count)
{
    int duplicate_count = 0;
    int found = 0;

    
    for (int i = 0; i < contact_count; i++)
    {
        if (strcmp(repeat_name, contacts[i].name) == 0)
        {
            duplicate_count++;
            found = 1;
        }
    }

    
    if (found == 0)
    {
        printf("\nNo contact found with the name: %s\n", repeat_name);
        return 0;
    }

    
    if (duplicate_count > 1)
    {
        printf("\n");
        char ptr6[] = "The name exists more than once!!";
        blink(ptr6, "\033[1;31m");
        printf("\n");

        char search_num[30];
        int matched = 0;

        
        while (1)
        {
            printf("Enter the mobile number of the contact: ");
            scanf(" %[^\n]", search_num);

            for (int i = 0; i < contact_count; i++)
            {
                if (strcmp(repeat_name, contacts[i].name) == 0 && strcmp(contacts[i].mobile, search_num) == 0)
                {
                    printf("\n");
                    char ptr7[] = "Contact found!";
                    blink(ptr7, "\033[1;32m");
                    printf("\n");

                    printf("Name    : %s\n", contacts[i].name);
                    printf("Mobile  : %s\n", contacts[i].mobile);
                    printf("Email   : %s\n", contacts[i].email);
                    printf("Address : %s\n", contacts[i].address);

                    matched = 1;
                    break;
                }
            }

            if (matched)
                break; 
            printf("\n");
            char ptr8[] = "Invalid number, try again!";
            blink(ptr8, "\033[1;31m");
            printf("\n");
        }
        return 1;
    }

   
    return 1;
}
