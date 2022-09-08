# include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>

FILE *popen(const char *command, const char *mode);
int pclose(FILE *stream);

void opcionesHabilitadas() {
  printf("Por favor inserte un número: \n");
  printf("1) Listado de acciones cuyo precio al contado en apertura supera los $200.\n");
  printf("2) Listado del promedio de cotización de compra y venta de todas las acciones al contado en .xls.\n");
  printf("3) Listado de acciones cuyo precio al contado en apertura supera los $200 en tabla html.\n");
  printf("4) Listado de acciones que tienen menor y mayor %% de variación.\n");
}

char * contenidoTd(char linea[4096], int posicion) {
  int k = posicion;
  char* campo = malloc(100 * sizeof(char*));
  int camp = 0;
  while (k != 0) {
    k++;
    if (linea[k] == '<') {
      k = 0;
      campo[camp] = '\0';
    } else {
      campo[camp] = linea[k]; 
      camp++;
    }
  }
  return campo;
}

char * lecturaTd(char linea[4096], int posicion) {
  int j = posicion;
  char *resultado;
  while (j != 0) {
    j++;
    if (linea[j] == '>') {
      resultado = contenidoTd(linea, j);
      if (strlen(resultado) == 0) strcpy(resultado, "-\0");
    }
    if(linea[j] == '<') j = 0;
  }
  return resultado;
}

char ** lecturaHastaTd(char linea[4096]) {
  int i = 0;
  int index = 0;         
  char ** results = malloc(16 * sizeof(char*));
  for(int j = 0; j < 16; j++){
    results[j] = (char*)malloc(50*sizeof(char));
  }
  while (i < 4096 && linea[i]!= '\0') {
    if (linea[i] == '<') {
      i++;
      if (linea[i] == 't') {
        i++;
        if (linea[i] == 'd') {
            int posicionTd = i;
            char *resultado = lecturaTd(linea, posicionTd);
            sprintf(results[index], resultado);
            index++;
        }
      }
    }
    i++;
  }
  return results;
}

int endTD(char linea[4096]) {
  int i = 0;
  while (i < 4096 && linea[i]!= '\0') {
    if (linea[i] == '<') {
      i++;
      if (linea[i] == '/') {
        i++;
        if (linea[i] == 't') {
          i++;
          if (linea[i] == 'd') {
            i++;
            if (linea[i] == '>') {
              return true;
            }
          }
        }
      }
    }
    i++;
  }
  return false;
}

int getIndexEndTD(char linea[4096]) {
  int i = 0;
  while (i < 4096 && linea[i]!= '\0') {
    if (linea[i] == '<') {
      i++;
      if (linea[i] == '/') {
        i++;
        if (linea[i] == 't') {
          i++;
          if (linea[i] == 'd') {
            i++;
            if (linea[i] == '>') {
              return i - 4;
            }
          }
        }
      }
    }
    i++;
  }
  return false;
}

int getIndexStartEndTD(char linea[4096], int endIndex) {
  int i = endIndex;
  while (i > 0) {
    if (linea[i - 1] == '>') {
      return i - 1;
    }
    i--;
  }
  return 0;
}

int isTD(char linea[4096]) {
  int i = 0;
  while (i < 4096 && linea[i]!= '\0') {
    if (linea[i] == '<') {
      i++;
      if (linea[i] == 't') {
        i++;
        if (linea[i] == 'd') {
          i++;
          if (linea[i] == ' ') {
            return true;
          }
        }
      }
    }
    i++;
  }
  return 0;
}

char * getWordByIndexes(char linea[4096], int initIndex, int endIndex) {
  int indexChar = 0;
  int i = 0;
  char *result = malloc(100);
  
  while (i < endIndex) {

    if (i > initIndex){
      result[indexChar] = linea[i];
      indexChar++;
    }
    i++;
  }
  result[indexChar] = '\0';
  return result;
}

// Función auxiliar conseguida en la web
void removerCaracteres(char *cadena, char *caracteres) {
  int indiceCadena = 0, indiceCadenaLimpia = 0;
  int deberiaAgregarCaracter = 1;
  // Recorrer cadena carácter por carácter
  while (cadena[indiceCadena]) {
    // Primero suponemos que la letra sí debe permanecer
    deberiaAgregarCaracter = 1;
    int indiceCaracteres = 0;
    // Recorrer los caracteres prohibidos
    while (caracteres[indiceCaracteres]) {
      // Y si la letra actual es uno de los caracteres, ya no se agrega
      if (cadena[indiceCadena] == caracteres[indiceCaracteres]) {
        deberiaAgregarCaracter = 0;
      }
      indiceCaracteres++;
    }
    // Dependiendo de la variable de arriba, la letra se agrega a la "nueva
    // cadena"
    if (deberiaAgregarCaracter) {
      cadena[indiceCadenaLimpia] = cadena[indiceCadena];
      indiceCadenaLimpia++;
    }
    indiceCadena++;
  }
  // Al final se agrega el carácter NULL para terminar la cadena
  cadena[indiceCadenaLimpia] = 0;
}

int obtenerCalculo(int opcion, char **contenido, FILE *csv, FILE *html) {
  const char* columnaCdo = "Cdo.";
  if (opcion == 1) {
    char caracter[] = ".";
    removerCaracteres(contenido[8], caracter);
    if (strcmp(contenido[1], columnaCdo) == 0 && atoi(contenido[8]) > 200)
    printf("Accion con apertura mayor a 200: %s \n", contenido[0]);
  }
  if (opcion == 2) {
    if (strcmp(contenido[1], columnaCdo)) {
      fprintf(csv, "%s;", contenido[0]); // Especie
      fprintf(csv, "%s;", contenido[3]); // Compra
      fprintf(csv, "%s;", contenido[4]); // Venta
      fprintf(csv, "%s;", contenido[8]); // Apertura
      fprintf(csv, "%d; \n", (atoi(contenido[3]) + atoi(contenido[4])) / 2);
    }
  }
  if (opcion == 3) {
    char caracter[] = ".";
    removerCaracteres(contenido[8], caracter);
    if (strcmp(contenido[1], columnaCdo) == 0 && atoi(contenido[8]) > 200) {
      
      if ((atoi(contenido[3]) < atoi(contenido[8])) && (atoi(contenido[4]) < atoi(contenido[8]))) {
        char row[] = "<tr style=\"color:green;\"><td style=\"border: 1px solid black\">";
        strcat(row, contenido[0]);
        strcat(row, "</td></tr>");
        fprintf(html, "%s", row);
      } else {
        char row[] = "<tr><td style=\"border: 1px solid black\">";
        strcat(row, contenido[0]);
        strcat(row, "</td></tr>");
        fprintf(html, "%s", row);
      }
    }
  }
  return true;
}

int lecturaArchivoHTML(int opcion, FILE *csv, FILE *html) {
  FILE *cmd = popen("C:\\Users\\vivia\\Downloads\\wget -q -O - http://localhost:5000/static/", "r");
  char result[4096];
  int columnNumber = 0;       

  char ** results = malloc(16 * sizeof(char*));
  for(int j = 0; j < 16; j++){
    results[j] = (char*)malloc(50*sizeof(char));
  }

  char ** lineResults = malloc(16 * sizeof(char*));
  for(int j = 0; j < 16; j++){
    lineResults[j] = (char*)malloc(50*sizeof(char));
  }

  char *word; 
  while(fgets(result, sizeof(result), cmd) != NULL) {
    if (isTD(result) && endTD(result)) {
      lineResults = lecturaHastaTd(result);
      for(int i = 0; i < 15; i++)
      {
        if (strcmp(lineResults[i], "") != 0 && strcmp(lineResults[i], " ") != 0 && strcmp(lineResults[i], "  ") != 0){
          sprintf(results[columnNumber], lineResults[i]);
          columnNumber++;
        }
      }
    } else{
      if (endTD(result)) {
        int startIndex = getIndexEndTD(result);
        int endIndex = getIndexStartEndTD(result, startIndex);
        word = getWordByIndexes(result, endIndex, startIndex);
        if (strcmp(word, "") != 0 && strcmp(word, " ") != 0 && strcmp(word, "  ") != 0){
          sprintf(results[columnNumber], word);
          columnNumber++;
        }
      }
    }

    if (columnNumber >= 13) {
      obtenerCalculo(opcion, results, csv, html);
      columnNumber = 0;
    }
  }

  pclose(cmd);
}

int obtenerEncabezadoCsv(FILE *file) {
  char encabezado[] = "Especie; Precio de compra; Precio de venta; Apertura; Promedio;";
  fprintf(file, "%s \n", encabezado);
  return true;
}

int obtenerInicioTabla(FILE *file) {
  char table[] = "<HTML><BODY><table style=\"border: 1px solid black\"><thead><tr><th style=\"border: 1px solid black\">Especie</th></tr></thead><tbody>";
  fprintf(file, "%s \n", table);
  return true;
}

int obtenerFinTabla(FILE *file) {
  char table[] = "</tbody></table></BODY></HTML>";
  fprintf(file, "%s \n", table);
  return true;
}

int main(void) {
  char opcion[1];
  opcionesHabilitadas();
  gets(opcion);

  if (atoi(opcion) < 0 || atoi(opcion) > 4) {
    printf("Opcion invalida\n");
    return -1;
  }

  FILE *csv;
  if (atoi(opcion) == 2) {
    csv = fopen("promedio.csv", "w");
    obtenerEncabezadoCsv(csv);
  }

  FILE *html;
  if (atoi(opcion) == 3) {
    html = fopen("table.html", "w");
    obtenerInicioTabla(html);
  }

  lecturaArchivoHTML(atoi(opcion), csv, html);

  if (atoi(opcion) == 3) {
    obtenerFinTabla(html);
  }
  fclose(csv);
  fclose(html);
  printf("programa ejecutado exitosamente");
  return 0;
}