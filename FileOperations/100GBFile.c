



/*
    large file (100GB)
    
    10GB * 10
    
    2GB after compression
    
    fd = socket()
    
    100GB {
        buffer = 10GB
        = compression(1GB)
    }
        
    
*/

#define MAX_COMPRESSION_SIZE (1024*1024*1024)UL; //1GB

struct CompressedFileBlocks {
    int seqNum;
    int data[MAX_COMPRESSION_SIZE];    
    struct CompressedFileBlocks *next;
};

struct FileBlock {
    char *name; // large file
    struct CompressedFileBlocks *compHead;
    sock_t *fd;
};

int main() {
    char *file = "file1.txt";
    struct CompressedFileBlocks *compHead;
    
    struct FileBlock *fblock = malloc(size(struct FileBlock));
    fblock->name = file;
    
    
    int fd = open(file, O_RD);
    
    void *buffer = read(fd, 0, sizeFile); 
    int indexBuf = 0;
    
    while(indexBuf < sizeFile) {
        fblock->compHead->seqNum = indexBuf / (1024*1024);
        thread_create(threadfn, fblock->compHead);
    }
    
}