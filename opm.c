/*
 * C program to find pattern from 1 Million data
 * using KMP as a filtration
 *
 * Author : Nisarg Patel
 *
 */ 
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <malloc.h>

/* size of text and pattern 1 million numbers*/
#define TXT_SIZE 1000000
#define PATTERN_SIZE 7

void radix_sort(uint32_t *p,uint32_t length);
void KMP_prefix(uint32_t *pat,uint32_t pat_length);
void KMP_matcher(uint32_t *txt,uint32_t *pat,uint32_t txt_length,uint32_t pat_length);
void transform(uint32_t *txt,uint32_t *pat,uint32_t txt_length,uint32_t pat_length);
void trans_equal(uint32_t *arr,uint32_t length);
void gen_aux(uint32_t *arr,uint32_t length);

/* text data array */
uint32_t text[TXT_SIZE];

/*pattern to be found inside text*/
uint32_t pattern[PATTERN_SIZE];

/*sorted pattern*/
uint32_t sorted_pat[PATTERN_SIZE];

/*auxiliary table to store relative position*/
uint32_t aux[PATTERN_SIZE];

/* Binary vector to store equalities*/
uint32_t equal[PATTERN_SIZE];

/* prefix table for KMP*/
uint32_t prefix[PATTERN_SIZE-1];

/* transformed text array which stores binary value */
uint32_t trans_text[TXT_SIZE-1];
/* transformed pattern array which stores binary value */
uint32_t trans_pattern[PATTERN_SIZE-1];

uint32_t pattern_num=0;

int main(void){
	
	uint32_t i;
	//setting zero 	
	memset((void *)prefix,0,(PATTERN_SIZE-1)*sizeof(uint32_t));
	memset((void *)equal,0,(PATTERN_SIZE)*sizeof(uint32_t));
	
	//seeding with time
	srand(time(NULL));
	
	//flush output
	fflush(stdout);
	
	//generate random text
	for(i=0;i<TXT_SIZE;i++){
		text[i]=rand();
	}
	
	//generate random pattern
	for(i=0;i<PATTERN_SIZE;i++){
		pattern[i] = rand();
	}
	
	/* copy to array for radix sort*/
	for(i=0;i<PATTERN_SIZE;i++){
		sorted_pat[i] = pattern[i];
	}
	
	//perform radix sort
	radix_sort(sorted_pat,PATTERN_SIZE);
	
	//generate auxiliary table
	gen_aux(aux,PATTERN_SIZE);
	
	//generate binary array
	transform(text,pattern,TXT_SIZE-1,PATTERN_SIZE-1);
	trans_equal(aux,PATTERN_SIZE-1);
	
	//perform KMP for order preserve matching
	KMP_matcher(trans_text,trans_pattern,TXT_SIZE-1,PATTERN_SIZE-1);
	
	printf("\n-------------------------------------------------\n");
	printf("Total %d patterns found.\n",pattern_num);
	
	return 0;
}

/*
 * Converts original text and pattern array to binary array
 */
void transform(uint32_t *txt,uint32_t *pat,uint32_t txt_length,uint32_t pat_length){
	
	uint32_t loop;
	
	/*converts text to binary array trans_text*/
	for(loop=0; loop<txt_length; loop++){
		if(txt[loop] < txt[loop+1]){
			trans_text[loop]=1;
		}else{
			trans_text[loop]=0;
		}
	}
	
	/*converts pattern to binary array trans_pattern*/
	for(loop=0; loop<pat_length; loop++){
		if(pat[loop] < pat[loop+1]){
			trans_pattern[loop]=1;
		}else{
			trans_pattern[loop]=0;
		}
		
		
		
	}
}

/*
 * Knuth morris Pratt for pattern matching
 */
void KMP_matcher(uint32_t *txt,uint32_t *pat,uint32_t txt_length,uint32_t pat_length){
	
	int32_t q=-1;
	uint32_t i=0;
	uint32_t flag=0,f=0;
	KMP_prefix(pat,pat_length);
	
	for(i=0;i<txt_length;i++){
		while((q>-1) && (pat[q+1]!=txt[i])){
			q = prefix[q];
		}
		if(pat[q+1]==txt[i]){
			q = q+1;
		}
		if(q==pat_length-1){
			
			uint32_t len_cand = (i-pat_length)+1 + pat_length;
			uint32_t j,k=0;
			uint32_t cand = i-pat_length+1;
			
			/* conditions checked proposed by paper */
			for(j=(i-pat_length)+1;j<len_cand;j++){
				if(text[cand-1+aux[k]] > text[cand-1+aux[k+1]]){
					f=0;
					break;
				}else if((text[cand-1+aux[k]] == text[cand-1+aux[k+1]])&&(equal[k]==0)){
					f=0;
					break;
				}else if((text[cand-1+aux[k]] < text[cand-1+aux[k+1]])&&(equal[k]==1)){
					f=0;
					break;
				}else{
					f=1;	
				}
				k++;
			}
			if(f>0){
					flag=1;
					printf("%d(",i-pat_length+2);
					for(j=(i-pat_length)+1;j<len_cand+1;j++){
						printf("%d",text[j]);
						if(j!=len_cand){
							printf(",");
						}
					}
					printf(")\n");
					pattern_num++;
			}
			q = prefix[q];
			
		}
	}
	if(flag==0){
		printf("No pattern found!\n");
	}
	
}
void KMP_prefix(uint32_t *pat,uint32_t pat_length){
	
	int32_t k=-1;
	uint32_t q=0;
	prefix[0]=k;
	
	for(q=1;q<pat_length;q++){
		while((k>-1) && (pat[k+1]!=pat[q])){
			k = prefix[k];
		}
		if(pat[k+1]==pat[q]){
			k = k + 1;
		}
		prefix[q]=k;
	}
}

/*
 * four pass radix sort
 */
void radix_sort(uint32_t *arr,uint32_t length){
	
	uint8_t pass;
	
	/* dont touch i and keep it signed to avoid faults*/
	int32_t i;
	
	/* Arrays to store count and final sorted array */
	uint8_t countArr[256];
	
	//allocating memory for temporary array
	uint32_t *copyArr=(uint32_t *)malloc(length*sizeof(uint32_t));
	memset(copyArr,0,length*sizeof(uint32_t));
	
	/*four passes*/
	for (pass = 0; pass < 4; pass++) {
		
		//setting zero in counting array
		memset((void *)countArr,0,256*sizeof(uint8_t));
		
		/* counting sort */
        for (i = 0; i <length; i++){
			countArr[(arr[i] >> 8*pass)& 255]++;
		}
        for (i = 1; i < 256; i++){
			countArr[i] += countArr[i-1];
		}
		for (i = length-1; i >=0; i--){
			copyArr[--countArr[(arr[i] >> 8*pass)& 255]] = arr[i];
		}
		
        /* copy array value to run next pass*/
        for(i=0;i<length;i++){
			arr[i]=copyArr[i];
		}
    }
    free(copyArr);
}

/* transform into equality binary vector*/
void trans_equal(uint32_t *arr,uint32_t length){
	uint32_t loop;
	
	for(loop=0;loop<length;loop++){
		if(arr[loop] == arr[loop+1]){
			equal[loop]=1;
		}else{
			equal[loop]=0;
		}
	}
}

/* generate auxiliary table */
void gen_aux(uint32_t *arr,uint32_t length){
	
	uint32_t i,j;
	
	for(i=0;i<length;i++){
		for(j=0;j<length;j++){
			if(sorted_pat[i]==pattern[j]){
				arr[i]=j+1;
			}
		}
	}
	
}
