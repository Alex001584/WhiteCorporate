//Prototipos de las funciones
void MenuPrincipal(); //Men� principal que se muestra al inicio

//Funciones para lectura de datos
void imprimirStringMenuCentrado(const char string[], PCOORD coordenadas, short aumentoEnY);
void imprimirMenuRegistroUsuario(COORD camposPosCursor[6]);
void manejoFlechasCursor(short *campo, short *privilegio, short *salida, char tecla, COORD camposPosCursor[6]);
void lecturaDinamicaUsuarioContra(char cadena[100], char tecla, short campo, COORD camposPosCursor[6]);
void lecturaContra(char contra[100]);
bool inicioSesion(short); //Inicio de sesion

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