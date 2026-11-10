#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"

OperationType check_operation_type(char *);

int main(int argc, char *argv[]) // .aout -e <source_file> <secret_data_file> <optional>
{
    if(argc==1){
        return e_failure;
    }
    int type=check_operation_type(argv[1]);
    if(type==e_encode){
        EncodeInfo encInfo;
        if(read_and_validate_encode_args(argv,&encInfo)){
            printf("Success\n");
            //return e_success;
            if(do_encoding(&encInfo)==e_success){
                printf("Capacity is sufficient for encoding\n");
                return e_success;
            }
            else{
                return e_failure;
            }
        }

        else{
            printf("Command line ARgument should be only .aout -e <source_file> <secret_data_file> <optional> in this format\n");
            return e_failure;
        }
    }
    //step 1 -> check check_operation_type(argv[1]) is returning e_encode or not
        // yes -> EncodeInfo encInfo;
            // check read_and_validate_encode_args(argv, &encInfo) is returning e_success or e_failure
                // failure -> print error msg and stop
                // success -> print success msg 
                            // check do_encoding(&encInfo) is returning e_success or e_failure
                                    // failure -> print error msg and stop
                                    // success -> print success msg and stop
}

OperationType check_operation_type(char *symbol)
{
    if(strcmp(symbol,"-e")==0){
        return e_encode;
    }
    else if(strcmp(symbol,"-d")==0){
        return e_decode;
    }
    return e_unsupported;
    //step 1 -> check symbol is -e or not
        // yes -> return e_encode
    
    // step 2 -> check symbol is -d or not
        // yes -> return e_decode

    // return e_unsupported
}
