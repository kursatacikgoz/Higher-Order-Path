#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <math.h>

/*
This project aims to represent these higher-order paths by using Linked-Lists.
*/


//This is MLL. Linked list to keep all words.
typedef struct words{
	char word[20];
	struct words *next;
	struct textnums *textnum;
	struct firstord *firstnext;
	struct secord *secnext;
	struct thirdord *thirdnext;
}words;

//This is the structure to hold the first orders.
typedef struct firstord{
	struct words *which;
	struct firstord *firstnext;
}firstord;

//This is the structure to hold the second orders.
typedef struct secord{
	struct words *which;
	struct secord *secnext;
}secord;

//This is the structure to hold the second orders.
typedef struct thirdord{
	struct words *which;
	struct thirdord *thirdnext;
}thirdord;

//This is the structure to hold the num of texts and frequency of the word in this text.
typedef struct textnums{
	int num,frequency;
	struct textnums *next;
}textnums;

//This is the structure to hold the sorted words of econ file as the frequency.
typedef struct sortedecon{
	int time;
	float texttime;
	struct words *which;
	struct sortedecon *next;
}sortedecon;

//This is the structure to hold the sorted words of health file as the frequency.
typedef struct sortedhealth{
	int time;
	float texttime;
	struct words *which;
	struct sortedhealth *next;
}sortedhealth;

//This is the structure to hold the sorted words of magazin file as the frequency.
typedef struct sortedmagazin{
	int time;
	float texttime;
	struct words *which;
	struct sortedmagazin *next;
}sortedmagazin;

struct words* insert(struct words* list, FILE *fp, int textnum);
void printfirstorders(struct words* list);
void printsecorders(struct words* list);
void printthirdorders(struct words* list);
void findfirstorder(struct words* head);
void findsecorder(struct words* head);
void findthirdorder(struct words* head);
struct textnums* pushnum(struct words* word,int textnum);
struct words* pushfirstorder(struct words* head,struct words* add);
struct words* pushsecorder(struct words* head,struct words* add);
struct words* pushthirdorder(struct words* head,struct words* add);
int isfirstorder(struct words* f,struct words* s);
int issecondorder(struct words* f,struct words* s);
int isthirdorder(struct words* f,struct words* s);
struct sortedecon* sortecon(words *head, sortedecon* e);
struct sortedhealth* sorthealth(words *h, sortedhealth* e);
struct sortedmagazin* sortmagazin(words *h, sortedmagazin* e);
void printallsorted(struct sortedecon* list1,struct sortedhealth* list2,struct sortedmagazin* list3);

//These numbers are how many txts are in total and how many files are in each folders.
int textnum,econtext,healthtext,magazintext;

int main(){
	char *locale = setlocale(LC_ALL, "");

	//this is the head of mll
	words *headlist =NULL;
	FILE *fp;

	//read texts in econ file. adds the words to mll sorted
	textnum=1;
	int i = 1;
	char filename[20] = "dataset/econ/";
	itoa(i, filename+13, 10);
	strcat(filename, ".txt");
	fp = fopen(filename, "r");
	while(fp!=NULL){
		headlist = insert(headlist,fp,textnum);
		i++;
		strcpy(filename,"dataset/econ/");
		itoa(i, filename+13, 10);
		strcat(filename, ".txt");
		fp = fopen(filename, "r");
		textnum++;
	}
	//calculates how many texts are there in econ file
	econtext=i-1;

	//read texts in health file. adds the words to mll sorted
	i=1;
	strcpy(filename,"dataset/health/");
	itoa(i, filename+15, 10);
	strcat(filename, ".txt");
	fp = fopen(filename, "r");
	while(fp!=NULL){
		headlist = insert(headlist,fp,textnum);
		i++;
		strcpy(filename,"dataset/health/");
		itoa(i, filename+15, 10);
		strcat(filename, ".txt");
		fp = fopen(filename, "r");
		textnum++;
	}
	//calculates how many texts are there in health file
	healthtext=i-1;
	
	//read texts in magazin file. adds the words to mll sorted
	i=1;
	strcpy(filename,"dataset/magazin/");
	itoa(i, filename+16, 10);
	strcat(filename, ".txt");
	fp = fopen(filename, "r");
	while(fp!=NULL){
		headlist = insert(headlist,fp,textnum);
		i++;
		strcpy(filename,"dataset/magazin/");
		itoa(i, filename+16, 10);
		strcat(filename, ".txt");
		fp = fopen(filename, "r");
		textnum++;
	}
	//calculates how many texts are there in magazin file
	magazintext=i-1;

	textnum--;

	//closes the file
    fclose(fp);

    //calls the functions to find higher order and print them
    findfirstorder(headlist);
    printf("1st-order term co-occurrence: \n");
    printfirstorders(headlist);

    //calls the functions to find higher order and print them
    findsecorder(headlist);
    printf("\n\n2nd-order term co-occurrence: \n");
    printsecorders(headlist);

    //calls the functions to find higher order and print them
    findthirdorder(headlist);
    printf("\n\n3rd-order term co-occurrence: \n");
    printthirdorders(headlist);


    sortedecon *secon=NULL;
    sortedhealth *shealth=NULL;
    sortedmagazin *smagazin=NULL;

    //calls the functions to sort them
    secon=sortecon(headlist,secon);
    shealth=sorthealth(headlist,shealth);
    smagazin=sortmagazin(headlist,smagazin);

    //calls the functions to print table
    printf("\n\n");
    printallsorted(secon,shealth,smagazin);


    return 0;
}

/*This function takes the list of all the words and the list of
words in the economy file. Checks each element in the list of all words.
It adds the words in the economy file to the "e" parameter in order
from upper to lower. finally returns the "e" parameter.*/
struct sortedecon* sortecon(words *h, sortedecon* e){
	words *iter=NULL;
	textnums *itertextnum=NULL;
	int start=1,end=econtext;
	sortedecon *iteration=NULL;
	sortedecon *sortedhead=e;


	if(h!=NULL)
		iter=h;
	else
		return ;

	//this loop works for every word
	while(iter!=NULL){
		sortedecon *newptr=(sortedecon*)malloc(sizeof(sortedecon));
		newptr->next=NULL;
		newptr->which=iter;
		newptr->time=0;
		if(iter->textnum!=NULL){
			itertextnum=iter->textnum;
			//this loop works for texts with each word
			while(itertextnum!=NULL){
				if(itertextnum->num>=start && itertextnum->num<=end){
					newptr->time=itertextnum->frequency + newptr->time;
					newptr->texttime++;
				}
				itertextnum=itertextnum->next;
			}
		}

		iteration=sortedhead;
		/*these conditions check for each word in the economy folder.
		if present, it will be added sequentially to the list.*/
		if(newptr->time!=0){
			if(sortedhead==NULL){
				sortedhead=newptr;
			}else if(iteration->time < newptr->time){
				newptr->next=iteration;
				sortedhead=newptr;
			}else{
				if(iteration->next!=NULL){
					while(newptr->time <= iteration->next->time){
						iteration=iteration->next;
						if(iteration->next==NULL){
							break;
						}
					}

					if(iteration->next==NULL){
						iteration->next=newptr;
					}else{
						newptr->next=iteration->next;
						iteration->next=newptr;
					}
				}else{
					iteration->next=newptr;
				}
			}
		}
		iter=iter->next;
	}
	return sortedhead;
}

/*This function takes the list of all the words and the list of
words in the health file. Checks each element in the list of all words.
It adds the words in the health file to the "e" parameter in order
from upper to lower. finally returns the "e" parameter.*/
struct sortedhealth* sorthealth(words *h, sortedhealth* e){
	words *iter=NULL;
	textnums *itertextnum=NULL;
	int start=econtext+1,end=healthtext+econtext;
	sortedhealth *iteration=NULL;
	sortedhealth *sortedhead=e;


	if(h!=NULL)
		iter=h;
	else
		return ;

	//this loop works for every word
	while(iter!=NULL){
		sortedhealth *newptr=(sortedhealth*)malloc(sizeof(sortedhealth));
		newptr->next=NULL;
		newptr->which=iter;
		newptr->time=0;
		if(iter->textnum!=NULL){
			itertextnum=iter->textnum;
			while(itertextnum!=NULL){
				if(itertextnum->num>=start && itertextnum->num<=end){
					newptr->time=itertextnum->frequency + newptr->time;
					newptr->texttime++;
				}
				itertextnum=itertextnum->next;
			}
		}

		iteration=sortedhead;

		/*these conditions check for each word in the health folder.
		if present, it will be added sequentially to the list.*/
		if(newptr->time!=0){
			if(sortedhead==NULL){
				sortedhead=newptr;
			}else if(iteration->time < newptr->time){
				newptr->next=iteration;
				sortedhead=newptr;
			}else{
				if(iteration->next!=NULL){
					while(newptr->time <= iteration->next->time){
						iteration=iteration->next;
						if(iteration->next==NULL){
							break;
						}
					}

					if(iteration->next==NULL){
						iteration->next=newptr;
					}else{
						newptr->next=iteration->next;
						iteration->next=newptr;
					}
				}else{
					iteration->next=newptr;
				}
			}
		}
		iter=iter->next;
	}
	return sortedhead;
}

/*This function takes the list of all the words and the list of
words in the magazin file. Checks each element in the list of all words.
It adds the words in the magazin file to the "e" parameter in order
from upper to lower. finally returns the "e" parameter.*/
struct sortedmagazin* sortmagazin(words *h, sortedmagazin* e){
	words *iter=NULL;
	textnums *itertextnum=NULL;
	int start=econtext+healthtext+1,end=magazintext+healthtext+econtext;
	sortedmagazin *iteration=NULL;
	sortedmagazin *sortedhead=e;


	if(h!=NULL)
		iter=h;
	else
		return ;

	//this loop works for every word
	while(iter!=NULL){
		sortedmagazin *newptr=(sortedmagazin*)malloc(sizeof(sortedmagazin));
		newptr->next=NULL;
		newptr->which=iter;
		newptr->time=0;
		if(iter->textnum!=NULL){
			itertextnum=iter->textnum;
			while(itertextnum!=NULL){
				if(itertextnum->num>=start && itertextnum->num<=end){
					newptr->time=itertextnum->frequency + newptr->time;
					newptr->texttime++;
				}
				itertextnum=itertextnum->next;
			}
		}

		iteration=sortedhead;

		/*these conditions check for each word in the magazin folder.
		if present, it will be added sequentially to the list.*/
		if(newptr->time!=0){
			if(sortedhead==NULL){
				sortedhead=newptr;
			}else if(iteration->time < newptr->time){
				newptr->next=iteration;
				sortedhead=newptr;
			}else{
				if(iteration->next!=NULL){
					while(newptr->time <= iteration->next->time){
						iteration=iteration->next;
						if(iteration->next==NULL){
							break;
						}
					}

					if(iteration->next==NULL){
						iteration->next=newptr;
					}else{
						newptr->next=iteration->next;
						iteration->next=newptr;
					}
				}else{
					iteration->next=newptr;
				}
			}
		}
		iter=iter->next;
	}
	return sortedhead;
}

/*This function takes "file", "file number" and "the address
of the list where all words are" kept as arguments. Reads all the
words in the file and inserts alphabetically linked list. Finally,
it returns the address of the list.*/
struct words* insert(struct words* list, FILE *fp,int textnum){
	//This loop continues until the end of the words in the text file.
	while(1){
		words *iter = (words*)malloc(sizeof(words));
		if(fscanf(fp, "%s", iter->word) != EOF){
			iter->next=NULL;
			iter->secnext=NULL;
			iter->firstnext=NULL;
			iter->thirdnext=NULL;
			iter->textnum=NULL;

			//identifies the first element if the list is empty.
			//It also adds the value of textnum by calling the pushnum function.
			if(list == NULL){
				list = iter;
				list->textnum=pushnum(list,textnum);
				continue;
			}else {
				/*if the list is not empty, it finds the right place and adds.
				then changes the textnum value with the pushum function.
				changes the frequency value only if the word has already been added.*/
				words *current = list;

				if(strcmp(current->word,iter->word) > 0){
					iter->next = current;
					list = iter;
					list->textnum=pushnum(list,textnum);
					continue;
				}else if(strcmp(current->word,iter->word) == 0){
					current->textnum=pushnum(current,textnum);
					continue;
				}

				while(current -> next != NULL && strcmp(current->next->word,iter->word) < 0 ){
					current = current -> next;
				}

				if(current->next != NULL){
					if(strcmp(current->next->word,iter->word) == 0){
						current->next->textnum=pushnum(current->next,textnum);
						continue;
					}
					iter->textnum=pushnum(iter,textnum);
					iter->next=current->next;
					current->next = iter;
					continue;
				}

				iter->textnum=pushnum(iter,textnum);
				current->next=iter;
				continue;
			}
		}else{
			return list;
		}
	}
}

/* This function starts from the first
file and continues for the entire number of text files.
for each text, it matches the words in the same text as first
order and adds them to the list of first orders.*/
void findfirstorder(struct words* head){
	struct words *iter;

	if(head!=NULL){
		iter = head;
	}else
		return;

	struct textnums *iteration = NULL;
	int i;
	//this loop repeats the number of text.
	for(i=1;i<=textnum;i++){
		//checks every word for each text
		while(iter!=NULL){
			if(iter->textnum!=NULL){
				iteration=iter->textnum;
				/*checks the text for each word. it starts to find
				the first order words if it exists in the searched file.*/
				while(iteration->num != i){
					if(iteration->next!=NULL){
						iteration=iteration->next;

					}else{
						iteration=NULL;
						break;
					}
				}

				//if the word is in the searched file, it will be added.
				if(iteration!=NULL){
					if(iteration->num==i){
						struct words *iter2 = head;
						struct textnums *iteration2 = NULL;
						//other words in the same file are added to the list of first orders of the first word.
						while(iter2!=NULL){
							if(iter2==iter){
								iter2=iter2->next;
								continue;
							}

							if(iter2->textnum!=NULL){
								iteration2=iter2->textnum;
								while(iteration2->num!=i){
									if(iteration2->next!=NULL){
										iteration2=iteration2->next;
									}else{
										iteration2=NULL;
										break;
									}
								}
							}
							if(iteration2!=NULL){
								if(iteration2->num==i){
									pushfirstorder(iter,iter2);
								}
							}
							iter2=iter2->next;
							iteration2=NULL;
						}
					}
				}
			}
			iter=iter->next;
		}
		iter=head;
	}
}

/* This function controls the first orders of the first orders of
each word. if the first order of the first order of the word is
not its first order or second order yet it is added as a second order.*/
void findsecorder(struct words* head){
	struct words *iter=NULL;
	struct words *iter2=NULL;
	struct firstord *iterationfirst = NULL;
	struct firstord *iterationfirst2 = NULL;

	if(head!=NULL)
		iter = head;
	else
		return;

	//This function checks for each word
	while(iter!=NULL){
		if(iter->firstnext!=NULL){
			iterationfirst=iter->firstnext;
			while(iterationfirst!=NULL){
				if(iterationfirst->which!=NULL){
					iter2=iterationfirst->which;
					if(iter2->firstnext!=NULL){
						iterationfirst2=iter2->firstnext;
						//checks the first orders of the word's first orders.
						while(iterationfirst2!=NULL){
							/* checks whether the word has already been added as a second order and whether
							the word is the first order. if it is not the first order and the second order, the
							pushsecorder function is called and the word is added as the second order.else
							move to the next first order.*/
							if(isfirstorder(iter,iterationfirst2->which)==1 && issecondorder(iter,iterationfirst2->which)==1){
								iter = pushsecorder(iter,iterationfirst2->which);
								iterationfirst2=iterationfirst2->firstnext;
							}else{
								iterationfirst2=iterationfirst2->firstnext;
								continue;
							}
						}
					}
				}
				iterationfirst=iterationfirst->firstnext;
			}
		}

		if(iter->next!=NULL)
			iter=iter->next;
		else
			break;
	}

}
/* This function works for each word. The first orders of
the second orders of each word are checked. If it has not previously
been added to the third order list and they are not first order
or second order, it will add the word as third order.*/
void findthirdorder(struct words* head){
	struct words *iter=NULL;
	struct words *iter2=NULL;
	struct secord *iterationsec = NULL;
	struct firstord *iteration2first = NULL;

	if(head!=NULL)
		iter = head;
	else
		return;

	//This loop provides control for each word.
	while(iter!=NULL){
		if(iter->secnext!=NULL){
			iterationsec=iter->secnext;
			//This loop checks the second order list of each word.
			while(iterationsec!=NULL){
				if(iterationsec->which!=NULL){
					iter2=iterationsec->which;
					if(iter2->firstnext!=NULL){
						iteration2first=iter2->firstnext;
						//This loop checks the first order list of the second order of each word.
						while(iteration2first!=NULL){
							/* If the word has not been added as a third order before and is not included in
							the first order or second order list, it will be added as a third order.
							else move to the next first order.*/
							if(isfirstorder(iter,iteration2first->which)==1 && issecondorder(iter,iteration2first->which)==1 && isthirdorder(iter,iteration2first->which)==1){
								iter = pushthirdorder(iter,iteration2first->which);
								iteration2first=iteration2first->firstnext;
							}else{
								iteration2first=iteration2first->firstnext;
								continue;
							}
						}
					}
				}
				iterationsec=iterationsec->secnext;
			}
		}

		if(iter->next!=NULL)
			iter=iter->next;
		else
			break;
	}
}

/*This is a print function. prints the first orders
of the words added alphabetically in the desired format.*/
void printfirstorders(struct words* list){
	struct words* iter = list;

	while(iter->next!=NULL){
		if(iter->firstnext!=NULL){
			firstord *iteration = iter->firstnext;
			while(iteration != NULL){
				if(strcmp(iter->word, iteration->which->word)<0)
					printf("(%s,%s) ",iter->word, iteration->which->word);
				iteration=iteration->firstnext;
			}
		}
		iter=iter->next;
	}

	if(iter->firstnext!=NULL){
		firstord *iteration = iter->firstnext;
		while(iteration != NULL){
			if(strcmp(iter->word, iteration->which->word)<0)
				printf("(%s,%s) ",iter->word, iteration->which->word);
			iteration=iteration->firstnext;
		}
	}


}

/* This is a print function. prints the second
orders of the words added alphabetically in the desired format.*/
void printsecorders(struct words* list){

	struct words* iter = list;

	while(iter->next!=NULL){
		if(iter->secnext!=NULL){
			secord *iteration = iter->secnext;
			while(iteration != NULL){
				if(strcmp(iter->word, iteration->which->word)<0)
					printf("(%s,%s) ",iter->word, iteration->which->word);
				iteration=iteration->secnext;
			}
		}
		iter=iter->next;
	}

	if(iter->secnext!=NULL){
		secord *iteration = iter->secnext;
		while(iteration != NULL){
			if(strcmp(iter->word, iteration->which->word)<0)
				printf("(%s,%s) ",iter->word, iteration->which->word);
			iteration=iteration->secnext;
		}
	}


}

/* This is a print function. prints the first orders of the
words added alphabetically in the desired format.*/
void printthirdorders(struct words* list){
	struct words* iter = list;

	while(iter->next!=NULL){
		if(iter->thirdnext!=NULL){
			thirdord *iteration = iter->thirdnext;
			while(iteration != NULL){
				if(strcmp(iter->word, iteration->which->word)<0)
					printf("(%s,%s) ",iter->word, iteration->which->word);
				iteration=iteration->thirdnext;
			}
		}
		iter=iter->next;
	}

	if(iter->thirdnext!=NULL){
		thirdord *iteration = iter->thirdnext;
		while(iteration != NULL){
			if(strcmp(iter->word, iteration->which->word)<0)
				printf("(%s,%s) ",iter->word, iteration->which->word);
			iteration=iteration->thirdnext;
		}
	}


}

/* This is a print function. print the first 5 of the
words added as a table with the idf values in the desired format.*/
void printallsorted(struct sortedecon* list1,struct sortedhealth* list2,struct sortedmagazin* list3){
	struct sortedecon* iter1 = list1;
	struct sortedhealth* iter2 = list2;
	struct sortedmagazin* iter3 = list3;
	int i;
	printf("%-20s%-7s%-8s%-20s%-7s%-8s%-20s%-7s%-8s\n","ECON","TIMES","LOG","HEALTH","TIMES","LOG","MAGAZIN","TIMES","LOG");
	for(i=0;i<5;i++){
		if(iter1!=NULL){
			printf("%-20s%-7d%-8.2f",iter1->which->word,iter1->time,log(econtext/iter1->texttime)*iter1->time);
			if(iter1->next!=NULL){
				iter1=iter1->next;
			}
		}

		if(iter2!=NULL){
			printf("%-20s%-7d%-8.2f",iter2->which->word,iter2->time,log(healthtext/iter2->texttime)*iter2->time);
			if(iter2->next!=NULL){
				iter2=iter2->next;
			}
		}

		if(iter3!=NULL){
			printf("%-20s%-7d%-8.2f\n",iter3->which->word,iter3->time,log(magazintext/iter3->texttime)*iter3->time);
			if(iter3->next!=NULL){
				iter3=iter3->next;
			}
		}

	}
}

struct textnums* pushnum(struct words* word,int textnum){
	//defines the first value if no additions have been made before
	if(word->textnum==NULL){
		word->textnum = (textnums*)malloc(sizeof(textnums));
		word->textnum->num=textnum;
		word->textnum->next=NULL;
		(word->textnum->frequency)++;
		return word->textnum;
	}else{
		/*if any textnum value has already been assigned, it is
		looped to the end of these values. it checks the values at the
		same time and only increases the frequency if there is already
		a value assigned to the same text.*/
		textnums *iter;
		if(word->textnum!=NULL){
			iter = word->textnum;
			while(iter->next!=NULL){
				if(iter->next->num!=textnum)
					iter=iter->next;
				else{
					(iter->next->frequency)++;
					return word->textnum;
				}

			}
		}

		if(iter->next!=NULL){
			if(iter->next->num==textnum){
				(iter->next->frequency)++;
				return word->textnum;
			}
		}

		if(iter->num!=textnum){
			iter->next=(textnums*)malloc(sizeof(textnums));
			iter->next->num=textnum;
			iter->next->frequency++;
			iter->next->next=NULL;
			return word->textnum;
		}else{
			iter->frequency++;
			return word->textnum;
		}




	}
}

//This function adds the second of the parameters to the end of the first order list of the first.
struct words* pushfirstorder(struct words* head,struct words* add){
	if(head->firstnext==NULL){
		head->firstnext = (firstord*)malloc(sizeof(firstord));
		head->firstnext->which = (words*)malloc(sizeof(words));
		head->firstnext->which=add;
		head->firstnext->firstnext=NULL;
		return head;
	}else{
		struct words *iter=head;
		struct firstord *iteration =iter->firstnext;

		while(iteration->firstnext!=NULL){
			if(iteration->which!=add){
				iteration=iteration->firstnext;
			}else{
				return head;
			}
		}

		iteration->firstnext=(firstord*)malloc(sizeof(firstord));
		iteration->firstnext->which = add;
		iteration->firstnext->firstnext=NULL;
		return head;
	}
}

//This function adds the second of the parameters to the end of the second order list of the first.
struct words* pushsecorder(struct words* head,struct words* add){
	if(head!=add){

		if(head->secnext==NULL){
			head->secnext = (secord*)malloc(sizeof(secord));
			head->secnext->which = (words*)malloc(sizeof(words));
			head->secnext->which=add;
			head->secnext->secnext=NULL;
			return head;
		}else{
			struct words *iter=head;
			struct secord *iteration =iter->secnext;

			while(iteration->secnext!=NULL){
				if(iteration->which!=add){
					iteration=iteration->secnext;
				}else{
					return head;
				}
			}

			iteration->secnext=(secord*)malloc(sizeof(secord));
			iteration->secnext->which = add;
			iteration->secnext->secnext=NULL;
			return head;
		}
	}
}

//This function adds the second of the parameters to the end of the third order list of the first.
struct words* pushthirdorder(struct words* head,struct words* add){
	if(head!=add){
		if(head->thirdnext==NULL){
			head->thirdnext = (thirdord*)malloc(sizeof(thirdord));
			head->thirdnext->which = (words*)malloc(sizeof(words));
			head->thirdnext->which=add;
			head->thirdnext->thirdnext=NULL;
			return head;
		}else{
			struct words *iter=head;
			struct thirdord *iteration =iter->thirdnext;

			while(iteration->thirdnext!=NULL){
				if(iteration->which!=add){
					iteration=iteration->thirdnext;
				}else{
					return head;
				}
			}

			iteration->thirdnext=(thirdord*)malloc(sizeof(thirdord));
			iteration->thirdnext->which = add;
			iteration->thirdnext->thirdnext=NULL;
			return head;
		}
	}
}
//This function checks if parameter 2 is in the first order list of parameter 1. Returns 0 if false, or 1 if not.
int isfirstorder(struct words* f,struct words* s){
	struct words* first=NULL;
	struct words* sec=NULL;
	struct firstord* iteration = NULL;
	if(f!=NULL && s!=NULL){
		first=f;
		sec=s;

		if(first->firstnext!=NULL){
			iteration=first->firstnext;
		}else{
			return 1;
		}
	}

	if (iteration->which==sec){
		return 0;
	}


	while(iteration->firstnext!=NULL){
		iteration=iteration->firstnext;
		if(iteration->which!=sec)
			continue;
		else
			return 0;
	}


	return 1;
}

//This function checks if parameter 2 is in the second order list of parameter 1. Returns 0 if false, or 1 if not.
int issecondorder(struct words* f,struct words* s){
	struct words* first=NULL;
	struct words* sec=NULL;
	struct secord* iteration = NULL;
	if(f!=NULL && s!=NULL){
		first=f;
		sec=s;

		if(first->secnext!=NULL){
			iteration=first->secnext;
		}else{
			return 1;
		}
	}

	if (iteration->which==sec){
		return 0;
	}


	while(iteration->secnext!=NULL){
		iteration=iteration->secnext;
		if(iteration->which!=sec)
			continue;
		else
			return 0;
	}

	return 1;
}

//This function checks if parameter 2 is in the third order list of parameter 1. Returns 0 if false, or 1 if not.
int isthirdorder(struct words* f,struct words* s){
	struct words* first=NULL;
	struct words* sec=NULL;
	struct thirdord* iteration = NULL;
	if(f!=NULL && s!=NULL){
		first=f;
		sec=s;

		if(first->thirdnext!=NULL){
			iteration=first->thirdnext;
		}else{
			return 1;
		}
	}

	if (iteration->which==sec){
		return 0;
	}


	while(iteration->thirdnext!=NULL){
		iteration=iteration->thirdnext;
		if(iteration->which==sec)
			return 0;
	}

	return 1;
}







