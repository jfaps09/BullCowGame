// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"
// #include "Math/UnrealMathUtility.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    Isograms = GetValidWords();
    SetupGame();
}

TArray<FString> UBullCowCartridge::GetValidWords() const {
    TArray<FString> WordsArray;

    for (FString Word : Words)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
            WordsArray.Emplace(Word); 
    }
    return WordsArray;
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    ClearScreen();

    if (bGameOver)
        EndGame(PlayerInput);   
    else
        ProcessGuess(PlayerInput);
}

void UBullCowCartridge::SetupGame() {
    bGameOver = false;
    HiddenWord = Isograms[FMath::RandRange(0,Isograms.Num() - 1)];
    Lives = HiddenWord.Len();

    ClearScreen();
    PrintLine(TEXT("The hidden word is %s"), *HiddenWord);
    PrintLine(TEXT("Welcome to the Bull Cow Game!"));
    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives"), Lives);
}

void UBullCowCartridge::ProcessGuess(const FString& Guess) {
    PrintLine(Guess);            

    if (Guess == HiddenWord) {
        PrintLine(TEXT("YOU WIN!"));
        EndGame(Guess);
    }
    else {
        if(Guess.Len() != HiddenWord.Len()) {
            PrintLine(TEXT("The Hidden Word is %i letters long, try again!"), HiddenWord.Len());
            return;
        }

        if(!IsIsogram(Guess))
        {
            PrintLine(TEXT("No repeating letters, try again!"));
            return;
        }
        FBullCowCount Count = GetBullCows(Guess);
        PrintLine(TEXT("You have %i Bulls and %i Cows"), Count.Bulls, Count.Cows);

        PrintLine(TEXT("WRONG! Lives: %i"), --Lives);
        if (Lives <= 0)
            EndGame(Guess);
    } 
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const{

    for (int32 Index = 0; Index < Word.Len()-1; Index++)
        for (int32 Index2 = Index + 1; Index2 < Word.Len(); Index2++)
            if (Word[Index] == Word[Index2])
                return false;

    return true;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) {
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex]) {
                Count.Cows++;
                break;
            }
        }   
    }
    return Count;
}


void UBullCowCartridge::EndGame(const FString& Input) {
    bGameOver = true;
    PrintLine(TEXT("GAME OVER!"));
    PrintLine(FString::Printf(TEXT("The Hidden word was: %s."), *HiddenWord));
    PrintLine(TEXT("\nWant to play again? Type YES or NO."));

    if (Input.ToLower().Equals("yes"))
        SetupGame();
    else
        return;
}