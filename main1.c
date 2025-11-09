/* Anna Engel, Michael McNaughton, Project 3 */
/* main1.c : Virtual Memory Manager with no page replacement */

/* This program will read a file containing several 32-bit integer numbers that represent
logical addresses. However, we are only concerned with 16-bit addresses, so we
must mask the rightmost 16 bits of each logical address. */

#define TLB_SIZE 16
#define PT_SIZE 256
#define MAX_ADDR 10000
int addr[MAX_ADDR]; //array of logical addresses
int pageNum[MAX_ADDR];
int offset[MAX_ADDR];
int numPages = 0; //number of pages in address.txt
int numPageFaults = 0, numTLBhits = 0; //keep track of number of page faults and TLB hits
int FIFOcounter = 0;
unsigned char physMem[256 * 256];
int nextFree = 0;
FILE *backing = NULL;

typedef struct{ //struct for TLB entries
    unsigned int TLBpage;
    unsigned int TLBframe;
    int full; // 0: empty, 1: full
    int time; //used for FIFO -> lowest # was the first one into TLB
} TLBentry;

typedef struct{ //struct for page table entries
    unsigned int PTFrame;
    int full; // 0: empty, 1: full
} PTEntry;

TLBentry TLB[TLB_SIZE]; //create TLB with 16 entries
for (int i = 0; i < TLB_SIZE; ++i){
    TLB[i].full = 0; // set all entries to empty
}

PTEntry pageTable[PT_SIZE]; //create page table with 256 entries
for (int i = 0; i < PT_SIZE; ++i){
    pageTable[i].full = 0; // set all entries to empty
}


void getPageNumAndOffset(){
    for (int i = 0; i < numPages; ++i){
        int logical = addr[i] & 0xFFFF; //use bit-masking to get rightmost 16 bits
        pageNum[i] = logical >> 8; //get page numbers
        offset[i] = logical & 0xFF; //get offset
    }
}

/* returns frame# if hit, else returns -1 */
int checkTLB(int page){
    for (int i = 0; i < TLB_SIZE; ++i){
        if (TLB[i].full == 1 && TLB[i].TLBpage == page){ //if hit
            numTLBhits++; //increment counter
            return TLB[i].TLBframe; //return cooresponding frame
        }
    }
    return -1; //if no hit
}

/* update TLB using FIFO */
void updateTLB (int page, int frame){
    int fullCount = 0;
    //check if TLB is full
    for (int i = 0; i < TLB_SIZE; ++i){
        if (TLB[i].full == 1) ++fullCount;
    }

    //if TLB is not full -> use next available slot
    if (fullCount < TLB_SIZE){
        for (int i = 0; i < TLB_SIZE; ++i){
            if (TLB[i].full == 0){ //found empty spot
                //update TLB
                TLB[i].TLBpage = page;
                TLB[i].TLBframe = frame;
                TLB[i].full = 1;
                TLB[i].time = FIFOcounter;
                FIFOcounter++;
                return;
            }
        }
    }
    //if TLB is full -> use FIFO to replace
    int victim = 0;
    int min = TLB[0].time;
    for(int i = 1; i < TLB_SIZE; ++i){
        if (TLB[i].time < min){
            min = TLB[i].time;
            victim = i;
        }
    }
    
    //replace first in entry with new one
    TLB[victim].TLBpage = page;
    TLB[victim].TLBframe = frame;
    TLB[victim].full = 1;
    TLB[victim].time = FIFOcounter;
    FIFOcounter++;
}


/* Checks page table for page, returns frame if found, else returns -1 */
int checkPageTable(int page){
    if (pageTable[page].full == 0){
        numPageFaults++; //increment counter
        return -1; //page fault
    }
    else return pageTable[page].PTFrame; //frame found
}

void updatePageTable(int page, int frame){
    pageTable[page].PTFrame = frame;
    pageTable[page].full = 1;
}

/* needs to be finished */
int getBacking(int page){
    int fileOffset = page * 256;
    fseek(fp, fileOffest, SEEK_SET);

    // destination in physical memory: start of the free frame
    unsigned char *dest = &physMem[nextFree * 256];
    // read exactly 256 bytes into that frame
    size_t bytesRead = fread(dest, 1, 256, backing);

    // update page table: this page now lives in nextFreeFrame
    pageTable[page].PTFrame = nextFree;
    pageTable[page].full = 1;

    // remember which frame we used, then advance for next time
    int frameUsed = nextFree;
    nextFree++;

    return frameUsed;
}


int translate(){
    //get page number and offset from logical address
    getPageNumAndOffset();

    for (int i = 0; i < numPages; ++i){ //check each page number
        int frame = 0;
    
        //check TLB for page
        frame = checkTLB(pageNum[i]);
            //if TLB hit --> get frame 
            if (frame != -1){
                
            }
        
            //if TLB miss --> check page table
            else{
                //check page table
                frame = checkPageTable(pageNum[i]);
                //if page fault --> read page from BACKING_STORE.bin
                if (frame == -1){
                    //read page from BACKING_STORE.bin
                    frame = getBacking(pageNum[i]);
                    //update TLB
                    updateTLB(pageNum[i], frame);
                    //update page table
                    updatePageTable();
                }
                else{ //page hit
                    
                }
                
                //update TLB using FIFO
                updateTLB(pageNum[i], frame);
                
            }
                //update page table
                

    }
}

int main(int argc, char** argv){
    //read from a file containing logical addresses 
    FILE* fp = fopen("addresses.txt", "rt"); //open file
    int address;
    int numPages = 0;
    
    while (scanf("%d", &address) == 1){ //read addresses from file into array
        addr[numPages] = address;
        numPages++;
    }
    fclose(fp); //close addresses.txt
        
    //output logical addresses to out1.txt
    fp = fopen("out1.txt", "wt");
    for (int i = 0; i < numPages; ++i){
        fprintf(fp, "&d\n", );
    }
    fclose(fp); //close out1.txt

    //open backing file
    backing = fopen("BACKING_STORE.bin", "rb");

    translate(); //translate logical address

    //output physical addresses to out2.txt
    fp = fopen("out2.txt", "wt");
    for (int i = 0; i < numPages; ++i){
        fprintf(fp, "&d\n", );
    }
    fclose(fp); //close out2.txt

    //output signed byte value stored in physical memory to out3.txt
    fp = fopen("out3.txt", "wt");
    for (int i = 0; i < numPages; ++i){
        fprintf(fp, "&d\n", );
    }
    fclose(fp); //close out3.txt

    fclose(backing):

        
    return 0;

}



