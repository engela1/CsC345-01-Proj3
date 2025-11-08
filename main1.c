/* Anna Engel, Michael McNaughton, Project 3 */
/* main1.c : Virtual Memory Manager with no page replacement */

/* This program will read a file containing several 32-bit integer numbers that represent
logical addresses. However, we are only concerned with 16-bit addresses, so we
must mask the rightmost 16 bits of each logical address. */

int* addr[]; //array of logical addresses
int numPages; //number of pages in address.txt

int getPageNumAndOffset(){
    int rightBits[numPages];
    int pageNum[numPages];
    int offset[numPages];
    
    for (int i = 0; i < numPages; ++i){
        rightBits[i] = addr[i] & 0xFFFF; //use bit-masking to get rightmost 16 bits
        pageNum[i] = rightBits[i] >> 8; //get page numbers
        offset[i] = rightBits[i] & 0xFF; //get offset
    }
}

int translate(){
    //get page number and offset from logical address
    getPageNumAndOffset();
    //consult TLB
    return address;
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
