#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#include "librerias.h"
#include "estructuras.h"
#include "regUserLib.h"
#include "inicioSesion.h"
#include "listasLib.h"
#include "prototipos.h"
#include "portada.h"

void MenuPrincipal(){
	HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
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
            imprimirStringCentrado("--- USUARIO O CONTRA INCORRECTOS ---",&pos,0);
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
    	
        char opciones[][64] = {
            "Registrar nuevo usuario",
            "Registrar proveedor",
            "Registrar materia prima",
            "Registrar modelo de producto",
            "Registrar pedido de materia prima",
            "Regresar al inicio de sesion"
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
                RegistrarModelos();
                break;

            case 5:
                PedidoMateriaPrima();
                break;
            
            case 6:
                MenuPrincipal();
                break;
            default:
                break;
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
        imprimirStringCentrado("--- OPERACION CANCELADA ---",&pos,0);
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
            imprimirStringCentrado("--- USUARIO CREADO EXITOSAMENTE ---",&pos,0);
            Sleep(100);
        }
    }

    return;
}

void RegistrarProveedor(){
    FILE *Fichero; Fichero = fopen("proveedores.txt","r+");
    if (Fichero == NULL)
    {
        system("cls");
        perror("Error al abrir el archivo\n\n");
        system("PAUSE");
        exit(-1);
    }

    PROVEEDOR nuevo; int v; bool bandera;
    printf("\n--- NUEVO PROVEEDOR ---\n");
    
    fscanf(Fichero,"%i",&nuevo.id);
    fseek(Fichero,0,SEEK_SET);
    fprintf(Fichero,"%i",(nuevo.id+1));
    fseek(Fichero,0,SEEK_END);

    printf("\nID: %i\n",++nuevo.id);

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

    printf("\n\n");
    system("PAUSE");
    system("cls");
    fclose(Fichero);
    return;
}

void RegistrarMatePrim(){
    FILE *Fichero; Fichero = fopen("materias_primas.txt","r+");
    if (Fichero == NULL)
    {
        perror("\nError al abrir el archivo\n\n");
        exit(-1);
    }

    MATERIA_PRIMA nuevo; int v,x;
    printf("\n--- NUEVA MATERIA PRIMA ---\n");
    
    fscanf(Fichero,"%i",&nuevo.id);
    fseek(Fichero,0,SEEK_SET);
    fprintf(Fichero,"%i",(++nuevo.id));
    fseek(Fichero,0,SEEK_END);
    
    printf("\nID: %i\n",nuevo.id);

    printf("\nNombre: ");
    fgets(nuevo.nombre,50,stdin);
    nuevo.nombre[strlen(nuevo.nombre)-1] = '\0';

    do
    {
        printf("\nCantidad (kg): ");
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

    printf("\n\n");
    system("PAUSE");
    system("cls");
    fclose(Fichero);
    return;
}

void RegistrarModelos() {
    FILE *Fichero = fopen("modelos.txt","r+");
    if (Fichero == NULL)
    {
        system("cls");
        perror("\n\nError al abrir el archivo\n\n");
        exit(-1);
    }

    MODELO_PRODUCTO nuevo; int v = 0,x; bool bandera; 

    COORD pos;
    obtenerCentroConsola(&pos);
    pos.Y -= 3;

    imprimirStringCentrado("--- NUEVO MODELO ---",&pos,0);
    const short Xinicial = pos.X;

    //ID
    fscanf(Fichero,"%i",&nuevo.id);
    fseek(Fichero,0,SEEK_SET);
    fprintf(Fichero,"%i",(++nuevo.id));
    fseek(Fichero,0,SEEK_END);
    pos.Y += 2;
    gotoxy(pos);
    printf("ID: %i",nuevo.id);
    
    //Nombre
    pos.X = Xinicial;
    pos.Y += 2;
    gotoxy(pos);
    printf("Nombre: ");
    fgets(nuevo.nombre,50,stdin);
    nuevo.nombre[strlen(nuevo.nombre)-1] = '\0';

    //ID materia prima
    pLista_Materia_Prima lista_MateriaPrima, cabeza;
    obtenerMateriasPrimas(&lista_MateriaPrima);

    bool esIDValida = false;
    pos.Y += 2;
    pos.X = Xinicial;
    gotoxy(pos);
    printf("ID materia prima: ");
    pos.X += 18;
    COORD input;
    input.X = pos.X;
    input.Y = pos.Y;
    do
    {
        v = scanf("%i",&nuevo.id_materia_prima);
        while ((x = getchar()) != '\n' && x != EOF);

        if (v == 1 || nuevo.id_materia_prima >= 1) bandera = true;
        else
        {
            bandera = false;
            gotoxy(input);
            for (size_t i = 0; i < 25; i++) printf(" ");
        }

        cabeza = lista_MateriaPrima;
        while (cabeza != NULL)
        {
            if (cabeza->materia_prima.id == nuevo.id_materia_prima)
            {
                esIDValida = true;
                pos.Y += 2;
                pos.X = 0;
                gotoxy(pos);
                for (size_t i = 0; i < 200; i++) printf(" ");
                pos.X = Xinicial;
                gotoxy(pos);
                printf("Nombre de materia: %s",cabeza->materia_prima.nombre);
                pos.Y++;
                gotoxy(pos);
                printf("Cantidad (kg): %0.2f",cabeza->materia_prima.cantidad);
                break;
            }
            else cabeza = cabeza->siguiente;
        }

        if (!esIDValida)
        {
            imprimirStringCentrado("--- ID de material invalida ---",&pos,2);
            pos.Y -= 2;
            gotoxy(input);
            for (size_t i = 0; i < 25; i++) printf(" ");
            gotoxy(input);
        }

    } while (!bandera || !esIDValida);

    v = fprintf(Fichero,"%i\t%s\t%i\n",
        nuevo.id,
        nuevo.nombre,
        nuevo.id_materia_prima
    );
    if (v < 1)
    {
        system("cls");
        perror("Error al escribir el archivo");
        printf("\n\n");
        system("PAUSE");
        exit(-1);
    }

    liberarMateriasPrimas(&lista_MateriaPrima);
    fclose(Fichero);

    imprimirStringCentrado("  --- MODELO GUARDADO CORRECTAMENTE ---  ",&pos,2);
    imprimirStringCentrado("Presiona cualquier tecla para continuar",&pos,2);
    getch();
    system("cls");
    return;
}

void PedidoMateriaPrima(){
	
    FILE *Fichero = fopen("pedidos.txt","r+");
    if (Fichero == NULL)
    {
        system("cls");
        perror("Error al abrir el archivo");
        printf("\n\n");
        system("PAUSE");
        exit(-1);
    }

    /* <--(18)-->
                    --- NUEVO PEDIDO ---

        ID: 123132

        ID Proveedor:
        Nombre Proveedor:
        Direccion Proveedor:
        Telefono Proveedor:

        ID Materia Prima: 
        Nombre Materia:
        Cantidad de Materia:

        Cantidad a pedir (kg):

        Costo:

        Fecha del Pedido: DD/MM/AAAA

        Fecha de Entrega (DD/MM/AAAA):
    */

    PEDIDO nuevo;
    COORD pos;
    obtenerCentroConsola(&pos);
    const short Xinicial = pos.X - 18;
    pos.Y -= 9;

    imprimirStringCentrado("--- NUEVO PEDIDO ---",&pos,0);

    //ID
    fscanf(Fichero,"%i",&nuevo.id);
    fseek(Fichero,0,SEEK_SET);
    fprintf(Fichero,"%i",(++nuevo.id));
    fseek(Fichero,0,SEEK_END);
    pos.Y += 2;
    pos.X = Xinicial;
    gotoxy(pos);
    printf("ID: %i",nuevo.id);

    //ID proveedor
    pLista_Proveedor proveedores;
    obtenerProveedores(&proveedores);

    pos.Y += 2;
    gotoxy(pos);
    printf("ID Proveedor: ");
    pos.X += 14;
    int v,x; bool entradaValida, existeID;
    do
    {
        v = scanf("%i",&nuevo.id_proveedor);
        while ((x = getchar()) != '\n' && x != EOF);

        if (v != 1 || nuevo.id_proveedor < 1)
        {
            imprimirStringCentrado("--- ID invalida ---",&pos,1);
            pos.Y--;
            pos.X = Xinicial + 14;
            gotoxy(pos);
            for (size_t i = 0; i < 50; i++) printf(" ");
            gotoxy(pos);
            entradaValida = false;
        }
        else entradaValida = true;

        pLista_Proveedor cabeza = proveedores;
        while (cabeza != NULL)
        {
            if (cabeza->proveedor.id == nuevo.id_proveedor)
            {
                pos.X = 0;
                pos.Y++;
                gotoxy(pos);
                for (size_t i = 0; i < 100; i++) printf(" ");
                pos.X = Xinicial;
                gotoxy(pos);
                printf("Nombre Proveedor: %s                           ",cabeza->proveedor.nombre);
                pos.Y++;
                gotoxy(pos);
                printf("Direccion Proveedor: %s",cabeza->proveedor.direccion);
                pos.Y++;
                gotoxy(pos);
                printf("Telefono Proveedor: %s",cabeza->proveedor.telefono);
                existeID = true;
                break;
            }
            else existeID = false;
            cabeza = cabeza->siguiente;
        }

        if (!existeID)
        {
            imprimirStringCentrado("--- Ningun proveedor con esta ID ---",&pos,1);
            pos.Y--;
            pos.X = Xinicial + 14;
            gotoxy(pos);
            for (size_t i = 0; i < 50; i++) printf(" ");
            gotoxy(pos);
        }

    } while (!entradaValida || !existeID);
    liberarProveedores(&proveedores);

    //ID materia prima
    pLista_Materia_Prima lista_MateriaPrima, cabeza;
    long cant_materias_primas = obtenerMateriasPrimas(&lista_MateriaPrima);

    pos.Y += 2;
    pos.X = Xinicial;
    gotoxy(pos);
    printf("ID materia prima: ");
    pos.X += 18;
    COORD input;
    input.X = pos.X;
    input.Y = pos.Y;
    do
    {
        v = scanf("%i",&nuevo.id_materia_prima);
        while ((x = getchar()) != '\n' && x != EOF);

        if (v == 1 || nuevo.id_materia_prima >= 1) entradaValida = true;
        else
        {
            entradaValida = false;
            gotoxy(input);
            for (size_t i = 0; i < 25; i++) printf(" ");
        }

        cabeza = lista_MateriaPrima;
        while (cabeza != NULL)
        {
            if (cabeza->materia_prima.id == nuevo.id_materia_prima)
            {
                existeID = true;
                pos.Y++;
                pos.X = 0;
                gotoxy(pos);
                for (size_t i = 0; i < 100; i++) printf(" ");
                pos.X = Xinicial;
                gotoxy(pos);
                printf("Nombre de materia: %s",cabeza->materia_prima.nombre);
                pos.Y++;
                gotoxy(pos);
                printf("Cantidad (kg): %0.2f",cabeza->materia_prima.cantidad);
                break;
            }
            else
            {
                existeID = false;
                cabeza = cabeza->siguiente;
            }
        }

        if (!existeID)
        {
            imprimirStringCentrado("--- ID de material invalida ---",&pos,2);
            pos.Y -= 2;
            gotoxy(input);
            for (size_t i = 0; i < 25; i++) printf(" ");
            gotoxy(input);
        }

    } while (!entradaValida || !existeID);

    //Cantidad
    pos.X = Xinicial;
    pos.Y += 2;
    gotoxy(pos);
    printf("Cantidad a pedir (kg):                                                                 ");
    pos.X += 23;
    gotoxy(pos);
    do
    {
        v = scanf("%f",&nuevo.cantidad);
        while ((x = getchar()) != '\n' && x != EOF);

        if (v != 1 || nuevo.cantidad < 0)
        {
            gotoxy(pos);
            for (size_t i = 0; i < 25; i++) printf(" ");
            gotoxy(pos);
            entradaValida = false;
        } else entradaValida = true;

        if (entradaValida == true)
        {
            if (nuevo.cantidad > cabeza->materia_prima.cantidad)
            {
                imprimirStringCentrado("--- Cantidad insuficiente de materia ---",&pos,2);
                pos.Y-=2;
                pos.X = Xinicial + 23;
                gotoxy(pos);
                for (size_t i = 0; i < 50; i++) printf(" ");
                gotoxy(pos);
                entradaValida = false;
            }
        }

    } while (!entradaValida);
    cabeza->materia_prima.cantidad -= nuevo.cantidad;
    escribirMateriasPrimas(lista_MateriaPrima,cant_materias_primas);
    liberarMateriasPrimas(&lista_MateriaPrima);

    //Costo
    pos.X = 0;
    pos.Y += 2;
    gotoxy(pos);
    for (size_t i = 0; i < 100; i++) printf(" ");
    
    pos.X = Xinicial;
    gotoxy(pos);
    printf("Costo: ");
    pos.X += 7;
    do
    {
        v = scanf("%f",&nuevo.monto_total);
        while ((x = getchar()) != '\n' && x != EOF);

        if (v != 1 || nuevo.monto_total < 0)
        {
            gotoxy(pos);
            for (size_t i = 0; i < 25; i++) printf(" ");
            gotoxy(pos);
            entradaValida = false;
        } else entradaValida = true;

    } while (!entradaValida);

    //Fecha de pedido
    time_t fechaActual = time(NULL);
    tm *fecha = localtime(&fechaActual);

    nuevo.fecha_pedido.dia = fecha->tm_mday;
    nuevo.fecha_pedido.mes = (fecha->tm_mon+1);
    nuevo.fecha_pedido.anio = (fecha->tm_year+1900);

    pos.X = Xinicial;
    pos.Y += 2;
    gotoxy(pos);
    printf("Fecha del pedido: %i/%.2i/%i",nuevo.fecha_pedido.dia,nuevo.fecha_pedido.mes,nuevo.fecha_pedido.anio);

    //Fecha de entrega
    pos.X = Xinicial;
    pos.Y += 2;
    gotoxy(pos);
    printf("Fecha de entrega (DD/MM/AAAA): ");
    pos.X += 31;

    tm fecha2 = {};
    tm fecha3 = {};
    fecha3.tm_mday = nuevo.fecha_pedido.dia;
    fecha3.tm_mon = nuevo.fecha_pedido.mes - 1;
    fecha3.tm_year = nuevo.fecha_pedido.anio - 1900;

    do
    {
        v = scanf("%hi/%hi/%hi",&nuevo.fecha_entrega.dia,&nuevo.fecha_entrega.mes,&nuevo.fecha_entrega.anio);
        while ((x = getchar()) != '\n' && x != EOF);

        if (v < 1
            || nuevo.fecha_entrega.dia < 1 || nuevo.fecha_entrega.dia > 31
            || nuevo.fecha_entrega.mes < 1 || nuevo.fecha_entrega.mes > 12 
            || nuevo.fecha_entrega.anio < 2000 || nuevo.fecha_entrega.anio > 3000
        )
        {
            gotoxy(pos);
            for (size_t i = 0; i < 25; i++) printf(" ");
            gotoxy(pos);
            entradaValida = false;
        } else entradaValida = true;

        if (entradaValida == true)
        {
                    
            fecha2.tm_mday = nuevo.fecha_entrega.dia;
            fecha2.tm_mon = nuevo.fecha_entrega.mes - 1;
            fecha2.tm_year = nuevo.fecha_entrega.anio - 1900;

            time_t tiempo1 = mktime(&fecha3);
            time_t tiempo2 = mktime(&fecha2);
            
            double diferencia = difftime(tiempo1,tiempo2);

            if (diferencia > 0)
            {
                imprimirStringCentrado("--- Fecha es anterior a el pedido ---",&pos,2);
                pos.Y-=2;
                pos.X = Xinicial + 31;
                gotoxy(pos);
                for (size_t i = 0; i < 100; i++) printf(" ");
                gotoxy(pos);
                entradaValida = false;
            }
        }

    } while (!entradaValida);

    //Guardando en archivo
    v = fprintf(Fichero,"%i\t%i\t%i\t%0.2f\t%0.2f\t%i\t%i\t%i\t%i\t%i\t%i\n",
        nuevo.id,
        nuevo.id_proveedor,
        nuevo.id_materia_prima,
        nuevo.cantidad,
        nuevo.monto_total,
        nuevo.fecha_pedido.dia,
        nuevo.fecha_pedido.mes,
        nuevo.fecha_pedido.anio,
        nuevo.fecha_entrega.dia,
        nuevo.fecha_entrega.mes,
        nuevo.fecha_entrega.anio
    );
    if (v < 1)
    {
        system("cls");
        perror("Error al escribir al archivo");
        printf("\n\n");
        system("PAUSE");
        exit(-1);
    }

    fclose(Fichero);
    imprimirStringCentrado("--- PEDIDO ESCRITO EXITOSAMENTE ---",&pos,2);
    imprimirStringCentrado("Presiona cualquier tecla para continuar",&pos,2);
    getch();
    system("cls");
    return;
}

void MenuCapturista(){
	HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    int capturista;
    
    do{
    	SetConsoleTextAttribute(hConsole, 4); //El color asignado de este men� es el rojo

        char opciones[][64] = {
            "Generar reporte de modelos por materia prima",
            "Generar reporte del total de material en inventario",
            "Generar reporte de pedidos de materia prima a proveedores",
            "Generar reporte en archivos xls",
            "Volver al inicio de sesion"
        };

        const short sizeOpciones = sizeof(opciones) / sizeof(opciones[0]);

        char** pOpciones = (char**)calloc(sizeOpciones,sizeof(char*));
        for (size_t i = 0; i < sizeOpciones; i++)
        {
            pOpciones[i] = opciones[i];
        }
        

        capturista = menu("--- MENU DEL CAPTURISTA ---",pOpciones,sizeOpciones);

        switch(capturista){
            case 1:
                ReporteModelos();
                break;
                
            case 2:
                ReporteInventario();
                break;
                
            case 3:
                PedidosMatPriProveedores();
                break;
                
            case 4:
                GenerarArchivosReporte();
                break;
            
            case 5:
                MenuPrincipal();
                break;

            default:
                break;
        }
    } while(capturista != 5);
}

void ReporteModelos(){
	
    FILE *Fichero = fopen("modelos.txt","r");
    if (Fichero == NULL)
    {
        system("cls");
        perror("\nError al abrir el archivo\n\n");
        system("PAUSE");
        exit(-1);
    }

    int v,cant_modelos;
    v = fscanf(Fichero,"%i",&cant_modelos);
    if (v < 1)
    {
        system("cls");
        perror("\nError al leer el archivo\n\n");
        system("PAUSE");
        exit(-1);
    }

    COORD pos; MODELO_PRODUCTO producto; pLista_Materia_Prima materias_primas;
    pos.X = 2;
    pos.Y = 2;
    gotoxy(pos);

    printf("%c",218);
    for (size_t i = 0; i < 110; i++) printf("%c",196);
    printf("%c",191);

    pos.Y++;
    gotoxy(pos);
    printf("%c\tID\tNombre del modelo\tID Materia\tNombre Materia\tCantidad de Materia",179);
    pos.X = 113;
    gotoxy(pos);
    printf("%c",179);

    pos.X = 2;
    pos.Y++;
    gotoxy(pos);
    printf("%c",195);
    for (size_t i = 0; i < 110; i++) printf("%c",196);
    printf("%c",180);

    obtenerMateriasPrimas(&materias_primas);
    pLista_Materia_Prima cabeza;

    while (!feof(Fichero) && v > 0)
    {
        v = fscanf(Fichero,"%i\t%[^\t]\t%i",&producto.id,producto.nombre,&producto.id_materia_prima);
        cabeza = materias_primas;
        while (cabeza != NULL && v > 0)
        {
            if (cabeza != NULL)
            {
                if (cabeza->materia_prima.id == producto.id_materia_prima) break;
                else cabeza = cabeza->siguiente;
            }
        }

        if (v > 0)
        {
            pos.X = 2;
            pos.Y++;
            gotoxy(pos);
            printf("%c\t%i\t%s\t%i\t%s\t%.2f",179,producto.id,producto.nombre,producto.id_materia_prima,cabeza->materia_prima.nombre,cabeza->materia_prima.cantidad);
            pos.X = 113;
            gotoxy(pos);
            printf("%c",179);
        }
    }
    
    
    pos.Y++;
    pos.X = 2;
    gotoxy(pos);
    printf("%c",192);
    for (size_t i = 0; i < 110; i++) printf("%c",196);
    printf("%c",217);
    
    fclose(Fichero);
    imprimirStringCentrado("Presiona cualquier tecla para volver al menu",&pos,2);
    getch();
    system("cls");
    return;
}

void ReporteInventario(){
	
    FILE *Fichero = fopen("materias_primas.txt","r");
    if (Fichero == NULL)
    {
        system("cls");
        perror("\nError al abrir el archivo\n\n");
        system("PAUSE");
        exit(-1);
    }

    COORD pos; int v, cant_materias; MATERIA_PRIMA cabeza;
    obtenerCentroConsola(&pos);
    const short Xcentro = pos.X;

    pos.Y = 2;
    pos.X -= 27;
    gotoxy(pos);

    printf("%c",218);
    for (size_t i = 0; i < 52; i++) printf("%c",196);
    printf("%c",191);
    
    pos.Y++;
    gotoxy(pos);
    printf("%c\tID\tNombre\t\tCantidad (kg)",179);
    pos.X = Xcentro + 26;
    gotoxy(pos);
    printf("%c",179);
    
    pos.X = Xcentro - 27;
    pos.Y++;
    gotoxy(pos);
    printf("%c",195);
    for (size_t i = 0; i < 52; i++) printf("%c",196);
    printf("%c",180);

    
    v = fscanf(Fichero,"%i",&cant_materias);
    if (v < 1)
    {
        system("cls");
        perror("\nError al leer el archivo\n\n");
        system("PAUSE");
        exit(-1);
    }    
    fscanf(Fichero,"%i \t %[^\t] \t %f",&cabeza.id,cabeza.nombre,&cabeza.cantidad);
    
    pos.Y++;
    gotoxy(pos);
    printf("%c\t%i\t%s\t\t%.2f",179,cabeza.id,cabeza.nombre,cabeza.cantidad);
    pos.X = Xcentro + 26;
    gotoxy(pos);
    printf("%c",179);

    while (!feof(Fichero) && v > 0)
    {
        v = fscanf(Fichero,"%i \t %[^\t] \t %f",&cabeza.id,cabeza.nombre,&cabeza.cantidad);
        if (v > 0)
        {
            pos.Y++;
            pos.X = Xcentro - 27;
            gotoxy(pos);
            printf("%c\t%i\t%s\t\t%.2f",179,cabeza.id,cabeza.nombre,cabeza.cantidad);
            pos.X = Xcentro + 26;
            gotoxy(pos);
            printf("%c",179);
        }
    }

    pos.Y++;
    pos.X = Xcentro - 27;
    gotoxy(pos);
    printf("%c",192);
    for (size_t i = 0; i < 52; i++) printf("%c",196);
    printf("%c",217);
    
    fclose(Fichero);
    imprimirStringCentrado("Presione cualquier tecla para volver al menu",&pos,2);
    getch();
    system("cls");
    return;
}

void PedidosMatPriProveedores(){
	
    FILE *Fichero = fopen("pedidos.txt","r");
    if (Fichero == NULL)
    {
        system("cls");
        perror("\nError al abrir el archivo\n\n");
        system("PAUSE");
        exit(-1);
    }

    int v,cant_pedidos;
    v = fscanf(Fichero,"%i",&cant_pedidos);
    if (v < 1)
    {
        system("cls");
        perror("\nError al leer el archivo\n\n");
        system("PAUSE");
        exit(-1);
    }

    COORD pos; PEDIDO pedidos; pLista_Materia_Prima materias_primas; pLista_Proveedor proveedores;
    pos.X = 2;
    pos.Y = 2;
    gotoxy(pos);

    printf("%c",218);
    for (size_t i = 0; i < 150; i++) printf("%c",196);
    printf("%c",191);

    pos.Y++;
    gotoxy(pos);
    printf("%c\tID\tCantidad\tCosto\tFechaPedido\tFechaEntrega\tID Proveedor,Nombre,Direccion,Telefono\tID Materias,Nombre,Cantidad",179);
    pos.X = 153;
    gotoxy(pos);
    printf("%c",179);

    pos.X = 2;
    pos.Y++;
    gotoxy(pos);
    printf("%c",195);
    for (size_t i = 0; i < 150; i++) printf("%c",196);
    printf("%c",180);

    obtenerProveedores(&proveedores);
    obtenerMateriasPrimas(&materias_primas);
    pLista_Materia_Prima cabeza_materias;
    pLista_Proveedor cabeza_proveedores;

    while (!feof(Fichero) && v > 0)
    {
        v = fscanf(Fichero,"%i\t%i\t%i\t%f\t%f\t%hi\t%hi\t%hi\t%hi\t%hi\t%hi",
            &pedidos.id,
            &pedidos.id_proveedor,
            &pedidos.id_materia_prima,
            &pedidos.cantidad,
            &pedidos.monto_total,
            &pedidos.fecha_pedido.dia,
            &pedidos.fecha_pedido.mes,
            &pedidos.fecha_pedido.anio,
            &pedidos.fecha_entrega.dia,
            &pedidos.fecha_entrega.mes,
            &pedidos.fecha_entrega.anio
        );

        cabeza_materias = materias_primas;
        while (cabeza_materias != NULL && v > 0)
        {
            if (cabeza_materias != NULL)
            {
                if (cabeza_materias->materia_prima.id == pedidos.id_materia_prima) break;
                else cabeza_materias = cabeza_materias->siguiente;
            }
        }

        cabeza_proveedores = proveedores;
        while (cabeza_proveedores != NULL && v > 0)
        {
            if (cabeza_proveedores != NULL)
            {
                if (cabeza_proveedores->proveedor.id == pedidos.id_proveedor) break;
                else cabeza_proveedores = cabeza_proveedores->siguiente;
            }
        }

        if (v > 0)
        {
            pos.X = 2;
            pos.Y++;
            gotoxy(pos);
            printf("%c\t%i\t%.2f\t%.2f\t%.2i/%.2i/%.2i\t%.2i/%.2i/%.2i\t%i,%s,%s,%s\t%i,%s,%.2f",
                179,

                pedidos.id,
                pedidos.cantidad,
                pedidos.monto_total,

                pedidos.fecha_pedido.dia,
                pedidos.fecha_pedido.mes,
                pedidos.fecha_pedido.anio,

                pedidos.fecha_entrega.dia,
                pedidos.fecha_entrega.mes,
                pedidos.fecha_entrega.anio,

                cabeza_proveedores->proveedor.id,
                cabeza_proveedores->proveedor.nombre,
                cabeza_proveedores->proveedor.direccion,
                cabeza_proveedores->proveedor.telefono,

                cabeza_materias->materia_prima.id,
                cabeza_materias->materia_prima.nombre,
                cabeza_materias->materia_prima.cantidad

            );
            pos.X = 153;
            gotoxy(pos);
            printf("%c",179);
        }
    }
    
    
    pos.Y++;
    pos.X = 2;
    gotoxy(pos);
    printf("%c",192);
    for (size_t i = 0; i < 150; i++) printf("%c",196);
    printf("%c",217);
    
    fclose(Fichero);
    imprimirStringCentrado("Presiona cualquier tecla para volver al menu",&pos,2);
    getch();
    system("cls");
    return;
}

void GenerarArchivosReporte()
{
    system("cls");
    COORD pos;
    obtenerCentroConsola(&pos);
    pos.Y-=2;
    FILE *Fichero;
    imprimirStringCentrado("Generando archivos...",&pos,0);

    //Productos
    pLista_Modelo_Producto productos;
    pLista_Materia_Prima materias_primas;
    obtenerProductos(&productos);
    obtenerMateriasPrimas(&materias_primas);
    
    Fichero = fopen("reporteProductos.xls","w+");
    if (Fichero == NULL)
    {
        system("cls");
        perror("\nError al abrir/crear el archivo\n\n");
        system("PAUSE");
        exit(-1);
    }

    int v;
    v = fprintf(Fichero,"ID\tNombre del modelo\tID Materia\tNombre Materia\tCantidad de Materia\n");
    if (v < 1)
    {
        system("cls");
        perror("\nError al escribir al archivo\n\n");
        system("PAUSE");
        exit(-1);
    }

    pLista_Modelo_Producto primero = productos;
    pLista_Materia_Prima cabeza;
    do
    {
        cabeza = materias_primas;
        while (cabeza != NULL)
        {
            if (cabeza->materia_prima.id == productos->modelos.id_materia_prima) break;
            else cabeza = cabeza->siguiente;
        }

        v = fprintf(Fichero,"%i\t%s\t%i\t%s\t%.2f\n",productos->modelos.id,productos->modelos.nombre,productos->modelos.id_materia_prima,cabeza->materia_prima.nombre,cabeza->materia_prima.cantidad);

        productos = productos->siguiente;
    } while (productos != primero);
    
    liberarProductos(&productos);
    fclose(Fichero);

    //Total de materia en inventario
    Fichero = fopen("reporteInventario.xls","w+");
    if (Fichero == NULL)
    {
        system("cls");
        perror("\nError al abrir/crear el archivo\n\n");
        system("PAUSE");
        exit(-1);
    }

    v = fprintf(Fichero,"ID\tNombre\t\tCantidad (kg)\n");
    if (v < 1)
    {
        system("cls");
        perror("\nError al escribir al archivo\n\n");
        system("PAUSE");
        exit(-1);
    }

    cabeza = materias_primas;
    while (cabeza != NULL)
    {
        fprintf(Fichero,"%i\t%s\t\t%.2f\n",cabeza->materia_prima.id,cabeza->materia_prima.nombre,cabeza->materia_prima.cantidad);
        cabeza = cabeza->siguiente;
    }
    fclose(Fichero);

    //Pedidos de materia prima
    Fichero = fopen("reportePedidos.xls","w+");
    if (Fichero == NULL)
    {
        system("cls");
        perror("\nError al abrir/crear el archivo\n\n");
        system("PAUSE");
        exit(-1);
    }

    v = fprintf(Fichero,"ID\tCantidad\tCosto\tFechaPedido\tFechaEntrega\tID Proveedor\tNombre\tDireccion\tTelefono\tID Materias\tNombre\tCantidad\n");
    if (v < 1)
    {
        system("cls");
        perror("\nError al escribir al archivo\n\n");
        system("PAUSE");
        exit(-1);
    }

    pLista_Proveedor proveedores;
    pLista_Pedido pedidos;
    obtenerProveedores(&proveedores);
    obtenerPedidos(&pedidos);

    pLista_Pedido primero_pedidos = pedidos;
    do
    {
        pLista_Materia_Prima cabeza_materias = materias_primas;
        while (cabeza_materias != NULL && v > 0)
        {
            if (cabeza_materias != NULL)
            {
                if (cabeza_materias->materia_prima.id == pedidos->pedido.id_materia_prima) break;
                else cabeza_materias = cabeza_materias->siguiente;
            }
        }

        pLista_Proveedor cabeza_proveedores = proveedores;
        while (cabeza_proveedores != NULL && v > 0)
        {
            if (cabeza_proveedores != NULL)
            {
                if (cabeza_proveedores->proveedor.id == pedidos->pedido.id_proveedor) break;
                else cabeza_proveedores = cabeza_proveedores->siguiente;
            }
        }

        fprintf(Fichero,"%i\t%.2f\t%.2f\t%.2i/%.2i/%.2i\t%.2i/%.2i/%.2i\t%i\t%s\t%s\t%s\t%i\t%s\t%.2f\n",

            pedidos->pedido.id,
            pedidos->pedido.cantidad,
            pedidos->pedido.monto_total,

            pedidos->pedido.fecha_pedido.dia,
            pedidos->pedido.fecha_pedido.mes,
            pedidos->pedido.fecha_pedido.anio,

            pedidos->pedido.fecha_entrega.dia,
            pedidos->pedido.fecha_entrega.mes,
            pedidos->pedido.fecha_entrega.anio,

            cabeza_proveedores->proveedor.id,
            cabeza_proveedores->proveedor.nombre,
            cabeza_proveedores->proveedor.direccion,
            cabeza_proveedores->proveedor.telefono,

            cabeza_materias->materia_prima.id,
            cabeza_materias->materia_prima.nombre,
            cabeza_materias->materia_prima.cantidad

        );

        pedidos = pedidos->siguiente;
    } while (pedidos != primero_pedidos);

    liberarPedidos(&pedidos);
    liberarProveedores(&proveedores);
    liberarMateriasPrimas(&materias_primas);
    fclose(Fichero);

    imprimirStringCentrado("Archivos Generados exitosamente",&pos,1);
    imprimirStringCentrado("Presione una tecla para continuar",&pos,2);
    _getch();
    return;
}

int main() {
    
    imprimirPortada();
    system("cls");

    system("cls");
    MenuPrincipal();

    return 0;
}
