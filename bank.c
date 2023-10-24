#include <stdio.h>
#include <stdlib.h>
#include <string.h>      

//This struct will be used as a template for a new account 
//or read user files
struct User{
    char name[40]; 
    char middleName[40];
    char surname[40];
    char password[40];
    short int day, month, year;
    long int money;

};         

//This function can be used to clear the input buffer
void emptyInputBuffer() {
    int c;
    while ((c = getchar()) != '\n');
}

//This function is used to create a new user
void createUser(void){
    struct User u1;
    char username[50],   password[40];    
    int month,loop = 1;


    printf("-----CREATE ACCOUNT------\n");
    printf("Name: ");
    scanf("%s", u1.name);

    printf("Middle Name:");
    scanf( "%s", u1.middleName);
    printf("Surname:");
    scanf(" %s", u1.surname);
    printf("Password: ");
    scanf("%s", u1.password);
    printf("Date of Birt:\n");
    printf("Day: ");
    scanf("%hd",&u1.day);


    while (loop == 1) {
        printf("Month: ");
        scanf("%d", &month);
        if (month<1||month>12) {
            printf("Try again...\n");
            continue;        
        }else {
            u1.month = month;
        loop = 0;
        }
    
    }

    printf("Year: ");
    scanf("%hd", &u1.year);
    printf("Balance: ");
    scanf("%ld", &u1.money);

    sprintf(username, "%s%d",u1.name, u1.day);
    printf("%s",username);

    
    FILE *file = fopen(username, "ab");

    if(file==NULL){
        printf("FILE DOES NOT EXIST");
    }

    if (fwrite(&u1, sizeof(struct User),1, file )!=1) {
        printf("FAILED TO WRITE");
        fclose(file);
    
    } 
    
    fclose(file);
    printf("\n-------USER CREATED------\n");
    printf("-----USER NAME: %s------\n",username);

}

//This function is used to transfer money to a different user
void transferMoney(char username[]){
     printf("----TRANSFER MONEY-----\n");
    long int transfer , newbalance, newbalance2;
    char userT[40];
    struct User user, user2;
    
    FILE *file = fopen(username, "r+");

    if(file==NULL){
        printf("USER DATA DOESNT EXIST");
    }

    if (fread(&user, sizeof(struct User),1 , file) != 1) {
        printf("Read error!\n");
        fclose(file); // Close the file
    }

    printf("BALANCE: %ld\n", user.money );

    printf("ENTER USER: ");
    scanf("%s", userT); 

    FILE *file2 = fopen(userT, "r+");
    if(file==NULL){
        printf("USER DATA DOESNT EXIST");
    }


    if (fread(&user2, sizeof(struct User),1 , file2) != 1) {
        printf("Read error!\n");
        fclose(file2); // Close the file
    }                

    printf("TRANSFER AMOUNT: ");
    scanf("%ld", &transfer);

    if ((user.money - transfer) < 0) {
        printf("------BALANCE TOO LOW------");
        fclose(file);
        fclose(file2);
    
    }else {
        newbalance = user.money - transfer;
        printf("\nNEW BALANCE: %ld\n",newbalance);
        user.money = newbalance;
        fseek(file, 0, SEEK_SET);

        if (fwrite(&user, sizeof(struct User),1, file )!=1) {
            printf("FAILED TO WRITE");
            fclose(file);
        
        } 

        newbalance2 = user2.money + transfer;
        user2.money = newbalance2;
        fseek(file2, 0, SEEK_SET);

        if (fwrite(&user2, sizeof(struct User),1, file2 )!=1) {
            printf("FAILED TO WRITE");
            fclose(file2);
        
        } 

        fclose(file);
        fclose(file2);

        printf("--------TRANSACTION COMPLETE------\n");

    }                             

}

//This function lets the logged in user withdraw money

void withdrawMoney(char username[]){
    printf("----WITHDRAW-----\n");
    long int withdraw;
    struct User user;
    
    FILE *file = fopen(username, "r+");

    if(file==NULL){
        printf("USER DATA DOESNT EXIST");
    }

    if (fread(&user, sizeof(struct User),1 , file) != 1) {
        printf("Read error!\n");
        fclose(file); // Close the file
        // Exit the program with an error code
    }

    printf("BALANCE: %ld\n", user.money );
    printf("WITHDRAW AMOUNT: ");
    scanf("%ld", &withdraw);

    if ((user.money - withdraw) < 0) {
        printf("------BALANCE TOO LOW------");
        fclose(file);
    
    }else {
        withdraw = user.money - withdraw;
        printf("\nNEW BALANCE: %ld\n", withdraw);
        user.money = withdraw;
        fseek(file, 0, SEEK_SET);

        if (fwrite(&user, sizeof(struct User),1, file )!=1) {
            printf("FAILED TO WRITE");
            fclose(file);
        
        } 
        fclose(file);

    }
        
}

//This function prints the menu for the user if they successfully login

void successMenu(void){
    printf("\n----------Non-Central Bank----------\n");
    printf("1 USER DETAIL\n");
    printf("2 WITHDRAW MONEY\n");
    printf("3 TRANFER MONEY\n");
    printf("4 LOG OUT\n");
}

//This function is called if the user successfully logs in
//It loops until the user logs out

void loggedIN(char username[]){
    struct User user;
    int lchoice;

    int loggedin = 1;

     while (loggedin == 1) {
       


        successMenu();
        printf("---------USER: %s--------\n", username);
        printf("> ");
        scanf("%d",&lchoice);
        if (lchoice==1) {

            FILE *file = fopen(username, "r+");

            if(file==NULL){
                printf("USER DATA DOESNT EXIST");
            }
            // system("cls");

            if (fread(&user, sizeof(struct User),1 , file) != 1) {
                printf("Read error!\n");
                fclose(file); // Close the file
                // Exit the program with an error code
            }
   
            printf("---------USER DETAILS---------\n");
            printf("Name: %s\n", user.name);
            printf("Middle Name: %s\n", user.middleName);
            printf("Surname: %s\n", user.surname);
            printf("Age: %d\n", 2023 - user.year);
            printf("DOB: %d/%d/%d\n", user.day, user.month, user.year);
            printf("Balance: GBP %lld\n", user.money);
            fclose(file);


        
        }else if (lchoice == 2 ){
            withdrawMoney(username);
        }else if(lchoice == 3) {
            transferMoney(username);
        }else{
            loggedin = 0;
            printf("--------LOGGED OUT---------\n");
        
        }
    
    }
    

}

// This function is called if the user wants to log in
//It checks if the entered username is valid and if the password matches
//If a match is found it gives the user access to the withdraw and transfer functionality
void login(void){
    struct User u2;
    char username[40];
    char password[40];
    printf("------------LOGIN-------------\n");
    printf("Username: ");
    scanf("%40s", username);
    printf("Password: ");
    scanf("%40s", password);
    //fgets(username, sizeof(username), stdin);
    FILE *file = fopen(username, "rb");

    if(file==NULL){
        printf("-------USER DOES NOT EXIST-----\n");
    }
    // system("cls");

    if (fread(&u2, sizeof(struct User),1 , file) != 1) {
        fclose(file); // Close the file
    }

    if (strcmp(u2.password, password)==0) {
        
        loggedIN(username);
        fclose(file);
    }else {
        printf("---------INCORRECT DETAILS----------\n");
    }    
 
}

//Main menu

void printMenu(void){
    //system("cls");
    printf("\n----------Non-Central Bank----------\n");
    printf("1 CREATE ACCOUNT\n");
    printf("2 LOGIN\n");
    printf("3 EXIT\n");
 
}

//The entry point for the program
//This loops until the user exits

int main(){
    int choice;
    int exit = 0;
    while (exit != 1) {
        printMenu();
        //emptyInputBuffer();
        printf("> ");
        scanf("%d",&choice);

        if (choice == 1) {
            createUser();
        }else if (choice == 2 ) {
            login();
        
        }else {
        exit = 1;
        }  
    
    }
    return 0;
}
