#pragma once

#include<iostream>
#include<fstream>
#include <string>
using namespace std;
template<typename T>
class Ngram
{
	T* file;
	int wordCount;
public:
	Ngram()
	{
		file = NULL;
		wordCount = 0;
	}
	void readInput(const char* path)
	{
		std::fstream inputFile;
		inputFile.open(path);
		wordCount=0;
		string Data;
		if (inputFile.is_open())
		{
			getline(inputFile,Data,'\0');
			for (int Index = 0; Index <Data.length(); Index++)
			{
				wordCount++;
			}
			file = new T[wordCount];
			for (int Index = 0; Index < Data.length(); Index++)
			{
				file[Index] = Data[Index];
			}
			inputFile.close();
		}
		else
			std::cout << "Unable to open file";
	}
	void removePunctucationMarks()
	{
		for (int Index = 0; Index < wordCount; Index++)
		{
			if ((file[Index] > 32 && file[Index] < 46) || file[Index] == 47 || (file[Index] > 57 && file[Index] < 65) ||
				(file[Index] > 90 && file[Index] < 97) || (file[Index] > 122 && file[Index] < 127))
			{
				if (Index + 1 < wordCount)
				{
					file[Index]='*';
					for (int Index_Second = Index ; Index < wordCount && (file[Index_Second] > 64 && file[Index_Second] < 91) &&
						(file[Index_Second] > 96 && file[Index_Second] < 123) && (file[Index_Second] > 47 && file[Index_Second] < 58) ||
						file[Index_Second] == 32; Index_Second++)
					{
						file[Index_Second] = '*';
					}
				}
				else
				{
					file[Index]='*';
				}
			}
			else if (file[Index] == '\n')
			{
				if(Index+1<wordCount)
				{
					file[Index]=' ';
				}
				else
				{
					file[Index]='.';
				}
			}
		}
		int length = 0;
		T* temp = new T[wordCount];
		for (int Index = 0, Index_Main = 0; Index < wordCount; Index++)
		{
			if (file[Index] != '*')
			{
				temp[Index_Main] = file[Index];
				Index_Main++;
				length++;
			}
		}
		delete[] file;
		file = new T[length];
		wordCount = length;
		for (int Index = 0; Index < wordCount; Index++)
		{
			file[Index] = temp[Index];
		}
		delete[] temp;
	}
	void removeSpacing()
	{
		int length = 0;
		for (int Index = 0, Index_Second = 0; Index < wordCount; Index++)
		{
			if (Index + 1 < wordCount)
			{
				if (file[Index] == ' ')
				{
					if (Index == 0)
						file[Index] = '*';
					Index_Second = Index + 1;
					if (file[Index_Second] == ' ')
					{
						while (file[Index_Second] == ' ')
						{
							file[Index_Second] = '*';
							if (Index + 1 < wordCount)
							{
								if (file[Index_Second + 1] == '.')
								{
									file[Index] = '*';
								}
							}
							Index_Second++;
						}
					}
					else if (file[Index_Second] == '.')
					{
						file[Index] = '*';
					}
					else if (Index == 0 && file[Index] == ' ')
					{
						file[Index] = '*';
					}
				}
				if (file[Index] == '.')
				{
					Index_Second = Index + 1;
					while (file[Index_Second] == ' ')
					{
						file[Index_Second] = '*';
						Index_Second++;
					}
				}
			}
		}
		T* temp = new T[wordCount];
		for (int Index = 0, Index_Main = 0; Index < wordCount; Index++)
		{
			if (file[Index] != '*')
			{
				temp[Index_Main] = file[Index];
				Index_Main++;
				length++;
			}
		}
		delete[] file;
		file = new T[length];
		wordCount = length;
		for (int Index = 0; Index < wordCount; Index++)
		{
			file[Index] = temp[Index];
		}
		delete[] temp;
	}
	void insertDots()
	{
		string updatedFile = "";
		for (int Index = 0; Index < wordCount; Index++)
		{
			if (Index == 0)
			{
				if (file[Index] != '.')
				{
					updatedFile += '.';
					updatedFile += '.';
					updatedFile += file[Index];
				}
				else
				{
					updatedFile += '.';
					updatedFile += file[Index];
				}
			}
			else if (file[Index] == '.')
			{
				updatedFile += '.';
				updatedFile += file[Index];
			}
			else if (Index == wordCount - 1)
			{
				if (file[Index] == '.')
				{
					updatedFile += file[Index];
					updatedFile += '.';
				}
				else if (file[Index] != '.')
				{
					updatedFile += file[Index];
					updatedFile += '.';
					updatedFile += '.';
				}
			}
			else
			{
				updatedFile += file[Index];
			}
		}
		int dotCount = 0;
		for (int Index = updatedFile.length() - 1; updatedFile[Index] == '.' || updatedFile[Index] == ' '; Index--)
		{
			dotCount++;
		}
		delete[] file;
		if (dotCount >= 2)
			wordCount = updatedFile.length() - dotCount + 2;
		else
			wordCount = updatedFile.length();
		file = new T[updatedFile.length()];
		for (int Index = 0; Index < wordCount; Index++)
		{
			file[Index] = updatedFile[Index];
		}
	}
	void Print()
	{
		for (int Index = 0; Index < wordCount; Index++)
		{
			std::cout << file[Index];
		}
		std::cout << std::endl;
	}
	void WriteFile(std::string GramName, std::string *&Words, int *&FileCount, int &Length)
	{
		ofstream OutFile;
		OutFile.open(GramName);
		if (OutFile.is_open())
		{
			for (int Index = 0; Index < Length; Index++)
			{
				OutFile << Words[Index];
				OutFile <<" "<< FileCount[Index];
				if(Index!=Length-1)
					OutFile<<"\n";
			}
			OutFile.close();
		}
		else
			cout << "ERROR" << endl;
		delete[]Words;
		delete[]FileCount;
		Length = 0;
	}
	void SizeFix(std::string *&Words, int *&FileCount, int &Length, int &Actual_Length)
	{
		std::string *tempW = new string[Actual_Length];
		int *tempL = new int[Actual_Length];
		for (int Index = 0; Index < Actual_Length; Index++)
		{
			tempW[Index] = Words[Index];
			tempL[Index] = FileCount[Index];

		}
		delete[] Words;
		delete[] FileCount;
		Length = Actual_Length;
		Words = new string[Length];
		FileCount = new int[Length];
		for (int Index = 0; Index < Actual_Length; Index++)
		{
			Words[Index] = tempW[Index];
			FileCount[Index] = tempL[Index];

		}
		delete[] tempW;
		delete[] tempL;
	}
	void AddStr(string *&Words, string word, int &Length, int*&FileCount, int &Index_Words)
	{
		
		bool isIn = false;
		for (int Index_Second = 0; Index_Second < Length; Index_Second++)
		{
			if (word == Words[Index_Second])
			{
				isIn = true;
				FileCount[Index_Second] += 1;
				break;
			}
		}
		if (isIn == false)
		{
			Words[Index_Words] = word;
			Index_Words++;
			Length++;
		}
	}
	string* nGrams(T *file, int wordCount, std::string GramName, int gCount, int &WordsLength)
	{
	int Length = 0;
	for (int Index = 0; Index < wordCount; Index++)
	{
		if (file[Index] == '.' || file[Index] == ' ')
		{
			Length++;
		}
	}
	string *Words = new string[Length];
	int *FileCount = new int[Length];
	for (int Index = 0; Index < Length; Index++)
	{
		Words[Index] = "";
		FileCount[Index] = 1;
	}
	string word;
	int Actual_Length = 0;
	for(int Index=0,Index_Words=0,Index_Second=0,count=0;Index<wordCount;Index++)
	{
		if(Index_Second>wordCount)
			break;
		if(file[Index]=='.')
		{
			if(word.length()==0)
			{
				if(Index_Second<wordCount)
				{
				word=".";
				if(gCount>1)
				{
					word+=' ';
				}
				count=1;
				for(Index_Second=Index+1;Index_Second<wordCount && count!=gCount;Index_Second++)
				{
					if(file[Index_Second]=='.')
					{
						if(gCount<=2)
						{
							count++;
							word+=file[Index_Second];
						}
						else if(gCount>2)
						{
							word+=file[Index_Second];
							if(count!=gCount)
								word+=' ';
							count++;
						}
					}
					else if(file[Index_Second]==' ')
					{
						if(gCount<=2)
						{
							count++;
						}
						else if(gCount>2)
						{
							count++;
							if(count!=gCount)
								word+=file[Index_Second];
						}
					}
					else
					{
						word+=file[Index_Second];
					}
				}
				AddStr(Words,word,Actual_Length,FileCount,Index_Words);
				}
			}
			else if(word.length()>0)
			{
				if(gCount>1)
				{
					word+=' ';
				}
				count=1;
				for(Index_Second=Index;Index_Second<wordCount;Index_Second++)
				{
					if(count==gCount)
						break;
					if(file[Index_Second]=='.')
					{
						if(gCount<=2)
						{
							count++;
							word+=file[Index_Second];
						}
						else if(gCount>2)
						{
							count++;
							word+=file[Index_Second];
							if(count!=gCount)
								word+=' ';
						}
					}
					else if(file[Index_Second]==' ')
					{
						if(count<=2)
						{
							count++;
						}
						else
						{
							count++;
							word+=file[Index_Second];
							if(count!=gCount)
								word+=' ';
						}
					}
					else
					{
						word+=file[Index_Second];
					}
				}
				AddStr(Words,word,Actual_Length,FileCount,Index_Words);
				if(Index_Second<wordCount)
				{
				Index--;
				if(gCount>1)
					Index_Second-=1;
				}
			}
			word="";
		}
		else if(file[Index]==' ')
		{
			if(gCount>1)
			{
				word+=' ';
			}
			count=1;
			for(Index_Second=Index+1;Index_Second<wordCount && count!=gCount;Index_Second++)
			{
				if(file[Index_Second]=='.')
				{
					if(gCount<=2)
					{
						count++;
					}
					else
					{
						count++;
						if(count!=gCount)
						{
							word+=' ';
							word+=file[Index_Second];
						}
					}
				}
				else if(file[Index_Second]==' ')
				{
					if(gCount<=2)
					{
						//word+=file[Index_Second];
						count++;
					}
					else
					{
						count++;
						if(gCount!=count)
							word+=file[Index_Second];
					}
				}
				else
				{
					word+=file[Index_Second];
				}
			}
			AddStr(Words,word,Actual_Length,FileCount,Index_Words);
			word="";
		}
		else
		{
			word+=file[Index];
		}
	}
	/*for(int Index=0;Index<Actual_Length;Index++)
		cout<<Words[Index]<<" "<<FileCount[Index]<<endl;*/
	SizeFix(Words, FileCount, Length, Actual_Length);
	if(GramName=="Validation")
	{
		WordsLength=Length;
		return Words;
	}
	WriteFile(GramName,Words,FileCount,Length);
}
	void generateUnigrams()
	{
		int UnNessary = 0;
		nGrams(file, wordCount, "unigram.txt", 1, UnNessary);

	}
	void generateBigrams()
	{
		int UnNessary = 0;
		nGrams(file, wordCount, "bigram.txt", 2, UnNessary);
	}
	void generateTrigrams()
	{
		int UnNessary = 0;
		nGrams(file, wordCount, "trigram.txt", 3, UnNessary);
	}
	char* getText()
	{
		char *temp=new char[wordCount];
		for(int Index=0;Index<wordCount;Index++)
		{
			temp[Index]=file[Index];
		}
		temp[wordCount]='\0';
		return temp;
	}
	int getCount()
	{
		return wordCount;
	}
	void FileSetting(T* &file, int &wordCount)
	{
		for (int Index = 0; Index < wordCount; Index++)
		{
			if ((file[Index] > 32 && file[Index] < 46) || file[Index] == 47 || (file[Index] > 57 && file[Index] < 65) ||
				(file[Index] > 90 && file[Index] < 97) || (file[Index] > 122 && file[Index] < 127))
			{
				file[Index] = ' ';
				if (Index + 1 < wordCount)
				{
					for (int Index_Second = Index + 1; Index < wordCount && (file[Index_Second] > 64 && file[Index_Second] < 91) &&
						(file[Index_Second] > 96 && file[Index_Second] < 123) && (file[Index_Second] > 47 && file[Index_Second] < 58) ||
						file[Index_Second] == 32; Index_Second++)
					{
						file[Index_Second] = '*';
					}
				}
			}
			else if (file[Index] == '\n')
			{
				file[Index] = '*';
			}
		}
		int length = 0;
		T* temp = new T[wordCount];
		for (int Index = 0, Index_Main = 0; Index < wordCount; Index++)
		{
			if (file[Index] != '*')
			{
				temp[Index_Main] = file[Index];
				Index_Main++;
				length++;
			}
		}
		delete[] file;
		file = new T[length];
		wordCount = length;
		for (int Index = 0; Index < wordCount; Index++)
		{
			file[Index] = temp[Index];
		}
		delete[] temp;
		length = 0;
		for (int Index = 0, Index_Second = 0; Index < wordCount; Index++)
		{
			if (Index + 1 < wordCount)
			{
				if (file[Index] == ' ')
				{
					if (Index == 0)
						file[Index] = '*';
					Index_Second = Index + 1;
					if (file[Index_Second] == ' ')
					{
						while (file[Index_Second] == ' ')
						{
							file[Index_Second] = '*';
							if (Index + 1 < wordCount)
							{
								if (file[Index_Second + 1] == '.')
								{
									file[Index] = '*';
								}
							}
							Index_Second++;
						}
					}
					else if (file[Index_Second] == '.')
					{
						file[Index] = '*';
					}
					else if (Index == 0 && file[Index] == ' ')
					{
						file[Index] = '*';
					}
				}
				if (file[Index] == '.')
				{
					Index_Second = Index + 1;
					while (file[Index_Second] == ' ')
					{
						file[Index_Second] = '*';
						Index_Second++;
					}
				}
			}
		}
		temp = new T[wordCount];
		for (int Index = 0, Index_Main = 0; Index < wordCount; Index++)
		{
			if (file[Index] != '*')
			{
				temp[Index_Main] = file[Index];
				Index_Main++;
				length++;
			}
		}
		delete[] file;
		file = new T[length];
		wordCount = length;
		for (int Index = 0; Index < wordCount; Index++)
		{
			file[Index] = temp[Index];
		}
		delete[] temp;
		string updatedFile = "";
		for (int Index = 0; Index < wordCount; Index++)
		{
			if (Index == 0)
			{
				if (file[Index] != '.')
				{
					updatedFile += '.';
					updatedFile += '.';
					updatedFile += file[Index];
				}
				else
				{
					updatedFile += '.';
					updatedFile += file[Index];
				}
			}
			else if (file[Index] == '.')
			{
				updatedFile += '.';
				updatedFile += file[Index];
			}
			else if (Index == wordCount - 1)
			{
				if (file[Index] == '.')
				{
					updatedFile += file[Index];
					updatedFile += '.';
				}
				else if (file[Index] != '.')
				{
					updatedFile += file[Index];
					updatedFile += '.';
					updatedFile += '.';
				}
			}
			else
			{
				updatedFile += file[Index];
			}
		}
		int dotCount = 0;
		for (int Index = updatedFile.length() - 1; updatedFile[Index] == '.' || updatedFile[Index] == ' '; Index--)
		{
			dotCount++;
		}
		delete[] file;
		if (dotCount >= 2)
			wordCount = updatedFile.length() - dotCount + 2;
		else
			wordCount = updatedFile.length();
		file = new T[updatedFile.length()];
		for (int Index = 0; Index < wordCount; Index++)
		{
			file[Index] = updatedFile[Index];
		}
	}
	bool compare(std::string word1, string word2, int length)
	{
		bool isEqual = true;
		for (int Index = 0; Index < length; Index++)
		{
			if (word1[Index] != word2[Index])
			{
				isEqual = false;
				break;
			}
		}
		return isEqual;

	}
	int StringToInt(std::string word)
	{
		int count = 0;
		for (int Index = 0; Index < word.length(); Index++)
		{
			count *= 10;
			count += 48 - int(word[Index]);
		}
		return count*-1 ;
	}
	double SearchInBigrams(std::string biGram)
	{
		double count = 0.0;
		ifstream Bigrams;
		Bigrams.open("bigram.txt");
		std::string line, substr;
		bool inBigrams = false;
		while (!Bigrams.eof())
		{
			line = "", substr = "";
			getline(Bigrams, line, '\n');
			if (line.length() > 0)
				inBigrams = compare(line, biGram, biGram.length());
			if (inBigrams)
			{
				substr="";
				for (int Index = 0, spaceCount = 0; Index < line.length(); Index++)
				{
					if (line[Index] == ' ')
						spaceCount++;
					else if (spaceCount == 2)
						substr += line[Index];
				}
				count = StringToInt(substr);
				break;
			}
		}
		Bigrams.close();
		return count;
	}
	double SearchInTrigrams(std::string triGram)
	{
		double count = 0.0;
		ifstream Trigrams;
		Trigrams.open("trigram.txt");
		std::string line, substr;
		bool inTrigrams = false;
		while (!Trigrams.eof())
		{
			line = "", substr = "";
			getline(Trigrams, line, '\n');
			substr=line;
			if (line.length() > 0)
				inTrigrams = compare(line, triGram, triGram.length());
			if (inTrigrams)
			{
				substr="";
				for (int Index = 0, spaceCount = 0; Index < line.length(); Index++)
				{
					if (line[Index] == ' ')
						spaceCount++;
					else if (spaceCount == 3)
						substr += line[Index];
				}
				count = StringToInt(substr);
				break;
			}
		}
		Trigrams.close();
		return count;
	}
	double Probability(std::string *Trigrams, int Length)
	{
		double Prob = 0;
		double count1 = 0.0, count2 = 0.0;
		std::string word;
		for (int Index_Grams = 0; Index_Grams < Length; Index_Grams++)
		{
			word = "";
			count1 = SearchInTrigrams(Trigrams[Index_Grams]);
			if (count1 > 0)
			{
				for (int Index = 0, spaceCount = 0; Index < Trigrams[Index_Grams].length(); Index++)
				{
					if (Trigrams[Index_Grams][Index] == ' ')
						spaceCount++;
					if (spaceCount == 2)
						break;
					else
						word += Trigrams[Index_Grams][Index];
				}
				count2 = SearchInBigrams(word);
				if (count2 == 0)
					return 1;
				Prob += log(count1 / count2);
			}
			if (count1 <= 0)
				return 1;
		}
		return Prob;
	}
	double CreationProbability(string Trigram)
	{
		double count1 = SearchInTrigrams(Trigram);
		std::string word = "";
		for (int Index = 0, spaceCount = 0; Index < Trigram.length(); Index++)
		{
			if (Trigram[Index] == ' ')
				spaceCount++;
			if (spaceCount == 2)
				break;
			else
				word += Trigram[Index];
		}
		double count2 = SearchInBigrams(word);
		return log(count1 / count2);
	}
	double validateSentance(string Sentence)
	{
		T *Ofile = new T[Sentence.length()];
		int length=0;
		for (int Index = 0; Index < Sentence.length(); Index++)
		{
			Ofile[Index] = Sentence[Index];
			length++;
		}
		FileSetting(Ofile,length);
		Sentence="";
		for (int Index = 0; Index < length; Index++)
		{
			 Sentence+=Ofile[Index];
		}
		double Prob = 0;
		int Length;
		std::string *myGrams;
		myGrams = nGrams(Ofile, Sentence.length(), "Validation", 3, Length);
		Prob = Probability(myGrams, Length);
		return Prob;
	}
	bool SearchForWord(int position, string &word)
	{
		bool isIn = false;
		word = "";
		ifstream Trigrams;
		Trigrams.open("trigram.txt");
		std::string line;
		int count = 1;
		while (!Trigrams.eof())
		{
			line = "";
			getline(Trigrams, line, '\n');
			if (count == position)
			{
				if (line[0] == '.' && line[2] != '.')
				{
					isIn = true;
					break;
				}
				else
				{
					isIn = false;
					break;
				}
			}
			count++;
		}
		if (isIn)
		{
			for (int Index = 0, spaceCount = 0; Index < line.length(); Index++)
			{
				if (line[Index] == ' ')
					spaceCount++;
				if (spaceCount == 3)
					break;
				else
					word += line[Index];
			}
			// For removing the first . and space 
			line="";
			for (int Index = 2; Index < word.length(); Index++)
			{
				line+=word[Index];
			}
			word=line;
		}
		Trigrams.close();
		return isIn;
	}
	void GetMaxProbWord(std::string word, string *&ProbWord, int *&ProbCount, int Length, int &Actual_Length)
	{
		delete[] ProbWord, delete[] ProbCount;
		ProbWord = new string[Length];
		ProbCount = new int[Length];
		ifstream Trigrams;
		Trigrams.open("trigram.txt");
		std::string line = "abcd", substr = "abcd", Bigram = "";
		bool inTrigrams;
		for (int Index_Prob = 0; !Trigrams.eof();)
		{
			getline(Trigrams, line, '\n');
			if (line.length() > 0)
				inTrigrams = compare(line, word, word.length());
			if (inTrigrams)
			{
				substr = "";
				for (int Index = 0, spaceCount = 0; Index < line.length(); Index++)
				{
					if (line[Index] == ' ')
						spaceCount++;
					if (spaceCount == 3)
						break;
					else
						substr += line[Index];
				}
				ProbWord[Index_Prob] = substr;
				ProbCount[Index_Prob] = CreationProbability(substr);
				Index_Prob++;
				Actual_Length++;
			}
		}
	}
	string sentenceProbability(std::string word)
	{
		int length = 5, Actual_Length = 0;
		int *ProbCount = new int[length];
		std::string sentence = word, save = "",sameWord="";
		int maxIndex = 0, Max = 0;
		std::string *ProbWord = new string[length];
		while (word[word.length() - 1] != '.')
		{
			Max = 0;
			maxIndex = 0;
			Actual_Length = 0;
			GetMaxProbWord(word, ProbWord, ProbCount, length, Actual_Length);
			for (int Index = 0; Index < Actual_Length; Index++)
			{
				if (ProbCount[Index] > Max)
				{
					Max = ProbCount[Index];
					maxIndex = Index;
				}
			}
			word = "";
			for (int Index = 0, spaceCount = 0; Index < ProbWord[maxIndex].length(); Index++)
			{
				if (spaceCount == 1)
					word += ProbWord[maxIndex][Index];
				if (spaceCount == 0 && ProbWord[maxIndex][Index] == ' ')
					spaceCount++;
			}
			save = "";
			for (int Index = 0, spaceCount = 0; Index < ProbWord[maxIndex].length(); Index++)
			{
				if (spaceCount == 2)
					save += ProbWord[maxIndex][Index];
				if (spaceCount < 2 && ProbWord[maxIndex][Index] == ' ')
					spaceCount++;
			}
			if(save!=".")
				sentence+=' ';
			sentence += save;
			if(sentence.length()>1000)
				return "";
		}
		return sentence;
	}
	string sentenceCreation(int position)
	{
		std::string word = "";
		std::string sentence = "NOT A VALID START POINT";
		if (SearchForWord(position, word))
		{
			sentence = sentenceProbability(word);
		}
		return sentence;
	}
};


Ngram<char> Word;

void readInput(const char* path)
{
	Word.readInput(path);
}
char* getText()
{
	char* temp=new char[Word.getCount()];; char* file=Word.getText();
	int Length=0;
	for(int Index=0;Index<Word.getCount();Index++)
		{
			temp[Index]=file[Index];
			Length++;
		}
	temp[Length]='\0';
	return temp;
}
void removePunctucationMarks()
{
	Word.removePunctucationMarks();
}
void removeSpacing()
{
	Word.removeSpacing();
}
void insertDots()
{
	Word.insertDots();
}
void generateUnigrams()
{
	Word.generateUnigrams();
}
void generateBigrams()
{
	Word.generateBigrams();
}
void generateTrigrams()
{
	Word.generateTrigrams();
}
double validateSentance(string valid)
{
	return Word.validateSentance(valid);
}
string sentenceCreation(int pos)
{
	return Word.sentenceCreation(pos);
}
int getCount()
{
		return Word.getCount();
}