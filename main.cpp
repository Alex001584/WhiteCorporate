#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <windows.h>
#include <conio.h>

//Definiciones para mejor lectura del codigo
#define CARACTER_CURSOR 175
#define CARACTER_BLOQUE_PARA_DIBUJO 219
#define CODIGO_FLECHA 224
#define FLECHA_ARRIBA 72
#define FLECHA_ABAJO 80

#include "librerias.h"
#include "estructuras.h"
#include "prototipos.h"
#include "portada.h"

void MenuPrincipal(){
	HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int sesion; bool bandera;

    char opciones[][64] = {
        "Iniciar sesion como administrador",
        "Iniciar sesion como capturista",
        "Salir"
    };

    //Calculo dinamicamente la cantidad de cadenas en las opciones
    const short sizeOpciones = sizeof(opciones) / sizeof(opciones[0]);

    //Aloco memoria en pedazos con calloc para asignar una cadena a cada fila de la matriz
    char **pOpciones = (char**)calloc(sizeOpciones,sizeof(char*));
    for (size_t i = 0; i < sizeOpciones; i++)
    {
        pOpciones[i] = opciones[i];
    }
    

    do{
    	SetConsoleTextAttribute(hConsole, 11); //El color asignado de este men� es el aguamarina

        sesion = crearMenu("--- MENU PRINCIPAL ---", pOpciones,sizeOpciones); //Creo la interfaz del menu y retorno la seleccion del usuario

        switch(sesion){
            case 1:
                bandera = inicioSesion(ADMINISTRADOR);
                if (bandera) MenuAdministrador();
                break;
            case 2:
                bandera = inicioSesion(CAPTURISTA);
                if (bandera) MenuCapturista();
                break;
            case 3:
                printf("\nQu%c tenga un excelente d%ca :3\n", 130, 161);
                return;
            default:
                printf("\nEst%c opci%cn no existe, int%cntelo de nuevo\n", 160, 162, 130);
        }
    }while(!bandera);
    
    free(pOpciones);
    system("cls");
    return;
}

void lecturaContra(char contra[100])
{
    size_t cont = 0; const int size = 100;

    while ((contra[cont] = getch()) != '\r')
    {
        if (contra[cont] == '\b')
        {
            if (cont > 0)
            {
                printf("\b \b");
                cont--;
                contra[cont] = '\0';
            }
        }
        else if (cont < size)
        {
            printf("*");
            cont++;
        }
    }
    contra[cont] = '\0';

    return;
}

bool inicioSesion(short privilegioEsperado)
{
    USUARIOS activo; char nombreArchivo[32];
    if (privilegioEsperado == ADMINISTRADOR) strcpy(nombreArchivo,"administradores.txt");
    else if (privilegioEsperado == CAPTURISTA) strcpy(nombreArchivo,"capturistas.txt");
    else
    {
        printf("\nParametro invalido\n");
        return false;
    }

    //Limipio la pantalla
    system("cls");

    //Declaracion de variables
    FILE *Fichero;
    pLISTA_USUARIOS lista;
    int v,x;

    //Abro el archivo a usar dependiendo de las credenciales esperadas
    Fichero = fopen(nombreArchivo,"r");

    //Aloco memoria
    lista = (pLISTA_USUARIOS)malloc(sizeof(LISTA_USUARIOS)); //Aloco memoria a la estructura puntero de la lista de usuarios
    lista->usuario = (USUARIOS*)malloc(sizeof(USUARIOS)); //Aloco memoria al sector que lleva los datos de la estructura puntero de usuarios
    
    //Hago la primera lectura de usuario y contra al archivo
    v = fscanf(Fichero,"%s\t%s",lista->usuario->usuario,lista->usuario->contra);
    lista->siguiente = NULL; lista->anterior = NULL; //Inicializo los apuntadores

    //Creo el loop que lee todos los usuarios y contras del archivo y los coloco en la lista
    while (v != EOF && !feof(Fichero))
    {
        //Creo y aloco memoria al nuevo nodo
        pLISTA_USUARIOS nuevo;
        nuevo = (pLISTA_USUARIOS)malloc(sizeof(LISTA_USUARIOS));

        //Le otorgo las credenciales leidas del archivo al nodo
        v = fscanf(Fichero,"%s\t%s",nuevo->usuario->usuario,nuevo->usuario->contra);

        //Inicializo los apuntadores
        nuevo->siguiente = NULL;
        nuevo->anterior = lista;

        //Actualizo los apuntadores de los nodos anteriores para incorporar el nuevo nodo
        lista->siguiente = nuevo;
        lista = lista->siguiente;
    }

    while (lista->anterior != NULL) lista = lista->anterior; //Regreso al inicio de la lista

    bool bandera = false; int intentos = 5; //Otorgo 5 intentos antes de expulsarlos del menu
    do {

        //Obtener el centro
        COORD pos;
        obtenerCentroConsola(&pos);
        const short Xcentro = pos.X;
        
        //Limpiar pantalla
        system("cls");

        //Centrar el titulo
        pos.Y -= 2;
        pos.X = Xcentro - 14;
        gotoxy(pos);
        printf("--- INTENTOS RESTANTES: %i ---",intentos);

        //Creo el campo de usuario
        pos.Y += 2;
        gotoxy(pos);
        printf("Usuario: ");
        fgets(activo.usuario,100,stdin);
        activo.usuario[strlen(activo.usuario) - 1] = '\0';

        //Creo el campo de contra
        pos.Y++;
        gotoxy(pos);
        printf("Contrase%ca: ",164);
        lecturaContra(activo.contra);

        do
        {
            if (strcmp(lista->usuario->usuario,activo.usuario) == 0 && strcmp(lista->usuario->contra,activo.contra) == 0)
                bandera = true; //Si se introducen credenciales validas se marca como verdadera la bandera
        } while (lista->siguiente != NULL && !bandera); //Recorro la lista hasta coincidir en una credencial valida o hasta terminar la lista

        if (bandera)
        {
            system("cls");
            printf("Bienvenido, %s",activo.usuario);
        }
        else
        {
            pos.X = Xcentro - 15;
            pos.Y += 2;
            gotoxy(pos);
            printf("Usuario o contrase%ca incorrecta",164);
            Sleep(1000);
        }

        intentos--;
    } while (!bandera && intentos > 0); //Continua mientras el usuario aun tenga intentos y no se hayan introducido credenciales correctas
    
    if (!bandera) printf("\nIntentos agotados, regresando al menu\n"); //Si sale del while sin haber encontrado la credencial avisa al usuario que se le acabaron los intentos
    
    fclose(Fichero);
    return bandera;
}

void MenuAdministrador(){
	HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int admin;

    do{
    	SetConsoleTextAttribute(hConsole, 13); //El color asignado de este men� es el morado
    	
        char opciones[][32] = {
            "Registrar nuevo usuario",
            "Registrar proveedor",
            "Registrar materia prima",
            "Gestion de modelos de productos",
            "Realizar pedidos a proveedores",
            "Regresar al menu principal"
        };

        const short sizeOpciones = sizeof(opciones) / sizeof(opciones[0]);

        char** pOpciones = (char**)calloc(sizeOpciones,sizeof(char*));
        for (size_t i = 0; i < sizeOpciones; i++)
        {
            pOpciones[i] = opciones[i];
        }
        

        admin = crearMenu("--- MENU DEL ADMINISTRADOR ---",pOpciones,sizeOpciones);

        switch(admin){
            case 1:
                RegistrarUsuario();
                break;
                
            case 2:
                RegistrarProveedor();
                break;
                
            case 3:
                RegistrarMatePrim();
                break;
                
            case 4:
                GestionProductos();
                break;
                
            case 5:
                PedidosProveedores();
                break;
            
            case 6:
                printf("\nVolviendo al men%c principal\n", 163);
                break;
                
            default:
                printf("\nEst%c opci%cn no existe, int%cntelo de nuevo\n", 160, 162, 130);
        }
    }while(admin != 6);
}

void RegistrarUsuario(){
    FILE *Fichero; Fichero = fopen("capturistas.txt","a");
    if (Fichero == NULL)
    {
        perror("\nError al abrir el archivo\n\n");
        exit(-1);
    }

    USUARIOS nuevo; int v,x;
    while ((x = getchar() != '\n' && x != EOF));
    printf("\n--- NUEVO USUARIO ---\n");
    printf("\nUsuario: ");
    fgets(nuevo.usuario,100,stdin);
    nuevo.usuario[strlen(nuevo.usuario)-1] = '\0';
    printf("\nContrase%ca: ",164);
    fgets(nuevo.contra,100,stdin);
    nuevo.contra[strlen(nuevo.contra)-1] = '\0';

    v = fprintf(Fichero,"\n%s\t%s",nuevo.usuario,nuevo.contra);
    if (v < 0)
    {
        perror("\nError al escribir al archivo\n\n");
        exit(-1);
    }
    else printf("\nUsuario a%cadido exitosamente\n",164);
    
    fclose(Fichero);
    return;
}

void RegistrarProveedor(){
    FILE *Fichero; Fichero = fopen("proveedores.txt","a");
    if (Fichero == NULL)
    {
        perror("\nError al abrir el archivo\n\n");
        exit(-1);
    }

    PROVEEDOR nuevo; int v,x; bool bandera;
    printf("\n--- NUEVO PROVEEDOR ---\n");
    do
    {
        printf("\nID: ");
        v = scanf("%i",&nuevo.id);
        while ((x = getchar()) != '\n' && x != EOF);
    } while (v != 1 || nuevo.id < 1);
    
    printf("\nNombre: ");
    fgets(nuevo.nombre,50,stdin);
    nuevo.nombre[strlen(nuevo.nombre)-1] = '\0';

    printf("\nDireccion: ");
    fgets(nuevo.direccion,100,stdin);
    nuevo.direccion[strlen(nuevo.direccion)-1] = '\0';

    do
    {
        printf("\nTelefono: ");
        fgets(nuevo.telefono,15,stdin);
        nuevo.telefono[strlen(nuevo.telefono)-1] = '\0';

        bandera = true;
        for (size_t i = 0; i < strlen(nuevo.telefono); i++)
        {
            if (!isdigit(nuevo.telefono[i])) bandera = false;
        }
        
        if (!bandera) printf("\nIntroduce un numero de telefono valido\n");
    } while (!bandera);
    
    v = fprintf(Fichero,"%i\t%s\t%s\t%s\n",nuevo.id,nuevo.nombre,nuevo.direccion,nuevo.telefono);
    if (v < 0)
    {
        perror("\nError al escribir al archivo\n\n");
        exit(-1);
    }
    else printf("\n--- PROVEEDOR ESCRITO EXITOSAMENTE ---\n");

    fclose(Fichero);
    return;
}

void RegistrarMatePrim(){
    FILE *Fichero; Fichero = fopen("materias_primas.txt","a");
    if (Fichero == NULL)
    {
        perror("\nError al abrir el archivo\n\n");
        exit(-1);
    }

    MATERIA_PRIMA nuevo; int v,x; bool bandera;
    printf("\n--- NUEVA MATERIA PRIMA ---\n");
    do
    {
        printf("\nID: ");
        v = scanf("%i",&nuevo.id);
        while ((x = getchar()) != '\n' && x != EOF);
    } while (v != 1 || nuevo.id < 1);
    
    printf("\nNombre: ");
    fgets(nuevo.nombre,50,stdin);
    nuevo.nombre[strlen(nuevo.nombre)-1] = '\0';

    do
    {
        printf("\nCantidad: ");
        v = scanf("%f",&nuevo.cantidad);
        while ((x = getchar() != '\n' && x != EOF));
    } while (v != 1 || nuevo.cantidad < 0);
    
    v = fprintf(Fichero,"%i\t%s\t%0.2f\n",nuevo.id,nuevo.nombre,nuevo.cantidad);
    if (v < 0)
    {
        perror("\nError al escribir al archivo\n\n");
        exit(-1);
    }
    else printf("\n--- MATERIA PRIMA ESCRITA EXITOSAMENTE ---\n");

    fclose(Fichero);
    return;
}

void GestionProductos(){
	
}

void PedidosProveedores(){
	
}

void MenuCapturista(){
	HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    int capturista;
    
    do{
    	SetConsoleTextAttribute(hConsole, 4); //El color asignado de este men� es el rojo
    	
        printf("\nMENU CAPTURISTA\n");
        printf("1. Registrar pedido de materia prima\n");
        printf("2. Generar reporte de modelos por materia prima\n");
        printf("3. Generar reporte del total de material en inventario\n");
        printf("4. Generar reporte de pedidos de materia prima a proveedores\n");
        printf("5. Volver al men%c principal\n", 163);
        printf("\nQue opci%cn desea elegir?: ", 162);
        scanf("%d", &capturista);

        switch(capturista){
            case 1:
                PedidoMateriaPrima();
                break;
                
            case 2:
                ReporteModelos();
                break;
                
            case 3:
                ReporteInventario();
                break;
                
            case 4:
                PedidosMatPriProveedores();
                break;
                
            case 5:
                printf("\nVolviendo al men%c principal\n", 163);
                break;
                
            default:
                printf("\nEst%c opci%cn no existe, int%cntelo de nuevo\n", 160, 162, 130);
        }
    } while(capturista != 5);
}

void PedidoMateriaPrima(){
	
}

void ReporteModelos(){
	
}

void ReporteInventario(){
	
}

void PedidosMatPriProveedores(){
	
}

int main() {
    
    // imprimirPortada();
    // system("cls");

    system("cls");
    MenuPrincipal();
    
    return 0;
}
