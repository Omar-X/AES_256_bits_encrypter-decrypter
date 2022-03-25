// omar masoud                          040190903
// https://drive.google.com/file/d/1fpVM9nzNSMK0xdz9WfSC-ZnTa1JpJXr5/view?usp=sharing
// https://drive.google.com/file/d/1LPxApbFVJp5c-01ZU8PPa_SQeQVDZBRc/view?usp=sharing

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include<gtk/gtk.h>
#include <time.h>
#define True 1
#define False 0

// substitution box with mod p(x) = x^(8)+x^(4)+x^(3)+x+1 == 0b100011011
static const uint8_t Sub_Box[16][16] = {
      //  0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
 /*0*/  {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},
 /*1*/  {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0},
 /*2*/  {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},
 /*3*/  {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},
 /*4*/  {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},
 /*5*/  {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},
 /*6*/  {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},
 /*7*/  {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},
 /*8*/  {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},
 /*9*/  {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},
 /*A*/  {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},
 /*B*/  {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},
 /*C*/  {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},
 /*D*/  {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},
 /*E*/  {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},
 /*F*/  {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}};

// inverse of the substitution box for decryption process
static const uint8_t In_Sub_Box[16][16] = {
      //   0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
        {0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb},
        {0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb},
        {0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e},
        {0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25},
        {0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92},
        {0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84},
        {0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06},
        {0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b},
        {0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73},
        {0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e},
        {0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b},
        {0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4},
        {0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f},
        {0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef},
        {0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61},
        {0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d}};

// this set is used for mix column layer in encryption process
static const int m_GF8_Box[4][4]={
        {0x02,0x03,0x01,0x01},
        {0x01,0x02,0x03,0x01},
        {0x01,0x01,0x02,0x03},
        {0x03,0x01,0x01,0x02}};

// for inverse mix column in decryption process
static const int in_m_GF8_Box[4][4] = {
        {0x0e,0x0b,0x0d,0x09},
        {0x09,0x0e,0x0b,0x0d},
        {0x0d,0x09,0x0e,0x0b},
        {0x0b,0x0d,0x09,0x0e}};

char Initialization_vector[17] = "omar_masoud_0401"; //IV for CBC mode
int back_up[100000][4][4] = {0}; // for CBC in decryption process XORed.
int round_key_array[60][4]={0}; // to set all key arrays from w(0) to w(256) for 256 bits key
int data_to_encrypt[100000][4][4]={0};

                //    {xi1, xi2, xi3, xi4}
// random box[xi] =   {xi5, xi6, xi7, xi8}         a box of 128 bits == 16 bytes
                //    {x9, xi10, xi11, xi12}
                //    {xi13, xi14, xi15, xi16}

int GF_multiplication_process(int , int ); //prototype for multiplication function in Galois filed
void Sub_process(int ); // for substitution layer in encryption
void In_Sub_process(int ); // for reverse substitution layer in decryption
double Encryptor(int );
double Decrypter(int );
void Shift_Row_process(int ); // for Shift layer in encryption
void In_Shift_Row_process(int ); // for reverse substitution layer in decryption
void Mix_Col_process(int); // for mix layer in encryption
void In_Mix_Col_process(int); // for reverse mix layer in encryption
int get_round_key(char* ); // calculating the arrays from W(0) to W(59)
char random_IV(char IV[17]){
    srand(time(NULL)); // calculating random 16-bytes initialization vector
    for (int i = 0; i < 16; ++i) {
        IV[i] = (char) (rand()%80+35);}
    IV[16] = '\0';
    return 0;} // for encryption process

// separating data into 16 bytes boxes to data_to_encrypt
int organize_data(char* data){
    int number_of_bytes = strlen(data) ;
    printf("%s\n",data);
    int k , count = 0;
    for (k = 0;number_of_bytes > 0 ; number_of_bytes -= 16)
    {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) { // completing last box with null char to reach 16 bytes
                if (data[count] == '\0')  data_to_encrypt[k][i][j] = 0x00; // NULL character

                else {
                    data_to_encrypt[k][i][j] = data[count];
                    count++;
}}}
        k ++;}
    return k; // number of boxes to encrypt
}

int main (void) {
    FILE * file, *report_file, *long_file, *new_file;
    DIR * folder, *test_folder;
    char back_up_path[1000]; // to save folder path
    unsigned count= 0;
    struct dirent *entry; // containing files within the chosen dir
    char files[1000][1000]; // set of files inside a folder
    char choose; //E or D
    char path[1000];    // file path with space at the end
    char t_path[1000]; // same path but without spaces in the end
    int number_of_processes; // or number of boxes to encrypt/decrypt
    double time_taken;
    random_IV(Initialization_vector);
    do {
        puts("press E to encrypt or D to decrypt");
        choose = getchar();
        if (choose == 'E'|| choose == 'D')break;
        else printf("Wrong enter try again");
    } while (True);
    getchar();
    do{ // path of the folder that contains the needed file
        puts("enter the path of a folder");
        fgets(path, 1000, stdin);
        for (int i = 0; i < strlen(path); ++i) {
            if (!isspace(path[i])) {
                t_path[i] = path[i];
            } else {
                t_path[i] = '\0';
                break;}}

        if ((folder = opendir(t_path)) == NULL) {
            printf("invalid path please try again\n");
        } else break;
    } while (True);
    count = 0;
    while ((entry=readdir(folder))) { // reading files within the folder
        strcpy(files[count], entry->d_name);
        printf("%d %s  \n",count+1, files[count]); count++;
    }// count total number of files in that folder
    closedir(folder);
    strcat(t_path,"/");
    strcpy(back_up_path,t_path);
    // asking for the desired file
    char choice[3];
    do{
        printf("\nenter the number next to the file/folder to %s or enter A for all: ",
               (choose == 'E') ? "Encrypt" : "decrypt");
        fgets(choice, 3, stdin);
        if (atoi(choice) > count && choice[0] !='A'&& choice[0] !='a' )puts("wrong entry try again ");
        else if((test_folder = opendir(strcat(t_path,files[atoi(choice)-1]))) != NULL){
            printf("your choice is a folder not a file try again \n");
            strcpy(t_path,back_up_path);
        closedir(test_folder);
        getchar();
        }
        else break;
    } while (True);
    strcpy(t_path,back_up_path);

        // asking for IV decryption case
    if (choose == 'D'){
        getchar(); // avoiding enter entry
        printf("enter your initialization vector: ");
        fgets(Initialization_vector,17,stdin);
        getchar(); // avoiding enter entry
        Initialization_vector[16] = '\0';
    } else {
        printf("your initialization vector %s\n", Initialization_vector);
        getchar(); // avoiding enter entry
    }


    // asking for the key
    char key[33];
    do{
        puts("please enter your key (32 letters): "); // 156 bits key
        fgets(key, 33, stdin);
        if (strlen(key) != 32)puts("the key doesn't contain 32 letters try again");
        else break;
    } while (True);

    get_round_key(key); // calculating the 60 arrays of the key


    int file_length; // number of letters within that file
    if (choice[0] != 'A'&& choice[0] != 'a') { // preparing to encrypt/decrypt a file.
        // reading the entire file and add it to a string variable
        char file_name[300];
        printf("the file name is %s\n",files[atoi(choice) - 1]);
        strcpy(file_name,files[atoi(choice) - 1]);
        strcat(t_path,file_name); // complete path
        t_path[strlen(t_path)] = '\0';
        file = fopen(t_path, "rb");
        fseek(file, 0, SEEK_END);
        file_length = ftell(file); // calculating the length of a file using ftell
        fseek(file, 0, SEEK_SET);
        if (choose == 'E') {
            char string[file_length], str[1000]; //str just reads line after that it's added to string
            string[0] = '\0';
            while (fgets(str, 1000, file) != '\0')
                strcat(string, str);
            fclose(file);
            number_of_processes = organize_data(string); // number of boxes to encrypt/decrypt.
            time_taken = Encryptor(number_of_processes);
        } else{
            number_of_processes = file_length/16;
            puts("check the Encrypted_file");
            for (int i = 0; i < number_of_processes; ++i) {
                for (int j = 0; j < 4; ++j) {
                    for (int k = 0; k < 4; ++k) {
                     data_to_encrypt[i][j][k] = fgetc(file);
                     printf("%c\t",data_to_encrypt[i][j][k]);
                    }
                printf("\n");
                }}
            getchar();
            time_taken = Decrypter(number_of_processes);
        }
        printf("the number of processes = %d",number_of_processes);

       getchar();// avoid enter entry
       char choice2;
        do {
            puts("\nsave in a new file enter S or rewrite the file itself enter R");
            choice2 = getchar();
            if (choice2 != 'S' && choice2 != 'R') {
                puts("Wrong entry try again");
            getchar(); }//avoiding enter entry
            else break;
        } while (True);
        getchar();// avoid enter entry
        if (choice2 == 'S'){
       printf("enter the the full path of the %s file (including the name): \n",(choose =='E')?"encrypted":"decrypted");
       char new_path[1000],t_new_path[1000];
       fgets(new_path,1000,stdin);
        for (int i = 0; i < strlen(new_path); ++i) {
            if (!isspace(new_path[i])) {
                t_new_path[i] = new_path[i]; // calculating the path without spaces at the end
            } else {
                t_new_path[i] = '\0';
                if (choose == 'D'){ strcat(t_new_path, ".txt"); }
                break;}}

            do{
                if ((new_file = fopen(t_new_path, "wb")) == NULL) { puts("can't open that file try again"); break; }
                else {
                    for (int i = 0; i < number_of_processes; ++i) {
                        for (int j = 0; j < 4; ++j) {
                            for (int k = 0; k < 4; ++k) {
                                  fprintf(new_file, "%c", data_to_encrypt[i][j][k]); // adding data to the global box to encrypt/decrypt later
                            }}}
                    printf("file saved successfully with key (%s) and IV is (%s) ", key, Initialization_vector);
                    break;}
            } while (True);
        } else{
            if (choose == 'D'){ strcat(t_path, ".txt"); }
            file = fopen(t_path,"wb"); // writing the cipher text over the plain text
            for (int i = 0; i < number_of_processes; ++i) {
                for (int j = 0; j < 4; ++j) {
                    for (int k = 0; k < 4; ++k) {
                        fprintf(new_file, "%c", data_to_encrypt[i][j][k]);
                    }}}
        }
            printf("file saved successfully with key (%s) and IV is (%s) \n", key, Initialization_vector);
            printf("time taken = %lf s\n",  time_taken);
            puts("enter the name of the report file with path (to keep important infos like IV for decryption process): ");
            char report_name[50];
            fgets(report_name, 50, stdin);
            strcat(report_name, ".txt");
            report_file = fopen(report_name, "w");
            fprintf(report_file, "%s\n", t_path);
            fprintf(report_file, "file saved successfully with key (%s) and IV is (%s) \n", key, Initialization_vector);
            fprintf(report_file,"time taken = %lf s\n",  time_taken);

        printf("time taken = %lf s\n",  time_taken);
            fclose(report_file);

    }

    else{

        // encrypting/decrypting the whole folder.
        char report_name[50];
        char string[800000]; // save the content of each file max limit 100KB
        char line_str[500]; // read each line of a file
        char file_name[300];
        char files_paths[1000][1000];
        char folders_paths[1000][1000];
        int number_of_files = 0, number_of_folders = 0,counter = 0;
        time_taken = 0;
        double time_list = 0;
        strcpy(back_up_path,t_path);
        // file for encrypted data
        getchar();// avoiding enter entry
        puts("enter the name of the file to save the encrypted folder: ");
        fgets(file_name,300,stdin);
        strcat(t_path,file_name);
        long_file = fopen(t_path,"ab");
        strcpy(t_path,back_up_path); // get folder path back

        // report file
        puts("enter the name of the report file");
        fgets(report_name,50,stdin);
        strcat(report_name,".txt");
        strcat(t_path,report_name);
        report_file = fopen(t_path,"ab"); // to add key IV and time for each file
        strcpy(t_path,back_up_path);

    char previous_folder[1000];
        while (counter <= number_of_folders) {

            if (counter == 0) {

                printf("\ncount is %d ", count);
                for (int i = 0; i < count - 1; ++i) {
                    if (files[i][0] != '.') {
                        strcat(t_path, files[i]);
                        if ((test_folder = opendir(t_path)) != NULL) {
                            strcpy(folders_paths[number_of_folders], t_path);
                            number_of_folders++;

                        } else {
                            strcpy(files_paths[number_of_files], t_path);
                            number_of_files++;
                        }
                        strcpy(t_path, back_up_path);
                        closedir(test_folder);
                    }
                }
            } else {
                if ((folder = opendir(folders_paths[counter - 1])) != NULL) {
                    strcpy(previous_folder, folders_paths[counter - 1]);
                    while ((entry = readdir(folder))) {
                        if (entry->d_name[0] != '.') {
                            strcat(folders_paths[counter - 1], "/");
                            strcat(folders_paths[counter - 1], entry->d_name);
                            if ((test_folder = opendir(folders_paths[counter - 1])) != NULL) {
                                strcpy(folders_paths[number_of_folders], folders_paths[counter - 1]);
                                number_of_folders++;
                                closedir(test_folder);

                            } else {
                                strcpy(files_paths[number_of_files], folders_paths[counter - 1]);
                                number_of_files++;
                            }
                        }
                            strcpy(folders_paths[counter - 1], previous_folder);
                        }
                }
                closedir(folder);
            }
            counter++;
            strcpy(t_path, back_up_path);
        }

        puts("All files within that folder are: ");
        for (int i = 0; i <= number_of_files; ++i) {
            printf("%s\n",files_paths[i]);
        }
    puts("press anything to continue");
    getchar();

        for (int i = 0; i <= number_of_files; ++i) {

            if ((file  = fopen(files_paths[i],"rb")) ==NULL ){puts("can't open that file");}
            else{
                string[0] = '\0';
                while (fgets(line_str,1000,file) != '\0')
                    strcat(string,line_str); //adding the file content to string variable
                fclose(file);
                number_of_processes = organize_data(string); // calling organize data to calculate # boxes
                time_taken = Encryptor(number_of_processes);
                time_list += time_taken;
                for (int l = 0; l < number_of_processes; ++l) {
                    for (int j = 0; j < 4; ++j) {
                        for (int k = 0; k < 4; ++k) {
                            fputc(data_to_encrypt[l][j][k],long_file); // adding encrypted data to the file
                        }}}
                printf("\n%s\n",files_paths[i]);
                printf("file saved successfully with key (%s) and IV is (%s) \n",key,Initialization_vector);
                printf("time taken = %lf s\n",  time_taken);
                sleep(1);
                fprintf(report_file,"%s\n",files_paths[i]);
                fprintf(report_file,"file saved successfully with key (%s) and IV is (%s) \n",key,Initialization_vector);

            } strcpy(t_path,back_up_path);
        }
        fclose(report_file);
        printf("total time taken %.4lf s",time_list);
    }
    sleep(5);
    printf("\n\n");
    for (int i = 0; i < number_of_processes; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
             printf("%c",data_to_encrypt[i][j][k]);
            }}}
    return 0;

}



int GF_addition(int x, int y, int z, int t){ // addition is Xor operation
    int result=0, separator;
    for (int i = 7; i >= 0; i--) {
        separator = (int)pow(2,i);// 2^(2) = 0b00000010
        // comparing each bit in the four inputs.
        if ((((x&separator)>>i)^((y&separator)>>i)^((z&separator)>>i)^((t&separator)>>i)) == 1){
            result += separator; // output
        }}return result;
}

int GF_multiplication_process(int x, int y){
    // multiplication with mod p(x) for GF(2^(8))
    int result_p=0;
    int set[16] = {0},separator_i,separator_j, separator_k =0;
    for (int i = 0; i < 8; ++i) {
        separator_i = (int)pow(2,i);
        for (int j = 0; j < 8; ++j) {
            separator_j =(int) pow(2,j);
            if (((x&separator_i)>>i == 1) && ((y&separator_j)>>j == 1) ){
                if (set[i + j] ==0){
                    set[i + j] = 1;
                    result_p += (int)pow(2,i+j);
                }
                else{
                    set[i + j] = 0;
                    result_p -= (int)pow(2,i+j);
                }}}}



    int mod = 0b100011011;// p(x) standard for AES
    for (int k = 8+14; k >= 0; k--) { //if the result is bigger than the mod narrow it
        separator_k = (int)pow(2,k);
        if (k<8) break; // below the mod limit .
        if ((((result_p&separator_k)>>k) ==1) && k>=8 ){// example (11001100 & 2^7)>>7 ==( 11000110011001100 & 01001100)>>7
            result_p = result_p^(mod<<(k-8)); //
        }}
    return result_p;
}

int get_round_key(char *key){// calculating the arrays
    int count = 0;
    for (int i = 0; i < 8; ++i) { // first 8 words are the same no changes. W(0) --> W(7)
        for (int j = 0; j < 4; ++j) {
            round_key_array[i][j] = key[count];
            count++;
        }
    }

    count = 0;
    int RC = 0b00000001; //for g(x) function
    for (int i = 8; i <60 ; ++i) {
        if (count%8 == 0){
            // calculating g(x) for w(4i) words
            count = 0;
            // shifting process
            round_key_array[i][0] = round_key_array[i-1][1];
            round_key_array[i][1] = round_key_array[i-1][2];
            round_key_array[i][2] = round_key_array[i-1][3];
            round_key_array[i][3] = round_key_array[i-1][0];
            for (int j = 0; j < 4; ++j) { // substitution process
                round_key_array[i][j] = Sub_Box[round_key_array[i][j]>>4][round_key_array[i][j]&0x0f];
            }
            round_key_array[i][3] += RC; // adding RC to the last byte of W
            RC = RC<<1;
            for (int j = 0; j < 4; ++j) {
                round_key_array[i][j] ^= round_key_array[i-8][j];
            }

        } else if (count == 4){ // calculating h(x) function
            for (int j = 0; j < 4; ++j) {//substitution process
                round_key_array[i][j]= Sub_Box[round_key_array[i-1][j]>>4][round_key_array[i-1][j]&0x0f]^
                round_key_array[i-8][j]; //0x47>>4 = 4, 0x47 & 0x0f = 7
            }}
        else {
            for (int j = 0; j < 4; ++j) {// for the rest of the keys
                round_key_array[i][j] = round_key_array[i - 1][j] ^ round_key_array[i - 8][j];
            }
        }
        count++;}
}

double Encryptor(int number_of_processes){
    // encryption process
    // number_of_process refers to the number of boxes
    int count = 0;
    long int time_taken = time(NULL);
    for (int i = 0; i < number_of_processes; ++i) {
        if (i == 0) {
            count = 0;
            for (int j = 0; j < 4; ++j) {
                for (int k = 0; k < 4; ++k) {// Xor IV for the first box Xi for CBC mode
                    data_to_encrypt[i][j][k] ^= Initialization_vector[count];
                    count++;
                }}count = 0;}
        else{
            for (int j = 0; j < 4; ++j) {
                for (int k = 0; k < 4; ++k) { //Xor last encrypted box with the new plaintext box
                    data_to_encrypt[i][j][k] ^= data_to_encrypt[i-1][j][k];
                }}}

        for (int l = 1; l < 15; ++l) {// 14 loops for the 60 array of the key 256bits key

            if (l == 1) {
                for (int j = 0; j < 4; ++j) {
                    for (int k = 0; k < 4; ++k) { //key addition layer (K0) not in the loop just for first box
                        data_to_encrypt[i][j][k] ^= round_key_array[j][k];
                    }}}

            Sub_process(i); // substitution layer

            Shift_Row_process(i);// shift layer

            if (l != 14) { Mix_Col_process(i); } // mix layer except for last round Rn

            for (int j = 0; j < 4; ++j) {
                for (int k = 0; k < 4; ++k) {// Ki addition process total 15 keys, 4 arrays in each round + K0
                    data_to_encrypt[i][j][k] ^= round_key_array[(j + 4*l)][k];
                }}

        }
        int percentage;
        percentage = (i*100/number_of_processes);
        for (int nl = 0; nl < 4; ++nl) {       // displaying the data in stdout file
            printf("\n");}
        printf("\033[0;34m");//adding blue color to the loading bar
        printf("[");
        for (int j = 0; j < percentage; ++j) {
            printf("#");}
        for (int j = 0; j < 100 - percentage; ++j) {
            printf(" ");}
        printf("]");
        printf("%%%d\n",(int)(((double )i/(double )number_of_processes)*100.));
        printf("\033[0m");
    }
    time_taken = time(NULL) - time_taken ; // time in seconds
    return (double) time_taken;}

double Decrypter(int number_of_processes){
    // decryption process

    for (int i = 0; i < number_of_processes; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                back_up[i][j][k] = data_to_encrypt[i][j][k];// back up for the decryption process (cbc mode requires this)
            }}}

    long int time_taken = time(NULL);
    for (int i = 0; i < number_of_processes; ++i) {
        for (int j = 14; j > 0; --j) {  //14 rounds  starting with key addition layer
            for (int k = 0; k < 4; ++k) {
                for (int l = 0; l < 4; ++l) {
                    data_to_encrypt[i][k][l]^= round_key_array[(k + 4*j)][l]; //Kn round 1 ---> k1
                }}

            if (j != 14){ In_Mix_Col_process(i);} // inverse Mix layer

            In_Shift_Row_process(i); // inverse shift layer

            In_Sub_process(i); // Inverse Substitution layer

            if (j == 1){ // adding K0
                for (int k = 0; k < 4; ++k) {
                    for (int l = 0; l < 4; ++l) {
                        data_to_encrypt[i][k][l] ^= round_key_array[k][l];
                    }}}

        }
        if(i == 0){
            int count = 0;
            for (int j = 0; j < 4; ++j) {
                for (int k = 0; k < 4; ++k) {// adding Initialization Vector to the first box i = 0 (CBC mode)
                    data_to_encrypt[i][j][k] ^= Initialization_vector[count];
                    count++;
                }}count=0;}

        else if (i > 0){ // adding Yi-1 + Yi
            for (int j = 0; j < 4; ++j) {
                for (int k = 0; k < 4; ++k) {
                    data_to_encrypt[i][j][k] ^= back_up[i-1][j][k]; // back_ up to get last cipher text
                }}}

        int percentage;
        percentage = (i*100/number_of_processes);
        for (int nl = 0; nl < 4; ++nl) {       // displaying the data in stdout file
            printf("\n");}
        printf("\033[0;34m"); //adding blue color to the loading bar
        printf("[");
        for (int j = 0; j < percentage; ++j) {
            printf("#");}
        for (int j = 0; j < 100 - percentage; ++j) {
            printf(" ");}
        printf("]");
        printf("%%%d\n",(int)(((double )i/(double )number_of_processes)*100.));
        printf("\033[0m");

    }

    time_taken = time(NULL) - time_taken ; // time calculated
    return (double) time_taken;
}

void Sub_process(int i ){
    int value; // sub layer
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                value = data_to_encrypt[i][j][k]; // value >>4 to get second byte, value&0x0f first byte
                // using the first box which has inverse values for all hex values in p(X) mod
                data_to_encrypt[i][j][k] = Sub_Box[value>>4][value&0x0f];
}}}

void In_Sub_process(int i){
    int value; // inverse  sub process from inverse sub box
    for (int j = 0; j < 4; ++j) {
        for (int k = 0; k < 4; ++k) {
            value = data_to_encrypt[i][j][k]; // inverse of teh sub_box to get data back
            data_to_encrypt[i][j][k] = In_Sub_Box[value>>4][value&0x0f];
        }}}

void Shift_Row_process(int i){
        // shift Layer
        // no shift for first row
        int value;
        printf("\n"); //for row 2 three positions right shift
        value = data_to_encrypt[i][1][1];
        data_to_encrypt[i][1][1] = data_to_encrypt[i][1][2];
        data_to_encrypt[i][1][2] = data_to_encrypt[i][1][3];
        data_to_encrypt[i][1][3] = data_to_encrypt[i][1][0];
        data_to_encrypt[i][1][0] = value;

        int value1,value2; // for row 3 two positions right shift
        value1 = data_to_encrypt[i][2][0];
        value2 = data_to_encrypt[i][2][1];
        data_to_encrypt[i][2][0] = data_to_encrypt[i][2][2];
        data_to_encrypt[i][2][1] = data_to_encrypt[i][2][3];
        data_to_encrypt[i][2][2] = value1;
        data_to_encrypt[i][2][3] = value2;

        int value3; // for row 4 one position right shift
        value3 = data_to_encrypt[i][3][3];
        data_to_encrypt[i][3][3] = data_to_encrypt[i][3][2];
        data_to_encrypt[i][3][2] = data_to_encrypt[i][3][1];
        data_to_encrypt[i][3][1] = data_to_encrypt[i][3][0];
        data_to_encrypt[i][3][0] =value3;


}

void In_Shift_Row_process(int i){
    // inverse Shift layer
    // no shift for first row
    int value;// for row 2 three positions left shift
    printf("\n");
    value = data_to_encrypt[i][1][3];
    data_to_encrypt[i][1][3] = data_to_encrypt[i][1][2];
    data_to_encrypt[i][1][2] = data_to_encrypt[i][1][1];
    data_to_encrypt[i][1][1] = data_to_encrypt[i][1][0];
    data_to_encrypt[i][1][0] = value;

    int value1,value2; // for row 3 two positions left shift, right and left are same since shift is 2 steps
    value1 = data_to_encrypt[i][2][0];
    value2 = data_to_encrypt[i][2][1];
    data_to_encrypt[i][2][0] = data_to_encrypt[i][2][2];
    data_to_encrypt[i][2][1] = data_to_encrypt[i][2][3];
    data_to_encrypt[i][2][2] = value1;
    data_to_encrypt[i][2][3] = value2;

    int value3; // for row 4 one position left shift
    value3 = data_to_encrypt[i][3][0];
    data_to_encrypt[i][3][0] = data_to_encrypt[i][3][1];
    data_to_encrypt[i][3][1] = data_to_encrypt[i][3][2];
    data_to_encrypt[i][3][2] = data_to_encrypt[i][3][3];
    data_to_encrypt[i][3][3] = value3;

}

void Mix_Col_process(int i){
    //multiplication and addition are in Galois Field
    // using a specific matrix m_GF8_Box[i][j]
    // matrix multiplication
    int storage_data[4][4]={0x0},m1,m2,m3,m4;
    printf("\n");
    for (int j = 0; j < 4; ++j) {
        for (int k = 0; k < 4; ++k) {
            m1 = GF_multiplication_process(m_GF8_Box[k][0],data_to_encrypt[i][0][j]);
            m2 = GF_multiplication_process(m_GF8_Box[k][1],data_to_encrypt[i][1][j]);
            m3 = GF_multiplication_process(m_GF8_Box[k][2],data_to_encrypt[i][2][j]);
            m4 = GF_multiplication_process(m_GF8_Box[k][3],data_to_encrypt[i][3][j]);
            storage_data[j][k] = GF_addition(m1, m2, m3, m4);
        }}
    printf("\n");
    for (int f = 0; f < 4; ++f) {
        for (int g = 0; g < 4; ++g) {
            data_to_encrypt[i][f][g] = storage_data[f][g];
        }
    }}

void In_Mix_Col_process(int i){
    // similar procedures to mix cols layer
    //but with different box
    int storage_data_i[4][4]={0x0},m1,m2,m3,m4;
    printf("\n");
    for (int j = 0; j < 4; ++j) {
        for (int k = 0; k < 4; ++k) {
            m1 = GF_multiplication_process(in_m_GF8_Box[k][0],data_to_encrypt[i][j][0]);
            m2 = GF_multiplication_process(in_m_GF8_Box[k][1],data_to_encrypt[i][j][1]);
            m3 = GF_multiplication_process(in_m_GF8_Box[k][2],data_to_encrypt[i][j][2]);
            m4 = GF_multiplication_process(in_m_GF8_Box[k][3],data_to_encrypt[i][j][3]);
            storage_data_i[j][k] = GF_addition(m1, m2, m3, m4);
        }}
    printf("\n");
    for (int f = 0; f < 4; ++f) {
        for (int g = 0; g < 4; ++g) {
            data_to_encrypt[i][f][g] = storage_data_i[g][f];
        }}}
