using Itmo.ObjectOrientedProgramming.Lab2.Entities;
using Itmo.ObjectOrientedProgramming.Lab2.Entities.Сomponents;
using Itmo.ObjectOrientedProgramming.Lab2.Entities.Сomponents.DataStorage;
using Itmo.ObjectOrientedProgramming.Lab2.Exceptions;

namespace Itmo.ObjectOrientedProgramming.Lab2.Services;

public interface IComputerBuilder
{
    IComputerBuilder Reset();

    IComputerBuilder SetCpu(Cpu cpu);

    IComputerBuilder SetMotherBoard(MotherBoard motherBoard);

    IComputerBuilder SetRamStick(RamStick ramStick);

    IComputerBuilder SetGraphicCard(GraphicCard graphicCard);

    IComputerBuilder SetDataStorage(DataStorageBase dataStorage);

    IComputerBuilder SetCpuCoolingSystem(CpuCoolingSystem cpuCoolingSystem);

    IComputerBuilder SetPowerSupply(PowerSupply powerSupply);

    IComputerBuilder SetComputerCase(ComputerCase computerCase);

    IComputerBuilder SetBios(Bios bios);

    IComputerBuilder SetWifiAdapter(WifiAdapter wifiAdapter);

    Result<Computer, string> Build();
}