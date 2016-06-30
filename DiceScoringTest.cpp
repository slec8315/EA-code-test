/*******************************************************
      Program Name 	:  Dice on a Yacht
      Version      	:  2.0
      Programmer   	:  Steve Leclaire

This file contains 2 primary functions as specified for
scoring the results of a dice cast:

int getScore(string category, array <int, 5> scores)
input:
    category: indicator for scoring result
    scores[]: dice results
output:
    total score based on category and dice results


string getSuggestion(array <int, 5> scores)
input:
    scores[]: dice results
output:
    highest winning category based on dice results

*******************************************************/

#include <iostream>									//Standard include for I/O
#include <stdlib.h>                             	//Standard include for system calls
#include <time.h>                               	//For Run Time Summary
#include <cstring>                                  //Standard include for string
#include <algorithm>                                //Standard include for sorting integer array
#include <random>									//Standard include for RNG
#include <array>                                    //Standard include for arrays

using namespace std;

const int
        seed = time(0);         					    //RNG seed

mt19937
        RANDOM(seed);                                   //Mersenne Twister object

int GetRand(int range)                                  //Randomization algorithm for generating
{                                                       //a random integer from 0 to called range.
		uniform_int_distribution<int> dist(0,range-1);  //Used for rolls of the dice for testing.
        return  dist(RANDOM);
};

bool OfAKind(int amount, array <int, 5> scores)     //Function to determine if the amount of required scores are in the array.
{                                                   //This function was created to avoid repeating code in the "getScore"
    int totalOfAKind = 1;                           //function.  Currently, this function receives the amount of scores to look
    for(int i = 1; i < scores.size(); i++)          //for and the sorted score array.  If the total amount is found, we have x
    {                                               //of a kind.  This was constructed with the expectation that additional
        if(scores[i] == scores[i - 1])              //x of a kind totals can be added later without changing this function.
        {
            totalOfAKind++;
            if(totalOfAKind == amount)
                return true;
        }
        else
            totalOfAKind = 1;
    }
    return false;
};

bool Straight(int length, array <int, 5> scores)    //Function to determine if a straight exists within a sorted array. This
{                                                   //function was created to avoid repeating code in the "getScore" function.
    int counter = 1;                                //Currently, this function receives the length of the straight to look for and
    for(int i = 1; i < scores.size(); i++)          //the sorted score array.  If the counter is equal to the length, We've found the
    {                                               //required length of the straight.  This was constructed with the expectation
        if(scores[i] - 1 == scores[i - 1])          //that additional lengths of straights  can be added later without changing this
            counter++;                              //function.
        else
            counter = 1;
        if(counter == length)
            return true;
    }
    return false;
};

int scoreNumber(int value, array <int, 5> scores)   //Scoring function for individual numbers.  This function was created
{                                                   //to avoid repeating code in the "getScore" function.  Currently, this
    int total = 0;                                  //function receives the value of the die side being scored and the score
    for(int i = 0; i < scores.size(); i++)          //array, and will output the total of all values found.  Should the
    {                                               //amount of dice or numerical scoring categories increase in the future,
        if(scores[i] == value)                      //this number scoring function will be able to handle it.
            total+=value;
    }
    return total;
};

int diceSum(array <int, 5> scores)
{                                                   //Total sum of all dice.  This function was created to avoid repeating code
    int total = 0;                                  //in the "getScore" function.  Currently, this function receives the score
    for(int i = 0; i < scores.size(); i++)          //array, and will output the total of all values within that array.  Should
    {                                               //the amount of dice or amount of sides increase in the future, this
        total+=scores[i];                           //function will be able to handle it.
    }
    return total;
};

/*******************************************************
Functions created for test starts here
*******************************************************/

int getScore(string category, array <int, 5> scores)
{
        const int scoresize = scores.size();                //Create a new array of scores that can be rearranged so as to not
        array <int, 5> sortedscores = {};                   //corrupt the order of the original array

		for(int i = 0; i < scoresize; i++)                  //For every array entry
            sortedscores[i] = scores[i];                    //assign to the new array

        for(int i = 1; i < scoresize; i++)                  //sort the sortedscore array
            if(sortedscores[i] < sortedscores[i - 1])       //if 2 elements are unsorted
            {
                swap(sortedscores[i], sortedscores[i - 1]); //sort them relative to each other
                i = 0;                                      //and start again
            }

        if(category == "Ones")                              //Score calculations for individual numbers are processed here.
            return(scoreNumber(1, scores));                 //Currently, as specified by the parameters given, only categories
        else if(category == "Twos")                         //one through eight are available.  Should the die require additional
            return(scoreNumber(2, scores));                 //sides in the future, additional scoring categories can be added as
        else if(category == "Threes")                       //needed by adding new "else if" statements.
            return(scoreNumber(3, scores));
        else if(category == "Fours")
            return(scoreNumber(4, scores));
        else if(category == "Fives")
            return(scoreNumber(5, scores));
        else if(category == "Sixes")
            return(scoreNumber(6, scores));
        else if(category == "Sevens")
            return(scoreNumber(7, scores));
        else if(category == "Eights")
            return(scoreNumber(8, scores));

        else if(category == "ThreeOfAKind")                 //Score calculations for the "OfAKind" series are processed here.
            if(OfAKind(3, sortedscores))                    //This also encompasses the AllSame scoring as well, which is essentially
                return(diceSum(scores));                    //5 of a kind.  Additional "OfAKind" scoring capabilities can be added as
            else                                            //needed by adding new "else if" statements.
                return(0);

        else if(category == "FourOfAKind")
            if(OfAKind(4, sortedscores))
                return(diceSum(scores));
            else
                return(0);

        else if(category == "FullHouse")                    //Due to the nature of scoring a FullHouse, this couldn't be made as dynamic
        {                                                   //as the other scoring functions.
            if((sortedscores[0] == sortedscores[1]) &&      //if the first 2 items are the same in the sorted array and
               (sortedscores[3] == sortedscores[4]) &&      //the last 2 items are the same in the sorted array and
               ((sortedscores[2] == sortedscores[0]) ||     //the middle item matches either the first item or
                (sortedscores[2] == sortedscores[4])))      //the middle item matches the last item
                return(25);                                 //Full House!
            else
                return(0);                                  //else not Full House
        }

        else if(category == "SmallStraight")                //Score calculations for the "Straight" series are processed here.
            if(Straight(4, sortedscores))                   //Additional "Straight" scoring capabilities can be added as needed by
                return(30);                                 //adding new "else if" statements.
            else
                return(0);

        else if(category == "LargeStraight")
            if(Straight(5, sortedscores))
                return(40);
            else
                return(0);

        else if(category == "AllDifferent")                 //Score calculation for "AllDifferent"
        {
            for(int i = 1; i < scoresize; i++)              //For every item starting with the second item
                for(int j = i - 1; j >= 0; j--)             //Look through every item before the current item
                    if(scores[i] == scores[j])              //if any are the same
                        return (0);                         //No score for "AllDifferent"
            return(40);                                     //if there are no matches, score for "AllDifferent"
        }

        else if(category == "Chance")                       //Score calculation for "Chance"
            return(diceSum(scores));                        //Return the total of all sides

        else if(category == "AllSame")                      //See "OfAKind" description above.
        {
            if(OfAKind(scoresize, sortedscores))
                return(50);
            else
                return(0);
        }
        else                                                //Catch all statement for categories not on the list
            return(0);                                      //no score

};

string getSuggestion(array <int, 5> scores)
{
    int highscore = 0, highcat = 0;                         //Current high score
    enum categories										    //All categories currently supported.  Required by test?
        {Ones, Twos, Threes, Fours, Fives, Sixes, Sevens, Eights, Chance, ThreeOfAKind, FourOfAKind,
        FullHouse, SmallStraight, LargeStraight, AllDifferent, AllSame, TotalCategories};
    string returncat[TotalCategories] =                     //string array of returnable characters
        {"Ones", "Twos", "Threes", "Fours", "Fives", "Sixes", "Sevens", "Eights", "Chance", "ThreeOfAKind",
        "FourOfAKind", "FullHouse", "SmallStraight", "LargeStraight", "AllDifferent", "AllSame"};

    for(int category = Ones; category < TotalCategories; category++)    //For each category
    {
        if(getScore(returncat[category], scores) >= highscore)          //Determine if the score beats the high score
        {
            highscore = getScore(returncat[category], scores);          //If it does, assign a new high score
            highcat = category;                                         //for this category
        }
    }
    return returncat[highcat];                                          //return the highest scoring category

};

/*******************************************************
Code created for testing purposes
*******************************************************/

class Dice                  //Dice class implemented in case of variable sides of dice
{
    int
        sides,              //total sides for the die
        currscore;          //current side for awarding

    public:
        void SetSides (int);//Set the amount of sides for this die
        int RollDemBones(); //Roll this die
        void Print();       //Print the current score for this die
};

void
Dice::SetSides(int val)
{
    sides = val;            //Set the amount of sides for this die
}

int
Dice::RollDemBones()
{
    currscore = GetRand(sides) + 1; //Roll this die
    return (currscore);     //return the resulting roll
}

void
Dice::Print()
{
    cout<<currscore<<" ";   //Print the score results of this die
}

int main()
{
        const int dice = 5;                         //amount of dice to roll
        Dice DemBones[dice];                        //Dice to be rolled
        int sides = 8;                              //amount of sides per die
        array <int, 5> scorearr = {0};              //dice rolls
        string category;                            //scoring category
        char keepRollin = 'y';                      //Loop terminator

		clock_t start, end;                         //stopwatch for run time of the program for efficiency testing

		start = clock();                            //Start the clock!

		for(int i = 0; i != dice; i++)              //for each die
            DemBones[i].SetSides(sides);            //set the amount of sides

        while(keepRollin != 'n')                    //One more! C'mon lucky 7...
        {
            cout<<"Rolls: ";
            for(int i = 0; i != dice; i++)              //for each die
            {
                scorearr[i] = DemBones[i].RollDemBones();//Roll dem bones!
                DemBones[i].Print();
            }
            cout<<endl;
            category = getSuggestion(scorearr);         //calculate the highest scoring category
//		      cout<<"\nEnter Category :";                   //alternatively, enter scoring category
//		      cin>>category;
            cout<<"Score total for "<<category<<" is "<<getScore(category, scorearr)<<".\n\n";
            cout<<"Continue? (y/n) ";
            cin>>keepRollin;
        }
		end = clock();                              //Stop the clock!

		cout<<"Program ran for "<<(end-start) / CLK_TCK<<" seconds.\n";

        system("pause");
        return 0;
}

