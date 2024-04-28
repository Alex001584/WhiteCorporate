long obtenerMateriasPrimas(pLista_Materia_Prima *lista)
{
    FILE *Fichero = fopen("materias_primas.txt","r");
    if (Fichero == NULL)
    {
        system("cls");
        perror("\nError al abrir el archivo\n\n");
        system("PAUSE");
        exit(-1);
    }

    long cant;
    fscanf(Fichero,"%ld",&cant);

    *lista = (pLista_Materia_Prima)malloc(sizeof(LISTA_MATERIA_PRIMA));
    (*lista)->anterior = NULL;
    (*lista)->siguiente = NULL;

    int v = fscanf(Fichero,"%i \t %[^\t] \t %f",
        &(*lista)->materia_prima.id,
        (*lista)->materia_prima.nombre,
        &(*lista)->materia_prima.cantidad
    );

    while (!feof(Fichero) && v > 0)
    {
        (*lista)->siguiente = (pLista_Materia_Prima)malloc(sizeof(LISTA_MATERIA_PRIMA));
        (*lista)->siguiente->anterior = *lista;
        (*lista)->siguiente->siguiente = NULL;
        *lista = (*lista)->siguiente;

        v = fscanf(Fichero,"%i \t %[^\t] \t %f",
            &(*lista)->materia_prima.id,
            (*lista)->materia_prima.nombre,
            &(*lista)->materia_prima.cantidad
        );
    }
    
    *lista = (*lista)->anterior;
    free((*lista)->siguiente);
    (*lista)->siguiente = NULL;

    while ((*lista)->anterior != NULL) *lista = (*lista)->anterior;

    fclose(Fichero);
    return cant;
}

void escribirMateriasPrimas(pLista_Materia_Prima lista, long materias)
{
    FILE *Fichero = fopen("materias_primas.txt","w+");
    if (Fichero == NULL)
    {
        system("cls");
        perror("\nError al abrir el archivo\n\n");
        system("PAUSE");
        exit(-1);
    }

    int v = fprintf(Fichero,"%ld\n",materias);
    if (v < 1)
    {
        system("cls");
        perror("\nError al escribir el archivo\n\n");
        system("PAUSE");
        exit(-1);
    }

    while (lista->siguiente != NULL)
    {
        fprintf(Fichero,"%i\t%s\t%f\n",lista->materia_prima.id,lista->materia_prima.nombre,lista->materia_prima.cantidad);
        lista = lista->siguiente;
    }
    fprintf(Fichero,"%i\t%s\t%f\n",lista->materia_prima.id,lista->materia_prima.nombre,lista->materia_prima.cantidad);

    while (lista->anterior != NULL) lista = lista->anterior;

    fclose(Fichero);
    return;
}

void liberarMateriasPrimas(pLista_Materia_Prima *lista)
{
    while ((*lista)->siguiente != NULL) *lista = (*lista)->siguiente;

    while ((*lista)->anterior != NULL)
    {
        *lista = (*lista)->anterior;
        free((*lista)->siguiente);
    }
    
    free(*lista);

    return;
}

void obtenerProveedores(pLista_Proveedor *lista)
{
    FILE *Fichero = fopen("proveedores.txt","r");
    if (Fichero == NULL)
    {
        system("cls");
        perror("\nError al abrir el archivo\n\n");
        system("PAUSE");
        exit(-1);
    }

    long cant;
    fscanf(Fichero,"%ld",&cant);

    *lista = (pLista_Proveedor)malloc(sizeof(LISTA_PROVEEDOR));
    (*lista)->anterior = NULL;
    (*lista)->siguiente = NULL;
    
    int v = fscanf(Fichero,"%i\t%[^\t]\t%[^\t]\t%[^\n]",
        &(*lista)->proveedor.id,
        (*lista)->proveedor.nombre,
        (*lista)->proveedor.direccion,
        (*lista)->proveedor.telefono
    );
    if (v < 1)
    {
        system("cls");
        perror("Error al escribir al archivo");
        printf("\n\n");
        system("PAUSE");
        exit(-1);
    }

    while (!feof(Fichero) && v > 0)
    {
        (*lista)->siguiente = (pLista_Proveedor)malloc(sizeof(LISTA_PROVEEDOR));
        (*lista)->siguiente->siguiente = NULL;
        (*lista)->siguiente->anterior = *lista;
        *lista = (*lista)->siguiente;

        fscanf(Fichero,"%i\t%[^\t]\t%[^\t]\t%[^\n]",
            &(*lista)->proveedor.id,
            (*lista)->proveedor.nombre,
            (*lista)->proveedor.direccion,
            (*lista)->proveedor.telefono
        );
    }

    *lista = (*lista)->anterior;
    free((*lista)->siguiente);
    (*lista)->siguiente = NULL;

    while ((*lista)->anterior != NULL) *lista = (*lista)->anterior;

    fclose(Fichero);
    return;
}

void liberarProveedores(pLista_Proveedor *lista)
{
    while ((*lista)->siguiente != NULL) *lista = (*lista)->siguiente;

    while ((*lista)->anterior != NULL)
    {
        *lista = (*lista)->anterior;
        free((*lista)->siguiente);
    }
    
    free(*lista);

    return;
}