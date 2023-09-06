//HANGMAN GAME CODED USING C
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

//structure to store a word of length 30 charcters.
typedef struct
{
  char word[30];
} words;

words wordlist[1000];		//array of stucture "words" used to store words upto 1000 words each of length 30.
char key[30];			//array of characters used to store all the letters guessed by player.
int keys = 0, done;		//"keys" keeps track of number of guesses, "done" is a flag used to indicate completion of game, "done=1" means game is underway. 

/*function to get a random index from the array of structure "words" containing words depending on the paramenter we pass into the function
 1 means we add all the words from the file and take custom inputs from player.
 2 means we add only words from the file to the array.
 3 we add only the words taken fromthe user and do not add words from the file.
  everything else is considered as exiting the game.*/
int
getRandomWord (int choice)
{
  int len, index, x = 0, n = 0;
  FILE *ptr = fopen ("words.txt", "r");

  switch (choice)
    {
    case 1:			//it's left empty without break so that it executes case 2 and case 3.
    case 2:
      if (ptr == NULL)
	{
	  printf ("no such file.");	// in case we do not find the file of words inform and exit the game.
	  exit (0);
	}
      char buf[100];		// words are copied into this buffer from file before adding into array.
      while (fscanf (ptr, "%s ", buf) == 1)
	{
	  strcpy (wordlist[n].word, buf);	// adding all words from the file into the array of words.
	  n++;
	}
      if (choice == 2)		// we break the switch inside if condition because we may want it to execute case 3 also, if user passed case 1.
	{
	  break;
	}
    case 3:
      printf ("Enter the number of words to add: ");
      scanf ("%d", &x);
      printf ("\nEnter %d words:\n", x);
      for (int i = n; i < n + x; i++)
	{
	  scanf ("%s", wordlist[i].word);
	  len = strlen (wordlist[i].word);
	  for (int j = 0; j < len; j++)	//loop used to verify that custom word contains only alphabets.
	    {
	      if (!isalpha (wordlist[i].word[j]))
		{
		  printf
		    ("\n****ONLY ALPHABETS PLEASE RENTER THE WORD****\n");
		  i--;
		}
	    }
	}
      n += x;
      break;
    default:
      exit (0);
    }
  srand (time (NULL));		//seed random fuction with time so every new run gives new value.
  index = rand () % n;		//get random number and mod with "n" (number of words).
  return index;			// return "index" which is now a random number between 0 and n.
}

/*Function to draw stickfigures given the word 
  the guesses are stored in a public array named "key" and the length of the array is stored in a variable named "keys"
  these are compared with our random word kept track of by the paramter passed into the function "index"
  then the appropriate stickfigure is drawn depending on the number of "strikes"*/
void
stickFigure (int index)
{
  int strikes = 0, len = strlen (wordlist[index].word), count = 0, flag = 0;
  for (int i = 0; i < keys + 1; i++)
    {
      for (int j = 0; j < len; j++)
	{			//each guess is compared with each letter of the random word, both are converted to uppercase first.
	  if (toupper (key[i]) == toupper (wordlist[index].word[j]))
	    {
	      flag = 1;		//"flag=1" if guess matches letter from random word. 
	    }
	}
      if (flag)
	{
	  count++;		//"count" stores the total number of correct guesses.
	  flag = 0;
	}
    }
  strikes = keys + 1 - count;	//"strikes" stores the total number of wrong guesses so far.
  switch (strikes)
    {				//each case contains the specific drawing for that many strikes.
    case 0:
      printf
	("\n\n    +---+\n    |   |\n        |\n        |\n        |\n        |\n  =========");
      break;
    case 1:
      printf
	("\n    +---+\n    |   |\n    O   |\n        |\n        |\n        |\n  =========");
      break;
    case 2:
      printf
	("\n    +---+\n    |   |\n    O   |\n    |   |\n        |\n        |\n  =========");
      break;
    case 3:
      printf
	("\n    +---+\n    |   |\n    O   |\n   /|   |\n        |\n        |\n  =========");
      break;
    case 4:
      printf
	("\n    +---+\n    |   |\n    O   |\n   /|\\  |\n        |\n        |\n  =========");
      break;
    case 5:
      printf
	("\n    +---+\n    |   |\n    O   |\n   /|\\  |\n   /    |\n        |\n  =========");
      break;
    case 6:			//last case should display the random word and indicate to player that game is over.
      printf
	("\n    +---+\n    |   |\n    O   |\n   /|\\  |\n   / \\  |\n        |\n  =========\n");
      printf ("\nTHE WORD WAS : %s\n", wordlist[index].word);
      printf ("\n**************************************************************************\n");
      printf ("********************************GAME OVER*********************************\n");
      printf ("**************************************************************************\n");
      done = 0;
      break;
    default:			//some how "strikes" value does not lie between 0 and 6 then execute this case.
      printf ("\n\n***ERROR STRIKES EXCCEDED***\n\n");
    }
}

/*Function to display the word given index
  we also need to display the word with blanks for undiscovered letters and letters for already guessed characters
  this function checks with array "key" and displays the word appropriately with blanks and letters.
  this function also checks for victory condition, if there are no blanks it means the player has guessed all letters 
  therefore we need to end the game and announce victory.*/
void
displayWord (int index)
{
  int len = strlen (wordlist[index].word), flag = 1;	//resets flag value to 1 every call.
  printf ("\t");
  for (int i = 0; i < len; i++)
    {
      for (int j = 0; j < keys + 1; j++)
	{
	  if (toupper (key[j]) == toupper (wordlist[index].word[i]))
	    {
	      printf ("%c ", key[j]);	//displays correctly guessed letters.
	      goto label;	//used to skip over setting of flag and printing a blank.
	    }
	}
      flag = 0;			//"flag=0"indicates there are still letters to be guessed.
      printf ("_ ");		//prints a blank space for undiscovered letter.
    label:continue;
    }
  printf ("\n");
  if (flag)			//checks if victory condition is fulfilled.
    {
      printf ("\n\n**************************************************************************\n");
      printf ("*******************************YOU WON!!**********************************\n");
      printf ("**************************************************************************\n");
      done = 0;			//denotes end of game and player has won.

    }
}

void
main ()
{
  int index, len, choice;
  char userkey;
  printf ("**************************************************************************\n");
  printf ("*****************************HANGMAN-GAME*********************************\n");
  printf ("**************************************************************************\n");
  printf ("**                      _________________________                       **\n");
  printf ("**                      |           |          |                        **\n");
  printf ("**                      |           |                                   **\n");                  
  printf ("**                      |          _|_                                  **\n");
  printf ("**                      |       .-""""""-.                              **\n");   
  printf ("**                      |     .'          '.                            **\n");
  printf ("**                      |    [   O      O   ]                           **\n");
  printf ("**                      |   :                :                          **\n");
  printf ("**                      |   |                |                          **\n");
  printf ("**                      |   : ',          ,' :                          **\n");
  printf ("**                      |    {  '-......-'  }                           **\n");
  printf ("**                      |     '.          .'                            **\n");
  printf ("**                      |       '-......-'                              **\n");
  printf ("**                      |                                               **\n");
  printf ("**                    __|________________________|                      **\n");
  printf ("**                    ---------------------------                       **\n");
  printf ("**                                                                      **\n");
  while (1)
    {
      printf ("**                             *****MENU*****                           **\n");
      printf ("**                __________________________________________            **\n");
      printf ("**                |                                         |           **\n");
      printf ("**                | PRESS 1 to ADD custom words.            |           **\n");
      printf ("**                | PRESS 2 to PLAY.                        |           **\n");
      printf ("**                | PRESS 3 to play ONLY with custom words. |           **\n");
      printf ("**                | PRESS ANYKEY to EXIT.                   |           **\n");
      printf ("**                |_________________________________________|           **\n");
      printf ("**                                                                      **\n");
      printf ("**                                                                      **\n");
      printf ("**                ENTER YOUR CHOICE : ");
      scanf ("%d", &choice);printf ("       **");
      printf ("**                                                                      **\n");
      printf ("**                                                                      **\n");
      printf ("**************************************************************************\n");
      printf ("**************************************************************************\n");
      
      index = getRandomWord (choice);
      len = strlen (wordlist[index].word);	//store length of random word.
      printf ("\nSTART:\n");
      printf
	("\n\n    +---+\n    |   |\n        |\n        |\n        |\n        |\n  =========");
      printf ("\t");
      for (int i = 0; i < len; i++)
	{
	  printf ("_ ");
	}			//until here we setup the initial stage for the player to input the first guess.
      done = 1;
      while (done)		//we run while loop until game ends by player guessing right or running out of tries.
	{
	  printf ("\n\nGUESS: ");
	  scanf (" %c", &key[keys]);	//takes player guess.
	  stickFigure (index);	//draws stickfigure and checks for loss.
	  if (done)
	    {
	      displayWord (index);	//displays word and checks for victory
	    }
	  keys++;
	}
      keys = 0;			//resets guesses for next game.
    }
}