//Prototipos de las funciones
void MenuPrincipal(); //Men� principal que se muestra al inicio

//Funciones de inicio de sesion
void manejoFlechasInicioSesion(char tecla, short *campo, PCOORD camposPos);
void lecturaDinamicaUsuarioContraInicio(char cadena[50], char tecla, short campo, PCOORD camposPosCursor);
void leerListaUsuarios(pLISTA_USUARIOS *lista);
void liberarListaUsuarios(pLISTA_USUARIOS *lista);
short inicioSesion();

//Funciones del registro de usuario
void imprimirStringCentrado(const char string[], PCOORD coordenadas, short aumentoEnY);
void imprimirMenuRegistroUsuario(COORD camposPosCursor[6]);
void manejoFlechasCursorRegUser(short *campo, short *privilegio, short *salida, char tecla, COORD camposPosCursor[6]);
void lecturaDinamicaUsuarioContra(char cadena[50], char tecla, short campo, PCOORD camposPosCursor);

void MenuAdministrador(); //Men� del administrador con sus respectivas opciones
void RegistrarUsuario();
void RegistrarProveedor();
void RegistrarMatePrim();
void RegistrarModelos();

void MenuCapturista(); //Men� del capturista con sus respectivas opciones
void PedidoMateriaPrima();
void ReporteModelos();
void ReporteInventario();
void PedidosMatPriProveedores();
void GenerarArchivosReporte();