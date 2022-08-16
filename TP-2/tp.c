# include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *popen(const char *command, const char *mode);
int pclose(FILE *stream);

void removeLeading(char *str, char *str1) {
	int idx = 0, j, k = 0;

	// Iterate String until last
	// leading space character
	while (str[idx] == ' ' || str[idx] == '\t' || str[idx] == '\n')
	{
		idx++;
	}

	// Run a for loop from index until the original
	// string ends and copy the content of str to str1
	for (j = idx; str[j] != '\0'; j++)
	{
		str1[k] = str[j];
		k++;
	}

	// Insert a string terminating character
	// at the end of new string
	str1[k] = '\0';
}

void opcionesHabilitadas() {
  printf("Por favor inserte un número: \n");
  printf("1) Listado de acciones cuyo precio al contado en apertura supera los $200.\n");
  printf("2) Listado del promedio de cotización de compra y venta de todas las acciones al contado en .xls.\n");
  printf("3) Listado de acciones cuyo precio al contado en apertura supera los $200 en tabla html.\n");
  printf("4) Listado de acciones que tienen menor y mayor %% de variación.\n");
}

void obtenerContenidoHtml(char *etiquetaHtml, char *contenido) {
  int i = 0;
  int esContenido;
  char *simboloI = "<";
  char *simboloC = ">";
  for (i = 0; i < strlen(etiquetaHtml); i++) {
    char *caracterActual = &etiquetaHtml[i];
  printf("llego a obtencion %s", caracterActual);
    if (caracterActual == simboloI) esContenido = 1;
    if (caracterActual == simboloC) esContenido = 0;
    if (!esContenido && caracterActual != ">") {
      printf("%s", contenido);
      strcat(contenido, caracterActual);
    }
  }
}

int lecturaTabla(FILE *archivo, int opcion) {
  char *et_table_cerrada = "</table>\n\0";
  char *et_thead_abierta = "<thead>";
  char *et_thead_cerrada = "</thead>";
  char *et_tbody_abierta = "<tbody>";
  char *et_tbody_cerrada = "</tbody>";
  char *et_tr_abierta = "<tr";
  char *et_tr_cerrada = "</tr>";
  char result[2048];

  // Columnas a trabajar
  int columna_accion = 0;
  int columna_vto = 0;
  int columna_compra = 0;
  int columna_venta = 0;
  int columna_apertura = 0;
  int columna_variacion = 0;

  // Variables auxiliares 
  int tHead = 1;
  int tBody = 1;
  int tR = 1;
  int contadorColumnas = -1;
  int contadorFilas = -1;

  // Variables de operaciones
  // Punto 1
  char *accion;
  int accionContado;
  float accionApertura;


  while(fgets(result, sizeof(result), archivo) != NULL && strstr(result, et_table_cerrada) == NULL) {
    if (strstr(result, et_thead_abierta)) {
      tHead = 0;
      tBody = 1;
    }
    if (strstr(result, et_tbody_abierta)) {
      tBody = 0;
      tHead = 1;
    }
    if (strstr(result, et_tr_abierta)) {
      tR = 0;
      contadorFilas = -1;
    }
    if (strstr(result, et_thead_cerrada)) tHead = 1;
    if (strstr(result, et_tbody_cerrada)) tBody = 1;
    if (strstr(result, et_tr_cerrada)) tR = 1;

    if (tHead == 0 && tR == 0) {
      if (strstr(result, "Especie</th>")) columna_accion = contadorColumnas;
      if (strstr(result, "Vto</th>")) columna_vto = contadorColumnas;
      if (strstr(result, "Compra</th>")) columna_compra = contadorColumnas;
      if (strstr(result, "Venta</th>")) columna_venta = contadorColumnas;
      if (strstr(result, "Apertura</th>")) columna_apertura = contadorColumnas;
      if (strstr(result, "Variación</th>")) columna_variacion = contadorColumnas;
      contadorColumnas++;
    }

    if (tBody == 0 && tR == 0) {
      if (contadorFilas == columna_accion) obtenerContenidoHtml(result, accion);
      if (opcion == 1) {
        if (contadorFilas == columna_vto && strstr(result, "Vto</th>")) {

        }
      }
      contadorFilas++;
    }

  }
  printf("resultado de lectura tabla %s", result);
  printf("posicion vto: %d \n", columna_vto);
  printf("posicion compra: %d \n", columna_compra);
  printf("posicion venta: %d \n", columna_venta);
  printf("posicion apertura: %d \n", columna_apertura);
  printf("posicion variacion: %d \n", columna_variacion);
  printf("contenido: %s \n", accion);
  return 0;
}

int lecturaArchivoHTML(int opcion) {
  char *et_html_abierta = "<HTML>\n\0";
  char *et_html_cerrada = "</HTML>\n\0";
  char *et_table_abierta = "<table";

  FILE *cmd = popen("C:\\Users\\vivia\\Downloads\\wget -q -O - http://localhost:5000/static/", "r");
  char result[2048];
  while(fgets(result, sizeof(result), cmd) != NULL) {
    if (strstr(result, et_table_abierta)) lecturaTabla(cmd, opcion);
  }
  pclose(cmd);
}

int main(void) {
  char opcion[100];
  opcionesHabilitadas();
  gets(opcion);

  if (atoi(opcion) < 0 || atoi(opcion) > 4) {
    printf("Opcion invalida\n");
    return -1;
  } 

  lecturaArchivoHTML(atoi(opcion));
  gets(opcion);
  return 0;
}