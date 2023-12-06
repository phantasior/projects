using Itmo.ObjectOrientedProgramming.Lab2.Entities.Сomponents;
using Itmo.ObjectOrientedProgramming.Lab2.Entities.Сomponents.DataStorage;

namespace Itmo.ObjectOrientedProgramming.Lab2.Services;

public class ComponentsFactory
{
    private readonly IComponentFactory<Cpu> _cpuFactory;
    private readonly IComponentFactory<Bios> _biosFactory;
    private readonly IComponentFactory<ComputerCase> _computerCaseFactory;
    private readonly IComponentFactory<CpuCoolingSystem> _cpuCoolingSystemFactory;
    private readonly IComponentFactory<DataStorageBase> _dataStorageFactory;
    private readonly IComponentFactory<GraphicCard> _graphicCardFactory;
    private readonly IComponentFactory<MotherBoard> _motherBoardFactory;
    private readonly IComponentFactory<PowerSupply> _powerSupplyFactory;
    private readonly IComponentFactory<RamStick> _ramStickFactory;
    private readonly IComponentFactory<WifiAdapter> _wifiAdapterFactory;
    private readonly IComponentFactory<XmpProfile> _xmpProfileFactory;

    public ComponentsFactory(
        IComponentFactory<Cpu> cpuFactory,
        IComponentFactory<Bios> biosFactory,
        IComponentFactory<ComputerCase> computerCaseFactory,
        IComponentFactory<CpuCoolingSystem> cpuCoolingSystemFactory,
        IComponentFactory<DataStorageBase> dataStorageFactory,
        IComponentFactory<GraphicCard> graphicCardFactory,
        IComponentFactory<MotherBoard> motherBoardFactory,
        IComponentFactory<PowerSupply> powerSupplyFactory,
        IComponentFactory<RamStick> ramStickFactory,
        IComponentFactory<WifiAdapter> wifiAdapterFactory,
        IComponentFactory<XmpProfile> xmpProfileFactory)
    {
        _cpuFactory = cpuFactory;
        _biosFactory = biosFactory;
        _computerCaseFactory = computerCaseFactory;
        _cpuCoolingSystemFactory = cpuCoolingSystemFactory;
        _dataStorageFactory = dataStorageFactory;
        _graphicCardFactory = graphicCardFactory;
        _motherBoardFactory = motherBoardFactory;
        _powerSupplyFactory = powerSupplyFactory;
        _ramStickFactory = ramStickFactory;
        _wifiAdapterFactory = wifiAdapterFactory;
        _xmpProfileFactory = xmpProfileFactory;
    }

    public Cpu? GetCpuByName(string name) => _cpuFactory.GetByName(name);
    public Bios? GetBiosByName(string name) => _biosFactory.GetByName(name);
    public ComputerCase? GetComputerCaseByName(string name) => _computerCaseFactory.GetByName(name);
    public CpuCoolingSystem? GetCpuCoolingSystemByName(string name) => _cpuCoolingSystemFactory.GetByName(name);
    public DataStorageBase? GetDataStorageByName(string name) => _dataStorageFactory.GetByName(name);
    public GraphicCard? GetGraphicCardByName(string name) => _graphicCardFactory.GetByName(name);
    public MotherBoard? GetMotherBoardByName(string name) => _motherBoardFactory.GetByName(name);
    public PowerSupply? GetPowerSupplyByName(string name) => _powerSupplyFactory.GetByName(name);
    public RamStick? GetRamStickByName(string name) => _ramStickFactory.GetByName(name);
    public WifiAdapter? GetWifiAdapterByName(string name) => _wifiAdapterFactory.GetByName(name);
    public XmpProfile? GetXmpProfileByName(string name) => _xmpProfileFactory.GetByName(name);
}