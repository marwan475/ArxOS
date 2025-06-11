#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define EXIT_ERROR -1
#define EXIT_SUCCESS 0

#define SUCCESS 1
#define ERROR -1

const char* GPT_IMG_NAME = "twistedosgpt.img";
const uint64_t LBA_SIZE = 512;
const uint64_t ESP_SIZE = 33*1024*1024; // 33 MiB
const uint64_t DATA_SIZE = 1*1024*1024; // 1 MiB
uint64_t IMAGE_SIZE = 0;
uint64_t ESP_LBA, DATA_LBA, IMAGE_LBA;
const uint64_t PADDING = 1024*1024;


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

} __attribute__ ((packed)) protective_mbr;

uint64_t byte_to_LBA(uint64_t bytes){
  return (bytes / LBA_SIZE) + (bytes % LBA_SIZE > 0 ? 1 : 0);
}

// For a bootable disk, a Protective MBR must be located at LBA 0
// UEFI spec 5.2.3
int write_protective_mbr(FILE *img){
  
  if (IMAGE_LBA > 0xFFFFFFFF) IMAGE_LBA = 0x100000000;

  protective_mbr mbr = {
    .BootCode = {0},
    .DiskSignature = 0,
    .Unknown = 0,
    .PartitionRecord[0] = {
      .BootIndicator = 0,
      .StartingCHS = { 0X00, 0X02, 0X00 },
      .OSType = 0xEE, // protective gpt
      .EndingCHS = { 0xFF, 0xFF, 0xFF },
      .StartingLBA = 0x1,
      .SizeInLBA = IMAGE_LBA - 1,
    },
    .Signature = 0xAA55,  
  };

  if (fwrite(&mbr, 1, sizeof(mbr), img) != sizeof(mbr)){
    
    return ERROR;

  }

  return SUCCESS;
}

int main(){

  FILE *img = fopen(GPT_IMG_NAME, "wb+");

  if (!img){
    
    fprintf(stderr,"Error::create_gpt.c: could not open file %s\n",GPT_IMG_NAME);
    
    return EXIT_ERROR;

  }

  IMAGE_SIZE = ESP_SIZE + DATA_SIZE + PADDING;
  IMAGE_LBA = byte_to_LBA(IMAGE_SIZE);

  if (!write_protective_mbr(img)){

    fprintf(stderr,"Error::create_gpt.c: could not write protective mbr\n");

    return EXIT_ERROR;
  
  }

  return EXIT_SUCCESS;

}
