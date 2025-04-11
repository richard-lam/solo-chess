#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>

const int TILE_SIZE = 100;
const int BOARD_SIZE = 4;

sf::Vector2u size(static_cast<unsigned int>(BOARD_SIZE), static_cast<unsigned int>(TILE_SIZE));

// Example values for parameters
sf::VideoMode mode(size);  // Using the constructor that takes only a Vector2u
std::string title = "My Game";  // Window title
std::uint32_t style = sf::Style::Default;  // Window style (default style)
sf::ContextSettings settings;  // Default context settings

class Game {
public:
    Game()
        : window(mode, title, style, sf::State::Windowed)// settings)  // Adjusted constructor call (removed state)
    {
        // Load piece images
        if (!kingTexture.loadFromFile("assets/king.png") ||
            !queenTexture.loadFromFile("assets/queen.png") ||
            !rookTexture.loadFromFile("assets/rook.png") ||
            !bishopTexture.loadFromFile("assets/bishop.png") ||
            !knightTexture.loadFromFile("assets/knight.png") ||
            !pawnTexture.loadFromFile("assets/pawn.png")) {
            std::cerr << "Error loading piece textures!" << std::endl;
        }


        // Initialize board with blank tiles
        for (int x = 0; x < BOARD_SIZE; ++x) {
            for (int y = 0; y < BOARD_SIZE; ++y) {
                board[x][y].shape.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                board[x][y].shape.setPosition(sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE));
                board[x][y].shape.setFillColor((x + y) % 2 == 0 ? sf::Color::White : sf::Color::Black);
            }
        }
    }

    void placeAllPiecesRandomly() {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            for (int y = 0; y < BOARD_SIZE; ++y) {
                if (x == 0 && y == 0) {
                    board[x][y].sprite.setTexture(kingTexture);
                    board[x][y].sprite.setPosition(sf::Vector2f(static_cast<float>(x * TILE_SIZE), static_cast<float>(y * TILE_SIZE)));
                    board[x][y].active = true;
                } else if (x == 1 && y == 1) {
                    board[x][y].sprite.setTexture(queenTexture);
                    board[x][y].sprite.setPosition(sf::Vector2f(static_cast<float>(x * TILE_SIZE), static_cast<float>(y * TILE_SIZE)));
                    board[x][y].active = true;
                }
            }
        }
    }

    void run() {
        placeAllPiecesRandomly();

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                } else if (event.type == sf::Event::MouseButtonPressed) {
                    int x = event.mouseButton.x / TILE_SIZE;
                    int y = event.mouseButton.y / TILE_SIZE;

                    if (!isValid(x, y)) return;

                    if (selectedX == -1 && board[x][y].active) {
                        selectedX = x;
                        selectedY = y;
                    } else if (selectedX != -1) {
                        attemptMove(selectedX, selectedY, x, y);
                        selectedX = selectedY = -1;
                    }
                }
            }

            window.clear();
            drawBoard();
            window.display();
        }
    }

private:
    struct Tile {
        sf::RectangleShape shape;
        sf::Sprite sprite;
        bool active = false;

        // Explicit constructor to initialize Tile properly
        Tile() : shape(), sprite() {}
    };

    sf::RenderWindow window;
    sf::Texture kingTexture;
    sf::Texture queenTexture;
    Tile board[BOARD_SIZE][BOARD_SIZE];
    int selectedX = -1, selectedY = -1;

    bool isValid(int x, int y) {
        return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
    }

    void attemptMove(int fromX, int fromY, int toX, int toY) {
        board[toX][toY].sprite = board[fromX][fromY].sprite;
        board[fromX][fromY].active = false;
    }

    void drawBoard() {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            for (int y = 0; y < BOARD_SIZE; ++y) {
                window.draw(board[x][y].shape);
                if (board[x][y].active) {
                    window.draw(board[x][y].sprite);
                }
            }
        }
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}
