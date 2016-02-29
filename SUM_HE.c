//Sagar Saija

//#ifdef STANDARD
/* STANDARD is defined, don't use any mysql functions */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* INCLUDE ENC/DEC file*/
#include <gmp.h>
#include "paillier.h"
//gcc -c SUM_HE.c -I/usr/include/mysql
//gcc -I /usr/local/mysql/bin/mysql -I/usr/local $(mysql_config --cflags) -shared -fPIC -o  SUM_HE.so SUM_HE.c paillier.c -lgmp


//#include <mysql/my_global.h>
#include <mysql/my_sys.h>

#include <mysql/mysql.h>

//typedef char my_bool;
//struct to hold sum of ciphertext
//my_bool SUM_HE_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
//void SUM_HE_deinit(UDF_INIT *initid);
//char* SUM_HE(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error);
//void SUM_HE_clear(UDF_INIT *initid, char *is_null, char *error);
//void SUM_HE_add(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error);

struct SUM
{
	paillier_pubkey_t *pubkey;
	paillier_ciphertext_t *ciphertext;
};

	//constructor
my_bool SUM_HE_init(UDF_INIT *initid, UDF_ARGS *args, char *message){
     struct SUM *sum = malloc(sizeof(struct SUM));
     sum->pubkey = paillier_pubkey_from_hex("cs174");
     sum->ciphertext = paillier_create_enc_zero();

     initid->ptr = (char*) sum;
    return 0;
 }


	//destructor
   void SUM_HE_deinit(UDF_INIT *initid)
   {
   	//free
	struct SUM *sum = (struct SUM*)initid->ptr;
	//delete (ciphertext*)initid->ptr;
	//delete (pubkey*)initid->ptr;
	paillier_freeciphertext(sum->ciphertext);
	paillier_freepubkey(sum->pubkey);
   }


//return function SUM_HE
   char* SUM_HE(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error)
   {
	struct SUM *sum = (struct SUM*)initid->ptr;
	//paillier_plaintext_to_str(sum->ciphertext);
	char* string =  mpz_get_str(0,16,sum->ciphertext->c); //paillier_plaintext_to_str()
     //ciphertext lat = *((ciphertext *)(SUM);
     //ciphertext lon = *((double *)(args->args[1]));



     return string;
   }


//clear

void SUM_HE_clear(UDF_INIT *initid, char *is_null, char *error)
{
        /* The clear function resets the sum to 0 for each new group
        Of course you have to allocate a longlong variable in the init
        function and assign it to the pointer as seen above */
        struct SUM *sum = (struct SUM*)initid->ptr;

	free(sum);
}


//add

void SUM_HE_add(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error)
{
	struct SUM *sum = (struct SUM*)initid->ptr;
        // For each row the current value is added to the sum
	paillier_ciphertext_t* tmp2;
        //paillier_ciphertext_t* result;
        paillier_plaintext_t* tmp;
	tmp = paillier_plaintext_from_str(args->args[0]);
	tmp2 = paillier_enc(0, sum->pubkey, tmp, paillier_get_rand_devurandom);
	paillier_mul(sum->pubkey, sum->ciphertext, sum->ciphertext, tmp2);
	//paillier_mul( *(sum->pubkey),*(sum->ciphertext),*((sum->ciphertext*)initid->ptr),(sum->ciphertext*)args->args[0]);
        //*(sum->ciphertext) = *((sum->ciphertext*)initid->ptr) +
                                   // *((sum->ciphertext*)args->args[0]);

}
