//Prototipos de las funciones
void MenuPrincipal(); //Men� principal que se muestra al inicio

//Funciones de inicio de sesion
void manejoFlechasInicioSesion(char tecla, short *campo, PCOORD camposPos);
void leerListaUsuarios(pLISTA_USUARIOS *lista);
void liberarListaUsuarios(pLISTA_USUARIOS *lista);
short inicioSesion();

//Funciones del registro de usuario
void imprimirStringMenuCentrado(const char string[], PCOORD coordenadas, short aumentoEnY);
void imprimirMenuRegistroUsuario(COORD camposPosCursor[6]);
void manejoFlechasCursorRegUser(short *campo, short *privilegio, short *salida, char tecla, COORD camposPosCursor[6]);
void lecturaDinamicaUsuarioContra(char cadena[100], char tecla, short campo, COORD camposPosCursor[6]);

void MenuAdministrador(); //Men� del administrador con sus respectivas opciones
void RegistrarUsuario();
void RegistrarProveedor();
void RegistrarMatePrim();
void GestionProductos();
void PedidosProveedores();

void MenuCapturista(); //Men� del capturista con sus respectivas opciones
void PedidoMateriaPrima();
void ReporteModelos();
void ReporteInventario();
void PedidosMatPriProveedores();