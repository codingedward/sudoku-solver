#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

typedef unsigned char uchar;

class Sudoku {
public:
    Sudoku() 
    {
        for (int i = 0; i < Size; ++i) {
            m_board.push_back(std::vector<uchar>(Size, Empty));
        }
    }

    void play()
    {
        printWelcomeBanner();
        printSudoku();
        printInstructions();

        int i;
        int j;
        int value;
        std::string input;
        while (true) {
            printPrompt();
            std::cin >> input;
            std::transform(input.begin(), input.end(), input.begin(), ::toupper);
            if (input == "X")
                return;
            else if (input == "S") {
                solve();
                return;
            }

            i = std::atoi(input.c_str());
            std::cin >> j >> value;
            if (canPlaceValueInCell(i, j, value)) {
                setValue(i, j, value);
            } else {
                printInvalidInput();
                clearInvalidInput();
            }
            clearScreen();
            printSudoku();
        }
    }

private:
    bool solve()
    {
        if (isSolved()) {
            clearScreen();
            printSolutionBanner();
            printSudoku();
            return true;
        } 

        for (uchar i = 0; i < Size; ++i) {
            for (uchar j = 0; j < Size; ++j) {
                if (getValue(i, j) == Empty) {
                    for (uchar value = Min; value <= Max; ++value) {
                        if (canPlaceValueInCell(i, j, value)) {
                            setValue(i, j, value);
                            if (solve())
                                return true;
                            setValue(i, j, Empty);
                        }
                    }
                    if (getValue(i, j) == Empty)
                        return false;
                }
            }
        }
        return false;
    }

    bool isSolved() const 
    {
        for (int i = 0; i < Size; ++i)
            for (int j = 0; j < Size; ++j)
                if (getValue(i, j) == Empty || !cellIsUnique(i, j))
                    return false;
        return true;
    }

    bool isValid() const 
    {
        for (int i = 0; i < Size; ++i)
            for (int j = 0; j < Size; ++j)
                if (!cellIsUnique(i, j))
                    return false;
        return true;
    }

    bool canPlaceValueInCell(int row, int column, uchar value)
    {
        int currentValue = getValue(row, column);
        setValue(row, column, value);
        bool isPlaceable = cellIsUnique(row, column);
        setValue(row, column, currentValue);
        return isPlaceable;
    }

    bool cellIsUnique(int row, int column) const
    {
        return cellIsUniqueInRow(row, column)
            && cellIsUniqueInColumn(row, column)
            && cellIsUniqueInCellGroup(row, column);
    }

    bool cellIsUniqueInRow(int row, int column) const
    {
        uchar cellValue = getValue(row, column);
        for (int i = 0; i < Size; ++i)
            if (i != column && getValue(row, i) == cellValue) 
                return false;
        return true;
    }

    bool cellIsUniqueInColumn(int row, int column) const
    {
        uchar cellValue = getValue(row, column);
        for (int i = 0; i < Size; ++i)
            if (i != row && getValue(i, column) == cellValue)
                return false;
        return true;
    }

    bool cellIsUniqueInCellGroup(int row, int column) const
    {
        int groupRow = getCellGroupIndex(row);
        int groupColumn = getCellGroupIndex(column);

        int cellValue = getValue(row, column);
        for (int i = groupRow; i < groupRow + CellGroupSize; ++i)
            for (int j = groupColumn; j < groupColumn + CellGroupSize; ++j)
                if ((i != row && j != column) && getValue(i, j) == cellValue)
                    return false;

        return true;
    }

    inline void setValue(int row, int column, uchar value)
    {
        m_board[row][column] = value;
    }

    inline uchar getValue(int row, int column) const
    {
        return m_board[row][column];
    }

    int getCellGroupIndex(int index) const
    {
        int groupIndex = 0;
        while (groupIndex + CellGroupSize <= index)
            groupIndex += CellGroupSize;
        return groupIndex;
    }

    inline void clear() 
    {
        for (auto & column : m_board)
            for (auto & cell : column)
                cell = Empty;
    }

    void printSudoku() const
    {
        std::cout << "\n\n";
        std::cout << "   || 0 | 1 | 2 || 3 | 4 | 5 || 6 | 7 | 8 ||\n";
        for (int i = 0; i < Size; ++i) {
            if (i % 3 == 0)
                std::cout << "============================================\n";
            else 
                std::cout << "--------------------------------------------\n";

            std::cout << " " << i << " ||";
            for (int j = 0; j < Size; ++j) {
                int value = static_cast<int>(getValue(i, j));
                if (value == 0) {
                    std::cout << "   |";
                } else {
                    std::cout << " " << value << " |";
                }
                if ((j + 1) % 3 == 0)
                    std::cout << "|";
            }
            std::cout << "\n";
        }
        std::cout << "============================================\n";
    }

    inline void printWelcomeBanner() 
    {
        std::cout << "********************************************\n"
            << "*************** SUDOKU SOLVER **************\n"
            << "********************************************";
    }
    
    inline void printSolutionBanner()
    {
        std::cout << "############################################\n"
            << "################# SOLUTION #################\n"
            << "############################################\n";        
    }

    inline void printInstructions()
    {
        std::cout << "\nINSTRUCTIONS\n"
            << "============\n";
        std::cout << "Enter the values in ith row and jth column as: \n"
            << "(i j value) without brackets and press Enter.\n";
    }
    
    inline void printPrompt()
    {
        std::cout << "\nTo solve, enter S, to quit enter X."
            << "\n(i j value)> ";
    }
    
    inline void printInvalidInput()
    {
        std::cout << "\nCannot enter that value!\n";
    }
    
    void clearInvalidInput()
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<int>::max(), '\n');        
    }

    void clearScreen()
    {
        std::system("clear");
    }

    const uchar Min = 1;
    const uchar Max = 9;
    const uchar Empty = 0;

    const int Size = 9;
    const int CellGroupSize = 3;
    std::vector<std::vector<uchar>> m_board;
};

int main()
{
    Sudoku sudoku;
    sudoku.play();
}
