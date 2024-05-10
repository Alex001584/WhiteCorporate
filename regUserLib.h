void imprimirStringCentrado(const char string[], PCOORD coordenadas, short aumentoEnY)
{
    COORD centro; obtenerCentroConsola(&centro);
    coordenadas->X = centro.X - strlen(string)/2;
    (coordenadas->Y) += aumentoEnY;
    gotoxy(*coordenadas);
    printf(string);
    return;
}

void imprimirMenuRegistroUsuario(COORD camposPosCursor[6])
{
    /*
                    --- NUEVO USUARO ---

        Usuario:
        Contrasena:

        Privilegio:  > Administrador  > Capturista

        > Cancelar  > Ok
    */

    //Configuracion incial
    COORD pos;
    obtenerCentroConsola(&pos);
    const short Xcentro = pos.X;
    pos.Y -= 4;

    //Titulo
    imprimirStringCentrado("--- NUEVO USUARIO ---",&pos,0);

    //Posicion en x de los elementos de este punto en adelante
    pos.X = Xcentro - 21;
    
    //Usuario
    pos.Y += 2;
    gotoxy(pos);
    puts("Usuario: ");
    camposPosCursor[USUARIO].X = pos.X + 9;
    camposPosCursor[USUARIO].Y = pos.Y;

    //Contra
    pos.Y++;
    gotoxy(pos);
    printf("Contrase%ca: ",164);
    camposPosCursor[CONTRA].X = pos.X + 12;
    camposPosCursor[CONTRA].Y = pos.Y;

    //Privilegio
    pos.Y += 2;
    gotoxy(pos);
    puts("Privilegio:    Administrador    Capturista");
    camposPosCursor[ADMIN].X = pos.X + 13;
    camposPosCursor[ADMIN].Y = pos.Y;
    camposPosCursor[CAPTU].X = pos.X + 30;
    camposPosCursor[CAPTU].Y = pos.Y;

    //Opciones de salida
    pos.Y += 2;
    gotoxy(pos);
    puts("  Cancelar    Ok");
    camposPosCursor[CANCELAR].X = pos.X;
    camposPosCursor[CANCELAR].Y = pos.Y;
    camposPosCursor[OK].X = pos.X + 12;
    camposPosCursor[OK].Y = pos.Y;

    return;
}

void manejoFlechasCursorRegUser(short *campo, short *privilegio, short *salida, char tecla, COORD camposPosCursor[6])
{
    switch (tecla)
    {
    case FLECHA_ARRIBA:
        switch (*campo)
        {
        case CONTRA:
            *campo = USUARIO;
            gotoxy(camposPosCursor[*campo]);
            break;
        case ADMIN:
        case CAPTU:
            *campo = CONTRA;
            gotoxy(camposPosCursor[*campo]);
            break;
        case OK:
        case CANCELAR:
            *campo = *privilegio;
            gotoxy(camposPosCursor[*campo]);
            printf(">");
            break;
        default:
            break;
        }
        break;
    case FLECHA_ABAJO:
    case '\r':
        switch (*campo)
        {
        case USUARIO:
            *campo = CONTRA;
            gotoxy(camposPosCursor[*campo]);
            break;
        case CONTRA:
            *campo = *privilegio;
            gotoxy(camposPosCursor[*campo]);
            printf(">");
            break;
        case ADMIN:
        case CAPTU:
            *campo = *salida;
            gotoxy(camposPosCursor[*campo]);
            printf(">");
        default:
            break;
        }
        break;
    case FLECHA_IZQUIERDA:
        if (*campo == CAPTU)
        {
            printf("\b \b");
            *campo = ADMIN;
            *privilegio = ADMIN;
            gotoxy(camposPosCursor[*campo]);
            printf(">");
        }
        else if (*campo == OK)
        {
            printf("\b \b");
            *campo = CANCELAR;
            *salida = CANCELAR;
            gotoxy(camposPosCursor[*campo]);
            printf(">");
        } 
        break;
    case FLECHA_DERECHA:
        if (*campo == ADMIN)
        {
            printf("\b \b");
            *campo = CAPTU;
            *privilegio = CAPTU;
            gotoxy(camposPosCursor[*campo]);
            printf(">");
        }
        else if (*campo == CANCELAR)
        {
            printf("\b \b");
            *campo = OK;
            *salida = OK;
            gotoxy(camposPosCursor[*campo]);
            printf(">");
        }
        break;
    default:
        break;
    }

    return;
}

void lecturaDinamicaUsuarioContra(char cadena[50], char tecla, short campo, PCOORD camposPosCursor)
{
    short cabeza = strlen(cadena);
    if (tecla == '\b')
    {
        if (cabeza > 0)
        {
            cadena[cabeza-1] = '\0';
            printf("\b \b");
            camposPosCursor[campo].X--;
        }
    }
    else if (cabeza < 49)
    {
        cadena[cabeza] = tecla;
        cadena[cabeza+1] = '\0';
        if (campo == USUARIO) printf("%c",tecla);
        else if (campo == CONTRA) printf("*");
        camposPosCursor[campo].X++;
    }

    return;
}