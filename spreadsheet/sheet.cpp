#include "sheet.h"

#include "cell.h"
#include "common.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>
#include <variant>

using namespace std::literals;

Sheet::~Sheet()
{}

void Sheet::SetCell(Position pos, std::string text)
{
    if (!pos.IsValid())
    {
        throw InvalidPositionException("Invalid Position");
    }

    if (pos.row >= static_cast<int>(sheet_.size()))
    {
        print_size_.rows = pos.row + 1;
        size_t old_size = sheet_.size();
        sheet_.resize(print_size_.rows);

        for (size_t row = old_size - 1; row < sheet_.size(); ++row)
        {
            sheet_[row].resize(print_size_.cols);
        }
    }

    if (sheet_.size() && pos.col >= static_cast<int>(sheet_[0].size()))
    {
        print_size_.cols = pos.col + 1;
        for (size_t row = 0; row < sheet_.size(); ++row)
        {
            sheet_[row].resize(print_size_.cols);
        }
    }

    auto& cell = sheet_[pos.row][pos.col];
    if (!cell)
    {
        cell = std::make_unique<Cell>(*this);
    }
    cell->Set(std::move(text));//*/

    /*if (sheet_[pos.row][pos.col] == nullptr)
    {
        std::unique_ptr<Cell> cell = std::make_unique<Cell>(*this);
        cell->Set(text);
        sheet_[pos.row][pos.col] = std::move(cell);
    }
    else
    {
        (*sheet_[pos.row][pos.col]).Set(text);
    }//*/
}

const CellInterface* Sheet::GetCell(Position pos) const
{
    if (!pos.IsValid())
    {
        throw InvalidPositionException("Invalid Position");
    }

    if (pos.row < static_cast<int>(sheet_.size()) && sheet_.size() && pos.col < static_cast<int>(sheet_[0].size()))
    {
        CellInterface* ptr = &(*sheet_[pos.row][pos.col]);
        return ptr;
    }
    return nullptr;
}

CellInterface* Sheet::GetCell(Position pos)
{
    if (!pos.IsValid())
    {
        throw InvalidPositionException("Invalid Position");
    }

    if (pos.row < static_cast<int>(sheet_.size()) && sheet_.size() && pos.col < static_cast<int>(sheet_[0].size()))
    {
        CellInterface* ptr = &(*sheet_[pos.row][pos.col]);
        return ptr;
    }
    return nullptr;//*/
}

void Sheet::ClearCell(Position pos)
{
    if (!pos.IsValid())
    {
        throw InvalidPositionException("Invalid Position");
    }

    if (pos.row < static_cast<int>(sheet_.size()) && sheet_.size() && pos.col < static_cast<int>(sheet_[0].size()))
    {
        sheet_[pos.row][pos.col] = nullptr;
    }

    PrintResize();
}

Size Sheet::GetPrintableSize() const
{
    return print_size_;
}

void Sheet::PrintValues(std::ostream& output) const
{
    for (int i = 0; i < print_size_.rows; ++i)
    {
        bool first_cell = true;
        for (int j = 0; j < print_size_.cols; ++j)
        {
            if (first_cell)
            {
                if (sheet_[i][j])
                {
                    if (std::holds_alternative<double>((*(sheet_[i][j])).GetValue()))
                    {
                        double d = std::get<double>((*(sheet_[i][j])).GetValue());
                        output << d;
                    }
                    else if (std::holds_alternative<std::string>((*(sheet_[i][j])).GetValue()))
                    {
                        std::string s = std::get<std::string>((*(sheet_[i][j])).GetValue());
                        output << s;
                    }
                    else
                    {
                        FormulaError err = std::get<FormulaError>((*(sheet_[i][j])).GetValue());
                        output << err;
                    }
                }
                else
                {
                    output << "";
                }
                first_cell = false;
            }
            else
            {
                if (sheet_[i][j])
                {
                    if (std::holds_alternative<double>((*(sheet_[i][j])).GetValue()))
                    {
                        double d = std::get<double>((*(sheet_[i][j])).GetValue());
                        output << "\t" << d;
                    }
                    else if (std::holds_alternative<std::string>((*(sheet_[i][j])).GetValue()))
                    {
                        std::string s = std::get<std::string>((*(sheet_[i][j])).GetValue());
                        output << "\t" << s;
                    }
                    else
                    {
                        FormulaError err = std::get<FormulaError>((*(sheet_[i][j])).GetValue());
                        output << "\t" << err;
                    }
                }
                else
                {
                    output << "\t" << "";
                }
            }
        }
        output << "\n";
    }
}

void Sheet::PrintTexts(std::ostream& output) const
{
    for (int i = 0; i < print_size_.rows; ++i)
    {
        bool first_cell = true;
        for (int j = 0; j < print_size_.cols; ++j)
        {
            if (first_cell)
            {
                if (sheet_[i][j])
                {
                    output << (*sheet_[i][j]).GetText();
                }
                else
                {
                    output << "";
                }
                first_cell = false;
            }
            else
            {
                if (sheet_[i][j])
                {
                    output << "\t" << (*sheet_[i][j]).GetText();
                }
                else
                {
                    output << "\t" << "";
                }
            }
        }
        output << "\n";
    }
}

Cell* Sheet::GetThisCell(Position pos) {
    if (!pos.IsValid())
    {
        throw InvalidPositionException("Invalid Position");
    }

    if (pos.row < static_cast<int>(sheet_.size()) && sheet_.size() && pos.col < static_cast<int>(sheet_[0].size()))
    {
        return sheet_[pos.row][pos.col].get();
    }
    return nullptr;//*/
}

void Sheet::PrintResize()
{
    size_t col_max_pos = 0, row_max_pos = 0;
    for (size_t row = 0; row < sheet_.size(); ++row)
    {
        for (size_t col = 0; col < sheet_[row].size(); ++col)
        {
            if (sheet_[row][col] != nullptr)
            {
                col_max_pos = col + 1;
                row_max_pos = row + 1;
            }
        }
    }

    print_size_.rows = row_max_pos;
    print_size_.cols = col_max_pos;
}

std::unique_ptr<SheetInterface> CreateSheet() {
    return std::make_unique<Sheet>();
}