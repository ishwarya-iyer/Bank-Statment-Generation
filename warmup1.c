#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <locale.h>

#include "cs402.h"

#include "my402list.h"

typedef struct transaction{
char sign;
long timestmp;
char desc[28];
int amount;
}transtruct;

static void BubbleForward(My402List *pList, My402ListElem **pp_elem1, My402ListElem **pp_elem2);
int insertinto( My402List *list, FILE *fp);
void SortInput(My402List *list1);
void PrintStatement(My402List *list);

int insertinto( My402List *list, FILE *fp )
{
char buf[1026];char *str_ptr, *tab_ptr,*str;int lines=0;char temp[13];char temp1[10];
transtruct *new=NULL;


    while (fgets( buf, sizeof(buf), fp ) != NULL ) 
    {  
new=malloc(sizeof(struct transaction));lines++;
	   str_ptr = buf;time_t seconds;time_t tempstmp;
   seconds = time(NULL);
	if(strcmp(str_ptr,"\n")==0)
	break;
	   tab_ptr = strchr(str_ptr, '\t');
        if(!tab_ptr)
        return (0); 
        
	   *tab_ptr++ ='\0';
	   if ((strcmp(str_ptr,"-")!=0)&&(strcmp(str_ptr, "+")!=0))
	   return(0);
	   new->sign=*str_ptr;
	
    str_ptr = tab_ptr;//for timestamp
	tab_ptr = strchr(str_ptr,'\t');
	 if(!tab_ptr)
        {return (0);}
	*tab_ptr++ ='\0';	
            
    strcpy(temp,str_ptr);
	if(strlen(temp)>11)
     return (0);
    tempstmp=atoi(temp);
        if(tempstmp>seconds)
            return (0);
    new ->timestmp=atoi(temp);
    
    str_ptr= tab_ptr;//for amount
	tab_ptr = strchr(str_ptr,'\t');
	 if(!tab_ptr)
         {return (0);}
	*tab_ptr++ ='\0';
   if(*str_ptr=='.')
      return (0);
	str = strchr(str_ptr,'.');
if(!str) return 0;
	*str++='\0';
if(strlen(str)!=2)
     return (0);
strcpy(temp1,str_ptr);
strcat(temp1,str);
    new->amount=atoi(temp1);
    
    str_ptr = tab_ptr;//for desc
	tab_ptr = strchr(str_ptr,'\t');
	 if(tab_ptr)
         return (0);
       if(*str_ptr=='\n')
     return (0);
	str = strchr(str_ptr,'\n');
	if(!str) return 0;
	*str++='\0';
       strncpy(new->desc,str_ptr,26);   
       (void)My402ListAppend(list, new);
     
    }//end of line reading

if (fp != stdin) fclose(fp);
if(lines==0)
 return (0);
  if(lines>1)      
SortInput(list);
return(1);
}

void SortInput(My402List *list1)
{

    My402ListElem *elem=NULL;
    int i=0;
    int num_items=My402ListLength(list1);transtruct *new;transtruct *next_new;
    for (i=0; i < num_items; i++) {
        int j=0, something_swapped=FALSE;
        My402ListElem *next_elem=NULL;

        for (elem=My402ListFirst(list1), j=0; j < num_items-i-1; elem=next_elem, j++) {
		new=elem->obj;
            long cur_val=(long)(new->timestmp),next_val=0;
		
            next_elem=My402ListNext(list1, elem);
		next_new=next_elem->obj;
            next_val = (long)(next_new->timestmp);
            if(cur_val == next_val){
            fprintf(stderr, "two transactions having same timestamp\n");
        exit(EXIT_FAILURE);
            }
            if (cur_val > next_val) {
                BubbleForward(list1, &elem, &next_elem);
                something_swapped = TRUE;
            }
        }
        if (!something_swapped) break;
    }
}
static void BubbleForward(My402List *pList, My402ListElem **pp_elem1, My402ListElem **pp_elem2)
    /* (*pp_elem1) must be closer to First() than (*pp_elem2) */
{
    My402ListElem *elem1=(*pp_elem1), *elem2=(*pp_elem2);
    void *obj1=elem1->obj, *obj2=elem2->obj;
    My402ListElem *elem1prev=My402ListPrev(pList, elem1);
/*  My402ListElem *elem1next=My402ListNext(pList, elem1); */
/*  My402ListElem *elem2prev=My402ListPrev(pList, elem2); */
    My402ListElem *elem2next=My402ListNext(pList, elem2);

    My402ListUnlink(pList, elem1);
    My402ListUnlink(pList, elem2);
    if (elem1prev == NULL) {
        (void)My402ListPrepend(pList, obj2);
        *pp_elem1 = My402ListFirst(pList);
    } else {
        (void)My402ListInsertAfter(pList, obj2, elem1prev);
        *pp_elem1 = My402ListNext(pList, elem1prev);
    }
    if (elem2next == NULL) {
        (void)My402ListAppend(pList, obj1);
        *pp_elem2 = My402ListLast(pList);
    } else {
        (void)My402ListInsertBefore(pList, obj1, elem2next);
        *pp_elem2 = My402ListPrev(pList, elem2next);
    }
}


void PrintStatement(My402List *list){
My402ListElem *elem=NULL;int j;static int mor=0;char digi[10];int len;int len1;transtruct *new;char temp[40];
    static int balance=0;int i=0;float amt;float bal;
    //int num_items=My402ListLength(list);
    printf("+");
  for(i=1;i<18;i++)
     printf("-");printf("+");
      for(i=20;i<=45;i++)
     printf("-");printf("+");
     for(i=46;i<=61;i++)
     printf("-");printf("+");
     for(i=63;i<=78;i++)
     printf("-"); printf("+");printf("\n");
     
     printf("|       ");printf("Date");
     for(i=12;i<18;i++)printf(" ");
     printf("| ");printf("Description");
     for(i=32;i<=44;i++)printf(" ");
     printf(" | ");
     for(i=48;i<=55;i++)printf(" ");printf("Amount");
     printf(" | ");for(i=64;i<=70;i++)printf(" ");printf("Balance");printf(" |");printf("\n");
     printf("+");
     for(i=1;i<18;i++)
     printf("-");printf("+");
      for(i=20;i<=45;i++)
     printf("-");printf("+");
     for(i=46;i<=61;i++)
     printf("-");printf("+");
     for(i=63;i<=78;i++)
     printf("-"); printf("+");printf("\n");
     
    
	
    for (elem=My402ListFirst(list); elem != NULL; elem=My402ListNext(list, elem)) {
        setlocale(LC_ALL, "en_US");
 	new= elem->obj;char a[30]={'\0'};char b[15]={'\0'};
time_t datetrans=(time_t)(new->timestmp); 
strcpy(a,ctime(&(datetrans)));
    for(i=0;i<=10;i++)
    b[i]=a[i];
    for(j=20;j<=23;i++,j++)
    b[i]=a[j];
        b[15]='\0';
 printf("| "); 
   printf("%s | ", b);//time displayed
   strncpy(temp,new->desc, 24);
    temp[25]='\0';
   printf("%s", temp);//displayed the description
   for(i=(21+strlen(temp));i<=45;i++)printf(" ");
printf("| ");
   if(!strcmp(&new->sign,"-"))
    printf("(");
    else printf(" ");
    
    sprintf(digi, "%d", new->amount);
    len=strlen(digi);
    len1=(len-2)/3;if((len-2)%3==0)len1--;

    len=len+len1+1;
    amt=(float)(new->amount)/100;
for(i=49;i<(61-len);i++)printf(" ");   
     printf("%'3.2f", amt); 
      if(!strcmp(&new->sign,"-"))
    printf(")");
    else printf(" ");
    printf(" | ");//end of amt display
    
    
    
    if(!strcmp(&new->sign,"-"))
    balance=balance-(new->amount);
    else
    balance=balance+(new->amount);
    if(balance<0)
    printf("(");
    else printf(" ");
if(mor)
    {printf("?,???,???.??");
     }
else{
    sprintf(digi, "%d", balance);
    len=strlen(digi);
    if(balance<0)len--;
if(len>8)
    {
    mor=1;printf("?,???,???.??");
        }
else
   {
    len1=(len-2)/3;if((len-2)%3==0)len1--;
    //if((len-2)<4)printf(" ");
    len=len+len1+1;
   bal=(float)(balance)/100;
        if(bal<0)(bal=bal*-1);
    for(i=66;i<78-len;i++)printf(" ");
    printf("%'3.2f", bal); }}
    
    if(balance<0)
    printf(")");
    else printf(" ");
    printf(" |\n");//end of amt display
    }
    printf("+");
  for(i=1;i<18;i++)
     printf("-");printf("+");
      for(i=20;i<=45;i++)
     printf("-");printf("+");
     for(i=46;i<=61;i++)
     printf("-");printf("+");
     for(i=63;i<=78;i++)
     printf("-"); printf("+");printf("\n");
        
}

int main(int argc, char **argv) {
    FILE *fptr;My402List *list2=malloc(sizeof(struct transaction));
if(argc<2||argc>3)  
 {fprintf(stderr, "Illegal Command line Format\n");
            return (0);}
if(strcmp(argv[1],"sort"))
 {fprintf(stderr, "Illegal Command line Format\n");
            return (0);}
else{
if (argc==2)
   fptr = stdin;
         if(fptr!= stdin)
             fptr = fopen(argv[2],"r");
         if (NULL == fptr) {
            fprintf(stderr, "Unable to open '%s':\n",
                  argv[2]);
            return (0); }

            
            (void)My402ListInit(list2);
            if(!insertinto(list2,fptr))
            {fprintf(stderr, "Wrong format of the content in the file\n");
        exit(EXIT_FAILURE);
            }
            //SortInput(list2);
            PrintStatement(list2);
            return (1);
}
}
