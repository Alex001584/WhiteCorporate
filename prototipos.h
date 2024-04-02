//Prototipos de las funciones
void MenuPrincipal(); //Men� principal que se muestra al inicio

void movimientoDelCampo(short *seleccion, short direccionalPresionada);
void registrarEntrada(char cadena[], short *cont, short *posX, char caracter, short campo);
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