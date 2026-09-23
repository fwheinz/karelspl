#include <stdio.h>
#include <string.h>

#include "cslib.h"
#include "simpio.h"

/*
 * Calculates and evaluates the BMI from user input
 * Author: Florian Heinz <florian.heinz@oth-regensburg.de>
 */

/*
 * evaluateBMI evaluates the passed bmi value and
 * outputs it as well as the category.
 * Parameter: bmi    - the bmi of the user
 * Return value: none
 */
void evaluateBMI (double bmi) {
    printf("Your BMI is %f\n", bmi);
    if (bmi < 18.5) {
        printf("You are underweight.\n");
    } else if (bmi <= 25) {
        printf("You have normal weight.\n");
    } else if (bmi <= 30) {
        printf("You are overweight.\n");
    } else {
        printf("You are heavy overweight.\n");
    }
}

/*
 * calculateBMI calculates the BMI from height and weight.
 * Parameter: kg     - the weight of the user
 *            height - the height of the user
 * Return value (by reference): the bmi
 */
void calculateBMI (double kg, double height, double *bmi) {
  *bmi = kg / height / height;
}

/*
 * getWeight requests the weight from the user.
 * Return value (by reference): The entered weight
 */
void readWeight (double *kg) {
    printf("Enter weight in kilos: ");
    *kg = getReal();
}

/*
 * getHeight requests the height from the user.
 * Return value (by reference): The entered height
 */
void readHeight (double *height) {
    printf("Enter height in meters: ");
    *height = getReal();
}

/*
 * checkYourBMI fetches weight and height from the user, outputs the BMI
 * as well as a category.
 * Return value: none
 */
void checkYourBMI (void) {
    double kg, height, bmi;

    readWeight(&kg);
    readHeight(&height);
    calculateBMI(kg, height, &bmi);
    evaluateBMI(bmi);
}

/*
 * Entry point of the program.
 */
int main (void) {
    checkYourBMI();

    return 0;
}