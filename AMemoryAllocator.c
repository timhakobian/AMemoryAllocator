// // // Xean Nguyenla ; ID#17133321
// // // Timothy Hakobian ; ID#56197426

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void memoryAllocator()

{

    struct byte{
        int isMalloc;
        int isHeader;
        int byteValue;
        int size;           // for header byte only
        int headerValue;    // for header byte only


    };

    struct freeBlock{
        int headerAddress;
        int size;

    };

    struct byte* Bytes = malloc(127 * (sizeof(struct byte)));

    int i = 0;
    while(i < 127){
        Bytes[i].isMalloc = 0;
        Bytes[i].isHeader = 0;
        Bytes[i].byteValue = 0;
        Bytes[i].size = 0;
        i++;
    }

    char response[80];

    int arraySize = 1;
    Bytes[0].isHeader = 2;
    Bytes[0].size = 126;

    Bytes[0].headerValue = Bytes[0].size + 1;
    Bytes[0].headerValue = Bytes[0].headerValue << 1;



    while(strncmp(response, "quit", 4) != 0)
    {

        printf("> ");
        fgets(response, 80, stdin);
        char *command = strtok(response, " ");
        char* secArg = strtok(NULL, " ");
        if (strncmp(command, "malloc", 6) == 0)
        {

            struct freeBlock* availLocs = malloc(127 * (sizeof(struct freeBlock)));
            int size = atoi(secArg);

            int byteIndex = 0;
            int freeCounter = 0;
            while (byteIndex < 127)
            {
                if (Bytes[byteIndex].isHeader == 2)
                {
                    if (Bytes[byteIndex].size >= size)
                    {

                        availLocs[freeCounter].headerAddress = byteIndex;
                        availLocs[freeCounter].size = Bytes[byteIndex].size;

                        freeCounter++;
                    }
                }
                byteIndex++;
            }


            if (freeCounter != 0)
            {

                int counter = 0;
                int freeAddress = availLocs[0].headerAddress;
                int freeSize = availLocs[0].size;

                while (counter < freeCounter)
                {
                    if (availLocs[counter].size < freeSize)
                    {
                        freeAddress = availLocs[counter].headerAddress;
                        freeSize = availLocs[counter].size;
                    }

                    counter++;
                }

                Bytes[freeAddress + size + 1].isHeader = 2;
                Bytes[freeAddress + size + 1].size = freeSize - size - 1;
                Bytes[freeAddress + size + 1].headerValue = Bytes[freeAddress + size + 1].size  + 1;
                Bytes[freeAddress + size + 1].headerValue = Bytes[freeAddress + size + 1].headerValue << 1;

                Bytes[freeAddress].isHeader = 1;
                Bytes[freeAddress].isMalloc = 1;
                Bytes[freeAddress].size = size;

                Bytes[freeAddress].headerValue = size + 1;
                Bytes[freeAddress].headerValue = Bytes[freeAddress].headerValue << 1;
                Bytes[freeAddress].headerValue++;

                printf("%d\n", freeAddress + 1);

                int startingSizeAddress = freeAddress + 1;
                int sizeIndex = 0;

                while(sizeIndex < size)
                {
                    Bytes[startingSizeAddress].isMalloc = 1;
                    startingSizeAddress++;
                    sizeIndex++;
                }

                arraySize++;


            }

        }
        else if (strncmp(command, "free", 4) == 0)
        {
            int address = atoi(secArg);
            int headerAddress = address - 1;

            Bytes[headerAddress].isHeader = 2;

            if (Bytes[headerAddress].isMalloc == 1)
            {
                Bytes[headerAddress].headerValue--;
            }

            Bytes[headerAddress].isMalloc = 0;

            int counter = 0;
            int startingAddress = address;

            while (counter < Bytes[headerAddress].size)
            {
                Bytes[startingAddress].isMalloc = 0;

                counter++;
                startingAddress++;
            }

            if (Bytes[startingAddress].isHeader == 2)
            {

                Bytes[headerAddress].size = Bytes[headerAddress].size + Bytes[startingAddress].size + 1;
                Bytes[headerAddress].headerValue = Bytes[headerAddress].size + 1;
                Bytes[headerAddress].headerValue = Bytes[headerAddress].headerValue << 1;

                Bytes[startingAddress].isHeader = 0;
                Bytes[startingAddress].size = 0;
                Bytes[startingAddress].headerValue = 0;
                arraySize--;
            }

        }

        else if (strncmp(command, "writemem", 8) == 0)
        {
            int address = atoi(secArg);
            char* word = strtok(NULL, " \n");
            int byteAddress = 0;
            while(byteAddress < 127){
                if(byteAddress == address)
                {
                    int charIndex = 0;
                    unsigned char asciiValue;
                    while(charIndex < strlen(word))
                    {
                        asciiValue = word[charIndex];

                        int byteValue = asciiValue;

                        Bytes[byteAddress].byteValue = byteValue;
                        charIndex++;
                        byteAddress++;
                    }
                    break;
                }
                byteAddress++;
            }
        }

        else if (strncmp(command, "printmem", 8) == 0)
        {
            int address = atoi(secArg);
            char* thirdArg = strtok(NULL, " \n");
            int numValues = atoi(thirdArg);
            int numIndex = 0;
            int startingAddress = address;
            while(numIndex < numValues)
            {
                if (Bytes[startingAddress].isHeader > 0)
                {
                    printf("%x ",Bytes[startingAddress].headerValue);
                }
                else
                {
                    printf("%x ",Bytes[startingAddress].byteValue);
                }

                startingAddress++;
                numIndex++;
            }

            printf("\n");
        }

        else if (strncmp(command, "blocklist", 8) == 0)
        {
            int byteIndex = 0;
            int numHeaders = 0;

            while (byteIndex < 127)
            {
                if (Bytes[byteIndex].isHeader > 0)
                {
                    char* allocOrFree;
                    if (Bytes[byteIndex].isHeader == 1)
                    {
                        allocOrFree = "allocated";
                    }
                    else
                    {
                        allocOrFree = "free";
                    }
                    printf("%d, %d, %s\n", byteIndex + 1, Bytes[byteIndex].size, allocOrFree);
                    numHeaders++;
                }
                byteIndex++;
            }
        }
    }
}

int main()
{
    memoryAllocator();
    return 0;

}