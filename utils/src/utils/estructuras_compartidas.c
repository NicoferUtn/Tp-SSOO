#include "estructuras_compartidas.h"

t_procesar_conexion* crear_procesar_conexion(t_log *logger, char *server_name, int socket_cliente)
{
    t_procesar_conexion *procesar_cconexion = malloc(sizeof(t_procesar_conexion));
    procesar_cliente -> server_name = server_name;
    procesar_cliente -> logger = logger;
    procesar_cliente -> socket_cliente = socket_cliente;

    return procesar_cconexion;
}

