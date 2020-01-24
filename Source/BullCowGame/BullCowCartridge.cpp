// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    Isograms = GetValidWords(Words);

    SetupGame();
}

void UBullCowCartridge::OnInput(const FString &Input) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else // check player guess
    {
        ProcessGuess(Input, Lives);
    }
}

void UBullCowCartridge::SetupGame()
{
    PrintLine(TEXT("*** Welcome to Bull Cows the Game! ***"));

    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)]; // set the hidden code
    Lives = HiddenWord.Len();
    bGameOver = false;

    PrintLine(TEXT("Guess the %i letter word..."), HiddenWord.Len());
    PrintLine(TEXT("You have %i Lives"), Lives);
    PrintLine(TEXT("Type in your guess and press Enter to\ncontinue..."));
    PrintLine(TEXT("The hidden word is %s"), *HiddenWord);
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress Enter to Continue...\n"));
}

void UBullCowCartridge::ProcessGuess(FString Guess, int32 Counter)
{
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("You Win!"));
        EndGame();
        return;
    }

    if (HiddenWord.Len() != Guess.Len())
    {
        PrintLine(TEXT("The hidden word is %i letters long"), HiddenWord.Len());
        PrintLine(TEXT("Sorry, try guessing again! \nYou have %i lives remaining."), Lives);
        return;
    }

    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters, guess again!"));
        return;
    };

    PrintLine(TEXT("Lost a life!"));

    --Lives;

    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no Lives Left!"));
        PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
        EndGame();
        return;
    }

    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
    PrintLine(TEXT("Guess again, you have %i lives left"), Lives);
}

bool UBullCowCartridge::IsIsogram(FString Word) const
{
    bool result = true;

    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                result = false;
            }
        }
    }

    return result;
}

TArray<FString> UBullCowCartridge::GetValidWords(TArray<FString> WordsList) const
{
    TArray<FString> result;
    for (FString Word : WordsList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            result.Emplace(Word);
        }
    }

    return result;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString &Guess) const
{
    FBullCowCount Count;

    for (int32 i = 0; i < Guess.Len(); i++)
    {
        if (Guess[i] == HiddenWord[i])
        {
            Count.Bulls++;
            continue;
        }

        for (int32 j = 0; j < HiddenWord.Len(); j++)
        {
            if (Guess[i] == HiddenWord[j])
            {
                Count.Cows++;
                break;
            }
        }
    }

    return Count;
}