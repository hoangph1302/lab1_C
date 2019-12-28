#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>

//Find all lines of text containing or not containing the specified substring
void findText(char** inputText,int lineInput,char* keyFind ,int find,int notFind,int cap,bool *result) {

    int lenFind=strlen(keyFind);

    for (int i = 0; i <lineInput ; ++i) {
        int lenInput=strlen(inputText[i]);
        char *p1,*p2,*p3;
        int b;
        p1=inputText[i];
        p2=keyFind;

        for (int a = 0; a < lenInput; ++a) {
            //distinguish between lowercase and capital letters when comparing
            if(cap==0){
                if (*p1==*p2){
                    p3=p1;
                    for ( b = 0; b <lenFind ; ++b) {
                        if(*p3==*p2){
                            p3++;p2++;
                        }
                        else
                            break;
                    }
                    p2=keyFind;
                    if (b==lenFind){
                        if (find==1 && notFind==0)
                            result[i]=true;
                        else
                            result[i]=false;
                    }

                }

                //do not distinguish between lowercase and capital letters when comparing
            } else{
                if ( (toupper(*p1))==(toupper(*p2))){
                    p3=p1;
                    for ( b = 0; b <lenFind ; ++b) {
                        if(toupper(*p3)==toupper(*p2)){
                            p3++;p2++;
                        }
                        else
                            break;
                    }
                    p2=keyFind;
                    if (b==lenFind){
                        if (find==1 && notFind==0)
                            result[i]=true;
                        else
                            result[i]=false;
                    }
                }
            }
            p1++;
        }
    }
}


//sort found rows in descending order of length
void sortDescending(char** outputText, int lineOutput){
    char* tg;
    for(int i = 0; i < lineOutput - 1; i++){
        for(int j = i + 1; j < lineOutput; j++){
            if(strlen(outputText[i])<strlen(outputText[j])){
                tg = outputText[i];
                outputText[i] = outputText[j];
                outputText[j] = tg;
            }
        }
    }
}


//sort found rows in ascending order of length
void sortUpAsceding(char** outputText, int lineOutput){
    char* tg;
    for(int i = 0; i < lineOutput - 1; i++){
        for(int j = i + 1; j < lineOutput; j++){
            if(strlen(outputText[i])>strlen(outputText[j])){
                tg = outputText[i];
                outputText[i] = outputText[j];
                outputText[j] = tg;
            }
        }
    }
}


int getFromFile(char ** inputText,char* pathFile,int *lineInput){

    FILE* fpointer;
    fpointer=fopen(pathFile,"r");

    if (fpointer==NULL)
    {
        printf("ERROR\n");
        return 0;                        //0 ~ false ~ file is not exist
    }
    else {
        char* line=(char*) calloc(100,sizeof(char));
        while(fgets(line,255,fpointer)!=NULL){
            strcpy(inputText[*lineInput],line);
            ++(*lineInput);
        }
    }

    return 1;                       // 1 ~ true ~ reading is successful
}


void getFromStind(char** inputText,int* lineInput){
    char* line=(char*) calloc(1000,sizeof(char));

    while(fgets(line,255,stdin)!=NULL){
        strcpy(inputText[*lineInput],line);
        ++(*lineInput);
    }
}


void process(char** inputText,int lineInput,char** outputText,int*lineOutput,char* keyFind,int find,
             int notFind,int sortAsc,int sortDesc,int cap){

    bool result[lineInput];

    //Find all lines of text containing the specified substring
    if(find==1 & notFind==0){

        for (int j = 0; j < lineInput; ++j)
            result[j] = false;

        findText(inputText,lineInput,keyFind,find,notFind,cap,result);
    }

    //Find all lines of text not containing the specified substring
    if(find==1 && notFind==1){

        for (int i = 0; i <lineInput ; ++i)
            result[i]=true;

        findText(inputText,lineInput,keyFind,find,notFind,cap,result);
    }

    for (int j = 0; j <lineInput ; ++j) {
        if(result[j]==true){
            outputText[*lineOutput]=inputText[j];
            ++(*lineOutput);
        }
    }

    //sort found rows in descending order of length
    if(sortDesc==1){
        sortDescending(outputText,*lineOutput);
    }

    //sort found rows in ascending order of length
    if(sortAsc==1){
        sortUpAsceding(outputText,*lineOutput);
    }

}


void putsToFile(char** outputText,int lineOut,char* pathFile){
    FILE* fpointer;
    fpointer=fopen(pathFile,"w");
    if(fpointer==NULL){
        printf("ERROR FILE");
        return;
    }
    else{
        for (int i = 0; i <lineOut ; ++i) {
            fputs(outputText[i],fpointer);
        }
    }
    fclose(fpointer);
    printf("Open file '%s' to watching result!\n",pathFile);
}


int main(int argc,char *argv[]){

    int lineInput=0;

    char** inputText=(char**) calloc(100,sizeof(char*));
    for (int i = 0; i <100 ; ++i) {
        inputText[i]=calloc(100, sizeof(char));
    }

    int lineOutput=0;
    char** outputText=(char**) calloc(100,sizeof(char*));
    for (int i = 0; i <100 ; ++i) {
        outputText[i]=calloc(100, sizeof(inputText));
    }

    int option;
    char* keyFind;
    int find,notFind,sortDesc, sortAsc,cap;
    find=notFind=sortDesc=sortAsc=cap=0;

    //set up option
    while((option = getopt(argc, argv,"f:cadi")) != -1)
    {
        switch(option)
        {
            case 'f':
                find=1;
                keyFind=optarg;
                break;

            case 'c':
                notFind=1;
                break;

            case 'a':
                sortAsc = 1;
                break;

            case 'd':
                sortDesc = 1;
                break;

            case 'i':
                cap = 1;
                break;

            case '?':
                printf("unknown option: %c\n", optopt);
                break;
        }

    }

    //check option
    if(find==0 ){
        printf("Please enter option -f\n");
        return 0;
    }

    if(sortAsc==1 && sortDesc==1){
        printf("ERROR: sort (-a and -d) \n");
        return 0;
    }

    switch (argc-optind){
        case 0: // using standard input and output
            getFromStind(inputText,&lineInput);
            process(inputText,lineInput,outputText,&lineOutput,keyFind,find,notFind,sortAsc,sortDesc,cap);
            printf("Result find:\n");
            for (int i = 0; i <lineOutput; ++i) {
                printf("%s",outputText[i]);
            }
            break;

        case 1:// get input from file and using standard output
            getFromFile(inputText,argv[optind],&lineInput);
            process(inputText,lineInput,outputText,&lineOutput,keyFind,find,notFind,sortAsc,sortDesc,cap);
            printf("Result find:\n");
            for (int i = 0; i <lineOutput; ++i) {
                printf("%s",outputText[i]);
            }
            break;

        case 2 :// get input from file, and put output to file
            getFromFile(inputText,argv[optind],&lineInput);
            process(inputText,lineInput,outputText,&lineOutput,keyFind,find,notFind,sortAsc,sortDesc,cap);
            putsToFile(outputText,lineOutput,argv[++optind]);
            break;

        default:
            printf("Too many arguments!!\n");
            break;
    }
    free(inputText); free(outputText);
    return 0;
}















