#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "librerias.h"

int main(int argc, char const *argv[])
{

    // char** string = (char**)malloc(sizeof(char*) * 5);
    // for (short i = 0; i < 5; i++)
    // {
    //     string[i] = (char*)malloc(sizeof(char) * 128);
    // }
    
    // strcpy(string[0],"Prueba");
    // strcpy(string[1],"Prueba1");
    // strcpy(string[2],"Prueba2");
    // strcpy(string[3],"Prueba3");
    // strcpy(string[4],"Prueba4");

    // // imprimirStringCentrado(string,5,false);

    // // int seleccion; bool bandera;
    // // do
    // // {
    // //     if (seleccion == 224) bandera = true;
    // //     switch (seleccion)
    // //     {
    // //     case 72:
    // //         printf("Arriba\n");
    // //         bandera = false;
    // //         break;
    // //     case 75:
    // //         printf("Izquierda\n");
    // //         bandera = false;
    // //         break;
    // //     case 80:
    // //         printf("Abajo\n");
    // //         bandera = false;
    // //         break;
    // //     case 77:
    // //         printf("Derecha\n");
    // //         bandera = false;
    // //         break;
    // //     default:
    // //         break;
    // //     }
    // // } while ((seleccion = getch()) != '\r');

    // char titulo[] = "TITULOPRUEBA";
    // crearMenu(string,5);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE *Imagen = fopen("dibujo.csv","r");
    int v = 1, color = 0;

    //223
    while (!feof(Imagen) && v > 0)
    {
        v = fscanf(Imagen,"%i,",&color);
        if (color == -1) printf("\n");
        else
        {
            SetConsoleTextAttribute(hConsole,color);
            printf("%c",219);
        }
    }
    

    fclose(Imagen);

    printf("\n\n");
    system("PAUSE");
    return 0;
}
