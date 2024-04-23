void manejoFlechasInicioSesion(char tecla, short *campo, PCOORD camposPos)
{
    enum {USUARIO, CONTRA, CANCELAR, OK};

    switch (tecla)
    {
    case FLECHA_ARRIBA:
        if (*campo == CONTRA) *campo = USUARIO;
        else if (*campo == CANCELAR || *campo == OK)
        {
            printf("\b ");
            *campo = CONTRA;
        }
        gotoxy(camposPos[*campo]);
        break;
    case FLECHA_ABAJO:
    case '\r':
        if (*campo == USUARIO)
        {
            *campo = CONTRA;
            gotoxy(camposPos[*campo]);
        }
        else if (*campo == CONTRA)
        {
            *campo = OK;
            gotoxy(camposPos[*campo]);
            printf(">");
        }
        break;
    case FLECHA_IZQUIERDA:
        if (*campo == OK)
        {
            printf("\b ");
            *campo = CANCELAR;
            gotoxy(camposPos[*campo]);
            printf(">");
        }
        break;
    case FLECHA_DERECHA:
        if (*campo == CANCELAR)
        {
            printf("\b ");
            *campo = OK;
            gotoxy(camposPos[*campo]);
            printf(">");
        }
        break;
    default:
        break;
    }
}

void leerListaUsuarios(pLISTA_USUARIOS *lista)
{
    //Declaracion de variables
    FILE *Fichero;
    int v,x;

    //Abro el archivo a usar dependiendo de las credenciales esperadas
    Fichero = fopen("usuarios.txt","r");
    if (Fichero == NULL)
    {
        system("cls");
        perror("Erorr al leer el archivo\n");
        system("PAUSE");
        exit(-1);
    }

    //Aloco memoria
    *lista = (pLISTA_USUARIOS)malloc(sizeof(LISTA_USUARIOS)); //Aloco memoria a la estructura puntero de la lista de usuarios
    (*lista)->usuario = (USUARIOS*)malloc(sizeof(USUARIOS)); //Aloco memoria al sector que lleva los datos de la estructura puntero de usuarios
    
    //Hago la primera lectura de usuario y contra al archivo
    v = fscanf(Fichero,"%s\t%s\t%d",(*lista)->usuario->usuario,(*lista)->usuario->contra,&(*lista)->usuario->privilegio);
    (*lista)->siguiente = NULL; (*lista)->anterior = NULL; //Inicializo los apuntadores

    //Creo el loop que lee todos los usuarios y contras del archivo y los coloco en la lista
    while (v != EOF && !feof(Fichero))
    {
        //Creo y aloco memoria al nuevo nodo
        pLISTA_USUARIOS nuevo;
        nuevo = (pLISTA_USUARIOS)malloc(sizeof(LISTA_USUARIOS));
        nuevo->usuario = (USUARIOS*)malloc(sizeof(USUARIOS));

        //Le otorgo las credenciales leidas del archivo al nodo
        v = fscanf(Fichero,"%s\t%s\t%d",nuevo->usuario->usuario,nuevo->usuario->contra,&(nuevo)->usuario->privilegio);

        //Inicializo los apuntadores
        nuevo->siguiente = NULL;
        nuevo->anterior = *lista;

        //Actualizo los apuntadores de los nodos anteriores para incorporar el nuevo nodo
        (*lista)->siguiente = nuevo;
        *lista = (*lista)->siguiente;
    }

    while ((*lista)->anterior != NULL) *lista = (*lista)->anterior; //Regreso al inicio de la lista

    fclose(Fichero);
    return;
}

void liberarListaUsuarios(pLISTA_USUARIOS *lista)
{
    while ((*lista)->siguiente != NULL) //Voy a la ultima posicion de la lista 
    {
        *lista = (*lista)->siguiente;
    }

    while ((*lista)->anterior != NULL)
    {
        *lista = (*lista)->anterior;

        // free((*lista)->siguiente->usuario);
        free((*lista)->siguiente);
        
    } 
    
    // free((*lista)->usuario);
    free(*lista);
    return;
}

short inicioSesion()
{
    USUARIOS activo;
    strcpy(activo.usuario,"\0");
    strcpy(activo.contra,"\0");

    //Limipio la pantalla
    system("cls");

    /*
            --- INICIO DE SESION ---
        
        Usuario:
        Contrasena:

            > Cancelar  > Ok
    */

    COORD pos; COORD camposPos[4];
    enum {USUARIO, CONTRA, CANCELAR, OK};
    obtenerCentroConsola(&pos);
    const short Xcentro = pos.X;
    pos.Y -= 3;

    imprimirStringMenuCentrado("--- INICIO DE SESION ---",&pos,0);

    //Usuario
    pos.Y += 2;
    pos.X = Xcentro - 15;
    gotoxy(pos);
    printf("Usuario: ");
    camposPos[USUARIO].X = pos.X + 9;
    camposPos[USUARIO].Y = pos.Y;

    //Contra
    pos.Y++;
    gotoxy(pos);
    printf("Contrase%ca: ",164);
    camposPos[CONTRA].X = pos.X + 12;
    camposPos[CONTRA].Y = pos.Y;

    //Cancelar y Ok
    imprimirStringMenuCentrado("  Cancelar    Ok",&pos,2);
    camposPos[CANCELAR].X = pos.X;
    camposPos[CANCELAR].Y = pos.Y;
    camposPos[OK].X = pos.X + 12;
    camposPos[OK].Y = pos.Y;

    gotoxy(camposPos[USUARIO]);

    char tecla; short campo = USUARIO, privilegio; bool esFlecha;
    while ((tecla = _getch()) != '\r' || (campo != OK && campo != CANCELAR))
    {
        if (esFlecha || tecla == '\r')
        {
            manejoFlechasInicioSesion(tecla,&campo,camposPos);
            esFlecha = false;
        }
        else if (isalnum(tecla) || tecla == '\b')
        {
            if (campo == USUARIO) lecturaDinamicaUsuarioContra(activo.usuario,tecla,4,camposPos);
            else if (campo == CONTRA) lecturaDinamicaUsuarioContra(activo.contra,tecla,5,camposPos);
        }

        esFlecha = (tecla == CODIGO_FLECHA2) ? true : false;
    }
   
    bool bandera = false;
    if (campo != CANCELAR)
    {
        pLISTA_USUARIOS lista;
        leerListaUsuarios(&lista);

        while (lista->siguiente != NULL)
        {
            if (strcmp(lista->usuario->usuario,activo.usuario) == 0 && strcmp(lista->usuario->contra,activo.contra) == 0)
            {
                privilegio = lista->usuario->privilegio;
                system("cls");
                printf("Bienvenido, %s",activo.usuario);
                bandera = true;
                break;
            }
            lista = lista->siguiente;
        }

        if (!bandera) privilegio = -1;

        liberarListaUsuarios(&lista);
    }
    else
    {
        system("cls");
        obtenerCentroConsola(&pos);
        imprimirStringMenuCentrado("--- OPERACION CANCELADA ---",&pos,0);
        Sleep(100);
        exit(0);
    }
 
    return privilegio;
}