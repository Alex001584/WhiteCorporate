#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <windows.h>
#include <conio.h>

#include "librerias.h"
#include "estructuras.h"
#include "regUserLib.h"
#include "inicioSesion.h"
#include "prototipos.h"
#include "portada.h"

void MenuPrincipal(){
	HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // int sesion; bool bandera;

    // char opciones[][64] = {
    //     "Iniciar sesion como administrador",
    //     "Iniciar sesion como capturista",
    //     "Salir"
    // };

    // //Calculo dinamicamente la cantidad de cadenas en las opciones
    // const short sizeOpciones = sizeof(opciones) / sizeof(opciones[0]);

    // //Creo apuntadores para el arreglo de opciones
    // char *pOpciones[sizeOpciones];
    // for (size_t i = 0; i < sizeOpciones; i++)
    // {
    //     pOpciones[i] = opciones[i];
    // }

    // do{
    // 	SetConsoleTextAttribute(hConsole, 11); //El color asignado de este men� es el aguamarina

    //     sesion = menu("--- MENU PRINCIPAL ---", pOpciones,sizeOpciones); //Creo la interfaz del menu y retorno la seleccion del usuario

    //     switch(sesion){
    //         case 1:
    //             bandera = inicioSesion();
    //             if (bandera) MenuAdministrador();
    //             break;
    //         case 2:
    //             bandera = inicioSesion();
    //             if (bandera) MenuCapturista();
    //             break;
    //         case 3:
    //             printf("\nQu%c tenga un excelente d%ca :3\n", 130, 161);
    //             return;
    //         default:
    //             printf("\nEst%c opci%cn no existe, int%cntelo de nuevo\n", 160, 162, 130);
    //     }
    // }while(!bandera);
    
    SetConsoleTextAttribute(hConsole, 11); //El color asignado de este men� es el aguamarina

    short privilegio;
    do
    {
        privilegio = inicioSesion();
        if (privilegio == ADMIN) MenuAdministrador();
        else if (privilegio == CAPTU) MenuCapturista();
        else if (privilegio == -1)
        {
            system("cls");
            COORD pos; obtenerCentroConsola(&pos);
            imprimirStringMenuCentrado("--- USUARIO O CONTRA INCORRECTOS ---",&pos,0);
            Sleep(1000);
        }
    } while (privilegio == -1);
    

    system("cls");
    return;
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
        

        admin = menu("--- MENU DEL ADMINISTRADOR ---",pOpciones,sizeOpciones);

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

void RegistrarUsuario()
{
    COORD camposPosCursor[6];
    USUARIOS nuevo;
    strcpy(nuevo.usuario,"\0");
    strcpy(nuevo.contra,"\0");
    imprimirMenuRegistroUsuario(camposPosCursor);
    gotoxy(camposPosCursor[USUARIO]);
    

    char tecla;
    short campo = USUARIO, salida = CANCELAR; nuevo.privilegio = ADMIN;
    bool esFlecha = false;

    while ((tecla = _getch()) != '\r' || (campo != OK && campo != CANCELAR))
    {
        
        if (esFlecha || tecla == '\r')
        {
            manejoFlechasCursorRegUser(&campo,&nuevo.privilegio,&salida,tecla,camposPosCursor);
            esFlecha = false;
        }
        else if (isalnum(tecla) || tecla == '\b')
        {
            if (campo == USUARIO) lecturaDinamicaUsuarioContra(nuevo.usuario,tecla,USUARIO,camposPosCursor);
            else if (campo == CONTRA) lecturaDinamicaUsuarioContra(nuevo.contra,tecla,CONTRA,camposPosCursor);
        }

        if (tecla == CODIGO_FLECHA2) esFlecha = true;
    }
    
    if (campo == CANCELAR)
    {
        system("cls"); COORD pos;
        imprimirStringMenuCentrado("--- OPERACION CANCELADA ---",&pos,0);
        Sleep(100);
    }
    else if (campo == OK)
    {
        FILE *Fichero;
        int v;

        Fichero = fopen("usuarios.txt","a");
        if (Fichero == NULL)
        {
            system("cls");
            perror("Error al abrir el archivo\n\n");
            system("PAUSE");
            exit(-1);
        }

        v = fprintf(Fichero,"\n%s\t%s\t%i",nuevo.usuario,nuevo.contra,nuevo.privilegio);
        if (v < 1)
        {
            system("cls");
            perror("Error al escribir el archivo\n\n");
            system("PAUSE");
            exit(-1);
        }
        else
        {
            system("cls"); COORD pos;
            imprimirStringMenuCentrado("--- USUARIO CREADO EXITOSAMENTE ---",&pos,0);
            Sleep(100);
        }
    }

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
