/* préambule :
Récapitulatif : Programme écrit en C généré à l'aide de mBlock.
Ventilateur avec capteur pour avoir la distance, si un objet se trouve à proximité du ventilateur, la vitesse est réduite.
Lorsque l'objet est très proche  
*/

#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeOrion.h>

// Initialisation nécessaire pour le bon fonctionnement du système (créé automatiquement par mBlock)
double angle_rad = PI/180.0;
double angle_deg = 180.0/PI;

// Initialisation des différentes fonctions
void Initialisation();
void ChangeSlowDistance(double count);
void ChangeStopDistance(double count);
void SetUpSlowDistance();
void SetUpStopDistance();

// InInitialisation des variables limites de ralentissement et d'arrêt selon la distance capté par le capteur ultrason 
double stopDistance;
double slowDistance;

// Initialisation des différents capteurs et actionneurs 
MeRGBLed rgbled_7(7, 4); // 4 leds RGB
MeJoystick joystick_6(6); // Joystick
Me7SegmentDisplay seg7_4(4); //afficheur 7 segment
MeDCMotor motor_9(9); // moteur
MeUltrasonicSensor ultrasonic_3(3);  // capteur ultrasons

// Définir les valeurs par défaut de la distance d'arrêt et de ralentissement
void Initialisation()
{
    stopDistance = 20;
    slowDistance = 40;
}

// Fonction de changement de la distance de ralentissement et vérification des bornes
void ChangeSlowDistance(double count)
{
    slowDistance += count;

    // Vérifie si 'slowDistance' ne dépasse pas la limite (80)
    if((slowDistance) > (80)){
        slowDistance = 80;
    }else{
        // Vérifie si 'slowDistance' ne descend pas en dessous la limite (20)
        if((slowDistance) < (20)){
            slowDistance = 20;
        }
        // Permet de s'assurer que la distance de ralentissement est 5 cm plus grande que la distance d'arret
        if((slowDistance) < ((stopDistance) + (5))){
            slowDistance = (stopDistance) + (5);
        }
    }
}

// Fonction de changement de la distance d'arrêt et vérification des bornes
void ChangeStopDistance(double count)
{
    stopDistance += count;

    if((stopDistance) > (40)){
        stopDistance = 40;
    }else{
        if((stopDistance) < (10)){
            stopDistance = 10;
        }
    }
}

// Fonction appelée quand l'utilisateur veut modifie la distance de ralentissement
void SetUpSlowDistance()
{
    // Change la couleur des LEDs pour informer l'utilisateur de la modification de la distance de ralentissment

    // Allume la led 2 et 4 (haut et bas) avec du orange
    rgbled_7.setColor(2,255,217,5);
    rgbled_7.show();
    rgbled_7.setColor(4,255,217,5);
    rgbled_7.show();

    // Lors de l'appel de cette fonction, la variable va être remise dans 
    ChangeSlowDistance(0);

    // Attend 1 seconde pour que le joystick soit redescendu, sinon il va détecter la validation de l'utilsateur alors que c'est celle d'avant
    _delay(1);

    // Execute le code tant que l'utilisateur n'a pas mis le joystick vers le haut, ce qui correspond à une validation
    while(!((joystick_6.read(2)) > (440)))
    {
        _loop();
        // Attend 0.1 sec, ce qui permet que le code s'execute environ 10 fois par seconde (sans prendre en compte le temps de calcul qui est négligeable ici)
        // L'executer 10 fois par seconde permet d'ajouter une valeur constant chaque seconde : si a chaque itération on ajoute 1, on sait que au bout d'une seconde, la valeur aura augmenté de 10

        _delay(0.1);

        // Affiche à l'utilisateur la valeur de distance de ralentissement
        seg7_4.display((float)slowDistance);

        // Le joystick a fond vers la droite donne une valeur de 490 en x, ici nous utilisons 440 comme valeur plus haute, puisque le joystick est peu précis et que s'il est legerement vers le haut, la valeur de x diminue legerement
        if((joystick_6.read(1)) > (440)){ // read(1) permet de lire sur l'axe x (horizontalement)
            ChangeSlowDistance(0.5); // ajout de 0.5 cm pour la distance de ralentissement (soit 5 cm, par seconde)
        }else{
            if((joystick_6.read(1)) > (220)){
                ChangeSlowDistance(0.2);
            }else{
                if((joystick_6.read(1)) > (110)){
                    ChangeSlowDistance(0.05);
                }
            }
        }

        // meme chose que précédemment mais quand le joystick est déplacé vers la droite
        if((joystick_6.read(1)) < (-440)){
            ChangeSlowDistance(-0.5);
        }else{
            if((joystick_6.read(1)) < (-220)){
                ChangeSlowDistance(-0.2);
            }else{
                if((joystick_6.read(1)) < (-110)){
                    ChangeSlowDistance(-0.05);
                }
            }
        }
    }
    seg7_4.display((float)slowDistance);
}

// Fonctionnement très similaire a la fonction précédente, mais cette fois pour la distance d'arret
void SetUpStopDistance()
{
    rgbled_7.setColor(2,255,0,0);
    rgbled_7.show();
    rgbled_7.setColor(4,255,0,0);
    rgbled_7.show();
    while(!((joystick_6.read(2)) > (440)))
    {
        _loop();
        _delay(0.1);
        seg7_4.display((float)stopDistance);
        if((joystick_6.read(1)) > (440)){
            ChangeStopDistance(0.5);
        }else{
            if((joystick_6.read(1)) > (220)){
                ChangeStopDistance(0.2);
            }else{
                if((joystick_6.read(1)) > (110)){
                    ChangeStopDistance(0.05);
                }
            }
        }
        if((joystick_6.read(1)) < (-440)){
            ChangeStopDistance(-0.5);
        }else{
            if((joystick_6.read(1)) < (-220)){
                ChangeStopDistance(-0.2);
            }else{
                if((joystick_6.read(1)) < (-110)){
                    ChangeStopDistance(-0.05);
                }
            }
        }
    }
    seg7_4.display((float)stopDistance);
}

// premiere fonction qui va être appelée quand l'arduino commence à executer ce programme
void setup(){
    // Désactive le moteur (non nécessaire)
    motor_9.run(0);

    // Désactive les LEDs (non nécessaire)
    rgbled_7.setColor(1,0,0,0); // LED 1
    rgbled_7.show();
    rgbled_7.setColor(3,0,0,0); // LED 3
    rgbled_7.show();
    rgbled_7.setColor(2,0,0,0); // LED 2
    rgbled_7.show();
    rgbled_7.setColor(4,0,0,0); // LED 4
    rgbled_7.show();

    Initialisation(); // Fonction qui met les variables avec les bonnes valeurs
    
    SetUpStopDistance(); // Demande à l'utilisateur la distance d'arrêt
    SetUpSlowDistance(); // Demande à l'utilisateur la distance de ralentissement
}

void loop(){
    // Affiche à l'utilisateur la distance capté par le capteur ultrason sur l'afficheur 7 segment
    seg7_4.display((float)ultrasonic_3.distanceCm());

    // Vérifie si l'utilisateur descend le joystick vers le bas, (correspond a la commande pour aller modifier les distances d'arrets et de ralentissment)
    if((joystick_6.read(2)) < (-440)){ // read(2) pour l'axe vertical
        // Met la vitesse du moteur à 0, car pendant la modification des valeurs le moteur doit être à l'arret
        motor_9.run(0);
        // Eteint les lumières (LED gauche et droite) pour indiquer que le ventilateur ne tourne pas
        rgbled_7.setColor(1,0,0,0);
        rgbled_7.show();
        rgbled_7.setColor(3,0,0,0);
        rgbled_7.show();
        
        // Demande à l'utilisateur de configurer les distances d'arrêts et de ralentissement
        SetUpStopDistance();
        SetUpSlowDistance();

        // Eteint les lumières pour dire que la configuration des distances est terminée (LED gauche et droite)
        rgbled_7.setColor(2,0,0,0);
        rgbled_7.show();
        rgbled_7.setColor(4,0,0,0);
        rgbled_7.show();
    }

    // Allume les LED ()
    rgbled_7.setColor(1,255,0,0);
    rgbled_7.show();
    rgbled_7.setColor(3,255,0,0);
    rgbled_7.show();
    
    // Vérification des distances et changement de vitesse du moteur en fonction

    // Si distance inférieur a la distance d'arrêt, alors arrêt total du moteur 
    if((ultrasonic_3.distanceCm()) < (stopDistance)){
        motor_9.run(0); // arrêt du moteur
        
        // Met les lumières en rouge pour indiquer l'arrêt du moteur
        rgbled_7.setColor(2,255,0,0);
        rgbled_7.show();
        rgbled_7.setColor(4,255,0,0);
        rgbled_7.show();
    }else{
        // Sinon si distance inférieur a la distance de ralentissement, alors arrêt partiel du moteur 
        if((ultrasonic_3.distanceCm()) < (slowDistance)){
            motor_9.run(45); // moitié de la vitesse classique du moteur

            // Led (haut et bas) en violet pour indiquer le ralentissement du ventilateur et la présence d'un objet proche
            rgbled_7.setColor(1,217,5,255);
            rgbled_7.show();
            rgbled_7.setColor(3,217,5,255);
            rgbled_7.show();
        }else{
            // Fonctionnement classique du moteur à pleine vitesse 
            motor_9.run(90);

            // Led (haut et bas) allumé en vert pour indiquer le fonctionnement à pleine vitesse du ventilateur
            rgbled_7.setColor(1,0,255,0);
            rgbled_7.show();
            rgbled_7.setColor(3,0,255,0);
            rgbled_7.show();
        }
    }
    // Appel la meme fonction de manière à faire une boucle
    _loop();
}

// Permet d'attendre un temps en seconde
void _delay(float seconds){
    long endTime = millis() + seconds * 1000;
    while(millis() < endTime)_loop();
}

// Appel une première fois la boucle
void _loop(){
}
