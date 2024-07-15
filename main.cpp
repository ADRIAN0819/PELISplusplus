#include <QCoreApplication>
#include <QTextStream>
#include <iostream>
#include "MovieSearcher.h"
#include "UserPreferences.h"

void printMenu() {
    std::cout << "\n--- MENU ---\n";
    std::cout << "1. Buscar película por letras\n";
    std::cout << "2. Buscar película por tags (géneros)\n";
    std::cout << "3. Ver películas 'Like'\n";
    std::cout << "4. Ver películas 'Ver más tarde'\n";
    std::cout << "5. Salir\n";
    std::cout << "Seleccione una opción: ";
}

void printMovies(const std::vector<QStringList> &movies, int startIndex) {
    std::cout << "\n--- Películas encontradas ---\n";
    for (size_t i = 0; i < movies.size(); ++i) {
        std::cout << startIndex + i + 1 << ". " << movies[i][1].toStdString() << std::endl;
    }
}

void printMovieDetails(const QStringList &movie) {
    std::cout << "\nSeleccionó la película:\n";
    std::cout << "IMDB ID: " << movie[0].toStdString() << std::endl;
    std::cout << "Title: " << movie[1].toStdString() << std::endl;
    std::cout << "Plot Synopsis: " << movie[2].toStdString() << std::endl;
    std::cout << "Tags: " << movie[3].toStdString() << std::endl;
    std::cout << "Split: " << movie[4].toStdString() << std::endl;
    std::cout << "Synopsis Source: " << movie[5].toStdString() << std::endl;
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    QString filePath = "mpst_fulldatadr_clean.csv";
    MovieSearcher searcher(filePath);
    UserPreferences* userPreferences = UserPreferences::getInstance();

    int opcion;
    std::string keyword;
    std::vector<QStringList> foundMovies;
    int startIndex = 0;

    do {
        printMenu();
        std::cin >> opcion;

        switch (opcion) {
            case 1:
                std::cout << "Introduzca letras para buscar: ";
                std::cin >> keyword;
                startIndex = 0;
                foundMovies = searcher.searchMovieByLetters(QString::fromStdString(keyword), startIndex);
                while (!foundMovies.empty()) {
                    printMovies(foundMovies, startIndex);
                    std::cout << "\nSeleccione una película por su número, ingrese 0 para ver más resultados, o -1 para volver al menú: ";
                    int selectedIndex;
                    std::cin >> selectedIndex;
                    if (selectedIndex == -1) {
                        break;
                    } else if (selectedIndex == 0) {
                        startIndex += 5;
                        foundMovies = searcher.searchMovieByLetters(QString::fromStdString(keyword), startIndex);
                    } else if (selectedIndex > 0 && selectedIndex <= static_cast<int>(foundMovies.size())) {
                        printMovieDetails(foundMovies[selectedIndex - 1]);
                        std::cout << "1. Añadir a 'Like'\n";
                        std::cout << "2. Añadir a 'Ver más tarde'\n";
                        std::cout << "3. Volver al menú principal\n";
                        int action;
                        std::cin >> action;
                        if (action == 1) {
                            userPreferences->addLike(foundMovies[selectedIndex - 1][1]);
                        } else if (action == 2) {
                            userPreferences->addWatchLater(foundMovies[selectedIndex - 1][1]);
                        }
                    }
                }
                break;

            case 2:
                std::cout << "Introduzca tags para buscar: ";
                std::cin >> keyword;
                startIndex = 0;
                foundMovies = searcher.searchMovieByTags(QString::fromStdString(keyword), startIndex);
                while (!foundMovies.empty()) {
                    printMovies(foundMovies, startIndex);
                    std::cout << "\nSeleccione una película por su número, ingrese 0 para ver más resultados, o -1 para volver al menú: ";
                    int selectedIndex;
                    std::cin >> selectedIndex;
                    if (selectedIndex == -1) {
                        break;
                    } else if (selectedIndex == 0) {
                        startIndex += 5;
                        foundMovies = searcher.searchMovieByTags(QString::fromStdString(keyword), startIndex);
                    } else if (selectedIndex > 0 && selectedIndex <= static_cast<int>(foundMovies.size())) {
                        printMovieDetails(foundMovies[selectedIndex - 1]);
                        std::cout << "1. Añadir a 'Like'\n";
                        std::cout << "2. Añadir a 'Ver más tarde'\n";
                        std::cout << "3. Volver al menú principal\n";
                        int action;
                        std::cin >> action;
                        if (action == 1) {
                            userPreferences->addLike(foundMovies[selectedIndex - 1][1]);
                        } else if (action == 2) {
                            userPreferences->addWatchLater(foundMovies[selectedIndex - 1][1]);
                        }
                    }
                }
                break;

            case 3:
                std::cout << "\nPelículas 'Like':\n";
                for (const QString &title : userPreferences->getLikes()) {
                    std::cout << title.toStdString() << std::endl;
                }
                break;

            case 4:
                std::cout << "\nPelículas 'Ver más tarde':\n";
                for (const QString &title : userPreferences->getWatchLater()) {
                    std::cout << title.toStdString() << std::endl;
                }
                break;

            case 5:
                std::cout << "Saliendo...\n";
                break;

            default:
                std::cout << "Opción no válida. Intente de nuevo.\n";
                break;
        }

    } while (opcion != 5);

    return a.exec();
}
