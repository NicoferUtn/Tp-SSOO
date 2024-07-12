#include "io-archivos-abiertos.h"

t_list *obtener_archivos_ya_abiertos(t_interfaz *interfaz) {

    // Obtenemos el directorio: 
    char *path_directorio = get_path_dial_fs(interfaz);
    DIR *directorio = opendir(path_directorio);

    if(!directorio) {
        log_error(logger, "No se pudo abrir el directorio %s", path_directorio);
        return NULL;
    }

    // Inicializamos la lista de archivos:
    t_list *archivos_abiertos = list_create();
    char *name_de_archivo_bloque = "bloques";
    char *name_de_archivo_bitmap = "bitmap";

    // Iteramos sobre los archivos del directorio:
    struct dirent *archivo;
    while(archivo = readdir(directorio)) {
        if(archivo->d_type == DT_REG) {
            if(strncmp(archivo->d_name, name_de_archivo_bloque, 7) != 0 && strncmp(archivo->d_name, name_de_archivo_bitmap, 6) != 0) {
    
                // Inicializamos el archivo abierto:
                t_config *archivo_metadata = abrir_archivo_metadata_config(interfaz, archivo->d_name, "r");
                t_archivo_abierto *archivo_abierto = malloc(sizeof(t_archivo_abierto));
                
                // Seteamos los datos del archivo abierto:
                set_archivo_metada_en_archivo_abierto(archivo_abierto, archivo_metadata);
                set_name_file_en_archivo_abierto(archivo_abierto, archivo->d_name);

                // Agregamos el archivo a la lista de archivos abiertos:
                if(es_un_archivo_valido(archivo_metadata))
                    list_add(archivos_abiertos, archivo_abierto);
            }
        }
    }

    // Cerramos el directorio:
    closedir(directorio);

    return archivos_abiertos;
}

// Funciones para setear los datos del archivo abierto:

void set_archivo_metada_en_archivo_abierto(t_archivo_abierto *archivo_abierto, t_config *archivo_metadata) {
    archivo_abierto->archivo_metadata = archivo_metadata;
}

void set_name_file_en_archivo_abierto(t_archivo_abierto *archivo_abierto, char *name_file) {
    archivo_abierto->name_file = name_file;
}

void set_nuevo_archivo_abierto(t_list *archivos_abiertos, char *name_file, t_config *archivo_metadata) {
    t_archivo_abierto *archivo_abierto = malloc(sizeof(t_archivo_abierto));
    archivo_abierto->name_file = name_file;
    archivo_abierto->archivo_metadata = archivo_metadata;

    list_add_sorted(archivos_abiertos, archivo_abierto, comparar_bloque_inicial);
}

// Funciones para gettear los datos del archivo abierto:

t_config *get_archivo_metadata(t_archivo_abierto *archivo_abierto) {
    return archivo_abierto->archivo_metadata;
}

// Funciones para verificar si existe ya un archivo:
int ya_esta_abierto(t_list *archivos_abiertos, char *nombre_archivo) {

    // Iteramos sobre los archivos abiertos:
    int size = list_size(archivos_abiertos);

    // Buscamos si el archivo ya esta abierto:
    for (int i = 0; i < size; i++) {
        t_archivo_abierto *archivo_abierto = list_get(archivos_abiertos, i);
        char *nombre_archivo_abierto = archivo_abierto->name_file;

        if (strcmp(nombre_archivo_abierto, nombre_archivo) == 0) {
            return 1;
        }
    }

    return 0;
}

// Funciones para obtener un archivo abierto:
t_archivo_abierto *obtener_archivo_abierto(t_list *archivos_abiertos, char *nombre_archivo) {

    // Iteramos sobre los archivos abiertos:
    int size = list_size(archivos_abiertos);
    int cantidad_de_caracteres = strlen(nombre_archivo);

    // Buscamos el archivo abierto:
    for (int i = 0; i < size; i++) {
        t_archivo_abierto *archivo_abierto = list_get(archivos_abiertos, i);
        char *nombre_archivo_abierto = archivo_abierto->name_file;

        if (strncmp(nombre_archivo_abierto, nombre_archivo, cantidad_de_caracteres) == 0) {
            return archivo_abierto;
        }
    }

    return NULL;
}

// Funciones para cerrar un archivo abierto:
void cerrar_archivo_abierto(t_list *archivos_abiertos, char *nombre_archivo) {

    // Iteramos sobre los archivos abiertos:
    int size = list_size(archivos_abiertos);
    int cantidad_de_caracteres = strlen(nombre_archivo);

    // Buscamos el archivo abierto:
    for (int i = 0; i < size; i++) {
        t_archivo_abierto *archivo_abierto = list_get(archivos_abiertos, i);
        char *nombre_archivo_abierto = archivo_abierto->name_file;

        if (strncmp(nombre_archivo_abierto, nombre_archivo, cantidad_de_caracteres) == 0) {
            list_remove(archivos_abiertos, i);
            free(archivo_abierto);
            free(nombre_archivo_abierto);
            return;
        }
    }
}

// Funciones para cerrar todos los archivos abiertos:
void cerrar_todos_los_archivos_abiertos(t_list *archivos_abiertos) {
    int size = list_size(archivos_abiertos);

    for (int i = 0; i < size; i++) {
        t_archivo_abierto *archivo_abierto = list_get(archivos_abiertos, i);
        t_config *archivo_metadata = archivo_abierto->archivo_metadata;
        char *nombre_archivo = archivo_abierto->name_file;

        // Cerramos el archivo:
        config_destroy(archivo_metadata);
        free(nombre_archivo);
        free(archivo_abierto); 
    }

    list_destroy(archivos_abiertos);
}   