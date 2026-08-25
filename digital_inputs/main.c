#include "stm32f4xx.h"
#include "../emlib/emlib.h"

/*
 * Activer l'horloge du port GPIO utilisé (ex : GPIOA via RCC).
Configurer la broche de la LED en sortie (push-pull).
Configurer les deux broches des boutons en entrée avec pull-up interne activé (les boutons seront câblés vers GND).
Câbler le circuit : LED + résistance 220 Ω entre la broche et GND, chaque bouton entre sa broche et GND.
Dans la boucle principale, lire l'état du bouton A : s'il est à LOW (appuyé) → allumer la LED.
Toujours dans la boucle, lire l'état du bouton B : s'il est à LOW → éteindre la LED.
Compiler et flasher sur la carte, puis tester.
*/



#define LED         7
#define BUTTON_OFF  6
#define BUTTON_ON   9

int main(void) {
    rcc_gpioa_enable();
    gpioa_output(LED);
    gpioa_input(BUTTON_ON);
    gpioa_input(BUTTON_OFF);
    gpioa_pull_up(BUTTON_ON);
    gpioa_pull_up(BUTTON_OFF);

    while (1) {
        if ((GPIOA->IDR & (1 << BUTTON_ON)) == 0)
            led_on(LED);
        else if ((GPIOA->IDR & (1 << BUTTON_OFF)) == 0)
            led_off(LED);
    }
}
