#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

const int WIDTH = 800;
const int HEIGHT = 600;

const float LANE_X[3] = {250.f, 400.f, 550.f};

struct Entity {
    sf::RectangleShape shape; // used for collision + positioning
    int lane;
};xx

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Shadow Runner");
    window.setFramerateLimit(60);

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // ================= PLAYER =================
    Entity player;
    player.lane = 1;
    player.shape.setSize({40, 60});
    player.shape.setPosition(LANE_X[player.lane], 450);

    sf::Texture playerTexture;
    sf::Sprite playerSprite;

    if (!playerTexture.loadFromFile(
    "D:/shadowrunner_game/shadowrunner/bin/Debug/assets/player.png")) {
        return -1;
    }

    playerSprite.setTexture(playerTexture);
    playerSprite.setScale(0.5f, 0.5f);
    playerSprite.setPosition(player.shape.getPosition());

    // ================= MONSTER =================
    sf::Texture monsterTexture;
    if (!monsterTexture.loadFromFile("D:/shadowrunner_game/shadowrunner/bin/Debug/assets/monster.png")) {
        return -1;
    }

    // ================= COIN =================
    sf::Texture coinTexture;
    if (!coinTexture.loadFromFile("D:/shadowrunner_game/shadowrunner/bin/Debug/assets/coin.png")) {
        return -1;
    }

    std::vector<Entity> enemies;
    std::vector<sf::Sprite> enemySprites;

    std::vector<Entity> coins;
    std::vector<sf::Sprite> coinSprites;

    float spawnTimer = 0.f;
    float speed = 4.f;
    int score = 0;
    bool gameOver = false;

    sf::Clock clock;

    // ================= GAME LOOP =================
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // ========== RESTART ==========
        if (gameOver && sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            enemies.clear();
            enemySprites.clear();
            coins.clear();
            coinSprites.clear();
            score = 0;
            speed = 4.f;
            gameOver = false;
            player.lane = 1;
        }

        if (!gameOver) {
            // ========== INPUT ==========
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                player.lane = std::max(0, player.lane - 1);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                player.lane = std::min(2, player.lane + 1);

            player.shape.setPosition(LANE_X[player.lane], 450);
            playerSprite.setPosition(player.shape.getPosition());

            // ========== SPAWNING ==========
            spawnTimer += dt;
            if (spawnTimer > 1.f) {
                spawnTimer = 0.f;

                // Coin
                Entity coin;
                coin.lane = rand() % 3;
                coin.shape.setSize({30, 30});
                coin.shape.setPosition(LANE_X[coin.lane], -30);
                coins.push_back(coin);

                sf::Sprite coinSprite;
                coinSprite.setTexture(coinTexture);
                coinSprite.setScale(0.4f, 0.4f);
                coinSprite.setPosition(coin.shape.getPosition());
                coinSprites.push_back(coinSprite);

                // Enemy
                if (rand() % 2 == 0) {
                    Entity enemy;
                    enemy.lane = rand() % 3;
                    enemy.shape.setSize({40, 60});
                    enemy.shape.setPosition(LANE_X[enemy.lane], -60);
                    enemies.push_back(enemy);

                    sf::Sprite enemySprite;
                    enemySprite.setTexture(monsterTexture);
                    enemySprite.setScale(0.5f, 0.5f);
                    enemySprite.setPosition(enemy.shape.getPosition());
                    enemySprites.push_back(enemySprite);
                }
            }

            // ========== MOVE COINS ==========
            for (size_t i = 0; i < coins.size(); i++) {
                coins[i].shape.move(0, speed * 100 * dt);
                coinSprites[i].setPosition(coins[i].shape.getPosition());
            }

            // ========== MOVE ENEMIES ==========
            for (size_t i = 0; i < enemies.size(); i++) {
                enemies[i].shape.move(0, speed * 100 * dt);
                enemySprites[i].setPosition(enemies[i].shape.getPosition());
            }

            // ========== COIN COLLISION ==========
            for (size_t i = 0; i < coins.size();) {
                if (coins[i].shape.getGlobalBounds().intersects(player.shape.getGlobalBounds())) {
                    score++;
                    coins.erase(coins.begin() + i);
                    coinSprites.erase(coinSprites.begin() + i);
                } else if (coins[i].shape.getPosition().y > HEIGHT) {
                    coins.erase(coins.begin() + i);
                    coinSprites.erase(coinSprites.begin() + i);
                } else {
                    i++;
                }
            }

            // ========== ENEMY COLLISION ==========
            for (auto &e : enemies) {
                if (e.shape.getGlobalBounds().intersects(player.shape.getGlobalBounds())) {
                    gameOver = true;
                }
            }

            speed += 0.1f * dt;
        }

        // ========== DRAW ==========
        window.clear(sf::Color(20, 20, 30));

        window.draw(playerSprite);

        for (auto &c : coinSprites)
            window.draw(c);

        for (auto &e : enemySprites)
            window.draw(e);

        window.display();
    }

    return 0;
}
