#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    uint size=ftell(fptr);
    return size;
    // Find the size of secret file data
    // return secret file size
}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if(strstr(argv[2],".bmp")==NULL){
        return e_failure;
    }
    else
    {
        encInfo->src_image_fname = argv[2];
    }
    if(strchr(argv[3],'.')==NULL){
        return e_failure;
    }
    else
    {
        encInfo->secret_fname=argv[3];
        strcpy(encInfo->extn_secret_file,strchr(argv[3],'.'));
    }
    if(argv[4]!=NULL){
        if(strstr(argv[4],".bmp")==NULL){
            return e_failure;
        }
        else{
            encInfo->stego_image_fname=argv[4];
        }
    }
    else{
        encInfo->stego_image_fname="stego.bmp";
    }
    return e_success;
    // step 1 -> check argv[2] is having .bmp or not
        //yes -> store the file name into encInfo -> src_image_fname = argv[2]
        //no - > return e_failure

    // step 2 -> check argv[3] is having extn there or not
        // yes - > store the file name into encInfo -> secret_fname = argv[3]
        // no -> return e_failure
    
    // step 3 -> check argv[4] is NULL or not
        //yes -> check argv[4] is having .bmp or not
                //yes -> store the file name into encInfo -> stego_image_fname = argv[4]
                //no - > return e_failure
        
        //no -> store default name the encInfo -> stego_image_fname = "stego.bmp"


    // return e_success;
}

Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

        return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

        return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

        return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
     encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file=get_file_size(encInfo->fptr_secret);
    if(encInfo->image_capacity > 16+32+(strlen(encInfo->extn_secret_file)*8)+32+(encInfo->size_secret_file*8)){
        return e_success;
    }
    else{
        printf("Secret file is greater than source image\n");
        return e_failure;
    }
    //step 1 -> encInfo -> image_capacity = call get_image_size_for_bmp(encoInfo -> fptr_src_image)

    //step 2 -> encInfo -> size_secret_file = call get_file_size(encInfo -> fptr_secret)

    //step 3 -> check encInfo -> image_capacity > 16 + 32 + (strlen(encInfo -> extn_secret_file) * 8) + 32 + (encInfo -> size_secret_file * 8)
                //t -> return e_success;
                //f -> return e_failure
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
   char header[54];
    fseek(fptr_src_image,0,SEEK_SET);
    if(fread(header,sizeof(char),54,fptr_src_image)!=54){
        printf("Read Failure\n");
        return e_failure;
    }
    if(fwrite(header,sizeof(char),54,fptr_dest_image)!=54){
        printf("Write Failure\n");
        return e_failure;
    }
    return e_success;
    // step 1 -> rewind source file pointer

    // step 2 -> read 54 bytes from source file

    // step 3 -> write 54 bytes to dest file

    // return e_success
}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    for(int i=0;magic_string[i]!='\0';i++){
        char str[8];
      fread(str,sizeof(char),8,encInfo->fptr_src_image)
       encode_byte_to_lsb(magic_string[i], buffer);
            
        }
        
        // for(int k=0 ; k<8; k++)
        // {
        //     printf("%c -- ",str[k]);
        //     for(int l=7 ; l>=0 ; l--)
        //     {
        //         printf("%d",(str[k]>>l) & 1);
        //     }
        //     printf("\n");
        // }
        // printf("-----------------------------------------\n");
    //    for(int j=0 ; j<8 ; j++)
    //     {
    //         int get = (magic_string[i]>>(7-j)) & 1;
    //         if(get)
    //             str[j] = str[j] | 1;
    //         else
    //             str[j] = str[j] & ~1;
    //     }
    //     // for(int k=0 ; k<8 ; k++)
    //     // {
    //     //     printf("%c -- ",str[k]);
    //     //     for(int l=7 ; l>=0 ; l--)
    //     //     {
    //     //         printf("%d",(str[k]>>l) & 1);
    //     //     }

    //     //     printf("\n");
    //     // }
    //     // printf("-----------------------------------------\n");
    //     if(fwrite(str,sizeof(char),8,encInfo->fptr_stego_image)!=8){
    //         return e_failure;
    //     }
    // }
    return e_success;
    // char buffer[8];

    //step 1 -> read 8 bytes from source file

    //step 2 -> call encode_byte_to_lsb(magic_string[i], buffer)

    //step 3 -> write the buffer into dest file

    // repeat this for strlen(magic_string) times from step 1
}
Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)
{
    char str[32];
    fread(str,sizeof(char),32,encInfo->fptr_src_image);
    encode_size_to_lsb(size, buffer);
    // for(int i=0;i<32;i++){
    //     int get=(size>>(31-i))&1;
    //         if(get)
    //            str[i] = str[i] | 1;
    //         else
    //             str[i] = str[i] & ~1;
    // }
    // fwrite(str,sizeof(char),32,encInfo->fptr_stego_image);
    // char buffer[32];

    //step 1 -> read 32 bytes from source file

    //step 2 -> call encode_size_to_lsb(size, buffer)

    //step 3 -> write the buffer into dest file
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    char str[8];
    fread(str,sizeof(char),8,encInfo->fptr_src_image);
     encode_byte_to_lsb(file_extn[i], buffer);
    // for(int i=0;i<32;i++){
    //     int get=(size>>(31-i))&1;
    //         if(get)
    //            str[i] = str[i] | 1;
    //         else
    //             str[i] = str[i] & ~1;
    // }
    // char buffer[8];

    //step 1 -> read 8 bytes from source file

    //step 2 -> call encode_byte_to_lsb(file_extn[i], buffer)

    //step 3 -> write the buffer into dest file

    // repeat this for strlen(file_extn) times from step 1   
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char str[32];
    fread(str,sizeof(char),32,encInfo->fptr_src_image);
     encode_size_to_lsb(file_size, buffer);

    // for(int i=0;i<32;i++){
    //     int get=(size>>(31-i))&1;
    //         if(get)
    //            str[i] = str[i] | 1;
    //         else
    //             str[i] = str[i] & ~1;
    // }
    // fwrite(str,sizeof(char),32,encInfo->fptr_stego_image);
    // char buffer[32];

    //step 1 -> read 32 bytes from source file

    //step 2 -> call encode_size_to_lsb(file_size, buffer)

    //step 3 -> write the buffer into dest file
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
   
    //step 1 -> read secret data from file and store it into encInfo -> secret_data

    // char buffer[8];

    //step 2 -> read 8 bytes from source file

    //step 3 -> call encode_byte_to_lsb(encInfo -> secret_data[i], buffer)

    //step 4 -> write the buffer into dest file

    // repeat this for strlen(encInfo -> size_secret_file) times from step 2
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    // write logic for copy remaining data

    // return e_success
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{

    for(int j=0 ; j<8 ; j++)
        {
            int get = (data>>(7-j)) & 1;
            if(get)
                image_buffer[j] = image_buffer[j] | 1;
            else
                image_buffer[j] =image_buffer[j] & ~1;
        }

fwrite(image_buffer,sizeof(char),8,encInfo->fptr_stego_image)

    // write logic to encode the char
}

Status encode_size_to_lsb(int size, char *imageBuffer)
{
    for(int i=0;i<32;i++){
        int get=(size>>(31-i))&1;
            if(get)
              imageBuffer[i] = imageBuffer[i] | 1;
            else
                imageBuffer[i] = imageBuffer[i] & ~1;
    }
    fwrite(str,sizeof(char),32,encInfo->fptr_stego_image);
    // write logic to encode the int
}

Status do_encoding(EncodeInfo *encInfo)
{ f(open_files(encInfo)==e_failure){
        return e_failure;
    }
    else{
        printf("Files Opened Success\n");
    }
    if(check_capacity(encInfo)==e_failure){
        printf("Error: no capacity\n");
        return e_failure;
    }
    else{
        printf("Image has sufficient capacity\n");
    }
    if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image)==e_failure){
        printf("Unable to copy\n");
        return e_failure;
    }
    else{
        printf("Copied successfully\n");
    }
    encode_magic_string(MAGIC_STRING,encInfo);
    encode_secret_file_extn_size(strlen(encInfo->extn_secret_file),encInfo);
     encode_secret_file_extn(encInfo->extn_secret_file,encInfo);
    //step 1 -> check open_files(encInfo) is returning e_success or not
            //no -> print error return e_failure
            //yes -> print success and goto next step

    //step 2 -> check check_capacity(encInfo) is returning e_success or not
            //no -> print error return e_failure
            //yes -> print success and goto next step

    //step 3 -> call copy_bmp_header(encInfo -> fptr_src_image, encInfo -> fptr_stego_image)is returning e_success or not
            //no -> print error return e_failure
            //yes -> print success and goto next step

    //step 4 -> call encode_magic_string(MAGIC_STRING, encInfo)

    //step 5 -> call encode_secret_file_extn_size(strlen(encInfo -> extn_secret_file), encInfo)

    //step 6 -> call encode_secret_file_extn(encInfo -> extn_secret_file, encInfo)

    //step 7 -> call encode_secret_file_size(encInfo ->  size_secret_file, encInfo)

    //step 8 -> call encode_secret_file_data(encInfo)

    //step 9 -> copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image)is returning e_success or not
            //no -> print error return e_failure
            //yes -> print success


    // return e_success

}