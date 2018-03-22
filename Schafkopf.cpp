/*
Thomas Redwig
F12Tb

Technologie/Informatik
Hr. Hildner

Schafkopf
*/

/*
TODO:
AI
Playing for money
sort cards in player's hand

https://www.sauspiel.de/schafkopf-lernen/strategie-kurze-karte
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <conio.h>
#include <unistd.h>
#include <windows.h>

#define black 0
#define dblue 1
#define dgreen 2
#define dturquoise 3
#define dred 4
#define dpink 5
#define dyellow 6
#define lgrey 7
#define grey 8
#define blue 9
#define green 10
#define turquoise 11
#define red 12
#define pink 13
#define yellow 14
#define white 15

int hands[4][6]; //2D array, 4 rows (players) 6 columns (cards in hand)
int pile[4]; // array to save played cards every turn
int score[4]; // playerscore
int playedGame=0; // saves the played the game; 9=ES, 8=GS, 7=HS, 6=SS, 5=W, 4=ER, 3=GR, 2=HR, 1=SR, 0=P
int playedBy; // saves the player who announced the game
bool tout; // saves if game was played as tout
int partner; // saves the player who is playing with playedBy
int first; // saves the player who went first this turn
int turn; // game turn counter

debug() //debug function (show every card of every player)
{
	printf("=========== D E B U G ===========\n");
	int i, j;
	for (i=0;i<4;i++)
	{
		printf("Spieler%i: ",i+1);
		for (j=0;j<6;j++)
		{
			printf("%3i ",hands[i][j]);
		}
		printf("\n");
	}
	
	printf("pile: ");
	for(i=0;i<4;i++)
	{
		printf("%i ",pile[i]);
	}
	printf("\npartner: Spieler %i\n",partner+1);
	printf("==== E N D   O F   D E B U G ====\n");
}


void textColor(int fg, int bg)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg+16*bg);
}

printTitle() // print game title
{
	textColor(yellow, black);
	printf( "  ____       _            __ _                __ \n"
	    	" / ___|  ___| |__   __ _ / _| | _____  _ __  / _|\n"
        	" \\___ \\ / __| '_ \\ / _` | |_| |/ / _ \\| '_ \\| |_ \n"
        	"  ___) | (__| | | | (_| |  _|   < (_) | |_) |  _|\n"
    		" |____/ \\___|_| |_|\\__,_|_| |_|\\_\\___/| .__/|_|  \n");
    		textColor(red,black);
    		printf("    by Thomas Redwig  ");
    		textColor(yellow,black);
	    	printf("                |_|       \n\n");
	textColor(white,black);
}

printTurn(int turn) // print turn number with border
{
	textColor(yellow,black);
	printf("\n\t%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",201,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,187);
	printf("\t%c     ",186);
	textColor(red,black);
	printf("R U N D E  %i",turn+1);
	textColor(yellow,black);
	printf("     %c\n",186);
	printf("\t%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",200,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,188);
	textColor(white,black);
}

printCard(int player, int card) // translate decimal code to card name
{
	switch((hands[player][card]-(hands[player][card]%10))/10) // switch on first digit of card-number-code
	{
		case 1:
			textColor(dyellow,black);
			printf("Eichel-   ");
			break;
		case 2:
			textColor(dgreen,black);
			printf("Gr%cn-     ",129);
			break;
		case 3:
			textColor(red,black);
			printf("Herz-     ");
			break;
		case 4:
			textColor(yellow,black);
			printf("Schellen- ");
			break;
		default:
			printf("");
			break;
	}
	switch(hands[player][card]%10) // switch on second digit of card-number-code
	{
		case 1:
			printf("Ass");
			break;
		case 2:
			printf("Zehn");
			break;
		case 3:
			printf("K%cnig",148);
			break;
		case 4:
			printf("Ober");
			break;
		case 5:
			printf("Unter");
			break;
		case 6:
			printf("Neun");
			break;
		default:
			printf("");
			break;
	}
	textColor(white,black);
		
}

printGame(int game) // translate decimal code to game name
{
	textColor(turquoise,black);
	switch(game)
	{
		case 9:
			printf("Eichelsolo");
			break;
		case 8:
			printf("Gr%cnsolo",129);
			break;
		case 7:
			printf("Herzsolo");
			break;
		case 6:
			printf("Schellensolo");
			break;
		case 5:
			printf("Wenz");
			break;
		case 4:
			printf("Geier");
			break;
		case 3:
			printf("Eichelruf");
			break;
		case 2:
			printf("Gr%cnruf",129);
			break;
		case 1:
			printf("Schellenruf");
			break;
		case 0:
			textColor(dturquoise,black);
			printf("Passe");
			break;
		default:
			textColor(red,black);
			printf("Fehler!");
			break;
	}
	if (tout && game != 0)
		printf(" Tout");
	textColor(white,black);
}

int findCard(int card) // input card name as decimal, return player who owns card, returns -1 if card has already been played
{
	int i,j;
	
	for(i=0;i<4;i++)
	{
		for(j=0;j<6;j++)
		{
			if(hands[i][j]==card)
			{
				return i;
			}
		}
	}
//should only reach if card doesn't exist
	return -1;
}

int color(int card)
{
	int i;
	int trumps1[12] = {14,15,24,25,31,32,33,34,35,36,44,45};
	int trumps2[4 ] = {15,25,35,45};
	int trumps6[4 ] = {14,24,34,44};
	int trumps3[12] = {14,15,24,25,34,35,44,45,41,42,43,46};
	int trumps4[12] = {14,15,24,25,34,35,44,45,21,22,23,26};
	int trumps5[12] = {14,15,24,25,34,35,44,45,11,12,13,16};
	switch(playedGame)
	{
		case 1: case 2: case 3: case 7: //ruf or heartsolo
			for (i=0;i<12;i++)
			{
				if (card==trumps1[i])
					return 5;
			}
			break;
		case 5: // Wenz
			for (i=0;i<4;i++)
			{
				if (card==trumps2[i])
					return 5;
			}
			break;
		case 4: // Geier
			for (i=0;i<4;i++)
			{
				if (card==trumps6[i])
					return 5;
			}
			break;
		case 6: // Schellensolo
			for (i=0;i<12;i++)
			{
				if (card==trumps3[i])
					return 5;
			}
			break;
		case 8: // GrÃ¼nsolo
			for (i=0;i<12;i++)
			{
				if (card==trumps4[i])
					return 5;
			}
			break;
		case 9: // Eichelsolo
			for (i=0;i<12;i++)
			{
				if (card==trumps5[i])
					return 5;
			}
			break;
	}
	return ((card-card%10)/10);
}

bool validMove(int player, int card, bool error) // checks if chosen move is valid //TODO:if partner owns two cards of color that was called and nothing else while that color wasn't called he gets stuck in a loop
{
	int i;
	bool hasColor=false, outOfOptions=true;
	for (i=0;i<6;i++)
	{
		if (color(hands[player][i])==color(pile[first]))//check all of player's cards for trump
			hasColor = true;
	}
	if(card < 0 || card > 5) // check if move number exists
	{
		if(error==true)
		{
			textColor(white,dred);
			printf("Bitte geben Sie eine Zahl zwischen 1 und 6 ein!\n");
			textColor(white,black);
		}
		return false;
	}
	else if  (hands[player][card]==0) // check if card has already been played
	{
		if(error==true)
		{
			textColor(white,dred);
			printf("Diese Karte wurde bereits gespielt!\n");
			textColor(white,black);
		}	
		return false;
	}
	else if (color(pile[first])!=color(hands[player][card]) && hasColor && pile[first]!=0)//check if player has suit or trump that was played first, if so, he has to play that color
	{
		if(error==true)
		{
			textColor(white,dred);
			printf("Die Farbe (%s) muss zugegeben werden!\n",color(pile[first])==1? "Eichel" : color(pile[first])==2 ? "Gr\x81n" : color(pile[first])==3? "Herz" : color(pile[first])==4? "Schellen" : "Trumpf");
			textColor(white,black);
		}
		return false;
	}
	else if (player==partner)
	{
		switch(playedGame)
		{
			case 1://schellenruf
				for (i=0;i<6;i++)
				{
					if(color(hands[player][i])!=0 && color(hands[player][i])!=4)
					{
						outOfOptions=false;
						break;
					}
				}
				if(findCard(41)==player && color(hands[player][card])==4 && hands[player][card]!=41 || color(pile[first])!=0 && color(pile[first])!= 4 && !outOfOptions && hands[player][card]==41 || !outOfOptions && pile[first]==0 && hands[player][card]==41)//if partner has ace in hand and he wants to play a card of same suit that isn't the ace, or if color hasn't been played this round and partner doesn't go first
				{
					if(error==true)
						{
							textColor(white,dred);
							printf("Beachten Sie die Regeln des Rufspiels!\n");
							textColor(white,black);
						}
						return false;
				}
				break;
			case 2://grünruf
				for (i=0;i<6;i++)
				{
					if(color(hands[player][i])!=0 && color(hands[player][i])!=2)
					{
						outOfOptions=false;
						break;
					}
				}
				if(findCard(21)==player && color(hands[player][card])==2 && hands[player][card]!=21 || color(pile[first])!=0 && color(pile[first])!= 2 && !outOfOptions && hands[player][card]==21 || !outOfOptions && pile[first]==0 && hands[player][card]==21)
				{
				if(error==true)
					{
						textColor(white,dred);
						printf("Beachten Sie die Regeln des Rufspiels!\n");
						textColor(white,black);
					}
					return false;
				}
				break;
			case 3://eichelruf
				for (i=0;i<6;i++)
				{
					if(color(hands[player][i])!=0 && color(hands[player][i])!=1)
					{
						outOfOptions=false;
						break;
					}
				}
				if(findCard(11)==player && color(hands[player][card])==1 && hands[player][card]!=11 || color(pile[first])!=0 && color(pile[first])!= 1 && !outOfOptions && hands[player][card]==11 || !outOfOptions && pile[first]==0 && hands[player][card]==11)
				{
					if(error==true)
						{
							textColor(white,dred);
							printf("Beachten Sie die Regeln des Rufspiels!\n");
							textColor(white,black);
						}
						return false;
				}
				break;
		}
	}
	return true; // if no breach of rules was detected return true
}

int highCard() // find the player with the highest card of the "pile" of played cards, taking currently played game into consideration
{
	int i,highest=first;
	for(i=0;i<4;i++)
	{
		if (color(pile[highest])!=color(pile[i]))//if different color
		{
			if(color(pile[i])==5)//if one is trump
				highest=i;
		}
		else if(color(pile[i])==5)//if both are trump
		{
			if(pile[highest]%10==pile[i]%10) //if both have same face value
			{
				if (pile[i]<pile[highest]) //check which card has better color
					highest=i;
			}
			else if(pile[i]%10==4 || pile[i]%10==5 && pile[highest]%10!=4)//if card is ober or card is unter and highest is not ober
				highest=i;
			else if(pile[i]<pile[highest]&&!(pile[highest]%10==5 || pile[highest]%10==4))
				highest=i;
		}
		else //if both are color
		{
			if(pile[i]<pile[highest])
				highest=i;
		}
	}
	
	return highest;
}

int pileScore() // return total points value of pile
{
	int i, score=0;
	for (i=0;i<4;i++)
	{
		switch(pile[i]%10)
		{
			case 1:
				score+=11;
				break;
			case 2:
				score+=10;
				break;
			case 3:
				score+=4;
				break;
			case 4:
				score+=3;
				break;
			case 5:
				score+=2;
				break;
			default:
				break;
		}
	}
	return score;
}

play(int player, int card)
{
	pile[player]=hands[player][card];
	printf("Spieler %i: ",player+1);
	printCard(player, card);
	printf("\n");
	hands[player][card]=0;
	return 0;
}

int countColor(int player, int c)
{
	int i, count=0;
	for (i=0;i<6;i++)
	{
		if(color(hands[player][i])==c)
			count++;
	}
	return count;
}

int chooseCard(int player)
{
	int card;
	//random temp
	do
	{
		card = rand()%6;
	}while(!validMove(player, card, false));
	return card;
}


int chooseGame(int player)
{
	int game=0, ober=0, unter=0, herz=0, blatt=0, eichel=0, schellen=0, ass=0, i, card,col;
	
	for(i=0;i<6;i++)//go through all cards in player's hand and count how many of a specific kind/color there are
	{
		card=hands[player][i];
		switch(card%10)
		{
			case 4:
				ober++;
				break;
			case 5:
				unter++;
				break;
			case 1:
				ass++;
				break;
			default:
				break;
		}
		if(card%10!=4 && card%10!=5)
		{
			switch((card-card%10)/10)
			{
				case 1:
					eichel++;
					break;
				case 2:
					blatt++;
					break;
				case 3:
					herz++;
					break;
				case 4:
					schellen++;
					break;
				default:
					break;
			}
		}
	}
	//printf("%i,%i,%i,%i,%i,%i",ober,unter,eichel,blatt,herz,schellen);
	if(ober+unter+herz==5 && ober+unter>=2 && ober>=1 || ober+unter+herz==4 && ober>=2 && ober+unter>=3) // rufspiel
	{
		if(findCard(11)!=player && (findCard(12)==player || findCard(13)==player || findCard(16)==player))
			game=3;
		if(findCard(21)!=player && (findCard(22)==player || findCard(23)==player || findCard(26)==player))
			game=2;
		if(findCard(41)!=player && (findCard(42)==player || findCard(43)==player || findCard(46)==player))
			game=1;
	}
	if(ober>=3 || ober>=2 && findCard(14)==player) //enough ober for geier
	{
		if(ass>=2 || findCard(11)==player && (findCard(12)==player || findCard(13)==player) || findCard(21)==player && (findCard(22)==player || findCard(23)==player) || findCard(31)==player && (findCard(32)==player || findCard(33)==player) || findCard(41)==player && (findCard(42)==player || findCard(43)==player))
			game=4;
	}
	if(unter>=3 || unter>=2 && findCard(15)==player) //enough unter for wenz
	{
		if(ass>=2 || findCard(11)==player && (findCard(12)==player || findCard(13)==player) || findCard(21)==player && (findCard(22)==player || findCard(23)==player) || findCard(31)==player && (findCard(32)==player || findCard(33)==player) || findCard(41)==player && (findCard(42)==player || findCard(43)==player))
			game=5;
	}
	
//	if(ober>=2)//nah
//		game=4;
//	if(unter>=2)//especially nah
//		game=5;
	
	
	return game;
}

int playerChooseCard()
{
	int card, status;
	char temp;
	do
	{
		printf("Welche Karte m%cchten Sie spielen?: ",148);
		status = scanf("%d",&card);//status saves error value of scanf (1 if digit was read correctly)
		scanf("%c",&temp);//catches \n character after pressing enter key
		card--;
		if(status!=1)
		{
			textColor(white, dred);
			printf("Bitte geben Sie eine Zahl ein!\n");
			textColor(white,black);
		}
	}while(status!=1 || !validMove(0, card, true));//keep asking for input while move is not valid or input is not a number
	return card;
}

int playerChooseGame()
{
	char input, option, option2;
	int game=-1;
	printf("\nM%cchten Sie ein Spiel ansagen?\n",148);
	do
	{
		do
		{
			printf("(S)olo, (W)enz, (G)eier, (R)ufspiel, (T)outspiel, (P)assen:\n>");
			fflush(stdin);
			input=getchar();
		} while(toupper(input)!='S' && toupper(input)!='W' && toupper(input)!='R' && toupper(input)!='G' && toupper(input)!='P' && toupper(input)!='T');
		switch(toupper(input))
		{
			case 'T':
				do
				{
				printf("Welches Spiel m%cchten Sie als Tout ansagen?\n(S)olo, (W)enz, (G)eier, (Z)ur%cck:\n>",148,129);
					fflush(stdin);
					option=getchar();
				} while(toupper(option)!='S' && toupper(option)!='W' && toupper(option)!='G' && toupper(option)!='Z');
				switch(toupper(option))
				{
					case 'S':
						do
						{
							printf("Farbe? (E)ichel, (G)r%cn, (H)erz, (S)chellen, (Z)ur%cck:\n>",129,129);
							fflush(stdin);
							option2=getchar();
						} while(toupper(option2)!='E' && toupper(option2)!='G' && toupper(option2)!='H' && toupper(option2)!='S' && toupper(option2)!='Z');
						switch(toupper(option2))
						{
							case 'E':
								game=9;
								tout=true;
								break;
							case 'G':
								game=8;
							tout=true;
								break;
							case 'H':
								game=7;
								tout=true;
								break;
							case 'S':
								game=6;
								tout=true;
								break;
							case 'Z':
								game=-1;
								break;
						}
						break;
					case 'W':
						game=5;
						tout=true;
						break;
					case 'G':
						tout=true;
						game=4;
						break;
					case 'Z':
						game=-1;
						break;
				}
				break;
			case 'S':
				do
				{
					printf("Farbe? (E)ichel, (G)r%cn, (H)erz, (S)chellen, (Z)ur%cck:\n>",129,129);
					fflush(stdin);
					option=getchar();
				} while(toupper(option)!='E' && toupper(option)!='G' && toupper(option)!='H' && toupper(option)!='S' && toupper(option)!='Z');
				switch(toupper(option))
				{
					case 'E':
						game=9;
						break;
					case 'G':
						game=8;
						break;
					case 'H':
						game=7;
						break;
					case 'S':
						game=6;
						break;
					case 'Z':
						game=-1;
						break;
				}
				break;
			case 'R':
				do
				{
					printf("Farbe? (E)ichel, (G)r%cn, (S)chellen, (Z)ur%cck:\n>",129,129);
					fflush(stdin);
					option=getchar();
				} while(toupper(option)!='E' && toupper(option)!='G' && toupper(option)!='H' && toupper(option)!='S' && toupper(option)!='Z');
				switch(toupper(option))
				{
					case 'E':
						if(findCard(11)!=0 && (findCard(12)==0 || findCard(13)==0 || findCard(16)==0))
						{
							game=3;
							break;
						}
						else
							{
								textColor(white,dred);
								printf("Rufspiel nur erlaubt, wenn man mindestens eine Karte der Farbe besitzt, aber nicht das Ass!\n");
								textColor(white,black);
								game=-1;
								break;
							}
					case 'G':
						if(findCard(21)!=0 && (findCard(22)==0 || findCard(23)==0 || findCard(26)==0))
						{
							game=2;
							break;
						}
						else
							{
								textColor(white,dred);
								printf("Rufspiel nur erlaubt, wenn man mindestens eine Karte der Farbe besitzt, aber nicht das Ass!\n");
								textColor(white,black);
								game=-1;
								break;
							}
					case 'S':
						if(findCard(41)!=0 && (findCard(42)==0 || findCard(43)==0 || findCard(46)==0))
						{
							game=1;
							break;
						}
						else
							{
								textColor(white,dred);
								printf("Rufspiel nur erlaubt, wenn man mindestens eine Karte der Farbe besitzt, aber nicht das Ass!\n");
								textColor(white,black);
								game=-1;
								break;
							}
					case 'Z':
						game=-1;
						break;
				}
				break;
			case 'W':
				game=5;
				break;
			case 'G':
				game=4;
				break;
			case 'P':
				game=0;
				break;
		}
	}while(game==-1);
	return game;
}

findPartner()
{
	switch(playedGame)
	{
		case 3:
			partner=(findCard(11));
			break;
		case 2:
			partner=(findCard(21));
			break;
		case 1:
			partner=(findCard(41));
			break;
		default:
			partner=-1;
			break;
	}
}

main()
{
	char cont;
	srand(time(NULL));
	int start=rand()%4;
	do
	{
		int cards[24] = {11,12,13,14,15,16,21,22,23,24,25,26,31,32,33,34,35,36,41,42,43,44,45,46}; //initializing all available cards as number-codes. First digit = suit, second digit = rank
		int i=0, j=0,index,length=24,player=0, card=0, game=0,winner,trick;
		char input;
		first=start;
		playedGame=0;
		playedBy=-1;
		partner=-1;
		
		for (i=0;i<4;i++)
			score[i]=0; // reset score
		
		for (i=0;i<4;i++) //loop through all players, shuffle and deal cards
		{
			for (j=0;j<6;j++) //loop through all cards in hand
			{
				index=rand()%length;
				hands[i][j]=cards[index]; // put card into one of the hands
				cards[index]=cards[length-1]; // mark card as used
				length--;
			}
		}
		system("cls");
		printTitle();
//debug();
		
		for (turn=0;turn<6;turn++) // Main game loop, 6 rounds
		{
			first=first%4;
			for (i=0;i<4;i++)
			{
				pile[i]=0; // reset score
			}
			if (turn == 0) // if game has just started
			{
				printf("Spieler %i hat gegeben, Spieler %i kommt raus\n\nIhre Hand:\n\n",first == 0 ? 4 : first ,first+1);
				for (i=0;i<6;i++)
				{
					printf("%i: ",i+1);
					printCard(0,i);
					printf("\n");
				}
				printf("\nWer spielt was?\n");
				for (i=0;i<4;i++)
				{
					player=(first+i)%4;
					if (player==0)
						game=playerChooseGame();
					else
						game=chooseGame(player);
					if(playedGame==0 || (playedGame < 4 && game >= 4) || (playedGame <6 && game >= 6) || (playedGame < 5 && game >= 5)) // check which game has priority
					{
						printf("\nSpieler %i: ",player+1);
						printGame(game);
						playedGame=game;
						playedBy=player;
						
					}
					else
					{
						printf("\nSpieler %i: ",player+1);
						printGame(0);
					}
				}
				findPartner();
			}
			
			
			if(playedGame==0) //if nobody calls a game, start new
				break;
			printf("\nDr%ccken Sie eine Taste, um Runde %i zu starten",129,turn+1);
			getch();
			system("cls"); //turn "log" on and off
			printTurn(turn);
//debug();
			printf("Gespielt wird ");
			printGame(playedGame);
			printf(" von Spieler %i",playedBy+1);
			printf("\n\nIhre Hand:\n\n");
			for (i=0;i<6;i++)
			{
				printf("%i: ",i+1);
				printCard(0,i);
				printf("\n");
			}
			printf("\n");
			for (i=0;i<4;i++) // 4 player loop
			{
				player=(first+i)%4;
				if (player==0)	
					play(player,playerChooseCard());
				else
					play(player, chooseCard(player));
			}
			
//this happens each turn when all players have played their cards
			
			winner=highCard();
			trick=pileScore();
			printf("\nAugen im Stich: %i\nSpieler %i bekommt den Stich\n",trick,winner+1);
			score[winner]+=trick;
			first=winner;
		}
		
// this happens after the game
		start++;
		
		if (playedGame<4 && playedGame>0)
		{
			if (score[playedBy]+score[partner]>60)
			{
				if (playedBy==0 || partner==0)
					textColor(green,black);
				else
					textColor(red,black);
				printf("\nSpieler %i und Spieler %i haben %sgewonnen (%i Augen)",playedBy+1,partner+1,score[playedBy]+score[partner]>90 ? (score[playedBy]+score[partner]==120?"schwarz ":"mit Schneider "):"",score[playedBy]+score[partner]);
				textColor(white,black);
			}
			else
			{
				if (playedBy==0 || partner==0)
					textColor(red,black);
				else
					textColor(green,black);
				printf("\nSpieler %i und Spieler %i haben %sverloren (%i Augen)",playedBy+1,partner+1,score[playedBy]+score[partner]>90 ? (score[playedBy]+score[partner]==120?"schwarz ":"mit Schneider "):"",score[playedBy]+score[partner]);
				textColor(white,black);
			}
		}
		else if(playedGame>=4 && tout==false)
		{
			if (score[playedBy]>60)
			{
				if (playedBy==0)
					textColor(green,black);
				else
					textColor(red,black);
				printf("\nSpieler %i hat %sgewonnen (%i Augen)\n",playedBy+1,score[playedBy]>90 ? (score[playedBy]==120?"schwarz ":"mit Schneider "):"",score[playedBy]);
				textColor(white,black);
			}
			else
			{
				if (playedBy==0)
					textColor(red,black);
				else
					textColor(green,black);
				printf("\nSpieler %i hat %sverloren (%i Augen)\n",playedBy+1,score[playedBy]>0 ? (score[playedBy]>30?"schneiderfrei ":"mit Schneider "):"schwarz ",score[playedBy]);
				textColor(white,black);
			}
		}
		else if(playedGame>=4 && tout==true)
		{
			if (score[playedBy]==120)
			{
				if (playedBy==0)
					textColor(green,black);
				else
					textColor(red,black);
				printf("\nSpieler %i hat gewonnen (%i Augen)\n",playedBy+1,score[playedBy]);
				textColor(white,black);
			}
			else
			{
				if (playedBy==0)
					textColor(red,black);
				else
					textColor(green,black);
				printf("\nSpieler %i hat verloren (%i Augen)\n",playedBy+1,score[playedBy]);
				textColor(white,black);
			}
		}
		else
		{
			printf("\nEs ist kein Spiel zustande gekommen.\n");
		}
		
		fflush(stdin);
		do
		{
			printf("\nM%cchten Sie noch ein mal spielen (J/N)?: ",148);
			cont=getchar();
		}while(toupper(cont)!='J' && toupper(cont)!='N');
	}while(toupper(cont)=='J');
	return 0;
}
