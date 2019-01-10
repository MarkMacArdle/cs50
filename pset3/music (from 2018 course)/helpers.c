// Helper functions for music

#include <cs50.h>
#include <string.h>
#include <math.h>

#include "helpers.h"

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    int nominator = fraction[0] - '0'; //subtracting ascii indexes gives actual number
    int denominator = fraction[2] - '0';
    return nominator * (8 / denominator);
}

// return index of a character in a string
int pos_in_string(string s, char char_to_find)
{
    char *pointer_to_char_in_string = strchr(s, char_to_find);
    return (int)(pointer_to_char_in_string - s);
}

// Calculates frequency (in Hz) of a note
int frequency(string note)
{
    //eprintf("note: %s\n", note);
    // subtracting '0' in ascii results in the actual number
    int octave = note[strlen(note) - 1] - '0';

    //placeholders used for sharps/flats, they'll be dealt with later.
    string note_list = "CcDdEFfGgAaB";

    int semitones_away_from_A4 = (pos_in_string(note_list, note[0]) - pos_in_string(note_list, 'A')) + ((octave - 4) * 12);

    //if note is sharp add a semitone, if flat subtract one.
    if (note[1] == '#')
    {
        semitones_away_from_A4++;
    }
    else if (note[1] == 'b')
    {
        semitones_away_from_A4--;
    }

    double power = (double) semitones_away_from_A4 / 12;
    double freq = pow(2, power) * 440;

    //eprintf("semitones_away_from_A4: %i\n", semitones_away_from_A4);
    //eprintf("power: %lf\n", power);
    //eprintf("freq: %i\n", (int) round(freq));

    return (int) round(freq);
}

// Determines whether a string represents a rest
bool is_rest(string s)
{
    //eprintf("check for rest: %s\n", s);
    if (strcmp(s, "\n") == 0 || strcmp(s, "\r") == 0 || strcmp(s, "\r\n") == 0 || strcmp(s, "") == 0)
    {
        return true;
    }
    return false;
}
