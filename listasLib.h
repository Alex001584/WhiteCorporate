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

void obtenerProductos(pLista_Modelo_Producto *lista)
{
    FILE *Fichero = fopen("modelos.txt","r");
    if (Fichero == NULL)
    {
        system("cls");
        perror("\nError al abrir el archivo\n\n");
        system("PAUSE");
        exit(-1);
    }

    int v,n;
    v = fscanf(Fichero,"%i\n",&n);
    if (v < 1)
    {
        system("cls");
        perror("\nError al leer el archivo\n\n");
        system("PAUSE");
        exit(-1);
    }
    
    pLista_Modelo_Producto primero = *lista;
    pLista_Modelo_Producto anterior;
    MODELO_PRODUCTO nuevo;

    while ((v = fscanf(Fichero,"%i\t%[^\t]\t%i",&nuevo.id,nuevo.nombre,&nuevo.id_materia_prima)) > 0 || !feof(Fichero))
    {
        *lista = (pLista_Modelo_Producto)malloc(sizeof(LISTA_MODELO_PRODUCTO));
        if (nuevo.id == 1) {anterior = *lista; primero = *lista;}

        anterior->siguiente = *lista;
        (*lista)->siguiente = NULL;
        (*lista)->anterior = anterior;
        anterior = *lista;

        (*lista)->modelos = nuevo;
        *lista = (*lista)->siguiente;
    }
    *lista = anterior;
    (*lista)->siguiente = primero;
    primero->anterior = *lista;

    fclose(Fichero);
    return;
}

void liberarProductos(pLista_Modelo_Producto *lista)
{
    pLista_Modelo_Producto primero = *lista;
    do
    {
        *lista = (*lista)->siguiente;
        if (*lista != primero) free((*lista)->anterior);
        else break;
    } while (*lista != primero);
    return;
}

void obtenerPedidos(pLista_Pedido *lista)
{
    FILE *Fichero = fopen("pedidos.txt","r");
    if (Fichero == NULL)
    {
        system("cls");
        perror("\nError al abrir el archivo\n\n");
        system("PAUSE");
        exit(-1);
    }

    int v,n;
    v = fscanf(Fichero,"%i\n",&n);
    if (v < 1)
    {
        system("cls");
        perror("\nError al leer el archivo\n\n");
        system("PAUSE");
        exit(-1);
    }
    
    pLista_Pedido primero = *lista;
    pLista_Pedido anterior;
    PEDIDO nuevo;

    while ((
            v = fscanf(Fichero,"%i\t%i\t%i\t%f\t%f\t%hi\t%hi\t%hi\t%hi\t%hi\t%hi",
            &nuevo.id,
            &nuevo.id_proveedor,
            &nuevo.id_materia_prima,
            &nuevo.cantidad,
            &nuevo.monto_total,
            &nuevo.fecha_pedido.dia,
            &nuevo.fecha_pedido.mes,
            &nuevo.fecha_pedido.anio,
            &nuevo.fecha_entrega.dia,
            &nuevo.fecha_entrega.mes,
            &nuevo.fecha_entrega.anio
        )
    ) > 0 || !feof(Fichero))
    {
        *lista = (pLista_Pedido)malloc(sizeof(LISTA_PEDIDO));
        if (nuevo.id == 1) {anterior = *lista; primero = *lista;}

        anterior->siguiente = *lista;
        (*lista)->siguiente = NULL;
        (*lista)->anterior = anterior;
        anterior = *lista;

        (*lista)->pedido = nuevo;
        *lista = (*lista)->siguiente;
    }
    *lista = anterior;
    (*lista)->siguiente = primero;
    primero->anterior = *lista;

    fclose(Fichero);
    return;
}

void liberarPedidos(pLista_Pedido *lista)
{
    pLista_Pedido primero = *lista;
    do
    {
        *lista = (*lista)->siguiente;
        if (*lista != primero) free((*lista)->anterior);
        else break;
    } while (*lista != primero);
    return;
}