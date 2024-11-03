#include <stdio.h>
// för att bara köra det program:
// gcc src/highscore.c -o test
// ./test


int loadHighScore() {
    const char *filename = "data/highscore.txt"; // Hardcoded file path
    FILE *file = fopen(filename, "r");           // Open file for reading
    int highscore = 0;

    if (file) {
        // Read the integer from the file
        if (fscanf(file, "%d", &highscore) != 1) {
            // If fscanf fails, set highscore to 0 as a default
            highscore = 0;
        }
        fclose(file); // Close the file
    } else {
        // Handle the case where the file doesn't open (e.g., file not found)
        printf("Could not open high score file for reading.\n");
    }

    return highscore;
}

void saveHighScore(int score) {
    const char *filename = "data/highscore.txt"; // Same hardcoded file path
    FILE *file = fopen(filename, "w");           // Open file for writing

    if (file) {
        fprintf(file, "%d", score); // Write the score to the file
        fclose(file);
    } else {
        printf("Could not open high score file for writing.\n");
    }
}

int main() {
    int highscore = loadHighScore();
    printf("Current High Score: %d\n", highscore);

    // Update high score in the game if needed
    int newScore = 2; // Example score
    if (newScore > highscore) {
        saveHighScore(newScore);
        printf("New High Score: %d\n", newScore);
    }

    return 0;
}
