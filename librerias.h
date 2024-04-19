#include <stdio.h>
#include <windows.h>
#include <conio.h>

//Definiciones para mejor lectura del codigo
#define RUTA_ADMINISTRADOR "administradores.txt"
#define RUTA_CAPTURISTA "capturistas.txt"
#define CARACTER_BLOQUE_PARA_DIBUJO 219
#define CARACTER_CURSOR 175
#define CODIGO_FLECHA 224
#define CODIGO_FLECHA2 -32
#define FLECHA_ARRIBA 72
#define FLECHA_ABAJO 80
#define FLECHA_IZQUIERDA 75
#define FLECHA_DERECHA 77

void caracterPrint(const char cadena[])
{
    for (int i = 0; i < strlen(cadena); i++)
    {
       printf("%c",cadena[i]);
       Sleep(50);
    }
}

void gotoxy(COORD coordenadas)
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coordenadas);
    return;
}

void obtenerCentroConsola(COORD *coordenadas)
{
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo);

    coordenadas->X = ((bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1)/2);
    coordenadas->Y = (((bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top)/2)-1);
    
    return;
}

COORD imprimirStringCentrado(char **string, const short filas, bool esLento)
{
    COORD pos;
    obtenerCentroConsola(&pos);
    
    int inicialX = pos.X;
    pos.Y -= (filas/2);
    
    for (size_t i = 0; i < filas; i++)
    {
        pos.X -= (strlen(string[i])/2);
        // if (strlen(string[i]) % 2 == 0) pos.X--;
        
        gotoxy(pos);

        if (esLento) caracterPrint(string[i]);
        else printf("%s",string[i]);

        pos.X = inicialX;
        pos.Y++;
    }
    return pos;
}

int menu(const char titulo[], char** opciones, int cantidadOpciones)
{
    //Obtengo el centro de la consola
    COORD pos;
    obtenerCentroConsola(&pos);
    int centroX = pos.X; //Guardo la posicion centro de X

    //Le resto la mitad de la cantidad de cadenas que voy a imprimir para que queden centradas
    pos.Y -= (cantidadOpciones+2)/2;

    //Ajusto la posicion del cursor para imprimir centrado el titulo
    pos.X = centroX - ( strlen(titulo) / 2 );
    gotoxy(pos);
    caracterPrint(titulo);
    pos.Y+=2;

    //Imprimo de manera centrada todas las opciones
    for (size_t i = 0; i < cantidadOpciones; i++)
    {
        pos.X = centroX - ( strlen(opciones[i]) / 2 );
        gotoxy(pos);
        printf("%s",opciones[i]);
        pos.Y++;
    }
    
    //Posiciono el cursor en la primera opcion
    pos.Y -= cantidadOpciones;
    pos.X = centroX - ( ( strlen(opciones[0]) / 2 ) + 2 );
    gotoxy(pos);
    printf("%c ",CARACTER_CURSOR);
    

    int cursor, seleccion = 1; bool esFlechaDelTeclado = false; //Inicializo las variables
    do
    {
        /*
        Cuando el usuario presiona una flecha _getch retorna primero el codigo 224 para indicar que es una flecha del teclado
        Despues retorna el codigo de cual flecha fue presionada

        Primero capturo el codigo que indica que una flecha fue presionada,
        Despues con switch ejecuto el codigo segun que direccional fue presionada
        */
        if (esFlechaDelTeclado)
        {
            switch (cursor) {
            case FLECHA_ARRIBA: //Arriba
                if (seleccion > 1)
                {
                    gotoxy(pos); //Reutilizo las variables de posicion que use para colocar el cursor, asi que no tengo que redifinirlas
                    printf("  "); //Elimino el cursor para colocarlo en la nueva opcion seleccionada
                    seleccion--;

                    //Coloco el cursor en la opcion que el usuario desea seleccionar
                    pos.X = centroX - ( ( strlen(opciones[seleccion - 1]) / 2 ) + 2 );
                    pos.Y--;
                    gotoxy(pos);
                    printf("%c ",CARACTER_CURSOR);
                }
                break;
            case FLECHA_ABAJO: //Abajo
                if (seleccion < cantidadOpciones)
                {
                    //El mismo codigo que para arriba, pero ahora hacia abajo
                    gotoxy(pos);
                    printf("  ");
                    seleccion++;

                    pos.Y++;
                    pos.X = centroX - ( ( strlen(opciones[seleccion - 1]) / 2 ) + 2 );
                    gotoxy(pos);
                    printf("%c ",CARACTER_CURSOR);
                }
                break;
            default: //Este caso se va a ejecutar cuando se presionen flechas no indicadas en el switch como las de izquierda y derecha, u otros casos inesperados
                break;
            }
            esFlechaDelTeclado = false; //Despues de capturar el codigo de cual flecha fue presionada desactivamos la bandera
        }

        if (cursor == CODIGO_FLECHA) esFlechaDelTeclado = true;

    } while ((cursor = _getch()) != '\r'); //Cuando el usuario haga una seleccion presionara enter y terminara con el loop de seleccion

    system("cls");
    return seleccion; //Retorno la seleccion del usuario
}