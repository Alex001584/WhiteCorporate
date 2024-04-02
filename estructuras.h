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
    char usuario[100];
    char contra[100];
};

struct LISTA_USUARIOS
{
    USUARIOS *usuario;
    LISTA_USUARIOS *siguiente;
    LISTA_USUARIOS *anterior;
}; typedef LISTA_USUARIOS* pLISTA_USUARIOS;