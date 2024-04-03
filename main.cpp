#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <windows.h>
#include <conio.h>

//Definiciones para mejor lectura del codigo
#define RUTA_ADMINISTRADOR "administradores.txt"
#define RUTA_CAPTURISTA "capturistas.txt"
#define CARACTER_CURSOR 175
#define CARACTER_BLOQUE_PARA_DIBUJO 219
#define CODIGO_FLECHA -32
#define FLECHA_ARRIBA 72
#define FLECHA_ABAJO 80
#define FLECHA_IZQUIERDA 75
#define FLECHA_DERECHA 77

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

void movimientoDelCampo(short *seleccion, short direccionalPresionada)
{    
    switch (*seleccion) {
    case CAMPO_USUARIO:
        if (direccionalPresionada == FLECHA_ABAJO) *seleccion = CAMPO_CONTRA;
        break;
    case CAMPO_CONTRA:
        if (direccionalPresionada == FLECHA_ARRIBA) *seleccion = CAMPO_USUARIO;
        if (direccionalPresionada == FLECHA_ABAJO) *seleccion = CAMPO_ADMIN;
        break;
    case CAMPO_ADMIN:
        if (direccionalPresionada == FLECHA_ARRIBA) *seleccion = CAMPO_CONTRA;
        if (direccionalPresionada == FLECHA_ABAJO) *seleccion = CAMPO_CANCELAR;
        if (direccionalPresionada == FLECHA_DERECHA) *seleccion = CAMPO_CAPTURISTA;
        break;
    case CAMPO_CAPTURISTA:
        if (direccionalPresionada == FLECHA_ARRIBA) *seleccion = CAMPO_CONTRA;
        if (direccionalPresionada == FLECHA_ABAJO) *seleccion = CAMPO_OK;
        if (direccionalPresionada == FLECHA_IZQUIERDA) *seleccion = CAMPO_ADMIN;
        break;
    case CAMPO_OK:
        if (direccionalPresionada == FLECHA_ARRIBA) *seleccion = CAMPO_CAPTURISTA;
        if (direccionalPresionada == FLECHA_IZQUIERDA) *seleccion = CAMPO_CANCELAR;
        break;
    case CAMPO_CANCELAR:
        if (direccionalPresionada == FLECHA_ARRIBA) *seleccion = CAMPO_ADMIN;
        if (direccionalPresionada == FLECHA_DERECHA) *seleccion = CAMPO_OK;
        break;
    default:
        break;
    }

    return;
}

void registrarEntrada(char cadena[], short *cont, short *posX, char caracter, short campo)
{
    const short size = 100;
    if (caracter == '\b')
    {
        if (*cont > 0)
        {
            printf("\b  \b");
            *posX = *posX - 1;
            *cont = *cont - 1;
            cadena[*cont] = '\0';
        }
    }
    else if (*cont < size)
    {
        if (campo == CAMPO_USUARIO) printf("%c",caracter);
        else if (campo == CAMPO_CONTRA) printf("*");

        cadena[*cont] = caracter;
        *cont = *cont + 1;
        *posX = *posX + 1;
    }
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
    if (privilegioEsperado == ADMINISTRADOR) strcpy(nombreArchivo,RUTA_ADMINISTRADOR);
    else if (privilegioEsperado == CAPTURISTA) strcpy(nombreArchivo,RUTA_CAPTURISTA);
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
    if (Fichero == NULL)
    {
        system("cls");
        perror("Erorr al leer el archivo\n");
        system("PAUSE");
        exit(-1);
    }

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

    char cursor; short seleccion = CAMPO_USUARIO, salir = 0, contUsuario = 0, contContra = 0; bool esFlecha = false;
    USUARIOS nuevo; COORD pos, campoUsuarioPos, campoContraPos, camposMultiSeleccionPos[4];

    //Centro
    obtenerCentroConsola(&pos);
    const short Xcentro = pos.X;

    //Preparo Y
    pos.Y -= 4;
    
    //Imprimo titulo
    pos.X = Xcentro - 10;
    gotoxy(pos);
    caracterPrint("--- NUEVO USUARIO ---");

    //Imprimo las areas de input
    pos.Y += 2;
    pos.X -= 5;
    gotoxy(pos);
    printf("Usuario: ");
    campoUsuarioPos.X = pos.X + 9; //Obtengo la posicion de la input
    campoUsuarioPos.Y = pos.Y;

    pos.Y++;
    gotoxy(pos);
    printf("Contrase%ca: ",164);
    campoContraPos.X = pos.X + 12;
    campoContraPos.Y = pos.Y;

    pos.Y += 2;
    pos.X = Xcentro - 20;
    gotoxy(pos);
    printf("Privilegio:  ");
    camposMultiSeleccionPos[CAMPO_ADMIN].X = pos.X + 13;
    camposMultiSeleccionPos[CAMPO_ADMIN].Y = pos.Y;
    camposMultiSeleccionPos[CAMPO_CAPTURISTA].X = camposMultiSeleccionPos[CAMPO_ADMIN].X + 17;
    camposMultiSeleccionPos[CAMPO_CAPTURISTA].Y = pos.Y;

    printf("  "); //Espacio para colocar el cursor
    printf("Administrador  ");
    printf("  ");
    printf("Capturista");

    pos.Y += 2;
    pos.X = Xcentro - 10;
    gotoxy(pos);
    printf("  ");
    printf("Cancelar  ");
    printf("  ");
    printf("Ok");
    camposMultiSeleccionPos[CAMPO_CANCELAR].X = pos.X;
    camposMultiSeleccionPos[CAMPO_CANCELAR].Y = pos.Y;
    camposMultiSeleccionPos[CAMPO_OK].X = pos.X + 12;
    camposMultiSeleccionPos[CAMPO_OK].Y = pos.Y;

    gotoxy(campoUsuarioPos);
    while ((cursor = getch()) != '\r' || salir == 0)
    {
        if (esFlecha)
        {
            movimientoDelCampo(&seleccion,cursor);

            switch (seleccion) {
            case CAMPO_USUARIO:
                gotoxy(campoUsuarioPos);
                salir = 0;
                break;
            case CAMPO_CONTRA:
                gotoxy(campoContraPos);
                salir = 0;
                break;
            case CAMPO_ADMIN:
                gotoxy(camposMultiSeleccionPos[CAMPO_CAPTURISTA]);
                printf(" ");
                gotoxy(camposMultiSeleccionPos[CAMPO_ADMIN]);
                printf("%c",CARACTER_CURSOR);
                nuevo.privilegio = ADMINISTRADOR;
                break;
            case CAMPO_CAPTURISTA:
                gotoxy(camposMultiSeleccionPos[CAMPO_ADMIN]);
                printf(" ");
                gotoxy(camposMultiSeleccionPos[CAMPO_CAPTURISTA]);
                printf("%c",CARACTER_CURSOR);
                nuevo.privilegio = CAPTURISTA;
                break;
            case CAMPO_OK:
                gotoxy(camposMultiSeleccionPos[CAMPO_CANCELAR]);
                printf(" ");
                gotoxy(camposMultiSeleccionPos[CAMPO_OK]);
                salir = CAMPO_OK;
                break;
            case CAMPO_CANCELAR:
                gotoxy(camposMultiSeleccionPos[CAMPO_OK]);
                printf(" ");
                gotoxy(camposMultiSeleccionPos[CAMPO_CANCELAR]);
                salir = CAMPO_CANCELAR;
                break;
            default:
                break;
            }

            esFlecha = false;
        }
        else if (isalnum(cursor))
        {
            switch (seleccion) {
            case CAMPO_USUARIO:
                registrarEntrada(nuevo.usuario,&contUsuario,&campoUsuarioPos.X,cursor,CAMPO_USUARIO);
                break;
            case CAMPO_CONTRA:
                registrarEntrada(nuevo.contra,&contContra,&campoContraPos.X,cursor,CAMPO_CONTRA);
                break;
            default:
                break;
            }
        }

        if (cursor == CODIGO_FLECHA) esFlecha = true;
    }

    if (seleccion == CAMPO_CANCELAR)
    {
        return;
    }
    else if (seleccion == CAMPO_OK)
    {
        FILE *Fichero; short v; char archivo[32];
        if (nuevo.privilegio == ADMINISTRADOR) strcpy(archivo,RUTA_ADMINISTRADOR);
        else if (nuevo.privilegio == CAPTURISTA) strcpy(archivo,RUTA_CAPTURISTA);

        Fichero = fopen(archivo,"a");
        if (Fichero == NULL)
        {
            system("cls");
            perror("Error al abrir el archivo\n");
            system("PAUSE");
            exit(-1);
        }

        v = fprintf(Fichero,"%s\t%s",nuevo.usuario,nuevo.contra);
        if (v < 1)
        {
            system("cls");
            perror("Error al escribir el archivo\n");
            system("PAUSE");
            exit(-1);
        }

        fclose(Fichero);
    }

    system("cls");
    return;
}

void RegistrarProveedor(){
    FILE *Fichero; Fichero = fopen("proveedores.txt","a");
    if (Fichero == NULL)
    {
        system("cls");
        perror("Error al abrir el archivo\n\n");
        system("PAUSE");
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
        system("cls");
        perror("Error al escribir al archivo\n\n");
        system("PAUSE");
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
