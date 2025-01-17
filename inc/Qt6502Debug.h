#pragma once

#include <QtWidgets/QMainWindow>
#include <QLabel>
#include <QKeyEvent>

#include "ui_MainWindow.h"

class MainBus;

class Qt6502Debug  : public QMainWindow
{
    Q_OBJECT

public:
    Qt6502Debug(MainBus* bus, QWidget* parent = nullptr);
    ~Qt6502Debug();

    void setRAM(uint16_t from, uint16_t to);
    void setPrg(std::vector<std::pair<uint16_t, std::string>>& prg);

    std::vector<std::pair<uint16_t, std::string>> disassemble(uint16_t address);

protected:
    void keyPressEvent(QKeyEvent* event) override;

public slots:
    void cpuChanged(uint16_t pc, uint16_t stkp, uint8_t a, uint8_t x, uint8_t y);

signals:
    void spacePressed();

private:
    Ui::MainWindow ui;
    std::map<uint16_t, QListWidgetItem*> m_prg;
    uint16_t lastPC;
    uint16_t curRamFrom;
    uint16_t curRamTo;
    MainBus* m_bus = nullptr;
};