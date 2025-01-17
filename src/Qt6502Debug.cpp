#include "Qt6502Debug.h"
#include "MainBus.hpp"

Qt6502Debug::Qt6502Debug(MainBus* bus, QWidget* parent) 
    : QMainWindow(parent), m_bus{ bus }
{
    ui.setupUi(this);
}

Qt6502Debug::~Qt6502Debug()
{
}

void Qt6502Debug::setRAM(uint16_t from, uint16_t to)
{
    curRamFrom = from;
    curRamTo = to;
    QString ramContent;
    std::string hexConvert = "0123456789ABCDEF";
    for (int i = from; i < to; i++)
    {
        uint8_t byte = m_bus->ram[i];
        ramContent.append(QChar(hexConvert[byte >> 4]));
        ramContent.append(QChar(hexConvert[byte & 0x0F]));
        ramContent.append(' ');
    }
    ui.ram_view->setText(ramContent);
}

void Qt6502Debug::setPrg(std::vector<std::pair<uint16_t, std::string>>& prg)
{
    lastPC = prg.front().first;
    int pos = 0;
    for (auto& instrAtAdr : prg)
    {
        auto item = new QListWidgetItem(QString::fromStdString(instrAtAdr.second));
        m_prg[instrAtAdr.first] = item;
        ui.prg_view->insertItem(pos++, item);
    }
}

void Qt6502Debug::keyPressEvent(QKeyEvent* event)
{
    switch (event->key()) 
    {
    case Qt::Key_Space:
        emit spacePressed();
        break; 
    default:
        QMainWindow::keyPressEvent(event);
        break;
    }
}

void Qt6502Debug::cpuChanged(uint16_t pc, uint16_t stkp, uint8_t a, uint8_t x, uint8_t y)
{
    auto lastItem = m_prg[lastPC];
    lastItem->setBackground(QBrush(QColor(86, 86, 86)));
    auto nextItem = m_prg[pc];
    if (nextItem == nullptr)
    {
        auto p = disassemble(pc);
        setPrg(p);
        nextItem = m_prg[pc];
    }
    nextItem->setBackground(QBrush(QColor(255, 255, 255)));
    lastPC = pc;

    setRAM(curRamFrom, curRamTo);
    ui.label_reg_A->setText(QString::fromStdString("A: " + std::to_string(a)));
    ui.label_reg_X->setText(QString::fromStdString("X: " + std::to_string(x)));
    ui.label_reg_Y->setText(QString::fromStdString("Y: " + std::to_string(y)));
    ui.label_reg_stkp->setText(QString::fromStdString("SP: " + std::to_string(stkp)));
}

std::vector<std::pair<uint16_t, std::string>> Qt6502Debug::disassemble(uint16_t address)
{
    std::vector<std::pair<uint16_t, std::string>> result;
    for (int i = 0; i < 20; i++)
    {
        std::string tmp;
        uint8_t opcode = m_bus->read(address);
        tmp = m_bus->cpu6502.instructions[opcode].name;
        (m_bus->cpu6502.*(m_bus->cpu6502.instructions[opcode].addrmode))();
        tmp += " " + m_bus->cpu6502.disassembled;
        result.push_back(std::pair(address, tmp));
        address++;
    }
    return result;
}

