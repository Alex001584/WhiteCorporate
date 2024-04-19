//Estructuras

//Estructura de datos del proveedor
struct PROVEEDOR
{
    int id;
    char nombre[50];
    char direccion[100];
    char telefono[15];
};

//Estructura de datos de la materia prima
struct MATERIA_PRIMA
{
    int id;
    char nombre[50];
    float cantidad;
};

//Estructura de datos de modelo del producto
/*typedef struct{
    int id;
    char nombre[50];
    int id_materia_prima;
} ModeloProducto;*/

//Estructura para realizar un pedido a un proveedor
/*typedef struct{
    int id;
    int id_proveedor;
    int cantidad;
    float monto_total;
    char fecha_entrega[20];
} PedidoProveedor;*/

//Estructura de usuario
enum {CAPTURISTA, ADMINISTRADOR};
struct USUARIOS
{
    char usuario[50];
    char contra[50];
    short privilegio;
};

struct LISTA_USUARIOS
{
    USUARIOS *usuario;
    LISTA_USUARIOS *siguiente;
    LISTA_USUARIOS *anterior;
}; typedef LISTA_USUARIOS* pLISTA_USUARIOS;

//Enum para la seleccion de usuario
enum {ADMIN, CAPTU, OK, CANCELAR, USUARIO, CONTRA}; //Con enum le doy al campo de admin valor 0 y capturista 1