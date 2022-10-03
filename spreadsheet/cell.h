#pragma once

#include "common.h"
#include "formula.h"

#include <unordered_set>

class Sheet;

class Cell : public CellInterface {
public:
    Cell(Sheet& sheet);
    ~Cell();

    void Set(std::string);
    void Clear();

    Value GetValue() const override;
    std::string GetText() const override;

    std::vector<Position> GetReferencedCells() const override;
    bool IsReferenced() const;


private:
    class Impl {
    public:
        virtual ~Impl() = default;
        virtual Value GetValue() const = 0;
        virtual std::string GetText() const = 0;
        virtual std::vector<Position> GetReferencedCells() const { return {}; }
    };

    class EmptyImpl : public Impl {
    public:
        EmptyImpl();
        Value GetValue() const override;
        std::string GetText() const override;
    };

    class TextImpl : public Impl {
    public:
        TextImpl(std::string text);
        Value GetValue() const override;
        std::string GetText() const override;
    private:
        std::string text_;
    };

    class FormulaImpl : public Impl {
    public:
        FormulaImpl(std::string text, Sheet& sheet);
        Value GetValue() const override;
        std::string GetText() const override;
        std::vector<Position> GetReferencedCells() const override;

    private:
        std::unique_ptr<FormulaInterface> formula_;
        Sheet& sheet_;
        Value val_;
    };//*/

private:
    bool HasCircularReferences(Impl& impl);
    void UpdateReferences();

private:
    std::unique_ptr<Impl> impl_;
    Sheet& sheet_;
    std::unordered_set<Cell*> prev_cells_;
    std::unordered_set<Cell*> next_cells_;
};