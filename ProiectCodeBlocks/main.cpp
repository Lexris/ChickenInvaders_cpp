#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>


using namespace std;
using namespace sf;
namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}


int lives=8, score=0;
int scoreArray[4];


int radtodeg(int rad)
{
    int deg;
    deg=rad/0.0174533;
    return deg;

}

int randgenX()
{
    int randnr=0;
    while(randnr<=0 || randnr>=1146)
        randnr=rand()%10000;
    return randnr;
}

int randgenY()
{
    int randnr=0;
    while(randnr<=0 || randnr>=706)
        randnr=rand()%1000;
    return randnr;
}


class explosionsClass
{
private:
    float Frame=0, animSpeed=0.4;
    int frameCount=8, aux=0, ok=0;

public:
    Sprite sExplosion;
    int terminator=0;
    void GG(Texture &t, Vector2f coord)
    {
        if(ok!=1)
        {
            sExplosion.setTexture(t);
            sExplosion.setOrigin(1,-698);
            sExplosion.scale(0.5, 0.5);
            sExplosion.setPosition(coord.x,coord.y+350);
            ok=1;
        }

    Frame+=animSpeed;
    if(Frame>frameCount)
        {
            Frame-=frameCount; aux++;
            if(aux>5) terminator=1;
        }
    sExplosion.setTextureRect(IntRect(int(Frame)*256,aux*256,256,256));


    }
};

class attackersClass
{
private:
    float Frame=0, animSpeed=0.2;
    int frameCount=5, aux=0, ok=0;
    int attackerandY()
    {
        int randnr=0;
        while(randnr<=0 || randnr>=698)
            randnr=rand()%1000;
        return randnr;
    }
public:
    Sprite sAttacker;
    int terminator=0;
    Vector2f coord;
    void Animation(Texture &t, FloatRect bbBG, FloatRect bbP)
    {
        if(terminator==0)
            {
                if(ok!=1)
                {
                    sAttacker.setTexture(t);
                    sAttacker.setOrigin(1,-698);
                    sAttacker.setPosition(-110,-attackerandY());
                    ok=1;
                }

                Frame+=animSpeed;
                if(Frame>frameCount)
                {
                    Frame-=frameCount; aux++;
                    if(aux>2) aux=0;
                }
                sAttacker.setTextureRect(IntRect(int(Frame)*110,aux*101,110,101));

                sAttacker.move(3.8,0.2);
                if(!sAttacker.getGlobalBounds().intersects(bbBG)) sAttacker.setPosition(-110,-attackerandY());
                if(sAttacker.getGlobalBounds().intersects(bbP))
                {
                    terminator=1;
                    if(lives>0)
                    {
                        score++;
                        lives--;
                    }
                }
            }
    }
};

class lasersClass
{
private:
    float Frame=0, animSpeed=0.2;
    int frameCount=5, aux=0, ok=0;

public:
    Sprite sLaser;
    int terminator=0;
    void Fire(Texture &t, FloatRect bbBG, Vector2f pP)
    {
        if(ok!=1)
        {
            sLaser.setTexture(t);
            sLaser.setOrigin(1,-705);
            sLaser.setPosition(pP.x+20.5,pP.y+645);
            sLaser.scale(0.1f,0.1f);
            ok=1;
        }

        sLaser.move(0,-15);

    }
};


int main()
{
    ///setting up the window, textures and sprites
    RenderWindow app(sf::VideoMode(1200, 800), "Success");
    app.setPosition(sf::Vector2i(100, 5));
    app.setFramerateLimit(60);

    Texture t1, t2, t3, t4, t5, t6;
    t1.loadFromFile("images/keyhd2.png");
    t2.loadFromFile("images/border.png");
    t3.loadFromFile("images/explosion.png");
    t4.loadFromFile("images/attacker.png");
    t5.loadFromFile("images/laser.png");
    t6.loadFromFile("images/gameoverpng.png");

    Sprite sPlayer(t1), sBackground(t2), sGameover(t6);
    sPlayer.setTextureRect(IntRect(0,0,50,94));
    sPlayer.setOrigin(1,-705);
    sPlayer.setPosition(552,-100);
    sGameover.setScale(0.8,0.8);
    sGameover.setPosition(120,80);


    ///setting up sounds
    sf::Music music;
    music.openFromFile("audioFX/bigshaq.ogg");
    music.setLoop(true);
    music.play();


    ///setting up scoreboard and livesboard
    sf::Font font;
    font.loadFromFile("fonts/American Captain.ttf");

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(45);
    text.setFillColor(sf::Color::Yellow);
    text.setPosition(1025,5);

    sf::Text text2;
    text2.setFont(font);
    text2.setCharacterSize(45);
    text2.setFillColor(sf::Color::Yellow);
    text2.setPosition(1025,50);


    ///for attackers
    int attackersCount=0;
    float attackersTimer=0;


    ///for lasers
    int lasersCount=0;


    ///for others
    int i, j, ok=0, first=0, howmuch=1;


    ///setting up the vectors
    vector<attackersClass> attackersVector;
    vector<explosionsClass> explosionsVector;
    vector<lasersClass> lasersVector;

    attackersClass initAttacker;
    explosionsClass initExplosion;
    lasersClass initLaser;

    for(i=0;i<1000;i++)
        {
            attackersVector.push_back(initAttacker);
            explosionsVector.push_back(initExplosion);
            lasersVector.push_back(initLaser);
        }


    ///in window
    while(app.isOpen())
    {
        Event event;
        while(app.pollEvent(event))
        {
            if (event.type == Event::Closed)
                app.close();
        }

        sf::FloatRect boundingBoxKEY = sPlayer.getGlobalBounds();
        sf::FloatRect boundingBoxBORDER = sBackground.getGlobalBounds();


        ///Spawning enemies
        if(attackersCount<999)
        {
            if(score>=20) attackersTimer+=0.01;
            if(score>=60) attackersTimer+=0.01;
            attackersTimer+=0.04;

            if(attackersTimer>2)
            {
                attackersTimer=0;
                attackersCount++;
            }
        }


        ///Adding lives when score hits a multiple of 50
        if(score/(howmuch*50)==1)
        {
            howmuch++;
            if(lives<8 && lives>0)
            {
                lives++;
            }
        }


        ///Checking for keys pressed
        if(Keyboard::isKeyPressed(Keyboard::Right) && boundingBoxKEY.intersects(boundingBoxBORDER)) sPlayer.move(10,0);
            else if(Keyboard::isKeyPressed(Keyboard::Right) && !boundingBoxKEY.intersects(boundingBoxBORDER)) sPlayer.setPosition(1,-randgenY());

        if(Keyboard::isKeyPressed(Keyboard::Left)  && boundingBoxKEY.intersects(boundingBoxBORDER)) sPlayer.move(-10,0);
            else if(Keyboard::isKeyPressed(Keyboard::Left) && !boundingBoxKEY.intersects(boundingBoxBORDER)) sPlayer.setPosition(1144,-randgenY());

        if(Keyboard::isKeyPressed(Keyboard::Up) && boundingBoxKEY.intersects(boundingBoxBORDER)) sPlayer.move(0,-10);
            else if(Keyboard::isKeyPressed(Keyboard::Up) && !boundingBoxKEY.intersects(boundingBoxBORDER)) sPlayer.setPosition(randgenX(),-1);

        if(Keyboard::isKeyPressed(Keyboard::Down) && boundingBoxKEY.intersects(boundingBoxBORDER)) sPlayer.move(0,10);
            else if(Keyboard::isKeyPressed(Keyboard::Down) && !boundingBoxKEY.intersects(boundingBoxBORDER)) sPlayer.setPosition(randgenX(),-704);

        if(Keyboard::isKeyPressed(Keyboard::Space))
        {
            if(ok==0)
            {
                lasersCount++;
                ok=1;
            }
        }
        else if(ok!=0) ok=0;


        ///Drawing everything + finishing mechanics and scoreboard
        app.clear();
        app.draw(sBackground);
        for(i=0;i<=attackersCount;i++)
        {
            if(attackersVector[i].terminator==0)
            {
                attackersVector[i].Animation(t4, boundingBoxBORDER, boundingBoxKEY);
                app.draw(attackersVector[i].sAttacker);
            }
            else if(explosionsVector[i].terminator==0)
            {
                explosionsVector[i].GG(t3, attackersVector[i].sAttacker.getPosition());
                app.draw(explosionsVector[i].sExplosion);
            }
        }
        if(lasersCount>0)                                                                                                               ///avem laser
            for(i=1;i<=lasersCount;i++)
            {
                for(j=0;j<=attackersCount;j++)
                {
                    if(lasersVector[i].terminator==0 && attackersVector[j].terminator==0)                                               ///laserul si attackerul nu au fost distrusi
                    {
                        if(attackersVector[j].sAttacker.getGlobalBounds().intersects(lasersVector[i].sLaser.getGlobalBounds()))         ///daca se ating laser si attacker devin distrusi
                        {
                            attackersVector[j].terminator=1;
                            if(lives>0) score++;
                            lasersVector[i].terminator=1;
                            explosionsVector[j].GG(t3, attackersVector[j].sAttacker.getPosition());
                        }
                        else if(lasersVector[i].terminator==0 && first==0)
                        {
                            lasersVector[i].Fire(t5, boundingBoxBORDER, sPlayer.getPosition());                                         ///daca nu, laserul se misca[bugged]
                            app.draw(lasersVector[i].sLaser);
                            first=1;
                        }
                    }
                    else if(lasersVector[i].terminator==0 && first==0)
                    {
                        lasersVector[i].Fire(t5, boundingBoxBORDER, sPlayer.getPosition());                                             ///daca nu, laserul se misca
                        app.draw(lasersVector[i].sLaser);
                        first=1;
                    }
                }
                first=0;
            }

        app.draw(sPlayer);
        if(lives==0) app.draw(sGameover);
        if(score<10)                                                                                                                    ///converting score to str, adding it to text spring and drawing
            text.setString("Score: 00" + patch::to_string(score));
        else if(score>=10 && score<100)
            text.setString("Score: 0" + patch::to_string(score));
        else text.setString("Score: " + patch::to_string(score));

        text2.setString("Lives: " + string(lives,'I'));

        app.draw(text);
        app.draw(text2);
        app.display();
    }
    return 0;
}
