#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
//Prototipos de funciones y estructuras**************************************************
struct nodo;
struct nodoGrafo;
struct grafo;
struct resultado;
int insertarNodo(struct grafo* grafo, int valorArista, int valorCamino);
int insertarNodoAux(struct nodo* lista, int valor);
struct nodo* buscar(int valor, struct grafo* grafo);
struct nodo* crearNodo(int valor);
struct nodoGrafo* crearNodoGrafo(int valorArista);
struct nodo* ultimoNodo(struct nodo* primerNodo);
bool estaEnCamino(int valor, struct nodo* Resultado);
struct nodo* encontrarCaminos(struct grafo* grafo, int destino, struct nodo* Resultado);
struct nodo* ultimoNodo(struct nodo* primerNodo);
void insertarResultado(struct resultado* res, struct nodo* camino);
struct resultado* crearResultado(struct nodo* camino);
struct nodo* retornarCamino(struct nodo *primerNodo, int valor);
void findPath(struct grafo* grafo, int siguienteCamino, struct nodo* caminos, struct resultado* resultado);
//***************************************************************************************
//Estructuras utilizada******************************************************************
struct nodo {
  int valor;
  struct nodo *siguente;
};

struct nodoGrafo{
    struct nodo* lista;//Valores a los que hay camino
    int valor;//valor de la arista (Esquina Actual)
    struct nodoGrafo* siguiente;//Valor de la sigueinte esquina
};

struct grafo{
    struct nodoGrafo* primerNodo;
    int size;//Numero de nodos en el grafo, util para insertar nuevos nodos
    int destino;//En que esquina esta el incendio
};

struct resultado{
    struct nodo* camino;
    struct resultado* siguiente;
    int size;
};
//***************************************************************************************
//Funciones de estructuras***************************************************************
int insertarNodo(struct grafo* grafo, int valorArista, int valorCamino){
    if(grafo->size == 0){//Caso: el grafo esta vacio
        //Crear el primer nodo del grafo
        struct nodoGrafo* nodoGrafo = crearNodoGrafo(valorArista);
        //Crear la lista de caminos;
        struct nodo* lista = crearNodo(valorCamino);
        //Asignar la lista de caminos al grafo
        nodoGrafo->lista = lista;
        grafo->primerNodo = nodoGrafo;
        grafo->size ++;
        return 1;
    }
    else{//Caso: el grafo tiene 1 o mas elementos
        struct nodoGrafo* temporal;//puntero al primer nodo del grafo
        temporal = grafo->primerNodo;
        while(temporal != NULL){
            if(temporal->valor == valorArista){//Caso: Se encuentra el nodo donde hay que insertar
                insertarNodoAux(temporal->lista, valorCamino);
                return 1;
            }
            else{
                if(temporal->siguiente == NULL){//Caso no existe la arista que se esta insertando
                    struct nodoGrafo* nodoGrafo;
                    nodoGrafo = crearNodoGrafo(valorArista);
                    //Crear la lista de caminos;
                    struct nodo* lista = crearNodo(valorCamino);
                    //Asignar la lista de caminos al grafo
                    nodoGrafo->lista = lista;
                    temporal->siguiente = nodoGrafo;
                    grafo->size++;
                    return 1;
                }
                else{//Caso existen más nodos para buscar
                    temporal = temporal->siguiente;
                }
            }
        }
    }
    return 0;
}

int insertarNodoAux(struct nodo* lista, int valor){
    //Crear el nuevo nodo de la lista
    struct nodo* nuevo = crearNodo(valor);
    if(lista->siguente == NULL){//Caso:La lista solo tiene un valor
        lista->siguente = nuevo;
        return 1;
    }
    else{//Caso: la lista tiene varios valores
        struct nodo* temporal;
        temporal = ultimoNodo(lista->siguente);
        temporal->siguente = nuevo;//Asignar el nuevo nodo a la lista
        return 1;
    }
}

//Retorna los caminos a los que puede ir valor
struct nodo* buscar(int valor, struct grafo* grafo){
    struct nodoGrafo* temporal = grafo->primerNodo;
    while(temporal != NULL){
        if(temporal->valor == valor){
            return temporal->lista;
        }
        else{
            temporal = temporal->siguiente;
        }
    }
    return NULL;
}

struct nodo* crearNodo(int valor){
    struct nodo* nuevo = (struct nodo*) malloc(sizeof(struct nodo));
    nuevo->siguente = NULL;
    nuevo->valor = valor;
    return nuevo;
}

struct nodoGrafo* crearNodoGrafo(int valorArista){
    struct nodoGrafo* nodoGrafo;
    nodoGrafo = (struct nodoGrafo*) malloc(sizeof(struct nodoGrafo));
    nodoGrafo->siguiente = NULL;
    nodoGrafo->valor = valorArista;
    return nodoGrafo;
}

//Retorna ultimo nodo de la lista
struct nodo* ultimoNodo(struct nodo* primerNodo){
    while(primerNodo->siguente != NULL){
        primerNodo = primerNodo->siguente;
    }
    return primerNodo;
}

//Revisar si un nodo esta en el camino
bool estaEnCamino(int valor, struct nodo* Resultado){
    struct nodo* tmp = Resultado;
    while (tmp != NULL) {
        if(tmp->valor == valor){
            return true;
        }
        else{
            tmp = tmp->siguente;
        }
    }
    return false;
}

void insertarResultado(struct resultado* res, struct nodo* camino){
    if(res->camino == NULL){
        res->camino = camino;
        res->size++;
    }
    else{
        struct resultado* tmp = res;
        while(tmp->siguiente != NULL){
            tmp = tmp->siguiente;
        }
        tmp->siguiente = crearResultado(camino);
        res->size++;
    }
}

struct resultado* crearResultado(struct nodo* camino){
    struct resultado* resultado = (struct resultado*) malloc(sizeof(struct resultado));
    resultado->camino = camino;
    resultado->siguiente = NULL;
    resultado->size = 0;
    return resultado;
}

struct nodo* retornarCamino(struct nodo *primerNodo, int valor){
    struct nodo* nueva = crearNodo(primerNodo->valor);
    struct nodo* tmp = nueva;
    if(valor!=nueva->valor){
        primerNodo = primerNodo->siguente;
        while(primerNodo != NULL){
            if(primerNodo->valor == valor){
                tmp->siguente = crearNodo(primerNodo->valor);
                tmp = tmp ->siguente;
                break;
            }
            else{
                tmp->siguente = crearNodo(primerNodo->valor);
                tmp = tmp ->siguente;
                primerNodo = primerNodo->siguente;
            }
        }
    }
    return nueva;
}
//***************************************************************************************
void findPath(struct grafo* grafo, int siguienteCamino, struct nodo* caminos, struct resultado* resultado){
    struct nodo* ultimo = ultimoNodo(caminos);
    if(ultimo->valor == 1){
        //Guardar el camino en resultado
        insertarResultado(resultado, caminos);
    }
    else{
        struct nodo* listaCaminos = buscar(siguienteCamino, grafo);//lista de caminos
        while(listaCaminos != NULL){
            //Revisar que el nodo no este en el camino actual
            if(!estaEnCamino(listaCaminos->valor, caminos)){
                ultimo->siguente = crearNodo(listaCaminos->valor);
                //Recursividad para revisar todos los caminos desde un nodo
                findPath(grafo, listaCaminos->valor, caminos, resultado);
                //Resetear las variables al estado del ciclo
                caminos = retornarCamino(caminos, siguienteCamino);
                ultimo = ultimoNodo(caminos);
                listaCaminos = listaCaminos->siguente;
                //
            }
            else{
                listaCaminos = listaCaminos->siguente;
            }
        }
    }
}

int main(){
    //variables intermedias para pasar la entrada al grafo
    int u, v;
    struct grafo* grafo;//Crear el grafo para representar las esquinas y los caminos posibles
    grafo = (struct grafo*) malloc(sizeof(struct grafo));
    grafo->size = 0;
    grafo->primerNodo = NULL;
    scanf("%d", &grafo->destino);
    scanf("%d%d", &u, &v);
    while(u != 0 && v != 0){
        //Los insertar estan para hacer el grafo no dirigido
        insertarNodo(grafo, u, v);
        insertarNodo(grafo, v, u);
        scanf("%d%d", &u, &v);
    }
    struct nodo* caminos = crearNodo(grafo->destino);
    struct resultado* resultado = crearResultado(NULL);
    findPath(grafo, grafo->destino, caminos, resultado);
    printf("%d", resultado->size);
    return 0;
}
