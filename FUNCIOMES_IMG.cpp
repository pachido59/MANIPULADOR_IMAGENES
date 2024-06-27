#include <iostream>
#include <fstream>
#include <string>

// Estructura para representar un pixel en escala de grises
struct Pixel {
    int value;
};

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
void increaseBrightness(int width, int height, Pixel **pixels) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            pixels[i][j].value = std::min(255, pixels[i][j].value + 50); // Incrementa el brillo en 50
        }
    }
}

// Función para disminuir el brillo de una imagen PGM
void decreaseBrightness(int width, int height, Pixel **pixels) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            pixels[i][j].value = std::max(0, pixels[i][j].value - 50); // Disminuye el brillo en 50
        }
    }
}

// Función para crear una imagen PGM con memoria dinámica
void createPGMImage(int width, int height, Pixel **pixels, const char *filename) {
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
