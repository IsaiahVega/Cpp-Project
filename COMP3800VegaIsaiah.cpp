/*
*   Proyecto: Sistema de Gestión de Cursos Universitarios
*   Archivo: COMP3800VegaIsaiah08.cpp
*
*   Descripción:
*   Este prograna procesa información académica de un estudiante 
*   (nombre,grado, fecha, cursos matriculados, aprobados y restantes).
*   A partir de un archivo de texto, organiza los datos, determina los 
*   cursos faltantes según el currículo (BSCS o ACSC, versiones 2024- o 2024+)
*   muestra la información en consola y permite exportarla en formato .txt o .cvs
*   del usuario desearlo
*
*   Autores:
*     - Isaiah X. Vega Rodriguez
      - https://youtu.be/-sAkc-yxUv8?si=E_FhPU0D8UyXUbDS -> Vid1
      - https://youtu.be/KhJ-3_s33L0?si=_gN3Q29Te0gZmZAh -> Vid2 

*     - Alejandro G. Rodriguez Ramos
*       https://youtu.be/IvzyRZ-XX7Y -> Vid1
*       https://youtu.be/7H1J3wCQLw0 -> Vid2
*        
*     - Yairemary Mendez Lopez
*       https://youtu.be/LE3_FojqNIA -> Vid1
*       https://youtu.be/PdRDUbMSoyU -> Vid2
*
*   Fecha de creación: 15-09-2025
*   Última modificación: 25-09-2025
*   Versión: 2.0
*/

// Librerias necesarias del programa
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm> // Autorizado por el Prof. Navarro 09-24-2025
#include <cctype>    // Autorizado por el Prof. Navarro 09-24-2025

// ----- Listas de cursos por programa/curriculo -----
std::string bscsArray[] = {
    "COMP2120", "GEIC1010", "GEMA1200", "COMP2025",
    "COMP2315", "MATH1500", "COMP2400", "COMP2501",
    "COMP2800", "MATH2251", "COMP2502", "COMP2900",
    "COMP3015", "COMP3300", "COMP3900", "PHYS3001",
    "COMP3400", "COMP3500", "COMP4200", "COMP4400",
    "COMP4420", "COMP4910", "COMP2850", "COMP3600",
    "COMP3800", "COMP397_", "COMP4160", "COMP4210",
    "COMP4415", "COMP4480", "COMP4580", "MATH2100"},

     bscsArray2024[] = {
    "COMP2051", "COMP2120", "COMP2200", "GEMA1200",
    "COMP2052", "COMP2315", "COMP2800", "COMP2053",
    "COMP2400", "COMP2700", "MATH1500", "COMP2900", 
    "GEIC1010", "COMP2501", "COMP3300", "COMP3350", 
    "MATH2251", "COMP2502", "COMP2850", "COMP3401", 
    "COMP3402", "COMP3500", "COMP4250", "COMP4910", 
    "COMP3600", "COMP3800", "COMP397_", "COMP4170", 
    "COMP4415", "COMP4480", "COMP4580", "MATH2100"}, 

     ascsArray[] = {
    "COMP2020", "GEIC1010", "GEMA1200", "COMP2025",
    "COMP2315", "MATH1500", "COMP2400", "COMP2501",
    "COMP2800", "COMP2850", "COMP2900", "COMP2970",
    "COMP3015"}, 
              
     ascsArray2024[] = {
    "COMP2051", "COMP2120", "COMP2200", "GEMA1200", 
    "COMP2052", "COMP2315", "COMP2800", "COMP2053", 
    "COMP2400", "COMP2700", "MATH1500", "COM2970",
    "GEIC1010"},

    cursosMatriculados[32], 
    cursosAprobados[32], 
    cursosRestantes[32];

// Variables globales que manejan datos y contadores
std::string fileName,
    docLine,
    nombre,
    grado,
    fecha,
    csvFile,
    csvQstn,
    txtFile,
    txtQstn,
    preguntaExp;

char dummy;

int matriculadosCount = 0,
    aprobadosCount = 0,
    restantesCount = 0;

bool YesOrNo = true,
     confirm = true;

/*
Parametros: N/A
Pre condiciones: Ya se deben haber guardado el nombre, grado, fecha y los cursos en las listas.
Post-condiciones: Se imprime en la consola la información del estudiante junto a los cursos matriculados, aprobados y restantes.
Autor: Alejandro G. Rodriguez Ramos
Fecha: 09-23-2025
Nota: Esta función solo muestra en consola los datos procesados previamente.
*/
void imprimirDatos()
{
    // Muestra el nombre, grado y fecha
    std::cout << "\nNombre: " + nombre << std::endl;
    std::cout << "Grado: " + grado << std::endl;
    std::cout << "Fecha: " + fecha + "\n"
              << std::endl;

    // Cursos Matriculados
    std::cout << "Cursos Matriculados:" << std::endl;
    for (int i = 0; i < matriculadosCount; i++)
    {
        std::cout << cursosMatriculados[i].insert(4, " ") << std::endl;
    }

    // Cursos Aprobados
    std::cout << "\nCursos Aprobados:" << std::endl;
    for (int i = 0; i < aprobadosCount; i++)
    {
        std::cout << cursosAprobados[i].insert(4, " ") << std::endl;
    }

    // Cursos Restantes
    std::cout << "\nCursos Restantes:" << std::endl;
    for (int i = 0; i < restantesCount; i++)
    {
        std::cout << cursosRestantes[i].insert(4, " ") << std::endl;
    }
}

/*
Parametros: fileText = texto que viene del archivo.
Pre condiciones: El texto puede venir con espacios o en minúsculas.
Post-condiciones: Devuelve el texto en mayúsculas y sin espacios
Autor: Alejandro G. Rodriguez Ramos
Fecha: 09-24-2025
Nota: Se usa para que todos lo snombres del curso estén en el mismo formato.
*/

// Quita espacios y convierte a mayúsculas
std::string formatLine(std::string fileText)
{

    std::string formattedLine = fileText;

    // 1) Remove whitespace: elimina espacios, tabs y saltos
    formattedLine.erase(std::remove_if(formattedLine.begin(), formattedLine.end(), ::isspace), formattedLine.end());

    // 2) Convert to uppercase: pone todo en mayúsculas para comparar fácilmente
    std::transform(formattedLine.begin(), formattedLine.end(), formattedLine.begin(), ::toupper);

    return formattedLine;
}
/*
Parametros: fileName = nombre del archivo de texto con la información del estudiante
Pre condiciones: El archivo debe existir y tener el formato correcto (nombre, grado, año, cursos).
Post-condiciones: Guarda en las listas los cursos matriculados, aprobados y faltantes, además del nombre, grado y fecha.
Autor: Alejandro G. Rodriguez Ramos
Fecha: 09-23-2025
Nota: Se adapta al programa (BSCS o ASCS) y al currículo (2024- o 2024+)
*/
void procesarDatos(std::string fileName)
{
   // Abre el archivo
    std::ifstream studentFile(fileName);

    // Si no existe, pide al usuario el nombre de nuevo hasta que lo encuentre.
    while (!studentFile.is_open())
    {
        std::cout << "Archivo no encontrado! Intente nuevamente.\n";
        std::cin >> fileName;
        studentFile.open(fileName);
    }

    // Lee línea por línea
    while (getline(studentFile, docLine))
    {
        // Si nombre está vacío, tomamos la primera línea como nombre del estudiante
        if (nombre == "")
        {
            nombre = docLine;
        }

        //Si la línea dice "BSCS" pertenece a bachillerato
        if (docLine == "BSCS")
        {
            grado = docLine;
            
            // Ahora se busca la línea que diga "2024-" o "2024+" y leemos los cursos según eso.
            while (getline(studentFile, docLine))
            {
                if (docLine == "2024-")
                {
                    fecha = docLine;

                   // Buscar "Cursos Matriculados" y leer hasta "Cursos Aprobados"
                    while (getline(studentFile, docLine))
                    {
                        if (docLine == "Cursos Matriculados")
                        {
                            while (getline(studentFile, docLine))
                            {
                                if (docLine == "Cursos Aprobados")
                                {
                                    // A partir de aquí, lo que quede son cursos aprobados (hasta EOF)
                                    while (getline(studentFile, docLine))
                                    {
                                        cursosAprobados[aprobadosCount] = formatLine(docLine);
                                        aprobadosCount++;
                                    }
                                    break; // sale del bucle de matriculados
                                }

                                // Guardar cada curso matriculado 
                                cursosMatriculados[matriculadosCount] = formatLine(docLine);
                                matriculadosCount++;
                            }
                        }
                    }

                    // Marcaren bscsArray los cursos que están en matriculados/aprobados.
                    // Se pone "" donde ya está cursado para luego identificar los restantes.
                    for (int i = 0; i < 32; i++)
                    {
                        for (int j = 0; j < matriculadosCount; j++)
                        {
                            if (bscsArray[i] == cursosMatriculados[j])
                            {
                                bscsArray[i] = "";
                            }
                        }
                    }

                    for (int i = 0; i < 32; i++)
                    {
                        for (int j = 0; j < aprobadosCount; j++)
                        {
                            if (bscsArray[i] == cursosAprobados[j])
                            {
                                bscsArray[i] = "";
                            }
                        }
                    }
                    // Lo que queda en bscsArray (no vaciado) son los cursos restantes.
                    for (int c = 0; c < 32; c++)
                    {
                        if (bscsArray[c] != "")
                        {
                            cursosRestantes[restantesCount] = bscsArray[c];
                            restantesCount++;
                        }
                    }
                }
                // Si el currículo es "2024+" hacemos proceso similar pero con bscsArray2024
                if (docLine == "2024+")
                {
                    fecha = docLine;
                    while (getline(studentFile, docLine))
                    {
                        if (docLine == "Cursos Matriculados")
                        {
                            while (getline(studentFile, docLine))
                            {
                                if (docLine == "Cursos Aprobados")
                                {
                                    while (getline(studentFile, docLine))
                                    {
                                        cursosAprobados[aprobadosCount] = formatLine(docLine);
                                        aprobadosCount++;
                                    }
                                    break;
                                }
                                cursosMatriculados[matriculadosCount] = formatLine(docLine);
                                matriculadosCount++;
                            }
                        }
                    }
                    for (int i = 0; i < 32; i++)
                    {
                        for (int j = 0; j < matriculadosCount; j++)
                        {
                            if (bscsArray2024[i] == cursosMatriculados[j])
                            {
                                bscsArray2024[i] = "";
                            }
                        }
                    }

                    for (int i = 0; i < 32; i++)
                    {
                        for (int j = 0; j < aprobadosCount; j++)
                        {
                            if (bscsArray2024[i] == cursosAprobados[j])
                            {
                                bscsArray2024[i] = "";
                            }
                        }
                    }

                    for (int c = 0; c < 32; c++)
                    {
                        if (bscsArray2024[c] != "")
                        {
                            cursosRestantes[restantesCount] = bscsArray2024[c];
                            restantesCount++;
                        }
                    }
                }
            }
        }

        // Si la línea dice "ASCS" pertenece al asociado.
        if (docLine == "ASCS")
        {
            grado = docLine;
            while (getline(studentFile, docLine))
            {
                if (docLine == "2024-")
                {
                    fecha = docLine;
                    while (getline(studentFile, docLine))
                    {
                        if (docLine == "Cursos Matriculados")
                        {
                            while (getline(studentFile, docLine))
                            {
                                if (docLine == "Cursos Aprobados")
                                {
                                    while (getline(studentFile, docLine))
                                    {
                                        cursosAprobados[aprobadosCount] = formatLine(docLine);
                                        aprobadosCount++;
                                    }
                                    break;
                                }
                                cursosMatriculados[matriculadosCount] = formatLine(docLine);
                                matriculadosCount++;
                            }
                        }
                    }
                    // Vaciar los ya matriculados/aprobados de la lista de asociado (ascsArray)
                    for (int i = 0; i < 13; i++)
                    {
                        for (int j = 0; j < matriculadosCount; j++)
                        {
                            if (ascsArray[i] == cursosMatriculados[j])
                            {
                                ascsArray[i] = "";
                            }
                        }
                    }

                    for (int i = 0; i < 13; i++)
                    {
                        for (int j = 0; j < aprobadosCount; j++)
                        {
                            if (ascsArray[i] == cursosAprobados[j])
                            {
                                ascsArray[i] = "";
                            }
                        }
                    }

                    for (int c = 0; c < 13; c++)
                    {
                        if (ascsArray[c] != "")
                        {
                            cursosRestantes[restantesCount] = ascsArray[c];
                            restantesCount++;
                        }
                    }
                }
                if (docLine == "2024+")
                {
                    fecha = docLine;
                    while (getline(studentFile, docLine))
                    {
                        if (docLine == "Cursos Matriculados")
                        {
                            while (getline(studentFile, docLine))
                            {
                                if (docLine == "Cursos Aprobados")
                                {
                                    while (getline(studentFile, docLine))
                                    {
                                        cursosAprobados[aprobadosCount] = formatLine(docLine);
                                        aprobadosCount++;
                                    }
                                    break;
                                }
                                cursosMatriculados[matriculadosCount] = formatLine(docLine);
                                matriculadosCount++;
                            }
                        }
                    }
                    for (int i = 0; i < 13; i++)
                    {
                        for (int j = 0; j < matriculadosCount; j++)
                        {
                            if (ascsArray2024[i] == cursosMatriculados[j])
                            {
                                ascsArray2024[i] = "";
                            }
                        }
                    }

                    for (int i = 0; i < 13; i++)
                    {
                        for (int j = 0; j < aprobadosCount; j++)
                        {
                            if (ascsArray2024[i] == cursosAprobados[j])
                            {
                                ascsArray2024[i] = "";
                            }
                        }
                    }

                    for (int c = 0; c < 13; c++)
                    {
                        if (ascsArray2024[c] != "")
                        {
                            cursosRestantes[restantesCount] = ascsArray2024[c];
                            restantesCount++;
                        }
                    }
                }
            }
        }
    }
}

//
/*
Parametros: csvQstn = opción del usuario (y/n) para exportar.
Pre condiciones: Ya deben estar listas las listas de cursos y datos del estudiante.
Post-condiciones: Si el usuario dice "y", crea un archivo con extensión .cvs con los datos del estudiante.
Autor: Isaiah X. Vega Rodríguez
Fecha: 09-21-2025
Nota: Solo se acepta guardar si la extensión es .cvs.
*/
void excelExport(std::string csvQstn)
{
    // Si la respuesta ya viene en el formato esperado, no pedirla de nuevo.
    if (csvQstn == "y" || csvQstn == "n")
    {
        YesOrNo = false;
    }

    // Si la variable YesOrNo sigue siendo true, pedimos la opción hasta que sea válida
    while (YesOrNo)
    {
        std::cout << "Error! Entre nuevamente sí o no en el siguiente formato: (y/n)";
        std::cin >> csvQstn;
        if (csvQstn == "y" || csvQstn == "n")
        {
            YesOrNo = false;
        }
    }

    // Pide al estudiante que entre el nombre del archivo que desea generar sin la extensión
    if (csvQstn == "y")
    {
        // Pide el nombre del archivo con extensión .cvs
        std::cout << "Ingrese el nombre del archivo con la extensión (.csv) para exportar la informacion al docuemtno. (ej.: Pancho.cvs)" << std::endl;
        std::cin >> csvFile;

        YesOrNo = true;

        // Verifica que la cadena (string) termine con ".cvs".
        while (YesOrNo)
        {
            if (csvFile.size() >= 4 && csvFile.substr(csvFile.size() - 4) == ".csv")
            {
                YesOrNo = false;
            }
            else
            {
                std::cout << "Estás colocando una extensión que no es .csv. Favor de asegurarse\n de que el archivo termine en .csv" << std::endl;
                std::cin >> csvFile;
            }
        }
        std::ofstream excelFile(csvFile);

        // Escribimos el encabezado
        excelFile << "Nombre: " << nombre << "\nGrado: " << grado << "\nFecha: " << fecha << "\n";

        // Cursos Matriculados
        excelFile << "\nCursos Matriculados:\n";
        for (int i = 0; i < matriculadosCount; i++)
        {
            excelFile << cursosMatriculados[i] << "\n";
        }
        // Cursos Aprobados
        excelFile << "\nCursos Aprobados:\n";
        for (int i = 0; i < matriculadosCount; i++)
        {
            excelFile << cursosMatriculados[i] << "\n";
        }
        // Cursos Restantes
        excelFile << "\nCursos Restantes:\n";
        for (int i = 0; i < restantesCount; i++)
        {
            excelFile << cursosRestantes[i] << "\n";
        }
        excelFile.close();
    }
}

/* 
Parametros: txtQstn = opción del usuario (y/n) para exportar.
Pre condiciones: Ya deben estar preparadas las listas de cursos y datos del estudiante.
Post-condiciones: Si el usuario dice "y", crea un archivo con extensión .txt con los datos del estudiante.
Autor: Yairemary Mendez Lopez
Fecha: 09-22-2025
Nota: Solo acepta guardar si la extensión es .txt
*/
void textExport(std::string txtQstn)
{
    if (txtQstn == "y" || txtQstn == "n")
    {
        YesOrNo = false;
    }

    // Si por error presiona otra tecla que no es, preguntará nuevamente hasta que el usuario escriba
    // el input en el formato correcto.
    while (YesOrNo)
    {
        std::cout << "Error! Entre nuevamente sí o no en el siguiente formato: (y/n)";
        std::cin >> txtQstn;
        if (txtQstn == "y" || txtQstn == "n")
        {
            YesOrNo = false;
        }
    }

    // Pide al estudiante que entre el nombre del archivo que desea generar sin la extensión
    if (txtQstn == "y")
    {

        std::cout << "Ingrese el nombre del archivo con la extensión (.txt) para exportar la informacion al docuemtno. (ej.: Pancho.txt)" << std::endl;
        
        std::cin >> txtFile;

        YesOrNo = true;

        // Verifica si el usuario ingresó la extensión correcta si no le pedirá que entre nuevamente la extensión.
        while (YesOrNo)
        {
            if (txtFile.size() >= 4 && txtFile.substr(txtFile.size() - 4) == ".txt")
            {
                YesOrNo = false;
            }
            else
            {
                std::cout << "Estás colocando una extensión que no es .txt. Favor de asegurarse\n de que el archivo termine en .txt" << std::endl;
                std::cin >> txtFile;
            }
        }
        std::ofstream textFile(txtFile);

        textFile << "Nombre: " << nombre << "\nGrado: " << grado << "\nFecha: " << fecha << "\n";

        textFile << "\nCursos Matriculados:\n";
        for (int i = 0; i < matriculadosCount; i++)
        {
            textFile << cursosMatriculados[i] << "\n";
        }
        textFile << "\nCursos Aprobados:\n";
        for (int i = 0; i < matriculadosCount; i++)
        {
            textFile << cursosMatriculados[i] << "\n";
        }
        textFile << "\nCursos Restantes:\n";
        for (int i = 0; i < restantesCount; i++)
        {
            textFile << cursosRestantes[i] << "\n";
        }
        textFile.close();
    }
}

/* 
Parametros: N/A
Pre condiciones: Debe existir un archivo de texto con la información del estudiante
Post-condiciones: Pide el archivo, procesa la información, muestra los datos en pantalla y pregunta si se quiere exportar a texto o Excel
Autor: Isaiah X. Vega Rodríguez, Alejandro G. Rodríguez Ramos, Yairemary Mendez Lopez
Fecha: 09-25-2025
Nota: Controla el flujo del programa utilizando las funciones establecidas.
*/

int main()
{
    // Pide el nombre del archivo
    std::cout << "Entre el nombre del archivo con los cursos aprobados y matriculados: " << std::endl;
   
    std::cin >> fileName;
    
    // Procesa datos del archivo y llena las listas globales
    procesarDatos(fileName);
    
    // Muesra resultados en pantalla
    imprimirDatos();

    // Bucle para preguntar si desea exportar; se repite hasta recibir y/n
    while (confirm)
    {

        std::cout << "Deseas exportar la informacion a un documento? (y/n)" << std::endl;
        std::cin >> preguntaExp;
       
        if (preguntaExp == "y")
        {

            confirm = false;
 
           // Pregunta si quiere archivo .txt
            std::cout << "\nAntes de que el programa termine, deseas crear un archivo de Texto? (y/n)" << std::endl;
            std::cin >> txtQstn;
            textExport(txtQstn);

            // Pregutna si quiere archivo .csv
            std::cout << "\nAntes de que el programa termine, deseas crear un archivo de Excel? (y/n)" << std::endl;
            std::cin >> csvQstn;
            excelExport(csvQstn);
        }
        else if (preguntaExp == "n")
        {
            confirm = false;
        }
    }

    std::cout << "\nGracias por usar el programa!\n" << std::endl;

    std::cout << "Favor entrar un caracter y presionar enter para culminar la ejeucion del programa!\n";
    std::cin >> dummy;

    return 0;
}
