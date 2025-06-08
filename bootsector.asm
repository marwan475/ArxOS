org 0x7C00
bits 16

jmp short start ; IGNORE
nop

oem: db "Twisted " ; 8 bytes
bytespersector: dw 512 ; bytes per sector 
sectorspercluster: db 1 ; sectors per cluster
reservedsectors: dw 1 ; number of reserved sectors
numberofFATs: db 2 ; number of FAT tables
rootentries: dw 224 ; number of entries in the root dir
totalsectors: dw 2880 ; number of sectors on floppy
media: db 0xF0 ; Ignore
sectorsperFAT: dw 9 ; sectors per FAT table (size of FAT table)
sectorspertrack: dw 18 ; sectors per track
headspercylinder: dw 2 ; heads per cylinder
hiddensectors: dd 0 ; Ignore
totalbigsectors: dd 0 ; total sector count for FAT32
drivenumber: db 0 ; Drive Number
unused: db 0 ; Ignore
bootsignature: db 0x29 ; Boot Signature
serialnumber: dd 0xb3b4b5b6 ; disk serial number
volumelabel: db "TWISTED  OS" ; 11 bytes
filesystem: db "FAT12   " ; 8 bytes 

start:
    ; Set up segment registers
    xor ax, ax
    mov ds, ax
    mov es, ax

    ; Print 'X' using BIOS teletype (INT 10h, AH=0Eh)
    mov ah, 0x0E     ; Teletype function
    mov al, 'X'      ; Character to print
    mov bh, 0x00     ; Page number
    mov bl, 0x07     ; Text attribute (light gray on black)
    int 0x10

hang:
    jmp hang         ; Infinite loop

; Fill the rest of the sector with zeros and end with the boot signature
times 510 - ($ - $$) db 0
dw 0xAA55

