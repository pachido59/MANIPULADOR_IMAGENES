#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int optionCASE = 0;
// Estructura para representar un pixel en escala de grises
struct Pixel {
    int value;
};

// Función para escribir los valores de los píxeles en un archivo PGM
void writePGMImage(int width, int height, Pixel **pixels, const std::string &filename) {
    std::ofstream outFile(filename);

    // Escribir el encabezado PGM
    outFile << "P2\n";
    outFile << width << " " << height << "\n";
    outFile << "255\n";

    // Escribir los valores de los píxeles en el archivo
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            outFile << pixels[i][j].value << " ";
        }
        outFile << "\n";
    }

    outFile.close();
}

// Función para calcular el histograma de una imagen PGM
std::vector<int> calculateHistogram(int width, int height, Pixel **pixels) {
    std::vector<int> histogram(256, 0); // Inicializar el histograma con 256 elementos, todos en 0

    // Calcular la frecuencia de cada nivel de intensidad de gris en la imagen
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            histogram[pixels[i][j].value]++;
        }
    }

    return histogram;
}

// Función para escribir los valores del histograma en un archivo CSV
void writeHistogramToCSV(const std::vector<int>& histogram, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error al abrir el archivo " << filename << " para escribir el histograma." << std::endl;
        return;
    }

    // Escribir los valores del histograma en el archivo CSV
    for (int i = 0; i < histogram.size(); ++i) {
        outFile << i << "," << histogram[i] << std::endl;
    }

    outFile.close();
}

// Función para generar y guardar la imagen modificada
void generateAndSaveModifiedImage(int width, int height, Pixel **pixels, const std::string &filename) {
    // Crear y guardar la imagen modificada
    writePGMImage(width, height, pixels, filename);
    std::cout << "Imagen modificada creada exitosamente: " << filename << std::endl;
}

// Función para cargar una imagen PGM existente
void loadPGMImage(const char *filename, int &width, int &height, Pixel **&pixels) {
    std::ifstream inFile(filename);

    if (!inFile.is_open()) {
        std::cerr << "Error al abrir el archivo " << filename << std::endl;
        return;
    }

    std::string format;
    inFile >> format;
    if (format != "P2") {
        std::cerr << "Formato de archivo no compatible. Debe ser PGM P2." << std::endl;
        inFile.close();
        return;
    }

    inFile >> width >> height;
    int maxVal;
    inFile >> maxVal;

    // Asignar memoria dinámica para almacenar los píxeles
    pixels = new Pixel*[height];
    for (int i = 0; i < height; ++i) {
        pixels[i] = new Pixel[width];
    }

    // Leer los valores de los píxeles del archivo
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            inFile >> pixels[i][j].value;
        }
    }

    inFile.close();
}

// Función para copiar una imagen PGM
void copyPGMImage(int width, int height, Pixel **sourcePixels, Pixel **&destinationPixels) {
    // Asignar memoria dinámica para almacenar los píxeles de destino
    destinationPixels = new Pixel*[height];
    for (int i = 0; i < height; ++i) {
        destinationPixels[i] = new Pixel[width];
    }

    // Copiar los valores de los píxeles de la imagen fuente a la imagen de destino
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            destinationPixels[i][j].value = sourcePixels[i][j].value;
        }
    }
}

// Función para incrementar el brillo de una imagen PGM
void increaseBrightness(int width, int height, Pixel **pixels, int variableS) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            pixels[i][j].value = std::min(255, pixels[i][j].value + variableS); // Incrementa el brillo en 110
        }
    }
}

// Función para disminuir el brillo de una imagen PGM
void decreaseBrightness(int width, int height, Pixel **pixels, int variable) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            pixels[i][j].value = std::max(0, pixels[i][j].value - variable); // Disminuye el brillo en 110
        }
    }
}

//Función para restar una imagen de otra
void minusImage(int width, int height, Pixel **pixels1, Pixel**pixels2) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
             pixels1[i][j].value = pixels2[i][j].value - pixels1[i][j].value;
        }
        
    }
}

// Función para obtener el negativo de una imagen
void NegativeImage(int width, int height, Pixel **pixels) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            pixels[i][j].value = 255 - pixels[i][j].value; 
        }
    }
}

// Función para segmentar una imagen PGM basada en un punto de referencia de 128
void referencePointSegmentation(int width, int height, Pixel **pixels) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (pixels[i][j].value < 128) {
                pixels[i][j].value = 0;
            } else {
                pixels[i][j].value = 255;
            }
        }
    }
}

int main() {
    int width = 0; // Ancho de la imagen
    int height = 0; // Alto de la imagen
    std::string filename;
    std::string filename2;
    int again;
    Pixel **sourcePixels = nullptr;
    Pixel **sourcePixels2 = nullptr;
    Pixel **copiedPixels = nullptr;
    Pixel **copiedPixels2 = nullptr;

    do {
        system("cls");
        std::cout << "Generador de imagenes .pgm\n";
        std::cout << "Ingrese el nombre de la primera imagen .pgm que desea copiar: ";
        std::cin >> filename;

        // Cargar la imagen .pgm seleccionada por el usuario
        loadPGMImage(filename.c_str(), width, height, sourcePixels);


        // Copiar la imagen seleccionada
        copyPGMImage(width, height, sourcePixels, copiedPixels);

        std::cout << "Ingrese el nombre de la segunda imagen .pgm que desea copiar: ";
        std::cin >> filename2;

        // Cargar la imagen .pgm seleccionada por el usuario
        loadPGMImage(filename2.c_str(), width, height, sourcePixels2);

        // Copiar la imagen seleccionada
        copyPGMImage(width, height, sourcePixels2, copiedPixels2);
        // Solicitar al usuario que elija la operación de modificación de la imagen
        int choice;


        std::cout << "Seleccione la operacion a realizar:\n";
        std::cout << "1- Incrementar brillo\n";
        std::cout << "2- Disminuir brillo\n";
        std::cout << "3- Segmentar\n";
        std::cout << "4- Restar una imagen de otra\n";
        std::cout << "5- Negativo de una imagen\n";
        std::cout << "6- Histograma de una imagen\n";
        std::cout << "7- Salir\n";
        std::cin >> choice;

        // Realizar la operación seleccionada
        switch (choice) {
            case 1:{
                
                int valorS;
                std::cout << "Seleccione la imagen a aumentar el brillo\n";
                std::cout << "1- Imagen 1\n";
                std::cout << "2- Imagen 2\n";
                std::cin >> optionCASE;
                std::cout << "Ingrese el valor que quiere incrementar\n(Considere el rango de valor por pixel = [0,255]): ";
                std::cin >> valorS;
                if(optionCASE == 1){
                    increaseBrightness(width, height, copiedPixels,valorS);
                }
                else{
                    increaseBrightness(width, height, copiedPixels2,valorS);
                }
            }
                break;
            case 2:
                
                int valorR;
                std::cout << "Seleccione la imagen a disminuir el brillo\n";
                std::cout << "1- Imagen 1\n";
                std::cout << "2- Imagen 2\n";
                std::cin >> optionCASE;
                std::cout << "Ingrese el valor que quiere reducir\n(Considere el rango de valor por pixel = [0,255]): ";
                std::cin >> valorR;
                if(optionCASE == 1){
                    decreaseBrightness(width, height, copiedPixels,valorR);
                }
                else{
                    decreaseBrightness(width, height, copiedPixels2,valorR);
                }
                break;
            case 3:    
            
                std::cout << "Seleccione la imagen a segmentar\n";
                std::cout << "1- Imagen 1\n";
                std::cout << "2- Imagen 2\n";
                std::cin >> optionCASE;
                if(optionCASE == 1){
                    referencePointSegmentation(width, height, copiedPixels);
                }
                else{
                    referencePointSegmentation(width, height, copiedPixels2);
                }
                break;
            case 4:
                minusImage(width, height, copiedPixels,copiedPixels2);
            break;
            case 5:{
        
        
            std::cout << "Seleccione la imagen a modificar\n";
            std::cout << "1- Imagen 1\n";
            std::cout << "2- Imagen 2\n";
            std::cin >> optionCASE;
    
            if(optionCASE == 1){
            NegativeImage(width, height, copiedPixels);
            }
            else{
            NegativeImage(width, height,copiedPixels2);
            }
            }
            break;
            case 6:{
            int optionH;
        std::cout << "Seleccione el histograma de la imagen deseada\n";
        std::cout << "1- Imagen 1\n";
        std::cout << "2- Imagen 2\n";
        std::cin >> optionH;

        if(optionH == 1){
            // Calcular el histograma de la imagen deseada
            std::vector<int> histogram = calculateHistogram(width, height, sourcePixels);
            std::string histogramFilename = "histograma_" + filename + ".csv";
            writeHistogramToCSV(histogram, histogramFilename);
        }

        else{
        // Calcular el histograma de la imagen y guardar los valores en un archivo CSV
        std::vector<int> histogram = calculateHistogram(width, height, sourcePixels2);
        std::string histogramFilename = "histograma_" + filename2 + ".csv";
        writeHistogramToCSV(histogram, histogramFilename);
        }
            }
            break;
            case 7:
                std::cout << "Saliendo del programa, bonito dia :)\n";
                return 0;
            default:
                std::cout << "Opción no válida. Saliendo del programa.\n";
                return 1;
        }

        
        if(optionCASE == 1){
        // Crear y guardar la imagen modificada 1
        std::string outputFilename = "imagen_modificada_" + filename;
        generateAndSaveModifiedImage(width, height, copiedPixels, outputFilename);
        }
        if(optionCASE == 2){       
        // Crear y guardar la imagen modificada 2
        std::string outputFilename2 = "imagen_modificada_" + filename2;
        generateAndSaveModifiedImage(width, height, copiedPixels2, outputFilename2);
        }
        std::cout << "¿Desea hacer otra operacion?\n";
        std::cout << "1- Si\n";
        std::cout << "2- No\n";
        std::cin >> again;

        // Liberar memoria
        for (int i = 0; i < height; ++i) {
            delete[] sourcePixels[i];
            delete[] copiedPixels[i];
            delete[] copiedPixels2[i];
        }
        delete[] sourcePixels;
        delete[] copiedPixels;
        delete[] copiedPixels2;
        
    } while(again == 1);

    return 0;
}
