using namespace std;

#include <SFML/Graphics.hpp>
#include "Collision.h"
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <vector>
#include <SFML/Audio.hpp>
#include <sstream>

const float pi = 3.14159f;
int ballSpeed = 350; // 400
float ballAngle = 0.f;

float racketSpeed = 0.5f; //0.7
int secondRacketSpeed = 275, secondRacketSpeedplayer2 = -275; //325

bool player1Auto = false, player1secondAuto = true;
bool player2Auto = false, player2secondAuto = true;

int player1Score = 0;
int player2Score = 0;

template <class T > string toString( T value )
{
   stringstream ss;
   ss << value;
   return ss.str();
}

int main()
{
    srand (time(NULL));

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Gina's Final - PONG");
    //window.setFramerateLimit(600);

    sf::View camera(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
    camera.setViewport(sf::FloatRect(0, 0, 1, 1));

      sf::Font font;
    if (!font.loadFromFile("Pixeled.ttf"))
    {
        //error
    }

    // Sounds, racket sound
    sf::SoundBuffer racketBuffer;
    if (!racketBuffer.loadFromFile("racket.wav"))
    {
        return -1;
    }

    sf::Sound racketSound;
    racketSound.setBuffer(racketBuffer);

    //Lose  sound
    sf::SoundBuffer loseBuffer;
    if (!loseBuffer.loadFromFile("lose.wav"))
    {
        return -1;
    }

    sf::Sound loseSound;
    loseSound.setBuffer(loseBuffer);

    //win Sound
    sf::SoundBuffer winBuffer;
    if (!winBuffer.loadFromFile("win.wav"))
    {
        return -1;
    }

    sf::Sound winSound;
    winSound.setBuffer(winBuffer);


    // Scores
     sf::Text score1Text;
    score1Text.setFont(font);
    score1Text.setString(toString(player1Score));
    score1Text.setCharacterSize(24);
    score1Text.setOrigin(score1Text.getLocalBounds().width/2, score1Text.getLocalBounds().height/2);
    score1Text.setPosition(window.getSize().x/2 - window.getSize().x/2/2, window.getSize().y/2 -225);

    sf::Text score2Text;
    score2Text.setFont(font);
    score2Text.setString(toString(player2Score));
    score2Text.setCharacterSize(24);
    score2Text.setOrigin(score2Text.getLocalBounds().width/2, score2Text.getLocalBounds().height/2);
    score2Text.setPosition(window.getSize().x/2 + window.getSize().x/2/2, window.getSize().y/2 -225);

    sf::Text banner;
    banner.setFont(font);
    banner.setString("Press space to play");
    banner.setCharacterSize(10);
    banner.setOrigin(banner.getLocalBounds().width/2, banner.getLocalBounds().height/2);
    banner.setPosition(window.getSize().x/2, 10);


     // DottedLine
    sf::Texture dottedLineTexture;
    dottedLineTexture.loadFromFile("DottedLine.png");
    sf::Sprite dottedLine(dottedLineTexture);
    dottedLine.setOrigin(dottedLine.getLocalBounds().width/2, dottedLine.getLocalBounds().height/2);
    dottedLine.setPosition(window.getSize().x/2, window.getSize().y/2);
    dottedLine.setScale(18, 18);


     // WallHorizontal1
    sf::Texture wallHorizontalTexture;
    wallHorizontalTexture.loadFromFile("WallHorizontal.png");
    sf::Sprite wallHorizontal1(wallHorizontalTexture);
    wallHorizontal1.setOrigin(wallHorizontal1.getLocalBounds().width/2, wallHorizontal1.getLocalBounds().height/2);
    wallHorizontal1.setPosition(window.getSize().x/2, 25);
    wallHorizontal1.setScale(15.35, 17);

    // WallVertical1
    sf::Texture wallVerticalTexture;
    wallVerticalTexture.loadFromFile("WallVertical.png");
    sf::Sprite wallVertical1(wallVerticalTexture);
    wallVertical1.setOrigin(wallVertical1.getLocalBounds().width/2, wallVertical1.getLocalBounds().height/2);
    wallVertical1.setPosition(25, window.getSize().y/2);
    wallVertical1.setScale(17, 17);


     // WallHorizontal2
    sf::Sprite wallHorizontal2(wallHorizontalTexture);
    wallHorizontal2.setOrigin(wallHorizontal2.getLocalBounds().width/2, wallHorizontal2.getLocalBounds().height/2);
    wallHorizontal2.setPosition(window.getSize().x/2, 575);
    wallHorizontal2.setScale(15.35, 17);

    // WallVertical2
    sf::Sprite wallVertical2(wallVerticalTexture);
    wallVertical2.setOrigin(wallVertical2.getLocalBounds().width/2, wallVertical2.getLocalBounds().height/2);
    wallVertical2.setPosition(776, window.getSize().y/2);
    wallVertical2.setScale(17, 17);

    // Player1
    sf::Texture playerTexture;
    playerTexture.loadFromFile("Racket.png");
    sf::Sprite player1(playerTexture);
    player1.setOrigin(player1.getLocalBounds().width/2, player1.getLocalBounds().height/2);
    player1.setPosition(75, window.getSize().y/2);
    player1.setScale(18, 18);

    // player1secondRacket
    sf::Sprite player1SecondRacket(playerTexture);
    player1SecondRacket.setOrigin(player1SecondRacket.getLocalBounds().width/2, player1SecondRacket.getLocalBounds().height/2);
    player1SecondRacket.setPosition(175, window.getSize().y/2);
    player1SecondRacket.setScale(18, 18);

    // Player2
    sf::Sprite player2(playerTexture);
    player2.setOrigin(player2.getLocalBounds().width/2, player2.getLocalBounds().height/2);
    player2.setPosition(725, window.getSize().y/2);
    player2.setScale(18, 18);

    // player2secondRacket
    sf::Sprite player2SecondRacket(playerTexture);
    player2SecondRacket.setOrigin(player2SecondRacket.getLocalBounds().width/2, player2SecondRacket.getLocalBounds().height/2);
    player2SecondRacket.setPosition(625, window.getSize().y/2);
    player2SecondRacket.setScale(18, 18);

    // Ball
    sf::Texture ballTexture;
    ballTexture.loadFromFile("Ball.png");
    sf::Sprite ball(ballTexture);
    ball.setOrigin(ball.getLocalBounds().width/2, ball.getLocalBounds().height/2);
    ball.setPosition(window.getSize().x/2, window.getSize().y/2);
    ball.setScale(18, 18);

    sf::Clock clock;
    bool isPlaying = false;
	// Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //Player1Controls
         if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            player1.setColor(sf::Color::Red);
            player1SecondRacket.setColor(sf::Color::White);

            player1secondAuto = true;
            player1Auto = false;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            player1.setColor(sf::Color::White);
            player1SecondRacket.setColor(sf::Color::Red);

            player1secondAuto = false;
            player1Auto = true;
        }

        if (player1Auto == false)
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                player1.move(0, -1 * racketSpeed);
                if(Collision::BoundingBoxTest(player1, wallHorizontal1))
                {
                  player1.setPosition(75, 70);
                }
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                player1.move(0, 1 * racketSpeed);
                if(Collision::BoundingBoxTest(player1, wallHorizontal2))
                {
                  player1.setPosition(75, 530);
                }
            }
        }

        else
        {
             if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                player1SecondRacket.move(0, -1 * racketSpeed);
                if(Collision::BoundingBoxTest(player1SecondRacket, wallHorizontal1))
                {
                  player1SecondRacket.setPosition(175, 70);
                }
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                player1SecondRacket.move(0, 1 * racketSpeed);
                if(Collision::BoundingBoxTest(player1SecondRacket, wallHorizontal2))
                {
                  player1SecondRacket.setPosition(175, 530);
                }
            }
        }




         if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            player2.setColor(sf::Color::Cyan);
            player2SecondRacket.setColor(sf::Color::White);

            player2secondAuto = true;
            player2Auto = false;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            player2.setColor(sf::Color::White);
            player2SecondRacket.setColor(sf::Color::Cyan);

            player2secondAuto = false;
            player2Auto = true;
        }


        //Player2
         if (player2Auto == false)
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                player2.move(0, -1 * racketSpeed);
                if(Collision::BoundingBoxTest(player2, wallHorizontal1))
                {
                  player2.setPosition(725, 70);
                }
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                player2.move(0, 1 * racketSpeed);
                if(Collision::BoundingBoxTest(player2, wallHorizontal2))
                {
                  player2.setPosition(725, 530);
                }
            }
        }

        else
        {
             if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                player2SecondRacket.move(0, -1 * racketSpeed);
                if(Collision::BoundingBoxTest(player2SecondRacket, wallHorizontal1))
                {
                  player2SecondRacket.setPosition(625, 70);
                }
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                player2SecondRacket.move(0, 1 * racketSpeed);
                if(Collision::BoundingBoxTest(player2SecondRacket, wallHorizontal2))
                {
                  player2SecondRacket.setPosition(625, 530);
                }
            }
        }


        //BallMovementControl
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            if (!isPlaying)
            {
                // (re)start the game
                isPlaying = true;
                clock.restart();
                // Reset the ball angle
                do
                {
                    // Make sure the ball initial angle is not too much vertical
                    ballAngle = (std::rand() % 360) * 2 * pi / 360;
                }
                while (std::abs(std::cos(ballAngle)) < 0.7f);

            }
        }

        if (isPlaying)
        {
            float deltaTime = clock.restart().asSeconds();

            float factor = ballSpeed * deltaTime;
            ball.move(std::cos(ballAngle) * factor, std::sin(ballAngle) * factor);



            if (player1secondAuto == true)
            {
                player1SecondRacket.move(0.f, secondRacketSpeed* deltaTime);
            }
            else
            {
                player1.move(0.f, secondRacketSpeed * deltaTime);
            }



             if (player2secondAuto == true)
            {
                player2SecondRacket.move(0.f, secondRacketSpeedplayer2* deltaTime);
            }
            else
            {
                player2.move(0.f, secondRacketSpeedplayer2 * deltaTime);
            }

            if (player1Score > player2Score)
            {
                banner.setString("Player 1 is winning");
            }
            else
            {
                banner.setString("Player 2 is winning");
            }

        }


        //Collisions between ball and rackets
        if(Collision::BoundingBoxTest(ball, player1) || Collision::BoundingBoxTest(ball, player1SecondRacket))
        {
             if (ball.getPosition().y > player1.getPosition().y || ball.getPosition().y > player1SecondRacket.getPosition().y)
             {
                 ballAngle = pi - ballAngle + (std::rand() % 20) * pi / 180;
             }
             else
             {
                 ballAngle = pi - ballAngle - (std::rand() % 20) * pi / 180;
             }

            racketSound.play();
        }


        if(Collision::BoundingBoxTest(ball, player2) || Collision::BoundingBoxTest(ball, player2SecondRacket))
        {
             if (ball.getPosition().y > player2.getPosition().y)
             {
                 ballAngle = pi - ballAngle + (std::rand() % 20) * pi / 180;
             }
             else
             {
                 ballAngle = pi - ballAngle - (std::rand() % 20) * pi / 180;
             }
            racketSound.play();
        }


        //Collisions between vertical walls
        if (Collision::BoundingBoxTest(ball, wallVertical1))
        {
            isPlaying = false;
            ball.setPosition(window.getSize().x/2, window.getSize().y/2);
            player2Score++;
            score2Text.setString(toString(player2Score));
            loseSound.play();
            banner.setString("Press space to continue");
        }

        if (Collision::BoundingBoxTest(ball, wallVertical2))
        {
            isPlaying = false;
            ball.setPosition(window.getSize().x/2, window.getSize().y/2);
            player1Score++;
            score1Text.setString(toString(player1Score));
            loseSound.play();
            banner.setString("Press space to continue");
        }


        //Collisions between horizontal walls
        if (Collision::BoundingBoxTest(ball, wallHorizontal1))
        {
                ballAngle = -ballAngle;
                racketSound.play();
        }

         if (Collision::BoundingBoxTest(ball, wallHorizontal2))
        {
                ballAngle = -ballAngle;
                racketSound.play();
        }



        if (player1secondAuto)
        {
        //collision between 2nd rackets and walls
         if(Collision::BoundingBoxTest(player1SecondRacket, wallHorizontal2))
            {
              player1SecondRacket.setPosition(175, 530);
              secondRacketSpeed = -secondRacketSpeed;
            }

        if(Collision::BoundingBoxTest(player1SecondRacket, wallHorizontal1))
            {
              player1SecondRacket.setPosition(175, 70);
              secondRacketSpeed = -secondRacketSpeed;
            }

        }

        else
        {
           if(Collision::BoundingBoxTest(player1, wallHorizontal2))
            {
              player1.setPosition(75, 530);
              secondRacketSpeed = -secondRacketSpeed;
            }

            if(Collision::BoundingBoxTest(player1, wallHorizontal1))
            {
              player1.setPosition(75, 70);
              secondRacketSpeed = -secondRacketSpeed;
            }

        }





         if (player2secondAuto)
        {
        //collision between 2nd rackets and walls
         if(Collision::BoundingBoxTest(player2SecondRacket, wallHorizontal2))
            {
              player2SecondRacket.setPosition(625, 530);
              secondRacketSpeedplayer2 = -secondRacketSpeedplayer2;
            }

        if(Collision::BoundingBoxTest(player2SecondRacket, wallHorizontal1))
            {
              player2SecondRacket.setPosition(625, 70);
              secondRacketSpeedplayer2 = -secondRacketSpeedplayer2;
            }

        }

        else
        {
           if(Collision::BoundingBoxTest(player2, wallHorizontal2))
            {
              player2.setPosition(725, 530);
              secondRacketSpeedplayer2 = -secondRacketSpeedplayer2;
            }

        if(Collision::BoundingBoxTest(player2, wallHorizontal1))
            {
              player2.setPosition(725, 70);
              secondRacketSpeedplayer2 = -secondRacketSpeedplayer2;
            }

        }

        if (player1Score == 5 || player2Score == 5)
        {
          winSound.play();
          banner.setString("Press space to restart");
          player1Score = 0;
          player2Score = 0;
          score1Text.setString(toString(player1Score));
          score2Text.setString(toString(player2Score));
        }

        // Clear screen
        window.clear();

        //camera
        window.setView(camera);

        // Draw the sprites
        window.draw(dottedLine);
        window.draw(wallHorizontal1);
        window.draw(wallVertical1);
        window.draw(wallHorizontal2);
        window.draw(wallVertical2);
        window.draw(player1);
        window.draw(player1SecondRacket);

        window.draw(player2);
        window.draw(player2SecondRacket);

        window.draw(ball);

        window.draw(score1Text);
        window.draw(score2Text);
        window.draw(banner);

        // Update the window
        window.display();
}
        return EXIT_SUCCESS;
}


