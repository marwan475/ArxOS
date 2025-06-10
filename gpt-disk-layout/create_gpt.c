#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define EXIT_ERROR -1
#define EXIT_SUCCESS 0

#define SUCCESS 1
#define ERROR -1

const char* gpt_img_name = "twistedosgpt.img";

// UEFI spec 5.2.3
typedef struct {
  
  uint8_t BootIndicator;
  uint8_t StartingCHS[3];
  uint8_t OSType;
  uint8_t EndingCHS[3];
  uint32_t StartingLBA;
  uint32_t SizeInLBA;

} __attribute__ ((packed)) protective_mbr_partition;

typedef struct {

  uint8_t BootCode[440];
  uint32_t DiskSignature;
  uint16_t Unknown;
  protective_mbr_partition PartitionRecord[4];
  uint16_t Signature;

} __attribute__ ((packed)) protective_mbr

// For a bootable disk, a Protective MBR must be located at LBA 0
// UEFI spec 5.2.3
int write_protective_mbr(FILE *img){
  
  protective_mbr mbr = {
    .BootCode = {0},
    .DiskSignature = 0,
    .Unknown = 0,
  };

  if (fwrite(&mbr, 1, sizeof(mbr), img) != sizeof(mbr)){
    
    return ERROR;

  }

  return SUCCESS;
}

int main(){

  FILE *img = fopen(gpt_img_name, "wb+");

  if (!img){
    
    fprintf(stderr,"Error::create_gpt.c: could not open file %s\n",gpt_img_name);
    
    return EXIT_ERROR;

  }

  if (!write_protective_mbr(img)){

    fprintf(stderr,"Error::create_gpt.c: could not write protective mbr\n");

    return EXIT_ERROR;
  
  }

  return EXIT_SUCCESS;

}
