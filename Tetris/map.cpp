#include "map.h"
#include "base.h"

Map::Map()
{
	// Make borders True
	for (int i = 0; i < 22; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			if (i == 0 || i == 21)
				arrange[i][j] = true;
			else if (j == 0 || j == 11)
				arrange[i][j] = true;
			else
				arrange[i][j] = false;
		}
	}
	for (int i = 0; i < 20; i++)
		line_status[i] = false;
}


Map::~Map()
{
	//empty
}


void Map::InitialDraw()
{
	int x = 10, y = 5;
	gotoxy(x, y);
	for (int i = 0; i < 12; i++)
		std::cout << "■";
	for (int i = 0; i < 21; i++)
	{
		gotoxy(x, ++y);
		std::cout << "■";
		gotoxy(x + 22, y);
		std::cout << "■";
	}
	gotoxy(x + 2, y);

	for (int i = 0; i < 11; i++)
		std::cout << "■";
}


void Map::MoveByArrow(ExtendedBlock& cur_block, int input_arrow)
{
	switch (input_arrow)
	{
	case Arrow::UP:
		try
		{
			cur_block.Rotate();
			IsMovable(cur_block);

		}
		catch (MovementException & expn)
		{
			for (int i = 0; i < 3; i++)
				cur_block.Rotate();
		}
		break;
	case Arrow::LEFT:
		try
		{
			cur_block.Set_xpos(cur_block.Get_xpos() - 1);
			IsMovable(cur_block);
		}
		catch (MovementException & expn)
		{
			cur_block.Set_xpos(cur_block.Get_xpos() + 1);
		}
		break;
	case Arrow::RIGHT:
		try
		{
			cur_block.Set_xpos(cur_block.Get_xpos() + 1);
			IsMovable(cur_block);
		}
		catch (MovementException & expn)
		{
			cur_block.Set_xpos(cur_block.Get_xpos() - 1);
		}
		break;
		// case Arrow::DOWN 필요
	case Arrow::DOWN:
		PullToBottom(cur_block);
		break;
	}
	cur_block.SetCellPosByCenter();
}

void Map::DrawBlock(ExtendedBlock& cur_block)
{
	//cur_block에 있는 4x4배열의 상대적 위치를 arrange 위치로 변환하여 arrange에 적용
	int vertical, horizontal, abs_vertical, abs_horizontal;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cur_block.GetBlockColor());
	for(int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			if ((*(cur_block.get_cell() + (i * 4) + j)).Is_active())
			{
				GetArrangePosition(cur_block, i, j, vertical, horizontal) = true;
				abs_vertical = 5 + vertical;
				abs_horizontal = 10 + horizontal * 2;
				gotoxy(abs_horizontal, abs_vertical);
				std::cout << "■";
			}
		}
	/*
	gotoxy(0, 0);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if ((*(cur_block.get_cell() + i * 4 + j)).Is_active())
			{
				std::cout << "xpos : " << (*(cur_block.get_cell() + i * 4 + j)).get_xpos();
				std::cout << "| ypos : " << (*(cur_block.get_cell() + i * 4 + j)).get_ypos() <<std::endl;
			}
		}
	}
	*/
}


void Map::PullDownBlock(ExtendedBlock& cur_block) throw (MovementException)
{
	try
	{
		cur_block.Set_ypos(cur_block.Get_ypos() + 1);
		IsMovable(cur_block);
	}
	catch (MovementException& expn)
	{
		cur_block.Set_ypos(cur_block.Get_ypos() - 1);
		throw;
	}
	cur_block.SetCellPosByCenter();
	//DrawBlock(cur_block);
}


void Map::PullToBottom(ExtendedBlock& cur_block)
{
	try
	{
		while (IsMovable(cur_block))
		{
			cur_block.Set_ypos(cur_block.Get_ypos() + 1);
		}
	}
	catch (MovementException & expn) 
	{
		cur_block.Set_ypos(cur_block.Get_ypos() - 1);
	}
	cur_block.SetCellPosByCenter();
	//DrawBlock(cur_block);
}


void Map::DeleteBlock(ExtendedBlock& cur_block)
{
	int vertical, horizontal, abs_vertical, abs_horizontal;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			if ((*(cur_block.get_cell() + (i * 4) + j)).Is_active())
			{
				GetArrangePosition(cur_block, i, j, vertical, horizontal) = false;
				abs_vertical = 5 + vertical;
				abs_horizontal = 10 + horizontal * 2;
				gotoxy(abs_horizontal, abs_vertical);
				std::cout << "  ";
			}
		}
}


bool Map::IsMovable(ExtendedBlock& cur_block) throw (MovementException)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			if ((*(cur_block.get_cell() + (i * 4) + j)).Is_active())
			{
				if (GetArrangePosition(cur_block, i, j))
					throw MovementException();
			}
		}
	return true;
}


bool& Map::GetArrangePosition(ExtendedBlock& cur_block, int i, int j)
{
	int vertical = cur_block.Get_ypos() + i - cur_block.Get_center_ypos();
	int horizontal = cur_block.Get_xpos() + j - cur_block.Get_center_xpos();
	return arrange[vertical][horizontal];
}


bool& Map::GetArrangePosition(ExtendedBlock& cur_block, int i, int j, int& vertical, int& horizontal)
{
	vertical = cur_block.Get_ypos() + i - cur_block.Get_center_ypos();
	horizontal = cur_block.Get_xpos() + j - cur_block.Get_center_xpos();
	return arrange[vertical][horizontal];
}


// 블록 착지 후 꽉 찬 가로줄이 있는지 검사합니다.
void Map::CheckLine()
{

	for (int i = 20; i > 0; i--)
		for (int j = 1; j < 11; j++)
		{
			if (!arrange[i][j])
				break;
			if (j == 10)
			{
				DeleteLine(i);
				ArrangeCells(i);
			}

		}
	//test
	
}


bool Map::IsLanded(ExtendedBlock& cur_block)
{
	static int num = 1;
	for(int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			if ((*(cur_block.get_cell() + (i * 4) + j)).Is_active())
			{
				if(i == 3 || !((*(cur_block.get_cell() + ((i+1) * 4) + j)).Is_active()))
					if (GetArrangePosition(cur_block, i + 1, j))
					{
						//test
						if (num == 1)
						{
							gotoxy(0, 0);
							for (int i = 0; i < 4; i++)
							{
								for (int j = 0; j < 4; j++)
								{
									if ((*(cur_block.get_cell() + i * 4 + j)).Is_active())
									{
										std::cout << "xpos : " << (*(cur_block.get_cell() + i * 4 + j)).get_xpos();
										std::cout << "| ypos : " << (*(cur_block.get_cell() + i * 4 + j)).get_ypos() << std::endl;
										//cell의 xpos, ypos가 arrange의 가로,세로와 호환이 제대로 되는지 확인하기
										std::cout << "|| arrange"
									}
								}
							}
							num++;
						}
						//endtest
					
						return true;
					}
			}
		}
	return false;
}


void Map::PushBlockList(ExtendedBlock& cur_block)
{
	block_list.push_back(cur_block);
}


bool Map::IsLost()
{
	for (int i = 1; i < 11; i++)
		if (arrange[1][i])
			return true;
	return false;
}

void Map::DeleteLine(int line)
{
	int vertical, horizontal;
	int num = 1;
	Cell tmp_cell;

	for (list<ExtendedBlock>::iterator itr = block_list.begin(); itr != block_list.end(); itr++)
	{
		for(int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
			{
				tmp_cell = *((*itr).get_cell() + (j * 4) + k);
				if (tmp_cell.Is_active())
				{
					
					//bool& position_in_arrange = GetArrangePosition(*itr, j, k, vertical, horizontal);
					if (tmp_cell.get_ypos() == line)
					{
						(*((*itr).get_cell() + j * 4 + k)).set_active(false);
						//position_in_space = false;
						arrange[tmp_cell.get_ypos()][tmp_cell.get_xpos()] = false;
						int abs_vertical = 5 + tmp_cell.get_ypos();
						int abs_horizontal = 10 + tmp_cell.get_xpos() * 2;
						gotoxy(abs_horizontal, abs_vertical);
						std::cout << "  ";
					}
				}
			}
	}
	
}


void Map::ArrangeCells(int line)
{
	int vertical, horizontal;
	Cell tmp_cell;
	for(line--; line > 0; line--)
		for (list<ExtendedBlock>::iterator itr = block_list.begin(); itr != block_list.end(); itr++)
		{
			for (int j = 0; j < 4; j++)
				for (int k = 0; k < 4; k++)
				{
					tmp_cell = *((*itr).get_cell() + j * 4 + k);
					if (tmp_cell.Is_active())
					{
						//bool& position_in_arrange = GetArrangePosition(*itr, j, k, vertical, horizontal);
						if (tmp_cell.get_ypos() == line)
						{
							//블록에 연연하지 말고 arrange 자체에서 지우고 그려야 하나
							EraseCell(tmp_cell);
							//PullCellsToBottom(tmp_cell);
							//DrawCell(tmp_cell);
						}
					}				
				}
				
		}
}


void Map::EraseCell(Cell& cur_cell)
{
	arrange[cur_cell.get_ypos()][cur_cell.get_xpos()] = false;
	gotoxy(cur_cell.get_xpos(), cur_cell.get_ypos());
	std::cout << "  ";
}


void Map::DrawCell(Cell& cur_cell)
{
	arrange[cur_cell.get_ypos()][cur_cell.get_xpos()] = true;
	gotoxy(cur_cell.get_ypos(), cur_cell.get_xpos());
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cur_cell.get_color());
	std::cout << "■";
	gotoxy(0, 0);
}


void Map::PullCellsToBottom(Cell& cur_cell)
{

	while (!arrange[cur_cell.get_ypos() + 1][cur_cell.get_xpos()])
	{
		cur_cell.set_ypos(cur_cell.get_ypos() + 1);
	}
	gotoxy(50, 5);
	std::cout << "xpos : " << cur_cell.get_xpos();
	std::cout << "| ypos : " << cur_cell.get_ypos() << std::endl;
}

/*
void Map::PullToBottomCells(ExtendedBlock& cur_block)
{
	int vertical, horizontal;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if ((*(cur_block.get_cell() + i * 4 + j)).Is_active())
			{
				bool& position_in_arrange = GetArrangePosition(cur_block, i, j, vertical, horizontal);

			}
		}
	}
}
*/