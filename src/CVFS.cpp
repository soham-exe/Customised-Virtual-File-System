/////////////////////////////////////////////////////////////////////
//
//  Header File Inclusion
//
/////////////////////////////////////////////////////////////////////

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>

/////////////////////////////////////////////////////////////////////
//
//  User Defined Macros
//
/////////////////////////////////////////////////////////////////////

// Maximum file size that we allow in the project
#define MAXFILESIZE 50

#define MAXOPENFILES 20

#define MAXINODE 5

#define READ 1
#define WRITE 2
#define EXECUTE 4

#define START 0
#define CURRENT 1
#define END 2

#define EXECUTE_SUCCESS 0

#define REGULARFILE 1
#define SPECIALFILE 2

/////////////////////////////////////////////////////////////////////
//
//  User Defined Macros for error handling
//
/////////////////////////////////////////////////////////////////////

#define ERR_INVALID_PARAMETER -1

#define ERR_NO_INODES -2

#define ERR_FILE_ALREADY_EXIST -3
#define ERR_FILE_NOT_EXIST -4
#define ERR_FILE_NOT_OPEN -9

#define ERR_PERMISSION_DENIED -5

#define ERR_INSUFFICIENT_SPACE -6
#define ERR_INSUFFICIENT_DATA -7

#define ERR_MAX_FILES_OPEN -8

/////////////////////////////////////////////////////////////////////
//
//  User Defined Structures
//
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
//
//  Structure Name :    BootBlock
//  Description :       Holds the information to boot the OS
//
/////////////////////////////////////////////////////////////////////

struct BootBlock
{
    char Information[100];
};

/////////////////////////////////////////////////////////////////////
//
//  Structure Name :    SuperBlock
//  Description :       Holds the information about the file system
//
/////////////////////////////////////////////////////////////////////

struct SuperBlock
{
    int TotalInodes;
    int FreeInodes;
};

/////////////////////////////////////////////////////////////////////
//
//  Structure Name :    Inode
//  Description :       Holds the information about file
//
/////////////////////////////////////////////////////////////////////

#pragma pack(1)
struct Inode
{
    char FileName[20];
    int InodeNumber;
    int FileSize;
    int ActualFileSize;
    int FileType;
    int ReferenceCount;
    int Permission;
    char *Buffer;
    struct Inode *next;
};

typedef struct Inode INODE;
typedef struct Inode * PINODE;
typedef struct Inode ** PPINODE;

/////////////////////////////////////////////////////////////////////
//
//  Structure Name :    FileTable
//  Description :       Holds the information about opened file
//
/////////////////////////////////////////////////////////////////////

struct FileTable
{
    int ReadOffset;
    int WriteOffset;
    int Mode;
    PINODE ptrinode;
};

typedef FileTable FILETABLE;
typedef FileTable * PFILETABLE;

/////////////////////////////////////////////////////////////////////
//
//  Structure Name :    UAREA
//  Description :       Holds the information about process file
//
/////////////////////////////////////////////////////////////////////

struct UAREA
{
    char ProcessName[20];
    PFILETABLE UFDT[MAXOPENFILES];
};

/////////////////////////////////////////////////////////////////////
//
//  Global variables or objects used in the project
//
/////////////////////////////////////////////////////////////////////

BootBlock bootobj;
SuperBlock superobj;
UAREA uareaobj;

PINODE head = NULL;

/////////////////////////////////////////////////////////////////////
//
//  Function Name :     InitialiseUAREA
//  Description :       It is used to initialise UAREA members
//  Author :            SOHAM SACHIN SONAR
//  Date :              13/01/2026
//
/////////////////////////////////////////////////////////////////////

void InitialiseUAREA()
{
   strcpy(uareaobj.ProcessName,"Myexe");
   
   int i = 0;

   for(i = 0; i < MAXOPENFILES; i++)
   {
        uareaobj.UFDT[i] = NULL;
   }
    printf("CVFS : UAREA gets initialised succesfully\n");
}

/////////////////////////////////////////////////////////////////////
//
//  Function Name :     InitialiseSuperBlock
//  Description :       It is used to initialise Super block members
//  Author :            SOHAM SACHIN SONAR
//  Date :              13/01/2026
//
/////////////////////////////////////////////////////////////////////

void InitialiseSuperBlock()
{
    superobj.TotalInodes = MAXINODE;
    superobj.FreeInodes = MAXINODE;

    printf("CVFS : Super block gets initialised succesfully\n");
}

/////////////////////////////////////////////////////////////////////
//
//  Function Name :     CreateDILB
//  Description :       It is used to create Linkedlist of inodes
//  Author :            SOHAM SACHIN SONAR
//  Date :              13/01/2026
//
/////////////////////////////////////////////////////////////////////

void CreateDILB()
{
    int i = 1;
    PINODE newn = NULL;
    PINODE temp = head;

    for(i = 1; i <= MAXINODE; i++)
    {
        newn = (PINODE)malloc(sizeof(INODE));

        strcpy(newn->FileName,"\0");
        newn->InodeNumber = i;
        newn->FileSize = 0;
        newn->ActualFileSize = 0;
        newn->FileType = 0;
        newn->ReferenceCount = 0;
        newn->Permission = 0;
        newn->Buffer = NULL;
        newn->next = NULL;

        if(temp == NULL)    // LL is empty
        {
            head = newn;
            temp = head;
        }
        else                // LL contains atleast 1 node
        {
            temp->next = newn;
            temp = temp->next;
        }
    }

    printf("CVFS : DILB created succesfully\n");
}

///////////////////////////////////////////////////////////////////////////
//
//  Function Name :     StartAuxillaryDataInitilisation
//  Description :       It is used to call all such functions which are
//                      used to initialise auxillary data
//  Author :            SOHAM SACHIN SONAR
//  Date :              13/01/2026
//
///////////////////////////////////////////////////////////////////////////

void StartAuxillaryDataInitilisation()
{
    strcpy(bootobj.Information,"Booting process of CVFS is done");

    printf("%s\n",bootobj.Information);

    InitialiseSuperBlock();

    CreateDILB();

    InitialiseUAREA();

    printf("CVFS : Auxillary data initialised succesfully\n");
}

/////////////////////////////////////////////////////////////////////
//
//  Function Name :     DisplayHelp
//  Description :       It is used to display the help page
//  Author :            SOHAM SACHIN SONAR
//  Date :              14/01/2026
//
/////////////////////////////////////////////////////////////////////

void DisplayHelp()
{
    printf("-----------------------------------------------\n");
    printf("---------- CVFS Help Page ----------\n");
    printf("-----------------------------------------------\n");

    printf("help   : It is used to display this help page\n");
    printf("man    : It is used to display manual page\n");
    printf("clear  : It is used to clear the terminal\n");
    printf("creat  : It is used to create new file\n");
    printf("write  : It is used to write the data into file\n");
    printf("read   : It is used to read the data from the file\n");
    printf("lseek  : It is used to reposition read/write file offset\n");
    printf("stat   : It is used to display statistical information using file name\n");
    printf("fstat   : It is used to display statistical information using file descriptor\n");
    printf("ls     : It is used to list all files\n");
    printf("unlink : It is used to delete the file\n");
    printf("exit   : It is used to terminate CVFS\n");
    
    printf("-----------------------------------------------\n");

}

/////////////////////////////////////////////////////////////////////
//
//  Function Name :     ManPageDisplay
//  Description :       It is used to display man page
//  Author :            SOHAM SACHIN SONAR
//  Date :              14/01/2026
//
/////////////////////////////////////////////////////////////////////

void ManPageDisplay(char *Name)
{
    FILE *fp;
    char line[256];
    char searchString[50];
    int found = 0;

    // Construct the search string, e.g., "COMMAND=ls"
    sprintf(searchString, "COMMAND=%s\n", Name);

    fp = fopen("Man_Page_Docs\\man_pages.txt", "r");
    if (fp == NULL)
    {
        printf("Error: Unable to open manual page file.\n");
        return;
    }

    // Read the file line by line
    while (fgets(line, sizeof(line), fp))
    {
        // Check if we found the command header
        if (strcmp(line, searchString) == 0)
        {
            found = 1;
            // Print everything until we hit "END"
            while (fgets(line, sizeof(line), fp))
            {
                if (strcmp(line, "END\n") == 0)
                {
                    break;
                }
                printf("%s", line);
            }
            break; // Stop searching after printing
        }
    }

    if (!found)
    {
        printf("No manual entry for %s\n", Name);
    }

    fclose(fp);
}

/////////////////////////////////////////////////////////////////////
//
//  Function Name :     IsFileExist
//  Description :       It is used to check whether file is already exist or not
//  Input :             It accepts file name
//  Output :            It returns the true or false
//  Author :            SOHAM SACHIN SONAR
//  Date :              16/01/2026
//
/////////////////////////////////////////////////////////////////////

bool IsFileExist(
                    char *name      // File name
                )
{
    PINODE temp = head;
    bool bFlag = false;

    while(temp != NULL)
    {
        if((strcmp(name,temp->FileName) == 0) && (temp->FileType == REGULARFILE))
        {
            bFlag = true;
            break;
        }
        temp = temp->next;
    }
    
    return bFlag;
}

/////////////////////////////////////////////////////////////////////
//
//  Function Name :     CreateFile
//  Description :       It is used to create new regular file
//  Input :             It accepts file name and permissions
//  Output :            It returns the file descriptor
//  Author :            SOHAM SACHIN SONAR
//  Date :              16/01/2026
//
/////////////////////////////////////////////////////////////////////

int CreateFile(
                    char *name,         // Name of new file
                    int permission      // Permission for that file
                )
{
    PINODE temp = head;
    int i = 0;

    printf("Total number of Inodes remaining : %d\n",superobj.FreeInodes);

    // If name is missing
    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    // If the permission value is wrong
    // permission -> 1 -> READ
    // permission -> 2 -> WRITE
    // permission -> 3 -> READ + WRITE
    if(permission < 1 || permission > 3)
    {
        return ERR_INVALID_PARAMETER;
    }

    // If the inodes are full
    if(superobj.FreeInodes == 0)
    {
        return ERR_NO_INODES;
    }

    // If file is already present
    if(IsFileExist(name) == true)
    {
        return ERR_FILE_ALREADY_EXIST;
    }

    // Search empty Inode
    while(temp != NULL)
    {
        if(temp -> FileType == 0)
        {
            break;    
        }
        temp = temp -> next;
    }
    
    if(temp == NULL)
    {
        printf("There is no inode\n");
        return ERR_NO_INODES;
    }

    // Search for empty UFDT entry
    // Note : 0,1,2 are reserved
    for(i = 3; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] == NULL)
        {
            break;
        }
    }

    // UFDT is full
    if(i == MAXOPENFILES)
    {
        return ERR_MAX_FILES_OPEN;
    }

    // Allocate ememory for file table
    uareaobj.UFDT[i] = (PFILETABLE)malloc(sizeof(FILETABLE));

    // Initialise File table
    uareaobj.UFDT[i]->ReadOffset = 0;
    uareaobj.UFDT[i]->WriteOffset = 0;
    uareaobj.UFDT[i]->Mode = permission;
    
    // Connect File table with Inode
    uareaobj.UFDT[i]->ptrinode = temp;

    // Initialise elements of Inode
    strcpy(uareaobj.UFDT[i]->ptrinode->FileName,name);
    uareaobj.UFDT[i]->ptrinode->FileSize = MAXFILESIZE;
    uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;
    uareaobj.UFDT[i]->ptrinode->FileType = REGULARFILE;
    uareaobj.UFDT[i]->ptrinode->ReferenceCount = 1;
    uareaobj.UFDT[i]->ptrinode->Permission = permission;

    // Allocate ememory for files data
    uareaobj.UFDT[i]->ptrinode->Buffer = (char *)malloc(MAXFILESIZE);

    superobj.FreeInodes--;

    return i;   // File descriptor
}

/////////////////////////////////////////////////////////////////////
//
//  Function Name :     LsFile()
//  Description :       It is used to list all files
//  Input :             Nothing
//  Output :            Nothing
//  Author :            SOHAM SACHIN SONAR
//  Date :              16/01/2026
//
/////////////////////////////////////////////////////////////////////

// ls -l
void LsFile()
{
    PINODE temp = head;

    printf("-----------------------------------------------\n");
    printf("------ CVFS Files Information ------\n");
    printf("-----------------------------------------------\n");

    while(temp != NULL)
    {
        if(temp -> FileType != 0)
        {
            printf("%d\t%s\t%d\n",temp->InodeNumber,temp->FileName,temp->ActualFileSize);
        }
        
        temp = temp -> next;
    }
    
    printf("-----------------------------------------------\n");

}

/////////////////////////////////////////////////////////////////////
//
//  Function Name :     UnlinkFile()
//  Description :       It is used to delete the file
//  Input :             File name
//  Output :            Nothing
//  Author :            SOHAM SACHIN SONAR
//  Date :              22/01/2026
//
/////////////////////////////////////////////////////////////////////

int UnlinkFile(
                    char *name
              )
{
    int i = 0;

    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(IsFileExist(name) == false)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // Travel the UFDT
    for(i = 0; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] != NULL)
        {
            if(strcmp(uareaobj.UFDT[i]->ptrinode->FileName, name) == 0)
            {
                // Deallocate memory of Buffer
                free(uareaobj.UFDT[i]->ptrinode->Buffer);
                uareaobj.UFDT[i]->ptrinode->Buffer = NULL;

                // Reset all values of inode
                // Dont deallocate memmory of inode
                uareaobj.UFDT[i]->ptrinode->FileSize = 0;
                uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;
                uareaobj.UFDT[i]->ptrinode->FileType = 0;
                uareaobj.UFDT[i]->ptrinode->ReferenceCount = 0;
                uareaobj.UFDT[i]->ptrinode->Permission = 0;

                memset(uareaobj.UFDT[i]->ptrinode->FileName, '\0', sizeof(uareaobj.UFDT[i]->ptrinode->FileName));

                // Dealloacte memory of file table
                free(uareaobj.UFDT[i]);

                // Set NULL to UFDT
                uareaobj.UFDT[i] = NULL;

                // // Increment free inodes count
                superobj.FreeInodes++;

                break;  // IMP
            }   // End of if
        }       // End of if
    }           // End of for

    return EXECUTE_SUCCESS;

}               // End of function

/////////////////////////////////////////////////////////////////////
//
//  Function Name :     WriteFile()
//  Description :       It is used to write the data into the file
//  Input :             File descriptor
//                      Address of buffer which contains data
//                      Size of data that we want to write
//  Output :            Number of bytes succesfully written
//  Author :            SOHAM SACHIN SONAR
//  Date :              22/01/2026
//
/////////////////////////////////////////////////////////////////////

int WriteFile(
                    int fd,
                    char *data,
                    int size
            )
{
    printf("File descriptor : %d\n",fd);
    printf("Data that we want to write : %s\n",data);
    printf("Number of bytes that we want to write : %d\n",size);

    // Invalid FD
    if(fd < 0 || fd > MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    // FD points to NULL
    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // There is no permission to write
    if(uareaobj.UFDT[fd]->ptrinode->Permission < WRITE)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Insufficient space
    if((MAXFILESIZE - uareaobj.UFDT[fd]->WriteOffset) < size)
    {
        return ERR_INSUFFICIENT_SPACE;
    }

    // Write the data into the file
    strncpy(uareaobj.UFDT[fd]->ptrinode->Buffer + uareaobj.UFDT[fd]->WriteOffset, data, size);

    // Update the write offset
    uareaobj.UFDT[fd]->WriteOffset = uareaobj.UFDT[fd]->WriteOffset + size;

    // Update the actual file size
    uareaobj.UFDT[fd]->ptrinode->ActualFileSize = uareaobj.UFDT[fd]->ptrinode->ActualFileSize + size;

    return size;
}

/////////////////////////////////////////////////////////////////////
//
//  Function Name :     ReadFile()
//  Description :       It is used to read the data from the file
//  Input :             File descriptor
//                      Address of empty buffer
//                      Size of data that we want to read
//  Output :            Number of bytes succesfully read
//  Author :            SOHAM SACHIN SONAR
//  Date :              22/01/2026
//
/////////////////////////////////////////////////////////////////////

int ReadFile(
                int fd,
                char *data,
                int size
            )
{

    //  Invaid FD
    if(fd < 0 || fd > MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(data == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(size <= 0)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // Filter for permission
    if(uareaobj.UFDT[fd]->ptrinode->Permission < READ)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Insuuficeint data
    if((MAXFILESIZE - uareaobj.UFDT[fd]->ReadOffset) < size)
    {
        return ERR_INSUFFICIENT_DATA;
    }

    // Read the data
    strncpy(data,uareaobj.UFDT[fd]->ptrinode->Buffer + uareaobj.UFDT[fd]->ReadOffset, size);

    // Update the read offset
    uareaobj.UFDT[fd]->ReadOffset = uareaobj.UFDT[fd]->ReadOffset + size;

    return size;
}

/////////////////////////////////////////////////////////////////////
//
//  Function Name :     StatFile
//  Description :       Display file information using EXISTING struct
//  Input :             File name
//  Output :            0 for success, negative for error
//  Author :            SOHAM SACHIN SONAR
//
/////////////////////////////////////////////////////////////////////

int StatFile(char *name)
{
    PINODE temp = head;
    
    if(name == NULL) return ERR_INVALID_PARAMETER;

    // Reuse your existing logic to find the inode
    while(temp != NULL)
    {
        if(strcmp(name, temp->FileName) == 0) break;
        temp = temp->next;
    }

    if(temp == NULL) return ERR_FILE_NOT_EXIST; // File not found

    printf("\n--------- File Statistics ----------\n");
    printf("File Name      : %s\n", temp->FileName);
    printf("Inode Number   : %d\n", temp->InodeNumber);
    printf("File Size      : %d bytes (Data written)\n", temp->ActualFileSize);
    printf("Size Limit     : %d bytes\n", temp->FileSize); // Max capacity
    printf("Link Count     : %d\n", temp->ReferenceCount);
    
    // Logic to translate int Permission to readable text
    printf("Permissions    : ");
    if(temp->Permission == 1)      printf("Read Only\n");
    else if(temp->Permission == 2) printf("Write Only\n");
    else if(temp->Permission == 3) printf("Read & Write\n");
    else                           printf("Unknown\n");

    printf("File Type      : %s\n", (temp->FileType == 1) ? "Regular File" : "Special File");
    printf("------------------------------------\n");

    return 0;
}

/////////////////////////////////////////////////////////////////////
//
//  Function Name :     FstatFile
//  Description :       Same as Stat, but uses File Descriptor (FD)
//  Input :             File descriptor
//  Output :            0 for success, negative for error
//  Author :            SOHAM SACHIN SONAR
//
/////////////////////////////////////////////////////////////////////

int FstatFile(int fd)
{
    PINODE temp = NULL;

    if(fd < 0 || fd > MAXOPENFILES) return ERR_INVALID_PARAMETER;
    
    if(uareaobj.UFDT[fd] == NULL) return ERR_FILE_NOT_OPEN;

    temp = uareaobj.UFDT[fd]->ptrinode;

    printf("\n--------- File Statistics (via FD) ----------\n");
    printf("File Name      : %s\n", temp->FileName);
    printf("Inode Number   : %d\n", temp->InodeNumber);
    printf("File Size      : %d bytes\n", temp->ActualFileSize);
    printf("Link Count     : %d\n", temp->ReferenceCount);
    
    printf("Permissions    : ");
    if(temp->Permission == 1)      printf("Read Only\n");
    else if(temp->Permission == 2) printf("Write Only\n");
    else if(temp->Permission == 3) printf("Read & Write\n");
    
    printf("---------------------------------------------\n");

    return 0;
}


/////////////////////////////////////////////////////////////////////
//
//  Function Name :     LseekFile
//  Description :       Reposition read/write file offset
//
/////////////////////////////////////////////////////////////////////

#define START 0
#define CURRENT 1
#define END 2

int LseekFile(int fd, int size, int from)
{
    // Sanity Checks
    if((fd < 0) || (fd >= MAXOPENFILES)) return ERR_INVALID_PARAMETER; // Invalid FD range
    if(from > 2) return ERR_INVALID_PARAMETER;                         // Invalid whence mode
    if(uareaobj.UFDT[fd] == NULL) return ERR_FILE_NOT_OPEN;        // FD is not open

    // --- CASE 1: Mode is READ or READ+WRITE ---
    if((uareaobj.UFDT[fd]->Mode == READ) || (uareaobj.UFDT[fd]->Mode == READ+WRITE))
    {
        if(from == START)
        {
            if(size > (uareaobj.UFDT[fd]->ptrinode->ActualFileSize)) return -1;
            if(size < 0) return -1;
            (uareaobj.UFDT[fd]->ReadOffset) = size;
        }
        else if(from == CURRENT)
        {
            if(((uareaobj.UFDT[fd]->ReadOffset) + size) > uareaobj.UFDT[fd]->ptrinode->ActualFileSize) return -1;
            if(((uareaobj.UFDT[fd]->ReadOffset) + size) < 0) return -1;
            (uareaobj.UFDT[fd]->ReadOffset) = (uareaobj.UFDT[fd]->ReadOffset) + size;
        }
        else if(from == END)
        {
            if((uareaobj.UFDT[fd]->ptrinode->ActualFileSize) + size > MAXFILESIZE) return -1;
            if(((uareaobj.UFDT[fd]->ReadOffset) + size) < 0) return -1;
            (uareaobj.UFDT[fd]->ReadOffset) = (uareaobj.UFDT[fd]->ptrinode->ActualFileSize) + size;
        }
    }
    // --- CASE 2: Mode is WRITE ---
    else if(uareaobj.UFDT[fd]->Mode == WRITE)
    {
        if(from == START)
        {
            if(size > MAXFILESIZE) return -1;
            if(size < 0) return -1;
            if(size > (uareaobj.UFDT[fd]->ptrinode->ActualFileSize))
                (uareaobj.UFDT[fd]->ptrinode->ActualFileSize) = size;
            (uareaobj.UFDT[fd]->WriteOffset) = size;
        }
        else if(from == CURRENT)
        {
            if(((uareaobj.UFDT[fd]->WriteOffset) + size) > MAXFILESIZE) return -1;
            if(((uareaobj.UFDT[fd]->WriteOffset) + size) < 0) return -1;
            if(((uareaobj.UFDT[fd]->WriteOffset) + size) > (uareaobj.UFDT[fd]->ptrinode->ActualFileSize))
                (uareaobj.UFDT[fd]->ptrinode->ActualFileSize) = (uareaobj.UFDT[fd]->WriteOffset) + size;
            (uareaobj.UFDT[fd]->WriteOffset) = (uareaobj.UFDT[fd]->WriteOffset) + size;
        }
        else if(from == END)
        {
            if((uareaobj.UFDT[fd]->ptrinode->ActualFileSize) + size > MAXFILESIZE) return -1;
            if(((uareaobj.UFDT[fd]->WriteOffset) + size) < 0) return -1;
            (uareaobj.UFDT[fd]->WriteOffset) = (uareaobj.UFDT[fd]->ptrinode->ActualFileSize) + size;
        }
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////
//
//  Entry Point function of the project
//
/////////////////////////////////////////////////////////////////////

int main()
{
    char str[80] = {'\0'};
    char Command[5][20] = {{'\0'}};
    char InputBuffer[MAXFILESIZE] = {'\0'};

    char *EmptyBuffer = NULL;

    int iCount = 0;
    int iRet = 0;

    StartAuxillaryDataInitilisation();

    printf("-----------------------------------------------\n");
    printf("----- CVFS started succesfully -----\n");
    printf("-----------------------------------------------\n");
    
    // Infinite Listening Shell
    while(1)
    {
        fflush(stdin);

        strcpy(str,"");

        printf("Soham's\\ CVFS : > ");
        fgets(str,sizeof(str),stdin);
        
        iCount = sscanf(str,"%s %s %s %s %s",Command[0],Command[1],Command[2],Command[3], Command[4]);

        fflush(stdin);

        if(iCount == 1)
        {
            // CVFS : > exit
            if(strcmp("exit",Command[0]) == 0)
            {
                printf("Thank you for using CVFS\n");
                printf("Deallocating all the allocated resources\n");

                break;
            }
            // CVFS : > ls
            else if(strcmp("ls",Command[0]) == 0)
            {
                LsFile();
            }
            // CVFS : > help
            else if(strcmp("help",Command[0]) == 0)
            {
                DisplayHelp();
            }
            // CVFS : > clear
            else if(strcmp("clear",Command[0]) == 0)
            {
                #ifdef _WIN32
                    system("cls");
                #else
                    system("clear");
                #endif
            }
            else
            {
                printf("There is no such command\n");
                printf("Please type 'help' to see all available commands\n");
            }
        } // End of else if 1
        else if(iCount == 2)
        {
            // CVFS : > man ls
            if(strcmp("man",Command[0]) == 0)
            {
                ManPageDisplay(Command[1]);
            }
            // CVFS : > unlink Demo.txt
            else if(strcmp("unlink",Command[0]) == 0)
            {
                iRet = UnlinkFile(Command[1]);
            
                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameter\n");
                }

                if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : Unable to delete as there is no such file\n");
                }

                if(iRet == EXECUTE_SUCCESS)
                {
                    printf("File gets succesfully deleted\n");
                }
            }
            // CVFS : > write 2
            else if(strcmp("write",Command[0]) == 0)
            {
                printf("Enter the data that you want to write : \n");
                fgets(InputBuffer,MAXFILESIZE,stdin);

                iRet = WriteFile(atoi(Command[1]), InputBuffer, strlen(InputBuffer)-1);
            
                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameters \n");
                }
                else if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : There is no such file\n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : Unable to write as there is no permission\n");
                }
                else if(iRet == ERR_INSUFFICIENT_SPACE)
                {
                    printf("Error : Unable to write as there is no space\n");
                }
                else
                {
                    printf("%d bytes gets succesfully written\n",iRet);
                }
            }
            // CVFS : > stat Demo.txt
            else if(strcmp("stat", Command[0]) == 0)
            {
                iRet = StatFile(Command[1]);
                if(iRet == ERR_INVALID_PARAMETER) printf("Error : Incorrect parameters\n");
                if(iRet == ERR_FILE_NOT_EXIST) printf("Error : File not found\n");
            }
            // CVFS : > fstat 2
            else if(strcmp("fstat", Command[0]) == 0)
            {
                // Convert string argument to integer for FD
                iRet = FstatFile(atoi(Command[1]));
                if(iRet == ERR_INVALID_PARAMETER) printf("Error : Invalid File Descriptor\n");
                if(iRet == ERR_FILE_NOT_OPEN) printf("Error : File Descriptor is not open\n");
            }
            else
            {
                printf("There is no such command\n");
                printf("Please type 'help' to see all available commands\n");
            }
        } // End of else if 2
        else if(iCount == 3)
        {
            // CVFS : > creat Ganesh.txt 3
            if(strcmp("creat",Command[0]) == 0)
            {
                iRet = CreateFile(Command[1],atoi(Command[2]));

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Unable to create the file as parameters are invalid\n");
                    printf("Please refer man page\n");
                }

                if(iRet == ERR_NO_INODES)
                {
                    printf("Error : Unable to create file as there is no inode\n");
                }

                if(iRet == ERR_FILE_ALREADY_EXIST)
                {
                    printf("Error : Unable to create file because the file is already present\n");
                }

                if(iRet == ERR_MAX_FILES_OPEN)
                {
                    printf("Error : Unable to create file\n");
                    printf("Max opened files limit reached\n");
                }

                printf("File gets succesfully created with FD %d\n",iRet);
            }
            // CVFS : > read 3 10
            else if(strcmp("read", Command[0]) == 0)
            {
                int readSize = atoi(Command[2]);
                
                // 1. Allocate correct size (+1 for null terminator)
                EmptyBuffer = (char *)malloc(readSize + 1); 

                // Safety check if malloc failed
                if (EmptyBuffer == NULL) {
                    printf("Error: Memory allocation failed\n");
                    return -1; // or break
                }

                // 2. Read the data
                iRet = ReadFile(atoi(Command[1]), EmptyBuffer, readSize);

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameter\n");
                    free(EmptyBuffer); // Don't forget to free on error!
                }
                else if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : File not exist\n");
                    free(EmptyBuffer);
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : Permission denied\n");
                    free(EmptyBuffer);
                }
                else if(iRet == ERR_INSUFFICIENT_DATA)
                {
                    printf("Error : Insufficient data\n");
                    free(EmptyBuffer);
                }
                else
                {
                    // 3. Add null terminator now
                    EmptyBuffer[iRet] = '\0'; 
                    
                    printf("Read operation is succesful\n");
                    printf("Data from file is : %s\n", EmptyBuffer);

                    free(EmptyBuffer);
                }
            }
            else
            {
                printf("There is no such command\n");
                printf("Please type 'help' to see all available commands\n");
            }
        } // End of else if 3
        else if(iCount == 4)
        {
            // Command Usage: lseek [FD] [Offset] [Whence]
            if(strcmp("lseek", Command[0]) == 0)
            {
                int fd = atoi(Command[1]);
                int offset = atoi(Command[2]);
                int whence = atoi(Command[3]);

                iRet = LseekFile(fd, offset, whence);

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Unable to perform lseek\n");
                }
                else if(iRet == ERR_FILE_NOT_OPEN)
                {
                    printf("Error : File is not open\n");
                }
                else
                {
                    printf("File offset successfully changed\n");
                }
            }
            else
            {
                printf("There is no such command\n");
                printf("Please type 'help' to see all available commands\n");
            }
            } // End of else if 4
        else
        {
            printf("There is no such command\n");
            printf("Please type 'help' to see all available commands\n");
        }// End of else
    } // End of while

    return 0;
} // End of main