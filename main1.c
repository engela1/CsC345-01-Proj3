/* Anna Engel, Michael McNaughton, Project 3 */
/* main1.c : Virtual Memory Manager with no page replacement */

/* This program will read a file containing several 32-bit integer numbers that represent
logical addresses. However, we are only concerned with 16-bit addresses, so we
must mask the rightmost 16 bits of each logical address. */

#define TLB_SIZE 16;
#define PT_SIZE 256;
int* addr[]; //array of logical addresses
int numPages; //number of pages in address.txt
int pageNum[numPages]; 
int offset[numPages];
int numPageFaults = 0, numTLBhits = 0; //keep track of number of page faults and TLB hits
int FIFOcounter = 0;

typedef struct{ //struct for TLB entries
    unsigned int TLBpage;
    unsigned int TLBframe;
    int full = 0; // 0: empty, 1: full
    int time; //used for FIFO -> lowest # was the first one into TLB
} TLBentry;

typedef struct{ //struct for page table entries
    unsigned int PTFrame;
    int full = 0; // 0: empty, 1: full
} PTEntry;

TLBentry TLB[TLB_SIZE]; //create TLB with 16 entries
PTEntry pageTable[PT_SIZE]; //create page table with 256 entries


int getPageNumAndOffset(){
    int rightBits[numPages];
    
    for (int i = 0; i < numPages; ++i){
        rightBits[i] = addr[i] & 0xFFFF; //use bit-masking to get rightmost 16 bits
        pageNum[i] = rightBits[i] >> 8; //get page numbers
        offset[i] = rightBits[i] & 0xFF; //get offset
    }
}

/* returns frame# if hit, else returns -1 */
int checkTLB(int page){
    for (int i = 0; i < TLB_SIZE; ++i){
        if (TLB[i]->TLBpage == page){ //if hit
            ++numTLBhits; //increment counter
            return TLB[i]->TLBframe; //return cooresponding frame
        }
    }
    return -1; //if no hit
}

/* update TLB using FIFO */
void updateTLB (int page, int frame){
    int full = 0;
    //check if TLB is full
    for (int i = 0; i < TLB_SIZE; ++i){
        if (TLB[i] -> full == 1) ++full;
    }

    //if TLB is not full -> use next available slot
    if (full < TLB_SIZE){
        for (int i = 0; i < TLB_SIZE; ++i){
            if (TLB[i]->full == 0){ //found empty spot
                //update TLB
                TLB[i]->TLBpage = page;
                TLB[i]->TLBframe = frame;
                TLB[i]->full = 1;
                TLB[i]->time = FIFOcounter;
                ++FIFOcounter;
                break;
            }
        }
    }
    //if TLB is full -> use FIFO to replace
    if (full == TLB_SIZE){
        TLBentry firstIn;
        int min = TLB[0]->time;
        for(int i = 1; i < TLB_SIZE; ++i){
            if (TLB[i]->time < min){
                min = TLB[i]->time;
                firstIn = TLB[i];
            }
        }
        //replace first in entry with new one
        TLB[firstIn]->TLBpage = page;
        TLB[firstIn]->TLBframe = frame;
        TLB[firstIn]->full = 1;
        TLB[firstIn]->time = FIFOcounter;
        ++FIFOcounter;
    }
}

/* Checks page table for page, returns frame if found, else returns -1 */
int checkPageTable(int page){
    if (pageTable[page]-> full == 0){
        ++numPageFaults; //increment counter
        return -1; //page fault
    }
    else return pageTable[page] -> PTFrame; //frame found
}

/* needs to be finished */
int getBacking(int page){
    FILE* fp = fopen("BACKING_STORE.bin", "rb"); //open file in read binary mode
    int fileOffset = page * 256;
    seek(fileOffest);
}

int translate(){
    //get page number and offset from logical address
    getPageNumAndOffset();

    for (int = i; i < numPages; ++i){ //check each page number
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
                    getBacking();
                    
                    //update page table
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

    fclose(fp) //close file
        
    //output logical addresses to out1.txt
    fp = fopen("out1.txt", "wt");
    fprintf(fp, "&d\n", translate()); //translate logical address

    return 0;

}


