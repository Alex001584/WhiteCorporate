#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h> 
#include <locale.h>
#include <windows.h>
#include <string.h>
#include <ctype.h>

void gotoxy(int x, int y){
      HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
      COORD dWPos;
      dWPos.X = x;
      dWPos.Y = y;
      SetConsoleCursorPosition(hcon,dWPos);
}

void portada(){
	int aux = 0;
	FILE *archivo = fopen("Inventario.txt", "r");
	
	for(int i=0; i<161; i++){ //Largo: 161, ancho: 82
		for(int j=0; j<82; j++){
			fscanf(archivo, "%d\t", &aux);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),aux);
			printf("%c", 178);
		}
		printf("\n");
	}
	fclose(archivo);
}

void datos(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
    gotoxy(60, 10);
    printf("Universidad Polit%ccnica de San Luis Potos%c", 130, 161);
    gotoxy(60, 11);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    printf("Programaci%cn II", 162);
    gotoxy(60, 12);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
    printf("Proyecto Final White Corporate");
    gotoxy(60, 13);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
    printf("Dom%cnguez Castro Alejandro 182679", 164);
    gotoxy(60, 14);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    printf("Mart%cnez Rosales Danna Yunuen 179996", 161);
    gotoxy(60, 15);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    printf("Presione cualquier tecla para continuar...");
}


int main(){
	datos();
	gotoxy(0,0);
	portada();
	getch();
	system("cls");
	
    return 0;
}
