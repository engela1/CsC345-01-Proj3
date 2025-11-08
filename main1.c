/* Anna Engel, Michael McNaughton, Project 3 */
/* main1.c : Virtual Memory Manager with no page replacement */

/* This program will read a file containing several 32-bit integer numbers that represent
logical addresses. However, we are only concerned with 16-bit addresses, so we
must mask the rightmost 16 bits of each logical address. */

#define TLB_SIZE 16;
int* addr[]; //array of logical addresses
int numPages; //number of pages in address.txt
int pageNum[numPages]; 
int offset[numPages];
int pageTable[256] = {0}; //page table with 256 entries, all initialize to invalid
int numPageFaults = 0, numTLBhits = 0; //keep track of number of page faults and TLB hits

typedef struct{ //struct for TLB entries
    unsigned int TLBpage;
    unsigned int TLBframe;
    int full = 0; // 0: empty, 1: full
} TLBentry;

TLBentry TLB[TLB_SIZE]; //create TLB with 16 entries


int getPageNumAndOffset(){
    int rightBits[numPages];
    
    for (int i = 0; i < numPages; ++i){
        rightBits[i] = addr[i] & 0xFFFF; //use bit-masking to get rightmost 16 bits
        pageNum[i] = rightBits[i] >> 8; //get page numbers
        offset[i] = rightBits[i] & 0xFF; //get offset
    }
}

int checkTLB(int page){ //returns frame# if hit, else returns -1
    for (int i = 0; i < TLB_SIZE; ++i){
        if (TLB[i]->TLBpage == page){ //if hit
            return TLB[i]->TLBframe; //return cooresponding frame
        }
    }
    return -1; //if no hit
}

int translate(){
    //get page number and offset from logical address
    getPageNumAndOffset();

    for (int = i; i < numPages; ++i){ //check each page number
        //check TLB for page
        int frame = checkTLB(pageNum[i]);
            //if hit --> get frame 
            if (frame != -1){
                
            }
        
            //if miss --> check page table
            else{
                //if page fault --> read page from BACKING_STORE.bin
            }
                //update page table
                //update TLB using FIFO

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

