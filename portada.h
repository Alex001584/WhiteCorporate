#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void imprimirPortada()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE *Imagen = fopen("dibujo.csv","r");
    int v = 1, color = 0;

    COORD pos; obtenerCentroConsola(&pos);
    const short Xcentro = pos.X;
    pos.X -= 42;

    while (!feof(Imagen) && v > 0)
    {
        v = fscanf(Imagen,"%i,",&color);
        if (color == -1) {pos.Y++; gotoxy(pos);}
        else
        {
            SetConsoleTextAttribute(hConsole,color);
            printf("%c",CARACTER_BLOQUE_PARA_DIBUJO);
        }
    }
    fclose(Imagen);

    SetConsoleTextAttribute(hConsole,FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
    pos.X = Xcentro - 11;
    pos.Y += 2;
    gotoxy(pos);
    caracterPrint("--- WHITE CORPORATE ---");

    pos.X = Xcentro - 17;
    pos.Y++;
    gotoxy(pos);
    caracterPrint("Alejandro Dominguez Castro - 182679");

    pos.X = Xcentro - 19;
    pos.Y++;
    gotoxy(pos);
    caracterPrint("Danna Yunuen Martinez Rosales - 179996");

    pos.X = Xcentro - 19;
    pos.Y += 2;
    gotoxy(pos);
    caracterPrint("Presione cualquier tecla para continuar");

    getch();
    return;
}