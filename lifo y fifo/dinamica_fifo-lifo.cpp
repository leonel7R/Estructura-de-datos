//visual studio code
#include <iostream>
#include <malloc.h>
using namespace std;

 struct nodo{
    int valor;        // valor que almacena el nodo
    struct nodo *sig;  //    
    
 };

//punteros globales para manipular las listta
struct nodo *top = NULL, *aux = NULL, *axul2 = NULL;

int addNodo(){
    aux = ((struct nodo *)  malloc(sizeof(struct nodo) ) );
    cout<<"digite el nodo: ";
    cin>>aux->valor;
    if(top==NULL){
        top = aux;
        top->sig = NULL;
    }
    else{
        aux->sig = top;
        top = aux;
    }
    aux = NULL;
    free(aux); 
}

void viewNodos(){
  for(aux=top; aux != NULL; aux = aux->sig) {
    cout <<"el valor del nodo es: " <<aux ->valor<<endl;
  }

}
//funcion para buscar un valor en la lista 
int searchNodo(){
    int seed;
    cout<< "digite el numero a buscar: ";
    cin>> seed;

    //recorre la lista comparando el valor
    for(aux=top; aux !=NULL; aux = aux->sig){
        if (aux->valor == seed){
        aux = NULL; // se limpian los punteros auxiliares
        free(aux);  // se libera la memoria de aux
        return 1; //
        }
    }
    aux = NULL;
    free(aux);
    return 0; // el valor no es encontrado
}

 
 
int  main(){
     int band, opc;

     do {
        //muestra menu al usuario
     cout << "\n    menu lifo     \n";
     cout<< "1. adicinar nodo\n";
     cout<< "2. mostrar \n";
     cout <<"3. buscar nodo\n";
     cout<< "4. salir\n";
     cout<<" seleciona una opcion: ";
     cin>> opc;

     switch(opc) {
        case 1:
             addNodo();
             break;
             case 2:
             viewNodos();
             break;
        case 3:
        band = searchNodo();
        if (band)
           cout<< "nodo encontrado.\n";     
        else
           cout <<" no encontrado.\n";
           break;
     }
     
     }while (opc != 5);


}
 

 

