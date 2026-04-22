#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
int main()
{
    // initialisation des lines, colonnes et taille de cellule :
    int nb_lignes;
    int nb_colonnes;

    // demande à l'utilisateur de rentrer les paramètre pour la grille :
    do
    {
        std::cout << "Nombre de lignes (min 1) : ";
        if (!(std::cin >> nb_lignes))
        { // Si la saisie échoue (lettre, symbole...)
            std::cout << "Erreur : Veuillez entrer un nombre entier !" << std::endl;
            std::cin.clear();            // réinitialise l'état d'erreur
            std::cin.ignore(1000, '\n'); // On jette les caractères restants jusqu'à la ligne suivante
            continue;                    // On repart au début du do
        }
    } while (nb_lignes <= 0);
    do
    {
        std::cout << "Nombre de colonnes (min 1) : ";
        if (!(std::cin >> nb_colonnes))
        { // Si la saisie échoue (lettre, symbole...)
            std::cout << "Erreur : Veuillez entrer un nombre entier !" << std::endl;
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }
    } while (nb_colonnes <= 0);

    float taille_cellule = 20.f;

    // initialisation fenêtre d'affichage :
    sf::RenderWindow window(sf::VideoMode({(nb_colonnes * taille_cellule) + 150, nb_lignes * taille_cellule}), "Pixel art");

    // déclaration matrice :
    std::vector<std::vector<sf::RectangleShape>> grille(nb_lignes, std::vector<sf::RectangleShape>(nb_colonnes));

    // boucle pour créer les cellules :
    for (int i = 0; i < nb_lignes; i++)
    {
        for (int j = 0; j < nb_colonnes; j++)
        {
            grille[i][j].setSize(sf::Vector2f(taille_cellule, taille_cellule)); // définir la taille de chaque cellule
            grille[i][j].setPosition({j * taille_cellule, i * taille_cellule}); // mettre des accolades car setPosition attend qu'un seul argument sinon
            grille[i][j].setOutlineThickness(1.f);                              // épaisseur du contour
            grille[i][j].setOutlineColor(sf::Color::Black);                     // couleur du contour
            grille[i][j].setFillColor(sf::Color::White);                        // couleur de remplissage des cases
        }
    }

    // création du menu latéral :
    sf::RectangleShape fondMenu;
    fondMenu.setSize(sf::Vector2f(150.f, nb_lignes * taille_cellule));
    fondMenu.setPosition({nb_colonnes * taille_cellule, 0.f});
    fondMenu.setFillColor(sf::Color(45, 45, 45));

    // calcul position départ menu :
    float menu = nb_colonnes * taille_cellule;

    // carre de couleur pour le menu :
    std::vector<sf::Color> listeCouleurs = {sf::Color::Red, sf::Color::Yellow, sf::Color::Green, sf::Color::Blue, sf::Color::Black, sf::Color::White};
    std::vector<std::string> listeNoms = {"Rouge", "Jaune", "Vert", "Bleu", "Noir", "Gomme"};
    std::vector<sf::RectangleShape> indicateurs;
    for (int i = 0; i < listeCouleurs.size(); i++)
    {
        sf::RectangleShape temp;
        temp.setSize({30.f, 30.f});
        temp.setPosition({menu + 20.f, 50 + 50 * i});
        temp.setFillColor(listeCouleurs[i]);
        indicateurs.push_back(temp);
    }

    // declaration variables couleurs :
    sf::Color couleurPinceau = sf::Color::Black;
    std::string nomCouleur = "Noir";

    // boucle d'affichage :
    while (window.isOpen())
    {
        window.clear();
        while (const std::optional event = window.pollEvent())
        {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
            {
                couleurPinceau = sf::Color::White;
            }

            // est_ce que l'évènement est une touche pressée ?
            if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                // keyPressed->code =  contient quelle touche à été pressée :
                // touche R = Rouge :
                if (keyPressed->code == sf::Keyboard::Key::R)
                {
                    couleurPinceau = sf::Color::Red;
                    nomCouleur = "Rouge";
                }
                // touche G = Vert :
                if (keyPressed->code == sf::Keyboard::Key::G)
                {
                    couleurPinceau = sf::Color::Green;
                    nomCouleur = "Vert";
                }
                // touche Y = Jaune :
                if (keyPressed->code == sf::Keyboard::Key::Y)
                {
                    couleurPinceau = sf::Color::Yellow;
                    nomCouleur = "Jaune";
                }
                // touche N = Noir :
                if (keyPressed->code == sf::Keyboard::Key::N)
                {
                    couleurPinceau = sf::Color::Black;
                    nomCouleur = "Noir";
                }
                // touche B = Bleu :
                if (keyPressed->code == sf::Keyboard::Key::B)
                {
                    couleurPinceau = sf::Color::Blue;
                    nomCouleur = "Bleu";
                }

                // touche c = Clear :
                if (keyPressed->code == sf::Keyboard::Key::C)
                {
                    for (int i = 0; i < nb_lignes; i++)
                    {
                        for (int j = 0; j < nb_colonnes; j++)
                        {
                            grille[i][j].setFillColor(sf::Color::White);
                        }
                    }
                    nomCouleur = "Clear";
                }
            }

            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            // changement de titre :
            window.setTitle("Pixel Art - Couleur : " + nomCouleur);
        }

        // récupération de la position de la souris :
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        // calcule des indices :
        int col = mousePos.x / taille_cellule;
        int ligne = mousePos.y / taille_cellule;

        // verification souris = dans la grille ?
        if (ligne >= 0 && ligne < nb_lignes && col >= 0 && col < nb_colonnes)
        {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                grille[ligne][col].setFillColor(couleurPinceau);
            }

            // fonction gomme :
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
            {
                grille[ligne][col].setFillColor(sf::Color::White);
                nomCouleur = "Gomme";
            }
        }

        // dessiner sur la grille :
        for (int i = 0; i < nb_lignes; i++)
        {
            for (int j = 0; j < nb_colonnes; j++)
            {
                window.draw(grille[i][j]);
            }
        }

        // bordure entourée pour la couleur utilisée :
        for (int i = 0; i < indicateurs.size(); i++)
        {
            if (listeNoms[i] == nomCouleur)
            {
                indicateurs[i].setOutlineThickness(3.f);
                // On choisit la couleur de la bordure
                if (listeNoms[i] == "Gomme")
                {
                    indicateurs[i].setOutlineColor(sf::Color::Black);
                }
                else
                {
                    indicateurs[i].setOutlineColor(sf::Color::White);
                }
            }
            else
            {
                indicateurs[i].setOutlineThickness(0.f);
            }
        }

        // afficher le menu latéral :
        window.draw(fondMenu);

        // dessiner les rectangles :
        for (auto &rectangle : indicateurs)
        {
            window.draw(rectangle);
        }

        // afficher le résultat :
        window.display();
    }
}