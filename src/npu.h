#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h> 
// for REST API
#include <curl.h>

CURLcode request(CURL *hCURL, FILE *image_fd);
CURL* curl_init();
void curl_end(CURL *hCURL);

#define MEM_BANK1 0x400000000
#define MEM_BANK2 0x400008000
#define MEM_BANK3 0x400010000 // Input

#define MEM_BANK4 0x400018000
#define MEM_BANK5 0x400020000
#define MEM_BANK6 0x400028000 // Filter

#define MEM_BANK7 0x400030000
#define MEM_BANK8 0x400038000 // PSum

#define Gemm_En 1 // address
#define MEM_RST 1 // address
#define MEM_RD 1  // address

// Memory R/W
#define MAP_SIZE 98304UL 			// Memory Paging Size 24KB
#define MAP_MASK (MAP_SIZE - 1) 	// Memory Address Mask
#define BLOCK_SIZE 24576



void quantize_8bits(float x[], float min, float max, int len) {
    uint8_t temp[len]; 
    
    int i;
    float scale = (max - min) / 255;
    uint8_t zero_point = 0 - round(min / scale);
    
    #pragma omp parallel for
    for (i = 0; i < len; i++) 
    {
        temp[i] = (uint8_t)round(x[i] / scale) + zero_point;
    }
    
    #pragma omp parallel for
    for (i = 0; i < len; i++) 
    {
        x[i] = (temp[i] - zero_point) * scale;
    }
    // dequantize
}

float compare(const void *a, const void *b)   
{
    float num1 = *(int *)a;    
    float num2 = *(int *)b;    
    if (num1 < num2)   
        return -1;     
    if (num1 > num2)    
        return 1;      
    return 0;    
}
/*

void read_mm(void *memory, int length, int c_offset, float *quantC)
{
    void* virt_addr;
    
    int l;
    off_t offset=0x00;
    float output;
    
    memcpy(quantC, memory, sizeof(int)*length);
}

void zero_init_mm(void *memory)
{
    int i;
    off_t offset=0x00;
    void* virt_addr;
    memcpy(memory, intbox, BLOCK_SIZE);
}

*/


//float read_SFR(int fd, off_t target);
//void write_SFR(int fd, off_t target, unsigned long value);
/*
float read_SFR(int fd, off_t target)
{
    void* map_base;
    void* virt_addr;
    float output;
    map_base = mmap(0, MAP_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fd, target & ~MAP_MASK);
    virt_addr = map_base + (target & MAP_MASK);
    output = *((float*)virt_addr);
    munmap(map_base, MAP_SIZE);
    return output;
}

void write_SFR(int fd, off_t target, unsigned long value)
{
    void* map_base;
    void* virt_addr;
    map_base = mmap(0, MAP_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fd, target & ~MAP_MASK);
    virt_addr = map_base + (target & MAP_MASK);
    *(volatile unsigned long*)virt_addr = value;
    munmap(map_base, MAP_SIZE);
}
*/
/*
//uint8_t read_SFR(int fd, off_t target)
{
    void* map_base;
    void* virt_addr;
    uint8_t output;
    map_base = mmap(0, MAP_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fd, target & ~MAP_MASK);
    virt_addr = map_base + (target & MAP_MASK);
    output = *((uint8_t*)virt_addr);
    munmap(map_base, MAP_SIZE);
    return output;
}

void write_SFR(int fd, off_t target, unsigned long value)
{
    void* map_base;
    void* virt_addr;
    map_base = mmap(0, MAP_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fd, target & ~MAP_MASK);
    virt_addr = map_base + (target & MAP_MASK);
    *(volatile unsigned long*)virt_addr = value;
    munmap(map_base, MAP_SIZE);
}
*/

/*
float* deQuantize_8bits(uint8_t x[], uint8_t min, uint8_t max, int len, float scale_factor, uint8_t zero_point) 
{
    int i;
    float temp[len];
    for(i=0;i<len;i++)
    {
        temp[i] = (x[i] - zero_point) * scale_factor;
    }
    return &temp;
}
*/
