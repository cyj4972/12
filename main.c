#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "board.h"

#define MAX_CHARNAME	200
#define N_PLAYER		3
#define MAX_DIE			6

#define PLAYERSTATUS_LIVE		0
#define PLAYERSTATUS_DIE		1
#define PLAYERSTATUS_END		2

int player_position[N_PLAYER];
char player_name[N_PLAYER][MAX_CHARNAME];
int player_coin[N_PLAYER];
int player_status[N_PLAYER];		//LIVE: 0, DIE: 1, END: 2
char player_statusString[3][MAX_CHARNAME] = {"LIVE", "DIE", "END"};

void opening(void)
{
	printf("============\n");
	printf("SHARK GAME\n");
	printf("============\n");
}

int rolldie(void)
{
	return rand()%MAX_DIE+1;
}

void printPlayerPosition(int player)
{
	int i;
	for(i=0;i<N_BOARD;i++)
	{
		printf("|");
		if(i != player_position[player])
		{
			if(board_getBoardStatus(i) == BOARDSTATUS_NOK)
				printf("X");
			else
				printf(" ");
		}
			
		else
			printf("%c", player_name[player][0]);
	}
	printf("|\n");
}

void printPlayerStatus(void)
{
	int i;
	printf("player status ---\n");
	for(i=0;i<N_PLAYER;i++)
	{
		printf("%s : pos %i, coin %i, status : %s\n", player_name[i], player_position[i], player_coin[i], player_statusString[player_status[i]]);
	}
	printf("----------------------------\n");
}

void checkDie(void)
{
	int i;
	for(i=0;i<N_PLAYER;i++)
		if(board_getBoardStatus(player_position[i]) == BOARDSTATUS_NOK)
			{
				player_status[i] = PLAYERSTATUS_DIE;
				printf("%s in pos %d has died\n", player_name[i], player_position[i]);
			}
			
}

int main(int argc, char *argv[]) 
{
	int pos = 0;
	int i;
	int turn = 0;
	int flag_end;
	srand((unsigned)time(NULL));

	//0. opening
	opening();
	
	//1. �ʱ�ȭ & �÷��̾� �̸� �Է� 
	board_initBoard();
	 
	//1-2. �÷��̾� ���� �ʱ�ȭ
	for(i=0;i<N_PLAYER;i++)
	{
		player_position[i] = 0;
		player_coin[i] = 0;
		player_status[i] = PLAYERSTATUS_LIVE;
		printf("Player %i's name: ", i);
		scanf("%s", player_name[i]);
	} 
	
	//2. �ݺ���(�÷��̾� ��)
	do{
		int step;
		int coinResult;
		char c;
		
		if(player_status[turn] != PLAYERSTATUS_LIVE)
		{
			turn = (turn + 1) % N_PLAYER;
			continue;
		}
		
		//2-1. ����, �÷��̾� ���� ���
		board_printBoardStatus();
		for(i=0;i<N_PLAYER;i++)
			printPlayerPosition(i);
		printPlayerStatus();
		
		
		//2-2. �ֻ��� ������
		printf("%s turn!", player_name[turn]);
		printf("press any key to roll a die\n");
		scanf("%d", &c);		//�ǹ�x, ���ߴ� �뵵 
		fflush(stdin);			//���� ��� scanf ����, �׻� �Է��ϱ� ���� 
		step = rolldie();
		
		//2-3. �̵�
		player_position[turn] += step;
		printf("die result: %d, %s moved to %d\n", step, player_name[turn], player_position[turn]);
		
		if(player_position[turn] >= N_BOARD)
			player_position[turn] = N_BOARD - 1;
		
		if(player_position[turn] == N_BOARD - 1)
		{
			player_status[turn] =  PLAYERSTATUS_END;
			printf("%s reached to the end!!(coin : %d)\n", player_name[turn], player_coin[turn]);
		}
		
		//2-4. ���� �ݱ� 
		coinResult = 0;
		coinResult = board_getBoardCoin(player_position[turn]);
		if(coinResult > 0)
			printf("	-> Lucky! %s got %d coins\n", player_name[turn], coinResult);
		player_coin[turn] += coinResult;
		 
		//2-5. ���� �� 
		turn = (turn + 1) % N_PLAYER;		//wrap around
		//2-6. if(����: ��� �÷��̾ ���� ����)
		if(turn == 0)
		{
			//��� ����
			int shark_pos = board_stepShark();
			if(shark_pos>=0)
			{
				printf("shark moved to %d\n", shark_pos);
				checkDie();
			}	 
		}
		//2-7. ���� ���� ���� 
		flag_end = 0;
		if(player_status[0] != PLAYERSTATUS_LIVE && player_status[1] != PLAYERSTATUS_LIVE && player_status[2] != PLAYERSTATUS_LIVE)
			flag_end = 1;
		
	} while(flag_end == 0);
		//3. ����(���� ���, ��� ��)

	printf("GAME END!\n");
	
	int cnt=0;
	for(i=0;i<N_PLAYER;i++)
	{
		if(player_status[i] == PLAYERSTATUS_END)
		cnt++;
	}
	printf("%d players are alive!\n", cnt);
	
	int winner=0;
	int max_coin=-1;
	
	for(i=0;i<N_PLAYER;i++)
	{
		if(player_status[i] == PLAYERSTATUS_END)
		{
			if(player_coin[i] > max_coin)
			{
				max_coin = player_coin[i];
				winner = i;
				printf("winner is %s\n", player_name[winner]);
			}
		}
		else if(player_status[0] == PLAYERSTATUS_DIE && player_status[1] == PLAYERSTATUS_DIE && player_status[2] == PLAYERSTATUS_DIE)
			printf("shark wins");	
	}

	system("PAUSE");
	return 0;
}
