#include <stdio.h>
#include <stdlib.h>

#include "calculations.h"

int main() {
  const int kFirstNumber = 3;
  const int kSecondNumber = 5;

  int added_numbers = AddNumbers(kFirstNumber, kSecondNumber);

  printf("First added_numbers: %d\n", kFirstNumber);
  printf("Second added_numbers: %d\n", kSecondNumber);
  printf("Added numbers: %d\n", added_numbers);

  return EXIT_SUCCESS;
}
