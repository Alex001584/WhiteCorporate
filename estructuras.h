//Estructuras

//Estructura fecha
struct FECHA
{
    short dia;
    short mes;
    short anio;
};

//Estructura de datos del proveedor
struct PROVEEDOR
{
    int id;
    char nombre[50];
    char direccion[100];
    char telefono[15];
};

struct LISTA_PROVEEDOR
{
    PROVEEDOR proveedor;
    LISTA_PROVEEDOR *siguiente;
    LISTA_PROVEEDOR *anterior;
};
typedef LISTA_PROVEEDOR *pLista_Proveedor;

//Estructura de datos de la materia prima
struct MATERIA_PRIMA
{
    int id;
    char nombre[50];
    float cantidad;
};

struct LISTA_MATERIA_PRIMA
{
    MATERIA_PRIMA materia_prima;
    LISTA_MATERIA_PRIMA *siguiente;
    LISTA_MATERIA_PRIMA *anterior;
};
typedef LISTA_MATERIA_PRIMA *pLista_Materia_Prima;

//Estructura de datos de modelo del producto
struct MODELO_PRODUCTO {
    int id;
    char nombre[50];
    int id_materia_prima;
};

struct LISTA_MODELO_PRODUCTO
{
    MODELO_PRODUCTO modelos;
    LISTA_MODELO_PRODUCTO *siguiente;
    LISTA_MODELO_PRODUCTO *anterior;
};
typedef LISTA_MODELO_PRODUCTO *pLista_Modelo_Producto;

//Estructura para realizar un pedido a un proveedor
struct PEDIDO {
    int id;
    int id_proveedor;
    int id_materia_prima;
    float cantidad;
    float monto_total;
    FECHA fecha_pedido;
    FECHA fecha_entrega;
};

struct LISTA_PEDIDO
{
    PEDIDO pedido;
    LISTA_PEDIDO *siguiente;
    LISTA_PEDIDO *anterior;
};
typedef LISTA_PEDIDO *pLista_Pedido;

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